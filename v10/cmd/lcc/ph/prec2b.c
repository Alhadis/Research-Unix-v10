/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 * PREC2B - the fourth quarter of precedence checking 
 */
#include "defs.h"

static void p2_17();
static void p2_18();
static void p2_19();
static void p2_20();
static void p2_21();
static void p2_22();
static void p2_23();
static void p2_24();
static void p2_25();
static void p2_26();
static void p2_27();
static void p2_28();
static void p2_29();
static void p2_30();
static void p2_31();
static void p2_32();
static void p2_33();
static void p2_34();
static void p2_35();

void prec2b()
	{
	Filename = "prec2b.c";
	p2_17();	/* * */
	p2_18();	/* / */
	p2_19();	/* % */
	p2_20();	/* + */
	p2_21();	/* - */
	p2_22();	/* >> */
	p2_23();	/* << */
	p2_24();	/* > */
	p2_25();	/* < */
	p2_26();	/* >= */
	p2_27();	/* <= */
	p2_28();	/* == */
	p2_29();	/* != */
	p2_30();	/* & */
	p2_31();	/* ^ */
	p2_32();	/* | */
	p2_33();	/* && */
	p2_34();	/* || */
	p2_35();	/* ?: */
	}

/* Z op X*Y */
static void p2_17()
	{
	int i = 7;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i/j*k, 4);
	iequals(__LINE__, i%j*k, 2);
	iequals(__LINE__, i+j*k, 13);
	iequals(__LINE__, i-j*k, 1);
	iequals(__LINE__, i>>j*k, 0);
	iequals(__LINE__, i<<j*k, 448);
	iequals(__LINE__, i>j*k, 1);
	iequals(__LINE__, i<j*k, 0);
	iequals(__LINE__, i<=j*k, 0);
	iequals(__LINE__, i>=j*k, 1);
	iequals(__LINE__, i==j*k, 0);
	iequals(__LINE__, i!=j*k, 1);
	iequals(__LINE__, i&j*k, 6);
	iequals(__LINE__, i^j*k, 1);
	iequals(__LINE__, i|j*k, 7);
	iequals(__LINE__, i&&j*k, 1);
	iequals(__LINE__, i||j*k, 1);
	iequals(__LINE__, i?j*k:0, 6);
	iequals(__LINE__, i=j*k, 6);
	iequals(__LINE__, i*=j*k, 36);
	iequals(__LINE__, i/=j*k, 6);
	iequals(__LINE__, i%=j*k, 0);
	iequals(__LINE__, i+=j*k, 6);
	i = 8;
	iequals(__LINE__, i-=j*k, 2);
	iequals(__LINE__, i<<=j*k, 128);
	iequals(__LINE__, i>>=j*k, 2);
	iequals(__LINE__, i&=j*k, 2);
	iequals(__LINE__, i^=j*k, 4);
	iequals(__LINE__, i|=j*k, 6);
	iequals(__LINE__, (i,j*k), 6);
	}

/* Z op X/Y */
static void p2_18()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i/j/k, 0);
	iequals(__LINE__, i%j/k, 1);
	iequals(__LINE__, i+j/k, 9);
	iequals(__LINE__, i-j/k, 5);
	iequals(__LINE__, i>>j/k, 1);
	iequals(__LINE__, i<<j/k, 28);
	iequals(__LINE__, i>j/k, 1);
	iequals(__LINE__, i<j/k, 0);
	iequals(__LINE__, i<=j/k, 0);
	iequals(__LINE__, i>=j/k, 1);
	iequals(__LINE__, i==j/k, 0);
	iequals(__LINE__, i!=j/k, 1);
	iequals(__LINE__, i&j/k, 2);
	iequals(__LINE__, i^j/k, 5);
	iequals(__LINE__, i|j/k, 7);
	iequals(__LINE__, i&&j/k, 1);
	iequals(__LINE__, i||j/k, 1);
	iequals(__LINE__, i?j/k:0, 2);
	iequals(__LINE__, i=j/k, 2);
	iequals(__LINE__, i*=j/k, 4);
	iequals(__LINE__, i/=j/k, 2);
	iequals(__LINE__, i%=j/k, 0);
	iequals(__LINE__, i+=j/k, 2);
	i = 3;
	iequals(__LINE__, i-=j/k, 1);
	iequals(__LINE__, i<<=j/k, 4);
	iequals(__LINE__, i>>=j/k, 1);
	iequals(__LINE__, i&=j/k, 0);
	iequals(__LINE__, i^=j/k, 2);
	iequals(__LINE__, i|=j/k, 2);
	iequals(__LINE__, (i,j/k), 2);
	}

