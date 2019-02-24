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
  * PREC2A -- try all operator pairs -- this file uses all binary operators
  * in the second position (see prec1 for the other way around).
  */
 static void p2_1();
 static void p2_2();
 static void p2_3();
 static void p2_4();
 static void p2_5();
 static void p2_6();
 static void p2_7();
 static void p2_8();
 static void p2_9();
 static void p2_10();
 static void p2_11();
 static void p2_12();
 static void p2_13();
 static void p2_14();
 static void p2_15();
 static void p2_16();

 void prec2a()
	{
	Filename = "prec2a.c";
	p2_1();	/* [] */
	p2_2();	/* () */
	p2_3();	/* . */
	p2_4();	/* -> */
	p2_5();	/* X++ */
	p2_6();	/* X-- */
	p2_7();	/* ++X */
	p2_8();	/* --X */
	p2_9();	/* & */
	p2_10();	/* * */
	p2_11();	/* + */
	p2_12();	/* - */
	p2_13();	/* ~ */
	p2_14();	/* ! */
	p2_15();	/* sizeof */
	p2_16();	/* cast */
	}

static int i = 1;
static char ca[10] = {0,1,2,3,4,5,6,7,8,9};
static char *pa[10] = {ca, ca+1};
static struct  X
	{
	int a[2];
	int b;
	char *p;
	struct X *px;
	}	r = {0,0,2}, 
		s = {{3,4},5, "x", &r},
		*ps = &s,
		as[2] =  {{{6,7},8,0,0},{{9,10},11,0,0}},
		*pas = &as[0];
static struct
	{
	struct X s;
	}	xx = {{{6,7},8, "a", 0}}, 
		*pxx = &xx;

static int f(){return 5;}
static char *g(){return "abc";}
static struct X *fps(){return ps;}
static char *fp(){return "abc";}

static void p2_1()
	{
	iequals(__LINE__, fp()[i], 'b');
	iequals(__LINE__, s.a[i], 4);
	iequals(__LINE__, ps->a[i], 4);
	iequals(__LINE__, ca[i++], 1);
	iequals(__LINE__, ca[i--], 2);
	iequals(__LINE__, ++ca[i], 2);
	iequals(__LINE__, --ca[i], 1);
	aequals(__LINE__, &ca[i], ca+i); 
	iequals(__LINE__, *pa[i], 1); 
#if ANSI
	iequals(__LINE__, +ca[i], 1);
#endif
	iequals(__LINE__, -ca[i], -1);
	iequals(__LINE__, ~ca[i], ~1);
	iequals(__LINE__, !ca[i], 0);
	iequals(__LINE__, sizeof ca[i], 1);
	dequals(__LINE__, (double)ca[i], 1.0);
	iequals(__LINE__, 2*ca[i], 2);
	iequals(__LINE__, 2/ca[i], 2);
	iequals(__LINE__, 2%ca[i], 0);
	iequals(__LINE__, i+ca[i], 2);
	iequals(__LINE__, i-ca[i], 0);
	iequals(__LINE__, i>>ca[i], 0);
	iequals(__LINE__, i<<ca[i], 2);
	iequals(__LINE__, i>ca[i], 0);
	iequals(__LINE__, i<ca[i], 0);
	iequals(__LINE__, i<=ca[i], 1);
	iequals(__LINE__, i>=ca[i], 1);
	iequals(__LINE__, i==ca[i], 1);
	iequals(__LINE__, i!=ca[i], 0);
	iequals(__LINE__, i&ca[i], 1);
	iequals(__LINE__, i^ca[i], 0);
	iequals(__LINE__, i|ca[i], 1);
	iequals(__LINE__, i&&ca[i], 1);
	iequals(__LINE__, i||ca[i], 1);
	iequals(__LINE__, i?ca[i]:0, 1);
	iequals(__LINE__, i=ca[9], 9);
	iequals(__LINE__, i *= ca[2], 18);
	iequals(__LINE__, i /= ca[9], 2);
	iequals(__LINE__, i %= ca[3], 2);
	iequals(__LINE__, i += ca[4], 6);
	iequals(__LINE__, i -= ca[2], 4);
	iequals(__LINE__, i <<= ca[2], 16);
	iequals(__LINE__, i >>= ca[2], 4);
	iequals(__LINE__, i&ca[4], 4);
	iequals(__LINE__, i ^= ca[9], 13);
	iequals(__LINE__, i |= ca[2], 15);
	iequals(__LINE__, (8,ca[9]), 9); 
	}

