#include "stdio.h"
#include "cbt.h"
extern char *malloc();
extern bfile *bopen();
extern mbuf bkey();
extern long lseek();

char *onearg();
mbuf key, value;
bfile *it;
#define OPEN	1
#define CLOSE	2
#define SEEK	3
#define FIRST	4
#define NEXT	5
#define WRITE	6
#define BT	7
#define LEV	8
#define NODE	9
#define KEY	10
#define FLUSH	11
#define COMMIT	12
#define SHELL	13
#define DELETE	14
#define START	15
#define CHECK	16
struct cmd
{	int cmt, cln;
	char *ccm;
} cmnd[] =
{	{OPEN, 4, "open"},
	{CLOSE, 5, "close"},
	{SHELL, 1, "!"},
	{DELETE, 6, "delete"},
	{SEEK, 4, "seek"},
	{FIRST, 5, "first"},
	{NEXT, 4, "next"},
	{NEXT, 4, "read"},
	{WRITE, 5, "write"},
	{CHECK, 5, "check"},
	{BT, 2, "bt"},
	{LEV, 3, "lev"},
	{NODE, 4, "node"},
	{KEY, 3, "key"},
	{FLUSH, 5, "flush"},
	{COMMIT, 6, "commit"},
	{START, 5, "start"},
	{START, 7, "trstart"},
	{0, 0, 0}
};
char line[128];
char ndbuf[512];
main()
{
	int n;
	char *s, *p;
	key.mdata = malloc(200);
	value.mdata = malloc(200);
	for(;;) {
		for(n = 0; n < sizeof(line); n++)
			line[n] = 0;
		(void) fgets(line, sizeof(line), stdin);
		if(feof(stdin)) break;
		switch(cmtp(line)) {
		default:
			printf("?\n");
			break;
		case CLOSE:
			bclose(it);
			break;
		case SHELL:
			(void) system(line);
			printf("!\n");
			break;
		case START:
	/*		printf("%d\n", trstart()); */
			break;
		case COMMIT:
	/*		btcommit(); */
			break;
		case FLUSH:
			bflush(it);
			break;
		case OPEN:
			s = onearg(line);
			it = bopen(s, 2);
			if(it == NULL || errno)
			{	perror(s);
			}
			break;
		case DELETE:
			s = onearg(line);
			if(s[strlen(s)-1] == '\n')
				s[strlen(s)-1] = 0;
			todatum(s, &key);
			printf("%d\n", bdelete(it, key));
			break;
		case SEEK:
			s = onearg(line);
			if(s[strlen(s)-1] == '\n')
				s[strlen(s)-1] = 0;
			todatum(s, &key);
			printf("%d\n", bseek(it, key));
			break;
		case FIRST:
			printf("%d\n", bfirst(it));
			break;
		case NEXT:
			printf("%d - ", bread(it, &key, &value));
			prbuf(value);
			printf(" | ");
			prbuf(key);
			putchar('\n');
			break;
		case WRITE:
			twoarg(line, &s, &p);
			todatum(s, &key);
			todatum(p, &value);
			printf("%d\n", bwrite(it, key, value));
			break;
		case BT:
			prbt(it);
			break;
		case LEV:
			(void) sscanf(line, "%d", &n);
			if(n < 0 || n > it->height)
			{	printf("out of range\n");
				break;
			}
			tprnode(it->path[n]);
			break;
		case NODE:
			(void) sscanf(line, "%d", &n);
			(void) lseek(it->tfd, (long)n*NDSZ, 0);
			(void) read(it->tfd, ndbuf, NDSZ);
			tprnode((hdr *)ndbuf);
			break;
		case CHECK:
			(void) sscanf(line, "%d", &n);
			(void) lseek(it->tfd, (long)n * NDSZ, 0);
			(void) read(it->tfd, ndbuf, NDSZ);
			checknode((hdr *)ndbuf);
			break;
		case KEY:
			printf("%d ", breclen(it));
			prbuf(bkey(it));
			putchar('\n');
			break;
		}
	}
	exit(0);
}
cmtp(s) char *s;
{	struct cmd *p;
	int i;
	for(p=cmnd; p->cln != 0; p++)
		if(strncmp(s, p->ccm, p->cln) == 0)
		{	for(i=0; i<p->cln; i++)
				line[i] =' ';
			return(p->cmt);
		}
	return(-1);
}
char *onearg(s) char *s;
{	char *p, *q;
	for(p=s; *p == ' '; p++);
	for(q=p; *q && *q!='\n'; q++);
	*q = 0;
	return(p);
}
todatum(s, k) char *s; mbuf *k;
{	int i;
	k->mlen = strlen(s);
	if(s[k->mlen - 1] == '\n')
		k->mlen--;
	for(i=0; i<k->mlen; i++)
		k->mdata[i] = s[i];
}
twoarg(a, b, c) char *a, **b, **c;
{	char *p, *q;
	for(; *a==' '; a++);
	for(p=a; *p && *p!='\t'; p++);
	*p++ = 0;
	for(q=p; *q && *q!='\n'; q++);
	*q = 0;
	*b = a;
	*c = p;
}
prbuf(a) mbuf a;
{	int i;
	printf("%d:", a.mlen);
	for(i=0; i<a.mlen; i++)
		putchar(a.mdata[i]);
}
prbt(a) bfile *a;
{	int i;
	printf("ht %d adv %d rdw %d flags ", a->height, a->advnc,
		a->rdwrt);
	for(i = 0; i <= MXHT; i++)
		printf(" %d", a->flag[i]);
	putchar('\n');
	printf("nodes ");
	for(i = 0; i <= MXHT; i++)
		printf(" %d", a->loc[i]);
	putchar('\n');
	printf("rdkey #%d:", a->rdptr.rnum);
	printf(":\n");
}
tprnode(a) hdr *a;
{	int i;
	dkey *q;
	prhdr(a);
	q = (dkey *)(a+1);
	for(i = 0; i < a->kcnt; i++) {
		if(a->hlev == 0)
			prlfa(lfadr(a, i));
		else prnda(ndadr(a, i));
		prkey(q);
		q = (dkey *)((char *)q + q->dlen);
	}
	if(a->hlev)
		prnda(ndadr(a, i));
	putchar('\n');
}
checknode(a)
hdr *a;
{	int i;
	char *p;
	for(i = 0, p = (char *)(a + 1); i < a->kcnt; i++)
		p += *p;
	i = p - (char *)a + nfree(a) + sizeof(trailer);
	if(a->hlev)
		i += (a->kcnt + 1) * sizeof(ndaddr);
	else if(!treeonly(it))
		i += a->kcnt * sizeof(lfaddr);
	if(i == NDSZ) {
		printf("ok\n");
		return;
	}
	printf("nfree should be %d not %d\n", NDSZ - i + nfree(a),
		nfree(a));
}
prhdr(a) hdr *a;
{
	printf("stamp %ld kcnt %d type %d lev %d free %d\n",
		a->hstamp, a->kcnt, a->htype, a->hlev, nfree(a));
}
prlfa(a) lfaddr *a;
{
	if(treeonly(it))
		printf("\t");
	else
		printf("%ld %u\t", a->lloc, a->llen);
}
prnda(a) ndaddr *a;
{
	printf("%u\t", *a);
}
prkey(b) dkey *b;
{	int i;
	for(i = 0; i < b->dcom; i++)
		putchar(' ');
	for(i = 0; i < b->dlen - DKEYSZ; i++)
		putchar(b->dkey[i]);
	putchar('\n');
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n82/10/9:bttest.c\n"};
/*0111000111010001*/
