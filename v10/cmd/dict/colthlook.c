/* "look" modified for collins thesaurus */
#include <stdio.h>
#include <ctype.h>

#define DICT "/s5/collins/thesaurus/thesaurus"

char *filename = DICT;
extern char *ggets(), *fgets(), *strchr();
FILE *dfile;

int exact;
int iflag;
int acomp();
int (*compare)() = acomp;
int tab = 0374;
char entry[3000];
char word[250];
char key[50];

main(argc,argv)
char **argv;
{
	char *arg;
	while(argc>=2 && *argv[1]=='-') {
		for(arg=argv[1];;arg++) {
			switch(arg[1]) {
			case 'd':
			case 'f':
				continue;
			case 'i': 
				iflag++;
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
		put(entry,dfile,stdout);
		while(getword(entry)) {
			canon(entry,word);
			switch((*compare)(key,word)) {
			case -1:
				if(exact)
					break;
			case 0:
				put(entry,dfile,stdout);
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
		if(!getword(entry))
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
	while(getword(entry)) {
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
			while((c=getc(dfile))!='\n' && c!=EOF)
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
	/* printf("%s:%s:\n", s, t); */
	for(;*s==*t;s++,t++)
		if(*s==0)
			return(0);
	return(*s==0? -1:
		*t==0? 1:
		*s<*t? -2:
		2);
}

getword(w)
char *w;
{
	register c;
	for(;;) {
		c = getc(dfile);
		if(c==EOF)
			return(0);
		*w++ = c;
		if(c=='*' && (*w++=getc(dfile))!='L')
			break;
	}
	*w = 0;
	return(1);
}

put(entry, ifile, ofile)
char *entry;
FILE *ofile, *ifile;
{
	fputs(entry, ofile);
	while( putc(getc(ifile), ofile) != '\n')
		continue;
	fflush(ofile);
}

canon(old,new)
unsigned char *old,*new;
{
	register c;
	unsigned char *savo = old, *savn = new;
	for(;;) {
		*new = c = *old++;
		if(c==0||c==',')
			break;
		if(c=='#') {
			*new = c = *old;
			old += 2;
		}
		if(c=='*') {
			old++;
			continue;
		}
		if(!isalpha(c))
			continue;
		if(isupper(c))
			*new += 'a' - 'A';
		new++;
	}
	*new = 0;
	/* printf(">%s %s\n",savo,savn); */
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