/* Z op X%Y */
static void p2_19()
	{
	int i = 7;
	int j = 6;
	int k = 4;
	iequals(__LINE__, i%j%k, 1);
	iequals(__LINE__, i+j%k, 9);
	iequals(__LINE__, i-j%k, 5);
	iequals(__LINE__, i>>j%k, 1);
	iequals(__LINE__, i<<j%k, 28);
	iequals(__LINE__, i>j%k, 1);
	iequals(__LINE__, i<j%k, 0);
	iequals(__LINE__, i<=j%k, 0);
	iequals(__LINE__, i>=j%k, 1);
	iequals(__LINE__, i==j%k, 0);
	iequals(__LINE__, i!=j%k, 1);
	iequals(__LINE__, i&j%k, 2);
	iequals(__LINE__, i^j%k, 5);
	iequals(__LINE__, i|j%k, 7);
	iequals(__LINE__, i&&j%k, 1);
	iequals(__LINE__, i||j%k, 1);
	iequals(__LINE__, i?j%k:0, 2);
	iequals(__LINE__, i=j%k, 2);
	iequals(__LINE__, i*=j%k, 4);
	iequals(__LINE__, i/=j%k, 2);
	iequals(__LINE__, i%=j%k, 0);
	iequals(__LINE__, i+=j%k, 2);
	i = 3;
	iequals(__LINE__, i-=j%k, 1);
	iequals(__LINE__, i<<=j%k, 4);
	iequals(__LINE__, i>>=j%k, 1);
	iequals(__LINE__, i&=j%k, 0);
	iequals(__LINE__, i^=j%k, 2);
	iequals(__LINE__, i|=j%k, 2);
	iequals(__LINE__, (i,j%k), 2);
	}

/* Z op X+Y */
static void p2_20()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i+j+k, 13);
	iequals(__LINE__, i-j+k, 5);
	iequals(__LINE__, i>>j+k, 0);
	iequals(__LINE__, i<<j+k, 448);
	iequals(__LINE__, i>j+k, 1);
	iequals(__LINE__, i<j+k, 0);
	iequals(__LINE__, i<=j+k, 0);
	iequals(__LINE__, i>=j+k, 1);
	iequals(__LINE__, i==j+k, 0);
	iequals(__LINE__, i!=j+k, 1);
	iequals(__LINE__, i&j+k, 6);
	iequals(__LINE__, i^j+k, 1);
	iequals(__LINE__, i|j+k, 7);
	iequals(__LINE__, i&&j+k, 1);
	iequals(__LINE__, i||j+k, 1);
	iequals(__LINE__, i?j+k:0, 6);
	iequals(__LINE__, i=j+k, 6);
	iequals(__LINE__, i*=j+k, 36);
	iequals(__LINE__, i/=j+k, 6);
	iequals(__LINE__, i%=j+k, 0);
	iequals(__LINE__, i+=j+k, 6);
	i = 8;
	iequals(__LINE__, i-=j+k, 2);
	iequals(__LINE__, i<<=j+k, 2<<6);
	iequals(__LINE__, i>>=j+k, 2);
	iequals(__LINE__, i&=j+k, 2);
	iequals(__LINE__, i^=j+k, 4);
	iequals(__LINE__, i|=j+k, 6);
	iequals(__LINE__, (i,j+k), 6);
	}

