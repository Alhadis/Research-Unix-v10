/* iodeg.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    real aj, bj;
    logical getjac, neesum;
} iodej_;

#define iodej_1 iodej_

extern struct {
    logical failed;
} iodef_;

#define iodef_1 iodef_

/* Table of constant values */

static real c_b2 = (float)0.;
static integer c__4 = 4;
static integer c__2 = 2;
static integer c__50 = 50;
static integer c__1 = 1;
static integer c__29 = 29;

logical iodeg_(d, t, v, vt, nv, delv, delvt, d0, nes, fnum, ldim, ldv, ldvt, 
	dv, dvt, getdvt, dummy, ldummy, d1)
/* Subroutine */ int (*d) ();
real *t, *v, *vt;
integer *nv;
real *delv, *delvt, *d0;
integer *nes, *fnum, *ldim, *ldv, *ldvt;
real *dv, *dvt;
logical *getdvt;
real *dummy;
integer *ldummy;
real *d1;
{
    /* Initialized data */

    static real sqrtr = (float)0.;
    static real big = (float)0.;

    /* System generated locals */
    integer dv_dim1, dv_offset, dvt_dim1, dvt_offset, i__1, i__2, i__3;
    real r__1;
    logical ret_val;

    /* Builtin functions */
    double sqrt();

    /* Local variables */
    static logical temp;
    extern /* Subroutine */ int setr_();
    static integer temp1, i, j;
    static real delvi, vsave;
    extern doublereal r1mach_();
    static real delvti;
    extern /* Subroutine */ int seterr_();
    static real vtsave;

    /* Parameter adjustments */
    --d1;
    --dummy;
    dvt_dim1 = *ldim;
    dvt_offset = dvt_dim1 + 1;
    dvt -= dvt_offset;
    dv_dim1 = *ldim;
    dv_offset = dv_dim1 + 1;
    dv -= dv_offset;
    --d0;
    --delvt;
    --delv;
    --vt;
    --v;

    /* Function Body */
/* THE FINITE-DIFFERENCE JACOBIAN GETTER FOR IODE. */
    setr_(ldummy, &c_b2, &dummy[1]);
    if (sqrtr == (float)0.) {
	sqrtr = sqrt(r1mach_(&c__4));
    }
/* SQRT( MACHINE PRECISION ). */
    if (big == (float)0.) {
	big = r1mach_(&c__2);
    }
/* THE BIGGEST FP NUMBER. */
/* GET INITIAL D VALUE AND DELV, DELVT TERMS. */
    setr_(nv, &c_b2, &d0[1]);
    iodef_1.failed = FALSE_;
    ++(*nes);
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
	delv[i] = sqrtr * (r__1 = v[i], dabs(r__1));
	if (delv[i] == (float)0.) {
	    delv[i] = sqrtr;
	}
/* L1: */
    }
    if (! (*getdvt)) {
	goto L3;
    }
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
	delvt[i] = sqrtr * (r__1 = vt[i], dabs(r__1));
	if (delvt[i] == (float)0.) {
	    delvt[i] = sqrtr;
	}
/* L2: */
    }
L3:
    (*d)(t, &v[1], &vt[1], nv, &d0[1], &delv[1], &delvt[1]);
    i__1 = *nv;
    for (j = 1; j <= i__1; ++j) {
/* /6S */
/*        IF (DELV(J) .EQ. 0.) CALL SETERR( */
/*    1      50H IODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT, 50
, 1 */
/*    2      , 2) */
/* /7S */
	if (delv[j] == (float)0.) {
	    seterr_(" IODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT", &
		    c__50, &c__1, &c__2, 50L);
	}
/* / */
/* L4: */
    }
    if (! (*getdvt)) {
	goto L6;
    }
    i__1 = *nv;
    for (j = 1; j <= i__1; ++j) {
/* /6S */
/*           IF (DELVT(J) .EQ. 0.) CALL SETERR( */
/*    1         50H IODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT,
 */
/*    2         50, 1, 2) */
/* /7S */
	if (delvt[j] == (float)0.) {
	    seterr_(" IODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT", &
		    c__50, &c__1, &c__2, 50L);
	}
/* / */
/* L5: */
    }
    goto L8;
L6:
    temp1 = min(*ldummy,*ldvt);
    i__1 = temp1;
    for (j = 1; j <= i__1; ++j) {
/* /6S */
/*           IF (DUMMY(J) .NE. 0.) CALL SETERR( */
/*    1         29H IODEG - D USED DV AND/OR DVT, 29, 2, 2) */
/* /7S */
	if (dummy[j] != (float)0.) {
	    seterr_(" IODEG - D USED DV AND/OR DVT", &c__29, &c__2, &c__2, 
		    29L);
	}
/* / */
/* L7: */
    }
L8:
    if (! iodef_1.failed) {
	goto L9;
    }
    *fnum = 1;
    ret_val = FALSE_;
    return ret_val;
L9:
    if (! (*getdvt)) {
	goto L22;
    }
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
/* USE FDS TO GET DV AND DVT. */
	delvi = (r__1 = delv[i], dabs(r__1));
	if (v[i] > (float)0.) {
	    delvi = -(doublereal)delvi;
	}
/* GUARD AGAINST OVERFLOW. */
	vsave = v[i];
	v[i] += delvi;
	setr_(nv, &c_b2, &d1[1]);
	iodef_1.failed = FALSE_;
	++(*nes);
	(*d)(t, &v[1], &vt[1], nv, &d1[1], &dummy[1], &dummy[1]);
