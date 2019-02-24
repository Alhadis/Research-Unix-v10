/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 * PRECEDENCE - Check for operator precedence.
 * All of the operator combinations are checked for precedence.
 */

#include "defs.h"

/*
 * PREC1 -- try all operator pairs -- this file uses all binary operators
 * in the first position (see prec 2 for the other way around).
 */

static void p1_1();
static void p1_2();
static void p1_3();
static void p1_4();
static void p1_5();
static void p1_6();
static void p1_7();
static void p1_8();
static void p1_9();
static void p1_10();
static void p1_11();
static void p1_12();
static void p1_13();
static void p1_14();
static void p1_15();
static void p1_16();

void prec1a()
	{
	Filename = "prec1a.c";
	p1_1();	/* [] */
	p1_2();	/* () */
	p1_3();	/* . */
	p1_4();	/* -> */
	p1_5();	/* X++ */
	p1_6();	/* X-- */
	p1_7();	/* ++X */
	p1_8();	/* --X */
	p1_9();	/* & */
	p1_10();	/* * */
	p1_11();	/* + */
	p1_12();	/* - */
	p1_13();	/* ~ */
	p1_14();	/* ! */
	p1_15();	/* sizeof */
	p1_16();	/* cast */
	}

static char ca[10] = {0,1,2,3,4,5,6,7,8,9};
static char *pa = "abc";
static char *pc = ca+1;
static char S3_3[3];
static struct  X
	{
	int a[2];
	int b;
	char *p;
	struct X *px;
	}	q = {0,0,1}, 
		r = {0,0,2}, 
		s = {{3,4},5, "x", &q},
		sa[2] = {{{3,4},5, "x", &q}, {{6,7},8,"y",&r}},
		*ps = &s,
		*psa[2] = {&sa[0],&sa[1]};

static int f(){return 5;}
static int g(){return 6;}
static int fret(i){return i;}
static struct X *fps(){return &sa[0];}
static char *fp(){return "abc";}
static int (*arfun[2])() = {f, g};

/* [] */
static void p1_1()
	{
	int i = 1;
	int j = -1;
	iequals(__LINE__, (*arfun[i])(), 6);
	iequals(__LINE__, sa[i].b, 8);
	iequals(__LINE__, psa[i]->b, 8);
	iequals(__LINE__, ca[i++], 1);
	iequals(__LINE__, ca[i--], 2);
	iequals(__LINE__, ca[++i], 2);
	iequals(__LINE__, ca[--i], 1);
	/* [&X] is illegal */
	iequals(__LINE__, pa[*pc], 'b'); 
#if ANSI
	iequals(__LINE__, ca[+i], 1);
#endif
	iequals(__LINE__, ca[-j], 1);
	j = ~0; iequals(__LINE__, ca[~j], 0); j = -1;
	iequals(__LINE__, ca[!i], 0);
	iequals(__LINE__, ca[sizeof S3_3], 3);
	iequals(__LINE__, ca[(char)i], 1);
	iequals(__LINE__, ca[i]*2, 2);
	i = 8;
	iequals(__LINE__, ca[i]/2, 4);
	iequals(__LINE__, ca[i]%3, 2);
	iequals(__LINE__, ca[i]+i, 16);
	iequals(__LINE__, ca[i]-i, 0);
	iequals(__LINE__, ca[i]>>2, 2);
	iequals(__LINE__, ca[i]<<2, 32);
	iequals(__LINE__, ca[i]>j, 1);
	iequals(__LINE__, ca[i]<1, 0);
	iequals(__LINE__, ca[i]<=i, 1);
	iequals(__LINE__, ca[i]>=i, 1);
	iequals(__LINE__, ca[i]==i, 1);
	iequals(__LINE__, ca[i]!=i, 0);
	iequals(__LINE__, ca[i]&8, 8);
	iequals(__LINE__, ca[i]^i, 0);
	iequals(__LINE__, ca[i]|i, 8);
	j = 0;
	iequals(__LINE__, ca[i]&&j, 0);
	iequals(__LINE__, ca[i]||j, 1);
	iequals(__LINE__, ca[i]?i:0, 8);
	iequals(__LINE__, ca[9]=1, 1);
	iequals(__LINE__, ca[9]*=2, 2);
	iequals(__LINE__, ca[9]/=2, 1);
	iequals(__LINE__, ca[9]%=1, 0);
	iequals(__LINE__, ca[9]+=2, 2);
	iequals(__LINE__, ca[9]-=1, 1);
	iequals(__LINE__, ca[9]<<=1, 2);
	iequals(__LINE__, ca[9]>>=1, 1);
	iequals(__LINE__, ca[9]&=1, 1);
	iequals(__LINE__, ca[9]^=1, 0);
	iequals(__LINE__, ca[9]|=9, 9);
	iequals(__LINE__, (ca[9],8), 8);
	}

