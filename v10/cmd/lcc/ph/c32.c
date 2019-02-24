/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP32
/*
 * 3.2 - Conversions.
 */
#include "defs.h"

static void c3_2_1_1();
static void c3_2_1_2();
static void c3_2_1_3();
static void c3_2_1_4();
static void c3_2_1_5();
static void c3_2_2_1();
static void c3_2_2_2();
static void c3_2_2_3();
 

void c3_2()
	{
	Filename = "c32.c";
	c3_2_1_1();
	c3_2_1_2();
	c3_2_1_3();
	c3_2_1_4();
	c3_2_1_5();
	c3_2_2_1();
	c3_2_2_2();
	c3_2_2_3();
	}

/*
 * 3.2.1.1 - Characters and integers
 */
static void c3_2_1_1()
	{
	char c;
	short ss;
#if ANSI
	signed char sc;
#endif



#if ANSI																					/* 3.2.1.1 (cont.) */
	UCHAR uc;
	USHORT us;
#endif

#if ANSI
/* check signed and unsigned integral promotions */
	sc = ivalue(4);
	ss = ivalue(5);
	dequals(__LINE__, (double)(sc-5), -1.);
	dequals(__LINE__, (double)(ss-6), -1.);
	uc = ivalue(4);
	us = ivalue(5);
	if (MAX_UCHAR <= MAX_INT)
		dequals(__LINE__, (double)(uc-5), -1.);
	else
		dequals(__LINE__, (double)(uc-5), (double)MAX_UINT);
	
	if (MAX_USHORT <= MAX_INT)
		dequals(__LINE__, (double)(us-6), -1.);
	else
		dequals(__LINE__, (double)(us-6), (double)MAX_UINT);
	
#endif

	/* sign of "plain" char is implem-def */
#if SIGNED_CHAR
	c = ivalue(-1);
	iequals(__LINE__, c, -1);
#endif
#if ANSI
#if UNSIGNED_CHAR
	c = ~0u;
	iequals(__LINE__, c, MAX_UCHAR);
#endif /* UNSIGNED_CHAR */
	/* demotion to shorter signed */
	sc = ivalue(-1);
	iequals(__LINE__, sc, -1);
#endif /* ANSI */
	ss = ivalue(-1);
	iequals(__LINE__, ss, -1);

#if ANSI
	uc = ~0u;
	checkthat(__LINE__, uc == MAX_UCHAR);
	us = ~0u;
	checkthat(__LINE__, us == MAX_USHORT);
#endif
	}

/*
 * 3.2.1.2 - Signed and unsigned integers
 */
