#if SPNAME

#include <sys/types.h>
#include <ndir.h>
#include <stdio.h>

#define	ARB	100

/*
 * char *spname(name)
 *	char name[];
 *
 * returns pointer to correctly spelled name,
 * or 0 if no reasonable name is found;
 * uses a static buffer to store correct name,
 * so copy it if you want to call the routine again.
 */
char *
spname(name)
	register char *name;
{
	register char *p, *new;
	register d, nd;
	DIR *dir;
	struct direct *np;
	static char newname[ARB+1];
	char guess[MAXNAMLEN+1], best[MAXNAMLEN+1];

	new = newname;
	for(;;){
		while(*name == '/') {
			if (new >= newname+ARB)
				return (NULL);
			*new++ = *name++;
		}
		*new = '\0';
		if(*name == '\0')
			return(newname);
		p = guess;
		while(*name!='/' && *name!='\0'){
			if(p != guess+MAXNAMLEN)
				*p++ = *name;
			name++;
		}
		*p = '\0';
		if((dir=opendir(newname)) == NULL)
			return(NULL);
		d = 3;
		while ((np = readdir(dir)) != NULL) {
			nd=SPdist(np->d_name, guess);
			if(nd<=d && nd!=3) {	/* <= to avoid "." */
				strcpy(best, np->d_name);
				d = nd;
				if(d == 0)
					break;
			}
		}
		closedir(dir);
		if(d == 3)
			return(0);
		p = best;
		while (*new++ = *p++)
			if (new >= newname+ARB)
				return (NULL);
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

#endif
