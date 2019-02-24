/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP33B
/*
 * c33b.c - extension of 3.3 (Expressions).
 */
#include "defs.h"
static void c3_3_5();
static void c3_3_6();
static void c3_3_7();
static void c3_3_8();
static void c3_3_9();
static void c3_3_10();
static void c3_3_11();
static void c3_3_12();
static void c3_3_13();
static void c3_3_14();
static void c3_3_15();
static void c3_3_16();
static void c3_3_16_1();
static void c3_3_16_2();
static void c3_3_17();
static long larray[10] = {0,1,2,3,4,5,6,7,8,9};
void c3_3b()
	{
	Filename = "c33b.c";
	c3_3_5();
	c3_3_6();
	c3_3_7();
	c3_3_8();
	c3_3_9();
	c3_3_10();
	c3_3_11();
	c3_3_12();
	c3_3_13();
	c3_3_14();
	c3_3_15();
	c3_3_16();
	c3_3_16_1((generic_ptr)Filename);
	c3_3_16_2();
	c3_3_17();
	}

/*
 * 3.3.5 - Multiplicative operators
 */
static void c3_3_5()
	{
	double d = 1.9, e = 2.7;
	int i = 4, j = 7;

	/* commutativity */
	iequals(__LINE__, i*j, j*i);

	/* associates L->R */
	dequals(__LINE__, j % i * d, d * 3);

	/* "usual arith conversions" (ANSI) are value-preserving */
#if ANSI
	{
	unsigned char uc = 5;

#if MAX_UCHAR <= MAX_INT
	checkthat(__LINE__, uc / -2 == -3 || uc / -2 == -2);
#endif
	}
#endif

	/* positive result of division must round toward 0 */
	iequals(__LINE__, j / i, 1);
	iequals(__LINE__, -j / -i, 1);

	/* mixed mode arithmetic -- this is done as double */
	iequals(__LINE__, (int)(d * i), 7); 
	iequals(__LINE__, (int)(i * d), 7); 
	iequals(__LINE__, (int)(i / d), 2); 

	/* but not this */
	iequals(__LINE__, i % (int)e, 0); 

	/* definitive property */
	iequals(__LINE__, (i/j)*j + i%j, i);
	i = -4;
	iequals(__LINE__, (i/j)*j + i%j, i);

	}







/*
 * 3.3.6 - Additive operators
 */
static void c3_3_6()
	{
	double d = 4.2;
	long *pd1, *pd2;
	int i = 1;

	/* check out mixed mode addition/subtraction */
	dequals(__LINE__, d+ivalue(7), 11.2);

	/* adding/subtracting an integral expression from a pointer */
	pd1 = &larray[4];
	pd2 = &larray[7];
	lequals(__LINE__, *(pd1 - ivalue(3)), 1L);
	lequals(__LINE__, *(pd2 - ivalue(3)), 4L);

	/* pointer - pointer */
	iequals(__LINE__, pd2 - pd1, 3);
	iequals(__LINE__, pd1 - pd2, -3);
	pd1 = &larray[9];
	pd2 = pd1 + 1;
	checkthat(__LINE__, pd2 - pd1 == 1);

	/* no re-grouping, except by "as-if" rule */
#if ANSI
	{
	unsigned ui = MAX_UINT;
	ULONG ul = 0;
	double d1 = dvalue((DBL_MAX/3.)*2. + DBL_MAX/10.);

	ul = (ui + 1) + ul;
	checkthat(__LINE__, ul == 0L);
#define SUM (d1 + -(DBL_MAX/3.)*2.)
	dequals(__LINE__, SUM + SUM, dvalue(DBL_MAX/10. + DBL_MAX/10.)); /* no re-grouping */
	}
#endif
	} /* end c3_3_6 */











/*
 * 3.3.7 - Bitwise shift operators
 */
