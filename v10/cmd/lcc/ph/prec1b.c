/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "defs.h"

/*
 * PREC1B -- try all operator pairs -- this file uses all binary operators
 * in the first position (see prec 2 for the other way around).
 */

static void p1_17();
static void p1_18();
static void p1_19();
static void p1_20();
static void p1_21();
static void p1_22();
static void p1_23();
static void p1_24();
static void p1_25();
static void p1_26();
static void p1_27();
static void p1_28();
static void p1_29();
static void p1_30();
static void p1_31();
static void p1_32();
static void p1_33();
static void p1_34();
static void p1_35();

void prec1b()
	{
	Filename = "prec1b.c";
	p1_17();	/* * */
	p1_18();	/* / */
	p1_19();	/* % */
	p1_20();	/* + */
	p1_21();	/* - */
	p1_22();	/* >> */
	p1_23();	/* << */
	p1_24();	/* > */
	p1_25();	/* < */
	p1_26();	/* >= */
	p1_27();	/* <= */
	p1_28();	/* == */
	p1_29();	/* != */
	p1_30();	/* & */
	p1_31();	/* ^ */
	p1_32();	/* | */
	p1_33();	/* && */
	p1_34();	/* || */
	p1_35();	/* ?= */
	}

/* X * Y */
static void p1_17()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i*j*k, 24);
	iequals(__LINE__, i*j/k, 6);
	iequals(__LINE__, i*j%k, 0);
	iequals(__LINE__, i*j+k, 14);
	iequals(__LINE__, i*j-k, 10);
	iequals(__LINE__, i*j>>k, 3);
	iequals(__LINE__, i*j<<k, 48);
	iequals(__LINE__, i*j>k, 1);
	iequals(__LINE__, i*j<k, 0);
	iequals(__LINE__, i*j<=k, 0);
	iequals(__LINE__, i*j>=k, 1);
	iequals(__LINE__, i*j==k, 0);
	iequals(__LINE__, i*j!=k, 1);
	iequals(__LINE__, i*j&k, 0);
	iequals(__LINE__, i*j^k, 14);
	iequals(__LINE__, i*j|k, 14);
	iequals(__LINE__, i*j&&k,1);
	iequals(__LINE__, i*j||k, 1);
	iequals(__LINE__, i*j?k:0, 2);
	/* X*Y=Z is illegal */
	/* X*Y*=Z is illegal */
	/* X*Y/=Z is illegal */
	/* X*Y%=Z is illegal */
	/* X*Y+=Z is illegal */
	/* X*Y-=Z is illegal */
	/* X*Y<<=Z is illegal */
	/* X*Y>>=Z is illegal */
	/* X*Y&=Z is illegal */
	/* X*Y^=Z is illegal */
	/* X*Y|=Z is illegal */
	iequals(__LINE__, (i*j,k), k);
	}

/* X / Y */
static void p1_18()
	{
	int i = 17;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i/j/k, 2);
	iequals(__LINE__, i/j%k, 1);
	iequals(__LINE__, i/j+k, 7);
	iequals(__LINE__, i/j-k, 3);
	iequals(__LINE__, i/j>>k, 1);
	iequals(__LINE__, i/j<<k, 20);
	iequals(__LINE__, i/j>k, 1);
	iequals(__LINE__, i/j<k, 0);
	iequals(__LINE__, i/j<=k, 0);
	iequals(__LINE__, i/j>=k, 1);
	iequals(__LINE__, i/j==k, 0);
	iequals(__LINE__, i/j!=k, 1);
	iequals(__LINE__, i/j&k, 0);
	iequals(__LINE__, i/j^k, 7);
	iequals(__LINE__, i/j|k, 7);
	iequals(__LINE__, i/j&&k,1);
	iequals(__LINE__, i/j||k, 1);
	iequals(__LINE__, i/j?k:0, 2);
	/* X/Y=Z is illegal */
	/* X/Y*=Z is illegal */
	/* X/Y/=Z is illegal */
	/* X/Y%=Z is illegal */
	/* X/Y+=Z is illegal */
	/* X/Y-=Z is illegal */
	/* X/Y<<=Z is illegal */
	/* X/Y>>=Z is illegal */
	/* X/Y&=Z is illegal */
	/* X/Y^=Z is illegal */
	/* X/Y|=Z is illegal */
	iequals(__LINE__, (i/j,k), 2);
	}

