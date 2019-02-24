#include <stdio.h>

#define N 500

unsigned char from[N];
int nfrom;
unsigned char to[N];
int nto;
unsigned char trans[256];
unsigned char occur[256];

int cflag;
int dflag;
int sflag;

main(argc, argv)
unsigned char **argv;
{
	int c;
	for( ; argc>1 && argv[1][0]=='-'; argc--, argv++) {
		while(c = *++argv[1]) {
			switch(c) {
			case 's':
				sflag++;
				continue;
			case 'd':
				dflag++;
				continue;
			case 'c':
				cflag++;
				continue;
			default:
				fprintf(stderr,"tr: bad option\n");
				exit(1);
			}
		}
	}
	if(argc>1)
		nfrom = canon(argv[1],from);
	if(argc>2)
		nto = canon(argv[2],to);
	if(cflag)
		complement();
	if(dflag)
		delete();
	else
		translit();
	return 0;
}

delete()
{
	register i, c, last;
	for(i=0; i<nfrom; i++)
		trans[from[i]] = 1;
	if(sflag)
		for(i=0; i<nto; i++)
			occur[to[i]] = 1;
	last = -1;
	while((c=getchar()) != EOF)
		if(!trans[c] && (c!=last || !occur[c]))
			putchar(last = c);
}

translit()
{
	register i, c;
	if(nfrom>0 && nto==0) {
		fprintf(stderr,"tr: arg count\n");
		exit(1);
	}
	for(i=0; i<sizeof trans; i++)
		trans[i] = i;
	for(i=0; i<nfrom; i++)
		trans[from[i]] = to[i<nto? i: nto-1];
	if(sflag) 
		squeeze();
	else
		while((c=getchar()) != EOF)
			putchar(trans[c]);
}

squeeze()
{
	register i, c, last;
	for(i=0; i<nto; i++)
		occur[to[i]] = 1;
	last = -1;
	while((c=getchar()) != EOF)
		if((c=trans[c]) != last || !occur[c])
			putchar(last = c);
}

complement()
{
	register i;
	for(i=0; i<nfrom; i++)
		occur[from[i]] = 1;
	for(i=nfrom=0; i<sizeof occur; i++)
		if(!occur[i])
			from[nfrom++] = i;
	memset((char*)occur,0,sizeof occur);
}

getch(ps)
unsigned char **ps;
{
	unsigned char *s = *ps;
	int c = *s++;
	int n, i;
	if(c=='\\' && *s) {
		n = 0;
		for(i=0; i<3; i++)
			if(*s>='0' && *s<='7')
				n = 8*n + *s++ - '0';
			else
				break;
		if(i == 0)
			c = *s;
		else if(n <= 0377)
			c = n;
		else {
			fprintf(stderr,"tr: char>0377\n");
			exit(1);
		}
	}
	*ps = s;
	return c;
}

canon(s, t)
unsigned char *s, *t;
{
	int n = 0;
	int i, c;
	while(*s && n<N) {
		if(*s=='-' && n>0 && s[1]) {
			s++;
			c = getch(&s);
			for(i=t[n-1]; ++i<=c && n<N ; )
				t[n++] = i;
		} else
			t[n++] = getch(&s);
	}
	if(n >= N) {
		fprintf(stderr,"tr: arg string too long\n");
		exit(1);
	}
	return n;
}

