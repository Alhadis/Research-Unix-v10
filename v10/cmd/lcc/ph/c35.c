/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP35
/*
 * 3.5 - Declarations
 */
#include "defs.h"




static void c3_5_1();
static void c3_5_2();
static void c3_5_2_1();
static void c3_5_2_2();
static void c3_5_2_3();
static void c3_5_3();
static void c3_5_4();
static void c3_5_4_1();
static void c3_5_4_2();
static void c3_5_4_3();
static void c3_5_5();
static void c3_5_6();
static void c3_5_7();
extern void bitfields();

void c3_5()
	{
	Filename = "c35.c";
	c3_5_1();
	c3_5_2();
	c3_5_2_1();
	c3_5_2_2();
	c3_5_2_3();
	c3_5_3();
	c3_5_4();
	c3_5_4_1();
	c3_5_4_2();
	c3_5_4_3();
	c3_5_5();
	c3_5_6();
	c3_5_7();
	bitfields();
	}


/*
 * 3.5.1 - Storage-class specifiers
 */
#if ANSI
extern void (*pc3_5_2)() = c3_5_2;	/* extern allowed with initializer */
#else
void (*pc3_5_2)() = c3_5_2;
#endif
static void c3_5_1()
	{
	/* just check for existence of the keywords here */
	auto int i;
	register int j;
	static int k;
	extern int l;
	typedef int M;
	M m;

	i = j = k = l = m = 0;
	checkthat(__LINE__, !(i || j || k || l || m));
	checkthat(__LINE__, pc3_5_2 == c3_5_2);
	}
int l = 0;

























/*
 * 3.5.2 - Type specifiers
 */
static void c3_5_2()
	{
	/* verify that all forms are handled */
	int x1;
	short int x2;
	short x3;
	long x4;
	int long x5;
	unsigned x6;
	int unsigned x7;
	char x8;
#if (ANSI || V7)
	UCHAR x9;
	USHORT x10;
	int USHORT x11;
	ULONG x12;
	ULONG int x13;
#endif

#if ANSI
	typedef int INT;
	typedef signed SIGNED;
	{
	signed char x14;
	short signed x15;
	short int signed x16;
	signed long x17;
	signed int long x18;
	signed x19;
	signed int x20;
	double long x21;
	const x22 = 22;
	int short const x23 = 23;
	volatile x24;
	long volatile int x25;
	const INT x26 = 26;
	INT volatile x27;
	INT const volatile x27a = 27;

	checkthat(__LINE__, sizeof(x24) == sizeof(int));
	}
#endif /* ANSI */





#if ANSI8712																			/* 3.5.2 (cont.) */
#define COMPATIBLE(t1, t2) { t1 v1={0}, *p1=&v1; t2 v2={0}, *p2=&v2; p1=p2; \
	checkthat(__LINE__, 0==mem_cmp((void *)p1, (void *)p2, sizeof(*p1))); }
#else
#define COMPATIBLE(t1, t2) { }
#endif
#if ANSI
	{
	struct s { int i:2; unsigned u:2; signed si:2; INT it:2; SIGNED st:2; } s = {-1, -1, -1, -1, -1};

	iequals(__LINE__, s.u, 3);
	iequals(__LINE__, s.si, -1);
	iequals(__LINE__, s.st, -1);
#if ANSI8804
	if (s.i == 3)			/* if "plain int" bitfield is unsigned */
		iequals(__LINE__, s.it, 3);
	else if (s.i == -1)		/* else if "plain int" bitfield is signed */
		iequals(__LINE__, s.it, -1);
	else
		complain(__LINE__);	/* should take one of previous choices */
#endif /* ANSI8804 */

	COMPATIBLE(short, signed short);
	COMPATIBLE(short, signed short int);
	COMPATIBLE(unsigned short, unsigned short int);
	COMPATIBLE(int, signed int);
	COMPATIBLE(signed, signed int);
	COMPATIBLE(unsigned, unsigned int);
	COMPATIBLE(long, signed long);
	COMPATIBLE(long, signed long int);
	COMPATIBLE(unsigned long, unsigned long int);
	COMPATIBLE(long unsigned, long int unsigned);
	COMPATIBLE(short unsigned, unsigned int short);

	}
#endif /* ANSI */

	{
	float x28;
	double x29;
	void (*x30)();
	static x31 = 31;

	checkthat(__LINE__, sizeof(x31) == sizeof(int));
	iequals(__LINE__, x31, 31);
	}
	} /* end c3_5_2 */



/*
 * 3.5.2.1 - Structure and union specifiers.
 */
