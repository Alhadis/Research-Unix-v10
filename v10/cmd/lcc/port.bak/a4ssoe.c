/* a4ssoe.f -- translated by f2c (version of 5 February 1993  1:37:14).
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

static integer c__1 = 1;
static integer c__30 = 30;
static integer c__16 = 16;
static integer c__47 = 47;
static integer c__17 = 17;
static integer c__42 = 42;
static integer c__18 = 18;
static integer c__13 = 13;
static integer c__15 = 15;

logical a4ssoe_(wv, rv, iv, lv, x, tstope, dte)
real *wv, *rv;
integer *iv;
logical *lv;
real *x, *tstope, *dte;
{
    /* System generated locals */
    integer i__1;
    real r__1;
    logical ret_val;

    /* Local variables */
    static logical temp1;
    static integer temp2, i, k, i1, i2;
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
#define ws ((real *)&cstak_1)
    extern /* Subroutine */ int seterr_(), istkrl_();

/* WIND THINGS UP AT THE END OF THE TIME-STEP. */
    /* Parameter adjustments */
    --x;
    --lv;
    --iv;
    --rv;
    --wv;

    /* Function Body */
    wv[2] = *tstope;
    wv[6] = *dte;
    ret_val = FALSE_;
/* RELEASE E. */
    istkrl_(&c__1);
    if (wv[11] + wv[6] != wv[11]) {
	goto L1;
    }
/* /6S */
/*        CALL SETERR(30H ESSOM - DT=0 RETURNED BY SOUT, 30, 16, 1) */
/* /7S */
    seterr_(" ESSOM - DT=0 RETURNED BY SOUT", &c__30, &c__16, &c__1, 30L);
/* / */
    ret_val = TRUE_;
    return ret_val;
L1:
    if (wv[6] / dabs(wv[6]) * (wv[2] - wv[11]) >= (float)0.) {
	goto L2;
    }
/* /6S */
/*        CALL SETERR(47H ESSOM - DT RETURNED BY SOUT HAS THE WRONG SIGN, 
*/
/*    1      47, 17, 1) */
/* /7S */
    seterr_(" ESSOM - DT RETURNED BY SOUT HAS THE WRONG SIGN", &c__47, &c__17,
	     &c__1, 47L);
/* / */
    ret_val = TRUE_;
    return ret_val;
L2:
    if (! lv[2]) {
	goto L9;
    }
    if (wv[10] != wv[11]) {
	goto L3;
    }
    iv[17] = min(iv[16],iv[17]);
    goto L8;
L3:
    temp2 = iv[1];
/* CONVERGED, SO UPDATE X0, HOPT, HUP, ETC. */
    i__1 = temp2;
    for (i = 1; i <= i__1; ++i) {
	i1 = iv[12] + i - 1;
	x[i] = ws[i1 - 1];
/* L4: */
    }
/* X=X1. */
    k = 1;
    goto L6;
L5:
    ++k;
L6:
    if (k > iv[15]) {
	goto L7;
    }
/* HOPTO = HOPT. */
    i1 = iv[7] + k - 1;
    i2 = iv[6] + k - 1;
    rs[i1 - 1] = rs[i2 - 1];
    goto L5;
L7:
/* Computing MIN */
    r__1 = rv[7] * (float)2.;
    rv[7] = dmin(r__1,rv[6]);
    iv[19] = iv[18];
    iv[17] = iv[16];
    lv[3] = FALSE_;
L8:
    goto L12;
L9:
    if (dabs(wv[9]) >= dabs(wv[6])) {
	goto L10;
    }
/* /6S */
/*           CALL SETERR(42H ESSOM - DT RAISED BY SOUT WHEN OK = FALSE, */
/*    1         42, 18, 1) */
/* /7S */
    seterr_(" ESSOM - DT RAISED BY SOUT WHEN OK = FALSE", &c__42, &c__18, &
	    c__1, 42L);
/* / */
    ret_val = TRUE_;
    return ret_val;
/*   THE DEFAULT RESPONSE IS TO LOWER DT BY 10**3. */
L10:
    if (dabs(wv[6]) == dabs(wv[9])) {
	wv[6] /= (float)1e3;
    }
    if (wv[11] + wv[6] != wv[11]) {
	goto L11;
    }
/* /6S */
/*           CALL SETERR(13H ESSOM - DT=0, 13, 15, 1) */
/* /7S */
    seterr_(" ESSOM - DT=0", &c__13, &c__15, &c__1, 13L);
/* / */
    ret_val = TRUE_;
    return ret_val;
L11:
L12:
    wv[10] = wv[11];
    wv[11] = wv[10] + wv[6];
    temp1 = wv[6] / dabs(wv[6]) * (wv[11] - wv[2]) >= (float)0.;
    if (! temp1) {
	temp1 = (r__1 = wv[11] - wv[2], dabs(r__1)) <= wv[7] * dabs(wv[6]);
    }
    if (temp1) {
	wv[11] = wv[2];
    }
    if (wv[10] != wv[2]) {
	wv[6] = wv[11] - wv[10];
    }
    *dte = wv[6];
    if (wv[11] + wv[6] != wv[11]) {
	goto L13;
    }
/* /6S */
/*        CALL SETERR(13H ESSOM - DT=0, 13, 15, 1) */
/* /7S */
    seterr_(" ESSOM - DT=0", &c__13, &c__15, &c__1, 13L);
/* / */
    ret_val = TRUE_;
L13:
    return ret_val;
} /* a4ssoe_ */

#undef ws
#undef rs
#undef ls
#undef is