static void c3_3_7()
	{
	unsigned char c = 1;
	int i = 4;
	unsigned int ui = 0xffff;
	int count;

	/* how many bits in a char ? */
	for (count = 0; c; c <<= 1, ++count)
		;
	/* bits in a long */
	count *= sizeof(long);

#if !ANSI
	/* before ANSI, if either operand was long, the shift was long */
	lequals(__LINE__, 1 << (count - 1L), 1L << (long)(count - 1L));
#else
	iequals(__LINE__, sizeof(c << 2L), sizeof(int));
	iequals(__LINE__, count, CHAR_BIT * sizeof(long));
#endif

	/* make sure that 'vacated bits' are filled with 0 */
	checkthat(__LINE__, ((int)0xff << i)  == (int)0xff0); 

	/* shift of unsigned must 0 fill */
	iequals(__LINE__, (ui >> i), (int)0x0fff); 

	/* right shift of signed item -- behavior is implem-defined */
	checkthat( - __LINE__, (~0 >> 1) == ~0 || (~0 >> 1) == ((unsigned)~0 >> 1));
	}

















/*
 * 3.3.8 - Relational operators
 */
static void c3_3_8()
	{
	long *pl1 = &larray[3];
	long *pl2 = &larray[5];
	double d = 11.0;
	unsigned ui1 = 5;
	int i = -2;

	/* (non-intuitive?) integer promotions */
	checkthat(__LINE__, ui1 < i);

	/* value context */
	iequals(__LINE__, pl1 < pl2, 1);
	iequals(__LINE__, d > *pl1, 1);
	iequals(__LINE__, pl1 <= pl2, 1);
	iequals(__LINE__, d >= *pl1, 1);

	/* condition context */
	if(pl1 < pl2)
		;
	else
		complain(__LINE__);
	if(d > *pl1)
		;
	else
		complain(__LINE__);

	if(pl1 <= pl2)
		;
	else
		complain(__LINE__);
	if(d >= *pl1)
		;
	else
		complain(__LINE__);

	/* check out associativity */
	iequals(__LINE__, pl1[0] < d < pl1[1] , 1);
	iequals(__LINE__, pl1[0] < d < 1 , 0);
	iequals(__LINE__, pl1[0] < d <= 1 , 1);
	iequals(__LINE__, *pl1 > d >= 0, 1);






#if ANSI8706																				/* 3.3.8 (cont.) */
	{
	struct known { int i; } s = {0};
	struct unknown *psu;
	void *gp; volatile void *vgp;

	psu = (struct unknown *)&s;
	vgp = gp = psu;
	checkthat(__LINE__, gp == psu);
	iequals(__LINE__, gp < vgp, 0);
	iequals(__LINE__, (const struct unknown *)psu < (struct unknown *)avalue(psu), 0);
	}
#endif
#if ANSI8709
	iequals(__LINE__, (const char *)avalue(pl1) < (char *)avalue(pl1), 0);
#endif

	/* special cases for "last-plus-one" members */
	{
	long a[10];
	long *p = &a[9];
	iequals(__LINE__, &a[9] < &a[10], 1);
	iequals(__LINE__, p+1 > p, 1);
	iequals(__LINE__, p+1 < p, 0);
	iequals(__LINE__, &a[9] < a+10 , 1);
	}

	/* result has type int */
#if ANSI
	{ double x=0, y=0; iequals(__LINE__, sizeof(x < y), sizeof(int)); }
#endif
	} /* end c3_3_8 */


















/*
 * 3.3.9 - Equality operators
 */
static void c3_3_9()
	{
	long *pl1 = &larray[4];
	long *pl2 = &larray[6];
	generic_ptr gp = (generic_ptr)pl1;
	void (*p3_8)() = c3_3_8;
	void (*p3_9)() = c3_3_9;

	/* equality of pointers */
	checkthat(__LINE__, p3_8 == c3_3_8);
	checkthat(__LINE__, p3_9 != c3_3_8);
#if ANSI
	checkthat(__LINE__, gp == pl1);
#else
	checkthat(__LINE__, gp == (generic_ptr)pl1);
#endif
#if ANSI8709
	checkthat(__LINE__, (char *)pl1 == (void *)pl1);
	checkthat(__LINE__, (const char *)pl1 == (char *)pl1);
	checkthat(__LINE__, pl1 != (void *)0);
	checkthat(__LINE__, pl1 != (const void *)pl2);
#endif

	/* value context */
	iequals(__LINE__, pl1 == &larray[4], 1);
	iequals(__LINE__, pl1 == pl2, 0);
	iequals(__LINE__, pl1 != 0, 1);
	iequals(__LINE__, *pl1 != *pl2, 1);

	/* condition context */
	if (pl1 == &larray[4])
		;
	else
		complain(__LINE__);

	if (pl1 != pl2)
		;
	else
		complain(__LINE__);
	if (pl1 != 0)
		;
	else
		complain(__LINE__);
	if (*pl1 != *pl2)
		;
	else
		complain(__LINE__);
																							/* 3.3.9 (cont.) */
	/* associativity */
	iequals(__LINE__, *pl1 == *pl2 == 1, 0);
	iequals(__LINE__, *pl1 != *pl2 == 1, 1);
	iequals(__LINE__, pl1 < pl2 != pl2 < pl1, 1);

	/* result has type int */
#if ANSI
	{ double x=0, y=0; iequals(__LINE__, sizeof(x != y), sizeof(int)); }
#endif
	}







































