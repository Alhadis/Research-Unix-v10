/* As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#ifndef DEFS_H
#define DEFS_H

#include "flags.h"

/* In your flags.h, you should
 *  #define FREESTANDING 1
 * if you are testing a freestanding system
 */

#include "compil.h"


#ifdef __cplusplus
#define ANSI 1
#endif /* C++ impementations assumed also ANSI C except sometimes */

#if !ANSI || !LIB_TEST
#include "machin.h"
#endif

/* THE REMAINING DEFINITIONS SHOULD NOT BE MODIFIED */
#define TRUE 1
#define FALSE 0
/* Where possible, we will try to make the compiler have
 * as many registers as possible tied up.  This usually
 * forces more error-prone paths.
 */
#define REGISTERS register int r1,r2,r3,r4,r5;register char *r6,*r7,*r8,*r9,*r10
#define USEREGISTERS r1=r2=r3=r4=r5=0;r6=r7=r8=r9=r10=0
extern char *Filename; /* will always get set to the current source file */

#define ENTRY(s) entry_fn(s)
#if !VOID_OK
#define void char
#endif

#if !ANSI
#define const
#endif

#if HAS_PROTOTYPES
#define PARMS(x) x
#else
#define PARMS(X) ()
#endif	/* HAS_PROTOTYPES */

int aequals PARMS((int, const void *, const void *));
char *arbhex PARMS((char *, const char *, int));
generic_ptr avalue PARMS((generic_ptr));
void begin_case PARMS((const char *));
int checkthat PARMS((int, int));
int complain PARMS((int));
double dabs PARMS((double));
void dbprint PARMS((const char *));
int dequals PARMS((int, double, double));
int do_nothing PARMS((void *, ...));
double dpowu PARMS((double, unsigned));
double dvalue PARMS((double));
void end_case PARMS((const char *));
void entry_fn PARMS((const char *));
int faequals PARMS((int, int (*)(), int (*)()));
int fequals PARMS((int, double, double));
float fvalue PARMS((double));
int iabs PARMS((int));
int iequals PARMS((int, int, int));
void ignore PARMS((const void *));
int inotequals PARMS((int, int, int));
int ivalue PARMS((int));
int ldequals PARMS((int, long double, long double));
int lequals PARMS((int, long, long));
char *ltostr PARMS((char *, long, unsigned));
long lvalue PARMS((long));
int mem_cmp PARMS((const void *, const void *, int));
void pr_err PARMS((const char *));
void pr_l PARMS((long, unsigned));
void pr_ok PARMS((const char *));
void pr_skip PARMS((const char *));
void pr_ul PARMS((ULONG, unsigned));
int report PARMS((const char *));
int scheck PARMS((int, int, int, int));
int stequals PARMS((int, const char*, const char*));
int st_ncmp PARMS(( const char *, const char *, int)); /* NOT str_n.. */
char *st_ncpy PARMS(( char *, const char *, int));     /* NOT str_n.. */
char *str_cat PARMS(( char *, const char *));
int str_cspn PARMS(( char *, char *));
int str_cmp PARMS((const char *, const char *));
char *str_cpye PARMS((char *, const char *));
int str_len PARMS((const char *));
char *str_rev PARMS((char *));
const char *str_str PARMS((const char *, const char *));
void trace PARMS((const char *, int));
int ulequals PARMS((int, ULONG, ULONG));
char *ultostr PARMS((char *, ULONG, unsigned));

#define ENTRY(s) entry_fn(s)

#define ieq(i1, i2)			iequals(__LINE__, i1, i2)
#define inoteq(i1, i2)		inotequals(__LINE__, i1, i2)
#define aeq(a1, a2)			aequals(__LINE__, a1, a2)
#define faeq(a1, a2)		faequals(__LINE__, a1, a2)
#define leq(L1, L2)			lequals(__LINE__, L1, L2)
#define deq(d1, d2)			dequals(__LINE__, d1, d2)
#define feq(d1, d2)			fequals(__LINE__, d1, d2)
#define chk(i1)				checkthat(__LINE__, i1)
#define steq(a1, a2)		stequals(__LINE__, a1, a2)
#define schk(i1, i2, i3)	scheck(__LINE__, i1, i2, i3)
#define boo()				complain(__LINE__)
#define ldeq(ld1, ld2)		ldequals(__LINE__, ld1, ld2)

#if NEW_STYLE_FN_DEF
#define FN_DEF1(a1, d1) (d1)
#define FN_DEF1V(a1, d1) (d1, ...)
#define FN_DEF2(a1, a2, d1, d2) (d1, d2)
#define FN_DEF3(a1, a2, a3, d1, d2, d3) (d1, d2, d3)
#define FN_DEF4(a1, a2, a3, a4, d1, d2, d3, d4) (d1, d2, d3, d4)
#else /* !NEW_STYLE_FN_DEF */
#define FN_DEF1(a1, d1) (a1) d1;
#define FN_DEF1V(a1, d1) (a1) d1;
#define FN_DEF2(a1, a2, d1, d2) (a1, a2) d1; d2;
#define FN_DEF3(a1, a2, a3, d1, d2, d3) (a1, a2, a3) d1; d2; d3;
#define FN_DEF4(a1, a2, a3, a4, d1, d2, d3, d4) (a1, a2, a3, a4) d1; d2; d3; d4;
#endif /* NEW_STYLE_FN_DEF */

#if !ANSI
#ifndef DBL_DIG
#define DBL_DIG DIGITS_MAX
#endif
#ifndef FLT_DIG
#define FLT_DIG FDIGITS_MAX
#endif
#ifndef LDBL_DIG
#define LDBL_DIG DIGITS_MAX
#endif
#ifndef DBL_MAX_10_EXP
#define DBL_MAX_10_EXP 37	/* a portable minimum value */
#endif
#ifndef DBL_MIN_10_EXP
#define DBL_MIN_10_EXP -37	/* a portable minimum value */
#endif
#ifndef LDBL_MIN_10_EXP
#define LDBL_MIN_10_EXP -37	/* a portable minimum value */
#endif
#endif	/* of !ANSI */
#endif  /* of DEFS_H */
