/*
 *
 *	UNIX debugger
 *
 */

#include "defs.h"

char	line[LINSIZ];
extern	int	infile;
char	*lp;
char	peekc,lastc = EOR;
int	eof;

/* input routines */

eol(c)
char	c;
{
	return(c==EOR || c==';');
}

int
rdc()
{
	do {
		readchar();
	} while (lastc==SPC || lastc==TB);
	return(lastc);
}

reread()
{
	peekc = lastc;
}

clrinp()
{

	lp = 0;
	peekc = 0;
}

int
readchar()
{
	register char *p;
	if (eof)
		lastc=0;
	else if (peekc) {
		lastc = peekc;
		peekc = 0;
	}
	else {
		if (lp==0) {
			for (p = line; p < &line[LINSIZ-1]; p++) {
				eof = read(infile, p, 1) <= 0;
				if (mkfault) {
					eof = 0;
					error(0);
				}
				if (eof) {
					p--;
					break;
				}
				if (*p == EOR) {
					if (p <= line)
						break;
					if (p[-1] != '\\')
						break;
					p -= 2;
				}
			}
			*++p = 0;
			lp = line;
		}
		if ((lastc = *lp) != 0)
			lp++;
	}
	return(lastc);
}

nextchar()
{
	if (eol(rdc())) {
		reread();
		return(0);
	}
	return(lastc);
}

quotchar()
{
	if (readchar()=='\\')
		return(readchar());
	else if (lastc=='\'')
		return(0);
	else
		return(lastc);
}

getformat(deformat)
char *deformat;
{
	register char *fptr;
	register BOOL	quote;

	fptr=deformat;
	quote=FALSE;
	while ((quote ? readchar()!=EOR : !eol(readchar())))
		if ((*fptr++ = lastc)=='"')
			quote = ~quote;
	lp--;
	if (fptr!=deformat)
		*fptr = '\0';
}
