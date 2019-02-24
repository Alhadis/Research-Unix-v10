#include "regexp.h"
#include <stdio.h>
#include <string.h>

/* version history of certificates
version 0, installed  Jun 6 1991
	Signed by name, ctime-date
	sum=string, count=number
	------
	message
	------
	sum=string, count=number
	End name, ctime-date
version 1, installed  Jun 10 1991
	Signed by name, ctime-date
	sum=string, date=gmtime-hex, count=number, ver=1
	------
	message with comma before each line, counted in sum
	------
	sum=string, date=gmtime-hex, count=number, ver=1
	End name, ctime-date
*/

extern char *malloc(), *realloc();
extern long time();
extern char *ctime();
extern void exit();
extern char *getline();

#define SIZEOF(a) (sizeof(a)/sizeof(*a))

int version = 0;

struct field {
	char *name, *pat;
	int len, seq;
	char *val;
};

struct field head[] = {
	{ "signer", "^..?.?.?.?.?.?.?$" },
	{ "ctime", "^... ... .. ..:..:.. ....$" }
};
enum { NAME, CTIME };

struct field data[] = { 
	{ "sum", "^[0-9a-f]+$" },
	{ "date", "^[0-9a-f]+$" },
	{ "count", "^[0-9]+$" },
	{ "ver", "^[0-9]+$" }
};
enum { SUM, DATE, COUNT, VERS };
int ndata[] = { 2, 4 };	/* # of fields in version */

char occ[][SIZEOF(data)] = {	/* sequence of fields in version*/
	{ 1,0,2,0 },
	{ 1,2,3,4 }
};

main(ac,av)
char **av;
{
	regsubexp mv[10];
	char *s, *body;
	int headlen, len1;
	long tloc;
	char *now;
	int i, lines;
	char canon[50];
	FILE *fil = stdin;
	int sflag = 0;
	regexp *l1= regcomp("Signed by (.+), (.+)$");
	char *pat = "signer=\\1, ctime=\\2";
	regexp *dashes = regcomp("------$");
	regexp *l3 = regcomp("End (.+), (.+)$");

	if(ac>1 && strcmp(av[1],"-s")==0) {
		sflag++;
		av++;
		ac--;
	}
	if(ac>1 && (fil=fopen(av[1],"r"))==0) {
		write(2,"verify: ",8);
		perror(av[1]);
		exit(1);
	}
	
	while(s=getline(fil)) {
		if(regexec(l1,s,mv,3)==0) continue;
		regsub(pat,canon,mv,3);	/* make the non-u uniform */
		if(parse(canon,head,SIZEOF(head))==2)
			break;
		for(i=0; i<SIZEOF(head); i++)
			head[i].val = 0;
	}
	if(s==0) error("cannot find signature line");

	s = getline(fil);
	if(s==0 || parse(s,data,SIZEOF(data))<2)
		error("cannot identify checksum line");
	version = data[VERS].val==0? 0: atoi(data[VERS].val);
	if(version >= SIZEOF(ndata))
		error("improper version number");
	for(i=0; i<SIZEOF(data); i++)
		if(data[i].seq != occ[version][i])
			error("incorrect field sequence");
	
	s = getline(fil);
	if(s==0 || regexec(dashes,s,mv,0)==0)
		error("cannot find beginning cut mark");

	len1 = atoi(data[COUNT].val);
	headlen = head[CTIME].len;
	if(version==1)
		headlen += data[DATE].len+head[NAME].len;
	body = malloc(len1+headlen+1);
	if(body==0) overflow();
	strcpy(body,head[CTIME].val);
	if(version!=0)
		strcat(strcat(body,data[DATE].val),head[NAME].val);
	if(input(body+headlen,len1,fil)==0)
		error("cannot read full signed text");

	s = getline(fil);
	if(s==0 || regexec(dashes,s,mv,0)==0)
		error("cannot find ending cut mark");

	s = getline(fil);
	if(s==0 || parse(s,data,SIZEOF(data))!=ndata[version])
		error("cannot find matching checksum line");

	s = getline(fil);
	if(s==0 || regexec(l3,s,mv,3)==0)
		error("cannot find matching signature line");
	regsub(pat,canon,mv,3);
	if(parse(canon,head,SIZEOF(head))!=2)
		error("cannot find matching signature line");

	lines = 0;
	if(version!=0) for(i=headlen+len1; --i>=headlen; )
		if(body[i]=='\n') lines++;

	switch(verify(head[NAME].val, data[SUM].val, body, headlen+len1)){
	case 1:
		(void)time(&tloc);
		now = ctime(&tloc);
		printf("Signature by %s, %s\n",
			head[NAME].val, head[DATE].val);
		printf("verified %.24s, count=%d\n",now, len1-lines);
		if(sflag) exit(0);
		printf("-----\n");
		output(body+headlen,len1);
		printf("-----\n");
		printf("verified %.24s, count=%d\n",now, len1-lines);
		printf("End %s, %s\n",
			head[NAME].val, head[DATE].val);
		exit(0);
	case 0:
		fprintf(stderr,"verify: Bogus\n");
		exit(1);
	case -1:
		fprintf(stderr,"verify: server unavailable; try later\n");
		exit(2);
	}
	/*NOTREACHED*/
}