static void c3_5_2_1()
	{
	/* structures and unions have the same form */
	struct S1
		{
		char a;
		short b;
		int c;
		long d;
		float e;
		double f;
		unsigned g:1;
		} s;

	union U1
		{
		char u1_a;
		short u1_b;
		int u1_c;
		unsigned u1_d;
		long u1_e;
		float u1_f;
		double u1_g;
#if (ANSI || V7)
		UCHAR u1_h;
		USHORT u1_i;
		ULONG  u1_j;
#endif
#if ANSI
		signed char u1_k;
		long double u1_l;
#endif
		} u, *pu = &u;

#if (ANSI || UNIQ_MEMB_NAMES)
	struct uniqnames
		{
		double a;
		};
#endif

	struct
		{
		unsigned x:1;
		} onefield;


																						/* 3.5.2.1 (cont.) */
	struct
		{
		unsigned fa_f:1;
		unsigned     :0;	/* break storage boundry */
		unsigned fa_g:1;
		} forcealign;
#if ANSI
	union ufields
		{
		unsigned x1 : 1;
		unsigned x3 : 2+1;
		} uf1;
	enum { ONE=1, TWO };
#endif /* ANSI */

	struct fields
		{
		unsigned x1:1;
		unsigned x2:2;
		unsigned x3:3;
		unsigned   :1;		/* just because it can be done */
		unsigned x4:4;
#if ANSI
		int      x5:'\2'; /* char-constant width */
		signed int x6:TWO; /* enum width */
#endif /* ANSI */
		} b, b2, *p = &b2;

#if !ANSI
	checkthat(__LINE__, sizeof(onefield) == sizeof(unsigned));
#endif

#if ANSI
	b.x6 = -1;
	dequals(__LINE__, (double)b.x6, -1.);
#endif
	b2.x4 = 0;
	b.x1 = p->x1 = 1;
	p->x2 = b.x1 + 2*p->x1;
	b.x3 = p->x2 + 4*b.x1;
	p->x4 += (b.x3 - b.x1) * p->x2;	/* unsigned wraparound (3.2.1.1) */
	iequals(__LINE__, b.x1, 1); 
	iequals(__LINE__, p->x2, 3); 
	iequals(__LINE__, b.x3, 7); 
	iequals(__LINE__, p->x4, 2); 




	/* any interactions? */																/* 3.5.2.1 (cont.) */
	b.x2 = p->x3 = b.x4 = 0;
	iequals(__LINE__, b.x1, 1); 
	iequals(__LINE__, p->x2, 3); 
	iequals(__LINE__, b.x3, 7); 
	iequals(__LINE__, p->x4, 2); 


	/* unsigned values convert modulo largest+1 */
	b.x1 = 2;
	b.x2 = 4;
	b.x3 = 8;
	b.x4 = 16;
	iequals(__LINE__, b.x1, 0); 
	iequals(__LINE__, b.x2, 0); 
	iequals(__LINE__, b.x3, 0); 
	iequals(__LINE__, b.x4, 0); 

	/* there are not very many things about fields that are not
	* implementation dependent. 
	* 
	*/
	checkthat( - __LINE__, sizeof(forcealign) >= 2 * sizeof(onefield));

	/* size of union is size of greatest element (+ padding?) */
	checkthat(__LINE__, sizeof(u) >= sizeof(double));
#if ANSI
	checkthat(__LINE__, sizeof(u) >= sizeof(long double));
#endif
	/* make sure all union members are accessible and have the right value */
#if SIGNED_CHAR
	u.u1_a = -1;
	iequals(__LINE__, u.u1_a, -1);
#else
	u.u1_a = 255;
	iequals(__LINE__, u.u1_a, 255);
#endif
	pu->u1_b = -1;
	iequals(__LINE__, u.u1_b, -1);
	u.u1_c = -2;
	iequals(__LINE__, u.u1_c, -2);
	u.u1_d = MAX_UINT;
	checkthat(__LINE__, u.u1_d == MAX_UINT);
	u.u1_e = -1;
	lequals(__LINE__, u.u1_e, -1L);
	u.u1_f = 1.0;
	dequals(__LINE__, u.u1_f, 1.0);
	u.u1_g = 2.2;
	dequals(__LINE__, u.u1_g, 2.2);

																						/* 3.5.2.1 (cont.) */






#if ANSI
	u.u1_h = MAX_UCHAR;
	iequals(__LINE__, u.u1_h, MAX_UCHAR);
	u.u1_i = MAX_USHORT;
	checkthat(__LINE__, u.u1_i == MAX_USHORT);
	u.u1_j = MAX_ULONG;
	checkthat(__LINE__, u.u1_j == MAX_ULONG);

	u.u1_k = -1;
	iequals(__LINE__, u.u1_k, -1);
	u.u1_l = 3.3L;
	ldequals(__LINE__, u.u1_l, 3.3L);

#endif

	/* addresses of members must be monotonically increasing */
	checkthat(__LINE__, (char *)&s.a < (char *)&s.b);
	checkthat(__LINE__, (char *)&s.b < (char *)&s.c);
	checkthat(__LINE__, (char *)&s.c < (char *)&s.d);
	checkthat(__LINE__, (char *)&s.d < (char *)&s.e);
	checkthat(__LINE__, (char *)&s.e < (char *)&s.f);

	/* also, the address of the first member is the address of the structure */
	checkthat(__LINE__, (char *)&s.a == (char *)&s);

	/* structs and unions are padded, to allow array alignment */
	{
	struct S1 as[2];
	union U1 au[2];

	checkthat(__LINE__, &as[1] == (struct S1 *)(sizeof(struct S1) + (char *)&as[0]));
	checkthat(__LINE__, &au[1] == (union U1 *)(sizeof(union U1) + (char *)&au[0]));
	}
	} /* end c3_5_2_1 */









