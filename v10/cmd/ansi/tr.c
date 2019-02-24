#include <libc.h>

#define N 500
typedef unsigned char uchar;

uchar	from[N];
int	nfrom;
uchar	to[N];
int	nto;
uchar	trans[256];
uchar	occur[256];
uchar	buf[4096];

int cflag;
int dflag;
int sflag;

void	complement(void);
void	delete(void);
void	squeeze(void);
void	translit(void);
void	error(char*);
int	canon(uchar*, uchar*);
int	getch(uchar**);

main(int argc, uchar **argv)
{
	int c;
	for( ; argc>1 && argv[1][0]=='-'; argc--, argv++){
		while(c = *++argv[1]){
			switch(c){
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
				error("bad option");
			}
		}
	}
	if(argc>1)
		nfrom = canon(argv[1],from);
	if(argc>2)
		nto = canon(argv[2],to);
	if(argc>3)
		error("arg count");
	if(cflag)
		complement();
	if(dflag)
		delete();
	else
		translit();
	return 0;
}

void
delete(void)
{
	int i, c, n, last;
	uchar *p, *q;
	for(i=0; i<nfrom; i++)
		if(c=from[i])
			trans[c] = 1;
	if(sflag)
		for(i=0; i<nto; i++)
			occur[to[i]] = 1;
	last = -1;
	while((n=read(0, buf, sizeof buf)) > 0){
		for(p=q=buf; --n>=0;){
			c = *p++;
			if(!trans[c] && (c!=last || !occur[c]))
				*q++ = last = c;
		}
		if(write(1, buf, q-buf) != q-buf)
			error("write error");
	}
}

void
translit(void)
{
	int i, n, c;
	uchar *p;
	if(nfrom>0 && nto==0)
		error("arg count");
	for(i=0; i<sizeof trans; i++)
		trans[i] = i;
	for(i=0; i<nfrom; i++) {
		n = from[i];
		c = to[i<nto? i: nto-1];
		if(occur[n] && trans[n]!=c)
			error("ambiguous translation");
		trans[n] = c;
		occur[n] = 1;
	}
	memset(occur,0,sizeof occur);
	if(sflag) 
		squeeze();
	else
		while((n=read(0, buf, sizeof buf)) > 0){
			for(p=buf; --n>=0; p++)
				*p = trans[*p];
			if(write(1, buf, p-buf) != p-buf)
				error("write error");
		}
}

void
squeeze(void)
{
	int i, n, c, last;
	uchar *p, *q;
	for(i=0; i<nto; i++)
		occur[to[i]] = 1;
	last = -1;
	while((n=read(0, buf, sizeof buf)) > 0){
		for(p=q=buf; --n>=0;){
			c = *p++;
			if((c=trans[c]) != last || !occur[c])
				*q++ = last = c;
		}
		if(write(1, buf, q-buf) != q-buf)
			error("write error");
	}
}

void
complement(void)
{
	int i;
	for(i=0; i<nfrom; i++)
		occur[from[i]] = 1;
	for(i=nfrom=0; i<sizeof occur; i++)
		if(!occur[i])
			from[nfrom++] = i;
	memset(occur,0,sizeof occur);
}

int
getch(uchar **ps)
{
	uchar *s = *ps;
	int c = *s++;
	int n, i;
	if(c=='\\' && *s){
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
		else
			error("char>0377");
	}
	*ps = s;
	return c;
}

int
canon(uchar *s, uchar *t)
{
	int n = 0;
	int i, c;
	while(*s && n<N){
		if(*s=='-' && n>0 && s[1]){
			s++;
			c = getch(&s);
			for(i=t[n-1]; ++i<=c && n<N ; )
				t[n++] = i;
		} else
			t[n++] = getch(&s);
	}
	if(n >= N)
		error("arg string too long");
	return n;
}

void
error(char *s)
{
	fprint(2, "tr: %s\n", s);
	exit(1);
}
