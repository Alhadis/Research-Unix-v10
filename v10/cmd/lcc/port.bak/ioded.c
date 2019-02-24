/* ioded.f -- translated by f2c (version of 5 February 1993  1:37:14).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

extern struct {
    doublereal ds[500];
} cstak_;

#define cstak_1 cstak_

extern struct {
    integer imem;
} a1odes_;

#define a1odes_1 a1odes_

extern struct {
    real aj, bj;
    logical getacc, neeumc;
} iodej_;

#define iodej_1 iodej_

extern struct {
    logical failed;
} iodef_;

#define iodef_1 iodef_

/* Table of constant values */

static integer c__10 = 10;
static integer c__2 = 2;
static integer c__3 = 3;
static integer c__1 = 1;
static real c_b33 = (float)0.;
static integer c__100 = 100;
static integer c__38 = 38;
static integer c__48 = 48;
static integer c__43 = 43;
static integer c__32 = 32;
static integer c__4 = 4;
static integer c__26 = 26;
static integer c__5 = 5;

logical ioded_(v, vt, nv, t, dt, d, dv, tj, dtj, getjac, sepate, usengj, 
	usenns, usenfd, nes, nfs, fnum, theta, ircs, keejac)
real *v, *vt;
integer *nv;
real *t, *dt;
/* Subroutine */ int (*d) ();
real *dv, *tj, *dtj;
logical *getjac, *sepate, *usengj, *usenns, *usenfd;
integer *nes, *nfs, *fnum;
real *theta;
integer *ircs, *keejac;
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    real r__1, r__2, r__3;
    logical ret_val;

    /* Local variables */
    extern /* Subroutine */ int rcsb_(), rcsc_();
    static integer nerr, idvt;
    static real temp;
    static integer ldvt;
    extern /* Subroutine */ int rcss_(), setr_(), rcsx_();
    static integer temp1, temp2, temp3, temp4;
    static logical temp5, temp6;
    static integer i, j;
    extern logical iodeg_();
    extern /* Subroutine */ int leave_();
    static integer idelv;
    extern /* Subroutine */ int enter_();
    static integer i0, i1;
    static real dummy[100];
    extern /* Subroutine */ int qrqtb_();
#define is ((integer *)&cstak_1)
#define ls ((logical *)&cstak_1)
#define rs ((real *)&cstak_1)
    static logical getact;
#define ws ((real *)&cstak_1)
    extern /* Subroutine */ int erroff_();
    static integer idelvt;
    static logical neesum, getdvt;
    static integer id1, id4[2];
    extern /* Subroutine */ int seterr_();
    extern integer istkgt_(), nerror_();
    extern /* Subroutine */ int istkrl_();
    static integer id43, idv, ldv;
    extern /* Subroutine */ int qrd_();

/* SCRATCH SPACE ALLOCATED - S( IODED) = */
/*     NV**2 + NV*(2*NV+1) + */
/*     MAX ( S(D), 2*NV + NV INTEGER ) */
/* REAL WORDS. */
/* A DUMMY ARRAY FOR VOID DV AND DVT. */
/* THE PORT LIBRARY STACK AND ITS ALIASES. */
    /* Parameter adjustments */
    --dv;
    --vt;
    --v;

    /* Function Body */
/* Computing 2nd power */
    i__1 = *nv;
    ldv = i__1 * i__1;
/* THE LENGTHS OF THE DV AND DVT ARRAYS. */
/* Computing 2nd power */
    i__1 = *nv;
    ldvt = i__1 * i__1;
    if (a1odes_1.imem <= 0) {
	goto L1;
    }
    id4[0] = is[a1odes_1.imem + 2];
    id4[1] = is[a1odes_1.imem + 3];
L1:
    temp5 = *nfs == -1;
    if (temp5) {
	temp5 = *keejac > 1;
    }
    if (temp5) {
	goto L2;
    }
    temp6 = *nfs == -2;
    if (temp6) {
	temp6 = *keejac == 1;
    }
    temp5 = temp6;
