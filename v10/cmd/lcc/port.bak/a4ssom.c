/* a4ssom.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
static integer c__13 = 13;
static integer c__15 = 15;

logical a4ssom_(wv, rv, iv, lv, dte)
real *wv, *rv;
integer *iv;
logical *lv;
real *dte;
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1, r__2, r__3, r__4, r__5;
    logical ret_val;

    /* Builtin functions */
    double exp(), sqrt();

    /* Local variables */
    static real temp;
    static integer nerr;
    static logical temp1;
    static integer temp2, temp3, temp4, temp5, temp6, temp7, temp8, k, i1, i2,
	     i3;
    extern logical a4ssol_();
    extern /* Subroutine */ int a4ssoo_();
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
#define ws ((real *)&cstak_1)
    extern /* Subroutine */ int seterr_();
    extern integer nerror_();
    extern /* Subroutine */ int istkrl_();
    static integer idx;

/* THE BASIC STEP-SIZE AND ORDER MONITOR. */
    /* Parameter adjustments */
    --lv;
    --iv;
    --rv;
    --wv;

    /* Function Body */
    if (nerror_(&nerr) == 0) {
	goto L1;
    }
    ret_val = TRUE_;
    return ret_val;
L1:
    ret_val = FALSE_;
/* L2: */
    if (iv[3] != 1) {
	goto L3;
    }
    if (lv[2]) {
	istkrl_(&c__1);
    }
    ret_val = FALSE_;
    return ret_val;
L3:
    if (! lv[7]) {
	wv[11] = wv[10];
    }
/* SIGNAL A RESTART. */
    temp1 = wv[10] == wv[11];
    if (temp1) {
	temp1 = wv[10] != wv[1];
    }
    if (! temp1) {
	goto L4;
    }
    iv[25] = iv[14];
    wv[12] = wv[10] + wv[6];
L4:
    temp1 = lv[2];
    if (temp1) {
	temp1 = ! lv[4];
    }
    if (! temp1) {
	goto L7;
    }
    if (lv[5]) {
	goto L6;
    }
    temp6 = iv[21];
/* FIND THE OPTIMAL DT AND M FOR */
/* THE NEXT TIME-STEP. */
    temp7 = iv[22];
    temp8 = iv[10];
    if (! a4ssol_(&rs[temp6 - 1], &rs[temp7 - 1], &iv[1], &iv[18], &iv[2], &
	    rs[temp8 - 1], &rv[10])) {
	goto L5;
    }
    ret_val = TRUE_;
    return ret_val;
L5:
L6:
    temp8 = iv[21];
    temp7 = iv[11];
    temp6 = iv[26];
    temp5 = iv[10];
    temp4 = iv[6];
    temp3 = iv[9];
    temp2 = iv[5];
    a4ssoo_(&rs[temp8 - 1], &iv[1], &iv[18], &iv[3], &iv[2], &ws[temp7 - 1], &
	    wv[4], &rs[temp6 - 1], &rs[temp5 - 1], &iv[16], &wv[6], &rs[temp4 
	    - 1], &rs[temp3 - 1], &rs[temp2 - 1], &rv[8], &rv[9], &rv[12]);
/* RELEASE THE LOZENGE ERROR ESTIMATES E2. */
    istkrl_(&c__1);
L7:
    if (! lv[2]) {
	goto L15;
    }
/* Computing MIN */
    i__1 = iv[16], i__2 = iv[17] + 1;
    iv[16] = min(i__1,i__2);
/* GET THE DT FOR THE NEXT TIME-STEP. */
    if (wv[10] != wv[11]) {
	goto L8;
    }
    iv[28] = min(iv[16],iv[17]);
    goto L9;
L8:
    iv[28] = iv[16];
L9:
    idx = iv[6] + iv[28];
/* ABS(HOPT(KOPTM+1))). */
/* Computing MIN */
    r__2 = dabs(wv[6]) * exp(rv[7]), r__3 = (r__1 = rs[idx - 1], dabs(r__1));
    wv[8] = wv[6] / dabs(wv[6]) * rv[3] * dmin(r__2,r__3);
