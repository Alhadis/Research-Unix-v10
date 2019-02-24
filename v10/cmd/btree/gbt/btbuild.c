#include "stdio.h"
#include "cbt.h"
#define SZ	MXHT+1

typedef struct {
	unsigned short mlen;
	char mdata[NDSZ];	/* max size of values */
} xbuf;
int address[SZ];	/* where the next address goes in the node */
xbuf curkey[SZ];	/* last key put in the node */
xbuf shortone;		/* for compression */
int freecnt[SZ];		/* number of bytes left in node */
dkey *kp[SZ];		/* where to put next dkey in node */
char node[SZ][NDSZ];	/* the nodes */
char used[SZ+1];	/* which nodes have been used */
xbuf nkey;		/* as read by getrec */
struct {
	unsigned short mlen;
	char mdata[32767];
} val;
lfaddr currec;		/* where getrec put val */
FILE *tfd, *dfd;
char name[16];
int type;
long reccnt;
extern char *malloc();

char *
prkey(a)
xbuf *a;
{	int i;
	unsigned char c;
	static char buf[4*NDSZ], *p;
	for(i = 0, p = buf; i < a->mlen; i++) {
		c = a->mdata[i];
		if(c < ' ') {
			*p++ = '^';
			c += ' ';
		}
		if(c < 127)
			*p++ = (char)c;
		else {
			*p++ = '\\';
			*p++ = (c >> 6) + '0';
			*p++ = ((c&63)>>3) + '0';
			*p++ = c&7 + '0';
		}
	}
	*p++ = 0;
	return(buf);
}
pref(a, b, lev) xbuf *a, *b;
{	register int n;
	register char *p, *q;
	for(n = 0, p = a->mdata, q = b->mdata; n < a->mlen && n < b->mlen; n++)
		if(*p++ != *q++)
			break;
	p--, q--;
	if(lev == 0 && ((n == b->mlen && n >= a->mlen) || *p > *q)) {
		fprintf(stderr, "key %ld out of order:\n", reccnt);
		fprintf(stderr, "key %ld is %s\n", reccnt - 1, prkey(a));
		fprintf(stderr, "key %ld is %s\n", reccnt, prkey(b));
	}
	return(n);
}

getrec()
{
	reccnt++;
	if(mget(&nkey))
		return(-1);
	if(mget((xbuf *)&val))
		fail("half a record read");
	if((currec.llen = val.mlen) != 0) {
		currec.lloc = ftell(dfd);
		(void) fwrite(val.mdata, 1, val.mlen, dfd);
		if(ferror(dfd))
			fail("value write");
	}
	else currec.lloc = 0;
	return(1);
}

mget(a) xbuf *a;
{
	(void) fread((char *)&(a->mlen), 1, sizeof(a->mlen), stdin);
	if(feof(stdin) || ferror(stdin))
		return(1);
	if(fread(a->mdata, 1, a->mlen, stdin) != a->mlen)
		return(1);
	return(0);
}

ndaddr ndwrt(level)
{	ndaddr loc;
	int i;
	hdr *x;
	trailer *t;
	loc = ftell(tfd)/NDSZ;
	x = (hdr *)(node[level]);
	x->hlev = level;
	t = (trailer *)(node[level] + NDSZ - sizeof(trailer));
	t->tfree = freecnt[level];
	x->kcnt = address[level] - (level == 0? 0: 1);
	x->htype = type;
	(void) fwrite(node[level], 1, NDSZ, tfd);
	if(ferror(tfd))
		fail("node write");
	for(i = 0; i < NDSZ; i++)
		node[level][i] = 0;
	address[level] = 0;
	curkey[level].mlen = 0;
	freecnt[level] = NDSZ - sizeof(hdr) - sizeof(trailer);
	if(level)
		freecnt[level] -= sizeof(ndaddr);
	kp[level] = (dkey *)(node[level] + sizeof(hdr));
	return(loc);
}

finish(level, ptr) ndaddr ptr;
{	ndaddr loc;
	if(!used[level])
		return;
	*ndadr(node[level], address[level]++) = ptr;
	if(!used[level + 1])
		fseek(tfd, 0L, 0);
	loc = ndwrt(level);
	finish(level + 1, loc);
}

