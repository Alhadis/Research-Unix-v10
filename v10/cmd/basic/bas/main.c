#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"
#include "tokens.h"

static int	listlevel,	/* listing indentation level */
		rflg;		/* automatic run flag */

char		lnrdelims[] = {MINUS, COMMA, '-', ',', 0};

char	*endstr();
Linep	findline();
Lnr	cvtlnr();
Stkptr	pop();
void	old(), dorun(), doline();
FILE	*file;
char	*frstwd, *using;


/*
 * main --- main program for basic
 */

main(argc, argv)
int	argc;
char	**argv;
{
	register char	*argp;

	--argc;
	++argv;

	while (argc > 0 && *(argp = *argv) == '-') {
		++argv;
		--argc;
		while (*++argp)
			switch (*argp) {
			case 'r':
				++rflg;
				break;
			case 's':
				typelens[FLOAT] = sizeof(float);
				break;
			case 't':
				++tflg;
				break;
			case 'p':
				pltini(*argv++);
				--argc;
				break;
			default:
				fprintf(stderr, "I don't know about -%c\n",
						*argp);
				exit(1);
				}
		}

	initsys();
	init();
	if (argc > 0) {
		argcnt = argc;
		argvec = argv;
		argc = 0;
		enter(old, *argv);
		}	
	if (rflg) {
		enter(dorun, NULL);
		exit(0);
		}
	for (;;)
		enter(doline, NULL);
}


/*
 * doline --- read and process one line
 */

void doline()
{

	if (tty)
		fputs(prompt, stderr);
	if (readline(line, infile) < 0) {
		if (attnflg)
			err("attn!!");
		exit(0);
		}
	inptr = line;
	if (line[0] == 0)
		;
	else if (!moncmd())
		compile();
}


/*
 * compile --- tokenize a line; store numbered ones, execute others
 */

compile()
{
	register Lnr	lnr;
	register int	l;
	register Linep	p;

	inptr = line;
	tokenize(line, line);
	if (tflg)
		listline(&immed, stderr);
	inptr = line;
	if (isdigit(line[0])) {
		lnr = cvtlnr();
		if (*inptr) {
			l = strlen(inptr) + 1;
			move(l, inptr, immed.l_line);
			if (l & 1)
				++l;
			l += LINESIZE;
			immed.l_len = l;
			immed.l_lnr = lnr;
			storeline(&immed);
			}
		else {
			p = findline(lnr, NEXTLNR);
			if (p->l_lnr == lnr)
				delline(p);
			}
		}
	else {
		curline = (Linep)NULL;
		immed.l_len = 0;
		execute(&immed);
		}
	return(YES);
}


/*
 * findline --- search for the specified line; return pointer to it
 *		if line is not found and flag==NEXTLNR, return pointer
 *		to next higher-numbered line
 */

Linep findline(lnr, flag)
Lnr	lnr;
int	flag;
{
	register Linep	p;

	for ALL_LINES(p)
		if (lnr <= p->l_lnr)
			break;
	if (p->l_lnr != lnr && flag != NEXTLNR)
		err("line not found");
	return(p);
}


/*
 * list --- list the specified range of lines
 */

list(line1, line2, file)
Lnr	line1, line2;
FILE	*file;
{
	register Linep	p;

	listlevel = 0;
	for ALL_LINES(p) {
		if (attnflg)
			err("attn!");
		if (p->l_lnr >= line1)
			if (p->l_lnr <= line2)
				listline(p, file);
			else
				break;
		}
}


/*
 * listline --- list a single line
 */

listline(linep, file)
Linep	linep;
FILE	*file;
{
	register char	*p;
	register Linep	l;
	register int	c;
	int		i, wastoken = YES;

	l = linep;
	fprintf(file, "%5u\t", l->l_lnr);
	for (i = 0; i < listlevel; ++i)
		putc('\t', file);

	for (p = l->l_line; *p; ++p)
		if (istoken(*p)) {
			if (!wastoken)
				putc(' ', file);
			wastoken = YES;
			switch (c = *p) {
			case QUOTE:
			case PRIME:
				c = tokens[UNTOKEN(c)][0];
				putc(c, file);
				while (*++p && !istoken(*p)) {
					if (*p == c)
						putc(c, file);
					putc(*p, file);
					}
				putc(c, file);
				putc(' ', file);
				break;
			case REM:
			case PRFORM:
			case DATA:
				fputs(tokens[UNTOKEN(c)], file);
				break;
			case FOR:
				if (p[1] != INPUT && p[1] != OUTPUT && p[1] != APPEND)
					listlevel++;
				goto normal;
			case NEXT:
				listlevel--;
			default:
			normal:
				fputs(tokens[UNTOKEN(c)], file);
				putc(' ', file);
				}
			}
		else {
			putc(*p, file);
			wastoken = NO;
			}

	putc('\n', file);
}


/*
 * getlnrs2 --- parse up to two line numbers from current line
 *		but don't check for end of line in case there
 *		are more arguments on line (for example, 
 *		renumber 1000, 10, 300-400
 */