/* () */
static void p2_2()
	{
	int i = 7;
	/* X.Y() is illegal */
	/* X->Y() is illegal */
	/* X++() is illegal */
	/* X--() is illegal */
	/* ++X() is illegal */
	/* --X() is illegal */
	/* &X() is illegal */
	iequals(__LINE__, *fp(), 'a'); 
#if ANSI
	iequals(__LINE__, +f(), 5);
#endif
	iequals(__LINE__, -f(), -5);
	iequals(__LINE__, ~f(), ~5);
	iequals(__LINE__, !f(), 0);
	iequals(__LINE__, sizeof g(), sizeof (char *));
	dequals(__LINE__, (double)f(), 5.0);
	iequals(__LINE__, i*f(), 35);
	iequals(__LINE__, i/f(), 1);
	iequals(__LINE__, i%f(), 2);
	iequals(__LINE__, i+f(), 12);
	iequals(__LINE__, i-f(), 2);
	iequals(__LINE__, i>>f(), 0);
	iequals(__LINE__, i<<f(), 224);
	iequals(__LINE__, i>f(), 1);
	iequals(__LINE__, i<f(), 0);
	iequals(__LINE__, i<=f(), 0);
	iequals(__LINE__, i>=f(), 1);
	iequals(__LINE__, i==f(), 0);
	iequals(__LINE__, i!=f(), 1);
	iequals(__LINE__, i&f(), 5);
	iequals(__LINE__, i^f(), 2);
	iequals(__LINE__, i|f(), 7);
	iequals(__LINE__, i&&f(), 1);
	iequals(__LINE__, i||f(), 1);
	iequals(__LINE__, i?f():0, 5);
	iequals(__LINE__, i=f(), 5);
	iequals(__LINE__, i *= f(), 25);
	iequals(__LINE__, i /= f(), 5);
	iequals(__LINE__, i %= f(), 0);
	i = 1;
	iequals(__LINE__, i += f(), 6);
	iequals(__LINE__, i -= f(), 1);
	iequals(__LINE__, i <<= f(), 32);
	iequals(__LINE__, i >>= f(), 1);
	iequals(__LINE__, i &= f(), 1);
	iequals(__LINE__, i ^= f(), 4);
	iequals(__LINE__, i |= f(), 5);
	iequals(__LINE__, (8,f()), 5); 
	}

/* . */
static void p2_3()
	{
	int i = 1;
	int j = 1;
	iequals(__LINE__, pxx->s.b, 8);
	/* .++ is illegal */
	/* .-- is illegal */
	iequals(__LINE__, ++s.b, 6);
	iequals(__LINE__, --s.b, 5);
	aequals(__LINE__, &s.b, &(s.b)); 
	iequals(__LINE__, *s.p, 'x'); 
#if ANSI
	iequals(__LINE__, +s.b, 5);
#endif
	iequals(__LINE__, -s.b, -5);
	iequals(__LINE__, ~s.b, ~5);
	iequals(__LINE__, !s.b, 0);
	iequals(__LINE__, sizeof s.b, sizeof (int));
	dequals(__LINE__, (double)s.b, 5.0);
	iequals(__LINE__, 2*s.b, 10);
	iequals(__LINE__, 10/s.b, 2);
	iequals(__LINE__, 11%s.b, 1);
	iequals(__LINE__, 2+s.b, 7);
	iequals(__LINE__, 7-s.b, 2);
	iequals(__LINE__, 32>>s.b, 1);
	iequals(__LINE__, 1<<s.b, 32);
	iequals(__LINE__, 2>s.b, 0);
	iequals(__LINE__, 2<s.b, 1);
	iequals(__LINE__, 2<=s.b, 1);
	iequals(__LINE__, 2>=s.b, 0);
	iequals(__LINE__, 2==s.b, 0);
	iequals(__LINE__, 2!=s.b, 1);
	iequals(__LINE__, 7&s.b, 5);
	iequals(__LINE__, 7^s.b, 2);
	iequals(__LINE__, 2|s.b, 7);
	iequals(__LINE__, i&&s.b, 1);
	iequals(__LINE__, i||s.b, 1);
	iequals(__LINE__, i?s.b:0, 5);
	iequals(__LINE__, i=s.b, 5);
	iequals(__LINE__, i *= s.b, 25);
	iequals(__LINE__, i /= s.b, 5);
	iequals(__LINE__, i %= s.b, 0);
	iequals(__LINE__, i += s.b, 5);
	iequals(__LINE__, i -= s.b, 0);
	iequals(__LINE__, j <<= s.b, 32);
	iequals(__LINE__, j >>= s.b, 1);
	iequals(__LINE__, j &= s.b, 1);
	j = 7;
	iequals(__LINE__, j ^= s.b, 2);
	iequals(__LINE__, j |= s.b, 7);
	iequals(__LINE__, (8,s.b), 5); 
	}

