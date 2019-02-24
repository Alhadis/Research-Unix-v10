# include "stdio.h"
# include "ctype.h"
# include "bib.h"

char    commlist[MAXCOMM]=   /*  list of strings of common words */
"";
int firsttime = 1;

/*  makekey(p,max_klen,common):  compresses *p into a key
        folds upper to lower case.  ignores non-alphanumeric
        drops keys of length <= 2.
        drops words in common (name of file of words, one per line)
            (first call determines common for all later calls)
*/
makekey(p,max_klen,common)
char *p;
int  max_klen;          /* max key length */
char *common;
{   
	register char *from, *to, *stop;

	if (firsttime) {
		firsttime= 0; 
		load_comm(common); 
	}

	from= p; 
	to= p; 
	stop= max_klen+p;
	while (*from != NULL  &&  to < stop) {   
		if      (islower(*from))      *to++ = *from++;
		else if (isdigit(*from))      *to++ = *from++;
		else if (isupper(*from))    { 
			*to++ = tolower(*from);  
			from++; 
		}
		else from++;
	}
	*to= NULL;
	if(isdigit(p[0])) {
		if ((p[0] != '1') || ((p[1] != '9') && (p[1] != '8')) || (to-p != 4)) {
			*p = NULL;
			return;
		}
	}

	if (to<=p+2 || c_look(p,1) ) {
		*p= NULL;
	}
}


char *
trimnl(ln)
char *ln;
{
	register char *p= ln;
	while (*p) p++;
	p--;
	if (*p == '\n') *p=0;
	return(ln);
}



#define COMNUM		500
#define COMTSIZE	997
int comcount = 500;
static char cbuf[COMNUM*9];
static char *cwds[COMTSIZE];


/*  read file common into common hashtable
*/
load_comm(common)
char *common;
{   	
	FILE    *commfile;          /*  stream of common words     */
	char *p;
	int i;

	commfile= fopen(common,"r");
	if (commfile==NULL) {
		fprintf(stderr, "cannot open '%s'\n", common);
		return;
	}
	p = cbuf;
	for(i=0; i <comcount; i++) {
		if(fgets(p,15,commfile) == NULL)
			break;
		trimnl(p);
		if(strlen(p) > 6)
			p[6] = 0;
		c_look(p, 0);
		while(*p++);
	}
	fclose(commfile);
}


c_look(s,fl)
char *s;
{
	int h;
	h = hash(s) % (COMTSIZE);
	while(cwds[h] != 0) {
		if(strcmp(s,cwds[h]) ==0)
			return(1);
		h = (h+1) % (COMTSIZE);
	}
	if(fl == 0)
		cwds[h] = s;
	return(0);
}


hash (s)
char *s;
{
	int c, n;
	for(n=0; c= *s; s++)
		n += (c*n+ c << (n%4));
	return(n>0 ? n : -n);
}