getlnrs2(lp1, lp2, type)
Lnr	*lp1, *lp2;
int	type;
{			
	register Lnr	line1, line2;

	if (type == LISTLNRS) {
		line1 = MINLNR;
		line2 = MAXLNR;
		}
	else {
		line1 = *lp1;
		line2 = *lp2;
		}
	if (*inptr) {
		skipbl();
		optional(lnrdelims);
		skipbl();
		line1 = cvtlnr();
		if (type == LISTLNRS)
			line2 = line1;
		skipbl();
		optional(lnrdelims);
		skipbl();
		if (*inptr)
			line2 = cvtlnr();
		}
	*lp1 = line1;
	*lp2 = line2;

}
/*
 * getlnrs --- parse up to two line numbers from current line
 */

getlnrs(lp1, lp2, type)
Lnr	*lp1, *lp2;
int	type;
{			
	register Lnr	line1, line2;

	if (type == LISTLNRS) {
		line1 = MINLNR;
		line2 = MAXLNR;
		}
	else {
		line1 = *lp1;
		line2 = *lp2;
		}
	if (*inptr) {
		skipbl();
		optional(lnrdelims);
		skipbl();
		line1 = cvtlnr();
		if (type == LISTLNRS)
			line2 = line1;
		skipbl();
		optional(lnrdelims);
		skipbl();
		if (*inptr)
			line2 = cvtlnr();
		}
	endchk();
	*lp1 = line1;
	*lp2 = line2;
}


/*
 * delete --- delete the specified range of lines
 */

delete(line1, line2)
Lnr	line1, line2;
{
	register Linep	p;

	for (p = findline(line1, NEXTLNR);
			isline(p) && p->l_lnr <= line2; delline(p))
		;
}


/*
 * delline --- delete the line pointed to by linep
 */

delline(linep)
register Linep	linep;
{
	register char	*p, *q;
	register int	l;

	p = (char *)linep;
	l = linep->l_len;
	q = p + l;
	smartmove(lastline - q, q, p);
	lastline -= l;
	--linecnt;
}


/*
 * storeline --- save the line pointed to by linep
 */

storeline(linep)
Linep	linep;
{
	register Linep	l;
	register char	*p, *q;
	int		overlap;

	l = linep;
	p = (char *)findline(l->l_lnr, NEXTLNR);
	if (l->l_lnr != ((Linep)p)->l_lnr)
		q = p;
	else
		q = p + ((Linep)p)->l_len;
	overlap = q - p;
	if (overlap >= l->l_len && overlap - l->l_len <= LEN_FUZZ)
		l->l_len = overlap;
	else {
		while (lastline + l->l_len - overlap > endlines)
			if (!morelines())
				err("out of room");
		smartmove(lastline - q, q, p + l->l_len);
		}
	move(l->l_len, (char *)l, p);
	lastline += l->l_len - overlap;
	if (!overlap)
		++linecnt;
}


/*
 * cvtlnr --- parse one line number from the current input line
 */

Lnr cvtlnr()
{
	register Lnr	l, lastl;
	register int	c;

	lastl = l = 0;
	while ((c = *inptr) && isdigit(c)) {
		l = 10 * l + (c - '0');
		if (l > MAXLNR || l < lastl)
			err("bad line number");
		lastl = l;
		++inptr;
		}
	return(l);
}


/*
 * optional --- accept any single character in str if present
 */

optional(str)
char	*str;
{
	register char	*s;

	if (*inptr)	
		for (s = str; *s; ++s)
			if (*s == *inptr) {
				inptr++;
				return(YES);
				}
	return(NO);
}


/*
 * tokenize --- convert input line into a string of tokens
 */

tokenize(inp, outp)
char	*inp, *outp;
{
	register char	*t, *p;
	register int	c;
	int		i;

	while ((c = *inp++)) {
	    if (c == ' ' || c == '\t')
		continue;
	    for (i = -1; (t = tokens[UNTOKEN(i)]); --i) {
		for (p = inp - 1;; ) {
		    if (*p++ != *t++) {
			if (p[-1] == ' ' || p[-1] == '\t') {
			    --t;
			    continue;
			    }
			else if (isupper(p[-1])
				   && p[-1] == toupper(t[-1]))
			    ;
			else
			    break;
			}
		    if (*t == 0) {
			if (tflg)
			    fprintf(stderr, "token %d ", i);
			inp = p;
			c = i;
			switch (i) {
			case REM:
			case PRFORM:
			case DATA:
			    *outp++ = c;
			    while ((*outp++ = *inp++))
				;
			    return;
			case PRIME:
			case QUOTE:
			    --t;
			    *outp++ = c;
			    while (*inp) {
				if (*inp == *t && *++inp != *t)
				    break;
				*outp++ = *inp++;
				}
			    if (*inp == 0) {
				*outp++ = c;
				*outp = 0;
				return;
				}
			    break;
			    }
			goto loop;
			}
		    }
		}
	    if (isupper(c))
		c = tolower(c);
loop:
	    *outp++ = c;
	    }
	*outp = c;
}


/*
 * execute --- interpret the line pointed to by linep
 */