L2:
    if (! temp5) {
	goto L3;
    }
    a1odes_1.imem = istkgt_(&c__10, &c__2);
/* INITIALIZE. */
/* D, DIAG, PIVOT, D41, D42, R, CA, RCA, CB AND RCB. */
    i__1 = ldv + *nv * 3 + 1;
    is[a1odes_1.imem - 1] = istkgt_(&i__1, &c__3);
    is[a1odes_1.imem] = is[a1odes_1.imem - 1] + ldv;
    is[a1odes_1.imem + 2] = 0;
    is[a1odes_1.imem + 3] = 0;
    is[a1odes_1.imem + 4] = is[a1odes_1.imem] + *nv;
    is[a1odes_1.imem + 5] = is[a1odes_1.imem + 4] + *nv;
    is[a1odes_1.imem + 7] = is[a1odes_1.imem + 5] + *nv;
    i__1 = (*nv << 1) + 1;
    is[a1odes_1.imem + 1] = istkgt_(&i__1, &c__2);
    is[a1odes_1.imem + 6] = is[a1odes_1.imem + 1] + *nv;
    is[a1odes_1.imem + 8] = is[a1odes_1.imem + 6] + *nv;
    goto L4;
L3:
    if (a1odes_1.imem == 0) {
	a1odes_1.imem = 1;
    }
/* SIGNAL THAT KNOW ABOUT A1ODES. */
L4:
    temp5 = *nfs == -1;
    if (temp5) {
	temp5 = *keejac > 1;
    }
    if (! temp5) {
	goto L5;
    }
    i__1 = ldv + ldvt;
    is[a1odes_1.imem + 2] = istkgt_(&i__1, &c__3);
/* THE JACOBIAN PARTS ARE GLOBAL. */
    is[a1odes_1.imem + 3] = is[a1odes_1.imem + 2] + ldv;
L5:
    if (*nfs >= 0) {
	goto L6;
    }
    ret_val = TRUE_;
    return ret_val;
L6:
    enter_(&c__1);
    neesum = *dtj != *dt;
    neesum = neesum && *sepate;
    if (*keejac != 0) {
	goto L11;
    }
    i__1 = (*nv << 1) + 11;
    a1odes_1.imem = istkgt_(&i__1, &c__2);
/* NEED STACK FRAME. */
    if (*usenns) {
	goto L7;
    }
    i__1 = ldv + ldvt + *nv * 3 + 1;
    id4[0] = istkgt_(&i__1, &c__3);
    id4[1] = id4[0] + ldv;
    goto L8;
L7:
    i__1 = ldv + *nv * 3 + 1;
    id4[0] = istkgt_(&i__1, &c__3);
    id4[1] = 0;
L8:
    is[a1odes_1.imem - 1] = id4[0];
    id43 = id4[0];
    if (*usenns) {
	goto L9;
    }
    is[a1odes_1.imem] = id4[1] + ldvt;
    goto L10;
L9:
    is[a1odes_1.imem] = id4[0] + ldv;
L10:
    is[a1odes_1.imem + 4] = is[a1odes_1.imem] + *nv;
    is[a1odes_1.imem + 5] = is[a1odes_1.imem + 4] + *nv;
    is[a1odes_1.imem + 7] = is[a1odes_1.imem + 5] + *nv;
    is[a1odes_1.imem + 1] = a1odes_1.imem + 10;
    is[a1odes_1.imem + 6] = is[a1odes_1.imem + 1] + *nv;
    is[a1odes_1.imem + 8] = is[a1odes_1.imem + 6] + *nv;
    goto L20;
L11:
    if (*keejac != 1) {
	goto L16;
    }
    if (! (*getjac)) {
	goto L14;
    }
    id4[0] = is[a1odes_1.imem - 1];
    id43 = id4[0];
    if (! (*usenns)) {
	goto L12;
    }
    id4[1] = 0;
    goto L13;
L12:
    id4[1] = istkgt_(&ldvt, &c__3);
