/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP33
/*
 * 3.3 - Expressions
 */
#include "defs.h"


static void c3_3_1();
static void c3_3_2_1();
static void c3_3_2_2();
static void c3_3_2_3();
static void c3_3_2_4();
static void c3_3_3();
static void c3_3_3_1();
static void c3_3_3_2();
static void c3_3_3_3();
static void c3_3_3_4();
static void c3_3_4();
extern void c3_3b();		/* in c33b.c */
extern void structures();	/* in c33c.c */
static long f() { return 99; }
/*
 * 3.3 - Expressions
 */
void c3_3()
	{
	Filename = "c33a.c";

	c3_3_1();
	c3_3_2_1();
	c3_3_2_2();
	c3_3_2_3();
	c3_3_2_4();
	c3_3_3();
	c3_3_3_1();
	c3_3_3_2();
	c3_3_3_3();
	c3_3_3_4();
	c3_3_4();
	/* too big for one module */
	c3_3b();
	}
/*
 * 3.3.1 - Primary expressions
 */
static int x;
static int *fp(){ return &x;}
static void c3_3_1()
	{
	char c = 0;
	char *pc = &c;
	char **ppc = &pc;
	char ***pppc = &ppc;
	char ****ppppc = &pppc;
	char *****pppppc = &ppppc;
	long int larray[2][3][4];
	int i=1,j=2,k=3;

	i = 1; j = 2; k = 3;
	do_nothing(&i,&j,&k);
	*fp() = 7;
	iequals(__LINE__, x, 7);
	*****pppppc = 17;
	iequals(__LINE__, c, 17);
	larray[i][j][k] = 277777;
	lequals(__LINE__, larray[1][2][3], 277777L);

	/* parentheses do not affect lvalues -- try the above examples */
	{
	char (((c))) = 0;
	char (*(pc)) = &c;
	char (*(*(ppc))) = &pc;
	char (*(*(*(pppc)))) = &ppc;
	char (*(*(*(*(ppppc))))) = &pppc;
	char (*(*(*(*(*(pppppc)))))) = &ppppc;
	long int larray[2][3][4];
	int i=1,j=2,k=3;

	do_nothing(&i,&j,&k);
	(*(fp())) = 8;
	((iequals(__LINE__, x, 8)));
	(*(*(*(*(*(pppppc)))))) = 18;
	iequals(__LINE__, c, 18);
	(larray[i][j][k]) = 288888;
	lequals(__LINE__, larray[1][2][3], 288888L);
	}
	} /* end c3_3_1 */





/*
 * 3.3.2.1 - Array subscripting
 */
static void c3_3_2_1()
	{
	int iarray[10], *pi = iarray, i = 3, j = -3, k;
	int i3d[2][3][4], count;

	iarray[0] = 17;
	iarray[3] = 3;

	/* the array name is equivalent to a pointer to the first element */
	iequals(__LINE__, *iarray, 17);
	iequals(__LINE__, iarray[i], 3);
	iequals(__LINE__, *(iarray + i), 3);
	iequals(__LINE__, *(i + iarray), 3);
	iequals(__LINE__, i[iarray], 3);
	iequals(__LINE__, (pi+3)[j], 17);
	iequals(__LINE__, *(pi + 3 + j), 17);
	iequals(__LINE__, *(j + pi + 3), 17);
	iequals(__LINE__, j[pi + 3], 17);

	/* multi dimensional arrays */
	for (i = 0, count = 0; i < 2; ++i)
		for (j = 0; j < 3; ++j)
			for (k = 0; k < 4; ++k)
				i3d[i][j][k] = count++;
	i = ivalue(1);
	j = ivalue(2);
	k = ivalue(3);
	pi = (int *)i3d[i];
	iequals(__LINE__, *pi, 12);
	pi = (int *)i3d[i][j];
	iequals(__LINE__, *pi, 20);
	pi = &i3d[i][j][k];
	iequals(__LINE__, *pi, 23);

	/* partial addressing yields pointers to sub-dimensions */
	aequals(__LINE__, i3d+1, &i3d[i][0][0]);
	aequals(__LINE__, i3d+1, &i3d[1][0][0]);
	aequals(__LINE__, i3d+2, &i3d[i+1][0][0]);
	aequals(__LINE__, i3d+2, &i3d[2][0][0]);
	aequals(__LINE__, i3d+2, &i3d[i][j][k+1]);
	aequals(__LINE__, i3d+2, &i3d[1][2][4]);
	aequals(__LINE__, i3d[i], &i3d[1][0][0]);
	aequals(__LINE__, i3d[i]+1, &i3d[1][1][0]);
	aequals(__LINE__, i3d[i][j], &i3d[1][2][0]);
	aequals(__LINE__, i3d[i][j]+1, &i3d[1][2][1]);
	aequals(__LINE__, j[i3d[i]]+1, &i3d[1][2][1]);
	}