/*
 * 3.5.2.2 - Enumeration specifiers
 */
static void c3_5_2_2()
	{
	int i;
#if (V7 || ANSI)
	/* number from 0 */
	enum a { a0, a1, a2, a3} aset;

	/* number explicitly, with the order picked up in the middle */
	enum b { b2=2, b1=1, b0=0, b1a, b2a, b2b=2} bset, *pbset;

	/* check aset */
	iequals(__LINE__, a0, 0);
	iequals(__LINE__, a1, 1);
	iequals(__LINE__, a2, 2);
	iequals(__LINE__, a3, 3);

	/* check bset */
	iequals(__LINE__, b2, 2);
	iequals(__LINE__, b1, 1);
	iequals(__LINE__, b0, 0);
	iequals(__LINE__, b1a, 1);
	iequals(__LINE__, b2a, 2);
	iequals(__LINE__, b2b, 2);

	/* before ANSI the use of enums was limited */
	bset = b2a;
	pbset = &bset;
	iequals(__LINE__, bset, b2a);
	checkthat(__LINE__, bset==b2a);
	checkthat(__LINE__, bset!=b1);
	iequals(__LINE__, *pbset, bset);
















																						/* 3.5.2.2 (cont.) */
	/* with ANSI, enums can be used anywhere an int can be used */
#if ANSI
	{
	int a[b2b] = { 0, 1 };

	iequals(__LINE__, a[b0], ivalue(0));
	switch (bset)
		{
		case b0:
			i = 999;
			break;
		case b2:
			i = 0;
			break;
		default :
			i = 999;
			break;
		case a1:
			/* not wise, but possible */
			i = 999;
			break;
		}
	iequals(__LINE__, i, 0);

	iequals(__LINE__, ++bset, 3);
	checkthat(__LINE__, sizeof(aset) <= sizeof(int));
	}
#endif /* ANSI */
#endif /* V& || ANSI */
	} /* end c3_5_2_2 */



















/* 
 * 3.5.2.3 - Tags.
 */

/* mutually-referencing structures */
struct mutrefa 							/* mutrefa is "tag" */
	{									/* followed by "{ struct-decl-list }" */
	struct mutrefb *refb;				/* "incomplete structure spec" for ptr-to-struct */
	int vala;
	};
struct mutrefb 
	{
	struct mutrefa *refa;				/* uses previously-decl "tag" */
	int valb;
	};
#if ANSI
struct mutrefb strb;					/* "tentative def" -- 3.7.2 */
#else
extern struct mutrefb strb;				/* ordinary "extern" ref */
#endif
struct mutrefa stra = { &strb, 11};
struct mutrefb strb = { &stra, 13};
/* "incomplete structure spec" must work for typedefs */
typedef struct s2 S2;	/* "incomplete structure spec" for typedef */
struct s2 				/* must be "completed" before size is needed */
	{
	struct s2 * ps1;	/* "incomplete spec" ok for ptr-to-struct */
	S2 *ps2;			/* so is the equivalent typedef */
	int val;
	} x1;				/* "tentative def" of x1 */
S2 x2;					/* "tentative def" of x2 */

#if ANSI
/* "incomplete structure spec" must work for functions returning structures */
struct s3 rets3();
struct s3 			/* "incomplete spec" must be completed before size is needed */
	{
	int s3_1, s3_2, s3_3, s3_4;
	} R3 = {1, 2, 3, 4};











																					/* 3.5.2.3 (cont.) */
/* watch out for nested scope in structure tag ... */
typedef struct nest NEST;
struct s3 rets3() 
	{				/* introduces new block scope */
	/* should be ignored by typedef NEST */
	struct nest { int a, b;};

	/* use "vacuous structure spec" to disambiguate */
	struct mutrefb;	/* introduces new tag */
	struct mutrefa 
		{
		struct mutrefb *refb;
		};
	struct mutrefb 
		{
		struct mutrefa *refa;
		};
	return R3;
	}
struct nest 		/* completing the spec from typedef NEST */
	{
	int nest_1;
	int nest_2;
	} Nest = { 1, 2};
#endif
























																						/* 3.5.2.3 (cont.) */
/* mutually-referencing unions */
union mutrefua 							/* mutrefua is "tag" */
	{									/* followed by "{ union-decl-list }" */
	union mutrefub *refub;				/* "incomplete union spec" for ptr-to-union */
	int vala;
	};
union mutrefub 
	{
	union mutrefua *refua;				/* uses previously-decl "tag" */
	int valb;
	};
#if ANSI
union mutrefub unb;					/* "tentative def" -- 3.7.2 */
union mutrefua una = { &unb };
union mutrefub unb = { &una };
#else
extern union mutrefub unb;				/* ordinary "extern" ref */
union mutrefua una;
union mutrefub unb;
#endif
/* "incomplete union spec" must work for typedefs */
typedef union u2 U2;	/* "incomplete union spec" for typedef */
union u2 				/* must be "completed" before size is needed */
	{
	union u2 * pu1;	/* "incomplete spec" ok for ptr-to-union */
	U2 *pu2;			/* so is the equivalent typedef */
	int val;
	} ux1;				/* "tentative def" of ux1 */