L13:
    goto L15;
L14:
    id4[0] = 0;
    id4[1] = 0;
    id43 = 0;
L15:
    goto L19;
L16:
    temp5 = *getjac;
    if (! temp5) {
	temp5 = neesum;
    }
    if (! temp5) {
	goto L17;
    }
    id43 = is[a1odes_1.imem - 1];
    goto L18;
L17:
    id43 = 0;
L18:
L19:
L20:
    neesum = *dt != *dtj;
    neesum = neesum && *sepate;
    temp5 = *getjac;
    if (temp5) {
	temp5 = ! (*sepate);
    }
    neesum = neesum || temp5;
    if (! (*sepate) || ! (*getjac)) {
	goto L27;
    }
    iodej_1.aj = (float)1.;
    iodej_1.bj = (float)1.;
    iodej_1.getacc = *getjac;
    iodej_1.neeumc = FALSE_;
/* DEFAULT VALUES. */
    setr_(nv, &c_b33, &dv[1]);
    temp4 = id4[0];
    setr_(&ldv, &c_b33, &ws[temp4 - 1]);
    temp4 = id4[1];
    setr_(&ldvt, &c_b33, &ws[temp4 - 1]);
    if (*usenfd) {
	goto L21;
    }
    iodef_1.failed = FALSE_;
    ++(*nes);
    temp4 = id4[0];
    temp3 = id4[1];
    (*d)(tj, &v[1], &vt[1], nv, &dv[1], &ws[temp4 - 1], &ws[temp3 - 1]);
    goto L23;
L21:
    iodej_1.getacc = FALSE_;
    i__1 = *nv * 3;
    idelv = istkgt_(&i__1, &c__3);
    idelvt = idelv + *nv;
    id1 = idelvt + *nv;
    temp3 = id4[0];
    temp4 = id4[1];
    if (iodeg_(d, tj, &v[1], &vt[1], nv, &ws[idelv - 1], &ws[idelvt - 1], &dv[
	    1], nes, fnum, nv, &ldv, &ldvt, &ws[temp3 - 1], &ws[temp4 - 1], &
	    c__1, dummy, &c__100, &ws[id1 - 1])) {
	goto L22;
    }
    leave_();
    ret_val = TRUE_;
    return ret_val;
L22:
    istkrl_(&c__1);
L23:
    if (! iodef_1.failed) {
	goto L24;
    }
    *fnum = 1;
    leave_();
    ret_val = TRUE_;
    return ret_val;
L24:
    if (*theta == (float)1.) {
	goto L26;
    }
    temp4 = id4[0];
    temp3 = id4[0] + ldv - 1;
    i__1 = temp3;
    for (i = temp4; i <= i__1; ++i) {
	ws[i - 1] *= *theta;
/* L25: */
    }
L26:
L27:
    getact = *getjac && ! (*sepate);
    temp5 = *usenns;
    if (! temp5) {
	temp5 = *usenfd;
    }
    if (! temp5) {
	goto L28;
    }
    iodej_1.aj = *theta;
    iodej_1.bj = *dt;
    goto L29;
L28:
    iodej_1.aj = (float)1.;
    iodej_1.bj = (float)1.;
L29:
    iodej_1.getacc = getact;
    temp5 = neesum;
    if (temp5) {
	temp5 = *usenns;
    }
    iodej_1.neeumc = temp5;
    if (! (*usenfd)) {
	goto L30;
    }
    getdvt = *keejac > 1;
    goto L31;
L30:
    getdvt = TRUE_;
L31:
    setr_(nv, &c_b33, &dv[1]);
    if (! getact) {
	goto L43;
    }
    setr_(&ldv, &c_b33, &ws[id43 - 1]);
    if (id4[1] <= 0) {
	goto L32;
    }
    temp3 = id4[1];
    setr_(&ldvt, &c_b33, &ws[temp3 - 1]);
    goto L33;
L32:
    i__1 = min(100,ldvt);
    setr_(&i__1, &c_b33, dummy);
