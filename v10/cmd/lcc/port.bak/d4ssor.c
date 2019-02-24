/* d4ssor.f -- translated by f2c (version of 5 February 1993  1:37:14).
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
static integer c__2 = 2;

logical d4ssor_(wv, rv, iv, lv, errare)
doublereal *wv;
real *rv;
integer *iv;
logical *lv;
real *errare;
{
    /* System generated locals */
    logical ret_val;

    /* Local variables */
    static integer temp1, temp2, temp3, temp4, temp5, temp6, temp7;
    static real rtemp;
    static integer i1, i2, i3;
    extern logical d4ssod_(), d4ssol_();
    extern /* Subroutine */ int d4ssoo_();
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
#define ws ((doublereal *)&cstak_1)
    extern /* Subroutine */ int seterr_(), istkrl_();
    static integer idx;

/* SEE IF A RESTART IS IN ORDER. */
    /* Parameter adjustments */
    --errare;
    --lv;
    --iv;
    --rv;
    --wv;

    /* Function Body */
    rv[1] = errare[1];
/* UPDATE ERRPAR. */
    rv[2] = errare[2];
    if (! lv[7]) {
	goto L1;
    }
    ret_val = TRUE_;
    return ret_val;
L1:
    ret_val = FALSE_;
/* L2: */
    if (iv[18] <= iv[17]) {
	goto L8;
    }
    lv[5] = TRUE_;
/* SEE IF A RE-START IS NECESSARY. */
    temp6 = iv[21];
    temp5 = iv[22];
    temp4 = iv[10];
    if (! d4ssol_(&rs[temp6 - 1], &rs[temp5 - 1], &iv[1], &iv[18], &iv[2], &
	    rs[temp4 - 1], &rv[10])) {
	goto L3;
    }
    ret_val = TRUE_;
    return ret_val;
/*   IF WILL NOT CONVERGE IN THIS LOZENGE, RESTART. */
L3:
    temp4 = iv[21];
    temp5 = iv[11];
    temp6 = iv[26];
    temp7 = iv[10];
    lv[6] = d4ssod_(&rs[temp4 - 1], &iv[1], &iv[18], &iv[3], &iv[2], &ws[
	    temp5 - 1], &wv[3], &wv[4], &wv[5], &rs[temp6 - 1], &rs[temp7 - 1]
	    , &iv[24], &iv[23], &iv[17]);
    if (! lv[6]) {
	goto L4;
    }
    ret_val = TRUE_;
    return ret_val;
L4:
    if (iv[18] <= iv[17] + 1) {
	goto L7;
    }
    temp7 = iv[21];
/* SEE IF A RE-START WOULD BE MORE EFFICIENT. */
    temp6 = iv[11];
    temp5 = iv[26];
    temp4 = iv[10];
    temp3 = iv[6];
    temp2 = iv[9];
    temp1 = iv[5];
    d4ssoo_(&rs[temp7 - 1], &iv[1], &iv[18], &iv[3], &iv[2], &ws[temp6 - 1], &
	    wv[4], &rs[temp5 - 1], &rs[temp4 - 1], &iv[16], &wv[6], &rs[temp3 
	    - 1], &rs[temp2 - 1], &rs[temp1 - 1], &rv[8], &rv[9], &rv[12]);
    idx = iv[6] + iv[16];
    if (wv[11] + rs[idx - 1] != wv[11]) {
	goto L5;
    }
/* /6S */
/*              CALL SETERR(13HDESSOM - DT=0, 13, 15, 1) */
/* /7S */
    seterr_("DESSOM - DT=0", &c__13, &c__15, &c__1, 13L);
/* / */
    ret_val = TRUE_;
    return ret_val;
/* = SNGL(DT/HOPT(KOPT+1)). */
L5:
    rtemp = wv[6] / rs[idx - 1];
    if (rtemp <= (float)1e3) {
	rtemp = (real) ((integer) (rtemp + (float).99));
    }
/*     IF ( WORK(M) + WORK(KOPT+2)*RTEMP <= WORK(L) ) */
    i1 = iv[8] + iv[18] - 1;
    i2 = iv[8] + iv[16] + 1;
    i3 = iv[8] + iv[24] - 1;
    if (rs[i1 - 1] + rs[i2 - 1] * rtemp > rs[i3 - 1]) {
	goto L6;
    }
    istkrl_(&c__1);
    lv[4] = TRUE_;
    ret_val = TRUE_;
    return ret_val;
L6:
L7:
L8:
    if (iv[18] < iv[3]) {
	istkrl_(&c__2);
    }
    return ret_val;
} /* d4ssor_ */

#undef ws
#undef rs
#undef ls
#undef is