/* () */
static void p1_2()
	{
	int i = 1;
	int j = 0;
	iequals(__LINE__, fps()->b, 5);
	iequals(__LINE__, fret(*pc), 1); 
#if ANSI
	iequals(__LINE__, fret(+i), 1);
#endif
	iequals(__LINE__, fret(-5), -5);
	iequals(__LINE__, fret(~i), ~1);
	iequals(__LINE__, fret(!i), !i);
	iequals(__LINE__, fret((int)sizeof(i)), (int)sizeof(int));
	iequals(__LINE__, fret((char)i), 1);
	iequals(__LINE__, f()*2, 10);
	iequals(__LINE__, f()/5, 1);
	iequals(__LINE__, f()%2, 1);
	iequals(__LINE__, f()+i, 6);
	iequals(__LINE__, f()-3, 2);
	iequals(__LINE__, f()>>1, 2);
	iequals(__LINE__, f()<<1, 10);
	iequals(__LINE__, f()>i, 1);
	iequals(__LINE__, f()<i, 0);
	iequals(__LINE__, f()>=i, 1);
	iequals(__LINE__, f()<=i, 0);
	iequals(__LINE__, f()==i, 0);
	iequals(__LINE__, f()!=i, 1);
	iequals(__LINE__, f()&i, 1);
	iequals(__LINE__, f()^i, 4);
	iequals(__LINE__, f()|i, 5);
	iequals(__LINE__, f()&&j, 0);
	iequals(__LINE__, f()||j, 1);
	iequals(__LINE__, f()?i:0, 1);
	/* functions cannot be assigned to */
	/* = */
	/* *= */
	/* /= */
	/* %= */
	/* += */
	/* -= */
	/* <<= */
	/* >>= */
	/* &= */
	/* ^= */
	/* |= */
	iequals(__LINE__, (f(), 8), 8);
	}

/* . */
static void p1_3()
	{
	int i = 2;
	int j = 5;
	iequals(__LINE__, s.px->b, 1);
	iequals(__LINE__, s.b++, 5);
	iequals(__LINE__, s.b--, 6);
	/* .++b illegal */
	/* .--b illegal */
	/* .& illegal */
	/* .* illegal */
#if ANSI
	/* .+ illegal */
#endif
	/* .- illegal */
	/* .~ illegal */
	/* .! illegal */
	/* .sizeof illegal */
	/* . (cast) illegal */
	iequals(__LINE__, s.b*i, 10);
	iequals(__LINE__, s.b/5, 1);
	iequals(__LINE__, s.b%3, 2);
	iequals(__LINE__, s.b+2, 7);
	iequals(__LINE__, s.b-3, 2);
	iequals(__LINE__, s.b>>i, 1);
	iequals(__LINE__, s.b<<i, 20);
	iequals(__LINE__, s.b>i, 1);
	iequals(__LINE__, s.b<i, 0);
	iequals(__LINE__, s.b<=j, 1);
	iequals(__LINE__, s.b>=j, 1);
	iequals(__LINE__, s.b==j, 1);
	iequals(__LINE__, s.b!=j, 0);
	iequals(__LINE__, s.b&i, 0);
	iequals(__LINE__, s.b&1, 1);
	iequals(__LINE__, s.b^i, 7);
	j = 0;
	iequals(__LINE__, s.b&&j, 0);
	iequals(__LINE__, s.b||j, 1);
	iequals(__LINE__, s.b?i:0, 2);
	iequals(__LINE__, s.b=1, 1);
	iequals(__LINE__, s.b*=2, 2);
	iequals(__LINE__, s.b/=2, 1);
	iequals(__LINE__, s.b%=1, 0);
	iequals(__LINE__, s.b+=2, 2);
	iequals(__LINE__, s.b-=1, 1);
	iequals(__LINE__, s.b<<=1, 2);
	iequals(__LINE__, s.b>>=1, 1);
	iequals(__LINE__, s.b&=1, 1);
	iequals(__LINE__, s.b^=1, 0);
	iequals(__LINE__, s.b|=5, 5);
	iequals(__LINE__, (s.b,8), 8);
	}