U2 ux2;					/* "tentative def" of ux2 */

#if ANSI
/* "incomplete union spec" must work for functions returning unions */
union u3 retu3();
union u3 			/* "incomplete spec" must be completed before size is needed */
	{
	int u3_1; char u3_2;
	} U3 = {1};












																						/* 3.5.2.3 (cont.) */
/* watch out for nested scope in union tag ... */
typedef union unest UNEST;
union u3 retu3() 
	{				/* introduces new block scope */
	/* should be ignored by typedef UNEST */
	union unest { int a, b;};

	/* use "vacuous union spec" to disambiguate */
	union mutrefub;	/* introduces new tag */
	union mutrefua 
		{
		union mutrefub *refub;
		};
	union mutrefub 
		{
		union mutrefua *refua;
		};
	return U3;
	}
union unest 		/* completing the spec from typedef UNEST */
	{
	int unest_1;
	float unest_2;
	} UNest = { 1 };
#endif /* ANSI */




																						/* 3.5.2.3 (cont.) */
static void c3_5_2_3() 
	{
	char *p = 0;		/* This ptr will be used to test ptr-to-struct cast */
	/* make declarations using struct/union tags */
	struct s1 
		{
		int a, b, c;	/* unique struct member name spaces were tested in 3.1.2.3; */
		};				/* here we use K&R-compatible name rules */
	struct s1 a, b, c;
	union u1 
		{
		int i;
		double d;
		};





																						/* 3.5.2.3 (cont.) */
	union u1 u1;

	auto struct astr 	/* test syntax of stor-cl with struct */
		{
		int a;
		} astr;

	static struct astr sstr = {123};

	/* make sure that they exist */
	a.a = 1;
	iequals(__LINE__, a.a, 1);	/* (auto) local struct holds proper value */
	u1.i = 2;
	iequals(__LINE__, u1.i, 2);	/* (auto) local union holds proper value */

	/* make sure forward references work (see x1 and x2 on prev page) */
	x1.val = 3;
	x2.val = 4;
	x2.ps1 = &x1;
	x2.ps2 = &x2;		/* points to itself !! */
	iequals(__LINE__, x2.ps1->val, 3);	/* ptr-to-struct in struct works */
	iequals(__LINE__, x2.ps2->val, 4);	/* ptr-to-struct in struct works */

	/* unions can accept and hold properly-typed values */
	ux1.val = 3;
	ux2.val = 4;
	iequals(__LINE__, ux1.val, ivalue(3));
	iequals(__LINE__, ux2.val, ivalue(4));
	ux2.pu1 = &ux1;
	aequals(__LINE__, (char *)ux2.pu1, (char *)avalue(&ux1));

	/* mutually-referencing structs */
	iequals(__LINE__, stra.vala, 11);
	iequals(__LINE__, strb.valb, 13);
	iequals(__LINE__, stra.refb->valb, 13);
	iequals(__LINE__, strb.refa->vala, 11);
#if ANSI
	/* mutually-referencing unions */
	aequals(__LINE__, (char *)una.refub, (char *)avalue(&unb));
	aequals(__LINE__, (char *)unb.refua, (char *)avalue(&una));
#endif
	/* astr and sstr are different */
	astr.a = 1;
	iequals(__LINE__, sstr.a, 123);





																						/* 3.5.2.3 (cont.) */
#if ANSI
	iequals(__LINE__, Nest.nest_2, 2);	/* test that block-local "nest" was not */
	iequals(__LINE__, rets3().s3_4, 4);	/* seen outside its block */

	/* A cast (3.3.4) uses a "type-name" (3.5.5), which can decl a tag */
	p = (char *)(struct incast { int a; int b; } * )p;	/* decls tag "incast" */
	checkthat(__LINE__, sizeof(struct incast) > 0);		/* test that struct incast decl'ed properly */

	{
	struct s1;	/* new scope */
	struct s1 { char c; } sc;

	sc.c = ivalue('a');
	iequals(__LINE__, sc.c, 'a');
	}

#endif /* ANSI */
	} /* end c3_5_2_3 */































/*
 * 3.5.3 - const and volatile
 */
