#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

static int egparen(Expr *e);

int
re_paren(re_re *re)
{
	return egparen(re->root);
}

static int
egparen(Expr *e)
{
	if(e == 0)
		return(0);
	switch(e->type)
	{
	case Null:
	case Literal:
	case Dot:
	case Carat:
	case Dollar:
	case Backref:
	case Compcharclass:
	case Charclass:
		break;
	case Cat:
	case Alternate:
		return(egparen(e->l)+egparen(e->r));
	case Star:
	case Plus:
	case Quest:
	case EOP:
		return(egparen(e->l));
	case Group:
		return(1+egparen(e->l));
	}
	return(0);
}
