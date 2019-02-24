#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <libc/libc.h>

/* tail command, posix plus v10 option -r.
   the simple command tail -c, legal in v10, is illegal */

long count;
int anycount;
int follow;
int file = 0;
enum { BEG, END } origin = END;
enum { CHARS, LINES } units = LINES;
enum { FWD, REV } dir = FWD;

extern void copy(void), keep(void), skip(void);
extern void usage(void), reverse(void);
extern void trunc(struct stat*, struct stat*);
extern void suffix(char*), fatal(char*);
extern long tseek(long, int);
extern long tread(char*, long);
extern void twrite(char*, long);
extern int getnumber(char*);
#define jump(o,p) tseek(o,p), copy()

main(int argc, char **argv)
{
	int seekable, c;
	for(; argc>1&&((c=*argv[1])=='-'||c=='+'); argc--,argv++ ) {
		if(getnumber(argv[1])) {
			suffix(argv[1]);
			continue;
		} else if(c == '-')
			switch(argv[1][1]) {
			case 'c':
				units = CHARS;
			case 'n':
				if(getnumber(argv[1]+2))
					continue;
				else if(argc>2&&getnumber(argv[2])) {
					argc--, argv++;
					continue;
				} else
					usage();
			case 'r':
				dir = REV;
				continue;
			case 'f':
				follow++;
				continue;
			case '-':
				argc--, argv++;
			}
		break;
	}
	if(dir==REV && (units==CHARS || follow || origin==BEG))
		fatal("incompatible options");
	if(!anycount)
		count = dir==REV? ~0UL>>1: 10;
	if(origin==BEG && units==LINES && count>0)
		count--;
	if(argc > 2)
		usage();
	if(argc > 1 && (file=open(argv[1],0)) < 0)
		fatal(argv[1]);
	seekable = lseek(file,0L,0) == 0;
	errno = 0;

	if(!seekable && origin==END)
		keep();
	else if(!seekable && origin==BEG)
		skip();
	else if(units==CHARS && origin==END)
		jump(-count, 2);
	else if(units==CHARS && origin==BEG)
		jump(count, 0);
	else if(units==LINES && origin==END)
		reverse();
	else if(units==LINES && origin==BEG)
		skip();
	if(follow && seekable)
		for(;;) {
			static struct stat sb0, sb1;
			trunc(&sb1, &sb0);
			copy();
			trunc(&sb0, &sb1);
			sleep(5);
		}
	return 0;
}

void
trunc(struct stat *old, struct stat *new)
{
	fstat(file, new);
	if(new->st_size < old->st_size)
		new->st_size = tseek(0L, 0);
}

void
suffix(char *s)
{
	while(*s && strchr("0123456789+-", *s))
		s++;
	switch(*s) {
	case 'b':
		if((count*=1024) < 0)
			fatal("too big");
	case 'c':
		units = CHARS;
	case 'l':
		s++;
	}
	switch(*s) {
	case 'r':
		dir = REV;
		return;
	case 'f':
		follow++;
		return;
	case 0:
		return;
	}
	usage();
}

void
skip(void)	/* read past head of the file to find tail */
{
	int i;
	long n;
	char buf[BUFSIZ];
	if(units == CHARS) {
		for( ; count>0; count -=n) {
			n = count<BUFSIZ? count: BUFSIZ;
			if(!(n = tread(buf, n)))
				return;
		}
	} else /*units == LINES*/ {
		n = i = 0;
		while(count > 0) {
			if(!(n = tread(buf, BUFSIZ)))
				return;
			for(i=0; i<n && count>0; i++)
				if(buf[i]=='\n')
					count--;
		}
		twrite(buf+i, n-i);
	}
	copy();
}

void
copy(void)
{
	long n;
	char buf[BUFSIZ];
	while((n=tread(buf, BUFSIZ)) > 0) {
		twrite(buf, n);
		fflush(stdout);	/* for FWD on pipe; else harmless */
	}
}

void
keep(void)	/* read whole file, keeping the tail */
{	/* complexity=length(file)*length(tail).  could be linear */
	int len = 0;
	long bufsiz = 0;
	char *buf = 0;
	int j, k, n;
	for(n=1; n;) {
		if(len+BUFSIZ > bufsiz) {
			bufsiz += 2*BUFSIZ;
			if(!(buf = realloc(buf, bufsiz+1)))
				fatal("out of space");
		}
		for( ; n && len<bufsiz; len+=n)
			n = tread(buf+len, bufsiz-len);
		if(count >= len)
			continue;
		if(units == CHARS)
			j = len - count;
		else /*units == LINES*/ {
			j = buf[len-1]=='\n'? len-1: len;
			for(k=0; j>0; j--)
				if(buf[j-1] == '\n')
					if(++k >= count)
						break;
		}
		memmove(buf, buf+j, len-=j);
	}
	if(dir == REV) {
		if(len>0 && buf[len-1]!='\n')
			buf[len++] = '\n';
		for(j=len-1 ; j>0; j--)
			if(buf[j-1] == '\n') {
				twrite(buf+j, len-j);
				if(--count <= 0)
					return;
				len = j;
			}
	}
	if(count > 0)
		twrite(buf, len);
}

void
reverse(void)	/* count backward and print tail of file */
{
	int first;
	long len = 0;
	long n = 0;
	long bufsiz = 0;
	char *buf = 0;
	long pos = tseek(0L, 2);
	for(first=1; pos>0 && count>0; first=0) {
		n = pos>BUFSIZ? BUFSIZ: (int)pos;
		pos -= n;
		if(len+n > bufsiz) {
			bufsiz += 2*BUFSIZ;
			if(!(buf = realloc(buf, bufsiz+1)))
				fatal("out of space");
		}
		memmove(buf+n, buf, len);
		len += n;
		tseek(pos, 0);
		if(tread(buf, n) != n)
			fatal("length error");
		if(first && buf[len-1]!='\n')
			buf[len++] = '\n';
		for(n=len-1 ; n>0 && count>0; n--)
			if(buf[n-1] == '\n') {
				count--;
				if(dir == REV)
					twrite(buf+n, len-n);
				len = n;
			}
	}
	if(dir == FWD) {
		tseek(n==0? 0: pos+n+1, 0);
		copy();
	} else if(count > 0)
		twrite(buf, len);
}

long
tseek(long o, int p)
{
	o = lseek(file, o, p);
	if(o == -1)
		fatal("");
	return o;
}

long
tread(char *buf, long n)
{
	int r = read(file, buf, n);
	if(r == -1)
		fatal("");
	return r;
}

void
twrite(char *s, long n)
{
	if(fwrite(s, 1 , n, stdout) != n)
		fatal("");
}

int
getnumber(char *s)
{
	if(*s=='-' || *s=='+')
		s++;
	if(!isdigit(*s))
		return 0;
	if(s[-1] == '+')
		origin = BEG;
	if(anycount++)
		fatal("excess option");
	count = atol(s);
	if(count < 0 ||		/* overflow */
	   (int)count != count)	/* protect int args (read, fwrite) */
		fatal("too big");
	return 1;
}	

void		
fatal(char *s)
{
	write(2, "tail: ", 6);
	if(errno)
		perror(s);
	else {
		write(2, s, strlen(s));
		write(2, "\n", 1);
	}
	exit(1);
}

char *u = "usage: tail [-n N] [-c N] [-f] [-r] [+-N[bc][fr]] [file]\n";
void
usage(void)
{
	write(2, u, strlen(u));
	exit(1);
}