/*
 * 3.3.2.2 - Function calls
 */
	char cfun(){return ivalue(1);}
	int ifun(){return ivalue(2);}
	long lfun(){return lvalue(3L);}
	float ffun(){return dvalue(4.0);}
	double dfun(){return dvalue(5.0);}
#if ANSI
	long double ldfun(){return dvalue(6.0L);}
#endif
#if ANSI && HAS_PROTOTYPES
unsigned knothole(unsigned char uc) { return uc; }
int f_int(int i) { return ivalue(i); }
typedef int (*pfi)(int i);
pfi f_dbl(double x) { return &f_int; }
#endif
	static void c3_3_2_2();
	void (*fpfun())(){return c3_3_2_2;}
static void c3_3_2_2()
	{
	int i = 0;
	extern int Side;
	
	/* Functions can return any type. Test for the simple types */
	iequals(__LINE__, cfun(), 1);
	iequals(__LINE__, ifun(), 2);
	lequals(__LINE__, lfun(), 3L);
	dequals(__LINE__, ffun(), 4.0);
	dequals(__LINE__, dfun(), 5.0);
#if ANSI
	{
	long double (*pldfun)() = ldfun;

	ldequals(__LINE__, ldfun(), 6.0L);
	ldequals(__LINE__, (*ldfun)(), 6.0L);
	ldequals(__LINE__, (**ldfun)(), 6.0L);
	ldequals(__LINE__, pldfun(), 6.0L);
	ldequals(__LINE__, (*pldfun)(), 6.0L);
	ldequals(__LINE__, (**pldfun)(), 6.0L);
	}
#endif
	checkthat(__LINE__, fpfun() == c3_3_2_2);

	/* this tests calls/returns involving structs and unions */
	structures();
	Filename = "c33a.c";


	/* make sure that argument passing is by value */																			/* 3.3.2.2 (cont.) */
	(void) wfunc(i);	/* tests call with no decl in scope */
	iequals(__LINE__, i, 0);

#if ANSI && HAS_PROTOTYPES
	/* check argument coercion in the presence of a prototype */
		{
		iequals(__LINE__, knothole(ULONG_MAX), UCHAR_MAX);
		iequals(__LINE__, 2.0, 2L);
		lequals(__LINE__, 3, 3.0);
		dequals(__LINE__, 4, 4L);
		do_nothing(&i, 1, 2, 3, 4, 5, 6, 7, 8, 9);
		}
	/* check calling through pointer-to-function */
	iequals(__LINE__, f_dbl(1.)(2), 2);
#endif

	/* side effects are completed before a function call */
	xfunc(Side++);
	Side = 0;
		{
		int zfunc();
	
		/* recursion must be possible */
		yfunc(3, zfunc, &i);	/* no proto, default conversions */
	
		/* zfunc changes a value through a pointer */
		iequals(__LINE__, i, 99);
		}
	} /* end c3_3_2_2 */






















wfunc(i) int i; { i = ivalue(17); return i; }																					/* 3.3.2.2 (cont.) */

xfunc(i) 	/* no return type, default to int */
	int i;
	{
	extern int Side;

	/* the side effect will have been completed at this point */
	iequals(__LINE__, i, 0);
	iequals(__LINE__, Side, 1);
	}		/* no return expression, would be erroneous to use value */

yfunc(i, pfn, pi)	/* no return type, default to int */
	int i;
#if ANSI
	int pfn();	/* parm w fn type becomes ptr-to-fn */
#else
	int (*pfn)();
#endif
	int *pi;
	{ 
	static int j = 3;

	iequals(__LINE__, i, j);
	if (--i == 0)
		return;
	--j;
	yfunc(i, pfn, pi);
	iequals(__LINE__, i, j);
	++j;
#if ANSI
	pfn(pi);	/* converts to (*pfn)() */
#else
	(*pfn)(pi);
#endif
	}		/* no return expression, would be erroneous to use value */

zfunc(pi)	/* no return type, default to int */
	int pi[2];	/* array parm is converted to ptr */
	{
	*pi = 99;
	}		/* no return expression, would be erroneous to use value */
	







/*
 * 3.3.2.3 - Structure and union members
 */
static void c3_3_2_3()
	{
	/* See strucs() in c33c.c (appx lines 50-90) for lvalue uses.
	 * These have been tested all over. One special assurance to check:
	 * if the union begins with several structures with identical initial
	 * sequences, then the common part may be inspected.  Otherwise, the
	 * only guarantee is that the member you put in comes back out 
	 * correctly.
	 */
	union
		{
		struct { int a; double d; } s1;
		struct { int b; long l; } s2; /* ANSI8804 - types matter, names don't */
		}u;

	u.s1.a = 1;
	u.s1.d = 2.0;
	if (u.s2.b == 1)
		dequals(__LINE__, u.s1.d, 2.0);
	else
		complain(__LINE__);
	}

