/* Z op X-Y */
static void p2_21()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i-j-k, 1);
	iequals(__LINE__, i>>j-k, 1);
	iequals(__LINE__, i<<j-k, 28);
	iequals(__LINE__, i>j-k, 1);
	iequals(__LINE__, i<j-k, 0);
	iequals(__LINE__, i<=j-k, 0);
	iequals(__LINE__, i>=j-k, 1);
	iequals(__LINE__, i==j-k, 0);
	iequals(__LINE__, i!=j-k, 1);
	iequals(__LINE__, i&j-k, 2);
	iequals(__LINE__, i^j-k, 5);
	iequals(__LINE__, i|j-k, 7);
	iequals(__LINE__, i&&j-k, 1);
	iequals(__LINE__, i||j-k, 1);
	iequals(__LINE__, i?j-k:0, 2);
	iequals(__LINE__, i=j-k, 2);
	iequals(__LINE__, i*=j-k, 4);
	iequals(__LINE__, i/=j-k, 2);
	iequals(__LINE__, i%=j-k, 0);
	iequals(__LINE__, i+=j-k, 2);
	iequals(__LINE__, i-=j-k, 0);
	i = 3;
	iequals(__LINE__, i<<=j-k, 12);
	iequals(__LINE__, i>>=j-k, 3);
	iequals(__LINE__, i&=j-k, 2);
	iequals(__LINE__, i^=j-k, 0);
	iequals(__LINE__, i|=j-k, 2);
	iequals(__LINE__, (i,j-k), 2);
	}

/* Z op X>>Y */
static void p2_22()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i>>j>>k, 0);
	iequals(__LINE__, i<<j>>k, 28);
	iequals(__LINE__, i>j>>k, 1);
	iequals(__LINE__, i<j>>k, 0);
	iequals(__LINE__, i<=j>>k, 0);
	iequals(__LINE__, i>=j>>k, 1);
	iequals(__LINE__, i==j>>k, 0);
	iequals(__LINE__, i!=j>>k, 1);
	iequals(__LINE__, i&j>>k, 1);
	iequals(__LINE__, i^j>>k, 6);
	iequals(__LINE__, i|j>>k, 7);
	iequals(__LINE__, i&&j>>k, 1);
	iequals(__LINE__, i||j>>k, 1);
	iequals(__LINE__, i?j>>k:0, 1);
	iequals(__LINE__, i=j>>k, 1);
	iequals(__LINE__, i*=j>>k, 1);
	iequals(__LINE__, i/=j>>k, 1);
	iequals(__LINE__, i%=j>>k, 0);
	iequals(__LINE__, i+=j>>k, 1);
	iequals(__LINE__, i-=j>>k, 0);
	i = 3;
	iequals(__LINE__, i<<=j>>k, 6);
	iequals(__LINE__, i>>=j>>k, 3);
	iequals(__LINE__, i&=j>>k, 1);
	iequals(__LINE__, i^=j>>k, 0);
	iequals(__LINE__, i|=j>>k, 1);
	iequals(__LINE__, (i,j>>k), 1);
	}

/* Z op X<<Y */
static void p2_23()
	{
	int i = 7;
	int j = 2;
	int k = 1;
	iequals(__LINE__, i<<j<<k, 56);
	iequals(__LINE__, i>j<<k, 1);
	iequals(__LINE__, i<j<<k, 0);
	iequals(__LINE__, i<=j<<k, 0);
	iequals(__LINE__, i>=j<<k, 1);
	iequals(__LINE__, i==j<<k, 0);
	iequals(__LINE__, i!=j<<k, 1);
	iequals(__LINE__, i&j<<k, 4);
	iequals(__LINE__, i^j<<k, 3);
	iequals(__LINE__, i|j<<k, 7);
	iequals(__LINE__, i&&j<<k, 1);
	iequals(__LINE__, i||j<<k, 1);
	iequals(__LINE__, i?j<<k:0, 4);
	iequals(__LINE__, i=j<<k, 4);
	iequals(__LINE__, i*=j<<k, 16);
	iequals(__LINE__, i/=j<<k, 4);
	iequals(__LINE__, i%=j<<k, 0);
	iequals(__LINE__, i+=j<<k, 4);
	iequals(__LINE__, i-=j<<k, 0);
	i = 3;
	iequals(__LINE__, i<<=j<<k, 48);
	iequals(__LINE__, i>>=j<<k, 3);
	iequals(__LINE__, i&=j<<k, 0);
	iequals(__LINE__, i^=j<<k, 4);
	iequals(__LINE__, i|=j<<k, 4);
	iequals(__LINE__, (i,j<<k), 4);
	}

