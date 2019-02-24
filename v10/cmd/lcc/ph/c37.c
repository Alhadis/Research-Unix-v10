/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP37
/*
 * 3.7 - External Definitions
 */
#include "defs.h"

void c3_7_1();
static void c3_7_1a();
#if (ANSI || V7)
static void c3_7_1b();
#endif
static int seven();
void c3_7_2();

void c3_7()
	{
	Filename = "c37.c";
	c3_7_1(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18);
	c3_7_2();
	}

/*
 * 3.7.1  - Function definitions
 */

#if NEW_STYLE_FN_DEF
typedef void D;
void c3_7pro(double D, const char *pcc, unsigned char uc)	/* called around line 104 below */
	{
	dequals(__LINE__, D, 123.);
	iequals(__LINE__, pcc[0], 'a');
	iequals(__LINE__, uc, UCHAR_MAX);
	}
typedef int F(void);	/* examples from 3.7.1 */
F f37, g37;
int f37(void) { return 37; }
int g37() { return 37; }
F *Fp = f37;
#endif



																					/* 3.7.1 (cont.) */
extern void c3_7_1(i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z)
	register int i;
	register int j;
	register int k;
	register int l;
	register int m;
	register int n;
	register int o;
	register int p;
	register int q;
	register int r;
	register int s;
	register int t;
	register int u;
	register int v;
	register int w;
	register int x;
	register int y;
	register int z;
	{
	short ss;
	char c;
	float f;
	static long larray[] = {1,2,3,4,5};
#if (ANSI || V7)
	unsigned short us;
	UCHAR uc;
#endif

	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 2);
	iequals(__LINE__, k, 3);
	iequals(__LINE__, l, 4);
	iequals(__LINE__, m, 5);
	iequals(__LINE__, n, 6);
	iequals(__LINE__, o, 7);
	iequals(__LINE__, p, 8);
	iequals(__LINE__, q, 9);
	iequals(__LINE__, r, 10);
	iequals(__LINE__, s, 11);
	iequals(__LINE__, t, 12);
	iequals(__LINE__, u, 13);
	iequals(__LINE__, v, 14);
	iequals(__LINE__, w, 15);
	iequals(__LINE__, x, 16);
	iequals(__LINE__, y, 17);
	iequals(__LINE__, z, 18);


																					/* 3.7.1 (cont.) */
#if NEW_STYLE_FN_DEF
	/* a new-style fn def serves as a proto */
	c3_7pro(123, "ab", ULONG_MAX);
	iequals(__LINE__, f37(), 37);
	iequals(__LINE__, g37(), 37);
	iequals(__LINE__, Fp(), 37);
#endif

	/* test conversion of parameters */
	ss = SHRT_MAX;		/* -> int */
	c = CHAR_MAX;		/* -> int */
	f = 3.14;			/* -> double */
	c3_7_1a(0,0,0, ss, c, f, larray, seven); 
#if (ANSI || V7)
	us = MAX_USHORT;
	uc = MAX_UCHAR;
	c3_7_1b(0,0,0, (int)us, (int)uc, 0,0,0);
#endif

	}

static void c3_7_1a(x1,x2,x3, ss, c, f, larray, pf)
	/* order should not matter */
	/* no argument declaration defaults to int */
	int (*pf)();
	long *larray;
	double f;
	{
	long l;

	iequals(__LINE__, ss, (int)(short)SHRT_MAX);
	iequals(__LINE__, c, (int)(char)CHAR_MAX);
	fequals(__LINE__, f, 3.14);

	/* the array was passed as a pointer to the first element */
	for (l = 0; l < 5; ++l)
		lequals(__LINE__, l+1, larray[l]);
	for (l = 1; l <= 5; ++l)
		lequals(__LINE__, l, *larray++);

	/* the funtion name was passed as a pointer to the function */
	iequals(__LINE__, (*pf)(), 7);

	/* each parm is an lvalue */
	f = dvalue(1.);
	dequals(__LINE__, f, 1.);
	} /* end c3_7_1a */


																					/* 3.7.1 (cont.) */
static int seven()
	{
	return(7);
	}

#if (ANSI || V7)
/* no-argument-declaration defaults to int */
static void c3_7_1b(x1,x2,x3, us, uc, x4, x5, x6)
	{
	checkthat(__LINE__, us == (int)MAX_USHORT);	/* same implem-def convert as the arg */
	checkthat(__LINE__, uc == (int)MAX_UCHAR);	/* same implem-def convert as the arg */
	}
#endif




































/*
 * 3.7.2 - External object definitions
 */
int nodef;
#if ANSI
int hasdef;
#else
int hasdef = 4;
#endif

/* the following external object definition examples were written
 * by Plum for the X3J11 standard (slightly revised)
 */
#if ANSI
int i1 = 1;
static int i2 = 2;
extern int i3 = 3;
int i4;
static int i5;

int i1;
int i3;
int i4;

extern int i1, i2, i3, i4, i5;
#endif

void c3_7_2()
	{
#if ANSI
	iequals(__LINE__, i1, 1);
	iequals(__LINE__, i2, 2);
	iequals(__LINE__, i3, 3);
	iequals(__LINE__, i4, 0);
	iequals(__LINE__, i5, 0);
#endif

	/* some compilers will create nodef in the linker, some in the compiler.
	 * But all of them must be == 0.
	 */
	iequals(__LINE__, nodef, 0);
	iequals(__LINE__, hasdef, 4);
	}
#if ANSI
int hasdef = 4;
#endif




#else /* if SKIP37 */

void c3_7() { pr_skip("c3_7: SKIPPED ENTIRELY\n"); }
#endif /* SKIP37 */