/*   TWO RESTARTS IN A ROW CAUSE DT TO DECREASE BY AT LEAST A */
/*   FACTOR OF 10**3. */
    temp1 = lv[3];
    if (temp1) {
	temp1 = wv[10] == wv[11];
    }
    if (temp1) {
/* Computing MIN */
	r__1 = dabs(wv[8]), r__2 = dabs(wv[6]) * (float).001;
	wv[8] = wv[6] / dabs(wv[6]) * dmin(r__1,r__2);
    }
/* Computing MIN */
    i__1 = iv[18], i__2 = iv[2] + 1;
    iv[15] = min(i__1,i__2);
/*   COMPUTE THE COST/UNIT TIME-STEP FOR EACH LOZENGE SIZE. */
/* COST(K) = WORK(K+1)/ABS(HOPT(K)). */
    temp2 = iv[15];
    i__1 = temp2;
    for (k = 1; k <= i__1; ++k) {
	i1 = iv[5] + k - 1;
	i2 = iv[8] + k;
	i3 = iv[6] + k - 1;
	if (rs[i3 - 1] != (float)0.) {
	    goto L10;
	}
	rs[i1 - 1] = (float)-1.;
	goto L11;
L10:
	rs[i1 - 1] = rs[i2 - 1] / (r__1 = rs[i3 - 1], dabs(r__1));
L11:
/* L12: */
	;
    }
    temp1 = wv[10] != wv[1];
    if (temp1) {
	temp1 = wv[10] != wv[11];
    }
    if (! temp1) {
	goto L14;
    }
/* Computing MIN */
    i__1 = iv[16] + 1, i__2 = iv[19] - 1, i__1 = min(i__1,i__2), i__2 = iv[18]
	     - 1;
    idx = min(i__1,i__2);
/* SEE IF SHOULD BE CAUTIOUS. */
/*     IF ABS(HOPT(NEW)) < ABS(HOPT(OLD)), BE CONSERVATIVE. */
    i1 = iv[6] + idx - 1;
    i2 = iv[7] + idx - 1;
    if (dabs(wv[8]) > (r__1 = rs[i1 - 1], dabs(r__1)) * (float).01 && (r__2 = 
	    rs[i1 - 1], dabs(r__2)) < (r__3 = rs[i2 - 1], dabs(r__3))) {
/* Computing MAX */
	r__5 = (r__4 = rs[i1 - 1] / rs[i2 - 1], dabs(r__4));
	wv[8] *= dmax(r__5,(float).01);
    }
    if (iv[25] <= 0) {
	goto L13;
    }
    --iv[25];
/* LOGARITHMIC BISECTION. */
    if (iv[25] == 0) {
	rv[7] = rv[5] * (float).5;
    }
    temp = (wv[12] - wv[11]) / wv[6];
    if (temp > (float).99) {
/* Computing MIN */
	r__2 = (r__1 = wv[12] - wv[11], dabs(r__1)) * (float).5, r__3 = dabs(
		wv[8]), r__2 = min(r__2,r__3), r__3 = sqrt(temp) * dabs(wv[6])
		;
	wv[8] = wv[6] / dabs(wv[6]) * dmin(r__2,r__3);
    }
L13:
L14:
L15:
    if (wv[10] == wv[11]) {
	lv[3] = TRUE_;
    }
    wv[9] = wv[6];
    if (lv[2]) {
	wv[6] = wv[8];
    }
    *dte = wv[6];
    if (wv[11] + wv[6] != wv[11]) {
	goto L16;
    }
/* /6S */
/*        CALL SETERR(13H ESSOM - DT=0, 13, 15, 1) */
/* /7S */
    seterr_(" ESSOM - DT=0", &c__13, &c__15, &c__1, 13L);
/* / */
    ret_val = TRUE_;
L16:
    return ret_val;
} /* a4ssom_ */

#undef ws
#undef rs
#undef ls
#undef is


