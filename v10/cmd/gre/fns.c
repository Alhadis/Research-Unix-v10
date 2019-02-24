#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

void
pr(char *b, char *e)
{
	nmatch++;
	if(prname)
		PR "%s:", curfile);
	if(bflag){
		PR "%ld:", nbytes/offsetunit);
		nbytes += (e-b) + noverflow;
		noverflow = 0;
	}
	if(nflag)
		PR "%ld:", ++lnum);
	WR(b, e-b);
}

void
pr_m(char *b, char *e)
{
	register char *nl;

	while(nl = (char*)memchr(b, '\n', e-b)){
		nmatch++;
		nl++;
		if(prname)
			PR "%s:", curfile);
		if(bflag){
			PR "%ld:", nbytes/offsetunit);
			nbytes += (nl-b) + noverflow;
			noverflow = 0;
		}
		if(nflag)
			PR "%ld:", ++lnum);
		WR(b, nl-b);
		if((b = nl) >= e)
			break;
	}
}

/* ARGSUSED */
void
inc(char *UNUSED, char *UNUSED2)
{
#pragma ref UNUSED
#pragma ref UNUSED2
	nmatch++;
}

void
inc_m(register char *b, register char *e)
{
	register char *nl;

	while(nl = (char*)memchr(b, '\n', e-b)){
		nmatch++;
		if((b = nl+1) >= e)
			break;
	}
}

void
null(char *UNUSED, char *UNUSED2)
#pragma ref UNUSED
#pragma ref UNUSED2
{
}

void
count(register char *b, register char *e)
{
	nbytes += (e-b) + noverflow;
	noverflow = 0;
	lnum++;
}

void
count_m(register char *b, register char *e)
{
	register char *nl;

	nbytes += (e-b) + noverflow;
	noverflow = 0;
	while(nl = (char*)memchr(b, '\n', e-b)){
		lnum++;
		if((b = nl+1) >= e)
			break;
	}
}

void
oneshot(char *b, char *e)
{
	register char *nl;

	nmatch++;
	nl = (char*)memchr(b, '\n', e-b)+1;
	if(oneflag)
		pr(b, nl);
	longjmp(env, 1);
}
