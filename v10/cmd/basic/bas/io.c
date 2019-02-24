#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"
#include "tokens.h"

#define MAXUNIT 	5
#define MAXSTR		255
#define MAXBUF		7
#define INUNIT		0
#define OUTUNIT 	1

static struct iotab {
	int	io_flag;	/* if currently open etc. */
	int	io_col; 	/* current column */
	FILE	*io_ptr;	/* input/output stdio table entry */
	} units[MAXUNIT];

static char	buffers[MAXBUF][BUFSIZ];
static char	bufflg[MAXBUF];
static char	pflgs[_NFILE];

char	askdelims[] = { COMMA, ';', 0 };

char	*getsvar(), *strpop();
FILE	*xopen(), *fopen(), *popen();
double	popfloat();
struct iotab	*getunit();


/*
 * xopen --- open a file or a pipe; assign a buffer
 */

FILE *xopen(file, how)
char	*file, *how;
{
	register FILE	*f;

	if (*file == '!') {	/* open a pipe */
		if (f = popen(&file[1], how))
			pflgs[fileno(f)]++;
		}
	else
		f = fopen(file, how);
	if (f != (FILE *)NULL)
		assbuf(f);
	return(f);
}


/*
 * xclose --- close a file previously opened by xopen
 */

xclose(file)
register FILE	*file;
{
	register char	*b = file->_base;
	register int	i;

	i = fileno(file);
	if (pflgs[i])
		pclose(file);
	else
		fclose(file);
	pflgs[i] = 0;
	for (i = 0; i < MAXBUF; ++i)
		if (b == buffers[i])
			bufflg[i] = 0;
}


/*
 * assbuf --- assign a buffer to a newly opened file
 */

assbuf(f)
register FILE	*f;
{
	register int	i;

	for (i = 0; i < MAXBUF; ++i)
		if (bufflg[i] == 0) {
			setbuf(f, buffers[i]);
			++bufflg[i];
			return;
			}
	err("no buffers available");
}


/*
 * initio --- initialize i/o related structures
 */

initio()
{

	assbuf(stdin);
	clrio();
}


/*
 * clrio --- reinitialize i/o related structures
 */

clrio()
{
	register struct iotab	*i;

	for (i = units; i < &units[MAXUNIT]; ++i)
		if (i->io_flag)
			iocls(i);

	units[INUNIT].io_flag = INPUT;
	units[INUNIT].io_ptr = stdin;
	units[INUNIT].io_col = 1;
	units[OUTUNIT].io_flag = OUTPUT;
	units[OUTUNIT].io_ptr = stdout;
	units[OUTUNIT].io_col = 1;
}

iocls(i)
register struct iotab	*i;
{
	if (i->io_flag) {
		if (i->io_ptr != stdin && i->io_ptr != stdout)
			xclose(i->io_ptr);
		i->io_flag = 0;
		i->io_ptr = (FILE *)NULL;
		i->io_col = 1;
		}
}


/*
 * getunit --- get and check unit number for OPEN statement
 */

struct iotab *getunit(flag, defunit)
{
	register int		n;
	register struct iotab	*i;

	if (*inptr != SHARP)
		n = defunit;
	else {
		++inptr;
		n = fexpr();
		if (!endtest()) {
			while (*inptr == ' ' || *inptr == TAB)
				++inptr;
			if (*inptr != THEN)
				expectc(COMMA);
			}
		}
	if (n < 0 || n >= MAXUNIT)
		err("invalid unit %d", n);
	i = &units[n];
	if (flag && flag != i->io_flag)
		err("unit not opened for %s",
			((flag == INPUT)? "input" : "output"));
	return(i);
}


/*
 * openstmt --- interpret an OPEN statement
 */

openstmt()
{
	register struct iotab	*i;
	int			len, how;
	char			*str, *code, *file;

	expr();
	file = strpop();
	expectc(FOR);
	how = *inptr;
	switch (how) {
	case INPUT:
		code = "r";
		break;
	case OUTPUT:
		code = "w";
		break;
	case APPEND:
		code = "a";
		how = OUTPUT;		/* same as OUTPUT mostly */
		break;
	default:
		badsyn();
		}
	++inptr;
	i = getunit(0, ((how == INPUT)? INUNIT : OUTUNIT));
	i->io_ptr = xopen(file, code);
	if (i->io_ptr == (FILE *)NULL)
		err("cannot open %s", file);
	i->io_flag = how;
	i->io_col = 1;
}