/* X % Y */
static void p1_19()
	{
	int i = 17;
	int j = 10;
	int k = 2;
	iequals(__LINE__, i%j%k, 1);
	iequals(__LINE__, i%j+k, 9);
	iequals(__LINE__, i%j-k, 5);
	iequals(__LINE__, i%j>>k, 1);
	iequals(__LINE__, i%j<<k, 28);
	iequals(__LINE__, i%j>k, 1);
	iequals(__LINE__, i%j<k, 0);
	iequals(__LINE__, i%j<=k, 0);
	iequals(__LINE__, i%j>=k, 1);
	iequals(__LINE__, i%j==k, 0);
	iequals(__LINE__, i%j!=k, 1);
	iequals(__LINE__, i%j&k, 2);
	iequals(__LINE__, i%j^k, 5);
	iequals(__LINE__, i%j|k, 7);
	iequals(__LINE__, i%j&&k,1);
	iequals(__LINE__, i%j||k, 1);
	iequals(__LINE__, i%j?k:0, 2);
	/* X%Y=Z is illegal */
	/* X%Y*=Z is illegal */
	/* X%Y/=Z is illegal */
	/* X%Y%=Z is illegal */
	/* X%Y+=Z is illegal */
	/* X%Y-=Z is illegal */
	/* X%Y<<=Z is illegal */
	/* X%Y>>=Z is illegal */
	/* X%Y&=Z is illegal */
	/* X%Y^=Z is illegal */
	/* X%Y|=Z is illegal */
	iequals(__LINE__, (i%j,k), 2);
	}

/* X + Y */
static void p1_20()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i+j+k, 9);
	iequals(__LINE__, i+j-k, 5);
	iequals(__LINE__, i+j>>k, 1);
	iequals(__LINE__, i+j<<k, 28);
	iequals(__LINE__, i+j>k, 1);
	iequals(__LINE__, i+j<k, 0);
	iequals(__LINE__, i+j<=k, 0);
	iequals(__LINE__, i+j>=k, 1);
	iequals(__LINE__, i+j==k, 0);
	iequals(__LINE__, i+j!=k, 1);
	iequals(__LINE__, i+j&k, 2);
	iequals(__LINE__, i+j^k, 5);
	iequals(__LINE__, i+j|k, 7);
	iequals(__LINE__, i+j&&k,1);
	iequals(__LINE__, i+j||k, 1);
	iequals(__LINE__, i+j?k:0, 2);
	/* X+Y=Z is illegal */
	/* X+Y*=Z is illegal */
	/* X+Y/=Z is illegal */
	/* X+Y%=Z is illegal */
	/* X+Y+=Z is illegal */
	/* X+Y-=Z is illegal */
	/* X+Y<<=Z is illegal */
	/* X+Y>>=Z is illegal */
	/* X+Y&=Z is illegal */
	/* X+Y^=Z is illegal */
	/* X+Y|=Z is illegal */
	iequals(__LINE__, (i+j,k), 2);
	}

/* X - Y */
static void p1_21()
	{
	int i = 9;
	int j = 2;
	int k = 2;
	iequals(__LINE__, i-j-k, 5);
	iequals(__LINE__, i-j>>k, 1);
	iequals(__LINE__, i-j<<k, 28);
	iequals(__LINE__, i-j>k, 1);
	iequals(__LINE__, i-j<k, 0);
	iequals(__LINE__, i-j<=k, 0);
	iequals(__LINE__, i-j>=k, 1);
	iequals(__LINE__, i-j==k, 0);
	iequals(__LINE__, i-j!=k, 1);
	iequals(__LINE__, i-j&k, 2);
	iequals(__LINE__, i-j^k, 5);
	iequals(__LINE__, i-j|k, 7);
	iequals(__LINE__, i-j&&k,1);
	iequals(__LINE__, i-j||k, 1);
	iequals(__LINE__, i-j?k:0, 2);
	/* X-Y=Z is illegal */
	/* X-Y*=Z is illegal */
	/* X-Y/=Z is illegal */
	/* X-Y%=Z is illegal */
	/* X-Y+=Z is illegal */
	/* X-Y-=Z is illegal */
	/* X-Y<<=Z is illegal */
	/* X-Y>>=Z is illegal */
	/* X-Y&=Z is illegal */
	/* X-Y^=Z is illegal */
	/* X-Y|=Z is illegal */
	iequals(__LINE__, (i-j,k), 2);
	}