L33:
    if (*sepate) {
	iodej_1.neeumc = FALSE_;
    }
    iodef_1.failed = FALSE_;
    if (id4[1] <= 0) {
	goto L37;
    }
    if (*usenfd) {
	goto L34;
    }
    ++(*nes);
    temp3 = id4[1];
    (*d)(t, &v[1], &vt[1], nv, &dv[1], &ws[id43 - 1], &ws[temp3 - 1]);
    goto L36;
L34:
    iodej_1.getacc = FALSE_;
    i__1 = *nv * 3;
    idelv = istkgt_(&i__1, &c__3);
    idelvt = idelv + *nv;
    id1 = idelvt + *nv;
    temp3 = id4[1];
    if (iodeg_(d, t, &v[1], &vt[1], nv, &ws[idelv - 1], &ws[idelvt - 1], &dv[
	    1], nes, fnum, nv, &ldv, &ldvt, &ws[id43 - 1], &ws[temp3 - 1], &
	    getdvt, dummy, &c__100, &ws[id1 - 1])) {
	goto L35;
    }
    leave_();
    ret_val = TRUE_;
    return ret_val;
L35:
    istkrl_(&c__1);
L36:
    goto L42;
L37:
    if (*usenfd) {
	goto L39;
    }
    ++(*nes);
    (*d)(t, &v[1], &vt[1], nv, &dv[1], &ws[id43 - 1], dummy);
    temp3 = min(100,ldvt);
    i__1 = temp3;
    for (i = 1; i <= i__1; ++i) {
/* /6S */
/*                 IF (DUMMY(I) .NE. 0.) CALL SETERR( */
/*    1               38H IODED - D USED DVT WHEN USENNS = TRUE, 38, 1
, 2 */
/*    2               ) */
/* /7S */
	if (dummy[i - 1] != (float)0.) {
	    seterr_(" IODED - D USED DVT WHEN USENNS = TRUE", &c__38, &c__1, &
		    c__2, 38L);
	}
/* / */
/* L38: */
    }
    goto L41;
L39:
    iodej_1.getacc = FALSE_;
    i__1 = *nv << 1;
    idelv = istkgt_(&i__1, &c__3);
    id1 = idelv + *nv;
    if (iodeg_(d, t, &v[1], &vt[1], nv, &ws[idelv - 1], dummy, &dv[1], nes, 
	    fnum, nv, &ldv, &ldvt, &ws[id43 - 1], dummy, &getdvt, dummy, &
	    c__100, &ws[id1 - 1])) {
	goto L40;
    }
    leave_();
    ret_val = TRUE_;
    return ret_val;
L40:
L41:
L42:
    goto L51;
L43:
    if (! (*usengj)) {
	goto L44;
    }
    idv = 0;
    idvt = 0;
/* Computing MIN */
    i__2 = 100, i__3 = max(ldv,ldvt);
    i__1 = min(i__2,i__3);
    setr_(&i__1, &c_b33, dummy);
    goto L47;
L44:
    if (*usenns) {
	goto L45;
    }
    i__1 = ldv + ldvt;
    idv = istkgt_(&i__1, &c__3);
    idvt = idv + ldv;
/* DEFAULT VALUES. */
    i__1 = ldv + ldvt;
    setr_(&i__1, &c_b33, &ws[idv - 1]);
    goto L46;
L45:
    i__1 = max(ldv,ldvt);
    idv = istkgt_(&i__1, &c__3);
    idvt = idv;
/* DEFAULT VALUES. */
    i__1 = max(ldv,ldvt);
    setr_(&i__1, &c_b33, &ws[idv - 1]);
L46:
L47:
    iodef_1.failed = FALSE_;
    ++(*nes);
    if (idv <= 0) {
	goto L48;
    }
    (*d)(t, &v[1], &vt[1], nv, &dv[1], &ws[idv - 1], &ws[idvt - 1]);
    goto L50;
L48:
    (*d)(t, &v[1], &vt[1], nv, &dv[1], dummy, dummy);