/* -> */
static void p1_4()
	{
	int i = 1;
	int j = 5;
	iequals(__LINE__, ps->b++, 5);
	iequals(__LINE__, ps->b--, 6);
	/* ->++ (pre) illegal */
	/* ->-- (pre) illegal */
	/* ->&  illegal */
	/* ->*  illegal */
#if ANSI
	/* ->+  illegal */
#endif
	/* ->-  illegal */
	/* ->~  illegal */
	/* ->!  illegal */
	/* ->sizeof  illegal */
	/* ->(cast)  illegal */
	iequals(__LINE__, ps->b*2, 10);
	iequals(__LINE__, ps->b/5, 1);
	iequals(__LINE__, ps->b%3, 2);
	iequals(__LINE__, ps->b+2, 7);
	iequals(__LINE__, ps->b-3, 2);
	iequals(__LINE__, ps->b>>2, 1);
	iequals(__LINE__, ps->b%2, 1);
	iequals(__LINE__, ps->b>i, 1);
	iequals(__LINE__, ps->b<i, 0);
	iequals(__LINE__, ps->b<=j, 1);
	iequals(__LINE__, ps->b>=j, 1);
	iequals(__LINE__, ps->b==j, 1);
	iequals(__LINE__, ps->b!=j, 0);
	iequals(__LINE__, ps->b&7, 5);
	iequals(__LINE__, ps->b^7, 2);
	iequals(__LINE__, ps->b|2, 7);
	iequals(__LINE__, ps->b&&j, 1);
	iequals(__LINE__, ps->b||j, 1);
	iequals(__LINE__, ps->b?5:0, 5);
	iequals(__LINE__, ps->b=1, 1);
 	iequals(__LINE__, ps->b*=2, 2);
	iequals(__LINE__, ps->b/=2, 1);
	iequals(__LINE__, ps->b%=1, 0);
	iequals(__LINE__, ps->b+=2, 2);
	iequals(__LINE__, ps->b-=1, 1);
 	iequals(__LINE__, ps->b<<=1, 2);
 	iequals(__LINE__, ps->b>>=1, 1);
	iequals(__LINE__, ps->b&=1, 1);
	iequals(__LINE__, ps->b^=1, 0);
	iequals(__LINE__, ps->b|=5, 5);
	iequals(__LINE__, (ps->b,8), 8);
	}

/* X++ */
static void p1_5()
	{
	char *p = "abc";
	int i = 5;
	int j = 0;

	/* ++X++ is illegal */
	/* --X++ is illegal */
	/* &X++ is illegal */
	/* X++* is illegal */
#if ANSI
	/* X+++(unary)  is illegal */
#endif
	i = 12;
	/* X++- (unary) is illegal */
	/* X++~ (unary) is illegal */
	/* X++! (unary) is illegal */
	/* X++sizeof (unary) is illegal */
	/* X++(cast) (unary) is illegal */
	iequals(__LINE__, i++*2, 24);
	iequals(__LINE__, i++/13, 1);
	iequals(__LINE__, i++%10, 4);
	iequals(__LINE__, i+++2, 17);
	iequals(__LINE__, i++-6, 10);
	iequals(__LINE__, i++>>2, 4);
	iequals(__LINE__, i++<<1, 36);
	iequals(__LINE__, i++>2, 1);
	iequals(__LINE__, i++<2, 0);
	iequals(__LINE__, i++<=21, 1);
	iequals(__LINE__, i++>=10, 1);
	iequals(__LINE__, i++==23, 1);
	iequals(__LINE__, i++!=24, 0);
	i=5;
	iequals(__LINE__, i++&7, 5);
	iequals(__LINE__, i++^7, 1);
	iequals(__LINE__, i++|2, 7);
	iequals(__LINE__, i++&&j, 0);
	iequals(__LINE__, i++||j, 1);
	i = 1;
	iequals(__LINE__, i++?1:0, 1);
	/* X++= is illegal */
	/* X++*= is illegal */
	/* X++/= is illegal */
	/* X++%= is illegal */
	/* X+++= is illegal */
	/* X++-= is illegal */
	/* X++<<= is illegal */
	/* X++>>= is illegal */
	/* X++&= is illegal */
	/* X++^= is illegal */
	/* X++|= is illegal */
	iequals(__LINE__, (i++,8), 8);
	}