/* -> */
static void p2_4()
	{
	int i = 1;
	int j = 1;
	iequals(__LINE__, pas++->b, 8);
	iequals(__LINE__, pas--->b, 11);
	iequals(__LINE__, ++ps->b, 6);
	iequals(__LINE__, --ps->b, 5);
	aequals(__LINE__, &ps->b, &(s.b)); 
	iequals(__LINE__, *ps->p, 'x'); 
#if ANSI
	iequals(__LINE__, +ps->b, 5);
#endif
	iequals(__LINE__, -ps->b, -5);
	iequals(__LINE__, ~ps->b, ~5);
	iequals(__LINE__, !ps->b, 0);
	iequals(__LINE__, sizeof ps->b, sizeof(s.b));
	dequals(__LINE__, (double)ps->b, 5.0);
	iequals(__LINE__, 2*ps->b, 10);
	iequals(__LINE__, 10/ps->b, 2);
	iequals(__LINE__, 11%ps->b, 1);
	iequals(__LINE__, 2+ps->b, 7);
	iequals(__LINE__, 7-ps->b, 2);
	iequals(__LINE__, 32>>ps->b, 1);
	iequals(__LINE__, 1<<ps->b, 32);
	iequals(__LINE__, 2>ps->b, 0);
	iequals(__LINE__, 2<ps->b, 1);
	iequals(__LINE__, 2<=ps->b, 1);
	iequals(__LINE__, 2>=ps->b, 0);
	iequals(__LINE__, 2==ps->b, 0);
	iequals(__LINE__, 2!=ps->b, 1);
	iequals(__LINE__, 7&ps->b, 5);
	iequals(__LINE__, 7^ps->b, 2);
	iequals(__LINE__, 2|ps->b, 7);
	iequals(__LINE__, i&&ps->b, 1);
	iequals(__LINE__, i||ps->b, 1);
	iequals(__LINE__, i?ps->b:0, 5);
	iequals(__LINE__, i=ps->b, 5);
	iequals(__LINE__, i *= ps->b, 25);
	iequals(__LINE__, i /= ps->b, 5);
	iequals(__LINE__, i %= ps->b, 0);
	iequals(__LINE__, i += ps->b, 5);
	iequals(__LINE__, i -= ps->b, 0);
	iequals(__LINE__, j <<= ps->b, 32);
	iequals(__LINE__, j >>= ps->b, 1);
	iequals(__LINE__, j &= ps->b, 1);
	j = 7;
	iequals(__LINE__, j ^= ps->b, 2);
	iequals(__LINE__, j |= ps->b, 7);
	iequals(__LINE__, (8,ps->b), 5); 
	}

/* X++ */
static void p2_5()
	{
	char *p = "abc";
	int i = 5;
	int j = 1;

	/* ++X++ is illegal */
	/* --X++ is illegal */
	/* ++X-- is illegal */
	iequals(__LINE__, i, 5);
	/* &X++ is illegal */
	iequals(__LINE__, *p++, 'a'); 
	iequals(__LINE__, *p, 'b'); 
#if ANSI
	iequals(__LINE__, +i++, 5);
	iequals(__LINE__, +i, 6);
#endif
	i = 8;
	iequals(__LINE__, -i++, -8);
	iequals(__LINE__, ~i++, ~9);
	iequals(__LINE__, !i++, 0);
	/* sizeof i++ is undefined */
	dequals(__LINE__, (double)i++, 11.0);
	iequals(__LINE__, 2*i++, 24);
	iequals(__LINE__, 26/i++, 2);
	iequals(__LINE__, 30%i++, 2);
	iequals(__LINE__, 2+i++, 17);
	iequals(__LINE__, 7-i++, -9);
	i = 1;
	iequals(__LINE__, 8>>i++, 4);
	iequals(__LINE__, 1<<i++, 4);
	iequals(__LINE__, 2>i++, 0);
	iequals(__LINE__, 2<i++, 1);
	iequals(__LINE__, 2<=i, 1);
	iequals(__LINE__, 2>=i++, 0);
	iequals(__LINE__, 2==i++, 0);
	iequals(__LINE__, 2!=i++, 1);
	i=5;
	iequals(__LINE__, 7&i++, 5);
	iequals(__LINE__, 7^i++, 1);
	iequals(__LINE__, 2|i++, 7);
	iequals(__LINE__, j&&i++, 1);
	iequals(__LINE__, j||i++, 1);
	i = 1;
	iequals(__LINE__, j?i++:0, 1);
	iequals(__LINE__, j=i++, 2);
	iequals(__LINE__, j *= i++, 6);
	iequals(__LINE__, j /= i++, 1);
	iequals(__LINE__, j %= i++, 1);
	iequals(__LINE__, j += i++, 7);
	iequals(__LINE__, j -= i++, 0);
	j = 1;
	iequals(__LINE__, j <<= i++, 1<<8);
	iequals(__LINE__, j >>= i++, 0);
	j = 7, i=5;
	iequals(__LINE__, j &= i++, 5);
	iequals(__LINE__, j ^= i++, 3);
	iequals(__LINE__, j |= i++, 7);
	iequals(__LINE__, (8,i++), 8); 
	}