static void c3_5_3()
	{
#if ANSI
	const char c1;
	const UCHAR c2;
	const signed char c3;
	const short c4;
	const USHORT c5;
	const int c6;
	const unsigned int c7;
	const long c8;
	const ULONG c9;
	const float c10;
	const double c11;
	const long double c12;
	volatile char v1;
	volatile UCHAR v2;
	volatile signed char v3;
	volatile short v4;
	volatile USHORT v5;
	volatile int v6;
	volatile unsigned int v7;
	volatile long v8;
	volatile ULONG v9;
	volatile float v10;
	volatile double v11;
	volatile long double v12;
	volatile int * const cv1;
	const int * volatile cv2;
	int *pi;
	int i;
	const int *pci = &i;	/* may be assigned */
	volatile int *pvi = &i;
	const struct sa { int i; } csa;
	struct sa sa2;		/* doesn't inherit const */

	pi = (int *)pci;	/* ok with cast */
	checkthat(__LINE__, pi == &i);
	pi = (int *)pvi;
	checkthat(__LINE__, pi == &i);
	sa2.i = 1;		/* ok, not const */
	iequals(__LINE__, sa2.i, 1);
	COMPATIBLE(volatile int, int volatile);
	COMPATIBLE(signed volatile int, int volatile);
	COMPATIBLE(short volatile, signed volatile short int);

#endif /* ANSI */
	} /* end c3_5_3 */


/*
 * 3.5.4 - Declarators
 */
typedef int INT;
int func1() { return 7;}
INT func2() { return 11;}
int (*fpfi1())(){return func1;}		/* func ret ptr to func ret int */
typedef int (*PFI)();				/* ptr to func ret ptr to int */
PFI fpfi2(){return func2;}			/* func ret ptr to func ret int */
static void c3_5_4()
	{
	/* 2d-arr of ptr to func ret ptr to func ret int */
	int (*(*apfpfi1[2][3])())();
	int i, j;
	int *p1 = &i;
	int (*p2);
	int ((*p3));	/* declarator in parens is identical to unadorned */
	int (func1)();	/* declarator in parens is identical to unadorned */

	/* try the same thing with typedefs */
	typedef PFI (*PFPFI)();		/* ptr to func ret ptr to func ret int */
	PFPFI apfpfi2[2][3];

	p2 = (int *)avalue(p1);
	p3 = p2;
	checkthat(__LINE__, p3 == p1);
	apfpfi1[1][2] = fpfi1;
	apfpfi2[1][2] = fpfi2;
	i = 1, j = 2;
	iequals(__LINE__,  (*(*apfpfi1[i][j])())(), 7);
	iequals(__LINE__,  (*(*apfpfi2[i][j])())(), 11);

#if ANSI
	{
	struct { int i1; const int i2; } x = { 1, 2 };

	x.i1 = ivalue(3);	/* not const */
	iequals(__LINE__, x.i1, 3);
	}
#endif /* ANSI */
	} /* end c3_5_4 */







/*
 * 3.5.4.1 - Pointer declarators
 */
static void c3_5_4_1()
	{
	char c = 'x';
	char *pc = &c;
	char **ppc = &pc;
	char ***pppc = &ppc;
	char ****ppppc = &pppc;
	char *****pppppc = &ppppc;
	char ******ppppppc = &pppppc;
	typedef char C;
	typedef C *PC;
	typedef PC *PPC;
	typedef PPC *PPPC;
	typedef PPPC *PPPPC;
	typedef PPPPC *PPPPPC;
	typedef PPPPPC *PPPPPPC;
	C d = 'y';
	PC pd = &d; 
	PPC ppd = &pd; 
	PPPC pppd = &ppd; 
	PPPPC ppppd = &pppd; 
	PPPPPC pppppd = &ppppd; 
	PPPPPPC ppppppd = &pppppd; 

#if ANSI
	const char *pconst = "constant string literal";
	char carray[] = "variable string literal"; char *const constp = carray;
	volatile int *vp = 0;
	const char * volatile * pvpconst = &pconst;

	++pconst;
	iequals(__LINE__, *pconst, 'o');
	++*constp;
	iequals(__LINE__, *constp, 'v'+1);
	vp;
	iequals(__LINE__, **pvpconst, 'o');
	{	 /* Now test that volatile int * and int signed volatile * are compatible... */
	volatile int        *v1={0}, **p1=&v1;
	int signed volatile *v2={0}, **p2=&v2;
	p1=p2;
	checkthat(__LINE__, 0==mem_cmp((void *)p1, (void *)p2, sizeof(*p1)));
	}
#endif
	iequals(__LINE__, ******ppppppc, 'x');
	iequals(__LINE__, ******ppppppd, 'y');
	} /* end c3_5_4_1 */

/*
 * 3.5.4.2 - Array declarators
 */
/* dimension can be omitted for first dimension on external */
extern int INTarray[][3][2][3]; /* links with INTarray on next page */

static void check_array(iarray)
	/* first dimension can be left off on formal parameter */
	int iarray[][3][2][3];
	{
	int i, j, k, l, count;

	/* as initialized below, there are 36 sequential items in the array */
	for (count=0, i=0; i < 2; ++i)
		for (j = 0; j < 3; ++j)
			for (k = 0; k < 2; ++k)
				for (l = 0; l < 3; ++l, ++count)
					iequals(__LINE__, iarray[i][j][k][l], count);
	}