/* X-- */
static void p1_6()
	{
	char *p = "abc";
	int i = 8;
	int j = 1;
	++p;

	/* --X-- is illegal */
	/* &X-- is illegal */
	/* &X-- is illegal */
	/* X--* is illegal */
#if ANSI
	/* X--+(unary)  is illegal */
#endif
	/* X--- (unary) is illegal */
	/* X--~ (unary) is illegal */
	/* X--! (unary) is illegal */
	/* X--sizeof (unary) is illegal */
	/* X--(cast) (unary) is illegal */
	iequals(__LINE__, i--*2, 16);
	iequals(__LINE__, i--/7, 1);
	iequals(__LINE__, i--%5, 1);
	iequals(__LINE__, i--+3, 8);
	iequals(__LINE__, i---3, 1);
	iequals(__LINE__, i-->>1, 1);
	iequals(__LINE__, i--<<2, 8);
	iequals(__LINE__, i-->0, 1);
	iequals(__LINE__, i--<1, 1);
	iequals(__LINE__, i--<=-1, 1);
	iequals(__LINE__, i-->=-2, 1);
	iequals(__LINE__, i--==-3, 1);
	iequals(__LINE__, i--!=-4, 0);
	i=5;
	iequals(__LINE__, i--&7, 5);
	iequals(__LINE__, i--^5, 1);
	iequals(__LINE__, i--|7, 7);
	iequals(__LINE__, i--&&j, 1);
	iequals(__LINE__, i--||j, 1);
	iequals(__LINE__, i--?0:1, 1);
	/* X--= is illegal */
	/* X--*= is illegal */
	/* X--/= is illegal */
	/* X--%= is illegal */
	/* X--+= is illegal */
	/* X---= is illegal */
	/* X--<<= is illegal */
	/* X-->>= is illegal */
	/* X--&= is illegal */
	/* X--^= is illegal */
	/* X--|= is illegal */
	iequals(__LINE__, (i--,8), 8);
	}

/* ++X */
static void p1_7()
	{
	static char a[] = "amz"; char *p = a;
	int i = 1;
	int j = 0;

	/* ++--X is illegal */
	/* ++&X is illegal */
	iequals(__LINE__, ++*p, 'b'); 
#if ANSI
	/* ++ +X is illegal */
#endif
	/* ++ -X is illegal */
	/* ++ ~X is illegal */
	/* ++ !X is illegal */
	/* ++ sizeof X is illegal */
	/* ++ (cast) X is illegal */
	iequals(__LINE__, ++i*2, 4);
	iequals(__LINE__, ++i/3, 1);
	iequals(__LINE__, ++i%3, 1);
	iequals(__LINE__, ++i+2, 7);
	iequals(__LINE__, ++i-1, 5);
	iequals(__LINE__, ++i<<1, 14);
	iequals(__LINE__, ++i>>3, 1);
	iequals(__LINE__, ++i>j, 1);
	iequals(__LINE__, ++i<j, 0);
	iequals(__LINE__, ++i<=11, 1);
	iequals(__LINE__, ++i>=12, 1);
	iequals(__LINE__, ++i==13, 1);
	iequals(__LINE__, ++i!=14, 0);
	iequals(__LINE__, ++i&2, 2);
	iequals(__LINE__, ++i^1, 17);
	iequals(__LINE__, ++i|2, 19);
	iequals(__LINE__, ++i&&j, 0);
	iequals(__LINE__, ++i||j, 1);
	iequals(__LINE__, ++i?1:0, 1);
	/* ++X= is illegal */
	/* ++X*= is illegal */
	/* ++X/= is illegal */
	/* ++X%= is illegal */
	/* ++X+= is illegal */
	/* ++X-= is illegal */
	/* ++X<<= is illegal */
	/* ++X>>= is illegal */
	/* ++X&= is illegal */
	/* ++X^= is illegal */
	/* ++X|= is illegal */
	iequals(__LINE__, (++i, 8), 8);
	}