/* X-- */
static void p2_6()
	{
	char *p = "abc";
	int i = 17;
	int j = 1;
	++p;
	/* --X-- is illegal */
	/* ++X-- is illegal */
	iequals(__LINE__, i, 17);
	/* &X++ is illegal */
	iequals(__LINE__, *p--, 'b'); 
	iequals(__LINE__, *p, 'a'); 
#if ANSI
	iequals(__LINE__, +i--, 17);
	iequals(__LINE__, i, 16);
#endif
	i = 8;
	iequals(__LINE__, -i--, -8);
	iequals(__LINE__, ~i--, ~7);
	iequals(__LINE__, !i--, 0);
	/* sizeof i-- is undefined */
	dequals(__LINE__, (double)i--, 5.0);
	iequals(__LINE__, 2*i--, 8);
	iequals(__LINE__, 27/i--, 9);
	iequals(__LINE__, 32%i--, 0);
	iequals(__LINE__, 2+i--, 3);
	iequals(__LINE__, 7-i--, 7);
	i = 2;
	iequals(__LINE__, 8>>i--, 2);
	iequals(__LINE__, 2<<i--, 4);
	iequals(__LINE__, 2>i--, 1);
	iequals(__LINE__, 2<i--, 0);
	iequals(__LINE__, -2<=i--, 1);
	iequals(__LINE__, 2>=i--, 1);
	iequals(__LINE__, -4==i--, 1);
	iequals(__LINE__, -5!=i--, 0);
	i=5;
	iequals(__LINE__, 7&i--, 5);
	iequals(__LINE__, 7^i--, 3);
	iequals(__LINE__, 2|i--, 3);
	iequals(__LINE__, j&&i--, 1);
	iequals(__LINE__, j||i--, 1);
	i = 8;
	iequals(__LINE__, j?i--:0, 8);
	iequals(__LINE__, j=i--, 7);
	iequals(__LINE__, j *= i--, 42);
	j = 20;
	iequals(__LINE__, j /= i--, 4);
	iequals(__LINE__, j %= i--, 0);
	iequals(__LINE__, j += i--, 3);
	iequals(__LINE__, j -= i--, 1);
	j = 1 ;
	iequals(__LINE__, j <<= i--, 1<<1);
	iequals(__LINE__, j >>= i--, 2);
	iequals(__LINE__, i, -1);
	j = 7, i=5;
	iequals(__LINE__, j &= i--, 5);
	iequals(__LINE__, j ^= i--, 1);
	iequals(__LINE__, j |= i--, 3);
	iequals(__LINE__, (8,i--), 2); 
	}

/* ++X */
static void p2_7()
	{
	char *p = "abc";
	int i = 5;
	int j = 1;

	/* ++--X is illegal */
	/* &++X is illegal */
	iequals(__LINE__, *++p, 'b'); 
#if ANSI
	iequals(__LINE__, + ++i, 6);
#endif
	i = 8;
	iequals(__LINE__, - ++i, -9);
	iequals(__LINE__, ~++i, ~10);
	iequals(__LINE__, !++i, 0);
	/* sizeof ++i is undefined */
	dequals(__LINE__, (double)++i, 12.0);
	iequals(__LINE__, 2*++i, 26);
	iequals(__LINE__, 28/++i, 2);
	iequals(__LINE__, 9%++i, 9);
	iequals(__LINE__, 2+ ++i, 18);
	iequals(__LINE__, 18-++i, 1);
	i = 2;
	iequals(__LINE__, 8>>++i, 1);
	iequals(__LINE__, 1<<++i, 16);
	iequals(__LINE__, 2>++i, 0);
	iequals(__LINE__, 2<++i, 1);
	iequals(__LINE__, 7<=++i, 1);
	iequals(__LINE__, 8>=++i, 1);
	iequals(__LINE__, 9==++i, 1);
	iequals(__LINE__, 10!=++i, 0);
	i=4;
	iequals(__LINE__, 7&++i, 5);
	iequals(__LINE__, 7^++i, 1);
	iequals(__LINE__, 8|++i, 15);
	iequals(__LINE__, j&&++i, 1);
	iequals(__LINE__, j||++i, 1);
	i = 8;
	iequals(__LINE__, j?++i:0, 9);
	iequals(__LINE__, j = ++i, 10);
	iequals(__LINE__, j *= ++i, 110);
	j = 24;
	iequals(__LINE__, j /= ++i, 2);
	iequals(__LINE__, j %= ++i, 2);
	iequals(__LINE__, j += ++i, 16);
	iequals(__LINE__, j -= ++i, 1);
	i = 1;
	j = 1 ;
	iequals(__LINE__, j <<= 2, 4);
	iequals(__LINE__, j >>= ++i, 1);
	j = 7, i=5;
	iequals(__LINE__, j &= ++i, 6);
	iequals(__LINE__, j ^= ++i, 1);
	iequals(__LINE__, j |= ++i, 9);
	iequals(__LINE__, (8,++i), 9); 
	}