/* Z op X>Y */
static void p2_24()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i>j>k, 0);
	iequals(__LINE__, i<j>k, 0);
	iequals(__LINE__, i<=j>k, 0);
	iequals(__LINE__, i>=j>k, 0);
	iequals(__LINE__, i==j>k, 0);
	iequals(__LINE__, i!=j>k, 1);
	iequals(__LINE__, i&j>k, 1);
	iequals(__LINE__, i^j>k, 6);
	iequals(__LINE__, i|j>k, 7);
	iequals(__LINE__, i&&j>k, 1);
	iequals(__LINE__, i||j>k, 1);
	iequals(__LINE__, i?j>k:0, 1);
	iequals(__LINE__, i=j>k, 1);
	iequals(__LINE__, i*=j>k, 1);
	iequals(__LINE__, i/=j>k, 1);
	iequals(__LINE__, i%=j>k, 0);
	iequals(__LINE__, i+=j>k, 1);
	iequals(__LINE__, i-=j>k, 0);
	i = 3;
	iequals(__LINE__, i<<=j>k, 6);
	iequals(__LINE__, i>>=j>k, 3);
	iequals(__LINE__, i&=j>k, 1);
	iequals(__LINE__, i^=j>k, 0);
	iequals(__LINE__, i|=j>k, 1);
	iequals(__LINE__, (i,j>k), 1);
	}

/* Z op X<Y */
static void p2_25()
	{
	int i = 7;
	int j = 2;
	int k = 4;
	iequals(__LINE__, i-j<k, 0);
	iequals(__LINE__, i>j<k, 1);
	iequals(__LINE__, i<j<k, 1);
	iequals(__LINE__, i<=j<k, 1);
	iequals(__LINE__, i>=j<k, 1);
	iequals(__LINE__, i==j<k, 0);
	iequals(__LINE__, i!=j<k, 1);
	iequals(__LINE__, i&j<k, 1);
	iequals(__LINE__, i^j<k, 6);
	iequals(__LINE__, i|j<k, 7);
	iequals(__LINE__, i&&j<k, 1);
	iequals(__LINE__, i||j<k, 1);
	iequals(__LINE__, i?j<k:0, 1);
	iequals(__LINE__, i=j<k, 1);
	iequals(__LINE__, i*=j<k, 1);
	iequals(__LINE__, i/=j<k, 1);
	iequals(__LINE__, i%=j<k, 0);
	iequals(__LINE__, i+=j<k, 1);
	iequals(__LINE__, i-=j<k, 0);
	i = 3;
	iequals(__LINE__, i<<=j<k, 6);
	iequals(__LINE__, i>>=j<k, 3);
	iequals(__LINE__, i&=j<k, 1);
	iequals(__LINE__, i^=j<k, 0);
	iequals(__LINE__, i|=j<k, 1);
	iequals(__LINE__, (i,j<k), 1);
	}

/* Z op X>=Y */
static void p2_26()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i<=j>=k, 0);
	iequals(__LINE__, i>=j>=k, 0);
	iequals(__LINE__, i==j>=k, 0);
	iequals(__LINE__, i!=j>=k, 1);
	iequals(__LINE__, i&j>=k, 1);
	iequals(__LINE__, i^j>=k, 6);
	iequals(__LINE__, i|j>=k, 7);
	iequals(__LINE__, i&&j>=k, 1);
	iequals(__LINE__, i||j>=k, 1);
	iequals(__LINE__, i?j>=k:0, 1);
	iequals(__LINE__, i=j>=k, 1);
	iequals(__LINE__, i*=j>=k, 1);
	iequals(__LINE__, i/=j>=k, 1);
	iequals(__LINE__, i%=j>=k, 0);
	iequals(__LINE__, i+=j>=k, 1);
	iequals(__LINE__, i-=j>=k, 0);
	i = 3;
	iequals(__LINE__, i<<=j>=k, 6);
	iequals(__LINE__, i>>=j>=k, 3);
	iequals(__LINE__, i&=j>=k, 1);
	iequals(__LINE__, i^=j>=k, 0);
	iequals(__LINE__, i|=j>=k, 1);
	iequals(__LINE__, (i,j>=k), 1);
	}