/* --X */
static void p1_8()
	{
	static char a[] = "amz"; char *p = a;
	int i = 20;
	int j = 0;

	/* --&X is illegal */
	iequals(__LINE__, --*p, 'a'-1); 
#if ANSI
	/* -- +X is illegal */
#endif
	/* -- -X is illegal */
	/* -- ~X is illegal */
	/* -- !X is illegal */
	/* -- sizeof X is illegal */
	/* -- (cast) X is illegal */
	iequals(__LINE__, --i*2, 38);
	iequals(__LINE__, --i/3, 6);
	iequals(__LINE__, --i%4, 1);
	iequals(__LINE__, --i+2, 18);
	iequals(__LINE__, --i-1, 14);
	iequals(__LINE__, --i<<1, 28);
	iequals(__LINE__, --i>>3, 1);
	iequals(__LINE__, --i>j, 1);
	iequals(__LINE__, --i<j, 0);
	iequals(__LINE__, --i<=10, 1);
	iequals(__LINE__, --i>=9, 1);
	iequals(__LINE__, --i==8, 1);
	iequals(__LINE__, --i!=7, 0);
	iequals(__LINE__, --i&7, 6);
	iequals(__LINE__, --i^2, 7);
	iequals(__LINE__, --i|1, 5);
	iequals(__LINE__, --i&&j, 0);
	iequals(__LINE__, --i||j, 1);
	iequals(__LINE__, --i?1:0, 1);
	/* --X= is illegal */
	/* --X*= is illegal */
	/* --X/= is illegal */
	/* --X%= is illegal */
	/* --X+= is illegal */
	/* --X-= is illegal */
	/* --X<<= is illegal */
	/* --X>>= is illegal */
	/* --X&= is illegal */
	/* --X^= is illegal */
	/* --X|= is illegal */
	iequals(__LINE__, (--i, 8), 8);
	}

/* &X */
static void p1_9()
	{
	char *p = "amz";
	char *q = 0;
	int i = 20;
	int j = 0;

	aequals(__LINE__, &*p, p); 
#if ANSI
	/* & +X is illegal */
#endif
	/* & -X is illegal */
	/* & ~X is illegal */
	/* & !X is illegal */
	/* & sizeof X is illegal */
	/* & (cast) X is illegal */
	/* & X * Y is illegal */
	/* & X / Y is illegal */
	/* & X % Y is illegal */
	aequals(__LINE__, &s+1, ps+1);
	aequals(__LINE__, &sa[1]-1, psa[1]-1);
	/* & X << Y is illegal */
	/* & X << Y is illegal */
	iequals(__LINE__, &s>ps, 0);
	iequals(__LINE__, &s<ps, 0);
	iequals(__LINE__, &s<=ps, 1);
	iequals(__LINE__, &s>=ps, 1);
	iequals(__LINE__, &s==ps, 1);
	iequals(__LINE__, &s!=ps, 0);
	/* & X & Y is illegal */
	/* & X ^ Y is illegal */
	/* & X | Y is illegal */
	/* & X && Y is illegal */
	/* & X || Y is illegal */
	/* &X?.... is illegal */
	/* &X= is illegal */
	/* &X*= is illegal */
	/* &X/= is illegal */
	/* &X%= is illegal */
	/* &X+= is illegal */
	/* &X-= is illegal */
	/* &X<<= is illegal */
	/* &X>>= is illegal */
	/* &X&= is illegal */
	/* &X^= is illegal */
	/* &X|= is illegal */
	iequals(__LINE__, (&i, 8), 8);
	}

