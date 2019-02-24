/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


/*
 * PREC2C - 
 */
#include "defs.h"

static void p2_36();
static void p2_37();
static void p2_38();
static void p2_39();
static void p2_40();
static void p2_41();
static void p2_42();
static void p2_43();
static void p2_44();
static void p2_45();
static void p2_46();

void prec2c()
	{
	Filename = "prec2c.c";
	p2_36();	/* = */
	p2_37();	/* *= */
	p2_38();	/* /= */
	p2_39();	/* %= */
	p2_40();	/* += */
	p2_41();	/* -= */
	p2_42();	/* <<= */
	p2_43();	/* >>= */
	p2_44();	/* &= */
	p2_45();	/* ^= */
	p2_46();	/* |= */
	}

/* Z op X=Y */
static void p2_36()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i=j=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, i*=j=k, 4);
	iequals(__LINE__, i, 4);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, i/=j=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, i%=j=k, 0);
	iequals(__LINE__, i, 0);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, i+=j=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, i-=j=k, 0);
	iequals(__LINE__, i, 0);
	iequals(__LINE__, j, 2);
	i = 3;
	j = 4;
	iequals(__LINE__, i<<=j=k, 12);
	iequals(__LINE__, i, 12);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, i>>=j=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, i&=j=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, i^=j=k, 0);
	iequals(__LINE__, i, 0);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, i|=j=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 2);
	j = 4;
	iequals(__LINE__, (i,j=k), 2);	/* TP2 */
	iequals(__LINE__, j, 2);	/* TP2 */
	}

/* X op Y *= Z */
static void p2_37()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i/=j*=k, 0);
	iequals(__LINE__, i, 0);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, i%=j*=k, 7);
	iequals(__LINE__, i, 7);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, i+=j*=k, 15);
	iequals(__LINE__, i, 15);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, i-=j*=k, -1);
	iequals(__LINE__, i, -1);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, i<<=j*=k, 1792);
	iequals(__LINE__, i, 1792);
	iequals(__LINE__, j, 8);
	j=4;
	iequals(__LINE__, i>>=j*=k, 7);
	iequals(__LINE__, i, 7);
	iequals(__LINE__, j, 8);
	i=7,j=3;
	iequals(__LINE__, i&=j*=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 6);
	i=7,j=4;
	iequals(__LINE__, i^=j*=k, 15);
	iequals(__LINE__, i, 15);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, i|=j*=k, 15);
	iequals(__LINE__, i, 15);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, (i,j*=k), 8);	/* TP2 */
	iequals(__LINE__, j, 8);	/* TP2 */
	}

/* X op Y /= Z */
static void p2_38()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i%=j/=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, i+=j/=k, 9);
	iequals(__LINE__, i, 9);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, i-=j/=k, 5);
	iequals(__LINE__, i, 5);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, i<<=j/=k, 28);
	iequals(__LINE__, i, 28);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, i>>=j/=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, i&=j/=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, i^=j/=k, 5);
	iequals(__LINE__, i, 5);
	iequals(__LINE__, j, 2);
	i=1,j=4;
	iequals(__LINE__, i|=j/=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, (i,j/=k), 2);	/* TP2 */
	iequals(__LINE__, j, 2);	/* TP2 */
	}

/* X op Y %= Z */
static void p2_39()
	{
	int i = 7;
	int j = 4;
	int k = 3;
	iequals(__LINE__, i+=j%=k, 8);
	iequals(__LINE__, i, 8);
	iequals(__LINE__, j, 1);
	i=7,j=4;
	iequals(__LINE__, i-=j%=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 1);
	i=7,j=4;
	iequals(__LINE__, i<<=j%=k, 14);
	iequals(__LINE__, i, 14);
	iequals(__LINE__, j, 1);
	i=7,j=4;
	iequals(__LINE__, i>>=j%=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 1);
	i=7,j=4;
	iequals(__LINE__, i&=j%=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 1);
	i=7,j=4;
	iequals(__LINE__, i^=j%=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 1);
	i=6,j=4;
	iequals(__LINE__, i|=j%=k, 7);
	iequals(__LINE__, i, 7);
	iequals(__LINE__, j, 1);
	i=7,j=4;
	iequals(__LINE__, (i,j%=k), 1);	/* TP2 */
	iequals(__LINE__, j, 1);	/* TP2 */
	}