/* --X */
static void p2_8()
	{
	int i = 31;
	int j = 31;
	char *p = "abc";
	++p;
	/* &--X() is illegal */
	iequals(__LINE__, *--p, 'a');
#if ANSI
	iequals(__LINE__, +--j, 30);
#endif
	j = 31;
	iequals(__LINE__, - --j, -30);
	iequals(__LINE__, ~--j, ~29);
	iequals(__LINE__, !--j, 0);
	/* sizeof --X is illegal */
	dequals(__LINE__, (double)--j, 27.0);
	iequals(__LINE__, i*--j, 806);
	iequals(__LINE__, i/--j, 1);
	iequals(__LINE__, i%--j, 7);
	iequals(__LINE__, i+--j, 54);
	iequals(__LINE__, i- --j, 9);
	j = 3; iequals(__LINE__, i>>--j, 7);
	iequals(__LINE__, i<<--j, 62); j = 20;
	iequals(__LINE__, i>--j, 1);
	iequals(__LINE__, i<--j, 0);
	iequals(__LINE__, i<=--j, 0);
	iequals(__LINE__, i>=--j, 1);
	iequals(__LINE__, i==--j, 0);
	iequals(__LINE__, i!=--j, 1);
	iequals(__LINE__, i&--j, 13);
	iequals(__LINE__, i^--j, 19);
	iequals(__LINE__, i|--j, 31);
	iequals(__LINE__, i&&--j, 1);
	iequals(__LINE__, i||--j, 1);
	iequals(__LINE__, i?--j:0, 9);
	iequals(__LINE__, i = --j, 8);
	iequals(__LINE__, i *= --j, 56);
	iequals(__LINE__, i /= --j, 9);
	iequals(__LINE__, i %= --j, 4);
	iequals(__LINE__, i += --j, 8);
	iequals(__LINE__, i -= --j, 5);
	i = 2;
	j = 6;
	iequals(__LINE__, i <<= --j, 64);
	iequals(__LINE__, i >>= --j, 4);
	iequals(__LINE__, i &= --j, 0);
	iequals(__LINE__, i ^= --j, 2);
	iequals(__LINE__, i |= --j, 3);
	iequals(__LINE__, (8,--j), 0); 
	}

/* &X */
static void p2_9()
	{
	char *p = "amz";
	struct X *q = 0;
	int i = 20;
	int j = 0;

	aequals(__LINE__, *&p, p); 
#if ANSI
	/*  +&X is illegal */
#endif
	/*  -&X is illegal */
	/*  ~&X is illegal */
	/*  !&X is illegal */
	/* & sizeof X is illegal */
	iequals(__LINE__, sizeof&s, sizeof(struct X *));
	aequals(__LINE__, (char *)&s, (char *)ps);
	/* Y * & X is illegal */
	/* Y / & X is illegal */
	/* Y % & X is illegal */
	aequals(__LINE__, 1+&s, ps+1);
	iequals(__LINE__, ps-&s, 0);
	/* Y << & X is illegal */
	/* Y >> & X is illegal */
	iequals(__LINE__, ps>&s, 0);
	iequals(__LINE__, ps<&s, 0);
	iequals(__LINE__, ps<=&s, 1);
	iequals(__LINE__, ps>=&s, 1);
	iequals(__LINE__, ps==&s, 1);
	iequals(__LINE__, ps!=&s, 0);
	/* & X & Y is illegal */
	/* & X ^ Y is illegal */
	/* & X | Y is illegal */
	iequals(__LINE__, q&&&s, 0);
	iequals(__LINE__, q||&s, 1);
	aequals(__LINE__, q?0:&s, ps);
	aequals(__LINE__, q = &s, ps);
	/* Y  *=  &X is illegal */
	/* Y  /=  &X is illegal */
	/* Y  %=  &X is illegal */
	/* Y  +=  &X is illegal */
	/* Y  -=  &X is illegal */
	/* Y  <<=  &X is illegal */
	/* Y  >>=  &X is illegal */
	/* Y  &=  &X is illegal */
	/* Y  ^=  &X is illegal */
	/* Y  |=  &X is illegal */
	aequals(__LINE__, (8, &s), q); 
	}