/* *X */
static void p1_10()
	{
	int i = 2;
	int c = 2;
	int *p = &c;
	long *q = (long *)p;

#if ANSI
	/* * +X is illegal */
#endif
	/* * -X is illegal */
	/* * ~X is illegal */
	/* * !X is illegal */
	/* * sizeof X is illegal */
	iequals(__LINE__, *(int *)q, 2);
	iequals(__LINE__, *p*i, 4);
	iequals(__LINE__, *p/i, 1);
	iequals(__LINE__, *p%i, 0);
	iequals(__LINE__, *p+i, 4);
	iequals(__LINE__, *p-i, 0);
	iequals(__LINE__, *p>i, 0);
	iequals(__LINE__, *p<i, 0);
	iequals(__LINE__, *p<=i, 1);
	iequals(__LINE__, *p>=i, 1);
	iequals(__LINE__, *p==i, 1);
	iequals(__LINE__, *p!=i, 0);
	iequals(__LINE__, *p&i, (2)&2);
	iequals(__LINE__, *p^i, (2)^2);
	iequals(__LINE__, *p|i, (2)|2);
	iequals(__LINE__, *p&&i, 1);
	iequals(__LINE__, *p||i, 1);
	iequals(__LINE__, *p?i:0, 2);
	iequals(__LINE__, *p=1, 1);
 	iequals(__LINE__, *p*=2, 2);
	iequals(__LINE__, *p/=2, 1);
	iequals(__LINE__, *p%=1, 0);
	iequals(__LINE__, *p+=2, 2);
	iequals(__LINE__, *p-=1, 1);
 	iequals(__LINE__, *p<<=1, 2);
 	iequals(__LINE__, *p>>=1, 1);
	iequals(__LINE__, *p&=1, 1);
	iequals(__LINE__, *p^=1, 0);
	iequals(__LINE__, *p|=5, 5);
	iequals(__LINE__, (*p,i), 2);
	}

/* +X */
static void p1_11()
	{
#if ANSI
	int i = 2; double d = 1;
	iequals(__LINE__, +~i, ~2);
	iequals(__LINE__, +!0, 1);
	iequals(__LINE__, +sizeof(char), 1);
	iequals(__LINE__, +(int)d, 1);
	iequals(__LINE__, +2*i, 4);
	iequals(__LINE__, +2/i, 1);
	iequals(__LINE__, +2%i, 0);
	iequals(__LINE__, +2+i, 4);
	iequals(__LINE__, +2-i, 0);
	iequals(__LINE__, +2>i, 0);
	iequals(__LINE__, +2<i, 0);
	iequals(__LINE__, +2<=i, 1);
	iequals(__LINE__, +2>=i, 1);
	iequals(__LINE__, +2==i, 1); 
	iequals(__LINE__, +2!=i, 0);
	iequals(__LINE__, +2&i, (+2)&2);
	iequals(__LINE__, +2^i, (+2)^2);
	iequals(__LINE__, +2|i, (+2)|2);
	iequals(__LINE__, +2&&i, 1);
	iequals(__LINE__, +2||i, 1);
	iequals(__LINE__, +2?i:0, 2);
	/* +X= is illegal */
	/* +X*= is illegal */
	/* +X/= is illegal */
	/* +X%= is illegal */
	/* +X+= is illegal */
	/* +X-= is illegal */
	/* +X<<= is illegal */
	/* +X>>= is illegal */
	/* +X&= is illegal */
	/* +X^= is illegal */
	/* +X|= is illegal */
	iequals(__LINE__, (+2,i), 2);
#endif
	}
	
