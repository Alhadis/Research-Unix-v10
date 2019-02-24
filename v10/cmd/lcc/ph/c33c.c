/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP33C
#include "defs.h"
void strucs(), q_s_a(), unions();
/*
 * Structures ..... all sorts of tests of structures and unions
 * Called at or about line 194 in c33a.c
 */
void structures()
	{
	Filename = "c33c.c";
#if (ANSI || V7)
	strucs();
	q_s_a();
	unions();
#endif
	}

#if (ANSI || V7)
/*
 * STRUCTS - this function tests the ways you can copy 
 * structures. It tests 2 sizes of structures: S1 has a single 
 * char element and SN has 20 int members plus a char. 
 */
typedef struct
	{
	char x;
	} S1;
typedef struct
	{
	int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t;
	char z;
	} SN;

extern S1 RetS1();
extern SN RetSN();

void strucs()
	{
	REGISTERS;
	S1 s1a, s1b, s1c;
	SN sna, snb, snc;
	int i, j;
	static S1 s1array[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };


	USEREGISTERS;

	/* simple assignment */
	s1a.x = ivalue(99);
	s1b = s1a;
	iequals(__LINE__, 99, s1b.x);
	sna.z = ivalue(98);
	snb = sna;
	iequals(__LINE__, 98, snb.z);
#if ANSI
	/* pass to a function and get result of function returning */
	s1a = RetS1(s1a);
	iequals(__LINE__, 100, s1a.x);
	iequals(__LINE__, 100, RetS1(s1a).x);
	sna = RetSN(sna);
	iequals(__LINE__, 101, sna.z);
	iequals(__LINE__, 101, RetSN(sna).z);
#endif
#if STRUCT_COND_OK
	/* copies involving question operators */
	i = j = ivalue(0);
	s1a.x = ivalue(102);
	s1c.x = ivalue(0);
	s1b = i ? s1c : (j ? s1c : s1a);
	iequals(__LINE__, 102, s1b.x);
	sna.z = ivalue(103);
	snc.z = ivalue(0);
	snb = i ? snc : (j ? snc : sna);
	iequals(__LINE__, 103, snb.z);
#endif

	/* make sure that structures are passed by value, not by reference */
	s1b.x = ivalue(104);
	s1a = RetS1(s1b);	/* sets s1a.x to 100 */
	iequals(__LINE__, 104, s1b.x);
	snb.z = ivalue(104);
	sna = RetSN(snb);	/* sets sna.z to 101 */
	iequals(__LINE__, 104, snb.z);
		{
		/* test lvalue use of p->m */
		S1 *ps1;
		SN *psN = &sna;

		ps1 = &s1b;
		++ps1->x;
		iequals(__LINE__, 105, ps1->x);
		psN->z++;
		iequals(__LINE__, 102, psN->z);
		ps1 = s1array;
		iequals(__LINE__, 1, (++ps1)->x);
		}
	}
	
S1 RetS1(s1)
	S1 s1;
	{
	S1 s1a;
	s1a.x = 100;
	return(s1a);
	}
SN RetSN(sn)
	SN sn;
	{
	SN sna;
	sna.z = 101;
	return(sna);
	}

/*
 * These tests go into the complexities of interaction between
 * question, comma, and structure copies.
 */
static true = 1;
static false = 0;
static int Total = 0;

struct mb {
	int	mb_i[10];
	};
struct mb mbr = {0}, mb1 = {1,2}, mb2 = {3,4}, mb3 = {5,6};
static int x()
	{
	Total++;
	}