static void c3_5_4_2()
	{
	double d[2], *dp[2];

	/* make sure that the first dimension gets handled correctly */
	check_array(INTarray);

	/* array of doubles vs. array of pointers to doubles */ 
	d[0] = 0.0;
	d[1] = 2.0;
	dp[0] = d;
	dp[1] = d+1;
	dequals(__LINE__, *dp[1], 2.0);

	/* just to be perverse */
	*dp[0] = (d[0] && d[1]) ^ (d[0] || d[1]);
	dequals(__LINE__, d[0], 1.0);
#if ANSI
	{
	void ckarr();

	ckarr();
	}
#endif /* ANSI */
	} /* end c3_5_4_2 */





																						/* 3.5.4.2 (cont.) */
/* array with size is compatible to one lacking size */
/* dimension can be omitted for first dimension if initialized */
int INTarray[][3][2][3] = 
	{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
	 21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};

#if ANSI
int arrfn1();
int arrfn1 PARMS((int [][3][2][3]));
#if NEW_STYLE_FN_DEF
int arrfn1(int a[2][3][2][3])
#else
int arrfn1(a)
	int a[2][3][2][3];
#endif /* NEW_STYLE_FN_DEF */
	{
	return (a[0][0][0][1]);
	}
void ckarr()
	{
	int (*parf1)() = arrfn1;
	int (*parf2) PARMS((int (*a)[3][2][3])) = &arrfn1;

	iequals(__LINE__, arrfn1(INTarray), 1);
	iequals(__LINE__, parf1(INTarray), 1);
	iequals(__LINE__, (*parf2)(INTarray), 1);
	}
#endif /* ANSI */





















/*
 * 3.5.4.3 - Function declarators 
 */

/* ANSI rules for compatible function types:
	First, convert parameters of type fn or array.
	Then, delete any qualifiers on top type of any parameter.
	Then, for all cases, return types must be compatible.
	Then, one of the following must be true, for compatible fn types:

	1. Both fn types are old-style (no parameter information).
	2. If both are prototypes, they agree in number of parms,
		  they agree in use of ellipsis, and parms are compatible.
	3. If one is prototype and other is old-style declaration (with
		  empty parentheses), the prototype must not have ellipsis
		  or non-default parm types.
	4. Otherwise, one is a prototype and the other is an old-style
		  function definition.  The number of parms must agree, and
		  each prototype parm must be compatible with the promoted(!)
		  definition parm.
 */

#if ANSI && HAS_PROTOTYPES
int niladic(void) { return 99; }
int fn_char(int);
int fn_char();	/* ok by rule 3 */
int fn_dbl(double);

#if ANSI8703	/* now old-style fn parms are taken as widened before type-matching */
int fn_char(c) char c; { return 98; } /* ok by rule 4 */
int fn_dbl(f) float f; { return 97; } /* ok by rule 4 */
#else
int fn_char(c) int c; { return 98; }
int fn_dbl(f) double f; { return 97; }
#endif /* ANSI8703 */
int proto(char, short const int, int, int volatile long, float, double, long double); 

static void c3_5_4_3()
	{
	int proto(char, const short, int, volatile long, float, double, register long double); /* ok by rule 2 */

	iequals(__LINE__, niladic(), 99);
	iequals(__LINE__, fn_char(1), 98);
	iequals(__LINE__, fn_dbl(0.), 97);
	iequals(__LINE__, proto(1L, 1.0, 1L, '\01', 1, 1, 1), 7);





																						/* 3.5.4.3 (cont.) */
#if ANSI8804
	{
	int arrfn1();
	signed int arrfn1();	/* ok by rule 1 */

	/* ambiguity of typedef vs identifier is always resolved to typedef */
	/* so, single typedef name in parentheses is taken as fn of one parm, */
	/* not as redundant parentheses on a new identifier */
	typedef int I;
	int fpfi(int (*)(I));	/* parm is ptr to function of int arg returning int */
	iequals(__LINE__, fpfi(fn_char), ivalue(98));
	}
#endif /* ANSI8804 */
	} /* end c3_5_4_3 (ANSI version) */

#if ANSI8804
int fpfi(int pfi(int)) { return pfi(0); }
#endif


int arrfn1();
signed int arrfn1();	/* ok by rule 1 */

#if NEW_STYLE_FN_DEF
int proto(char x1, const short x2, int x3, volatile long x4, float x5, double x6, long double x7)
#else
int proto(x1, x2, x3, x4, x5, x6, x7)
	char x1; const short x2; register int x3; volatile long x4; float x5; double x6; long double x7;
#endif	/* NEW_STYLE_FN_DEF */
	{
	return((int)(x1+x2+x3+x4+x5+x6+x7));
	}
#else	/* if !ANSI || !HAS_PROTOTYPES */
static void c3_5_4_3()
	{
	/* nothing to test, if no prototypes */
	}
#endif /* ANSI && HAS_PROTOTYPES */











/*
 * 3.5.5 - Type names
 */
static void c3_5_5()
	{
	/* these are tested all over, but a few extra ones here ... */

#if ANSI
	char *(*(*pfpapc1)(int, ...))[10];
	char *(*(*pfpapc2)(char *(*)()))[11] = 0;
	iequals(__LINE__, sizeof( char *(*(*)(int, ...))[10]), sizeof pfpapc1);
	pfpapc1 = (char *(*(*)(int, ...))[10])pfpapc2;
#else
	char *(*(*pfpapc1)())[10];
	char *(*(*pfpapc2)())[11] = 0;
	iequals(__LINE__, sizeof(char *(*(*)())[10]), sizeof pfpapc1);
	pfpapc1 = (char *(*(*)())[10])pfpapc2;
#endif
	checkthat( - __LINE__, pfpapc1 == 0);
	}






