/* -X */
static void p1_12()
	{
	int i = 2;
	int j = 0;
	double d = 1.0;
	iequals(__LINE__, -~i, -(~2));
	iequals(__LINE__, -!j, -1);
	iequals(__LINE__, -(int)sizeof(char), -1);
	iequals(__LINE__, -(int)d, -1);
	iequals(__LINE__, -2*i, -4);
	iequals(__LINE__, -2/i, -1);
	iequals(__LINE__, -2%i, 0);
	iequals(__LINE__, -2+i, 0);
	iequals(__LINE__, -2-i, -4);
	iequals(__LINE__, -2>>i, (-2)>>i);
	iequals(__LINE__, -2<<i, (-2)<<i);
	iequals(__LINE__, -2>i, 0);
	iequals(__LINE__, -2<i, 1);
	iequals(__LINE__, -2<=i, 1);
	iequals(__LINE__, -2>=i, 0);
	iequals(__LINE__, -2==i, 0);
	iequals(__LINE__, -2!=i, 1);
	iequals(__LINE__, -2&i, (-2)&2);
	iequals(__LINE__, -2^i, (-2)^2);
	iequals(__LINE__, -2|i, (-2)|2);
	iequals(__LINE__, -2&&i, 1);
	iequals(__LINE__, -2||i, 1);
	iequals(__LINE__, -2?i:0, 2);
	/* -X= is illegal */
	/* -X*= is illegal */
	/* -X/= is illegal */
	/* -X%= is illegal */
	/* -X+= is illegal */
	/* -X-= is illegal */
	/* -X<<= is illegal */
	/* -X>>= is illegal */
	/* -X&= is illegal */
	/* -X^= is illegal */
	/* -X|= is illegal */
	iequals(__LINE__, (-2,i), 2);
	}

/* ~X */
static void p1_13()
	{
	int i = 2;
	int j = 5;
	double d = 1.0;
	iequals(__LINE__, ~!i, ~0);
	iequals(__LINE__, ~(int)sizeof(char), ~1);
	iequals(__LINE__, ~(int)d, ~1);
	iequals(__LINE__, ~(unsigned)j*i, (~(unsigned)5)*2);
	iequals(__LINE__, ~j/i, (~5)/2);
	iequals(__LINE__, ~j%i, (~5)%2);
	iequals(__LINE__, ~j+i, (~5)+2);
	iequals(__LINE__, ~j-i, (~5)-2);
	iequals(__LINE__, ~j>>i, (~5)>>2);
	iequals(__LINE__, ~j<<i, (~5)<<2);
	iequals(__LINE__, ~j>i, (~5)>2);
	iequals(__LINE__, ~j<i, (~5)<2);
	iequals(__LINE__, ~j<=i, (~5)<=2);
	iequals(__LINE__, ~j>=i, (~5)>=2);
	iequals(__LINE__, ~j==i, (~5)==2);
	iequals(__LINE__, ~j!=i, (~5)!=2);
	iequals(__LINE__, ~j&i, (~5)&2);
	iequals(__LINE__, ~j^i, (~5)^2);
	iequals(__LINE__, ~j|i, (~5)|2);
	iequals(__LINE__, ~j&&i, 1);
	iequals(__LINE__, ~j||i, 1);
	iequals(__LINE__, ~j?i:0, 2);
	/* ~X= is illegal */
	/* ~X*= is illegal */
	/* ~X/= is illegal */
	/* ~X%= is illegal */
	/* ~X+= is illegal */
	/* ~X-= is illegal */
	/* ~X<<= is illegal */
	/* ~X>>= is illegal */
	/* ~X&= is illegal */
	/* ~X^= is illegal */
	/* ~X|= is illegal */
	iequals(__LINE__, (~j,3), 3);
	}

/* !X */
static void p1_14()
	{
	int i = 2;
	int j = 0;
	double d = 1.0;
	iequals(__LINE__, !sizeof(char), 0);
	iequals(__LINE__, !(int)d, 0);
	iequals(__LINE__, !j*i, 1*2);
	iequals(__LINE__, !j/i, 1/2);
	iequals(__LINE__, !j%i, 1%2);
	iequals(__LINE__, !j+i, 1+2);
	iequals(__LINE__, !j-i, 1-2);
	iequals(__LINE__, !j>>i, 1>>2);
	iequals(__LINE__, !j<<i, 1<<2);
	iequals(__LINE__, !j>i, 1>2);
	iequals(__LINE__, !j<i, 1<2);
	iequals(__LINE__, !j<=i, 1<=2);
	iequals(__LINE__, !j>=i, 1>=2);
	iequals(__LINE__, !j==i, 1==2);
	iequals(__LINE__, !j!=i, 1!=2);
	iequals(__LINE__, !j&i, 1&2);
	iequals(__LINE__, !j^i, 1^2);
	iequals(__LINE__, !j|i, 1|2);
	iequals(__LINE__, !j&&i,1&&2);
	iequals(__LINE__, !j||i, 1||2);
	iequals(__LINE__, !j?i:0, 2);
	/* !X= is illegal */
	/* !X*= is illegal */
	/* !X/= is illegal */
	/* !X%= is illegal */
	/* !X+= is illegal */
	/* !X-= is illegal */
	/* !X<<= is illegal */
	/* !X>>= is illegal */
	/* !X&= is illegal */
	/* !X^= is illegal */
	/* !X|= is illegal */
	iequals(__LINE__, (!j,3), 3);
	}