/* *X */
static void p2_10()
	{
	int i = 31;
	int j = 2;
	int *p = &j;
	/* &--X() is illegal */
#if ANSI
	iequals(__LINE__, +*p, 2);
#endif
	i = 31;
	iequals(__LINE__, - *p, -2);
	iequals(__LINE__, ~*p, ~2);
	iequals(__LINE__, !*p, 0);
	/* sizeof --X is illegal */
	dequals(__LINE__, (double)*p, 2.0);
	iequals(__LINE__, i**p, 62);
	iequals(__LINE__, i/ *p, 15);
	iequals(__LINE__, i%*p, 1);
	iequals(__LINE__, i+*p, 33);
	iequals(__LINE__, i-*p, 29);
	iequals(__LINE__, i>>*p, 7);
	iequals(__LINE__, i<<*p, 124);
	iequals(__LINE__, i>*p, 1);
	iequals(__LINE__, i<*p, 0);
	iequals(__LINE__, i<=*p, 0);
	iequals(__LINE__, i>=*p, 1);
	iequals(__LINE__, i==*p, 0);
	iequals(__LINE__, i!=*p, 1);
	iequals(__LINE__, i&*p, 2);
	iequals(__LINE__, i^*p, 29);
	iequals(__LINE__, i|*p, 31);
	iequals(__LINE__, i&&*p, 1);
	iequals(__LINE__, i||*p, 1);
	iequals(__LINE__, i?*p:0, 2);
	iequals(__LINE__, i = *p, 2);
	iequals(__LINE__, i *= *p, 4);
	iequals(__LINE__, i /= *p, 2);
	iequals(__LINE__, i %= *p, 0);
	i = 1;
	iequals(__LINE__, i += *p, 3);
	iequals(__LINE__, i -= *p, 1);
	iequals(__LINE__, i <<= *p, 4);
	iequals(__LINE__, i >>= *p, 1);
	iequals(__LINE__, i &= *p, 0);
	iequals(__LINE__, i ^= *p, 2);
	iequals(__LINE__, i |= *p, 2);
	iequals(__LINE__, (8,*p), 2); 
	}

/* +X */
static void p2_11()
	{
	int i = 2;
	int j = 5;
#if ANSI
	iequals(__LINE__, -+j, -5);
	iequals(__LINE__, ~+j, ~5);
	iequals(__LINE__, !+j, 0);
	/* sizeof +X is illegal */
	dequals(__LINE__, (double)+j, 5.0);
	iequals(__LINE__, i*+j, 10);
	iequals(__LINE__, i/+j, 0);
	iequals(__LINE__, i%+j, 2);
	iequals(__LINE__, i+ +j, 7);
	iequals(__LINE__, i-+j, -3);
	iequals(__LINE__, i>>+j, 0);
	iequals(__LINE__, i<<+j, 64);
	iequals(__LINE__, i>+j, 0);
	iequals(__LINE__, i<+j, 1);
	iequals(__LINE__, i<=+j, 1);
	iequals(__LINE__, i>=+j, 0);
	iequals(__LINE__, i==+j, 0);
	iequals(__LINE__, i!=+j, 1);
	iequals(__LINE__, i&+j, 0);
	iequals(__LINE__, i^+j, 7);
	iequals(__LINE__, i|+j, 7);
	iequals(__LINE__, i&&+j, 1);
	iequals(__LINE__, i||+j, 1);
	iequals(__LINE__, i?+j:0, 5);
	iequals(__LINE__, i=+j, 5); i = 7;
	iequals(__LINE__, i *= +j, 35);
	iequals(__LINE__, i /= +j, 7);
	iequals(__LINE__, i %= +j, 2);
	iequals(__LINE__, i += +j, 7);
	iequals(__LINE__, i -= +j, 2);
	iequals(__LINE__, i <<= +j, 64);
	iequals(__LINE__, i >>= +j, 2);
	iequals(__LINE__, i&+j, 0);
	iequals(__LINE__, i ^= +j, 7);
	iequals(__LINE__, i |= +j, 7);
	iequals(__LINE__, (8,+j), 5); 
#endif
	}