/* X >> Y */
static void p1_22()
	{
	int i = 28;
	int j = 2;
	int k = 2;
	iequals(__LINE__, i>>j>>k, 1);
	iequals(__LINE__, i>>j<<k, 28);
	iequals(__LINE__, i>>j>k, 1);
	iequals(__LINE__, i>>j<k, 0);
	iequals(__LINE__, i>>j<=k, 0);
	iequals(__LINE__, i>>j>=k, 1);
	iequals(__LINE__, i>>j==k, 0);
	iequals(__LINE__, i>>j!=k, 1);
	iequals(__LINE__, i>>j&k, 2);
	iequals(__LINE__, i>>j^k, 5);
	iequals(__LINE__, i>>j|k, 7);
	iequals(__LINE__, i>>j&&k,1);
	iequals(__LINE__, i>>j||k, 1);
	iequals(__LINE__, i>>j?k:0, 2);
	/* X>>Y=Z is illegal */
	/* X>>Y*=Z is illegal */
	/* X>>Y/=Z is illegal */
	/* X>>Y%=Z is illegal */
	/* X>>Y+=Z is illegal */
	/* X>>Y-=Z is illegal */
	/* X>>Y<<=Z is illegal */
	/* X>>Y>>=Z is illegal */
	/* X>>Y&=Z is illegal */
	/* X>>Y^=Z is illegal */
	/* X>>Y|=Z is illegal */
	iequals(__LINE__, (i>>j,k), 2);
	}

/* X << Y */
static void p1_23()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i<<j<<k, 128);
	iequals(__LINE__, i<<j>k, 1);
	iequals(__LINE__, i<<j<k, 0);
	iequals(__LINE__, i<<j<=k, 0);
	iequals(__LINE__, i<<j>=k, 1);
	iequals(__LINE__, i<<j==k, 0);
	iequals(__LINE__, i<<j!=k, 1);
	iequals(__LINE__, i<<j&k, 0);
	iequals(__LINE__, i<<j^k, 34);
	iequals(__LINE__, i<<j|k, 34);
	iequals(__LINE__, i<<j&&k,1);
	iequals(__LINE__, i<<j||k, 1);
	iequals(__LINE__, i<<j?k:0, 2);
	/* X<<Y=Z is illegal */
	/* X<<Y*=Z is illegal */
	/* X<<Y/=Z is illegal */
	/* X<<Y%=Z is illegal */
	/* X<<Y+=Z is illegal */
	/* X<<Y-=Z is illegal */
	/* X<<Y<<=Z is illegal */
	/* X<<Y>>=Z is illegal */
	/* X<<Y&=Z is illegal */
	/* X<<Y^=Z is illegal */
	/* X<<Y|=Z is illegal */
	iequals(__LINE__, (i<<j,k), 2);
	}

/* X > Y */
static void p1_24()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i>j>k, 0);
	iequals(__LINE__, i>j<k, 1);
	iequals(__LINE__, i>j<=k, 1);
	iequals(__LINE__, i>j>=k, 0);
	iequals(__LINE__, i>j==k, 0);
	iequals(__LINE__, i>j!=k, 1);
	iequals(__LINE__, i>j&k, 0);
	iequals(__LINE__, i>j^k, 3);
	iequals(__LINE__, i>j|k, 3);
	iequals(__LINE__, i>j&&k,1);
	iequals(__LINE__, i>j||k, 1);
	iequals(__LINE__, i>j?k:0, 2);
	/* X>Y=Z is illegal */
	/* X>Y*=Z is illegal */
	/* X>Y/=Z is illegal */
	/* X>Y%=Z is illegal */
	/* X>Y+=Z is illegal */
	/* X>Y-=Z is illegal */
	/* X>Y<<=Z is illegal */
	/* X>Y>>=Z is illegal */
	/* X>Y&=Z is illegal */
	/* X>Y^=Z is illegal */
	/* X>Y|=Z is illegal */
	iequals(__LINE__, (i>j,k), 2);
	}

/* X < Y */
static void p1_25()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i<j<k, 1);
	iequals(__LINE__, i<j<=k, 1);
	iequals(__LINE__, i<j>=k, 0);
	iequals(__LINE__, i<j==k, 0);
	iequals(__LINE__, i<j!=k, 1);
	iequals(__LINE__, i<j&k, 0);
	iequals(__LINE__, i<j^k, 2);
	iequals(__LINE__, i<j|k, 2);
	iequals(__LINE__, i<j&&k,0);
	iequals(__LINE__, i<j||k, 1);
	iequals(__LINE__, i<j?k:0, 0);
	/* X<Y=Z is illegal */
	/* X<Y*=Z is illegal */
	/* X<Y/=Z is illegal */
	/* X<Y%=Z is illegal */
	/* X<Y+=Z is illegal */
	/* X<Y-=Z is illegal */
	/* X<Y<<=Z is illegal */
	/* X<Y>>=Z is illegal */
	/* X<Y&=Z is illegal */
	/* X<Y^=Z is illegal */
	/* X<Y|=Z is illegal */
	iequals(__LINE__, (i<j,k), 2);
	}