static void c3_2_1_2()
	{
#if ANSI
	signed int si;
	int i;
	signed short int ssi;
	short int ss;
	signed long int sl;
	long int l;
	signed int *psi = &i;	/* allowable, compatible type */
	signed short int *pssi = &ss;
	signed long int *psli = &l;
	ULONG ul;
	struct {
		unsigned ubf2 : 2;
		signed   sbf2 : 2;
		int      ibf2 : 2;
		} bf;
	UCHAR uc=ivalue(4);
	USHORT us=ivalue(5);
	unsigned int ui;

	/* each of the pairs are compatible types */
	iequals(__LINE__, sizeof(signed int), sizeof(int));
	iequals(__LINE__, sizeof(signed short int), sizeof(short int));
	iequals(__LINE__, sizeof(signed long int), sizeof(long int));

	/* when unsigned integers are converted to representable signed values, the value is unchanged.  */
	uc = lvalue(MAX_UCHAR);
	sl = uc;
	/* allow for the rare case where sizeof(UCHAR) == sizeof(long) */
	if (sizeof(sl) > sizeof(uc))
		dequals(__LINE__, (double)sl, (double)MAX_UCHAR);
	us = lvalue(MAX_USHORT);
	sl = us;
	/* allow for the rare case where sizeof(USHORT) == sizeof(long) */
	if (sizeof(sl) > sizeof(us))
		dequals(__LINE__, (double)sl, (double)MAX_USHORT);

	checkthat(__LINE__, (unsigned char)(long)(UCHAR_MAX+1+ivalue(2)) == 2);
	checkthat(__LINE__, (unsigned long)(short)ivalue(-2) == (ULONG_MAX-1));
	bf.ubf2 = ivalue(-1);
	iequals(__LINE__, bf.ubf2, 3);
	bf.sbf2 = ivalue(-1);
	iequals(__LINE__, bf.sbf2, -1);
	bf.ibf2 = ivalue(-1);
	checkthat(__LINE__, bf.ibf2 == -1 || bf.ibf2 == 3);

	/* signed to unsigned of equal or greater length: if non-negative, value unchanged */									/* 3.2.1.2 (cont.) */
	ssi = ivalue(4);
	ui = ssi;
	checkthat(__LINE__, ui == 4);
	/* signed to unsigned of equal or greater length: if negative, converted modulo "largest plus one" */
	ssi = ivalue(-1);
	ul = (unsigned long) ivalue(ssi);
	checkthat(__LINE__, ul == MAX_ULONG);
	us = ssi;
	checkthat(__LINE__, us == MAX_USHORT);

	/* integer demoted to smaller unsigned: modulo "largest plus one" */
	sl = ivalue(-3);
	uc = sl;
	iequals(__LINE__, uc, MAX_UCHAR-2);
#endif
	}
































/*
 * 3.2.1.3 - floating and integral
 */
static void c3_2_1_3()
	{
#if ANSI
	float f;
	double d;
	char c;
	short ss;
	int si;
	long sl;
	unsigned int ui;
	unsigned char uc;
	unsigned short us;
	unsigned long ul;
	signed char sc;
	enum { a, b } en;
	struct { unsigned u : 7; signed i : 7; } su;

	/* conversion from floating->integral truncates positive numbers */
	f = dvalue(99.999);
	c = f;  iequals(__LINE__, c, ivalue(99));
	sc = f; iequals(__LINE__, sc, ivalue(99));
	uc = f; iequals(__LINE__, uc, ivalue(99));
	ss = f; iequals(__LINE__, ss, ivalue(99));
	us = f; iequals(__LINE__, us, ivalue(99));
	si = f; iequals(__LINE__, si, ivalue(99));
	ui = f; iequals(__LINE__, ui, ivalue(99));
	en = f; iequals(__LINE__, en, ivalue(99));
	su.u = f; iequals(__LINE__, su.u, ivalue(99));
	su.i = 33.333; iequals(__LINE__, su.i, ivalue(33));
	sl = f; lequals(__LINE__, sl, lvalue(99L));
	ul = f; lequals(__LINE__, ul, lvalue(99L));
	d = dvalue(99.999);
	c = d;  iequals(__LINE__, c, ivalue(99));
	sc = d; iequals(__LINE__, sc, ivalue(99));
	uc = d; iequals(__LINE__, uc, ivalue(99));
	ss = d; iequals(__LINE__, ss, ivalue(99));
	us = d; iequals(__LINE__, us, ivalue(99));
	si = d; iequals(__LINE__, si, ivalue(99));
	ui = d; iequals(__LINE__, ui, ivalue(99));
	en = d; iequals(__LINE__, en, ivalue(99));
	su.u = d; iequals(__LINE__, su.u, ivalue(99));
	su.i = 33.333; iequals(__LINE__, su.i, ivalue(33));
	sl = d; lequals(__LINE__, sl, lvalue(99L));
	ul = d; lequals(__LINE__, ul, lvalue(99L));
#endif
	}

/*
 * 3.2.1.4 - Floating types
 */
