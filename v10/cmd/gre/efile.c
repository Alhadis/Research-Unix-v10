#include	"re.h"
#include	<string.h>
#include	<stdio.h>

etofile(e, fp)
	Expr *e;
	FILE *fp;
{
	Expr ee;
	char res[256];

	if(e == 0){
		e = &ee;
		e->type = Null;
	}
	putw(e->type, fp);
	putw(e->lit, fp);
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

Expr *
filetoe(fp)
	FILE *fp;
{
	Expr *ee, *er, *el;
	int t, l;
	char res[256];

	t = getw(fp);
	if((t == EOF) && feof(fp))
		return(0);
	l = getw(fp);
	switch(t)
	{
	case Null:
		return(0);
	case Literal:
	case Dot:
	case Carat:
	case Dollar:
	case Backref:
		return(eg_newexpr(t, l, (Expr *)0, (Expr *)0));
	case Compcharclass:
	case Charclass:
		ee = eg_newexpr(t, l, (Expr *)0, (Expr *)0);
		l = getw(fp);
		ee->r = (Expr *)egmalloc(l, "filetoe copy");
		ee->l = (Expr *)l;
		fread((char *)ee->r, l, 1, fp);
		return(ee);
	case Cat:
	case Alternate:
		el = filetoe(fp);
		er = filetoe(fp);
		return(eg_newexpr(t, l, el, er));
	case Star:
	case Plus:
	case Quest:
	case Group:
	case EOP:
		el = filetoe(fp);
		return(eg_newexpr(t, l, el, (Expr *)0));
	default:
		SPR res, "<reading expr undef type %d lit=%d>", t, l);
		re_error(res);
		return((Expr *)0);
	}
}

#ifdef	MAIN
#include	<tmpnam.h>

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
	etofile(re, tmp);
	rewind(tmp);
	re = filetoe(tmp);
	eg_epr(re, e2, 0);
	if(strcmp(e1, e2))
		printf("MISMATCH!!\nbefore:\n%s\nafter:\n%s\n", e1, e2);
	else
		printf("ok\n");
	re_refree(r);
	exit(0);
}
#endif
