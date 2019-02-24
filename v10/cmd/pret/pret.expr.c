#define DEBUG	 0

#define setv	 1
#define addeq	 2
#define subeq	 3
#define muleq	 4
#define diveq	 5
#define modeq	 6
#define plus	 7
#define minus	 8
#define times	 9
#define div	10
#define mod	11
#define power	12
#define uminus	13
#define gt	14
#define lt	15
#define ge	16
#define le	17
#define eq	18
#define ne	19
#define land	20
#define lor	21
#define lnot	22
#define princ	23
#define prdec	24
#define poinc	25
#define podec	26

#define OP	0
#define NM	1

#define unary(c)	(c == uminus || c == lnot || c >= princ)
#define binary(c)	!unary(c)

struct REVPOL {
	char toktyp;
	short tokval;
} *parsed;

int prs = 0;
extern int verbose;
char * Emalloc();

pushnm(tok)
{	if (prs >= EXPRMAX)
		whoops("expression too long");
	parsed[prs].toktyp = NM;
	parsed[prs++].tokval = tok;
}

pushop(tok)
{	if (prs >= EXPRMAX)
		whoops("expression too long");
	parsed[prs].toktyp = OP;
	parsed[prs++].tokval = tok;
}

struct Node *
newnode(ntyp, nval, left, right)
	struct Node *left, *right;
{
	struct Node *try;

	try = (struct Node *) Emalloc(sizeof(struct Node));
	try->ntyp = ntyp;
	try->nval = nval;
	try->left = left;
	try->right = right;
	return try;
}

makeexpr(n)
{	pushexpr(n);
	return -(2 + addrevpol());
}

pushexpr(n)
	struct Node *n;
{
	if (n == NULL) return;
	switch (n->ntyp) {
	case NM: pushnm(n->nval); break;
	case OP: pushexpr(n->left); pushexpr(n->right); pushop(n->nval); break;
	default: whoops("unknown node type");
	}
}

struct {
	short n;
	struct REVPOL *p;
} revpols[MANY];

int npols = 0;

numexps(fd)
	FILE *fd;
{ int i, j;
  struct REVPOL *p;

	fprintf(fd, "EXPR %d\n", npols);
	for (i = 0; i < npols; i++)
	{	fprintf(fd, "%d: ", revpols[i].n);
		p = revpols[i].p;
		for (j = (revpols[i].n)-1; j >= 0; j--)
			fprintf(fd, "%d/%d ", p[j].toktyp, p[j].tokval);
		putc('\n', fd);
}	}

findsame(a)
{	struct REVPOL *p, *q;
	int k = revpols[a].n;
	int i, j;

	q = revpols[a].p;
	for (i = 0; i < npols; i++)
	{	if (revpols[i].n != revpols[a].n)
			continue;
		p = revpols[i].p;
		for (j = 0; j < k; j++)
			if (p[j].toktyp != q[j].toktyp
			||  p[j].tokval != q[j].tokval)
				break;
		if (j == k)
			break;
	}
	return i;
}

addrevpol()
{ int i;
  int retval;

	if (npols >= MANY)
		whoops("too many expressions");

	revpols[npols].n = prs;
	revpols[npols].p = parsed;

	if ((retval = findsame(npols)) == npols)
		npols++;

#if 0
	printf("exp(%d): ", prs);
	for (i = prs-1; i >= 0; i--)
	{	if (parsed[i].toktyp == NM)
		{	printf("%d ", parsed[i].tokval);
			continue;
		}
		switch(parsed[i].tokval) {
		case setv:	printf("= "); break;
		case addeq:	printf("+= "); break;
		case subeq:	printf("-= "); break;
		case muleq:	printf("*= "); break;
		case diveq:	printf("/= "); break;
		case modeq:	printf("%= "); break;
		case plus:	printf("+ "); break;
		case minus:	printf("- "); break;
		case times:	printf("* "); break;
		case div:	printf("/ "); break;
		case mod:	printf("% "); break;
		case power:	printf("^ "); break;
		case uminus:	printf(".- "); break;
		case gt:	printf("> "); break;
		case lt:	printf("< "); break;
		case ge:	printf(">= "); break;
		case le:	printf("<= "); break;
		case eq:	printf("== "); break;
		case ne:	printf("!= "); break;
		case land:	printf("&& "); break;
		case lor:	printf("|| "); break;
		case lnot:	printf("! "); break;
		case princ:	printf("++. "); break;
		case prdec:	printf("--. "); break;
		case poinc:	printf(".++ "); break;
		case podec:	printf(".-- "); break;
		default:	fprintf(stderr, "%d", parsed[i].tokval);
				whoops("unknown operator");
		}
	}
	putchar('\n');
#endif
	parsed = (struct REVPOL *) Emalloc(EXPRMAX * sizeof(struct REVPOL));
	prs = 0;

	return retval;
}