/* Z op X<=Y */
static void p2_27()
	{
	int i = 7;
	int j = 4;
	int k = 4;
	iequals(__LINE__, i>=j<=k, 1);
	iequals(__LINE__, i==j<=k, 0);
	iequals(__LINE__, i!=j<=k, 1);
	iequals(__LINE__, i&j<=k, 1);
	iequals(__LINE__, i^j<=k, 6);
	iequals(__LINE__, i|j<=k, 7);
	iequals(__LINE__, i&&j<=k, 1);
	iequals(__LINE__, i||j<=k, 1);
	iequals(__LINE__, i?j<=k:0, 1);
	iequals(__LINE__, i=j<=k, 1);
	iequals(__LINE__, i*=j<=k, 1);
	iequals(__LINE__, i/=j<=k, 1);
	iequals(__LINE__, i%=j<=k, 0);
	iequals(__LINE__, i+=j<=k, 1);
	iequals(__LINE__, i-=j<=k, 0);
	i = 3;
	iequals(__LINE__, i<<=j<=k, 6);
	iequals(__LINE__, i>>=j<=k, 3);
	iequals(__LINE__, i&=j<=k, 1);
	iequals(__LINE__, i^=j<=k, 0);
	iequals(__LINE__, i|=j<=k, 1);
	iequals(__LINE__, (i,j<=k), 1);
	}

/* Z op X==Y */
static void p2_28()
	{
	int i = 7;
	int j = 4;
	int k = 4;
	iequals(__LINE__, i==j==k, 0);
	iequals(__LINE__, i!=j==k, 0);
	iequals(__LINE__, i&j==k, 1);
	iequals(__LINE__, i^j==k, 6);
	iequals(__LINE__, i|j==k, 7);
	iequals(__LINE__, i&&j==k, 1);
	iequals(__LINE__, i||j==k, 1);
	iequals(__LINE__, i?j==k:0, 1);
	iequals(__LINE__, i=j==k, 1);
	iequals(__LINE__, i*=j==k, 1);
	iequals(__LINE__, i/=j==k, 1);
	iequals(__LINE__, i%=j==k, 0);
	iequals(__LINE__, i+=j==k, 1);
	iequals(__LINE__, i-=j==k, 0);
	i = 3;
	iequals(__LINE__, i<<=j==k, 6);
	iequals(__LINE__, i>>=j==k, 3);
	iequals(__LINE__, i&=j==k, 1);
	iequals(__LINE__, i^=j==k, 0);
	iequals(__LINE__, i|=j==k, 1);
	iequals(__LINE__, (i,j==k), 1);
	}

/* Z op X!=Y */
static void p2_29()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i!=j!=k, 1);
	iequals(__LINE__, i&j!=k, 1);
	iequals(__LINE__, i^j!=k, 6);
	iequals(__LINE__, i|j!=k, 7);
	iequals(__LINE__, i&&j!=k, 1);
	iequals(__LINE__, i||j!=k, 1);
	iequals(__LINE__, i?j!=k:0, 1);
	iequals(__LINE__, i=j!=k, 1);
	iequals(__LINE__, i*=j!=k, 1);
	iequals(__LINE__, i/=j!=k, 1);
	iequals(__LINE__, i%=j!=k, 0);
	iequals(__LINE__, i+=j!=k, 1);
	iequals(__LINE__, i-=j!=k, 0);
	i = 3;
	iequals(__LINE__, i<<=j!=k, 6);
	iequals(__LINE__, i>>=j!=k, 3);
	iequals(__LINE__, i&=j!=k, 1);
	iequals(__LINE__, i^=j!=k, 0);
	iequals(__LINE__, i|=j!=k, 1);
	iequals(__LINE__, (i,j!=k), 1);
	}

/* Z op X&Y */
static void p2_30()
	{
	int i = 7;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i&j&k, 2);
	iequals(__LINE__, i^j&k, 5);
	iequals(__LINE__, i|j&k, 7);
	iequals(__LINE__, i&&j&k, 1);
	iequals(__LINE__, i||j&k, 1);
	iequals(__LINE__, i?j&k:0, 2);
	iequals(__LINE__, i=j&k, 2);
	iequals(__LINE__, i*=j&k, 4);
	iequals(__LINE__, i/=j&k, 2);
	iequals(__LINE__, i%=j&k, 0);
	iequals(__LINE__, i+=j&k, 2);
	iequals(__LINE__, i-=j&k, 0);
	i = 3;
	iequals(__LINE__, i<<=j&k, 12);
	iequals(__LINE__, i>>=j&k, 3);
	iequals(__LINE__, i&=j&k, 2);
	iequals(__LINE__, i^=j&k, 0);
	iequals(__LINE__, i|=j&k, 2);
	iequals(__LINE__, (i,j&k), 2);
	}

