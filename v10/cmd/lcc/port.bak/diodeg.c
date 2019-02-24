/* diodeg.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal aj, bj;
    logical getjac, neesum;
} diodej_;

#define diodej_1 diodej_

extern struct {
    logical failed;
} diodef_;

#define diodef_1 diodef_

/* Table of constant values */

static doublereal c_b2 = 0.;
static integer c__4 = 4;
static integer c__2 = 2;
static integer c__50 = 50;
static integer c__1 = 1;
static integer c__29 = 29;

logical diodeg_(d, t, v, vt, nv, delv, delvt, d0, nes, fnum, ldim, ldv, ldvt, 
	dv, dvt, getdvt, dummy, ldummy, d1)
/* Subroutine */ int (*d) ();
doublereal *t, *v, *vt;
integer *nv;
doublereal *delv, *delvt, *d0;
integer *nes, *fnum, *ldim, *ldv, *ldvt;
doublereal *dv, *dvt;
logical *getdvt;
doublereal *dummy;
integer *ldummy;
doublereal *d1;
{
    /* Initialized data */

    static doublereal sqrtr = 0.;
    static doublereal big = 0.;

    /* System generated locals */
    integer dv_dim1, dv_offset, dvt_dim1, dvt_offset, i__1, i__2, i__3;
    doublereal d__1;
    logical ret_val;

    /* Builtin functions */
    double sqrt();

    /* Local variables */
    extern /* Subroutine */ int setd_();
    static logical temp;
    static integer temp1, i, j;
    static doublereal delvi, vsave;
    extern doublereal d1mach_();
    static doublereal delvti;
    extern /* Subroutine */ int seterr_();
    static doublereal vtsave;

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
    setd_(ldummy, &c_b2, &dummy[1]);
    if (sqrtr == 0.) {
	sqrtr = sqrt(d1mach_(&c__4));
    }
/* SQRT( MACHINE PRECISION ). */
    if (big == 0.) {
	big = d1mach_(&c__2);
    }
/* THE BIGGEST FP NUMBER. */
/* GET INITIAL D VALUE AND DELV, DELVT TERMS. */
    setd_(nv, &c_b2, &d0[1]);
    diodef_1.failed = FALSE_;
    ++(*nes);
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
	delv[i] = sqrtr * (d__1 = v[i], abs(d__1));
	if (delv[i] == 0.) {
	    delv[i] = sqrtr;
	}
/* L1: */
    }
    if (! (*getdvt)) {
	goto L3;
    }
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
	delvt[i] = sqrtr * (d__1 = vt[i], abs(d__1));
	if (delvt[i] == 0.) {
	    delvt[i] = sqrtr;
	}
/* L2: */
    }