/* X op Y += Z */
static void p2_40()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i-=j+=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 6);
	i=7,j=4;
	iequals(__LINE__, i<<=j+=k, 448);
	iequals(__LINE__, i, 448);
	iequals(__LINE__, j, 6);
	iequals(__LINE__, i>>=j+=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, i&=j+=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 6);
	i=7,j=4;
	iequals(__LINE__, i^=j+=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 6);
	i=7,j=4;
	iequals(__LINE__, i|=j+=k, 7);
	iequals(__LINE__, i, 7);
	iequals(__LINE__, j, 6);
	i=7,j=4;
	iequals(__LINE__, (i,j+=k), 6);	/* TP2 */
	iequals(__LINE__, j, 6);	/* TP2 */
	}

/* X op Y -= Z */
static void p2_41()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i<<=j-=k, 28);
	iequals(__LINE__, i, 28);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, i>>=j-=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, i&=j-=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, i^=j-=k, 5);
	iequals(__LINE__, i, 5);
	iequals(__LINE__, j, 2);
	i=2,j=4;
	iequals(__LINE__, i|=j-=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 2);
	i=7,j=4;
	iequals(__LINE__, (i,j-=k), 2);	/* TP2 */
	iequals(__LINE__, j, 2);	/* TP2 */
	}

/* X op Y <<= Z */
static void p2_42()
	{
	int i = 256;
	int j = 4;
	int k = 1;
	iequals(__LINE__, i>>=j<<=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, i&=j<<=k, 0);
	iequals(__LINE__, i, 0);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, i^=j<<=k, 15);
	iequals(__LINE__, i, 15);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, i|=j<<=k, 15);
	iequals(__LINE__, i, 15);
	iequals(__LINE__, j, 8);
	i=7,j=4;
	iequals(__LINE__, (i,j<<=k), 8);	/* TP2 */
	iequals(__LINE__, j, 8);	/* TP2 */
	}

/* X op Y >>= Z */
static void p2_43()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i&=j>>=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 1);
	i=7,j=4;
	iequals(__LINE__, i^=j>>=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 1);
	i=4,j=4;
	iequals(__LINE__, i|=j>>=k, 5);
	iequals(__LINE__, i, 5);
	iequals(__LINE__, j, 1);
	i=7,j=4;
	iequals(__LINE__, (i,j>>=k), 1);	/* TP2 */
	iequals(__LINE__, j, 1);	/* TP2 */
	}

/* X op Y &= Z */
static void p2_44()
	{
	int i = 7;
	int j = 3;
	int k = 2;
	iequals(__LINE__, i^=j&=k, 5);
	iequals(__LINE__, i, 5);
	iequals(__LINE__, j, 2);
	i=4,j=3;
	iequals(__LINE__, i|=j&=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 2);
	i=7,j=3;
	iequals(__LINE__, (i,j&=k), 2);	/* TP2 */
	iequals(__LINE__, j, 2);	/* TP2 */
	}

/* X op Y ^= Z */
static void p2_45()
	{
	int i = 8;
	int j = 4;
	int k = 2;
	iequals(__LINE__, i|=j^=k, 14);
	iequals(__LINE__, i, 14);
	iequals(__LINE__, j, 6);
	iequals(__LINE__, (i,j^=k), 4);	/* TP2 */
	iequals(__LINE__, j, 4);	/* TP2 */
	}

/* X op Y |= Z */
static void p2_46()
	{
	int i = 7;
	int j = 4;
	int k = 2;
	iequals(__LINE__, (i,j|=k), 6);	/* TP2 */
	iequals(__LINE__, j, 6);	/* TP2 */
	}
