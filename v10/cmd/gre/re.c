#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

re_re *
re_recomp(char *b, char *e, unsigned char *map)
{
	return(egprep(greparse, (unsigned char *)b, (unsigned char *)e, map, 1));
}

re_reexec(re_re *pat, char *b, char *e, char *match[10][2])
{
	unsigned char *mb[10], *me[10], **rb, **re;
	int n, i;

	if(match)
		rb = mb, re = me;
	else
		rb = re = 0;
	n = eg_match(pat, (unsigned char *)b, (unsigned char *)e, rb, re);
	if(match)
		for(i = 0; i < 10; i++){
			match[i][0] = (char *)mb[i];
			match[i][1] = (char *)me[i];
		}
	return(n);
}

static void
freeexpr(register Expr *e)
{
	switch(e->type)
	{
	case Literal:
	case Dot:
	case Carat:
	case Dollar:
		if(e->follow)
			free((char *)e->follow);
		break;
	case Compcharclass:
	case Charclass:
		free((char *)e->r);
		break;
	case Cat:
	case Alternate:
		freeexpr(e->l);
		freeexpr(e->r);
		break;
	case Star:
	case Plus:
	case Quest:
	case Group:
	case EOP:
		freeexpr(e->l);
		break;
	case Backref:
	default:
		break;
	}
}

void
re_refree(re_re *re)
{
	if(re == 0)
		return;
	if(re->posbase)
		free((char *)re->posbase);
	if(re->root)
		freeexpr(re->root);
	if(re->ptr)
		free((char *)re->ptr);
	if(re->states)
		free((char *)re->states);
	/* leave br alone for now; it is hard to get right */
	free((char *)re);
}