insert(level, key, ptr) xbuf *key; ndaddr ptr;
{	int n, len;
	char *p;
	ndaddr loc;
	used[level] = 1;
	n = pref(&curkey[level], key, level);
	len = DKEYSZ + key->mlen - n + sizeof(ptr);
	if(len < freecnt[level]) {
		*ndadr(node[level], address[level]++) = ptr;
		kp[level]->dlen = len - sizeof(ptr);
		kp[level]->dcom = n;
		mvgbt(kp[level]->dkey, key->mdata + n, key->mlen - n);
		p = (char *)(kp[level]) + kp[level]->dlen;
		kp[level] = (dkey *)p;
		freecnt[level] -= len;
		curkey[level] = *key;
	}
	else {
		*ndadr(node[level],address[level]++) = ptr;
		loc = ndwrt(level);
		insert(level+1, key, loc);
		curkey[level].mlen = 0;
	}
}

doit()
{	int n, len;
	ndaddr loc;
	char *p;
	for(;;) {
		if(getrec() == -1)
			break;
		used[0] = 1;
		n = pref(&curkey[0], &nkey, 0);
		len = DKEYSZ + nkey.mlen - n + sizeof(currec);
		if(type & INDEX)
			len -= sizeof(currec);
		if(len < freecnt[0]) {
			freecnt[0] -= len;
			if(!(type & INDEX))
				*lfadr(node[0], address[0]++) = currec;
			else
				address[0]++;
			kp[0]->dlen = DKEYSZ + nkey.mlen - n;
			kp[0]->dcom = n;
			mvgbt(kp[0]->dkey, nkey.mdata + n, nkey.mlen - n);
			p = (char *)(kp[0]) + kp[0]->dlen;
			kp[0] = (dkey *)p;
			curkey[0] = nkey;
		}
		else {
			shortest(&curkey[0], &nkey);
			loc = ndwrt(0);
			freecnt[0] -= DKEYSZ + nkey.mlen + sizeof(currec);
			if(type & INDEX)
				freecnt[0] += sizeof(currec);
			insert(1, &shortone, loc);
			if(!(type & INDEX))
				*lfadr(node[0], address[0]++) = currec;
			else
				address[0]++;
			kp[0]->dlen = DKEYSZ + nkey.mlen;
			kp[0]->dcom = 0;
			mvgbt(kp[0]->dkey, nkey.mdata, nkey.mlen);
			p = (char *)(kp[0]) + kp[0]->dlen;
			kp[0] = (dkey *)p;
			curkey[0] = nkey;
		}
	}
	if(!used[1])
		(void) fseek(tfd, 0L, 0);
	loc = ndwrt(0);
	finish(1, loc);
}

init(s)
char *s;
{	int i;
	hdr *b;
	char xx[NDSZ];
	(void) sprintf(name, "%s.T", s);
	tfd = fopen(name, "r");
	if(tfd == NULL) {
		perror(s);
		exit(1);
	}
	fread(xx, 1, NDSZ, tfd);
	fclose(tfd);
	tfd = fopen(name, "w");
	(void) fseek(tfd, (long)NDSZ, 0);
	b = (hdr *)xx;
	type = b->htype;
	(void) sprintf(name, "%s.F", s);
	if((type & INDEX) != INDEX)
		dfd = fopen(name, "w");
	for(i = 0; i < SZ; i++) {
		freecnt[i] = NDSZ - sizeof(hdr) - sizeof(trailer);
		if(i)
			freecnt[i] -= sizeof(ndaddr);
		kp[i] = (dkey *)(node[i] + sizeof(hdr));
	}
}

main(argc, argv)
char **argv;
{
	if(argc != 2) {
		fprintf(stderr, "%s: too many arguments\n", argv[1]);
		exit(1);
	}
	init(argv[1]);
	doit();
	exit(0);
}

shortest(a, b) xbuf *a, *b;
{	int n;
	char *p, *q, *s;
	p = a->mdata;
	q = b->mdata;
	s = shortone.mdata;
	for(n = 0; n < a->mlen && n < b->mlen; n++, p++, q++)
		if(*p == *q)
			*s++ = *p;
		else break;
	if(n < a->mlen) {
	again:
		if(n + 1 == a->mlen)
			*s++ = *p;
		else if(*p + 1 < *q)
			*s++ = *p + 1;
		else {
			*s++ = *p++;
			n++;
			if(*p + 1 > *p)
				*s++ = *p + 1;
			else goto again;
		}
	}
	shortone.mlen = s - shortone.mdata;
}
prbuf(x) xbuf *x;
{	int i;
	for(i = 0; i < x->mlen; i++)
		printf("%3.3o ", x->mdata[i]);
	putchar('\n');
}
fail(s)
char *s;
{
	perror(s);
	exit(2);
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/8/9:btbuild.c\n"};
/*1101000110100100*/