/* sizeof X */
static void p1_15()
	{
	int i = 2;
	char j = 0;
	iequals(__LINE__, sizeof (char), 1);
	iequals(__LINE__, sizeof j*i, 1*2);
	iequals(__LINE__, sizeof j/i, 1/2);
	iequals(__LINE__, sizeof j%i, 1%2);
	iequals(__LINE__, sizeof j+i, 1+2);
	iequals(__LINE__, (unsigned)(sizeof j-i), ((unsigned)1)-2);
	iequals(__LINE__, sizeof j>>i, 1>>2);
	iequals(__LINE__, sizeof j<<i, 1<<2);
	iequals(__LINE__, sizeof j>i, 1>2);
	iequals(__LINE__, sizeof j<i, 1<2);
	iequals(__LINE__, sizeof j<=i, 1<=2);
	iequals(__LINE__, sizeof j>=i, 1>=2);
	iequals(__LINE__, sizeof j==i, 1==2);
	iequals(__LINE__, sizeof j!=i, 1!=2);
	iequals(__LINE__, sizeof j&i, 1&2);
	iequals(__LINE__, sizeof j^i, 1^2);
	iequals(__LINE__, sizeof j|i, 1|2);
	iequals(__LINE__, sizeof j&&i,1&&2);
	iequals(__LINE__, sizeof j||i, 1||2);
	iequals(__LINE__, sizeof j?i:0, 2);
	/* sizeof X= is illegal */
	/* sizeof X*= is illegal */
	/* sizeof X/= is illegal */
	/* sizeof X%= is illegal */
	/* sizeof X+= is illegal */
	/* sizeof X-= is illegal */
	/* sizeof X<<= is illegal */
	/* sizeof X>>= is illegal */
	/* sizeof X&= is illegal */
	/* sizeof X^= is illegal */
	/* sizeof X|= is illegal */
	iequals(__LINE__, (sizeof j,3), 3);
	}

/* (cast) */
static void p1_16()
	{
	int i = 2;
	double j = 1.1;
	iequals(__LINE__, (int) j*i, 1*2);
	iequals(__LINE__, (int) j/i, 1/2);
	iequals(__LINE__, (int) j%i, 1%2);
	iequals(__LINE__, (int) j+i, 1+2);
	iequals(__LINE__, (int) j-i, 1-2);
	iequals(__LINE__, (int) j>>i, 1>>2);
	iequals(__LINE__, (int) j<<i, 1<<2);
	iequals(__LINE__, (int) j>i, 1>2);
	iequals(__LINE__, (int) j<i, 1<2);
	iequals(__LINE__, (int) j<=i, 1<=2);
	iequals(__LINE__, (int) j>=i, 1>=2);
	iequals(__LINE__, (int) j==i, 1==2);
	iequals(__LINE__, (int) j!=i, 1!=2);
	iequals(__LINE__, (int) j&i, 1&2);
	iequals(__LINE__, (int) j^i, 1^2);
	iequals(__LINE__, (int) j|i, 1|2);
	iequals(__LINE__, (int) j&&i,1&&2);
	iequals(__LINE__, (int) j||i, 1||2);
	iequals(__LINE__, (int) j?i:0, 2);
	/* (int) X= is illegal */
	/* (int) X*= is illegal */
	/* (int) X/= is illegal */
	/* (int) X%= is illegal */
	/* (int) X+= is illegal */
	/* (int) X-= is illegal */
	/* (int) X<<= is illegal */
	/* (int) X>>= is illegal */
	/* (int) X&= is illegal */
	/* (int) X^= is illegal */
	/* (int) X|= is illegal */
	iequals(__LINE__, ((int) j,3), 3);
	}
