#include <stdio.h>
#include "typedef.h"
#include "basic.h"
#include "tokens.h"
#include <signal.h>

#define SEEK_EOF	2
#define MAGIC		2249413169	/* magic number for image files */

char		*prompt = ">";		/* the prompt charcter */
int		col = 1;		/* current output column */
static int	symsize = MAXSYMSPACE;	/* size of symbol table */

FILE	*xopen();
char	*sbrk(), *endstr();
void	fpterr(), attn();
double	fabs();
long	ftell();


/*
 * initsys --- perfor system-dependent initialization
 */

initsys()
{

	infile = stdin;
	tty = isatty(fileno(stdin));
	signal(SIGFPE, fpterr);
	signal(SIGINT, attn);
	lines = sbrk(MAXLINES + MAXSYMSPACE);	/* base of line buffer */
	endlines = lines + MAXSYMSPACE;
	symspace = endlines;	/* start of symbol table */
	symend = sbrk(0);
	if ((int)lines == -1)
		error("unable to allocate initial space");
	initio();
}


/*
 * attn --- function invoked upon SIGINT
 */

void attn()
{

	signal(SIGINT, attn);
	if (attnflg++)
		err("attn!");
}


/*
 * readline --- get a newline-terminated line from specified file
 */

readline(linep, inf)
char	*linep;
FILE	*inf;
{
	register int	c;
	register char	*p;

	for (p = linep; (c = getc(inf)) >= 0 ; ) {
		if (c == '\r' || c == 0)
			;
		else if (c == '\n') {
			*p = 0;
			if (inf == stdin)
				col = 1;
			return(p - linep);
			}
		else {
			if (inf == stdin)
				++col;
			if (p >= linep + MAXLINELEN)
				err("line too long");
			*p++ = c;
			}
		}
	return(-1);
}


/*
 * syserr --- do system-dependent error handling
 */

syserr()
{

	fseek(infile, 0L, SEEK_EOF);	/* in case input is a file */
	if (infile != stdin) {		/* reset input to stdin */
		xclose(infile);
		infile = stdin;
		}
}


/*
 * error --- print an error message and exit
 */

error(fmt, d1, d2, d3)
{

	fprintf(stderr, fmt, d1, d2, d3);
	exit(1);
}


/*
 * err --- print an error message and jump back to main program
 */

err(fmt, d1, d2, d3)
{

	fprintf(stderr, fmt, d1, d2, d3);
	if (curline != (Linep)NULL && curline != &immed)
		fprintf(stderr, " at line %u", curline->l_lnr);
	putc('\n', stderr);
	attnflg = 0;
	syserr();
	leave(fmt);
	exit(1);
}


/*
 * fpterr --- function invoked upon SIGFPE
 */

void fpterr()
{

	signal(SIGFPE, fpterr);
	err("floating point error");
}


/*
 * fprint --- convert floating point to string
 */

char *fprint(f)
double	f;
{
	register char	*s;
	static char	fpbuff[30];
	double		e;
	int		decpt, sign;

#define MINF		0.00000001
#define MAXF		10000000.0
#define NDIGITS 	5

	e = fabs(f);
	if (e > MAXF || (e != 0.0 && e < MINF))
		ftoa(f, fpbuff, NDIGITS, 'e');
	else
		ftoa(f, fpbuff, NDIGITS, 'f');
	return(fpbuff);
}


/*
 * ftoa --- do the real work for fprint
 */

ftoa(f, ptr, ndig, format)
double	f;
char	*ptr;
{
	register char	*s;

	if (format == 'e')
		sprintf(ptr, "%.*e", ndig, f);
	else {
		sprintf(ptr, "%.*f", ndig, f);
		s = endstr(ptr);
		while (*--s == '0')
			;
		if (*s == '.')
			--s;
		s[1] = 0;
		}
}


/*
 * morelines --- try to allocate more space for line storage
 */

morelines()
{
	register char	*p;

	p = endlines + EXPANDLINES;
	if ((int)sbrk(EXPANDLINES) == -1)
		return(NO);
	endlines = p;
	symspace = endlines;
	symend = symspace + symsize;
	clrsym();
	return(YES);
}


/*
 * moresym --- try to allocate more space for the symbol table
 */

moresym(p)
char	*p;
{
	register int	n;

	n = p - symend;
	n = (n + EXPANDSYM - 1) & ~(EXPANDSYM - 1);	/* make it even */
	if (p < symend || (int)sbrk(n) == -1)
		return(NO);
	symend += n;
	symsize = symend - symspace;
	return(YES);
}


/*
 * old --- read a program from the file with specified name
 */

void old(file)
char	*file;
{
	register FILE	*i, *oldfile;

	if (*file == 0)
		err("file name expected");
	if ((i = xopen(file, "r")) == (FILE *)NULL)
		err("can't open %s", file);
	if (file != tempfile)
		strcpy(curfile, file);	/* save current file name */
	init();
	oldfile = infile;
	infile = i;
	if (readline(line, infile) > 0 && fastload(line, infile) == NO) {
		compile();
		while (readline(line, infile) > 0)
			compile();
		}
	xclose(infile);
	infile = oldfile;
}


/*
 * fastload --- try to load a program image from the specified file
 */

fastload(lp, file)
register char	*lp;
FILE		*file;
{
	int	magic, bcount, nlines, check;

	if (sscanf(lp, "#%d %d %d %d\n", &magic, &bcount, &nlines, &check) != 4
			|| magic != MAGIC
			|| (magic ^ bcount ^ nlines ^ check))
		return(NO);
	while (endlines - lines < bcount && morelines())
		;
	if (bcount > endlines - lines)
		err("program image too big");
	lastline = lines + bcount;
	linecnt = nlines;
	lseek(fileno(file), ftell(file), 0);
	if (read(fileno(file), lines, bcount) != bcount)
		err("error in loading program image");
	return(YES);
}


/*
 * save --- write the program onto the named file
 */

save(file)
char	*file;
{
	register FILE	*f;
	int i, j;

	for (i = j = 0; file[i] != '\0'; i++)
		if (file[i] != ' ')
			file[j++] = file[i];
	file[j] = '\0';

/*	remove the blanks from the name		*/


	if (file[0] == 0 || (f = xopen(file, "w")) == (FILE *)NULL)
		err("can't create file %s", file);
	inptr = endstr(inptr);
	list(MINLNR, MAXLNR, f);
	xclose(f);
}


/*
 * fastsave --- write a program image to the named file
 */

fastsave(file)
char	*file;
{
	register FILE	*f;
	register int	check, bcount;

	if (file[0] == 0 || (f = xopen(file, "w")) == (FILE *)NULL)
		err("can't create file %s", file);
	inptr = endstr(inptr);
	bcount = lastline - lines;
	check = MAGIC ^ bcount ^ linecnt;
	fprintf(f, "#%u %u %u %u\n", MAGIC, bcount, linecnt, check);
	fflush(f);
	if (write(fileno(f), lines, bcount) != bcount)
		err("error in writing program image");
	xclose(f);
}