/*
 * prtusing --- interpret a PRINT USING statement 
 */

prtusing(fmtptr)
char	*fmtptr;
{

	register struct iotab	*i;


	/*	linefeed and carriage return and
	 *	set column count to 1
	 */

	while (*inptr == ' ' || *inptr == COMMA || *inptr == TAB)
		inptr++;	/* skip spaces & first comma or tab */

	i = getunit(OUTPUT, OUTUNIT);
	col = i->io_col;
	if (col > 1) {
		i->io_col = 1;
		}

	getformat(fmtptr, i->io_ptr);
}

/*
 *	getformat - get format field and variable for print-using
 */

getformat(fmtptr, file)
char	*fmtptr;
FILE	*file;
{

	register Stkptr	s;
	register int	l;
	int		flag;
	double		f;
	char		*p, *fp, *tmp, *doformat(), *doformat(), *donumeric();

	fp = fmtptr;
	while (!endtest()) {
		while (*inptr == ' ' || *inptr == COMMA || *inptr == TAB)
			inptr++;	/* skip spaces & first comma or tab */
		expr();
		s = (Stkptr)stkptr;
		switch(s->k_type) {
		case	FLOATEXPR:
			f = popfloat();
			fp = donumeric(fp, f, file);
			break;
		case	STRINGEXPR:
			pop(ANYTYPE);
			fp = doformat(fp, s->k_un.k_str.s_ptr,
				s->k_un.k_str.s_len, file);
			break;
		default:
			badtype();
			break;
			}
		switch(*inptr) {
		case	' ':
		case	';':
		case	COMMA:
			++inptr;
			break;
		case	'\0':
		case	'\n':
		case	ELSE:
		case	COLON:
			break;
		default:
			badsyn();
			break;
		}
		}
	while (*fp > 0) {
		switch (*fp) {
		case '\'':	/* found a ' */
			fp++;
			switch (*fp) {
			case 'l':
			case 'L':
			case 'r':
			case 'R':
			case 'c':
			case 'C':
				fprintf(file, " ");
				for(tmp = fp+1;*fp && (*tmp == *fp); fp++,tmp++)
					fprintf(file, " ");
				fprintf(file, " ");
				fp++;
				break;
			default:
				fprintf(file, "'");
				break;
			}
			break;
		case '#':
			fprintf(file, " ");
			fp++;
			break;
		default:
			putc(*fp++, file);
			break;
		}
		}
	if (*(inptr - 1) != ';')
		putc('\n', file);
	if (file == stdout)
		fflush(file);

}

/*
 * donumeric -- print numeric variable in print-using format
 */

char	*donumeric(fmtptr, num, file)
char	*fmtptr;
double	num;
FILE	*file;

