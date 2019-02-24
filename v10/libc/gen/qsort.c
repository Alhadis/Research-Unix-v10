/* qsort -- qsort interface implemented by faster quicksort */

#define SWAPINIT(a, es) swaptype =         \
    (a-(char*)0 | es) % sizeof(long) ? 2 : es > sizeof(long) ? 1 : 0;
#define swapcode(TYPE, parmi, parmj, n) {  \
    register TYPE *pi = (TYPE *) (parmi);  \
    register TYPE *pj = (TYPE *) (parmj);  \
    do {                                   \
        register TYPE t = *pi;             \
        *pi++ = *pj;                       \
        *pj++ = t;                         \
    } while ((n -= sizeof(TYPE)) > 0);     \
}
#include <stddef.h>
static void swapfunc(char *a, char *b, size_t n, int swaptype)
{   if (swaptype <= 1) swapcode(long, a, b, n)
    else swapcode(char, a, b, n)
}
#define swap(a, b)                         \
    if (swaptype == 0) {                   \
        t = *(long*)(a);                   \
        *(long*)(a) = *(long*)(b);         \
        *(long*)(b) = t;                   \
    } else                                 \
        swapfunc(a, b, es, swaptype)

#define PVINIT(pv, pm)                                \
    if (swaptype != 0) { pv = a; swap(pv, pm); }      \
    else { pv = (char*)&v; *(long*)pv = *(long*)pm; }

#define vecswap(a, b, n) if (n > 0) swapfunc(a, b, n, swaptype)

#define min(x, y) ((x)<=(y) ? (x) : (y))

static char *med3(char *a, char *b, char *c, int (*cmp)())
{	return cmp(a, b) < 0 ?
		  (cmp(b, c) < 0 ? b : cmp(a, c) < 0 ? c : a)
		: (cmp(b, c) > 0 ? b : cmp(a, c) > 0 ? c : a);
}

void qsort(char *a, size_t n, size_t es, int (*cmp)())
{
	char *pa, *pb, *pc, *pd, *pl, *pm, *pn, *pv;
	int r, swaptype;
	long s, t, v;

	SWAPINIT(a, es);
	if (n < 7) {	 /* Insertion sort on smallest arrays */
		for (pm = a + es; pm < a + n*es; pm += es)
			for (pl = pm; pl > a && cmp(pl-es, pl) > 0; pl -= es)
				swap(pl, pl-es);
		return;
	}
	pm = a + (n/2)*es;    /* Small arrays, middle element */
	if (n > 7) {
		pl = a;
		pn = a + (n-1)*es;
		if (n > 40) {    /* Big arrays, pseudomedian of 9 */
			s = (n/8)*es;
			pl = med3(pl, pl+s, pl+2*s, cmp);
			pm = med3(pm-s, pm, pm+s, cmp);
			pn = med3(pn-2*s, pn-s, pn, cmp);
		}
		pm = med3(pl, pm, pn, cmp); /* Mid-size, med of 3 */
	}
	PVINIT(pv, pm);
	pa = pb = a;
	pc = pd = a + (n-1)*es;
	for (;;) {
		while (pb <= pc && (r = cmp(pb, pv)) <= 0) {
			if (r == 0) { swap(pa, pb); pa += es; }
			pb += es;
		}
		while (pb <= pc && (r = cmp(pc, pv)) >= 0) {
			if (r == 0) { swap(pc, pd); pd -= es; }
			pc -= es;
		}
		if (pb > pc) break;
		swap(pb, pc);
		pb += es;
		pc -= es;
	}
	pn = a + n*es;
	s = min(pa-a,  pb-pa   ); vecswap(a,  pb-s, s);
	s = min(pd-pc, pn-pd-es); vecswap(pb, pn-s, s);
	if ((s = pb-pa) > es) qsort(a,    s/es, es, cmp);
	if ((s = pd-pc) > es) qsort(pn-s, s/es, es, cmp);
}