/* Computing MIN */
    i__1 = 100, i__2 = max(ldv,ldvt);
    temp3 = min(i__1,i__2);
    i__1 = temp3;
    for (i = 1; i <= i__1; ++i) {
/* /6S */
/*              IF (DUMMY(I) .NE. 0.) CALL SETERR( */
/*    1            48H IODED - D USED DV AND/OR DVT WHEN USENGJ = TRUE
, */
/*    2            48, 2, 2) */
/* /7S */
	if (dummy[i - 1] != (float)0.) {
	    seterr_(" IODED - D USED DV AND/OR DVT WHEN USENGJ = TRUE", &
		    c__48, &c__2, &c__2, 48L);
	}
/* / */
/* L49: */
    }
L50:
    if (! (*usengj)) {
	istkrl_(&c__1);
    }
L51:
    if (! iodef_1.failed) {
	goto L52;
    }
    *fnum = 1;
    leave_();
    ret_val = TRUE_;
    return ret_val;
L52:
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
	dv[i] = -(doublereal)dv[i];
/* L53: */
    }
    temp5 = getact;
    if (temp5) {
	temp5 = ! (*usenns);
    }
    if (temp5) {
	temp5 = ! (*usenfd);
    }
    if (temp5) {
	temp5 = *theta != (float)1.;
    }
    if (! temp5) {
	goto L55;
    }
    temp3 = id43 + ldv - 1;
    i__1 = temp3;
    for (i = id43; i <= i__1; ++i) {
	ws[i - 1] *= *theta;
/* L54: */
    }
L55:
    temp5 = neesum;
    if (! temp5) {
	goto L56;
    }
    temp6 = ! (*usenns);
    if (temp6) {
	temp6 = ! (*usenfd);
    }
    if (! temp6) {
	temp6 = *sepate;
    }
    temp5 = temp6;
L56:
    if (! temp5) {
	goto L58;
    }
    i0 = id4[0];
    i1 = id4[1];
    temp3 = id43 + ldv - 1;
    i__1 = temp3;
    for (i = id43; i <= i__1; ++i) {
	ws[i - 1] = ws[i0 - 1] + ws[i1 - 1] / *dt;
	++i0;
	++i1;
/* L57: */
    }
L58:
    if (! neesum && ! (*getjac)) {
	goto L64;
    }
    ++(*nfs);
    if (*ircs != 1) {
	goto L62;
    }
    i__1 = *nv;
    for (i = 1; i <= i__1; ++i) {
/* SCALE THE JACOBIAN. */
/* FIRST, GET THE ROW SCALE FACTORS. */
	temp = (float)0.;
	temp3 = is[a1odes_1.imem - 1] + i - 1;
/* Computing 2nd power */
	i__2 = *nv;
	temp4 = is[a1odes_1.imem - 1] + i__2 * i__2 - 1;
	i__2 = temp4;
	i__3 = *nv;
	for (j = temp3; i__3 < 0 ? j >= i__2 : j <= i__2; j += i__3) {
/* Computing MAX */
	    r__2 = temp, r__3 = (r__1 = ws[j - 1], dabs(r__1));
	    temp = dmax(r__2,r__3);
/* L59: */
	}
	temp4 = is[a1odes_1.imem + 4] + i;
	ws[temp4 - 2] = temp;
/* L60: */
    }
/* COLUMN SCALE FACTORS. */
    temp4 = is[a1odes_1.imem - 1];
    temp3 = is[a1odes_1.imem + 4];
    temp2 = is[a1odes_1.imem + 5];
    temp1 = is[a1odes_1.imem + 6];
    rcsc_(&ws[temp4 - 1], nv, nv, &ws[temp3 - 1], &ws[temp2 - 1], &is[temp1 - 
	    1]);