execute(linep)
Linep	linep;
{
	register Lnr	line1;
	register int	c;
	register Stkptr s;
	Linep		fmlnep;
	char		*p;

	curline = linep;
	if (curline != &immed && !isline(curline))
		return;
loop:
	inptr = curline->l_line;
	if (trace && curline != &immed)
		fprintf(stderr, "#%u\n", curline->l_lnr);
	do {
		if (attnflg)
			err("attn");
next:
		c = *inptr;
		if (istoken(c)) {
			++inptr;
			switch (c) {
			case END:
				return;
			case TRACE:
				trace = YES;
				break;
			case NOTRACE:
				trace = NO;
				break;
			case PRINT:
				if (*inptr == ' ')
					inptr++;
				if (*inptr != USING)
					prtstmt();
				else {
					inptr++;
					line1 = cvtlnr();
					fmlnep = findline(line1, EXACTLNR);
					p = fmlnep->l_line;
					if (*p++ != PRFORM)
						err("image-line not found");
					else
						prtusing(p);
					}
				break;
			case INPUT:
			case ASK:
				ask();
				break;
			case READ:
				readdata();
				break;
			case RESTORE:
				if (data.k_un.k_gosub.g_inptr != NULL)
					restore();
				break;
			case DIM:
				dim();
				break;
			case FOR:
				forstmt();
				break;
			case NEXT:
				nextstmt();
				break;
			case REM:
			case MORE:
			case ELSE:
			case QUOTE:
			case PRIME:
			case PRFORM:
			case DATA:
				while (*inptr)
					++inptr;
				break;
			case RETURN:
				while (((Stkptr)stkptr)->k_type == STK_FOR)
					(void)pop(ANYTYPE);
				s = pop(STK_GOSUB);
				curline = s->k_un.k_gosub.g_curline;
				inptr = s->k_un.k_gosub.g_inptr;
				break;
			case GOSUB:
				line1 = cvtlnr();
				endchk();
				gosub.k_un.k_gosub.g_curline = curline;
				gosub.k_un.k_gosub.g_inptr = inptr;
				gosub.k_type = STK_GOSUB;
				gosub.k_len = GOSUBFRLEN;
				push(&gosub);
				curline = findline(line1, EXACTLNR);
				inptr = NULL;
				break;
			case GOTO:
				line1 = cvtlnr();
				endchk();
				curline = findline(line1, EXACTLNR);
				inptr = NULL;
				break;
			case DEF:
				def();
				break;
			case LET:
				let();
				break;
			case IF:
				ifstmt();
				goto nochk;
			case STOP:
				endchk();
				err("stop");
				break;
			case COLON:
				goto next;
			case ON:
				ongoto();
				break;
			case CHAIN:
				chain();
				break;
			case HPLOT:
				hplot();
				break;
			case HGR:
				hgr();
				break;
			case OPEN:
				openstmt();
				break;
			case CLOSE:
				clsstmt();
				break;
			case FLUSH:
				flsstmt();
				break;
			case USING:
				err("print must precede using");
				break;
			default:
				badsyn();
				}
			}
		else
			let();
		if (inptr)
			endchk();
nochk:
		;
		} while (inptr && *inptr);

	if (inptr == NULL) {
		if (curline && isline(curline))
			goto loop;
		return;
		}
	else if (curline != &immed) {
		curline = nextline(curline);
		if (isline(curline))
			goto loop;
		}
}


/*
 * run --- handle the RUN command
 */

run()
{
	register Lnr	line1;

	line1 = cvtlnr();
	endchk();
	dorun(line1);
}


/*
 * dorun --- begin execution at the specified line
 */

void dorun(line1)
Lnr	line1;
{

	clrsym();
	clrstk();
	clrio();
	curline = findline(line1, NEXTLNR);
	inptr = NULL;
	execute(curline);
}


/*
 * autonumber --- handle the AUTO command
 */

autonumber(line1, line2)
register Lnr	line1, line2;
{
	register Linep	p;
	register char	*q;

	curline = (Linep)NULL;
	for (;; line1 += line2) {
		if ((p = findline(line1, NEXTLNR)) && p->l_lnr == line1)
			err("line %u already exists", line1);
		sprintf(line, "%5u ", line1);
		fputs(line, stderr);
	     /* fflush(stderr); */
		q = endstr(line);
		if (readline(q, infile) < 0 || *q == 0) {
			if (attnflg)
				err("attn!!");
			break;
			}
		inptr = line;
		compile();
		}
}


/*
 * badsyn --- report bad syntax
 */

badsyn()
{

	err("bad syntax");
}


/*
 * init --- perform startup initialization
 */

init()
{

	initprio();
	clrsym();
	clrstk();
	clrprog();
}


/*
 * clrprog --- reinitialize the line storage space
 */

clrprog()
{
	register Linep	l;

	l = (Linep)lines;
	l->l_len = 0;
	l->l_lnr = MAXLNR;
	lastline = lines + LINESIZE;
	linecnt = 0;
}


/*
 * nextline --- return ptr to line following one pointed to by lp
 */

Linep nextline(lp)
register Linep	lp;
{

	return((Linep)((char *)lp + lp->l_len));
}
