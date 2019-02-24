/* a1odeh.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    integer fnum;
} a1odef_;

#define a1odef_1 a1odef_

extern struct {
    integer njs, nfs, ntss, nsss, nes, nnits, nnds, nnfs, nrs;
} a10der_;

#define a10der_1 a10der_

extern struct {
    real theta, egive;
    integer minit, maxit, keejac, ircs;
} a1odem_;

#define a1odem_1 a1odem_

extern struct {
    real tj, dtj;
    logical getjac, sepate, usengj, usenns, usenfd;
} a1odeg_;

#define a1odeg_1 a1odeg_

extern struct {
    real tgood;
} a10det_;

#define a10det_1 a10det_

/* Subroutine */ int a1odeh_(t0, v0, t1, v1, nv, dt, tstop, ev, ok, handle)
real *t0, *v0, *t1, *v1;
integer *nv;
real *dt, *tstop, *ev;
logical *ok;
/* Subroutine */ int (*handle) ();
{
/* OUTPUT FILTER FOR IODES. */
/* SCRATCH SPACE ALLOCATED - S(A1ODEH) = S(HANDLE). */
    /* Parameter adjustments */
    --ev;
    --v1;
    --v0;

    /* Function Body */
    if (*t0 == *t1) {
	goto L1;
    }
    a1odef_1.fnum = 0;
    a10det_1.tgood = *t1;
    goto L2;
L1:
    ++a10der_1.nrs;
L2:
    if (*t0 != *t1 || a1odem_1.keejac != 3) {
	goto L3;
    }
    a1odeg_1.getjac = *t0 != a1odeg_1.tj;
    a1odeg_1.tj = *t0;
L3:
    ++a10der_1.ntss;
    (*handle)(t0, &v0[1], t1, &v1[1], nv, dt, tstop);
    return 0;
} /* a1odeh_ */