/* -X */
static void p2_12()
	{
	int i = 5;
	int j = 2;
	iequals(__LINE__, - -j, 2);
	iequals(__LINE__, ~-j, ~(-2));
	iequals(__LINE__, !-j, 0);
	/* sizeof -X is illegal */
	dequals(__LINE__, (double)-j, -2.0);
	iequals(__LINE__, i*-j, -10);
	j = -2; iequals(__LINE__, i/-j, 2);
	iequals(__LINE__, i%-j, 1); j = 2;
	iequals(__LINE__, i+-j, 3);
	iequals(__LINE__, i- -j, 7);
	j = -2; iequals(__LINE__, i>>-j, 1);
	iequals(__LINE__, i<<-j, 20); j = 2;
	iequals(__LINE__, i>-j, 1);
	iequals(__LINE__, i<-j, 0);
	iequals(__LINE__, i<=-j, 0);
	iequals(__LINE__, i>=-j, 1);
	iequals(__LINE__, i==-j, 0);
	iequals(__LINE__, i!=-j, 1);
	iequals(__LINE__, i&-j, 5&(-2));
	iequals(__LINE__, i^-j, 5^(-2));
	iequals(__LINE__, i|-j, 5|(-2));
	iequals(__LINE__, i&&-j, 1);
	iequals(__LINE__, i||-j, 1);
	iequals(__LINE__, i?-j:0, -2);
	iequals(__LINE__, i = -j, -2);
	iequals(__LINE__, i *= -j, 4);
	iequals(__LINE__, i /= -j, -2);
	iequals(__LINE__, i %= -j, 0);
	i = 1;
	iequals(__LINE__, i += -j, -1);
	iequals(__LINE__, i -= -j, 1);
	i = 1;
	j = -2;
	iequals(__LINE__, i <<= -j, 4);
	iequals(__LINE__, i >>= -j, 1);
	iequals(__LINE__, i &= -j, 0);
	iequals(__LINE__, i ^= -j, 2);
	iequals(__LINE__, i |= -j, 2);
	iequals(__LINE__, (8,-j), 2); 
	}
  
/* ~X */
static void p2_13()
	{
	unsigned int i = 5;
	unsigned int j = 2;
	iequals(__LINE__, ~~j, 2);
	iequals(__LINE__, !~j, 0);
	/* sizeof !X is illegal */
	dequals(__LINE__, (double)~j, (double)(~(unsigned)2));
	iequals(__LINE__, i*~j, 5*(~(unsigned)2));
	iequals(__LINE__, i/~j, 5/(~(unsigned)2));
	iequals(__LINE__, i%~j, 5%(~(unsigned)2));
	iequals(__LINE__, i+~j, 5+(~(unsigned)2));
	iequals(__LINE__, i-~j, 5-(~(unsigned)2));
	j = ~j; iequals(__LINE__, i>>~j, 1);
	iequals(__LINE__, i<<~j, 20); j = ~j;
	iequals(__LINE__, i>~j, 0);
	iequals(__LINE__, i<~j, 1);
	iequals(__LINE__, i<=~j, 1);
	iequals(__LINE__, i>=~j, 0);
	iequals(__LINE__, i==~j, 0);
	iequals(__LINE__, i!=~j, 1);
	iequals(__LINE__, i&~j, 5);
	iequals(__LINE__, i^~j, ~(unsigned)7);
	iequals(__LINE__, i|~j, ~(unsigned)2);
	iequals(__LINE__, i&&~j, 1);
	iequals(__LINE__, i||~j, 1);
	iequals(__LINE__, i?~j:0, ~(unsigned)2);
	iequals(__LINE__, i=~j, ~(unsigned)2);
	iequals(__LINE__, i *= ~j, 9);
	iequals(__LINE__, i /= ~j, 0);
	iequals(__LINE__, i %= ~j, 0);
	iequals(__LINE__, i += ~j, ~(unsigned)2);
	iequals(__LINE__, i -= ~j, 0);
	i = 2;
	j = ~(unsigned)1;
	iequals(__LINE__, i <<= ~j, 4);
	iequals(__LINE__, i >>= ~j, 2);
	iequals(__LINE__, i&~j, 0);
	iequals(__LINE__, i ^= ~j, 3);
	iequals(__LINE__, i |= ~j, 3);
	iequals(__LINE__, (8,~j), 1); 
	}

/* ! X */
static void p2_14()
	{
	int i = 5;
	char j = 0;
	iequals(__LINE__, !!j, 0);
	dequals(__LINE__, (double)!j, 1.0);
	iequals(__LINE__, i*!j, 5);
	iequals(__LINE__, i/!j, 5);
	iequals(__LINE__, i%!j, 0);
	iequals(__LINE__, i+!j, 6);
	iequals(__LINE__, i-!j, 4);
	iequals(__LINE__, i>>!j, 2);
	iequals(__LINE__, i<<!j, 10);
	iequals(__LINE__, i>!j, 1);
	iequals(__LINE__, i<!j, 0);
	iequals(__LINE__, i<=!j,0);
	iequals(__LINE__, i>=!j, 1);
	iequals(__LINE__, i==!j, 0);
	iequals(__LINE__, i!=!j, 1);
	iequals(__LINE__, i&!j, 1);
	iequals(__LINE__, i^!j, 4);
	iequals(__LINE__, i|!j, 5);
	iequals(__LINE__, i&&!j, 1);
	iequals(__LINE__, i||!j, 1);
	iequals(__LINE__, i?!j:0, 1);
	iequals(__LINE__, i=!j, 1);
	iequals(__LINE__, i *= !j, 1);
	iequals(__LINE__, i /= !j, 1);
	iequals(__LINE__, i %= !j, 0);
	iequals(__LINE__, i += !j, 1);
	iequals(__LINE__, i -= !j, 0);
	i = 5;
	iequals(__LINE__, i <<= !j, 10);
	iequals(__LINE__, i >>= !j, 5);
	iequals(__LINE__, i &= !j, 1);
	iequals(__LINE__, i ^= !j, 0);
	iequals(__LINE__, i |= !j, 1);
	iequals(__LINE__, (8,!j), 1); 
	}