/* X <= Y */
static void p1_26()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i<=j<=k, 1);
	iequals(__LINE__, i<=j>=k, 0);
	iequals(__LINE__, i<=j==k, 0);
	iequals(__LINE__, i<=j!=k, 1);
	iequals(__LINE__, i<=j&k, 0);
	iequals(__LINE__, i<=j^k, 2);
	iequals(__LINE__, i<=j|k, 2);
	iequals(__LINE__, i<=j&&k,0);
	iequals(__LINE__, i<=j||k, 1);
	iequals(__LINE__, i<=j?k:0, 0);
	/* X<=Y=Z is illegal */
	/* X<=Y*=Z is illegal */
	/* X<=Y/=Z is illegal */
	/* X<=Y%=Z is illegal */
	/* X<=Y+=Z is illegal */
	/* X<=Y-=Z is illegal */
	/* X<=Y<<=Z is illegal */
	/* X<=Y>>=Z is illegal */
	/* X<=Y&=Z is illegal */
	/* X<=Y^=Z is illegal */
	/* X<=Y|=Z is illegal */
	iequals(__LINE__, (i<=j,k), 2);
	}

/* X >= Y */
static void p1_27()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i>=j>=k, 0);
	iequals(__LINE__, i>=j==k, 0);
	iequals(__LINE__, i>=j!=k, 1);
	iequals(__LINE__, i>=j&k, 0);
	iequals(__LINE__, i>=j^k, 3);
	iequals(__LINE__, i>=j|k, 3);
	iequals(__LINE__, i>=j&&k,1);
	iequals(__LINE__, i>=j||k, 1);
	iequals(__LINE__, i>=j?k:0, 2);
	/* X>=Y=Z is illegal */
	/* X>=Y*=Z is illegal */
	/* X>=Y/=Z is illegal */
	/* X>=Y%=Z is illegal */
	/* X>=Y+=Z is illegal */
	/* X>=Y-=Z is illegal */
	/* X>=Y<<=Z is illegal */
	/* X>=Y>>=Z is illegal */
	/* X>=Y&=Z is illegal */
	/* X>=Y^=Z is illegal */
	/* X>=Y|=Z is illegal */
	iequals(__LINE__, (i>=j,k), 2);
	}

/* X == Y */
static void p1_28()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i==j==k, 0);
	iequals(__LINE__, i==j!=k, 1);
	iequals(__LINE__, i==j&k, 0);
	iequals(__LINE__, i==j^k, 2);
	iequals(__LINE__, i==j|k, 2);
	iequals(__LINE__, i==j&&k,0);
	iequals(__LINE__, i==j||k, 1);
	iequals(__LINE__, i==j?k:0, 0);
	/* X==Y=Z is illegal */
	/* X==Y*=Z is illegal */
	/* X==Y/=Z is illegal */
	/* X==Y%=Z is illegal */
	/* X==Y+=Z is illegal */
	/* X==Y-=Z is illegal */
	/* X==Y<<=Z is illegal */
	/* X==Y>>=Z is illegal */
	/* X==Y&=Z is illegal */
	/* X==Y^=Z is illegal */
	/* X==Y|=Z is illegal */
	iequals(__LINE__, (i==j,k), 2);
	}

/* X != Y */
static void p1_29()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i!=j!=k, 1);
	iequals(__LINE__, i!=j&k, 0);
	iequals(__LINE__, i!=j^k, 3);
	iequals(__LINE__, i!=j|k, 3);
	iequals(__LINE__, i!=j&&k,1);
	iequals(__LINE__, i!=j||k, 1);
	iequals(__LINE__, i!=j?k:0, 2);
	/* X!=Y=Z is illegal */
	/* X!=Y*=Z is illegal */
	/* X!=Y/=Z is illegal */
	/* X!=Y%=Z is illegal */
	/* X!=Y+=Z is illegal */
	/* X!=Y-=Z is illegal */
	/* X!=Y<<=Z is illegal */
	/* X!=Y>>=Z is illegal */
	/* X!=Y&=Z is illegal */
	/* X!=Y^=Z is illegal */
	/* X!=Y|=Z is illegal */
	iequals(__LINE__, (i!=j,k), 2);
	}