/* SCALE THE JACBOBIAN. */
    temp1 = is[a1odes_1.imem - 1];
    temp2 = is[a1odes_1.imem + 4];
    temp3 = is[a1odes_1.imem + 5];
    temp4 = is[a1odes_1.imem + 6];
    rcss_(&ws[temp1 - 1], nv, nv, &ws[temp2 - 1], &ws[temp3 - 1], &is[temp4 - 
	    1]);
    if (nerror_(&nerr) == 0) {
	goto L61;
    }
    erroff_();
/* /6S */
/*              CALL SETERR( */
/*    1            43H IODED - MUST HAVE 1/(S*L) IN MACHINE RANGE, 43, 3, 
*/
/*    2            2) */
/* /7S */
    seterr_(" IODED - MUST HAVE 1/(S*L) IN MACHINE RANGE", &c__43, &c__3, &
	    c__2, 43L);
/* / */
L61:
/* FACTOR JACOBIAN. */
L62:
    temp4 = is[a1odes_1.imem - 1];
    temp3 = is[a1odes_1.imem];
    temp2 = is[a1odes_1.imem + 1];
    qrd_(nv, nv, &ws[temp4 - 1], &ws[temp3 - 1], &is[temp2 - 1]);
    if (nerror_(&nerr) == 0) {
	goto L63;
    }
    erroff_();
    *fnum = 2;
    leave_();
    ret_val = TRUE_;
    return ret_val;
L63:
L64:
    if (*ircs != 1) {
	goto L65;
    }
    temp2 = is[a1odes_1.imem + 4];
/* SCALE FACTORS FOR THE RHS. */
    temp3 = is[a1odes_1.imem + 7];
    temp4 = is[a1odes_1.imem + 8];
    rcsc_(&dv[1], nv, &c__1, &ws[temp2 - 1], &ws[temp3 - 1], &is[temp4 - 1]);
/* SCALE THE RHS. */
    temp4 = is[a1odes_1.imem + 4];
    temp3 = is[a1odes_1.imem + 7];
    temp2 = is[a1odes_1.imem + 8];
    rcsb_(&dv[1], nv, &c__1, &ws[temp4 - 1], &ws[temp3 - 1], &is[temp2 - 1]);
/* SOLVE JACOBIAN * DV = RHS. */
L65:
    temp2 = is[a1odes_1.imem - 1];
    temp3 = is[a1odes_1.imem];
    temp4 = is[a1odes_1.imem + 1];
    qrqtb_(nv, nv, &ws[temp2 - 1], &ws[temp3 - 1], &is[temp4 - 1], &c__1, &dv[
	    1], &dv[1]);
/* /6S */
/*     IF (NERROR(NERR) .NE. 0) CALL SETERR( */
/*    1   32H IODED - SINGULAR DJAC IN  QRQTB, 32, 4, 2) */
/* /7S */
    if (nerror_(&nerr) != 0) {
	seterr_(" IODED - SINGULAR DJAC IN  QRQTB", &c__32, &c__4, &c__2, 32L)
		;
    }
/* / */
    if (*ircs != 1) {
	goto L68;
    }
    temp4 = is[a1odes_1.imem + 5];
/* UN-SCALE THE SOLUTION. */
    temp3 = is[a1odes_1.imem + 6];
    temp2 = is[a1odes_1.imem + 7];
    temp1 = is[a1odes_1.imem + 8];
    rcsx_(&dv[1], nv, &c__1, &ws[temp4 - 1], &is[temp3 - 1], &ws[temp2 - 1], &
	    is[temp1 - 1]);
    if (nerror_(&nerr) == 0) {
	goto L67;
    }
    if (nerr != 8) {
	goto L66;
    }
    erroff_();
/* /6S */
/*              CALL SETERR(26H IODED - DV HAS OVERFLOWED, 26, 5, 2) */
/* /7S */
    seterr_(" IODED - DV HAS OVERFLOWED", &c__26, &c__5, &c__2, 26L);
/* / */
L66:
L67:
L68:
    leave_();
    ret_val = FALSE_;
    return ret_val;
} /* ioded_ */

#undef ws
#undef rs
#undef ls
#undef is