static void c3_2_1_4()
	{
	float f;
	double d;
#if ANSI
	long double l;
#endif

	/* float->double, etc. has no value change */
	f = dvalue(1.23456789012345);
	d = dvalue(f);
	dequals(__LINE__, f, d);
#if ANSI
	l = dvalue(f);
	ldequals(__LINE__, (long double)f, l);
	d = dvalue(1.23456789012345);
	l = d;
	ldequals(__LINE__, (long double)d, l);
#endif

	d = dvalue(.25);
	f = dvalue(d);
	checkthat(__LINE__, f == .25);

	/* conversion downward has undefined behavior if not representable */

	}




















/*
 * 3.2.1.5 - Usual arithmetic conversions 
 * These will be checked in greater detail in 'expressions'.
 */
static void c3_2_1_5()
	{
#if ANSI
	long double ld = dvalue(3.0);
	signed char sc = ivalue(4);
	char c = ivalue(5);
	int si = ivalue(6);
	short ss = ivalue(7);
	long sl = ivalue(8);
	unsigned int ui = ivalue(9);
	float f = ivalue(10);
	double d = ivalue(11);
	UCHAR uc = ivalue(12);
	USHORT us = ivalue(13);
	ULONG ul = ivalue(14);

	/* if either argument is long double, that is the type of the result */
	ldequals(__LINE__, ld+sc, 7.0L);

	/* if either argument is double, that is the type of the result */
	dequals(__LINE__, d+f, 21.0);
	dequals(__LINE__, d+si, 17.0);
	dequals(__LINE__, (double)f+si, 16.0);

	/* otherwise, if either float, then float */
	iequals(__LINE__, sizeof(d+c), sizeof(double));
	iequals(__LINE__, sizeof(f+c), sizeof(float));
	/* otherwise, int promotions are performed. Then, following rules: */

	/* if either unsigned long int, then that */
	ldequals(__LINE__, (long double)(ss-ul), (long double)(MAX_ULONG-6));
	/* otherwise, if long vs unsigned int:  */
	if (UINT_MAX <= LONG_MAX) /* if unsigned int can be held in long, then long */
		ldequals(__LINE__, sl-ui, -1);
	else							/* if unsigned int cannot be held in long, then unsigned long */
		ldequals(__LINE__, sl-ui, MAX_ULONG);
	/* otherwise, if either long int, then that */
	ldequals(__LINE__, (long double)(si-sl), (long double)(-2));
	iequals(__LINE__, sizeof(si-sl), sizeof(long));

	/* otherwise, if either unsigned int, then that */
	ldequals(__LINE__, (long double)(ss-ui), (long double)(MAX_UINT - 1));
	/* integral widening has been tested in 3.2.1.1 */
#endif
	}

/*
 * 3.2.2.1 - Lvalues and function designators
 */
#if ANSI
static char iarray[6] = "abcdef";
#else
static char iarray[6] = "abcde";
#endif
static void c3_2_2_1()
	{
	char *p;

	/* the name of an array is equivalent to a pointer to the first element */
	aequals(__LINE__, iarray, &iarray[0]);

	/* except for use in 'sizeof' */
	iequals(__LINE__, sizeof(iarray), 6 * sizeof(char));
#if ANSI
	/* ... or as operand of & */
	{
	char (*pa)[6] = &iarray;

	checkthat(__LINE__, pa == &iarray);
	}
#endif

	/* or except as a string literal initializer */
	iequals(__LINE__, iarray[0], 'a');
	iequals(__LINE__, iarray[1], 'b');
	iequals(__LINE__, iarray[2], 'c');
	iequals(__LINE__, iarray[3], 'd');
	iequals(__LINE__, iarray[4], 'e');
	iequals(__LINE__, iarray[5], ANSI ? 'f' : '\0');

	/* similarly we can overaddress and check a pointer */
	for (p = iarray; p < &iarray[6]; ++p)
		;
	aequals(__LINE__, p, iarray+6);

	/* function name takes on type 'pointer to function returning type' */
	checkthat(__LINE__, c3_2_2_1 != 0);
#if ANSI
	checkthat(__LINE__, &c3_2_2_1 == c3_2_2_1);
#endif
	}