/* sizeof X */
static void p2_15()
	{
	int i = 55;
	int j = 2;
	int k = sizeof j;
	/* sizeof sizeof is illegal */
	dequals(__LINE__, (double)sizeof j, (double)k);
	iequals(__LINE__, i*sizeof j, i*k);
	iequals(__LINE__, i/sizeof j, i/k);
	iequals(__LINE__, i%sizeof j, i%k);
	iequals(__LINE__, i+sizeof j, i+k);
	iequals(__LINE__, i-sizeof j, i-k);
	iequals(__LINE__, i>>sizeof j, i>>k);
	iequals(__LINE__, i<<sizeof j, i<<k);
	iequals(__LINE__, i>sizeof j, i>k);
	iequals(__LINE__, i<sizeof j, i<k);
	iequals(__LINE__, i<=sizeof j, i<=k);
	iequals(__LINE__, i>=sizeof j, i>=k);
	iequals(__LINE__, i==sizeof j, i==k);
	iequals(__LINE__, i!=sizeof j, i!=k);
	iequals(__LINE__, i&sizeof j, i&k);
	iequals(__LINE__, i^sizeof j, i^k);
	iequals(__LINE__, i|sizeof j, i|k);
	iequals(__LINE__, i&&sizeof j, 1);
	iequals(__LINE__, i||sizeof j, 1);
	iequals(__LINE__, i?sizeof j:0, k);
	iequals(__LINE__, i=sizeof j, k);
	iequals(__LINE__, i *= sizeof j, k*k);
	iequals(__LINE__, i /= sizeof j, k);
	iequals(__LINE__, i %= sizeof (char), 0);
	iequals(__LINE__, i += sizeof j, k);
	iequals(__LINE__, i -= sizeof j, 0);
	i = 1;
	iequals(__LINE__, i <<= sizeof j, 1<<k);
	iequals(__LINE__, i >>= sizeof j, 1);
	iequals(__LINE__, i &= sizeof j, 1&k);
	iequals(__LINE__, i ^= sizeof j, k^1&k);
	iequals(__LINE__, i |= sizeof j, k^1&k|k);
	iequals(__LINE__, (8,sizeof j), k); 
	}

 /* (cast) */
 static void p2_16()
	{
	int i = 5;
	float j = 2.0;
	iequals(__LINE__, i-(int) j, 3);
	iequals(__LINE__, i>>(int)j, 1);
	iequals(__LINE__, i<<(int)j, 20);
	iequals(__LINE__, i>(int)j, 1);
	iequals(__LINE__, i<(int)j, 0);
	iequals(__LINE__, i<=(int)j, 0);
	iequals(__LINE__, i>=(int)j, 1);
	iequals(__LINE__, i==(int)j, 0);
	iequals(__LINE__, i!=(int)j, 1);
	iequals(__LINE__, i&(int)j, 0);
	iequals(__LINE__, i^(int)j, 7);
	iequals(__LINE__, i|(int)j, 7);
	iequals(__LINE__, i&&(int)j, 1);
	iequals(__LINE__, i||(int)j, 1);
	iequals(__LINE__, i?(int)j:0, 2);
	iequals(__LINE__, i=(int)j, 2);
	iequals(__LINE__, i *= (int)j, 4);
	iequals(__LINE__, i /= (int)j, 2);
	iequals(__LINE__, i %= (int)j, 0);
	iequals(__LINE__, i += (int)j, 2);
	iequals(__LINE__, i -= (int)j, 0);
	i = 1;
	iequals(__LINE__, i <<= (int)j, 4);
	iequals(__LINE__, i >>= (int)j, 1);
	iequals(__LINE__, i&(int)j, 0);
	iequals(__LINE__, i ^= (int)j, 3);
	iequals(__LINE__, i |= (int)j, 3);
	iequals(__LINE__, (8,(int)j), 2); 
	}
