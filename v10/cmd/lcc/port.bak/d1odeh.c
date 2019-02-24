/* d1odeh.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    integer fnum;
} d1odef_;

#define d1odef_1 d1odef_

extern struct {
    integer njs, nfs, ntss, nsss, nes, nnits, nnds, nnfs, nrs;
} d10der_;

#define d10der_1 d10der_

extern struct {
    doublereal theta;
    real egive;
    integer minit, maxit, keejac, ircs;
} d1odem_;

#define d1odem_1 d1odem_

extern struct {
    doublereal tj, dtj;
    logical getjac, sepate, usengj, usenns, usenfd;
} d1odeg_;

#define d1odeg_1 d1odeg_

extern struct {
    doublereal tgood;
} d10det_;

#define d10det_1 d10det_

/* Subroutine */ int d1odeh_(t0, v0, t1, v1, nv, dt, tstop, ev, ok, handle)
doublereal *t0, *v0, *t1, *v1;
integer *nv;
doublereal *dt, *tstop;
real *ev;
logical *ok;
/* Subroutine */ int (*handle) ();
{
/* OUTPUT FILTER FOR IODES. */
/* SCRATCH SPACE ALLOCATED - S(D1ODEH) = S(HANDLE). */
    /* Parameter adjustments */
    --ev;
    --v1;
    --v0;

    /* Function Body */
    if (*t0 == *t1) {
	goto L1;
    }
    d1odef_1.fnum = 0;
    d10det_1.tgood = *t1;
    goto L2;
L1:
    ++d10der_1.nrs;
L2:
    if (*t0 != *t1 || d1odem_1.keejac != 3) {
	goto L3;
    }
    d1odeg_1.getjac = *t0 != d1odeg_1.tj;
    d1odeg_1.tj = *t0;
L3:
    ++d10der_1.ntss;
    (*handle)(t0, &v0[1], t1, &v1[1], nv, dt, tstop);
    return 0;
} /* d1odeh_ */

