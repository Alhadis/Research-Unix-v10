#include <sys/types.h>
#ifndef BSD4_2
#include	<ndir.h>
#else
#include	<sys/dir.h>
#endif

/*
 * char *spname(name, score)
 *	char name[];
 *	int *score;
 *
 * returns pointer to correctly spelled name,
 * or 0 if no reasonable name is found;
 * uses a static buffer to store correct name,
 * so copy it if you want to call the routine again.
 * score records how good the match was; ignore if NULL return.
 */
char *
spname(name, score)
	register char *name;
	int *score;
{
#undef	DIRSIZ
#define	DIRSIZ	14
	register char *p, *q, *new;
	register d, nd;
	register DIR *dirf;
	register struct direct *ep;
	static char newname[128], guess[DIRSIZ+1], best[DIRSIZ+1];

	new = newname;
	*score = 0;
	for(;;){
		if (new >= &newname[128-DIRSIZ-2])
			return((char *)0);
		while(*name == '/')
			*new++ = *name++;
		*new = '\0';
		if(*name == '\0')
			return(newname);
		p = guess;
		while(*name!='/' && *name!='\0'){
			if(p != guess+DIRSIZ)
				*p++ = *name;
			name++;
		}
		*p = '\0';
		if((dirf=opendir(*newname? newname : ".",0)) == NULL)
			return((char *)0);
		d = 3;
		while(ep = readdir(dirf)) {
			nd = SPdist(ep->d_name, guess);
			if (nd>0
			 && (SPeq(".", ep->d_name) || SPeq("..", ep->d_name)))
				continue;
			if(nd<d) {
				p = best;
				q = ep->d_name;
				do; while(*p++ = *q++);
				d = nd;
				if(d == 0)
					break;
			}
		}
		closedir(dirf);
		if(d == 3)
			return((char *)0);
		p = best;
		*score += d;
		do; while(*new++ = *p++);
		--new;
	}
}
/*
 * very rough spelling metric
 * 0 if the strings are identical
 * 1 if two chars are interchanged
 * 2 if one char wrong, added or deleted
 * 3 otherwise
 */
SPdist(s, t)
	register char *s, *t;
{
	while(*s++ == *t)
		if(*t++ == '\0')
			return(0);
	if(*--s){
		if(*t){
			if(s[1] && t[1] && *s==t[1] && *t==s[1] && SPeq(s+2,t+2))
				return(1);
			if(SPeq(s+1, t+1))
				return(2);
		}
		if(SPeq(s+1, t))
			return(2);
	}
	if(*t && SPeq(s, t+1))
		return(2);
	return(3);
}
SPeq(s, t)
	register char *s, *t;
{
	while(*s++ == *t)
		if(*t++ == '\0')
			return(1);
	return(0);
}