/*
 * 3.2.2.2 - Void
 */
static void c3_2_2_2()
	{
	static int iterations = 0;

	if (iterations++ == 0)
#if ANSI8709
		(void)c3_2_2_2();
#else
		c3_2_2_2();
#endif
	else
		{
		(void)1;
		(void)iequals(__LINE__, 1, ivalue(1));
		(void)(iterations + 1);
		iequals(__LINE__, 7, ((void)3, ivalue(7)));
		(void)++iterations;
		return;
		}
	iequals(__LINE__, iterations, 3);
	}


























/*
 * 3.2.2.3 - Pointers
 */
static void c3_2_2_3()
	{
	long li, *pl = (long *)avalue(&li);
	double d, *pd = (double *)avalue(&d);
	char *pc;

	int (*ipf)();
#if ANSI
	/* pointer to void can be converted to any ptr-to-object */
	void *gp;

	gp = avalue(pl);
	aequals(__LINE__, pl, gp);
#endif
#if ANSI8706
	{
	struct unknown *pu;

	pu = avalue(gp);
	gp = avalue(pu);
	pu = avalue(gp);
	aequals(__LINE__, gp, pu);
	}
#endif

	/* A pointer can be converted to one of less strict alignment
	 * and back again without loss of information.  (See 3.3.4.)
	 */
	li = ivalue(1234); pc = (char *)pl; do_nothing(&pc, &pl); pl = (long *)pc;
	lequals(__LINE__, *pl, 1234L);

	d = dvalue(3.1416); pc = (char *)pd; do_nothing(&pc, &pd); pd = (double *)pc;
	dequals(__LINE__, *pd, 3.1416);

	/* null pointers */
	pc = 0;
	do_nothing(&pc);
	checkthat(__LINE__, pc == 0);
	pd = 0;
	do_nothing(&pd);
	checkthat(__LINE__, pd == 0);
	pd = &d;	/* again */





#if ANSI8712																				/* 3.2.2.3 (cont.) */
	{
	void *gp;
	struct unknown *pu, *pu2;
	const long *pcl;

	/* a ptr-to-void may be converted to/from a ptr to any object type */
	li = ivalue(1234); gp = pl; do_nothing(&gp, &pl); pl = gp;
	lequals(__LINE__, *pl, 1234L);

	d = dvalue(3.1416); gp = pd; do_nothing(&gp, &pd); pd = gp;
	dequals(__LINE__, *pd, 3.1416);

	/* a ptr-to-void may be converted to/from a ptr to any incomplete type */
	pu = (struct unknown *)pl; gp = pu; pu2 = gp;
	checkthat(__LINE__, pu == pu2);

	/* qualifiers do not affect comparisons */
	pcl = (const long *)pl;
	checkthat(__LINE__, pcl == pl);

	/* null pointer can be e.g. 0L or (void *)0 */
	pc = 0L;
	do_nothing(&pc);
	checkthat(__LINE__, pc == 0L);
	pd = 0L;
	do_nothing(&pd);
	checkthat(__LINE__, pd == 0L);

	pc = (void *)0;
	do_nothing(&pc);
	checkthat(__LINE__, pc == (void *)0);
	pd = (void *)0;
	do_nothing(&pd);
	checkthat(__LINE__, pd == (void *)0);

	/* two null pointers, thru different cast sequences, compare equal */
	checkthat(__LINE__, (char *)(long *)pc == (char *)(float *)pd);
	}
#endif
	}
#else /* if SKIP32 */

void c3_2() { pr_skip("c3_2: SKIPPED ENTIRELY\n"); }
#endif /* SKIP32 */

/*
 * Section 3.2.2.4 (const and volatile) has been moved to 3.5.3
 * See c35.c
 */
