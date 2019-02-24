#include	<string.h>
#include	<stdio.h>
#if defined(__STDC__) || defined(c_plusplus) || defined(__cplusplus)
#include	<stdlib.h>
#endif
#include	"re.h"

#ifdef	MAIN

main(argc, argv)
	char **argv;
{
	Expr *re;
	re_re *r;
	char *pat;
	FILE *tmp;
	char *tmpn;
	extern char *tmpnam();
	char e1[4096], e2[4096];
	unsigned char map[256];
	int n;

	if(argc != 2){
		fprintf(stderr, "Usage: efile pattern\n");
		exit(1);
	}
	pat = argv[1];
	for(n = 0; n < 256; n++)
		map[n] = n;
	r = re_recomp(pat, pat+strlen(pat), map);
	if(r == 0)
		exit(1);
	re = r->root;
	tmpn = tmpnam((char *)0);
	if((tmp = fopen(tmpn, "w+r")) == NULL){
		perror(tmpn);
		exit(1);
	}
	eg_epr(re, e1, 0);
	re_refile(r, tmp);
	rewind(tmp);
	r = re_filere(tmp);
	eg_epr(r->root, e2, 0);
	if(strcmp(e1, e2))
		printf("MISMATCH!!\nbefore:\n%s\nafter:\n%s\n", e1, e2);
	else
		printf("ok\n");
	re_refree(r);
	exit(0);
}
#else

#include	"lre.h"

#if defined(__STDC__) || defined(c_plusplus) || defined(__cplusplus)
static void etofile(Expr *, FILE *);
static Expr *filetoe(FILE *);
#else
static void etofile();
static Expr *filetoe();
#endif

#define		REVERSION		2

void
re_refile(re_re *re, FILE *fp)
{
	if(re == 0)
		return;
	if(putc(REVERSION, fp) != REVERSION){
		re_error("couldn't write version");
		return;
	}
	if(fwrite(re->mymap, 256, 1, fp) != 1){
		re_error("couldn't write char map");
		return;
	}
	putw(re->carat, fp);
	etofile(re->root, fp);
}

static void
etofile(Expr *e, FILE *fp)
{
	Expr ee;

	if(e == 0){
		e = &ee;
		e->type = Null;
	}
	putw(e->type, fp);
	putw(e->lit, fp);
	putc(e->backref, fp);
	putc(e->parens, fp);
	switch(e->type)
	{
	case Null:
	case Literal:
	case Dot:
	case Carat:
	case Dollar:
	case Backref:
		break;
	case Compcharclass:
	case Charclass:
		putw((int)e->l, fp);
		fwrite((char *)e->r, (int)e->l, 1, fp);
		break;
	case Cat:
	case Alternate:
		etofile(e->l, fp);
		etofile(e->r, fp);
		break;
	case Star:
	case Plus:
	case Quest:
	case Group:
	case EOP:
		etofile(e->l, fp);
		break;
	}
}

re_re *
re_filere(FILE *fp)
{
	register re_re *r;

	r = (re_re *)egmalloc(sizeof (re_re), "allocating re_re");
	if(r == 0)
		return(0);
	memset((char *)r, 0, sizeof (re_re));
	if(getc(fp) != REVERSION){
		re_error("read bad version number");
		goto err;
	}
	if(fread(r->mymap, 256, 1, fp) != 1){
		re_error("couldn't read char map");
		goto err;
	}
	r->carat = getw(fp);
	eg_lexinit((char *)0, (char *)0);
	if((r->root = filetoe(fp)) == 0){
err:
		free((char *)r);
		return(0);
	}
	egpost(r);
	if(r->backref || r->parens)
		egbr(r);
	else
		eginit(r, r->carat);
	return(r);
}

static Expr *
filetoe(FILE *fp)
{
	Expr *ee, *er, *el, *ret;
	int t, l;
	Exprtype et;
	char res[256];
	int br, parens;

	t = getw(fp);
	if((t == EOF) && feof(fp))
		return(0);
	et = (Exprtype)t;
	l = getw(fp);
	br = getc(fp);
	parens = getc(fp);
	switch(et)
	{
	case Null:
		return(0);
	case Literal:
	case Dot:
	case Carat:
	case Dollar:
	case Backref:
		ret = eg_newexpr(et, l, (Expr *)0, (Expr *)0);
		break;
	case Compcharclass:
	case Charclass:
		ee = eg_newexpr(et, l, (Expr *)0, (Expr *)0);
		l = getw(fp);
		ee->r = (Expr *)egmalloc(l, "filetoe copy");
		if (!ee->r)
			return 0;
		ee->l = (Expr *)l;
		fread((char *)ee->r, l, 1, fp);
		ret = ee;
		break;
	case Cat:
	case Alternate:
		el = filetoe(fp);
		er = filetoe(fp);
		ret = eg_newexpr(et, l, el, er);
		break;
	case Star:
	case Plus:
	case Quest:
	case Group:
	case EOP:
		el = filetoe(fp);
		ret = eg_newexpr(et, l, el, (Expr *)0);
		break;
	default:
		SPR res, "<reading expr undef type %d lit=%d>", t, l);
		re_error(res);
		return((Expr *)0);
	}
	ret->backref = br;
	ret->parens = parens;
	return(ret);
}
#endif