L3:
    (*d)(t, &v[1], &vt[1], nv, &d0[1], &delv[1], &delvt[1]);
    i__1 = *nv;
    for (j = 1; j <= i__1; ++j) {
/* /6S */
/*        IF (DELV(J) .EQ. 0D0) CALL SETERR( */
/*    1      50HDIODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT, 50
, 1 */
/*    2      , 2) */
/* /7S */
	if (delv[j] == 0.) {
	    seterr_("DIODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT", &
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
/*           IF (DELVT(J) .EQ. 0D0) CALL SETERR( */
/*    1         50HDIODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT,
 */
/*    2         50, 1, 2) */
/* /7S */
	if (delvt[j] == 0.) {
	    seterr_("DIODEG - D RETURNED 0 VALUES FOR DELV AND/OR DELVT", &
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
/*           IF (DUMMY(J) .NE. 0D0) CALL SETERR( */
/*    1         29HDIODEG - D USED DV AND/OR DVT, 29, 2, 2) */
/* /7S */
	if (dummy[j] != 0.) {
	    seterr_("DIODEG - D USED DV AND/OR DVT", &c__29, &c__2, &c__2, 
		    29L);
	}
/* / */
/* L7: */
    }
L8:
    if (! diodef_1.failed) {
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
	delvi = (d__1 = delv[i], abs(d__1));
	if (v[i] > 0.) {
	    delvi = -delvi;
	}
/* GUARD AGAINST OVERFLOW. */
	vsave = v[i];
	v[i] += delvi;
	setd_(nv, &c_b2, &d1[1]);
	diodef_1.failed = FALSE_;
	++(*nes);
	(*d)(t, &v[1], &vt[1], nv, &d1[1], &dummy[1], &dummy[1]);
/* Computing MIN */
	i__2 = *ldummy, i__3 = max(*ldv,*ldvt);
	temp1 = min(i__2,i__3);
	i__2 = temp1;
	for (j = 1; j <= i__2; ++j) {
/* /6S */
/*              IF (DUMMY(J) .NE. 0D0) CALL SETERR( */
/*    1            29HDIODEG - D USED DV AND/OR DVT, 29, 2, 2) */
/* /7S */
	    if (dummy[j] != 0.) {
		seterr_("DIODEG - D USED DV AND/OR DVT", &c__29, &c__2, &c__2,
			 29L);
	    }
/* / */
/* L10: */
	}
	if (! diodef_1.failed) {
	    goto L11;
	}
	*fnum = 1;
	ret_val = FALSE_;
	return ret_val;
L11:
	i__2 = *nv;
	for (j = 1; j <= i__2; ++j) {
	    temp = abs(delvi) < 1.;
	    if (temp) {
		temp = (d__1 = d1[j] - d0[j], abs(d__1)) > abs(delvi) * big;
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
	delvti = (d__1 = delvt[i], abs(d__1));
	if (vt[i] > 0.) {
	    delvti = -delvti;
	}
/* GUARD AGAINST OVERFLOW. */
	vtsave = vt[i];
	vt[i] += delvti;
	setd_(nv, &c_b2, &d1[1]);
	diodef_1.failed = FALSE_;
	++(*nes);
	(*d)(t, &v[1], &vt[1], nv, &d1[1], &dummy[1], &dummy[1]);
/* Computing MIN */
	i__2 = *ldummy, i__3 = max(*ldv,*ldvt);
	temp1 = min(i__2,i__3);
	i__2 = temp1;
	for (j = 1; j <= i__2; ++j) {
/* /6S */
/*              IF (DUMMY(J) .NE. 0D0) CALL SETERR( */
/*    1            29HDIODEG - D USED DV AND/OR DVT, 29, 2, 2) */
/* /7S */
	    if (dummy[j] != 0.) {
		seterr_("DIODEG - D USED DV AND/OR DVT", &c__29, &c__2, &c__2,
			 29L);
	    }
/* / */
/* L16: */
	}
	if (! diodef_1.failed) {
	    goto L17;
	}
	*fnum = 1;
	ret_val = FALSE_;
	return ret_val;
L17:
	i__2 = *nv;
	for (j = 1; j <= i__2; ++j) {
	    temp = abs(delvti) < 1.;
	    if (temp) {
		temp = (d__1 = d1[j] - d0[j], abs(d__1)) > abs(delvti) * big;
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
	delvi = (d__1 = delv[i], abs(d__1));
	v[i] += delvi * diodej_1.aj;
	vt[i] += delvi / diodej_1.bj;
	setd_(nv, &c_b2, &d1[1]);
	diodef_1.failed = FALSE_;
	++(*nes);
	(*d)(t, &v[1], &vt[1], nv, &d1[1], &dummy[1], &dummy[1]);
/* Computing MIN */
	i__2 = *ldummy, i__3 = max(*ldv,*ldvt);
	temp1 = min(i__2,i__3);
	i__2 = temp1;
	for (j = 1; j <= i__2; ++j) {
/* /6S */
/*              IF (DUMMY(J) .NE. 0D0) CALL SETERR( */
/*    1            29HDIODEG - D USED DV AND/OR DVT, 29, 2, 2) */
/* /7S */
	    if (dummy[j] != 0.) {
		seterr_("DIODEG - D USED DV AND/OR DVT", &c__29, &c__2, &c__2,
			 29L);
	    }
/* / */
/* L23: */
	}
	if (! diodef_1.failed) {
	    goto L24;
	}
	*fnum = 1;
	ret_val = FALSE_;
	return ret_val;
L24:
	i__2 = *nv;
	for (j = 1; j <= i__2; ++j) {
	    temp = abs(delvi) < 1.;
	    if (temp) {
		temp = (d__1 = d1[j] - d0[j], abs(d__1)) > big * abs(delvi);
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
} /* diodeg_ */

