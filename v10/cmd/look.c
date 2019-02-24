#include <stdio.h>
#include <ctype.h>

#define DICT "/usr/dict/words"
extern char *ggets(), *fgets(), *strchr();
#define WORDSIZ BUFSIZ

char *filename = DICT;
FILE *dfile;

int fold;
int direc;
int exact;
int iflag;
int rev = 1;	/*-1 for reverse-ordered file, not implemented*/
int acomp();
int ncomp();
int (*compare)() = acomp;
int tab;
char entry[WORDSIZ];
char word[WORDSIZ];
char key[50];

main(argc,argv)
char **argv;
{
	char *arg;
	while(argc>=2 && *argv[1]=='-') {
		for(arg=argv[1];;arg++) {
			switch(arg[1]) {
			case 'd':
				direc++;
				continue;
			case 'f':
				fold++;
				continue;
			case 'i': 
				iflag++;
				continue;
			case 'n':
				compare = ncomp;
				continue;
			case 't':
				tab = arg[2];
				if(tab)
					++arg;
				continue;
			case 'x':
				exact++;
				continue;
			case 0:
				break;
			default:
				fprintf(stderr,"look: bad option %s\n",arg);
				return(2);
			}
			break;
		}
		argc --;
		argv++;
	}
	if(!iflag) {
		if(argc<2)
			iflag++;
		else {
			canon(argv[1],key);
			argv++;
			argc--;
		}
	}
	if(argc>=2)
		filename = argv[1];
	else {
		direc++;
		fold++;
		canon(key,key);
	}
	dfile = fopen(filename,"r");
	if(dfile==NULL) {
		fprintf(stderr,"look: can't open %s\n",filename);
		return(2);
	}
	if(!iflag) {
		if(locate(key,entry)==0)
			return(1);
	}
	do {
		if(iflag) {
			if(ggets(entry,sizeof entry,stdin)==0)
				return(0);
			canon(entry,key);
			if(locate(key,entry)==0)
				continue;
		}
		puts(entry);
		while(getword(entry,WORDSIZ)) {
			canon(entry,word);
			switch((*compare)(key,word)) {
			case -1:
				if(exact)
					break;
			case 0:
				puts(entry);
				continue;
			}
			break;
		}
	} while(iflag);
	return(0);

}

locate(key,entry)
char *key;
{
	long top,bot,mid;
	register c;
	bot = 0;
	fseek(dfile,0L,2);
	top = ftell(dfile);
	for(;;) {
		mid = (top+bot)/2;
		fseek(dfile,mid,0);
		do {
			c = getc(dfile);
			mid++;
		} while(c!=EOF && c!='\n');
		if(!getword(entry,WORDSIZ))
			break;
		canon(entry,word);
		switch((*compare)(key,word)) {
		case -2:
		case -1:
		case 0:
			if(top<=mid)
				break;
			top = mid;
			continue;
		case 1:
		case 2:
			bot = mid;
			continue;
		}
		break;
	}
	fseek(dfile,bot,0);
	while(getword(entry,WORDSIZ)) {
		canon(entry,word);
		switch((*compare)(key,word)) {
		case -2:
			return(0);
		case -1:
			if(exact)
				return(0);
		case 0:
			return(1);
		case 1:
		case 2:
			continue;
		}
	}
	return(0);
}

/* acomp(s,t) returns -2 if s strictly precedes t
		      -1 if s is a prefix of t
		      0 if s is the same as t
		      1 if t is a prefix of s
		      2 if t strictly precedes s
*/

acomp(s,t)
register char *s,*t;
{
	for(;*s==*t;s++,t++)
		if(*s==0)
			return(0);
	return(*s==0? -1:
		*t==0? 1:
		*s<*t? -2:
		2);
}

getword(w,n)
char *w;
{
	register c;
	while(--n>0) {
		c = getc(dfile);
		if(c==EOF)
			return(0);
		if(c=='\n') {
			*w = 0;
			return 1;
		}
		*w++ = c;
	}
	fprintf(stderr,"look: line too long\n");
	return(0);
}

canon(old,new)
char *old,*new;
{
	register c;
/*	printf(">%s\n",old); */
	for(;;) {
		*new = c = *old++;
		if(c==0||c==tab) {
			*new = 0;
			break;
		}
		if(direc) {
			if(!(isalnum(c)||c==' '||c=='\t'))
				continue;
		}
		if(fold) {
			if(isupper(c))
				*new += 'a' - 'A';
		}
		new++;
	}
}

ncomp(s,t)
register char *s, *t;
{
	char *is, *it, *js, *jt;
	int a, b;
	int ssgn, tsgn;
	while(isspace(*s))
		s++;
	while(isspace(*t))
		t++;
	ssgn = tsgn = -2*rev;
	if(*s == '-') {
		s++;
		ssgn = -ssgn;
	}
	if(*t == '-') {
		t++;
		tsgn = -tsgn;
	}
	for(is = s; isdigit(*is); is++) ;
	for(it = t; isdigit(*it); it++) ;
	js = is;
	jt = it;
	a = 0;
	if(ssgn==tsgn)
		while(it>t && is>s)
			if(b = *--it - *--is)
				a = b;
	while(is > s)
		if(*--is != '0')
			return(-ssgn);
	while(it > t)
		if(*--it != '0')
			return(tsgn);
	if(a) return(sgn(a)*ssgn);
	if(*(s=js) == '.')
		s++;
	if(*(t=jt) == '.')
		t++;
	if(ssgn==tsgn)
		while(isdigit(*s) && isdigit(*t))
			if(a = *t++ - *s++)
				return(sgn(a)*ssgn);
	while(isdigit(*s))
		if(*s++ != '0')
			return(-ssgn);
	while(isdigit(*t))
		if(*t++ != '0')
			return(tsgn);
	return(0);
}

char *
ggets(s,n,f)
char *s;
FILE *f;
{
	char *p = fgets(s,n,f);
	char *q;
	if(p && (q=strchr(s,'\n')))
		*q = 0;
	return p;
}