/* X & Y */
static void p1_30()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i&j&k, 0);
	iequals(__LINE__, i&j^k, 2);
	iequals(__LINE__, i&j|k, 2);
	iequals(__LINE__, i&j&&k,0);
	iequals(__LINE__, i&j||k, 1);
	iequals(__LINE__, i&j?k:0, 0);
	/* X&Y=Z is illegal */
	/* X&Y*=Z is illegal */
	/* X&Y/=Z is illegal */
	/* X&Y%=Z is illegal */
	/* X&Y+=Z is illegal */
	/* X&Y-=Z is illegal */
	/* X&Y<<=Z is illegal */
	/* X&Y>>=Z is illegal */
	/* X&Y&=Z is illegal */
	/* X&Y^=Z is illegal */
	/* X&Y|=Z is illegal */
	iequals(__LINE__, (i&j,k), 2);
	}

/* X ^ Y */
static void p1_31()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i^j^k, 5);
	iequals(__LINE__, i^j|k, 7);
	iequals(__LINE__, i^j&&k,1);
	iequals(__LINE__, i^j||k, 1);
	iequals(__LINE__, i^j?k:0, 2);
	/* X^Y=Z is illegal */
	/* X^Y*=Z is illegal */
	/* X^Y/=Z is illegal */
	/* X^Y%=Z is illegal */
	/* X^Y+=Z is illegal */
	/* X^Y-=Z is illegal */
	/* X^Y<<=Z is illegal */
	/* X^Y>>=Z is illegal */
	/* X^Y&=Z is illegal */
	/* X^Y^=Z is illegal */
	/* X^Y|=Z is illegal */
	iequals(__LINE__, (i^j,k), 2);
	}

/* X | Y */
static void p1_32()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i|j|k, 7);
	iequals(__LINE__, i|j&&k,1);
	iequals(__LINE__, i|j||k, 1);
	iequals(__LINE__, i|j?k:0, 2);
	/* X|Y=Z is illegal */
	/* X|Y*=Z is illegal */
	/* X|Y/=Z is illegal */
	/* X|Y%=Z is illegal */
	/* X|Y+=Z is illegal */
	/* X|Y-=Z is illegal */
	/* X|Y<<=Z is illegal */
	/* X|Y>>=Z is illegal */
	/* X|Y&=Z is illegal */
	/* X|Y^=Z is illegal */
	/* X|Y|=Z is illegal */
	iequals(__LINE__, (i|j,k), 2);
	}

/* X && Y */
static void p1_33()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i&&j&&k, 1);
	iequals(__LINE__, i&&j||k, 1);
	iequals(__LINE__, i&&j?k:0, 2);
	/* X&&Y=Z is illegal */
	/* X&&Y*=Z is illegal */
	/* X&&Y/=Z is illegal */
	/* X&&Y%=Z is illegal */
	/* X&&Y+=Z is illegal */
	/* X&&Y-=Z is illegal */
	/* X&&Y<<=Z is illegal */
	/* X&&Y>>=Z is illegal */
	/* X&&Y&=Z is illegal */
	/* X&&Y^=Z is illegal */
	/* X&&Y|=Z is illegal */
	iequals(__LINE__, (i&&j,k), 2);
	}

/* X || Y */
static void p1_34()
	{
	int i = 4;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i||j||k, 1);
	iequals(__LINE__, i||j?k:0, 2);
	/* X||Y=Z is illegal */
	/* X||Y*=Z is illegal */
	/* X||Y/=Z is illegal */
	/* X||Y%=Z is illegal */
	/* X||Y+=Z is illegal */
	/* X||Y-=Z is illegal */
	/* X||Y<<=Z is illegal */
	/* X||Y>>=Z is illegal */
	/* X||Y&=Z is illegal */
	/* X||Y^=Z is illegal */
	/* X||Y|=Z is illegal */
	iequals(__LINE__, (i||j,k), 2);
	}

/* X ? Y : Z */
static void p1_35()
	{
	int i = 0;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i? j : j?k:0, 2);
	/* X?:Y=Z is illegal */
	/* X?:Y*=Z is illegal */
	/* X?:Y/=Z is illegal */
	/* X?:Y%=Z is illegal */
	/* X?:Y+=Z is illegal */
	/* X?:Y-=Z is illegal */
	/* X?:Y<<=Z is illegal */
	/* X?:Y>>=Z is illegal */
	/* X?:Y&=Z is illegal */
	/* X?:Y^=Z is illegal */
	/* X?:Y|=Z is illegal */
	iequals(__LINE__, (i?j:j,k), 2);
	}

