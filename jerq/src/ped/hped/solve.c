#include "beauty.h"
#include "dep.h"

#define MOV_THR 0.2

typedef	int	boolean;
#define	TRUE	1
#define	FALSE	0

static void errmsg (s, a, b, c, d)
char *s, *a, *b, *c, *d;
{
	fprintf (fq, s, a, b, c, d);
}

static void die (s, a, b, c, d)
char *s, *a, *b, *c, *d;
{
	fprintf (fq, s, a, b, c, d);
	abort ();
}

static variable vartab[2*SIDE_BUF];
static int nvartab;

#define	xVARTAB(i)	(&vartab[2*(i)])
#define yVARTAB(i)	(&vartab[2*(i)+1])

#define	ROW(i)	(((i)%2 == 0)?\
		row[(i)/2].x:\
		row[(i-1)/2].y)

#define	ROWP(v)	ROW(v - vartab)

#define	MOVTHR(i)	(((i)%2 == 0)?\
			mv_tol[(i)/2].x:\
			mv_tol[(i-1)/2].y)

#define	MOVTHRP(v)	MOVTHR(v - vartab)

static double epsilon;

varinit (k)
register int k;
{
	/* Make all variables independent.
	*/
	register variable *v;
	register j;

	epsilon = EPSILON;
	nvartab = 2*k;
	for (; --k >= 0;) {
		v = xVARTAB(k);
		v->rep = v->nrep = depgen (1.0, v);
		v = yVARTAB(k);
		v->rep = v->nrep = depgen (1.0, v);
	}
	epsilon = MOV_THR*smalls;
	for(j=0;j<nvartab;j++) {
		mv_tol[j].x *= MOV_THR; mv_tol[j].y *= MOV_THR;
	}
	depinit ();
}

#define	morevert(x)	((QPI < (x)) && ((x) < TPI))
#define	cot(x)	tan(HPI - (x))

static deplist *side_dx (s)
sides *s;
{
	return depadd (1.0, xVARTAB(s->p2 - row)->rep,
		-1.0, xVARTAB(s->p1 - row)->rep);
}

static deplist *side_dy (s)
sides *s;
{
	return depadd (1.0, yVARTAB(s->p2 - row)->rep,
		-1.0, yVARTAB(s->p1 - row)->rep);
}

static int sign_dx (s)
sides *s;
{
	if (s->p2->x < s->p1->x)
		return -1;
	else
		return 1;
}

static int sign_dy (s)
sides *s;
{
	if (s->p2->y < s->p1->y)
		return -1;
	else
		return 1;
}

#define	depeq(c1,d1,c2,d2)	(depadd((c1),(d1),-(c2),(d2)))

static deplist *makedep (e)
register eqnS *e;
{
	/* Use equation e to create a dependency list
	/* that is formally equal to zero.
	/* Note that an equation's "s" is an *angle*; thus slope = tan s.
	/* We must decide which of dy = (tan s)*dx and dx = (cot s)*dy to use.
	/* The relevant identity is tan(PI/2 - x) = cot(x).
	*/
	register deplist *result;

	if(detail)
	fprintf(fq,"side=%d sl1=%g side=%d sl2=%g type=%d\n",
		e->s1-stt,e->s1->s,e->s2-stt,e->s2->s,e->u);

	switch (e->u) {
		case SLOPE:
			{
			if (morevert(e->s1->s))
				result = depeq(1.0, side_dx(e->s1),
					cot(e->s1->s), side_dy(e->s1));
			else
				result = depeq(1.0, side_dy(e->s1),
					tan(e->s1->s), side_dx(e->s1));
				
			}
			break;
		case EXTENSION:
			{
			register deplist *dx, *dy;
			/* Note: this code could be smarter about collinearities
			/* at a point, by choosing a maximally distant pair;
			/* but it's not.
			*/
			if (abs(e->s1->s - e->s2->s) > EPSILON)
				errmsg ("extension of lines of slopes %g and %g\n",
					e->s1->s, e->s2->s);
			dx = depadd (1.0, xVARTAB(e->s2->p1 - row)->rep,
				-1.0, xVARTAB(e->s1->p1 - row)->rep);
			dy = depadd (1.0, yVARTAB(e->s2->p1 - row)->rep,
				-1.0, yVARTAB(e->s1->p1 - row)->rep);
			if (morevert(e->s1->s))
				result = depeq(1.0, dx,
					cot(e->s1->s), dy);
			else
				result = depeq(1.0, dy,
					tan(e->s1->s), dx);
			}
			break;
		case EQUAL:
			{
			register deplist *lhd, *rhd; /* differences */
			double lhq, rhq;	/* quantities; coeff = sqrt (1 + q*q) */
			int lhsign, rhsign;
			if (morevert(e->s1->s)) {
				lhq = cot(e->s1->s);
				lhd = side_dy(e->s1);
				lhsign = sign_dy(e->s1);
			} else {
				lhq = tan(e->s1->s);
				lhd = side_dx(e->s1);
				lhsign = sign_dx(e->s1);
			}
			if (morevert(e->s2->s)) {
				rhq = cot(e->s2->s);
				rhd = side_dy(e->s2);
				rhsign = sign_dy(e->s2);
			} else {
				rhq = tan(e->s2->s);
				rhd = side_dx(e->s2);
				rhsign = sign_dx(e->s2);
			}
			result = depeq(lhsign*sqrt (1 + lhq*lhq), lhd,
				rhsign*sqrt (1 + rhq*rhq), rhd);
			}
			break;
		case SAMEX:
			result = depeq(1.0, xVARTAB(e->s1->p1 - row)->rep,
				1.0, xVARTAB(e->s2->p1 - row)->rep);
			break;
		case SAMEY:
			result = depeq(1.0, yVARTAB(e->s1->p1 - row)->rep,
				1.0, yVARTAB(e->s2->p1 - row)->rep);
			break;
		default:
			die ("unknown equation type: %d\n", e->u);
	}
	return result;
}