{
	register	int	k,c,d,l;
	register	char	*fp;
			int	negativeflag, dollarflag, periodflag;
			int	intsize, decsize;
			long	intpart, decpart;
			double 	f;


	/* the next line is for testing 
	printf("beginning donumeric routine");*/

	fp = fmtptr;
	c = 0;
	d = 0;
	f = num;

	if (f > 999999999)
		err("number too large ( > 999,999,999)");
		/*	check to see is number is too large to handle  */

	if (f < 0) {
		negativeflag = YES;
		f = -f;
		}
	else
		negativeflag = NO;

	dollarflag = NO;	/* dollar in format field	*/
	periodflag = NO;	/* period in format field	*/
	intsize = 1;		/* 1 more than max integer part	*/
	decsize = 1;		/* 1 more than max decimal part */

	/* if character in the format line is not a $ or #, print it. 
	 Or, if it is a $ or a . but is not followed by a #, print it. */

	while ((*fp > 0) && ((*fp != '$' && *fp != '#' && *fp != '.') ||  
			(*fp == '$' && *(fp + 1) != '#') ||
			(*fp == '.' && *(fp + 1) != '#')))
		fprintf(file, "%.1s", fp++);




	/* at this point we have either a # or 
			a $ followed by a #		*/


	if (*fp == '$') {
		dollarflag = YES;
		fp++;
		}

	while (*fp == '#') {
		c++;
		intsize = intsize * 10;
		fp++;
		}

	if (*fp == '.') {
		periodflag = YES;
		fp++;
		while (*fp == '#') {
			d++;
			decsize = decsize * 10;
			fp++;
			}
		}


	/* round-off number before checking for size	*/

	f = f + (5 / (10 * (float)decsize));

	intpart = f;		/* integer part of passed float	*/

	/* check for number too large for field
	if so, print *-filled format field	*/

	if (negativeflag == YES)
		intsize = intsize/10;	/* leave room for negative sign	*/

	if (intpart > (intsize -1)) {
		if (dollarflag == YES)
			fprintf(file, "$");
		for (; c; c--)
			fprintf(file, "*");
		if (periodflag == YES)
			fprintf(file, ".");
		for (; d; d--)
			fprintf(file, "*");
		return(fp);
		}


	/* number fits in field, begin print	*/

	/* the following line is for testing only
	printf("beginning print %d %d", intsize, c);
	used to enclode previous line in a comment when not used */

	/* if number is zero, divide intsize by 10 to allow room
		for the printing of the zero	*/

	if (intpart == 0)
		intsize = intsize/10;

	for (; intsize > ( 1 + intpart * 10); intsize = intsize / 10)
		fprintf(file, " ");

	if (negativeflag == YES)
		fprintf(file, "-");

	if (dollarflag == YES)
		fprintf(file, "$");

	if (intsize >= .01)
		fprintf(file, "%u", intpart);

	if (periodflag == YES)
		fprintf(file, ".");


	if (decsize > 1) {   /* if decsize is 1, no decimal part of # field  */

		decpart = (f - intpart) * decsize;
		if (decpart == 0)
			while (d--)
				fprintf(file, "0");
		else {
			while (decpart < (decsize / 10)){
				fprintf(file, "0");
				decsize = decsize / 10;
				}
			fprintf(file, "%u", decpart);
			}
		}


	return(fp);

}

/*
 *	doformat - determine format and print string
 */

char	*doformat(fmtptr, ptr, len, file)
char	*fmtptr;
char	*ptr;
int	len;
FILE	*file;
	{
	register int	k, c, d, l;
	register char	*p;
	register char	*fp, *tmp;

	p = ptr;
	fp = fmtptr;
	l = len;
	c = 1;
	while (*fp > 0) {
		if (*fp != '\'') {
			fprintf(file, "%.1s", fp++);
			continue;
			}
		fp++;
		switch (*fp) {
		case 'l':
		case 'L':
			tmp = fp + 1;
			while (*fp == *tmp) {
				c++;
				fp++;
				tmp++;
 				}
			c = c + 1;  /* count the last l or L */
			fp++;		/* move past last l or L */
			while (c-- > 0 && l-- > 0)
				fprintf(file, "%.1s", p++);
			while (c-- >= 0)
				fprintf(file, " ");
			return (fp);
			break;
		case 'r':
		case 'R':
			tmp = fp + 1;
			while (*fp == *tmp) {
				c++;
				fp++;
				tmp++;
 				}
			c = c + 1;  /* count the last r or R */
			fp++;		/* move past last r or R */
			if (l >= c) {
				p = p + (l - c);
				while (c-- > 0)
					fprintf(file, "%.1s", p++);
				}
			else {
				for (d = c - l; d > 0; d--)
					fprintf(file, " ");
				while (l-- > 0)
					fprintf(file, "%.1s", p++);
				}
			return (fp);
			break;
		case 'c':
		case 'C':
			tmp = fp + 1;
			while (*fp == *tmp) {
				c++;
				fp++;
				tmp++;
 				}
			c = c + 1;  /* count the last c or C */
			fp++;		/* move past last c or C */
			if (l < c) {
				d = (c - l)/2;
				k = (c - l) - d;

	/* this next line is for testing 
			printf( "d = %d c = %d k = %d l = %d, d, c, k, l);*/

				while (d--)
					fprintf(file, " ");
				while (l-- > 0)
					fprintf(file, "%.1s", p++);
				while (k-- > 0)
					fprintf(file, " ");
				}
			else {
				d = (l - c)/2;
				p = p + d;
				while (c-- > 0)
					fprintf(file, "%.1s", p++);
				}
			return(fp);
			break;
		default:
			fprintf(file, "'");
			break;
 
		}
	}
	return(fp);
}


/*
 * prtstmt --- interpret a PRINT statement
 */

