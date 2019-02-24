#include "ideal.h"
#include "y.tab.h"

LINEPTR opqact (opqstmt, noadtree, linelist)
STMTPTR opqstmt;
NOADPTR noadtree;
LINEPTR linelist;
{
	STMTPTR bdstmt;
	LINEPTR inlines, outlines, both;
	LINENODE nuline;
	LINEPTR prevline;

	if (when_bug & 0100) bug_on;
	prevline = &nuline;
	prevline->next = NULL;
	both = linelist;
	if ((bdstmt = nextstmt (BDLIST, noadtree->defnode->parm->stmtlist))
		|| (bdstmt = nextstmt (BDLIST,findbox (noadtree->defnode->parm->name,FALSE)->stmtlist))) {
		EDGENODE edgelist;
		EXPRPTR bdwalk, lastbd;
		INTLPTR prevtx, curvtx, postvtx;
		EDGEPTR edgewalk;
		EDGEPTR forfreeing;
		lastbd = (EXPRPTR) tail ((BOXPTR) bdstmt->stmt);
		lastbd->next = exprgen (((EXPRPTR) bdstmt->stmt)->expr);
		edgewalk = &edgelist;
		prevtx = expreval (((EXPRPTR) bdstmt->stmt)->expr, noadtree);
		for (bdwalk = ((EXPRPTR) bdstmt->stmt)->next;
			bdwalk;
			bdwalk = bdwalk->next) {
			curvtx = expreval (bdwalk->expr, noadtree);
			if (((INTLPTR) bdwalk->expr)->oper == '^') {
				bdwalk = bdwalk->next;
				if (!bdwalk) {
					fprintf (stderr, "ideal: arc point may not begin boundary specification\n");
					return (linelist);
				}
				postvtx = expreval (bdwalk->expr, noadtree);
				edgewalk->next = edgearc (
					Re(prevtx),
					Im(prevtx),
					Re(curvtx),
					Im(curvtx),
					Re(postvtx),
					Im(postvtx)
				);
			} else {
				postvtx = curvtx;
				edgewalk->next = edgeline (
					Re(prevtx),
					Im(prevtx),
					Re(postvtx),
					Im(postvtx)
				);
			}
			prevtx = postvtx;
			edgewalk = edgewalk->next;
		}
		edgewalk->next = edgelist.next;
		lastbd->next = NULL;
		opqpoly (
			edgelist.next->next,
			linelist,
			&inlines,
			&outlines,
			&both
		);
		forfreeing = edgelist.next->next;
		edgelist.next->next = NULL;
		linefree (forfreeing);
	} else {
		fprintf(stderr, "ideal: no boundary list\n");
	}
	if (((MISCPTR) opqstmt->stmt)->info == INTERIOR) {
		prevline->next = outlines;
		linefree (inlines);
	} else {
		prevline->next = inlines;
		linefree (outlines);
	}
	if (both) {
		while (prevline->next)
			prevline = prevline->next;
		prevline->next = both;
	}
	linelist = lineclean (nuline.next);
	bug_off;
	return (linelist);
} /* opqact */

void opqinsert (code, alpha, opqlist)
int code;
float alpha;
OPQPTR *opqlist;
{
	OPQNODE head;
	OPQPTR walk, prev, new;
	walk = &head;
	walk->alpha = -INFINITY;
	walk->next = *opqlist;
	prev = NULL;
	while (walk->next && walk->next->alpha < alpha + EPSILON) {
		prev = walk;	
		walk = walk->next;
	}
	if (walk->alpha < alpha - EPSILON) {
		new = opqgen(code, alpha);
		new->next = walk->next;
		walk->next = new;
	} else {
		if (walk->code == EXT0 || walk->code == INFL0)
			if (code == EXT1 || code == INFL1) {
				walk->code = IGNORE;
			}
		else if (walk->code == EXT1 || walk->code == INFL1)
			if (code == EXT0 || code == INFL0) {
				walk->code = IGNORE;
			}
		else if (walk->code == SIMPLE && code != INHERIT)
			walk->code = code;
	}
	*opqlist = head.next;
}

LINEPTR lineclean (linelist)
LINEPTR linelist;
{
	/* clean short lines from linelist */
	LINEPTR prevline, linewalk;
	LINENODE nuhead;
	prevline = &nuhead;
	prevline->next = linewalk = linelist;
	while (linewalk) {
		if ((linewalk->kind == LINE)
			&& (fabs(linewalk->x0 - linewalk->x1) < EPSILON)
			&& (fabs(linewalk->y0 - linewalk->y1) < EPSILON)) {
			dprintf "Removing chopped line\n");
			prevline->next = linewalk->next;
			tryfree(linewalk);
			linewalk = prevline->next;
		} else {
			prevline = linewalk;
			linewalk = linewalk->next;
		}
	}
	linelist = nuhead.next;
	return (linelist);
}