#define	ROWDEX(v)	((v - vartab)/2)
#define	VARCHAR(v)	(((v - vartab)%2 == 0)?'x':'y')
extern char *varnameprint();

static variable *equatetozero (p)
deplist *p;
{
	/* Dependency list p is formally equal to zero.
	/* Make one of the variables it contains
	/* depend on the others.
	*/
	if (!VAR(p)) {
		if (abs(COEFF(p)) > EPSILON)
			die ("inconsistent equation\n");
		return NULL;
	} else {
		double curmax;
		deplist *q, *r;
		curmax = 0;
		for (q = p; q; q = NEXT(q))
			if (VAR(q) && curmax < abs(COEFF(q))) {
				curmax = abs(COEFF(q));
				r = q;
			}
		/* solve p = 0 for r */
		VAR(r)->nrep = depadd (1.0, VAR(r)->rep, -1.0/COEFF(r), p);
		if(detail) {
			errmsg ("%s: ", varnameprint (VAR(r)));
			depprint (VAR(r)->nrep);
		}
		return VAR(r);
	}
}

static double depeval (p)
register deplist *p;
{
	double value;
	value = 0.0;
	for (; p && VAR(p); p = NEXT(p))
		value += COEFF(p)*ROWP(VAR(p));
	if (p && !VAR(p))
		value += COEFF(p);
	return value;
}

static double depok (v)
variable *v;
{
	double evalvalue, origvalue;
	/* v->nrep is a dependency list representing constraints on v.
	/* How far does plugging in the original values of
	/* the variables on which v depends cause v to move?
	*/
	evalvalue = depeval(v->nrep);
	origvalue = ROWP(v);
	if(detail)
	errmsg ("%s originally = %g; evaluates to %g\n",
		varnameprint (v), origvalue, evalvalue);
	return abs(origvalue - evalvalue);
}

static void ped_out(v)
variable *v;
{
	register j, k;
	j = ROWDEX(v);
	if((v - vartab)%2) row[j].y = depeval(v->nrep);
	else row[j].x = depeval(v->nrep);
}

static void alldepeval ()
{
	for (; --nvartab >= 0;)
		ped_out (&vartab[nvartab]);
}

static void eqprint (e)
eqnS *e;
{
	switch (e->u) {
		case SLOPE:
			fprintf (fq, "SLOPE of side %d",
				e->s1 - stt);
			break;
		case EXTENSION:
			fprintf (fq, "EXTENSION of sides %d and %d",
				e->s1 - stt, e->s2 - stt);
			break;
		case EQUAL:
			fprintf (fq, "EQUAL of sides %d and %d",
				e->s1 - stt, e->s2 - stt);
			break;
		case SAMEX:
			fprintf (fq, "SAMEX of row[%d].x",
				e->s1->p1 - row);
			fprintf (fq, " and row[%d].x",
				e->s2->p1 - row);
			break;
		case SAMEY:
			fprintf (fq, "SAMEY of row[%d].y",
				e->s1->p1 - row);
			fprintf (fq, " and row[%d].y",
				e->s2->p1 - row);
			break;
	}
}

ignoreeq (v, e)
variable *v;
eqnS *e;
{
	/* Equation e would make v dependent,
	/* but we will ignore it instead.
	/* First, throw away the proposed new representation.
	*/
	v->nrep = v->rep;
	/* Now, print an informative message.
	*/
	fprintf (fq, "equation %d [", e - eq);
	eqprint (e);
	fprintf (fq, "] ignored; threshold %g\n", MOVTHRP(v));
}

static int pencomp (e1, e2)
eqnS *e1, *e2;
{
	if (e1->penalty < e2->penalty)
		return -1;
	else if (e1->penalty > e2->penalty)
		return 1;
	else
		return 0;
}

solve (eq, neq)
eqnS *eq;
{
	eqnS *p, *q;
	deplist *d;
	variable *v;
	detail = 1;
	errmsg ("entering solve()\n");
	q = eq + neq;
	/* horribly expensive way to compute penalties */
	for (p = eq; p < q; p ++) {
		if (p->penalty < 0.0)
			continue;
		d = makedep (p);
		v = equatetozero (d);
		if (!v) {
			p->penalty = 0.0;
			continue;
		} else
			p->penalty = depok (v);
		if (detail) {
			eqprint (p);
			fprintf (fq, ":	%f\n", p->penalty);
		}
		v->nrep = v->rep;
	}
	qsort (eq, neq, sizeof(eqnS), pencomp);
	errmsg ("beginning to apply constraints\n");
	for (p = eq; p < q; p ++) {
		d = makedep (p);
		v = equatetozero (d);
		if (!v) /* redundant equation */
			continue;
		if (depok (v) < MOVTHRP(v)) {
			removedeps (v);
			resetreps ();
		} else {
			ignoreeq (v, p);
		}
	}
	alldepeval ();
	depclean ();
}

void depprint (p)
register deplist *p;
{
	while (p) {
		fprintf (fq, "%f", COEFF(p));
		if (VAR(p))
			fprintf (fq, "* %s + ", varnameprint (VAR(p)));
		p = NEXT(p);
	}
	fprintf (fq, "\n");
}

char *varnameprint (v)
register variable *v;
{
	char buff[100];
	sprintf (buff, "row[%d].%c", ROWDEX(v), VARCHAR(v));
	return buff;
}