/*
 * 3.5.6 - Type definitions 
 */
static void c3_5_6()
	{
	/* typedef is not a new type, but rather a synonym. */
	struct x 
		{
		unsigned a:1;
		unsigned b:2;
		};
	typedef struct x X;
	X x1;
	struct x x2;
	unsigned us1;
	typedef unsigned US;
	US us2;

	x1.a = 2;
	x2.a = 2;
	iequals(__LINE__, x1.a, 0);
	iequals(__LINE__, x2.a, 0);
	us1 = us2 = MAX_UINT;
	iequals(__LINE__, ++us1, 0);
	iequals(__LINE__, ++us2, 0);

	/* ambiguity of typedef vs identifier is always resolved to typedef */
	/* In this case, "type spec shall not be omitted" means that if a symbol */
	/* could be either a type-name on an un-named field, or an identifier naming a field, */
	/* then it must be the former. */
#if ANSI8804
	{
	typedef int I;
	struct { const I : 2; int j : 2; } x = {1}; /* "const I" is the type of un-named field */

	iequals(__LINE__, x.j, 1);
	}
#endif /* ANSI8804 */
		











																						/* 3.5.6 (cont.) */
#if ANSI
		{
		/* typedefs can be nested */
		typedef int X;
		X a = 0;
		iequals(__LINE__, a, 0);
			{
			/* and redefined in other nested contexts */
			X X = 3;
			iequals(__LINE__, X, 3);
			}
		/* also, they don't interfere with other name spaces */
			{
#if TAG_AND_MEMB_SPACES
			static struct X { int X; } x = {7};
			static struct Y { int Y; } y = {8};
#else
			static struct X { int X1; } x = {7};
			static struct Y { int Y1; } y = {8};
#endif
			static enum Z { e0, e1, e2 = (X)3 } e;
			struct X *px = (struct X *)&y;

			e = e2;
			iequals(__LINE__, a, 0);
			iequals(__LINE__, e, 3);
#if TAG_AND_MEMB_SPACES
			iequals(__LINE__, x.X, 7);
			iequals(__LINE__, (X) px->X, 8);
#else
			iequals(__LINE__, x.X1, 7);
			iequals(__LINE__, (X) px->X1, 8);
#endif
			}
		}
#endif	/* ANSI */
	} /* end c3_5_6 */












/*
 * 3.5.7 - Initialization
 */
static int sarray[5];
static struct { int a, b;} st;
static struct s1 {int a, b;} s1_init = {3, 4};

struct
	{
	char *strings[2];
	struct
		{
		int a;
		int nums[2][3];
		}s;
	}x3[] = 
/* note that each explicit initializer list can have an extra comma */
		{{{"abc", "def", }, {1, {{2,3,4,},{5,6,7,},},},},
		{{"ghi", "jkl"}, {9, {{8,7,6},{5,4,3}}}}},
#if ANSI	/* very strict about the "Ritchie" rules */
	x4[] = {"abc", "def", 1,2,3,4,5,6,7, {"ghi", "jkl", {9, {8,7,6,{5,4,3}}}}};
	int a1[2][2][2] = { 0, 1, 2, 3, {4, 5, 6, 7}};
#else		/* other compilers, e.g. PCC, vary, so use full braces */
	x4[] = {{{"abc", "def", }, {1, {{2,3,4,},{5,6,7,},},},}, {{"ghi", "jkl"}, {9, {{8,7,6},{5,4,3}}}}};
	int a1[2][2][2] = { 0, 1, 2, 3, 4, 5, 6, 7};
#endif
	int a2[2][2][2] = { 0, 1, 2, 3, 4, 5, 6, 7};

	/* special case of a string literal */
	char s1[] = "abc";
	char s2[] = {'a', 'b', 'c', 0};
#if ANSI
	char s3[3] = { "def" } ;	/* optional braces */
#else
	char s3[3] = "de";
#endif
#if ANSI8712 && WIDE_CHARS
	wchar_t s4[] = L"abc";		/* "wide-char" string */
#endif

	/* if there are not enough initializers, close off the aggregate with 0 */
	int a3[2][2][2] = 
		{{{99}}, {{98}}};
	struct
		{
		struct { int a, b;} sm1;
		struct { int a, b;} sm2;
		} s = 
			{{11},{13}};


																						/* 3.5.7 (cont.) */
#if ANSI
	/* unions can be intiialized to their first element */
	union
		{
		double d;
		int i;
		} u1 = {2.2};
#endif