/*
 * 3.3.2.4 - Postfix increment and decrement operators
 */
static long larray[] = {1, 2};
static void c3_3_2_4()
	{
	long *p = larray;
	double d = 2.2;
	int i = 3;
	long l = 4;
	char c = 5;

	/* these are tested in several places. make sure that ++ and --
	 * work right together.
	 * See prec*.c for combination of ++ and other ops.
	 * See bitfields() in c35b.c for ++ with bitfield.
	 */
	lequals(__LINE__, *p++, 1L);
	lequals(__LINE__, *p--, 2L);
	lequals(__LINE__, *p, 1L);

	/* make sure that they work on scalar types correctly */
	dequals(__LINE__, d++, 2.2);
	dequals(__LINE__, d--, 3.2);
	dequals(__LINE__, d, 2.2);
	iequals(__LINE__, i++, 3);
	iequals(__LINE__, i--, 4);
	iequals(__LINE__, i, 3);
	lequals(__LINE__, l++, 4L);
	lequals(__LINE__, l--, 5L);
	lequals(__LINE__, l, 4L);
	iequals(__LINE__, c++, 5);
	iequals(__LINE__, c--, 6);
	iequals(__LINE__, c, 5);
#if ANSI
	{
	enum {ZERO, ONE, TWO} x = ivalue(0);
	long double ld = dvalue(1E9);

	iequals(__LINE__, x++, 0);
	iequals(__LINE__, x++, ONE);
	iequals(__LINE__, x--, TWO);
	iequals(__LINE__, x, 1);
	ldequals(__LINE__, ld++, 1E9);
	ldequals(__LINE__, ld--, 1E9 + 1);
	ldequals(__LINE__, --ld, 999999999);
	}
#endif
	} /* end c3_3_2_4 */

/*
 * 3.3.3 - Unary operators.
 */
static void c3_3_3() {}

/*
 * 3.3.3.1 - Prefix increment and decrement operators
 */
static void c3_3_3_1()
	{
	long *p = larray;
	long *q;
	double d = 2.2;
	int i = 3;
	long l = 4;
	char c = 5;

	/* these are tested in several places. make sure that ++ and --
	 * work right together.
	 * See prec*.c for combination of ++ and other ops.
	 * See bitfield.c for ++ with bitfield.
	 */
	lequals(__LINE__, *++p, 2L);
	lequals(__LINE__, *--p, 1L);
	q = p;
	lequals(__LINE__, *++p, *(q+=1));
	lequals(__LINE__, *--p, *(q-=1));

	/* make sure that they work on scalar types correctly */
	dequals(__LINE__, ++d, 3.2);
	dequals(__LINE__, --d, 2.2);
	iequals(__LINE__, ++i, 4);
	iequals(__LINE__, --i, 3);
	lequals(__LINE__, ++l, 5L);
	lequals(__LINE__, --l, 4L);
	iequals(__LINE__, ++c, 6);
	iequals(__LINE__, --c, 5);
	}












/*
 * 3.3.3.2 - Address and indirection operators.
 */
static void c3_3_3_2()
	{
	char c;
	char *p = &c;
	long l = 0;

#if ANSI
	{
	void (*pf)() = &c3_3_3_2;	/* & with fn designator */
	char a[2];
	char (*pa)[2] = &a;	/* & with lvalue of array type */
	struct { int x; } s;
	union { char c; float f; } u;

	checkthat(__LINE__, pf == c3_3_3_2);
	aequals(__LINE__, pa, &a);
	aequals(__LINE__, &s.x, &s);
	aequals(__LINE__, &u.c, &u.f);
	checkthat(__LINE__, &*pf == c3_3_3_2);
	}
#endif
	aequals(__LINE__, *&p, &c);
	aequals(__LINE__, &*p, &c);

#if (V7 || ANSI)
	/* casts of addresses are allowed */
	p = (char *)&l;
	*(long *)p = 17;
	lequals(__LINE__, l, 17L);
#endif
	}
















/*
 * 3.3.3.3 - Unary arithmetic operators
 */