/*
 * 3.3.10 - Bitwise AND operator
 */
static void c3_3_10()
	{
	int a = 7, b = 2;
	long l = 6;
#if ANSI
	iequals(__LINE__, sizeof(a & l), sizeof(long));
#endif
	lequals(__LINE__, a & l, 6L);
	iequals(__LINE__, a & b, 2);
	iequals(__LINE__, b & a, 2);
	iequals(__LINE__, a & 0, 0);
	iequals(__LINE__, a & ~0, a);
	}
/*
 * 3.3.11 - Bitwise exclusive OR operator
 */
static void c3_3_11()
	{
	int a = 7, b = 2;
	long l = 6;
#if ANSI
	iequals(__LINE__, sizeof(a ^ l), sizeof(long));
#endif
	lequals(__LINE__, a ^ l, 1L);
	iequals(__LINE__, a ^ b, 5);
	iequals(__LINE__, b ^ a, 5);
	a = a^b; b = a^b; a = a^b;	/* swap a and b */
	iequals(__LINE__, a, 2);
	iequals(__LINE__, b, 7);
	}
/*
 * 3.3.12 - Bitwise OR operator
 */
static void c3_3_12()
	{
	int a = 5, b = 2;
	long l = 6;
#if ANSI
	iequals(__LINE__, sizeof(a | l), sizeof(long));
#endif
	lequals(__LINE__, a | l, 7L);
	iequals(__LINE__, a | b, 7);
	iequals(__LINE__, b | a, 7);
	iequals(__LINE__, a | 0, a);
	iequals(__LINE__, a | ~0, ~0);
	}

/*
 * 3.3.13 - Logical AND operator
 */
static void c3_3_13()
	{
	int zero = 0;
	int one = 1;
	extern int Side;

	/* side effect checks in a value producing context */
	Side = 0;
	scheck(__LINE__, ++Side && zero, 1, 0); 
	scheck(__LINE__, ++Side && one, 1, 1); 
	scheck(__LINE__, zero && ++Side, 0, 0);
	scheck(__LINE__, one && ++Side, 1, 1);

	/* side effects in a condition producing context */
	if (++Side && zero)
		complain(__LINE__);
	iequals(__LINE__, Side--, 1);

	if (++Side && one)
		;
	else
		complain(__LINE__);
	iequals(__LINE__, Side--, 1);

	if (zero && ++Side)
		complain(__LINE__);
	iequals(__LINE__, Side, 0);

	if (one && ++Side)
		;
	else
		complain(__LINE__);
	iequals(__LINE__, Side--, 1);

#if ANSI
	/* result is of type int */
	{ double x=0, y=0; iequals(__LINE__, sizeof(x && y), sizeof(int)); }
#endif
	/* operands are of scalar type */
	{ double x=0, y=0; iequals(__LINE__, x && y, 0); }
	{ generic_ptr x=0, y=0; iequals(__LINE__, x && y, 0); }
	{ enum {z} x=0, y=0; iequals(__LINE__, x && y, 0); }
	} /* end c3_3_14 */




/*
 * 3.3.14 - Logical OR operator
 */
