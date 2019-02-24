./ ADD NAME=Version.c TIME=699256367
static char junk[] = "\n@(#)LIBF77 VERSION 2.01 28 Feb. 1992\n";

/*
2.00	11 June 1980.  File version.c added to library.
2.01	31 May 1988.  s_paus() flushes stderr; names of hl_* fixed
	[ d]erf[c ] added
	 8 Aug. 1989: #ifdefs for f2c -i2 added to s_cat.c
	29 Nov. 1989: s_cmp returns long (for f2c)
	30 Nov. 1989: arg types from f2c.h
	12 Dec. 1989: s_rnge allows long names
	19 Dec. 1989: getenv_ allows unsorted environment
	28 Mar. 1990: add exit(0) to end of main()
	 2 Oct. 1990: test signal(...) == SIG_IGN rather than & 01 in main
	17 Oct. 1990: abort() calls changed to sig_die(...,1)
	22 Oct. 1990: separate sig_die from main
	25 Apr. 1991: minor, theoretically invisible tweaks to s_cat, sig_die
	31 May  1991: make system_ return status
	18 Dec. 1991: change long to ftnlen (for -i2) many places
	28 Feb. 1992: repair z_sqrt.c (scribbled on input, gave wrong answer)
*/
./ ADD NAME=abort_.c TIME=708871716
#include "stdio.h"
#include "f2c.h"

#ifdef KR_headers
extern VOID sig_die();

VOID abort_()
#else
extern void sig_die(char*,int);

void abort_(void)
#endif
{
sig_die("Fortran abort routine called", 1);
}
./ ADD NAME=c_abs.c TIME=708960773
#include "f2c.h"

#ifdef KR_headers
extern double Cabs();

double c_abs(z) complex *z;
#else
extern double Cabs(double, double);

double c_abs(complex *z)
#endif
{
return( Cabs( z->r, z->i ) );
}
./ ADD NAME=c_cos.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
extern double sin(), cos(), sinh(), cosh();

VOID c_cos(r, z) complex *r, *z;
#else
#undef abs
#include "math.h"

void c_cos(complex *r, complex *z)
#endif
{
r->r = cos(z->r) * cosh(z->i);
r->i = - sin(z->r) * sinh(z->i);
}
./ ADD NAME=c_div.c TIME=708872168
#include "f2c.h"

#ifdef KR_headers
extern VOID sig_die();
VOID c_div(c, a, b)
complex *a, *b, *c;
#else
extern void sig_die(char*,int);
void c_div(complex *c, complex *a, complex *b)
#endif
{
double ratio, den;
double abr, abi;

if( (abr = b->r) < 0.)
	abr = - abr;
if( (abi = b->i) < 0.)
	abi = - abi;
if( abr <= abi )
	{
	if(abi == 0)
		sig_die("complex division by zero", 1);
	ratio = (double)b->r / b->i ;
	den = b->i * (1 + ratio*ratio);
	c->r = (a->r*ratio + a->i) / den;
	c->i = (a->i*ratio - a->r) / den;
	}

else
	{
	ratio = (double)b->i / b->r ;
	den = b->r * (1 + ratio*ratio);
	c->r = (a->r + a->i*ratio) / den;
	c->i = (a->i - a->r*ratio) / den;
	}
}
./ ADD NAME=c_exp.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
extern double exp(), cos(), sin();

 VOID c_exp(r, z) complex *r, *z;
#else
#undef abs
#include "math.h"

void c_exp(complex *r, complex *z)
#endif
{
double expx;

expx = exp(z->r);
r->r = expx * cos(z->i);
r->i = expx * sin(z->i);
}
./ ADD NAME=c_log.c TIME=708960882
#include "f2c.h"

#ifdef KR_headers
extern double log(), Cabs(), atan2();
VOID c_log(r, z) complex *r, *z;
#else
#undef abs
#include "math.h"
extern double Cabs(double, double);

void c_log(complex *r, complex *z)
#endif
{
r->i = atan2(z->i, z->r);
r->r = log( Cabs(z->r, z->i) );
}
./ ADD NAME=c_sin.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
extern double sin(), cos(), sinh(), cosh();

VOID c_sin(r, z) complex *r, *z;
#else
#undef abs
#include "math.h"

void c_sin(complex *r, complex *z)
#endif
{
r->r = sin(z->r) * cosh(z->i);
r->i = cos(z->r) * sinh(z->i);
}
./ ADD NAME=c_sqrt.c TIME=708960905
#include "f2c.h"

#ifdef KR_headers
extern double sqrt(), Cabs();

VOID c_sqrt(r, z) complex *r, *z;
#else
#undef abs
#include "math.h"
extern double Cabs(double, double);

void c_sqrt(complex *r, complex *z)
#endif
{
double mag, t;

if( (mag = Cabs(z->r, z->i)) == 0.)
	r->r = r->i = 0.;
else if(z->r > 0)
	{
	r->r = t = sqrt(0.5 * (mag + z->r) );
	t = z->i / t;
	r->i = 0.5 * t;
	}
else
	{
	t = sqrt(0.5 * (mag - z->r) );
	if(z->i < 0)
		t = -t;
	r->i = t;
	t = z->i / t;
	r->r = 0.5 * t;
	}
}
./ ADD NAME=cabs.c TIME=708960702
#ifdef KR_headers
extern double sqrt();
double Cabs(real, imag) double real, imag;
#else
#undef abs
#include "math.h"
double Cabs(double real, double imag)
#endif
{
double temp;

if(real < 0)
	real = -real;
if(imag < 0)
	imag = -imag;
if(imag > real){
	temp = real;
	real = imag;
	imag = temp;
}
if((real+imag) == real)
	return(real);

temp = imag/real;
temp = real*sqrt(1.0 + temp*temp);  /*overflow!!*/
return(temp);
}
./ ADD NAME=d_abs.c TIME=708873064
#include "f2c.h"