void q_s_a()
	{
	int i = 1;
	do_nothing(&i);

#if STRUCT_COND_OK
    mbr = true ? mb1 : mb2;
	iequals(__LINE__, mbr.mb_i[i], 2);
    mbr = false ? mb1 : mb2;
	iequals(__LINE__, mbr.mb_i[i], 4);
    mbr = true ? (x(),mb1) : mb2;
	iequals(__LINE__, mbr.mb_i[i], 2);
    mbr = false ? mb1 : (x(), mb2);
	iequals(__LINE__, mbr.mb_i[i], 4);
    mbr = true ? (x(),mb1) : (x(),mb2);
	iequals(__LINE__, mbr.mb_i[i], 2);
    mbr = false ? (x(),mb1) : (x(),mb2);
	iequals(__LINE__, mbr.mb_i[i], 4);

	mbr = true ? (mb1=mb2) : mb3;
	iequals(__LINE__, mbr.mb_i[i], 4);
	iequals(__LINE__, mb1.mb_i[i], 4);
	mbr = false ? mb1 : (mb1=mb3);
	iequals(__LINE__, mbr.mb_i[i], 6);
	iequals(__LINE__, mb1.mb_i[i], 6);
	mbr = true ? (mb1=mb2) : (mb1=mb3);
	iequals(__LINE__, mbr.mb_i[i], 4);
	iequals(__LINE__, mb1.mb_i[i], 4);
	mbr = false ? (mb1=mb2) : (mb1=mb3);
	iequals(__LINE__, mbr.mb_i[i], 6);
	iequals(__LINE__, mb1.mb_i[i], 6);

	mbr = true ? (x(),(mb1=mb2)) : mb3;
	iequals(__LINE__, mbr.mb_i[i], 4);
	iequals(__LINE__, mb1.mb_i[i], 4);
	mbr = false ? mb1 : (x(),(mb1=mb3));
	iequals(__LINE__, mbr.mb_i[i], 6);
	iequals(__LINE__, mb1.mb_i[i], 6);
	mbr = true ? (x(),(mb1=mb2)) : (x(),(mb1=mb3));
	iequals(__LINE__, mbr.mb_i[i], 4);
	iequals(__LINE__, mb1.mb_i[i], 4);
	mbr = false ? (x(),(mb1=mb2)) : (x(),(mb1=mb3));
	iequals(__LINE__, mbr.mb_i[i], 6);
	iequals(__LINE__, mb1.mb_i[i], 6);

	/* make sure that side effects took place the proper number of times */
	iequals(__LINE__, Total, 8);
#endif
	}


/*
 * UNIONS - try out all of the aggregate things you can do to unions.
 */
union u1 {
	UCHAR u1_c ;
	int u1_i;
	} u1a, u1b, u1c;

union u2 {
	UCHAR u2_c;
	int u2_i;
	long u2_l;
	struct
		{
		int u2s_a, u2s_b, u2s_c, u2s_d, u2s_e, u2s_f, u2s_g;
		} u2_s;
	double u2_d;
	} u2a, u2b, u2c;

union u1 fu1();
union u2 fu2();
void vfu1();
void vfu2();

void unions()
	{
	int i = 0;

	u1a.u1_c = 255;
	u1b = u1a;
	iequals(__LINE__, u1b.u1_c, 255);
	u2a.u2_s.u2s_b = -7;
	u2b = u2a;
	iequals(__LINE__, u2b.u2_s.u2s_b, -7);

	/* check passing by value */
	u1a.u1_i = 0;
	vfu1(u1a);
	iequals(__LINE__, u1a.u1_i, 0);
	u2a.u2_d = 0.0;
	vfu2(u2a);
	dequals(__LINE__, u2a.u2_d, 0.0);
#if ANSI
	/* check voided return of union */
	(void)fu1(0);
	(void)fu2(0);
#endif
	/* check union return and assignment */
	u1a = fu1(9);
	iequals(__LINE__, u1a.u1_i, 10); 
	u2a = fu2(11);
	iequals(__LINE__, u2a.u2_s.u2s_g, 12); 
#if ANSI
	/* check direct access of returned unions */
	iequals(__LINE__, fu1(13).u1_i, 14); 
	iequals(__LINE__, fu2(15).u2_s.u2s_g, 16);
#endif
	/* unions involving question operators */
#if STRUCT_COND_OK
	u1a.u1_c = 17;
	u1b.u1_c = 18;
	u1c = i ? u1a : u1b;
	iequals(__LINE__, u1c.u1_c, 18);
	u2a.u2_s.u2s_g = 19;
	u2b.u2_s.u2s_g = 20;
	u2c = i ? u2a : u2b;
	iequals(__LINE__, u2c.u2_s.u2s_g, 20);
#endif
	}

union u1 fu1(i)
	int i;
	{
	static union u1 u;
	u.u1_i = i+1;
	return u;
	}

union u2 fu2(i)
	int i;
	{
	static union u2 u;
	u.u2_s.u2s_g = i+1;
	return u;
	}

/* set i field of passed in union */
void vfu1(u)
	union u1 u;
	{
	u.u1_i = 99;
	}

/* set d field of passed in union */
void vfu2(u)
	union u2 u;
	{
	u.u2_d = 99.99;
	}
#endif

#else /* if SKIP33C */

void structures() { pr_skip("c3_3c (structures): SKIPPED ENTIRELY\n"); }
#endif /* SKIP33C */