prtstmt()
{
	register struct iotab	*i;

	i = getunit(OUTPUT, OUTUNIT);
	col = i->io_col;
	print(i->io_ptr);
	i->io_col = col;
}


/*
 * clsstmt --- interpret a CLOSE statement
 */

clsstmt()
{
	register struct iotab	*i;

	i = getunit(0, 0);
	iocls(i);
}


/*
 * flsstmt --- interpret a FLUSH statement
 */

flsstmt()
{
	register struct iotab	*i;

	i = getunit(OUTPUT, OUTUNIT);
	if (i->io_flag == OUTPUT)
		fflush(i->io_ptr);
}


/*
 * print --- do the work for a PRINT statement
 */

print(file)
FILE	*file;
{
	register Stkptr s;
	register int	l;
	int		flag;
	float		f;

	flag = NO;
	while (!endtest()) {
		flag = NO;
		if (*inptr == TAB) {	/* tab(expr) */
			++inptr;
			l = fexpr();
			if (l < 0 || l > 80)
				l = 1;
			expectc(RPAR);
			while (col < l - 8)
				printstr("\t", 1, file);
			if (col < l)
				printstr("        ", l - col, file);
			}
		else {
			expr();
			s = (Stkptr)stkptr;
			switch (s->k_type) {
			case FLOATEXPR:
				printstr(fprint(popfloat()), MAXSTR, file);
				break;
			case STRINGEXPR:
				pop(ANYTYPE);
				printstr(s->k_un.k_str.s_ptr,
					 s->k_un.k_str.s_len, file);
				break;
			default:
				badtype();
				}
			}
		switch(*inptr) {
		case COMMA:
			++inptr;
			printstr("\t", 1, file);
			if ((col % 16) == 9)
				printstr("\t", 1, file);
			flag = YES;		/* suppress the NL */
			break;
		case ';':
			++inptr;
			flag = YES;
			break;
		case '\0':
		case ELSE:
		case COLON:
			break;
		default:
			badsyn();
			}
		}
	if (!flag)
		printstr("\n", 1, file);
	if (file == stdout)
		fflush(file);
}


/*
 * printstr --- print a string of specified length; update col
 */

printstr(ptr, len, file)
char	*ptr;
FILE	*file;
{
	register int	c, l;
	register char	*p;

	l = len;
	if ((p = ptr) == NULL && l)
		err("invalid string pointer");
	while (--l >= 0 && (c = *p++)) {
		switch (c) {
		case '\n':
			col = 1;
			break;
		case '\t':
			col = ((col-1 + 8) & ~07) + 1;
			break;
		default:
			col++;
			break;
			}
		putc(c, file);
		}
}


/*
 * ask --- interpret an INPUT or ASK statement
 */

ask()
{
	register char	*v;
	char		*ptr;
	int		len, type;
	FILE		*f;

	f = getunit(INPUT, INUNIT)->io_ptr;
	askptr = askline;
	askline[0] = 0;
	while (!endtest()) {
		if (*inptr == QUOTE || *inptr == PRIME) {
			strconst(*inptr++);
			popstring(&ptr, &len);
			printstr(ptr, len, stderr);
			optional(askdelims);
			}
		v = getsvar(&type);
		optional(askdelims);
		while (*askptr == ' ')
			++askptr;
		if (*askptr == 0) {
			if (f == stdin)
			fputs("? ", stderr);
			if (readline(askline, f) < 0)
				err("EOF on input");
			askptr = askline;
			}
		cvtdata(v, type, &askptr);
		}
}


/*
 * morefiledata --- check to see if specified file is at end
 */

morefiledata()
{
	int		c;
	FILE		*f;

	f = getunit(INPUT, INUNIT)->io_ptr;
	if ((c = getc(f)) != EOF) {

		/* not at end of file. put character back and return 1 */

		ungetc(c, f);
		return(1);
		}
	else

		/* at end of file. return 0 */

		return(0);
	
}
/*
 * strpop --- pop string from stack; convert to null-terminated format
 */

char *strpop()
{
	static char	strtemp[MAXSTR];
	char		*str;
	int		len;

	popstring(&str, &len);
	if (len >= MAXSTR - 1)
		err("string too long");
	move(len, str, strtemp);
	strtemp[len] = 0;
	return(strtemp);
}