static void c3_5_7()
	{
	int i = {2};
	int j, k, count;
	/* auto variables can be initialized to arbitrary expressions */
	int a = {1}, b = a*3, c = ivalue(5); /* braces optional on single scalar */

#if ANSI
	union
		{
		double d;
		int i;
		} u2 = {3.3};
	/* aggregates can be initialized */
	int aarray[4] = {0, 1, 2, 3};
	struct { int a[3], b; } w[] = { {1}, 2 };	/* "inconsistent-braces" example from 3.5.7 */

	for (i = 0; i < 4; ++i)
		iequals(__LINE__, aarray[i], i);
	iequals(__LINE__, w[0].a[0], 1);
	iequals(__LINE__, w[1].a[0], 2);
	iequals(__LINE__, w[0].a[1], 0);
	iequals(__LINE__, w[1].a[1], 0);
	iequals(__LINE__, w[0].b, 0);
#endif /* ANSI */
	
#if ANSI
	/* initialize one structure with another */
	{
	struct s1 x = s1_init;
	iequals(__LINE__, x.a, 3);
	iequals(__LINE__, x.b, 4);
	}
#endif





																						/* 3.5.7 (cont.) */
	iequals(__LINE__, a, 1);
	iequals(__LINE__, b, 3);
	iequals(__LINE__, c, 5);

	/* static items without initializers are initialized to 0 */
	for (i = 0; i < 5; ++i)
		iequals(__LINE__, sarray[i], 0);
	iequals(__LINE__, st.a, 0);
	iequals(__LINE__, st.b, 0);

	/* x3 used the explicit form for sub aggregates */
	checkthat(__LINE__, 0 == str_cmp(x3[1].strings[1], "jkl"));
	iequals(__LINE__, x3[0].s.nums[0][0], 2);
	iequals(__LINE__, x3[0].s.nums[1][2], 7);
	iequals(__LINE__, x3[1].s.nums[0][0], 8);
	iequals(__LINE__, x3[1].s.nums[1][2], 3);

	/* x4 left out some of them , but should look the same */
	checkthat(__LINE__, 0 == str_cmp(x4[1].strings[1], "jkl"));
	iequals(__LINE__, x4[0].s.nums[0][0], 2);
	iequals(__LINE__, x4[0].s.nums[1][2], 7);
	iequals(__LINE__, x4[1].s.nums[0][0], 8);
	iequals(__LINE__, x4[1].s.nums[1][2], 3);

	/* the two should be identical */
	checkthat(__LINE__, sizeof x3 == sizeof x4);

	/* elision of braces */
	for (i = 0, count = 0; i < 2; ++i)
		for (j = 0; j < 2; ++j)
			for (k = 0; k < 2; ++k, ++count)
				iequals(__LINE__, a1[i][j][k], count);
	for (i = 0, count = 0; i < 2; ++i)
		for (j = 0; j < 2; ++j)
			for (k = 0; k < 2; ++k, ++count)
				iequals(__LINE__, a2[i][j][k], count);

	/* special forms using string literal initialization */
	checkthat(__LINE__, !str_cmp(s1, s2));
	iequals(__LINE__, s3[0], 'd');
	iequals(__LINE__, s3[1], 'e');
	iequals(__LINE__, s3[2], ANSI ? 'f' : '\0');







																						/* 3.5.7 (cont.) */
	/* partial intialization fills with 0 */
	for (i = 0, count = 0; i < 2; ++i)
		for (j = 0; j < 2; ++j)
			for (k = 0; k < 2; ++k, ++count)
				if (count == 0)
					iequals(__LINE__, a3[i][j][k], 99);
				else if (count == 4)
					iequals(__LINE__, a3[i][j][k], 98);
				else
					iequals(__LINE__, a3[i][j][k], 0);
	iequals(__LINE__, s.sm1.a, 11);
	iequals(__LINE__, s.sm1.b, 0);
	iequals(__LINE__, s.sm2.a, 13);
	iequals(__LINE__, s.sm2.b, 0);

#if ANSI
	/* check union initialization */
	dequals(__LINE__, u1.d, 2.2);
	dequals(__LINE__, u2.d, 3.3);

	/* auto aggregates are zero-padded */
	{
		auto int a[10] = {1};
		int i;

		for (i = 1; i < 10; ++i)
			iequals(__LINE__, a[i], 0);
	}
	/* full-expr in initializer is sequence point */
	{
		int a = ivalue(1), b = a++, c = ++b;

		iequals(__LINE__, a, ivalue(2));
		iequals(__LINE__, b, ivalue(2));
		iequals(__LINE__, c, ivalue(2));
	}

#endif /* ANSI */











																						/* 3.5.7 (cont.) */
#if ANSI8804
	/* un-named members are ignored during initialization */
	{
		struct s1
			{
			unsigned m1:2;
			unsigned :2;
			unsigned m2:2;
			unsigned m3:2;
			} s1 = { 1, 2 };
		iequals(__LINE__, s1.m1, 1);
		iequals(__LINE__, s1.m2, 2);
		iequals(__LINE__, s1.m3, 0);
	}
#endif /* ANSI8804 */

	} /* end c3_5_7 */

#else /* if SKIP35 */
void c3_5() { pr_skip("c3_5: SKIPPED ENTIRELY\n"); }
#endif /* SKIP35 */

