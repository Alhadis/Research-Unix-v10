/* a4ssox.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

/* Table of constant values */

static integer c__13 = 13;
static integer c__15 = 15;
static integer c__1 = 1;
static integer c__3 = 3;
static integer c__47 = 47;
static integer c__20 = 20;
static integer c__2 = 2;
static integer c__50 = 50;
static integer c__21 = 21;

logical a4ssox_(wv, rv, iv, lv, n, me)
real *wv, *rv;
integer *iv;
logical *lv;
integer *n, *me;
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    real r__1, r__2, r__3;
    logical ret_val;

    /* Local variables */
    static integer temp1, temp2, temp3, temp4, temp5, itemp;
    extern /* Subroutine */ int xtrap_();
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
#define ws ((real *)&cstak_1)
    extern integer istkmd_();
    extern /* Subroutine */ int seterr_();
    extern integer istkgt_();

/* DO THE EXTRAPOLATION. */
    /* Parameter adjustments */
    --n;
    --lv;
    --iv;
    --rv;
    --wv;

    /* Function Body */
    lv[6] = FALSE_;
    lv[7] = FALSE_;
    lv[4] = FALSE_;
    lv[5] = FALSE_;
    ret_val = FALSE_;
    iv[18] = *me;
    temp5 = iv[18];
/* Computing MAX */
    r__2 = dabs(wv[10]), r__3 = dabs(wv[11]);
    if (dmax(r__2,r__3) * (float)10. * rv[11] < (r__1 = wv[11] - wv[10], dabs(
	    r__1)) * rv[4] / (real) n[temp5]) {
	goto L1;
    }
/* /6S */
/*        CALL SETERR(13H ESSOM - DT=0, 13, 15, 1) */
/* /7S */
    seterr_(" ESSOM - DT=0", &c__13, &c__15, &c__1, 13L);
/* / */
    ret_val = TRUE_;
    return ret_val;
L1:
    if (lv[2]) {
	goto L2;
    }
    iv[22] = istkgt_(&iv[1], &c__3);
    ret_val = TRUE_;
    return ret_val;
L2:
    if (min(iv[18],iv[2]) <= iv[20]) {
	goto L3;
    }
    i__1 = iv[1] * min(iv[18],iv[2]);
    itemp = istkmd_(&i__1);
/* EXPAND THE EXTRAPOLATION LOZENGE. */
/* /6S */
/*        IF (IV(13) .LT. ITEMP) CALL SETERR( */
/*    1      47H ESSOM - SOMEBODY IS LEAVING STUFF ON THE STACK, 47, 20 */
/*    2      , 2) */
/*        IF (IV(13) .GT. ITEMP) CALL SETERR( */
/*    1      50H ESSOM - SOMEBODY IS REMOVING STUFF FROM THE STACK, 50, */
/*    2      21, 2) */
/* /7S */
    if (iv[13] < itemp) {
	seterr_(" ESSOM - SOMEBODY IS LEAVING STUFF ON THE STACK", &c__47, &
		c__20, &c__2, 47L);
    }
    if (iv[13] > itemp) {
	seterr_(" ESSOM - SOMEBODY IS REMOVING STUFF FROM THE STACK", &c__50, 
		&c__21, &c__2, 50L);
    }
/* / */
L3:
    iv[20] = max(iv[18],iv[20]);
/* THE BEST ERROR IN THE LOZENGE. */
    iv[22] = istkgt_(&iv[1], &c__3);
/* THE LOZENGE ERROR. */
/* Computing MAX */
/* Computing MIN */
    i__4 = iv[18] - 1;
    i__2 = 1, i__3 = iv[1] * min(i__4,iv[2]);
    i__1 = max(i__2,i__3);
    iv[21] = istkgt_(&i__1, &c__3);
    temp5 = iv[12];
    temp4 = iv[27];
    temp3 = iv[13];
    temp2 = iv[21];
    temp1 = iv[22];
    xtrap_(&ws[temp5 - 1], &iv[18], &iv[1], &ws[temp4 - 1], &iv[2], &lv[1], &
	    ws[temp3 - 1], &rs[temp2 - 1], &rs[temp1 - 1]);
    return ret_val;
} /* a4ssox_ */

#undef ws
#undef rs
#undef ls
#undef is