static void c3_3_3_3()
	{
	float f = dvalue(1.1);
	double d = dvalue(2.2);
	int i = ivalue(3);
	long l = ivalue(4);
	char c = ivalue(5);
	char *p = &c, *q = 0;

	/* test on arithmetic operands */
#if ANSI
	checkthat(__LINE__, ~(unsigned)i == (UINT_MAX - 3));
	checkthat(__LINE__, ~(unsigned long)l == (ULONG_MAX - 4));
	fequals(__LINE__, +f, 1.1);
	dequals(__LINE__, +d, 2.2);
	iequals(__LINE__, +i, 3);
	lequals(__LINE__, +l, 4L);
	iequals(__LINE__, +c, 5);
/*	aequals(__LINE__, +p, &c); DELETED, ACCORDING TO LATEST ANSI */
#endif
	dequals(__LINE__, -d, -2.2);
	iequals(__LINE__, -i, -3);
	lequals(__LINE__, -l, -4L);
	iequals(__LINE__, -c, -5);

	/* a few miscellaneous checks */
	iequals(__LINE__, -i, 0-i);
	iequals(__LINE__, - -i, i);
	iequals(__LINE__, ~i, ~3);
	iequals(__LINE__, !i, 0);
	iequals(__LINE__, !i, 0 == i);
	iequals(__LINE__, !!i, 1);
	iequals(__LINE__, !!!i, 0);

	/* ! can take any scalar type */
	iequals(__LINE__, !p, 0);
	iequals(__LINE__, !q, 1);
	}









#if ANSI
#include <stddef.h>  /* for  size_t  below */
#endif
/*
 * 3.3.3.4 - The 'sizeof' operator
 */
static void c3_3_3_4()
	{
	int iarray[10];
	double *pd = 0;
	int i = 1;

	/* get the size of objects */
	checkthat(__LINE__, sizeof iarray == 10 * sizeof(int));
	checkthat(__LINE__, sizeof *pd == sizeof(double));

	/* works on types too */
	checkthat(__LINE__, sizeof (int [10]) == 10 * sizeof(int));
	checkthat(__LINE__, sizeof (struct x { int a, b, c;}) == sizeof (struct x));
	checkthat(__LINE__, sizeof(char) == 1);

#if ANSI
	/* the operand of sizeof is not evaluated */
	checkthat(__LINE__, sizeof(++i) == sizeof(int));
	iequals(__LINE__, i, 1);
	/* test the size of  sizeof  */
		{
		size_t usize = 0;
		unsigned char uc;

		checkthat(__LINE__, usize - 1 > 0);	/* must be an unsigned type */
		checkthat(__LINE__, sizeof(size_t) == sizeof(sizeof(int)));
		checkthat(__LINE__, sizeof(size_t) == sizeof(sizeof(const char *const)));
		checkthat(__LINE__, sizeof(size_t) == sizeof(sizeof(const char *)));
		checkthat(__LINE__, sizeof(size_t) == sizeof(sizeof(const int)));
		iequals(__LINE__, sizeof(+uc), sizeof(int));
		iequals(__LINE__, sizeof(-uc), sizeof(int));
		}
	checkthat(__LINE__, sizeof("123456") == 7);
#endif
	}









/*
 * 3.3.4 - Cast operators
 */
static void c3_3_4()
	{
	short i, j;
	long l, *pl = &l;
	char c, *pc = &c;
	double (*fp)();

	i = (char)(short)(long)(float)(double)(double)(float)(long)(short)(char)dvalue(3.0);
	iequals(__LINE__, i, ivalue(3));
	i = ivalue(-1);
	lequals(__LINE__, (long)i, -1L);
	iequals(__LINE__, (int)3.1416, 3);
	dequals(__LINE__, (double)4, 4.0);

	/* pointer to integer conversion (implem-def, strictly speaking) */
	l = (long)pc;
	aequals( - __LINE__, pc, (char *)l);
	/* and back again should preserve original value */
	pc = (char *)l;
	aequals( - __LINE__, pc, &c);

	/* pointer type conversion */
	l = 7;
	pc = (char *)pl;
	lequals(__LINE__, *(long *)pc, 7L);

	/* function pointer type conversion */
	fp = (double (*)())f;
	lequals(__LINE__, (*(long(*)())fp)(), 99L);
#if ANSI
	{
	float f = (float)1. / (float)2.;
	long double ld = dvalue(1.234 - 90 - 90 - 9);

	checkthat(__LINE__, f == .5F);
	f = ld;
	dequals(__LINE__, dvalue(f), (float)ld);
	}
#endif








#if ANSI8706																					/* 3.3.4 (cont.) */
	{
	struct known { int i; } s = {0};
	struct unknown *psu;
	char *pc;

	psu = (struct unknown *)avalue(&s);
	pc = (char *)avalue(psu);
	checkthat(__LINE__, pc == (char *)psu);
	}
#endif
	} /* end c3_3_4 */

#else /* if SKIP33 */

void c3_3() { pr_skip("c3_3: SKIPPED ENTIRELY\n"); }
#endif /* SKIP33 */