error(err)
char *err;
{
	fprintf(stderr,"verify: document format error: %s\n", err);
	exit(1);
}

overflow()
{
	fprintf(stderr,"verify: out of space\n");
	exit(2);
}

output(s, n)
char *s;
{
	int d = version!=0;

	s += d;
	n -= d;
	while(--n >= 0) {
		putchar(*s);
		if(*s == '\n')
			s+=d, n-=d;
		s++;
	}
}

input(s, n, fil)
char *s;
FILE *fil;
{
	int c, nl;

	nl = 1;
	while(n>0) {
		c = getc(fil);
		if(c==EOF) break;
		if(nl && version!=0 && c!=',')
			continue;
		nl = 0;
		*s++ = c;
		n--;
		if(c=='\n') nl++;
	}
	return n==0;
}

parse(s,field,n)	/* extract name=val fields */
char *s;
struct field *field;
{
	regsubexp mv[3];
	int c = ',';
	regexp *nameval, *re;
	int seq, len, i;

	nameval = regcomp("^ *([a-z]+)=([^,]+)");
	if(nameval==0) overflow();

	for(seq=1; c==',' && regexec(nameval,s,mv,3); seq++) {
		c = mv[0].ep[0];
		s = mv[0].ep + 1;
		for(i=0; i<n; i++) {
			mv[1].ep[0] = 0;
			if(strcmp(field[i].name,mv[1].sp)==0)
				break;
		}
		if(i >= n)
			return -1;
		len = mv[2].ep - mv[2].sp;
		if(field[i].val==0) {	/* 1st time make entry */
			field[i].len = len;
			field[i].seq = seq;
			mv[2].ep[0] = 0;
			field[i].val = strdup(mv[2].sp);
			if(field[i].val==0) overflow();
			re = regcomp(field[i].pat);
			if(re==0) overflow();
			if(regexec(re,field[i].val,mv,0)==0)
				return -1;
			free((char*)re);
		}
		else		/* 2nd time check entry (hack) */
			if(field[i].len!=len || field[i].seq!=seq ||
			   strncmp(field[i].val,mv[2].sp,len)!=0)
				return -1;
	}
	free((char*)nameval);
	return c!=0? -1: seq-1;
}

char *
getline(fil)
FILE *fil;
{
	static char *s;
	static len;
	int n, c;

	if(s==0) {
		len = BUFSIZ;
		s = malloc(len);
		if(s==0) overflow();
	}
	n = 0;
	for(;;) {
		if(n>=len) {
			len += BUFSIZ;
			s = realloc(s,len);
			if(s==0) overflow();
		}
		c = getc(fil);
		switch(c) {
		case EOF:
			return 0;
		case '\n':
			s[n] = 0;
			return s;
		default:
			s[n++] = c;
		}
	}
}