static void c3_3_14()
	{
	int zero = 0;
	int one = 1;
	extern int Side;

	/* side effect checks in a value producing context */
	Side = 0;
	scheck(__LINE__, ++Side || zero, 1, 1); 
	scheck(__LINE__, ++Side || one, 1, 1); 
	scheck(__LINE__, zero || ++Side, 1, 1);
	scheck(__LINE__, one || ++Side, 0, 1);

	/* side effects in a condition producing context */
	if (++Side || zero)
		;
	else
		complain(__LINE__);
	iequals(__LINE__, Side--, 1);

	if (++Side || one)
		;
	else
		complain(__LINE__);
	iequals(__LINE__, Side--, 1);

	if (zero || ++Side)
		;
	else
		complain(__LINE__);
	iequals(__LINE__, Side--, 1);

	if (one || ++Side)
		;
	else
		complain(__LINE__);
	iequals(__LINE__, Side, 0);

#if ANSI
	/* result is of type int */
	{ double x=0, y=0; iequals(__LINE__, sizeof(x || y), sizeof(int)); }
#endif
	/* operands are of scalar type */
	{ double x=0, y=0; iequals(__LINE__, x || y, 0); }
	{ generic_ptr x=0, y=0; iequals(__LINE__, x || y, 0); }
	{ enum {z} x=0, y=0; iequals(__LINE__, x || y, 0); }
	} /* end c3_3_13 */
/*
 * 3.3.15 - Conditional operator
 */
static int If1, If2;
void f1(){If1 = 7;}
void f2(){If2 = 11;}
static int iarray[] = {7, 11};
static void c3_3_15()
	{
	int one = ivalue(1), zero = ivalue(0);
	int *p1 = &one, *p0 = &zero;
	generic_ptr gp = (generic_ptr)p1;
	extern int Side;
	char *pc;

	Side = 0;
	/* side effect checks in a value producing context */
	scheck(__LINE__, ++Side ? one : zero, 1, 1); 
	scheck(__LINE__, zero ? one : ++Side, 1, 1);
	scheck(__LINE__, one ? one : ++Side, 0, 1);

	/* side effects in a condition producing context */
	if (++Side ? one : zero)
		;
	else
		complain(__LINE__);
	iequals(__LINE__, Side--, 1);

	if (zero ? one : ++Side)
		;
	else
		complain(__LINE__);
	iequals(__LINE__, Side--, 1);

	if (one ? ++Side: zero)
		;
	else
		complain(__LINE__);
	iequals(__LINE__, Side, 1);

#if ANSI8709
	checkthat(__LINE__, (one ? Side=5 : 6)                    == 5);
	checkthat(__LINE__, (one ?  2,3  :  4)                    == 3);
	checkthat(__LINE__, (one ? (char *)p1 : (const char *)p0) == (const char *)p1);
	checkthat(__LINE__, (one ? (char *)p1 : (void *)p0)       == /* (void *) */ p1);
	pc = zero ? (char *)p1 : 0;
	checkthat(__LINE__, pc == 0);
	{ double x=0; char y=1; checkthat(__LINE__, sizeof(one ? x : y) == sizeof(x) && (one ? x : y) == 0); }
#endif

	/* arms can be scalar, void, struct, pointer */																		/* 3.3.15 (cont.) */
	iequals(__LINE__, one ? one : zero, one); 
	iequals(__LINE__, zero ? one : zero, zero); 
	aequals(__LINE__, one ? p1 : p0, p1);
	aequals(__LINE__, zero ? p1 : p0, p0);
	aequals(__LINE__, one ? NULL : p0, NULL);
	aequals(__LINE__, zero ? p1 : NULL, NULL);
	iequals(__LINE__, *(one ? p1 : p0), 1); 
	iequals(__LINE__, *(zero ? p1 : p0), 0); 
#if ANSI
	aequals(__LINE__, one ? gp : p0, gp);
	checkthat(__LINE__, (zero ? &f1 : 0) == 0);
#endif
	checkthat(__LINE__, (one ? f1 : 0) == f1);
#if ANSI
	/* function calls in both arms -- only one should be called */
	If1 = If2 = 0;
	one ? f1(): f2();
	iequals(__LINE__, If1, 7);
	iequals(__LINE__, If2, 0);
	If1 = If2 = 0;
	zero ? f1(): f2();
	iequals(__LINE__, If1, 0);
	iequals(__LINE__, If2, 11);
#endif
	/* function pointers in both arms */
	If1 = If2 = 0;
	(*(one ? f1 : f2))();
	iequals(__LINE__, If1, 7);
	iequals(__LINE__, If2, 0);
	If1 = If2 = 0;
	(*(zero ? f1 : f2))();
	iequals(__LINE__, If1, 0);
	iequals(__LINE__, If2, 11);

	/* struct/union in arms are tested in structures() in c33c.c */

	/* check out nested question ops */
	iequals(__LINE__,
		zero ? 1 : zero ? 2 : zero ? 3 : zero ? 4 : 5, 5);
	iequals(__LINE__,
		one ? one ? one ? one ? one ? 1 : 2 : 3 : 4 : 5 : 6, 1);

	/* questions as array indexes */
	iequals(__LINE__, iarray[zero ? zero : one], 11);
	iequals(__LINE__, iarray[one ? zero : one], 7);




	/* question in a switch */																							/* 3.3.15 (cont.) */
	switch (one ? one : zero)
		{
		case 1 :
			break;
		default :
			complain(__LINE__);
		}

	/* conditional is a sequence point */
	{
	int i = ivalue(1);

	iequals(__LINE__, i++ == 1 ? i : 0, ivalue(2));
	iequals(__LINE__, i++ != 2 ? 0 : i, ivalue(3));
	}

	} /* end c3_3_15 */