/* Computing MIN */
	i__2 = *ldummy, i__3 = max(*ldv,*ldvt);
	temp1 = min(i__2,i__3);
	i__2 = temp1;
	for (j = 1; j <= i__2; ++j) {
/* /6S */
/*              IF (DUMMY(J) .NE. 0.) CALL SETERR( */
/*    1            29H IODEG - D USED DV AND/OR DVT, 29, 2, 2) */
/* /7S */
	    if (dummy[j] != (float)0.) {
		seterr_(" IODEG - D USED DV AND/OR DVT", &c__29, &c__2, &c__2,
			 29L);
	    }
/* / */
/* L10: */
	}
	if (! iodef_1.failed) {
	    goto L11;
	}
	*fnum = 1;
	ret_val = FALSE_;
	return ret_val;
L11:
	i__2 = *nv;
	for (j = 1; j <= i__2; ++j) {
	    temp = dabs(delvi) < (float)1.;
	    if (temp) {
		temp = (r__1 = d1[j] - d0[j], dabs(r__1)) > dabs(delvi) * big;
	    }
	    if (! temp) {
		goto L12;
	    }
	    dv[j + i * dv_dim1] = big;
/* OVERFLOW. */
	    goto L13;
L12:
	    dv[j + i * dv_dim1] = (d1[j] - d0[j]) / delvi;
L13:
/* L14: */
	    ;
	}
	v[i] = vsave;
/* L15: */
    }
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
	delvti = (r__1 = delvt[i], dabs(r__1));
	if (vt[i] > (float)0.) {
	    delvti = -(doublereal)delvti;
	}
/* GUARD AGAINST OVERFLOW. */
	vtsave = vt[i];
	vt[i] += delvti;
	setr_(nv, &c_b2, &d1[1]);
	iodef_1.failed = FALSE_;
	++(*nes);
	(*d)(t, &v[1], &vt[1], nv, &d1[1], &dummy[1], &dummy[1]);
/* Computing MIN */
	i__2 = *ldummy, i__3 = max(*ldv,*ldvt);
	temp1 = min(i__2,i__3);
	i__2 = temp1;
	for (j = 1; j <= i__2; ++j) {
/* /6S */
/*              IF (DUMMY(J) .NE. 0.) CALL SETERR( */
/*    1            29H IODEG - D USED DV AND/OR DVT, 29, 2, 2) */
/* /7S */
	    if (dummy[j] != (float)0.) {
		seterr_(" IODEG - D USED DV AND/OR DVT", &c__29, &c__2, &c__2,
			 29L);
	    }
/* / */
/* L16: */
	}
	if (! iodef_1.failed) {
	    goto L17;
	}
	*fnum = 1;
	ret_val = FALSE_;
	return ret_val;
L17:
	i__2 = *nv;
	for (j = 1; j <= i__2; ++j) {
	    temp = dabs(delvti) < (float)1.;
	    if (temp) {
		temp = (r__1 = d1[j] - d0[j], dabs(r__1)) > dabs(delvti) * 
			big;
	    }
	    if (! temp) {
		goto L18;
	    }
	    dvt[j + i * dvt_dim1] = big;
/* OVERFLOW. */
	    goto L19;
L18:
	    dvt[j + i * dvt_dim1] = (d1[j] - d0[j]) / delvti;
L19:
/* L20: */
	    ;
	}
	vt[i] = vtsave;
/* L21: */
    }
    goto L29;
L22:
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
/* NOT GETTING DVT. */
	vsave = v[i];
	vtsave = vt[i];
	delvi = (r__1 = delv[i], dabs(r__1));
	v[i] += delvi * iodej_1.aj;
	vt[i] += delvi / iodej_1.bj;
	setr_(nv, &c_b2, &d1[1]);
	iodef_1.failed = FALSE_;
	++(*nes);
	(*d)(t, &v[1], &vt[1], nv, &d1[1], &dummy[1], &dummy[1]);
/* Computing MIN */
	i__2 = *ldummy, i__3 = max(*ldv,*ldvt);
	temp1 = min(i__2,i__3);
	i__2 = temp1;
	for (j = 1; j <= i__2; ++j) {
/* /6S */
/*              IF (DUMMY(J) .NE. 0.) CALL SETERR( */
/*    1            29H IODEG - D USED DV AND/OR DVT, 29, 2, 2) */
/* /7S */
	    if (dummy[j] != (float)0.) {
		seterr_(" IODEG - D USED DV AND/OR DVT", &c__29, &c__2, &c__2,
			 29L);
	    }
/* / */
/* L23: */
	}
	if (! iodef_1.failed) {
	    goto L24;
	}
	*fnum = 1;
	ret_val = FALSE_;
	return ret_val;
L24:
	i__2 = *nv;
	for (j = 1; j <= i__2; ++j) {
	    temp = dabs(delvi) < (float)1.;
	    if (temp) {
		temp = (r__1 = d1[j] - d0[j], dabs(r__1)) > big * dabs(delvi);
	    }
	    if (! temp) {
		goto L25;
	    }
	    dv[j + i * dv_dim1] = big;
/* OVERFLOW. */
	    goto L26;
L25:
	    dv[j + i * dv_dim1] = (d1[j] - d0[j]) / delvi;
L26:
/* L27: */
	    ;
	}
	v[i] = vsave;
	vt[i] = vtsave;
/* L28: */
    }
L29:
    ret_val = TRUE_;
    return ret_val;
} /* iodeg_ */

