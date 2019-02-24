/*
 * manage a simple symbol table
 *
 *	_rfnewtag(t, f)
 *		enter Rfile *f with tag t
 *	_rfdeltag(t)
 *		remove entry with tag t
 *	_rftagtof(t)
 *		return f for tag t
 * _rfdeltag returns 1 for OK, 0 for error
 * _rfnewtag returns f, or the old f if tag was already in use
 * (dup tag, no tag to delete, no room for more tags)
 */


#include "rf.h"
#include "tag.h"

#define	NULL	0

struct tagp {
	Tag tag;
	Rfile *f;
};

#define	NTAG	100

static struct tagp tags[NTAG];

Rfile *
_rftagtof(t)
register Tag t;
{
	register struct tagp *tp;
	register int i;

	for (tp = tags, i = 0; i < NTAG; tp++, i++)
		if (tp->f && tp->tag == t)
			return (tp->f);
	return (NULL);
}

Rfile *
_rfnewtag(t, f)
register Tag t;
Rfile *f;
{
	register struct tagp *tp;
	register int i;
	register struct tagp *ep;

	ep = NULL;
	for (tp = tags, i = 0; i < NTAG; tp++, i++) {
		if (tp->f) {
			if (tp->tag == t)
				return (tp->f);	/* dup tag */
			continue;
		}
		if (ep == NULL)
			ep = tp;
	}
	if (ep == NULL)
		return (NULL);
	ep->tag = t;
	ep->f = f;
	return (f);
}

_rfdeltag(t)
register Tag t;
{
	register struct tagp *tp;
	register int i;

	for (tp = tags, i = 0; i < NTAG; tp++, i++)
		if (tp->f && tp->tag == t) {
			tp->f = NULL;
			return (1);
		}
	return (0);
}