/* Z op X^Y */
static void p2_31()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i^j^k, 1);
	iequals(__LINE__, i|j^k, 7);
	iequals(__LINE__, i&&j^k, 1);
	iequals(__LINE__, i||j^k, 1);
	iequals(__LINE__, i?j^k:0, 6);
	iequals(__LINE__, i=j^k, 6);
	iequals(__LINE__, i*=j^k, 36);
	iequals(__LINE__, i/=j^k, 6);
	iequals(__LINE__, i%=j^k, 0);
	iequals(__LINE__, i+=j^k, 6);
	iequals(__LINE__, i-=j^k, 0);
	iequals(__LINE__, i<<=j^k, 0);
	iequals(__LINE__, i>>=j^k, 0);
	i = 3;
	iequals(__LINE__, i&=j^k, 2);
	iequals(__LINE__, i^=j^k, 4);
	iequals(__LINE__, i|=j^k, 6);
	iequals(__LINE__, (i,j^k), 6);
	}

/* Z op X|Y */
static void p2_32()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i|j|k, 7);
	iequals(__LINE__, i&&j|k, 1);
	iequals(__LINE__, i||j|k, 1);
	iequals(__LINE__, i?j|k:0, 6);
	iequals(__LINE__, i=j|k, 6);
	iequals(__LINE__, i*=j|k, 36);
	iequals(__LINE__, i/=j|k, 6);
	iequals(__LINE__, i%=j|k, 0);
	iequals(__LINE__, i+=j|k, 6);
	iequals(__LINE__, i-=j|k, 0);
	i = 3;
	iequals(__LINE__, i<<=j|k, 192);
	iequals(__LINE__, i>>=j|k, 3);
	iequals(__LINE__, i&=j|k, 2);
	iequals(__LINE__, i^=j|k, 4);
	iequals(__LINE__, i|=j|k, 6);
	iequals(__LINE__, (i,j|k), 6);
	}

/* Z op X&&Y */
static void p2_33()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i&&j&&k, 1);
	iequals(__LINE__, i||j&&k, 1);
	iequals(__LINE__, i?j&&k:0, 1);
	iequals(__LINE__, i=j&&k, 1);
	iequals(__LINE__, i*=j&&k, 1);
	iequals(__LINE__, i/=j&&k, 1);
	iequals(__LINE__, i%=j&&k, 0);
	iequals(__LINE__, i+=j&&k, 1);
	iequals(__LINE__, i-=j&&k, 0);
	i = 3;
	iequals(__LINE__, i<<=j&&k, 6);
	iequals(__LINE__, i>>=j&&k, 3);
	iequals(__LINE__, i&=j&&k, 1);
	iequals(__LINE__, i^=j&&k, 0);
	iequals(__LINE__, i|=j&&k, 1);
	iequals(__LINE__, (i,j&&k), 1);
	}

/* Z op X||Y */
static void p2_34()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i||j||k, 1);
	iequals(__LINE__, i?j||k:0, 1);
	iequals(__LINE__, i=j||k, 1);
	iequals(__LINE__, i*=j||k, 1);
	iequals(__LINE__, i/=j||k, 1);
	iequals(__LINE__, i%=j||k, 0);
	iequals(__LINE__, i+=j||k, 1);
	iequals(__LINE__, i-=j||k, 0);
	i = 3;
	iequals(__LINE__, i<<=j||k, 6);
	iequals(__LINE__, i>>=j||k, 3);
	iequals(__LINE__, i&=j||k, 1);
	iequals(__LINE__, i^=j||k, 0);
	iequals(__LINE__, i|=j||k, 1);
	iequals(__LINE__, (i,j||k), 1);
	}

/* Z op X?Y */
static void p2_35()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	int l = 0;
	iequals(__LINE__, i=j?k:l, 2);
	iequals(__LINE__, i*=j?k:l, 4);
	iequals(__LINE__, i/=j?k:l, 2);
	iequals(__LINE__, i%=j?k:l, 0);
	iequals(__LINE__, i+=j?k:l, 2);
	iequals(__LINE__, i-=j?k:l, 0);
	i = 3;
	iequals(__LINE__, i<<=j?k:l, 12);
	iequals(__LINE__, i>>=j?k:l, 3);
	iequals(__LINE__, i&=j?k:l, 2);
	iequals(__LINE__, i^=j?k:l, 0);
	iequals(__LINE__, i|=j?k:l, 2);
	iequals(__LINE__, (i,j?k:l), 2);
	}

