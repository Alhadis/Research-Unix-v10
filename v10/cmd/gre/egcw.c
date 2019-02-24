#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

static altlist(Expr*, unsigned char *);
static word(Expr*, unsigned char*);
static re_cw *pat;

re_cw *
re_recw(re_re *r, unsigned char *map)
{
	unsigned char buf[20000];
	register Expr *e, *root = r->root;

	if(root->type != EOP)
		return(0);
	if(root->l->type != Cat)
		return(0);
	if(root->l->l->type != Star)
		return(0);
	if(root->l->l->l->type != Dot)
		return(0);
	e = root->l->r;
	pat = re_cwinit(map);
	if(altlist(e, buf) == 0)
		return(0);
	re_cwcomp(pat);
	return(pat);
}

static
altlist(Expr *e, unsigned char *buf)
{
	if(e->type == Alternate)
		return(altlist(e->l, buf) && altlist(e->r, buf));
	return(word(e, buf));
}

static unsigned char *p;

static
word(Expr *e, unsigned char *buf)
{
	if(buf)
		p = buf;
	if(e->type == Cat){
		if(word(e->l, (unsigned char *)0) == 0)
			return(0);
		if(word(e->r, (unsigned char *)0) == 0)
			return(0);
	} else if(e->type == Literal)
		*p++ = e->lit;
	else
		return(0);
	if(buf)
		re_cwadd(pat, buf, p);
	return(1);
}