#ifdef KR_headers
double d_abs(x) doublereal *x;
#else
double d_abs(doublereal *x)
#endif
{
if(*x >= 0)
	return(*x);
return(- *x);
}
./ ADD NAME=d_acos.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
double acos();
double d_acos(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_acos(doublereal *x)
#endif
{
return( acos(*x) );
}
./ ADD NAME=d_asin.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
double asin();
double d_asin(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_asin(doublereal *x)
#endif
{
return( asin(*x) );
}
./ ADD NAME=d_atan.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
double atan();
double d_atan(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_atan(doublereal *x)
#endif
{
return( atan(*x) );
}
./ ADD NAME=d_atn2.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
double atan2();
double d_atn2(x,y) doublereal *x, *y;
#else
#undef abs
#include "math.h"
double d_atn2(doublereal *x, doublereal *y)
#endif
{
return( atan2(*x,*y) );
}
./ ADD NAME=d_cnjg.c TIME=708873825
#include "f2c.h"

 VOID
#ifdef KR_headers
d_cnjg(r, z) doublecomplex *r, *z;
#else
d_cnjg(doublecomplex *r, doublecomplex *z)
#endif
{
r->r = z->r;
r->i = - z->i;
}
./ ADD NAME=d_cos.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
double cos();
double d_cos(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_cos(doublereal *x)
#endif
{
return( cos(*x) );
}
./ ADD NAME=d_cosh.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
double cosh();
double d_cosh(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_cosh(doublereal *x)
#endif
{
return( cosh(*x) );
}
./ ADD NAME=d_dim.c TIME=708874152
#include "f2c.h"

#ifdef KR_headers
double d_dim(a,b) doublereal *a, *b;
#else
double d_dim(doublereal *a, doublereal *b)
#endif
{
return( *a > *b ? *a - *b : 0);
}
./ ADD NAME=d_exp.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
double exp();
double d_exp(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_exp(doublereal *x)
#endif
{
return( exp(*x) );
}
./ ADD NAME=d_imag.c TIME=708874254
#include "f2c.h"

#ifdef KR_headers
double d_imag(z) doublecomplex *z;
#else
double d_imag(doublecomplex *z)
#endif
{
return(z->i);
}
./ ADD NAME=d_int.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
double floor();
double d_int(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_int(doublereal *x)
#endif
{
return( (*x>0) ? floor(*x) : -floor(- *x) );
}
./ ADD NAME=d_lg10.c TIME=708889712
#include "f2c.h"

#define log10e 0.43429448190325182765

#ifdef KR_headers
double log();
double d_lg10(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_lg10(doublereal *x)
#endif
{
return( log10e * log(*x) );
}
./ ADD NAME=d_log.c TIME=708889712
#include "f2c.h"

#ifdef KR_headers
double log();
double d_log(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_log(doublereal *x)
#endif
{
return( log(*x) );
}
./ ADD NAME=d_mod.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
#ifdef IEEE_drem
double drem();
#else
double floor();
#endif
double d_mod(x,y) doublereal *x, *y;
#else
#ifdef IEEE_drem
double drem(double, double);
#else
#undef abs
#include "math.h"
#endif
double d_mod(doublereal *x, doublereal *y)
#endif
{
#ifdef IEEE_drem
	double xa, ya, z;
	if ((ya = *y) < 0.)
		ya = -ya;
	z = drem(xa = *x, ya);
	if (xa > 0) {
		if (z < 0)
			z += ya;
		}
	else if (z > 0)
		z -= ya;
	return z;
#else
	double quotient;
	if( (quotient = *x / *y) >= 0)
		quotient = floor(quotient);
	else
		quotient = -floor(-quotient);
	return(*x - (*y) * quotient );
#endif
}
./ ADD NAME=d_nint.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double floor();
double d_nint(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_nint(doublereal *x)
#endif
{
return( (*x)>=0 ?
	floor(*x + .5) : -floor(.5 - *x) );
}
./ ADD NAME=d_prod.c TIME=708874891
#include "f2c.h"

#ifdef KR_headers
double d_prod(x,y) real *x, *y;
#else
double d_prod(real *x, real *y)
#endif
{
return( (*x) * (*y) );
}
./ ADD NAME=d_sign.c TIME=708874925
#include "f2c.h"

#ifdef KR_headers
double d_sign(a,b) doublereal *a, *b;
#else
double d_sign(doublereal *a, doublereal *b)
#endif
{
double x;
x = (*a >= 0 ? *a : - *a);
return( *b >= 0 ? x : -x);
}
./ ADD NAME=d_sin.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double sin();
double d_sin(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_sin(doublereal *x)
#endif
{
return( sin(*x) );
}
./ ADD NAME=d_sinh.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double sinh();
double d_sinh(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_sinh(doublereal *x)
#endif
{
return( sinh(*x) );
}
./ ADD NAME=d_sqrt.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double sqrt();
double d_sqrt(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_sqrt(doublereal *x)
#endif
{
return( sqrt(*x) );
}
./ ADD NAME=d_tan.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double tan();
double d_tan(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_tan(doublereal *x)
#endif
{
return( tan(*x) );
}
./ ADD NAME=d_tanh.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double tanh();
double d_tanh(x) doublereal *x;
#else
#undef abs
#include "math.h"
double d_tanh(doublereal *x)
#endif
{
return( tanh(*x) );
}
./ ADD NAME=derf_.c TIME=708875112
#include "f2c.h"

#ifdef KR_headers
double erf();
double derf_(x) doublereal *x;
#else
extern double erf(double);
double derf_(doublereal *x)
#endif
{
return( erf(*x) );
}
./ ADD NAME=derfc_.c TIME=708873008
#include "f2c.h"

#ifdef KR_headers
extern double erfc();

double derfc_(x) doublereal *x;
#else
extern double erfc(double);

double derfc_(doublereal *x)
#endif
{
return( erfc(*x) );
}
./ ADD NAME=ef1asc_.c TIME=708875832
/* EFL support routine to copy string b to string a */

#include "f2c.h"


#define M	( (long) (sizeof(long) - 1) )
#define EVEN(x)	( ( (x)+ M) & (~M) )

#ifdef KR_headers
extern VOID s_copy();
VOID ef1asc_(a, la, b, lb) ftnint *a, *b; ftnlen *la, *lb;
#else
extern void s_copy(char*,char*,ftnlen,ftnlen);
void ef1asc_(ftnint *a, ftnlen *la, ftnint *b, ftnlen *lb)
#endif
{
s_copy( (char *)a, (char *)b, EVEN(*la), *lb );
}
./ ADD NAME=ef1cmc_.c TIME=708876086
/* EFL support routine to compare two character strings */

#include "f2c.h"

#ifdef KR_headers
extern integer s_cmp();
integer ef1cmc_(a, la, b, lb) ftnint *a, *b; ftnlen *la, *lb;
#else
extern integer s_cmp(char*,char*,ftnlen,ftnlen);
integer ef1cmc_(ftnint *a, ftnlen *la, ftnint *b, ftnlen *lb)
#endif
{
return( s_cmp( (char *)a, (char *)b, *la, *lb) );
}
./ ADD NAME=erf_.c TIME=708875594
#include "f2c.h"

#ifdef KR_headers
double erf();
double erf_(x) real *x;
#else
extern double erf(double);
double erf_(real *x)
#endif
{
return( erf(*x) );
}
./ ADD NAME=erfc_.c TIME=708875664
#include "f2c.h"

#ifdef KR_headers
double erfc();
double erfc_(x) real *x;
#else
extern double erfc(double);
double erfc_(real *x)
#endif
{
return( erfc(*x) );
}
./ ADD NAME=f2c.h TIME=708964532
/* f2c.h  --  Standard Fortran to C header file */

/**  barf  [ba:rf]  2.  "He suggested using FORTRAN, and everybody barfed."

	- From The Shogakukan DICTIONARY OF NEW ENGLISH (Second edition) */

#ifndef F2C_INCLUDE
#define F2C_INCLUDE

typedef long int integer;
typedef char *address;
typedef short int shortint;
typedef float real;
typedef double doublereal;
typedef struct { real r, i; } complex;
typedef struct { doublereal r, i; } doublecomplex;
typedef long int logical;
typedef short int shortlogical;

#define TRUE_ (1)
#define FALSE_ (0)

/* Extern is for use with -E */
#ifndef Extern
#define Extern extern
#endif

/* I/O stuff */

#ifdef f2c_i2
/* for -i2 */
typedef short flag;
typedef short ftnlen;
typedef short ftnint;
#else
typedef long flag;
typedef long ftnlen;
typedef long ftnint;
#endif

/*external read, write*/
typedef struct
{	flag cierr;
	ftnint ciunit;
	flag ciend;
	char *cifmt;
	ftnint cirec;
} cilist;

/*internal read, write*/
typedef struct
{	flag icierr;
	char *iciunit;
	flag iciend;
	char *icifmt;
	ftnint icirlen;
	ftnint icirnum;
} icilist;

/*open*/
typedef struct
{	flag oerr;
	ftnint ounit;
	char *ofnm;
	ftnlen ofnmlen;
	char *osta;
	char *oacc;
	char *ofm;
	ftnint orl;
	char *oblnk;
} olist;

/*close*/
typedef struct
{	flag cerr;
	ftnint cunit;
	char *csta;
} cllist;

/*rewind, backspace, endfile*/
typedef struct
{	flag aerr;
	ftnint aunit;
} alist;

/* inquire */
typedef struct
{	flag inerr;
	ftnint inunit;
	char *infile;
	ftnlen infilen;
	ftnint	*inex;	/*parameters in standard's order*/
	ftnint	*inopen;
	ftnint	*innum;
	ftnint	*innamed;
	char	*inname;
	ftnlen	innamlen;
	char	*inacc;
	ftnlen	inacclen;
	char	*inseq;
	ftnlen	inseqlen;
	char 	*indir;
	ftnlen	indirlen;
	char	*infmt;
	ftnlen	infmtlen;
	char	*inform;
	ftnint	informlen;
	char	*inunf;
	ftnlen	inunflen;
	ftnint	*inrecl;
	ftnint	*innrec;
	char	*inblank;
	ftnlen	inblanklen;
} inlist;

#define VOID void

union Multitype {	/* for multiple entry points */
	shortint h;
	integer i;
	real r;
	doublereal d;
	complex c;
	doublecomplex z;
	};

typedef union Multitype Multitype;

typedef long Long;	/* No longer used; formerly in Namelist */

struct Vardesc {	/* for Namelist */
	char *name;
	char *addr;
	ftnlen *dims;
	int  type;
	};
typedef struct Vardesc Vardesc;

struct Namelist {
	char *name;
	Vardesc **vars;
	int nvars;
	};
typedef struct Namelist Namelist;

#define abs(x) ((x) >= 0 ? (x) : -(x))
#define dabs(x) (doublereal)abs(x)
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define dmin(a,b) (doublereal)min(a,b)
#define dmax(a,b) (doublereal)max(a,b)

/* procedure parameter types for -A and -C++ */

#define F2C_proc_par_types 1
#ifdef __cplusplus
typedef int /* Unknown procedure type */ (*U_fp)(...);
typedef shortint (*J_fp)(...);
typedef integer (*I_fp)(...);
typedef real (*R_fp)(...);
typedef doublereal (*D_fp)(...), (*E_fp)(...);
typedef /* Complex */ VOID (*C_fp)(...);
typedef /* Double Complex */ VOID (*Z_fp)(...);
typedef logical (*L_fp)(...);
typedef shortlogical (*K_fp)(...);
typedef /* Character */ VOID (*H_fp)(...);
typedef /* Subroutine */ int (*S_fp)(...);
#else
#ifndef __LCC__
typedef int /* Unknown procedure type */ (*U_fp)();
typedef shortint (*J_fp)();
typedef integer (*I_fp)();
typedef real (*R_fp)();
typedef doublereal (*D_fp)(), (*E_fp)();
typedef /* Complex */ VOID (*C_fp)();
typedef /* Double Complex */ VOID (*Z_fp)();
typedef logical (*L_fp)();
typedef shortlogical (*K_fp)();
typedef /* Character */ VOID (*H_fp)();
typedef /* Subroutine */ int (*S_fp)();
#endif
#endif
/* E_fp is for real functions when -R is not specified */
typedef VOID C_f;	/* complex function */
typedef VOID H_f;	/* character function */
typedef VOID Z_f;	/* double complex function */
typedef doublereal E_f;	/* real function with -R not specified */

/* undef any lower-case symbols that your C compiler predefines, e.g.: */

#ifndef Skip_f2c_Undefs
#undef mips
#undef sgi
#undef unix
#endif
#endif

#ifdef __cplusplus
extern "C" {
extern void abort_(void);
extern double c_abs(complex *);
extern void c_cos(complex *, complex *);
extern void c_div(complex *, complex *, complex *);
extern void c_exp(complex *, complex *);
extern void c_log(complex *, complex *);
extern void c_sin(complex *, complex *);
extern void c_sqrt(complex *, complex *);
extern double d_abs(double *);
extern double d_acos(double *);
extern double d_asin(double *);
extern double d_atan(double *);
extern double d_atn2(double *, double *);
extern void d_cnjg(doublecomplex *, doublecomplex *);
extern double d_cos(double *);
extern double d_cosh(double *);
extern double d_dim(double *, double *);
extern double d_exp(double *);
extern double d_imag(doublecomplex *);
extern double d_int(double *);
extern double d_lg10(double *);
extern double d_log(double *);
extern double d_mod(double *, double *);
extern double d_nint(double *);
extern double d_prod(float *, float *);
extern double d_sign(double *, double *);
extern double d_sin(double *);
extern double d_sinh(double *);
extern double d_sqrt(double *);
extern double d_tan(double *);
extern double d_tanh(double *);
extern double derf_(double *);
extern double derfc_(double *);
extern void ef1asc_(long int *, long int *, long int *, long int *);
extern long int ef1cmc_(long int *, long int *, long int *, long int *);
extern double erf(double);
extern double erf_(float *);
extern double erfc(double);
extern double erfc_(float *);
extern void getarg_(long int *, char *, long int);
extern void getenv_(char *, char *, long int, long int);
extern int getpid(void);
extern short h_abs(short *);
extern short h_dim(short *, short *);
extern short h_dnnt(double *);
extern short h_indx(char *, char *, long int, long int);
extern short h_len(char *, long int);
extern short h_mod(short *, short *);
extern short h_nint(float *);
extern short h_sign(short *, short *);
extern short hl_ge(char *, char *, long int, long int);
extern short hl_gt(char *, char *, long int, long int);
extern short hl_le(char *, char *, long int, long int);
extern short hl_lt(char *, char *, long int, long int);
extern long int i_abs(long int *);
extern long int i_dim(long int *, long int *);
extern long int i_dnnt(double *);
extern long int i_indx(char *, char *, long int, long int);
extern long int i_len(char *, long int);
extern long int i_mod(long int *, long int *);
extern long int i_nint(float *);
extern long int i_sign(long int *, long int *);
extern long int iargc_(void);
extern long int l_ge(char *, char *, long int, long int);
extern long int l_gt(char *, char *, long int, long int);
extern long int l_le(char *, char *, long int, long int);
extern long int l_lt(char *, char *, long int, long int);
extern int main(int, char **);
extern int pause(void);
extern void pow_ci(complex *, complex *, long int *);
extern double pow_dd(double *, double *);
extern double pow_di(double *, long int *);
extern short pow_hh(short *, short *);
extern long int pow_ii(long int *, long int *);
extern double pow_ri(float *, long int *);
extern void pow_zi(doublecomplex *, doublecomplex *, long int *);
extern void pow_zz(doublecomplex *, doublecomplex *, doublecomplex *);
extern double r_abs(float *);
extern double r_acos(float *);
extern double r_asin(float *);
extern double r_atan(float *);
extern double r_atn2(float *, float *);
extern void r_cnjg(complex *, complex *);
extern double r_cos(float *);
extern double r_cosh(float *);
extern double r_dim(float *, float *);
extern double r_exp(float *);
extern double r_imag(complex *);
extern double r_int(float *);
extern double r_lg10(float *);
extern double r_log(float *);
extern double r_mod(float *, float *);
extern double r_nint(float *);
extern double r_sign(float *, float *);
extern double r_sin(float *);
extern double r_sinh(float *);
extern double r_sqrt(float *);
extern double r_tan(float *);
extern double r_tanh(float *);
extern void s_cat(char *, char **, long int *, long int *, long int);
extern long int s_cmp(char *, char *, long int, long int);
extern void s_copy(char *, char *, long int, long int);
extern void s_paus(char *, long int);
extern void s_rnge(char *, long int, char *, long int);
extern void s_stop(char *, long int);
extern void sig_die(char *, int);
extern long int signal_(long int *, void *);
extern int system_(char *, long int);
extern double z_abs(doublecomplex *);
extern void z_cos(doublecomplex *, doublecomplex *);
extern void z_div(doublecomplex *, doublecomplex *, doublecomplex *);
extern void z_exp(doublecomplex *, doublecomplex *);
extern void z_log(doublecomplex *, doublecomplex *);
extern void z_sin(doublecomplex *, doublecomplex *);
extern void z_sqrt(doublecomplex *, doublecomplex *);
	}
#endif
./ ADD NAME=getarg_.c TIME=708888157
#include "f2c.h"

/*
 * subroutine getarg(k, c)
 * returns the kth unix command argument in fortran character
 * variable argument c
*/

#ifdef KR_headers
VOID getarg_(n, s, ls) ftnint *n; register char *s; ftnlen ls;
#else
void getarg_(ftnint *n, register char *s, ftnlen ls)
#endif
{
extern int xargc;
extern char **xargv;
register char *t;
register int i;

if(*n>=0 && *n<xargc)
	t = xargv[*n];
else
	t = "";
for(i = 0; i<ls && *t!='\0' ; ++i)
	*s++ = *t++;
for( ; i<ls ; ++i)
	*s++ = ' ';
}
./ ADD NAME=getenv_.c TIME=708888060
#include "f2c.h"

/*
 * getenv - f77 subroutine to return environment variables
 *
 * called by:
 *	call getenv (ENV_NAME, char_var)
 * where:
 *	ENV_NAME is the name of an environment variable
 *	char_var is a character variable which will receive
 *		the current value of ENV_NAME, or all blanks
 *		if ENV_NAME is not defined
 */

#ifdef KR_headers
VOID getenv_(fname, value, flen, vlen) char *value, *fname; ftnlen vlen, flen;
#else
void getenv_(char *fname, char *value, ftnlen flen, ftnlen vlen)
#endif
{
extern char **environ;
register char *ep, *fp, *flast;
register char **env = environ;

flast = fname + flen;
for(fp = fname ; fp < flast ; ++fp)
	if(*fp == ' ')
		{
		flast = fp;
		break;
		}

while (ep = *env++)
	{
	for(fp = fname; fp<flast ; )
		if(*fp++ != *ep++)
			goto endloop;

	if(*ep++ == '=') {	/* copy right hand side */
		while( *ep && --vlen>=0 )
			*value++ = *ep++;

		goto blank;
		}
endloop: ;
	}

blank:
	while( --vlen >= 0 )
		*value++ = ' ';
}
./ ADD NAME=h_abs.c TIME=708876130
#include "f2c.h"

#ifdef KR_headers
shortint h_abs(x) shortint *x;
#else
shortint h_abs(shortint *x)
#endif
{
if(*x >= 0)
	return(*x);
return(- *x);
}
./ ADD NAME=h_dim.c TIME=708876178
#include "f2c.h"

#ifdef KR_headers
shortint h_dim(a,b) shortint *a, *b;
#else
shortint h_dim(shortint *a, shortint *b)
#endif
{
return( *a > *b ? *a - *b : 0);
}
./ ADD NAME=h_dnnt.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double floor();
shortint h_dnnt(x) doublereal *x;
#else
#undef abs
#include "math.h"
shortint h_dnnt(doublereal *x)
#endif
{
return( (*x)>=0 ?
	floor(*x + .5) : -floor(.5 - *x) );
}
./ ADD NAME=h_indx.c TIME=708876294
#include "f2c.h"

#ifdef KR_headers
shortint h_indx(a, b, la, lb) char *a, *b; ftnlen la, lb;
#else
shortint h_indx(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
int i, n;
char *s, *t, *bend;

n = la - lb + 1;
bend = b + lb;

for(i = 0 ; i < n ; ++i)
	{
	s = a + i;
	t = b;
	while(t < bend)
		if(*s++ != *t++)
			goto no;
	return(i+1);
	no: ;
	}
return(0);
}
./ ADD NAME=h_len.c TIME=708876334
#include "f2c.h"

#ifdef KR_headers
shortint h_len(s, n) char *s; ftnlen n;
#else
shortint h_len(char *s, ftnlen n)
#endif
{
return(n);
}
./ ADD NAME=h_mod.c TIME=708876382
#include "f2c.h"

#ifdef KR_headers
shortint h_mod(a,b) short *a, *b;
#else
shortint h_mod(short *a, short *b)
#endif
{
return( *a % *b);
}
./ ADD NAME=h_nint.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double floor();
shortint h_nint(x) real *x;
#else
#undef abs
#include "math.h"
shortint h_nint(real *x)
#endif
{
return( (*x)>=0 ?
	floor(*x + .5) : -floor(.5 - *x) );
}
./ ADD NAME=h_sign.c TIME=708876507
#include "f2c.h"

#ifdef KR_headers
shortint h_sign(a,b) shortint *a, *b;
#else
shortint h_sign(shortint *a, shortint *b)
#endif
{
shortint x;
x = (*a >= 0 ? *a : - *a);
return( *b >= 0 ? x : -x);
}
./ ADD NAME=hl_ge.c TIME=708875517
#include "f2c.h"

#ifdef KR_headers
extern integer s_cmp();
shortlogical hl_ge(a,b,la,lb) char *a, *b; ftnlen la, lb;
#else
extern integer s_cmp(char *, char *, ftnlen, ftnlen);
shortlogical hl_ge(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
return(s_cmp(a,b,la,lb) >= 0);
}
./ ADD NAME=hl_gt.c TIME=708875484
#include "f2c.h"

#ifdef KR_headers
extern integer s_cmp();
shortlogical hl_gt(a,b,la,lb) char *a, *b; ftnlen la, lb;
#else
extern integer s_cmp(char *, char *, ftnlen, ftnlen);
shortlogical hl_gt(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
return(s_cmp(a,b,la,lb) > 0);
}
./ ADD NAME=hl_le.c TIME=708875449
#include "f2c.h"

#ifdef KR_headers
extern integer s_cmp();
shortlogical hl_le(a,b,la,lb) char *a, *b; ftnlen la, lb;
#else
extern integer s_cmp(char *, char *, ftnlen, ftnlen);
shortlogical hl_le(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
return(s_cmp(a,b,la,lb) <= 0);
}
./ ADD NAME=hl_lt.c TIME=708875277
#include "f2c.h"

#ifdef KR_headers
extern integer s_cmp();
shortlogical hl_lt(a,b,la,lb) char *a, *b; ftnlen la, lb;
#else
extern integer s_cmp(char *, char *, ftnlen, ftnlen);
shortlogical hl_lt(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
return(s_cmp(a,b,la,lb) < 0);
}
./ ADD NAME=i_abs.c TIME=708877760
#include "f2c.h"

#ifdef KR_headers
integer i_abs(x) integer *x;
#else
integer i_abs(integer *x)
#endif
{
if(*x >= 0)
	return(*x);
return(- *x);
}
./ ADD NAME=i_dim.c TIME=708877832
#include "f2c.h"

#ifdef KR_headers
integer i_dim(a,b) integer *a, *b;
#else
integer i_dim(integer *a, integer *b)
#endif
{
return( *a > *b ? *a - *b : 0);
}
./ ADD NAME=i_dnnt.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double floor();
integer i_dnnt(x) doublereal *x;
#else
#undef abs
#include "math.h"
integer i_dnnt(doublereal *x)
#endif
{
return( (*x)>=0 ?
	floor(*x + .5) : -floor(.5 - *x) );
}
./ ADD NAME=i_indx.c TIME=708877932
#include "f2c.h"

#ifdef KR_headers
integer i_indx(a, b, la, lb) char *a, *b; ftnlen la, lb;
#else
integer i_indx(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
ftnlen i, n;
char *s, *t, *bend;

n = la - lb + 1;
bend = b + lb;

for(i = 0 ; i < n ; ++i)
	{
	s = a + i;
	t = b;
	while(t < bend)
		if(*s++ != *t++)
			goto no;
	return(i+1);
	no: ;
	}
return(0);
}
./ ADD NAME=i_len.c TIME=708877965
#include "f2c.h"

#ifdef KR_headers
integer i_len(s, n) char *s; ftnlen n;
#else
integer i_len(char *s, ftnlen n)
#endif
{
return(n);
}
./ ADD NAME=i_mod.c TIME=708878002
#include "f2c.h"

#ifdef KR_headers
integer i_mod(a,b) integer *a, *b;
#else
integer i_mod(integer *a, integer *b)
#endif
{
return( *a % *b);
}
./ ADD NAME=i_nint.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double floor();
integer i_nint(x) real *x;
#else
#undef abs
#include "math.h"
integer i_nint(real *x)
#endif
{
return( (*x)>=0 ?
	floor(*x + .5) : -floor(.5 - *x) );
}
./ ADD NAME=i_sign.c TIME=708878100
#include "f2c.h"

#ifdef KR_headers
integer i_sign(a,b) integer *a, *b;
#else
integer i_sign(integer *a, integer *b)
#endif
{
integer x;
x = (*a >= 0 ? *a : - *a);
return( *b >= 0 ? x : -x);
}
./ ADD NAME=iargc_.c TIME=708877715
#include "f2c.h"

#ifdef KR_headers
ftnint iargc_()
#else
ftnint iargc_(void)
#endif
{
extern int xargc;
return ( xargc - 1 );
}
./ ADD NAME=l_ge.c TIME=708878341
#include "f2c.h"

#ifdef KR_headers
extern integer s_cmp();
logical l_ge(a,b,la,lb) char *a, *b; ftnlen la, lb;
#else
extern integer s_cmp(char *, char *, ftnlen, ftnlen);
logical l_ge(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
return(s_cmp(a,b,la,lb) >= 0);
}
./ ADD NAME=l_gt.c TIME=708878405
#include "f2c.h"

#ifdef KR_headers
extern integer s_cmp();
logical l_gt(a,b,la,lb) char *a, *b; ftnlen la, lb;
#else
extern integer s_cmp(char *, char *, ftnlen, ftnlen);
logical l_gt(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
return(s_cmp(a,b,la,lb) > 0);
}
./ ADD NAME=l_le.c TIME=708878439
#include "f2c.h"

#ifdef KR_headers
extern integer s_cmp();
logical l_le(a,b,la,lb) char *a, *b; ftnlen la, lb;
#else
extern integer s_cmp(char *, char *, ftnlen, ftnlen);
logical l_le(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
return(s_cmp(a,b,la,lb) <= 0);
}
./ ADD NAME=l_lt.c TIME=708878471
#include "f2c.h"

#ifdef KR_headers
extern integer s_cmp();
logical l_lt(a,b,la,lb) char *a, *b; ftnlen la, lb;
#else
extern integer s_cmp(char *, char *, ftnlen, ftnlen);
logical l_lt(char *a, char *b, ftnlen la, ftnlen lb)
#endif
{
return(s_cmp(a,b,la,lb) < 0);
}
./ ADD NAME=main.c TIME=708912028
/* STARTUP PROCEDURE FOR UNIX FORTRAN PROGRAMS */

#include "stdio.h"
#include "signal.h"

#ifndef SIGIOT
#define SIGIOT SIGABRT
#endif

#ifndef KR_headers
#include "stdlib.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO__STDC
#define ONEXIT onexit
extern void f_exit();
#else
#ifndef KR_headers
extern void f_exit(void);
#ifndef NO_ONEXIT
#define ONEXIT atexit
extern int atexit(void (*)(void));
#endif
#else
#ifndef NO_ONEXIT
#define ONEXIT onexit
extern void f_exit();
#endif
#endif
#endif

#ifdef KR_headers
extern void f_init(), sig_die();
extern int MAIN__();
#define Int /* int */
#else
extern void f_init(void), sig_die(char*, int);
extern int MAIN__(void);
#define Int int
#endif

static void sigfdie(Int n)
{
sig_die("Floating Exception", 1);
}


static void sigidie(Int n)
{
sig_die("IOT Trap", 1);
}

#ifdef SIGQUIT
static void sigqdie(Int n)
{
sig_die("Quit signal", 1);
}
#endif


static void sigindie(Int n)
{
sig_die("Interrupt", 0);
}



static void sigtdie(Int n)
{
sig_die("Killed", 0);
}


int xargc;
char **xargv;

#ifdef KR_headers
main(argc, argv) int argc; char **argv;
#else
main(int argc, char **argv)
#endif
{
xargc = argc;
xargv = argv;
signal(SIGFPE, sigfdie);	/* ignore underflow, enable overflow */
signal(SIGIOT, sigidie);
#ifdef SIGQUIT
if(signal(SIGQUIT,sigqdie) == SIG_IGN)
	signal(SIGQUIT, SIG_IGN);
#endif
if(signal(SIGINT, sigindie) == SIG_IGN)
	signal(SIGINT, SIG_IGN);
signal(SIGTERM,sigtdie);

#ifdef pdp11
	ldfps(01200); /* detect overflow as an exception */
#endif

f_init();
#ifndef NO_ONEXIT
ONEXIT(f_exit);
#endif
MAIN__();
#ifdef NO_ONEXIT
f_exit();
#endif
exit(0);	/* exit(0) rather than return(0) to bypass Cray bug */
}
#ifdef __cplusplus
	}
#endif
./ ADD NAME=pow_ci.c TIME=708964413
#include "f2c.h"

#ifdef KR_headers
VOID pow_ci(p, a, b) 	/* p = a**b  */
 complex *p, *a; integer *b;
#else
extern void pow_zi(doublecomplex*, doublecomplex*, integer*);
void pow_ci(complex *p, complex *a, integer *b) 	/* p = a**b  */
#endif
{
doublecomplex p1, a1;

a1.r = a->r;
a1.i = a->i;

pow_zi(&p1, &a1, b);

p->r = p1.r;
p->i = p1.i;
}
./ ADD NAME=pow_dd.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double pow();
double pow_dd(ap, bp) doublereal *ap, *bp;
#else
#undef abs
#include "math.h"
double pow_dd(doublereal *ap, doublereal *bp)
#endif
{
return(pow(*ap, *bp) );
}
./ ADD NAME=pow_di.c TIME=708879979
#include "f2c.h"

#ifdef KR_headers
double pow_di(ap, bp) doublereal *ap; integer *bp;
#else
double pow_di(doublereal *ap, integer *bp)
#endif
{
double pow, x;
integer n;

pow = 1;
x = *ap;
n = *bp;

if(n != 0)
	{
	if(n < 0)
		{
		if(x == 0)
			{
			return(pow);
			}
		n = -n;
		x = 1/x;
		}
	for( ; ; )
		{
		if(n & 01)
			pow *= x;
		if(n >>= 1)
			x *= x;
		else
			break;
		}
	}
return(pow);
}
./ ADD NAME=pow_hh.c TIME=708880026
#include "f2c.h"

#ifdef KR_headers
shortint pow_hh(ap, bp) shortint *ap, *bp;
#else
shortint pow_hh(shortint *ap, shortint *bp)
#endif
{
shortint pow, x, n;

pow = 1;
x = *ap;
n = *bp;

if(n < 0)
	{ }
else if(n > 0)
	for( ; ; )
		{
		if(n & 01)
			pow *= x;
		if(n >>= 1)
			x *= x;
		else
			break;
		}
return(pow);
}
./ ADD NAME=pow_ii.c TIME=708880096
#include "f2c.h"

#ifdef KR_headers
integer pow_ii(ap, bp) integer *ap, *bp;
#else
integer pow_ii(integer *ap, integer *bp)
#endif
{
integer pow, x, n;

pow = 1;
x = *ap;
n = *bp;

if(n < 0)
	{ }
else if(n > 0)
	for( ; ; )
		{
		if(n & 01)
			pow *= x;
		if(n >>= 1)
			x *= x;
		else
			break;
		}
return(pow);
}
./ ADD NAME=pow_ri.c TIME=708880062
#include "f2c.h"

#ifdef KR_headers
double pow_ri(ap, bp) real *ap; integer *bp;
#else
double pow_ri(real *ap, integer *bp)
#endif
{
double pow, x;
integer n;

pow = 1;
x = *ap;
n = *bp;

if(n != 0)
	{
	if(n < 0)
		{
		if(x == 0)
			{
			return(pow);
			}
		n = -n;
		x = 1/x;
		}
	for( ; ; )
		{
		if(n & 01)
			pow *= x;
		if(n >>= 1)
			x *= x;
		else
			break;
		}
	}
return(pow);
}
./ ADD NAME=pow_zi.c TIME=708879685
#include "f2c.h"

#ifdef KR_headers
VOID pow_zi(p, a, b) 	/* p = a**b  */
 doublecomplex *p, *a; integer *b;
#else
extern void z_div(doublecomplex*, doublecomplex*, doublecomplex*);
void pow_zi(doublecomplex *p, doublecomplex *a, integer *b) 	/* p = a**b  */
#endif
{
integer n;
double t;
doublecomplex x;
static doublecomplex one = {1.0, 0.0};

n = *b;
p->r = 1;
p->i = 0;

if(n == 0)
	return;
if(n < 0)
	{
	n = -n;
	z_div(&x, &one, a);
	}
else
	{
	x.r = a->r;
	x.i = a->i;
	}

for( ; ; )
	{
	if(n & 01)
		{
		t = p->r * x.r - p->i * x.i;
		p->i = p->r * x.i + p->i * x.r;
		p->r = t;
		}
	if(n >>= 1)
		{
		t = x.r * x.r - x.i * x.i;
		x.i = 2 * x.r * x.i;
		x.r = t;
		}
	else
		break;
	}
}
./ ADD NAME=pow_zz.c TIME=708960922
#include "f2c.h"

#ifdef KR_headers
double log(), exp(), cos(), sin(), atan2(), Cabs();
VOID pow_zz(r,a,b) doublecomplex *r, *a, *b;
#else
#undef abs
#include "math.h"
extern double Cabs(double,double);
void pow_zz(doublecomplex *r, doublecomplex *a, doublecomplex *b)
#endif
{
double logr, logi, x, y;

logr = log( Cabs(a->r, a->i) );
logi = atan2(a->i, a->r);

x = exp( logr * b->r - logi * b->i );
y = logr * b->i + logi * b->r;

r->r = x * cos(y);
r->i = x * sin(y);
}
./ ADD NAME=r_abs.c TIME=708880224
#include "f2c.h"

#ifdef KR_headers
double r_abs(x) real *x;
#else
double r_abs(real *x)
#endif
{
if(*x >= 0)
	return(*x);
return(- *x);
}
./ ADD NAME=r_acos.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double acos();
double r_acos(x) real *x;
#else
#undef abs
#include "math.h"
double r_acos(real *x)
#endif
{
return( acos(*x) );
}
./ ADD NAME=r_asin.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double asin();
double r_asin(x) real *x;
#else
#undef abs
#include "math.h"
double r_asin(real *x)
#endif
{
return( asin(*x) );
}
./ ADD NAME=r_atan.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double atan();
double r_atan(x) real *x;
#else
#undef abs
#include "math.h"
double r_atan(real *x)
#endif
{
return( atan(*x) );
}
./ ADD NAME=r_atn2.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double atan2();
double r_atn2(x,y) real *x, *y;
#else
#undef abs
#include "math.h"
double r_atn2(real *x, real *y)
#endif
{
return( atan2(*x,*y) );
}
./ ADD NAME=r_cnjg.c TIME=708885238
#include "f2c.h"

#ifdef KR_headers
VOID r_cnjg(r, z) complex *r, *z;
#else
VOID r_cnjg(complex *r, complex *z)
#endif
{
r->r = z->r;
r->i = - z->i;
}
./ ADD NAME=r_cos.c TIME=708889713
#include "f2c.h"

#ifdef KR_headers
double cos();
double r_cos(x) real *x;
#else
#undef abs
#include "math.h"
double r_cos(real *x)
#endif
{
return( cos(*x) );
}
./ ADD NAME=r_cosh.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double cosh();
double r_cosh(x) real *x;
#else
#undef abs
#include "math.h"
double r_cosh(real *x)
#endif
{
return( cosh(*x) );
}
./ ADD NAME=r_dim.c TIME=708885323
#include "f2c.h"

#ifdef KR_headers
double r_dim(a,b) real *a, *b;
#else
double r_dim(real *a, real *b)
#endif
{
return( *a > *b ? *a - *b : 0);
}
./ ADD NAME=r_exp.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double exp();
double r_exp(x) real *x;
#else
#undef abs
#include "math.h"
double r_exp(real *x)
#endif
{
return( exp(*x) );
}
./ ADD NAME=r_imag.c TIME=708885387
#include "f2c.h"

#ifdef KR_headers
double r_imag(z) complex *z;
#else
double r_imag(complex *z)
#endif
{
return(z->i);
}
./ ADD NAME=r_int.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double floor();
double r_int(x) real *x;
#else
#undef abs
#include "math.h"
double r_int(real *x)
#endif
{
return( (*x>0) ? floor(*x) : -floor(- *x) );
}
./ ADD NAME=r_lg10.c TIME=708889714
#include "f2c.h"

#define log10e 0.43429448190325182765

#ifdef KR_headers
double log();
double r_lg10(x) real *x;
#else
#undef abs
#include "math.h"
double r_lg10(real *x)
#endif
{
return( log10e * log(*x) );
}
./ ADD NAME=r_log.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double log();
double r_log(x) real *x;
#else
#undef abs
#include "math.h"
double r_log(real *x)
#endif
{
return( log(*x) );
}
./ ADD NAME=r_mod.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
#ifdef IEEE_drem
double drem();
#else
double floor();
#endif
double r_mod(x,y) real *x, *y;
#else
#ifdef IEEE_drem
double drem(double, double);
#else
#undef abs
#include "math.h"
#endif
double r_mod(real *x, real *y)
#endif
{
#ifdef IEEE_drem
	double xa, ya, z;
	if ((ya = *y) < 0.)
		ya = -ya;
	z = drem(xa = *x, ya);
	if (xa > 0) {
		if (z < 0)
			z += ya;
		}
	else if (z > 0)
		z -= ya;
	return z;
#else
	double quotient;
	if( (quotient = (double)*x / *y) >= 0)
		quotient = floor(quotient);
	else
		quotient = -floor(-quotient);
	return(*x - (*y) * quotient );
#endif
}
./ ADD NAME=r_nint.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double floor();
double r_nint(x) real *x;
#else
#undef abs
#include "math.h"
double r_nint(real *x)
#endif
{
return( (*x)>=0 ?
	floor(*x + .5) : -floor(.5 - *x) );
}
./ ADD NAME=r_sign.c TIME=708885772
#include "f2c.h"

#ifdef KR_headers
double r_sign(a,b) real *a, *b;
#else
double r_sign(real *a, real *b)
#endif
{
double x;
x = (*a >= 0 ? *a : - *a);
return( *b >= 0 ? x : -x);
}
./ ADD NAME=r_sin.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double sin();
double r_sin(x) real *x;
#else
#undef abs
#include "math.h"
double r_sin(real *x)
#endif
{
return( sin(*x) );
}
./ ADD NAME=r_sinh.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double sinh();
double r_sinh(x) real *x;
#else
#undef abs
#include "math.h"
double r_sinh(real *x)
#endif
{
return( sinh(*x) );
}
./ ADD NAME=r_sqrt.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double sqrt();
double r_sqrt(x) real *x;
#else
#undef abs
#include "math.h"
double r_sqrt(real *x)
#endif
{
return( sqrt(*x) );
}
./ ADD NAME=r_tan.c TIME=708890521
#include "f2c.h"

#ifdef KR_headers
double tan();
double r_tan(x) real *x;
#else
#undef abs
#include "math.h"
double r_tan(real *x)
#endif
{
return( tan(*x) );
}
./ ADD NAME=r_tanh.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double tanh();
double r_tanh(x) real *x;
#else
#undef abs
#include "math.h"
double r_tanh(real *x)
#endif
{
return( tanh(*x) );
}
./ ADD NAME=s_cat.c TIME=708887176
#include "f2c.h"

#ifdef KR_headers
VOID s_cat(lp, rpp, rnp, np, ll) char *lp, *rpp[]; ftnlen rnp[], *np, ll;
#else
VOID s_cat(char *lp, char *rpp[], ftnlen rnp[], ftnlen *np, ftnlen ll)
#endif
{
int i, n, nc;
char *rp;

n = *np;
for(i = 0 ; i < n ; ++i)
	{
	nc = ll;
	if(rnp[i] < nc)
		nc = rnp[i];
	ll -= nc;
	rp = rpp[i];
	while(--nc >= 0)
		*lp++ = *rp++;
	}
while(--ll >= 0)
	*lp++ = ' ';
}
./ ADD NAME=s_cmp.c TIME=708887238
#include "f2c.h"

/* compare two strings */

#ifdef KR_headers
integer s_cmp(a, b, la, lb) register char *a, *b; ftnlen la, lb;
#else
integer s_cmp(register char *a, register char *b, ftnlen la, ftnlen lb)
#endif
{
register char *aend, *bend;
aend = a + la;
bend = b + lb;

if(la <= lb)
	{
	while(a < aend)
		if(*a != *b)
			return( *a - *b );
		else
			{ ++a; ++b; }

	while(b < bend)
		if(*b != ' ')
			return( ' ' - *b );
		else	++b;
	}

else
	{
	while(b < bend)
		if(*a == *b)
			{ ++a; ++b; }
		else
			return( *a - *b );
	while(a < aend)
		if(*a != ' ')
			return(*a - ' ');
		else	++a;
	}
return(0);
}
./ ADD NAME=s_copy.c TIME=708887356
#include "f2c.h"

/* assign strings:  a = b */

#ifdef KR_headers
VOID s_copy(a, b, la, lb) register char *a, *b; ftnlen la, lb;
#else
void s_copy(register char *a, register char *b, ftnlen la, ftnlen lb)
#endif
{
register char *aend, *bend;

aend = a + la;

if(la <= lb)
	while(a < aend)
		*a++ = *b++;

else
	{
	bend = b + lb;
	while(b < bend)
		*a++ = *b++;
	while(a < aend)
		*a++ = ' ';
	}
}
./ ADD NAME=s_paus.c TIME=708917853
#include "stdio.h"
#include "f2c.h"
#define PAUSESIG 15

#ifdef KR_headers
#define Void /* void */
#define Int /* int */
#else
#define Void void
#define Int int
#undef abs
#include "stdlib.h"
#include "signal.h"
extern int getpid(void), isatty(int), pause(void);
#endif

#ifdef __cplusplus
extern "C" void f_exit(void);
#else
extern VOID f_exit(Void);
#endif

static VOID waitpause(Int n)
{
return;
}

#ifdef KR_headers
VOID s_paus(s, n) char *s; ftnlen n;
#else
void s_paus(char *s, ftnlen n)
#endif
{
int i;

fprintf(stderr, "PAUSE ");
if(n > 0)
	for(i = 0; i<n ; ++i)
		putc(*s++, stderr);
fprintf(stderr, " statement executed\n");
if( isatty(fileno(stdin)) )
	{
	fprintf(stderr, "To resume execution, type go.  Any other input will terminate job.\n");
	fflush(stderr);
	if( getchar()!='g' || getchar()!='o' || getchar()!='\n' )
		{
		fprintf(stderr, "STOP\n");
		f_exit();
		exit(0);
		}
	}
else
	{
	fprintf(stderr, "To resume execution, execute a   kill -%d %d   command\n",
		PAUSESIG, getpid() );
	signal(PAUSESIG, waitpause);
	fflush(stderr);
	pause();
	}
fprintf(stderr, "Execution resumes after PAUSE.\n");
}
./ ADD NAME=s_rnge.c TIME=708887851
#include "stdio.h"
#include "f2c.h"

/* called when a subscript is out of range */

#ifdef KR_headers
extern VOID sig_die();
VOID s_rnge(varn, offset, procn, line) char *varn, *procn; ftnint offset, line;
#else
extern VOID sig_die(char*,int);
void s_rnge(char *varn, ftnint offset, char *procn, ftnint line)
#endif
{
register int i;

fprintf(stderr, "Subscript out of range on file line %ld, procedure ", line);
while((i = *procn) && i != '_' && i != ' ')
	putc(*procn++, stderr);
fprintf(stderr, ".\nAttempt to access the %ld-th element of variable ", offset+1);
while((i = *varn) && i != ' ')
	putc(*varn++, stderr);
sig_die(".", 1);
}
./ ADD NAME=s_stop.c TIME=708917916
#include "stdio.h"
#include "f2c.h"

#ifdef KR_headers
extern void f_exit();
VOID s_stop(s, n) char *s; ftnlen n;
#else
#undef abs
#include "stdlib.h"
#ifdef __cplusplus
extern "C" void f_exit(void);
#else
extern VOID f_exit(Void);
#endif
void s_stop(char *s, ftnlen n)
#endif
{
int i;

if(n > 0)
	{
	fprintf(stderr, "STOP ");
	for(i = 0; i<n ; ++i)
		putc(*s++, stderr);
	fprintf(stderr, " statement executed\n");
	}
f_exit();
exit(0);
}
./ ADD NAME=sig_die.c TIME=708905928
#include "stdio.h"
#include "signal.h"

#ifndef SIGIOT
#define SIGIOT SIGABRT
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef KR_headers
void sig_die(s, kill) register char *s; int kill;
#else
#include "stdlib.h"
 extern void f_exit(void);

void sig_die(register char *s, int kill)
#endif
{
	/* print error message, then clear buffers */
	fprintf(stderr, "%s\n", s);
	fflush(stderr);
	f_exit();
	fflush(stderr);

	if(kill)
		{
		/* now get a core */
		signal(SIGIOT, SIG_DFL);
		abort();
		}
	else
		exit(1);
	}
#ifdef __cplusplus
	}
#endif
./ ADD NAME=signal_.c TIME=708887100
#include "f2c.h"

#ifdef KR_headers
typedef int (*sig_type)();
extern sig_type signal();

ftnint signal_(sigp, procp) ftnint *sigp, *procp;
#else
#include "signal.h"
typedef void (*sig_type)(int);

ftnint signal_(ftnint *sigp, void *procp)
#endif
{
	int sig;
	sig_type proc;
	sig = *sigp;
	proc = *(sig_type *)procp;

	return (ftnint)signal(sig, proc);
	}
./ ADD NAME=system_.c TIME=708886771
/* f77 interface to system routine */

#include "f2c.h"

#ifdef KR_headers
system_(s, n) register char *s; ftnlen n;
#else
#undef abs
#include "stdlib.h"
system_(register char *s, ftnlen n)
#endif
{
char buff[1000];
register char *bp, *blast;

blast = buff + (n < 1000 ? n : 1000);

for(bp = buff ; bp<blast && *s!='\0' ; )
	*bp++ = *s++;
*bp = '\0';
return system(buff);
}
./ ADD NAME=z_abs.c TIME=708960777
#include "f2c.h"

#ifdef KR_headers
double Cabs();
double z_abs(z) doublecomplex *z;
#else
double Cabs(double, double);
double z_abs(doublecomplex *z)
#endif
{
return( Cabs( z->r, z->i ) );
}
./ ADD NAME=z_cos.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double sin(), cos(), sinh(), cosh();
VOID z_cos(r, z) doublecomplex *r, *z;
#else
#undef abs
#include "math.h"
void z_cos(doublecomplex *r, doublecomplex *z)
#endif
{
r->r = cos(z->r) * cosh(z->i);
r->i = - sin(z->r) * sinh(z->i);
}
./ ADD NAME=z_div.c TIME=708886308
#include "f2c.h"

#ifdef KR_headers
extern void sig_die();
VOID z_div(c, a, b) doublecomplex *a, *b, *c;
#else
extern void sig_die(char*, int);
void z_div(doublecomplex *c, doublecomplex *a, doublecomplex *b)
#endif
{
double ratio, den;
double abr, abi;

if( (abr = b->r) < 0.)
	abr = - abr;
if( (abi = b->i) < 0.)
	abi = - abi;
if( abr <= abi )
	{
	if(abi == 0)
		sig_die("complex division by zero", 1);
	ratio = b->r / b->i ;
	den = b->i * (1 + ratio*ratio);
	c->r = (a->r*ratio + a->i) / den;
	c->i = (a->i*ratio - a->r) / den;
	}

else
	{
	ratio = b->i / b->r ;
	den = b->r * (1 + ratio*ratio);
	c->r = (a->r + a->i*ratio) / den;
	c->i = (a->i - a->r*ratio) / den;
	}

}
./ ADD NAME=z_exp.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double exp(), cos(), sin();
VOID z_exp(r, z) doublecomplex *r, *z;
#else
#undef abs
#include "math.h"
void z_exp(doublecomplex *r, doublecomplex *z)
#endif
{
double expx;

expx = exp(z->r);
r->r = expx * cos(z->i);
r->i = expx * sin(z->i);
}
./ ADD NAME=z_log.c TIME=708960939
#include "f2c.h"

#ifdef KR_headers
double log(), Cabs(), atan2();
VOID z_log(r, z) doublecomplex *r, *z;
#else
#undef abs
#include "math.h"
extern double Cabs(double, double);
void z_log(doublecomplex *r, doublecomplex *z)
#endif
{

r->i = atan2(z->i, z->r);
r->r = log( Cabs( z->r, z->i ) );
}
./ ADD NAME=z_sin.c TIME=708889714
#include "f2c.h"

#ifdef KR_headers
double sin(), cos(), sinh(), cosh();
VOID z_sin(r, z) doublecomplex *r, *z;
#else
#undef abs
#include "math.h"
void z_sin(doublecomplex *r, doublecomplex *z)
#endif
{
r->r = sin(z->r) * cosh(z->i);
r->i = cos(z->r) * sinh(z->i);
}
./ ADD NAME=z_sqrt.c TIME=708960856
#include "f2c.h"

#ifdef KR_headers
double sqrt(), Cabs();
VOID z_sqrt(r, z) doublecomplex *r, *z;
#else
#undef abs
#include "math.h"
extern double Cabs(double, double);
void z_sqrt(doublecomplex *r, doublecomplex *z)
#endif
{
double mag;

if( (mag = Cabs(z->r, z->i)) == 0.)
	r->r = r->i = 0.;
else if(z->r > 0)
	{
	r->r = sqrt(0.5 * (mag + z->r) );
	r->i = z->i / r->r / 2;
	}
else
	{
	r->i = sqrt(0.5 * (mag - z->r) );
	if(z->i < 0)
		r->i = - r->i;
	r->r = z->i / r->i / 2;
	}
}
./ ENDUP