/*
 * 3.3.16 - Assignment operators
 */
static void c3_3_16()
	{
	int i = 3;
	double d = 4.0;
	/* takes type and value from the left operand */
	iequals(__LINE__, i = d, 4);
	i = 2;
	dequals(__LINE__, d = i, 2.0);
	iequals(__LINE__, i += d, 4);
#if ANSI
	{ char x=0; double y=1; checkthat(__LINE__, sizeof(x = y) == sizeof(x) && (x = y) == 1); }
#endif
	}





/*
 * 3.3.16.1 - Simple assignment
 */
static void c3_3_16_1(q)
	generic_ptr q;
	{
	char *p;
	unsigned ui;

#if ANSI8712
	struct unk;	/* structure of unkown contents */

/* The constraints for assignment are quite detailed:
 * Define a "sufficiently-qualified" (or "suf-qual") left operand type
 * to be one that has all the qualifiers of the type pointed to by the
 * right operand (and perhaps others).
 * Let qual=qualified, arith=arithmetic, ptr=pointer.
 * Then, as the Standard says, one of the folllowing
 * shall hold:
 
	Left operand				Right operand
1.	qual arith type				arith type
2.	unqual arith type			arith type
3.	qual struct type			compatible struct type
4.	qual union type				compatible union type
5.	unqual struct type			compatible struct type
6.	unqual union type			compatible union type
7.	ptr to suf-qual type		ptr to compatible qual type
8.	ptr to suf-qual type		ptr to compatible unqual type
9.	ptr to suf-qual object		ptr to qual void
10.	ptr to suf-qual object		ptr to unqual void
11.	ptr to suf-qual incomplete	ptr to qual void
12.	ptr to suf-qual incomplete	ptr to unqual void
13.	any ptr						null ptr constant	...
 */
	/* 1 */ { volatile int a; int b={1}; a = b; iequals(__LINE__, a, b); }
	/* 2 */ { int a; int b={2}; a = b; iequals(__LINE__, a, b); }
	/* 3 */ { volatile struct s {int i;} a; struct s b={3}; a = b; iequals(__LINE__, a.i, b.i); }
	/* 4 */ { volatile union u {int i;} a; union u b={4}; a = b; iequals(__LINE__, a.i, b.i); }
	/* 5 */ { struct s {int i;} a; struct s b={5}; a = b; iequals(__LINE__, a.i, b.i); }
	/* 6 */ { union u {int i;} a; union u b={6}; a = b; iequals(__LINE__, a.i, b.i); }
	/* 7 */ { const int *a; const signed *b=q; a = b; aequals(__LINE__, a, b); }
	/* 8 */ { const int *a; signed *b=q; a = b; aequals(__LINE__, a, b); }
	/* 9 */ { const int *a; const void *b=q; a = b; aequals(__LINE__, a, (int *)b); }
	/* 10*/ { volatile int *a; void *b=q; a = b; aequals(__LINE__, (int *)a, (int *)b); }
	/* 11*/ { const struct unk *a; const void *b=q; a = b; aequals(__LINE__, a, (struct unk *)b); }
	/* 12*/ { struct unk *a; void *b=q; a = b; aequals(__LINE__, a, (struct unk *)b); }
	/* 13*/ { struct unk *a; a = (void *)0L; aequals(__LINE__, a, 0); }
#endif

	/* struct/union assignments are done in structures() in c33c.c */
										/* 3.3.16.1 (cont.) */

	/* assignment of pointers */
	p = q;
	aequals(__LINE__, p, Filename);
	q = 0L;
	aequals(__LINE__, q, NULL);
	q = p;
	aequals(__LINE__, p, q);

	/* integral constant expression with value 0 */
	p = 0;
	aequals(__LINE__, p, NULL);

#if (V7 || ANSI)
	{
	UCHAR c;

	/* right side values are converted to the type on the left */
	c = MAX_ULONG;
	iequals(__LINE__, c, MAX_UCHAR);
	}
#endif

	/* right side values are converted to the type on the left */
	ui = MAX_ULONG;
	checkthat(__LINE__, ui == MAX_UINT);
	}






















/*
 * 3.3.16.2 - Compound assignment
 */
static int *fpi() { ++If1; return & If2;}
static void c3_3_16_2()
	{
	long *pl1 = &larray[4];
	long *pl2 = &larray[6];
	char c = 10;
	double d = 1.21;

	/* allow pointer ops on += and -= */
	aequals(__LINE__, pl1+=2, pl2);
	aequals(__LINE__, pl1-=6, larray);
	/* make sure that the left side is only evaluated once */
	If2 = ivalue(5);
	If1 = ivalue(0);
	*fpi()*=2;
	iequals(__LINE__, If1, 1);
	iequals(__LINE__, If2, 10);
	*fpi()/=2;
	iequals(__LINE__, If1, 2);
	iequals(__LINE__, If2, 5);
	*fpi()%=2;
	iequals(__LINE__, If1, 3);
	iequals(__LINE__, If2, 1);
	*fpi()+=2;
	iequals(__LINE__, If1, 4);
	iequals(__LINE__, If2, 3);
	*fpi()-=2;
	iequals(__LINE__, If1, 5);
	iequals(__LINE__, If2, 1);
	*fpi()<<=2;
	iequals(__LINE__, If1, 6);
	iequals(__LINE__, If2, 4);
	*fpi()>>=2;
	iequals(__LINE__, If1, 7);
	iequals(__LINE__, If2, 1);
	*fpi()&=7;
	iequals(__LINE__, If1, 8);
	iequals(__LINE__, If2, 1);
	*fpi()^=7;
	iequals(__LINE__, If1, 9);
	iequals(__LINE__, If2, 6);
	*fpi()|=1;
	iequals(__LINE__, If1, 10);
	iequals(__LINE__, If2, 7);


																						/* 3.3.16.2 (cont.) */
	/* make sure that mixed mode ops are done right */
	iequals(__LINE__, c *= d, 12);
	iequals(__LINE__, c /= d, 9);
	}

/*
 * 3.3.17 - Comma operator
 */
static void c3_3_17()
	{
	If1 = ivalue(0);
	iequals(__LINE__, (If1++,If1++,If1++,If1++,If1++,If1++,0), 0);
	iequals(__LINE__, If1, 6);
	iequals(__LINE__, (If1 = 6, If1 >>= 1, If1 + 2), 5);
	If2 = ivalue(5);
	If1 = ivalue(0);
	iequals(__LINE__, (*fpi(), If1), 1);
#if ANSI8809
	{
	char arr[100];
	char c;

	/* array-conversion takes place in comma (3.2.2.1 doesn't mention it), ... */
	/* but integral conversions do not take place (because no mention in Semantics of comma */
	iequals(__LINE__, sizeof(0,arr), sizeof(&arr[0]));
	iequals(__LINE__, sizeof(0,c), ivalue(1));
	}
#endif
	}

#else /* if SKIP33B */

void c3_3b() { pr_skip("c3_3b: SKIPPED ENTIRELY\n"); }
#endif /* SKIP33B */

