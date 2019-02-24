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
 * PREC1C -- try all operator pairs -- this file uses all binary operators
 * in the first position (see prec 2 for the other way around).
 */

static void p1_36();
static void p1_37();
static void p1_38();
static void p1_39();
static void p1_40();
static void p1_41();
static void p1_42();
static void p1_43();
static void p1_44();
static void p1_45();
static void p1_46();
static void p1_47();


void prec1c()
	{
	Filename = "prec1c.c";
	p1_36();	/* = */
	p1_37();	/* *= */
	p1_38();	/* /= */
	p1_39();	/* %= */
	p1_40();	/* += */
	p1_41();	/* -= */
	p1_42();	/* <<= */
	p1_43();	/* >>= */
	p1_44();	/* &= */
	p1_45();	/* ^= */
	p1_46();	/* |= */
	p1_47();	/* , */
	}


/*  X*=YopZ */
static void p1_36()
	{
	int i = 1;
	int j = 7;
	int k = 4;
	iequals(__LINE__, i=j*=k, 28);
	iequals(__LINE__, i, 28);
	iequals(__LINE__, j, 28);
	i = 1;
	j = 7;
	iequals(__LINE__, i=j*=k, 28);
	iequals(__LINE__, i, 28);
	iequals(__LINE__, j, 28);
	iequals(__LINE__, i=j/=k, 7);
	iequals(__LINE__, i, 7);
	iequals(__LINE__, j, 7);
	iequals(__LINE__, i=j%=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	iequals(__LINE__, i=j+=k, 7);
	iequals(__LINE__, i, 7);
	iequals(__LINE__, j, 7);
	iequals(__LINE__, i=j-=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	k = 1;
	iequals(__LINE__, i=j<<=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 6);
	iequals(__LINE__, i=j>>=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	k = 3;
	iequals(__LINE__, i=j&=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	j = 0;
	iequals(__LINE__, i=j^=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	k = 3;
	iequals(__LINE__, i=j|=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	iequals(__LINE__, (i=j,k), 3);
	iequals(__LINE__, i, 3);
	}

/*  X*=YopZ */
static void p1_37()
	{
	int i = 1;
	int j = 7;
	int k = 4;
	iequals(__LINE__, i*=j*=k, 28);
	iequals(__LINE__, i, 28);
	iequals(__LINE__, j, 28);
	iequals(__LINE__, i*=j/=k, 196);
	iequals(__LINE__, i, 196);
	iequals(__LINE__, j, 7);
	iequals(__LINE__, i*=j%=k, 588);
	iequals(__LINE__, i, 588);
	iequals(__LINE__, j, 3);
	iequals(__LINE__, i*=j+=k, 4116);
	iequals(__LINE__, i, 4116);
	iequals(__LINE__, j, 7);
	iequals(__LINE__, i*=j-=k, 12348);
	iequals(__LINE__, i, 12348);
	iequals(__LINE__, j, 3);
	i = 11;
	k = 1;
	iequals(__LINE__, i*=j<<=k, 66);
	iequals(__LINE__, i, 66);
	iequals(__LINE__, j, 6);
	i = 11;
	iequals(__LINE__, i*=j>>=k, 33);
	iequals(__LINE__, i, 33);
	iequals(__LINE__, j, 3);
	i = 11;
	k = 3;
	iequals(__LINE__, i*=j&=k, 33);
	iequals(__LINE__, i, 33);
	iequals(__LINE__, j, 3);
	i = 11;
	j = 0;
	iequals(__LINE__, i*=j^=k, 33);
	iequals(__LINE__, i, 33);
	iequals(__LINE__, j, 3);
	i = 11;
	k = 3;
	iequals(__LINE__, i*=j|=k, 33);
	iequals(__LINE__, i, 33);
	iequals(__LINE__, j, 3);
	i = 11;
	iequals(__LINE__, (i*=j,k), 3);
	iequals(__LINE__, i, 33);
	}

/*  X/=YopZ */
static void p1_38()
	{
	int i = 11;
	int j = 8;
	int k = 4;
	iequals(__LINE__, i/=j/=k, 5);
	iequals(__LINE__, i, 5);
	iequals(__LINE__, j, 2);
	i = 11;
	j = 7;
	iequals(__LINE__, i/=j%=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	i = 11;
	iequals(__LINE__, i/=j-=k, -11);
	iequals(__LINE__, i, -11);
	iequals(__LINE__, j, -1);
	i = 11;
	k = 1;
	j = 3;
	iequals(__LINE__, i/=j<<=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 6);
	i = 11;
	iequals(__LINE__, i/=j>>=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	i = 11;
	k = 3;
	iequals(__LINE__, i/=j&=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	i = 11;
	j = 0;
	iequals(__LINE__, i/=j^=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	i = 11;
	k = 3;
	iequals(__LINE__, i/=j|=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	i = 11;
	iequals(__LINE__, (i/=j,k), 3);
	iequals(__LINE__, i, 3);
	}

/*  X%=YopZ */
static void p1_39()
	{
	int i = 11;
	int j = 7;
	int k = 4;
	iequals(__LINE__, i%=j%=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 3);
	i = 11;
	iequals(__LINE__, i%=j+=k, 4);
	iequals(__LINE__, i, 4);
	iequals(__LINE__, j, 7);
	i = 11;
	iequals(__LINE__, i%=j-=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 3);
	i = 11;
	k = 1;
	iequals(__LINE__, i%=j<<=k, 5);
	iequals(__LINE__, i, 5);
	iequals(__LINE__, j, 6);
	i = 11;
	iequals(__LINE__, i%=j>>=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 3);
	i = 11;
	k = 3;
	iequals(__LINE__, i%=j&=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 3);
	i = 11;
	j = 0;
	iequals(__LINE__, i%=j^=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 3);
	i = 11;
	k = 3;
	iequals(__LINE__, i%=j|=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 3);
	i = 11;
	iequals(__LINE__, (i%=j,k), 3);
	iequals(__LINE__, i, 2);
	}

/*  X+=YopZ */
static void p1_40()
	{
	int i = -6;
	int j = 3;
	int k = 4;
	iequals(__LINE__, i+=j+=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 7);
	iequals(__LINE__, i+=j-=k, 4);
	iequals(__LINE__, i, 4);
	iequals(__LINE__, j, 3);
	k = 1;
	iequals(__LINE__, i+=j<<=k, 10);
	iequals(__LINE__, i, 10);
	iequals(__LINE__, j, 6);
	iequals(__LINE__, i+=j>>=k, 13);
	iequals(__LINE__, i, 13);
	iequals(__LINE__, j, 3);
	k = 3;
	iequals(__LINE__, i+=j&=k, 16);
	iequals(__LINE__, i, 16);
	iequals(__LINE__, j, 3);
	iequals(__LINE__, i+=j^=k, 16);
	iequals(__LINE__, i, 16);
	iequals(__LINE__, j, 0);
	k = 3;
	iequals(__LINE__, i+=j|=k, 19);
	iequals(__LINE__, i, 19);
	iequals(__LINE__, j, 3);
	iequals(__LINE__, (i+=j,k), k);
	iequals(__LINE__, i, 22);
	}

/*  X-=YopZ */
static void p1_41()
	{
	int i = 21;
	int j = 7;
	int k = 4;
	iequals(__LINE__, i-=j-=k, 18);
	iequals(__LINE__, i, 18);
	iequals(__LINE__, j, 3);
	k = 1;
	iequals(__LINE__, i-=j<<=k, 12);
	iequals(__LINE__, i, 12);
	iequals(__LINE__, j, 6);
	iequals(__LINE__, i-=j>>=k, 9);
	iequals(__LINE__, i, 9);
	iequals(__LINE__, j, 3);
	k = 3;
	iequals(__LINE__, i-=j&=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 3);
	iequals(__LINE__, i-=j^=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 0);
	k = 3;
	iequals(__LINE__, i-=j|=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 3);
	iequals(__LINE__, (i-=j,k), k);
	iequals(__LINE__, i, 0);
	}

/*  X<<=YopZ */
static void p1_42()
	{
	int i = 1;
	int j = 2;
	int k = 1;
	iequals(__LINE__, i<<=j<<=k, 16);
	iequals(__LINE__, i, 16);
	iequals(__LINE__, j, 4);
	j = 2;
	iequals(__LINE__, i<<=j>>=k, 32);
	iequals(__LINE__, i, 32);
	iequals(__LINE__, j, 1);
	iequals(__LINE__, i<<=j&=k, 64);
	iequals(__LINE__, i, 64);
	iequals(__LINE__, j, 1);
	j = 0;
	iequals(__LINE__, i<<=j^=k, 128);
	iequals(__LINE__, i, 128);
	iequals(__LINE__, j, 1);
	iequals(__LINE__, i<<=j|=k, 256);
	iequals(__LINE__, i, 256);
	iequals(__LINE__, j, 1);
	iequals(__LINE__, (i<<=j,k), k);
	iequals(__LINE__, i, 512);
	}

/*  X>>=YopZ */
static void p1_43()
	{
	int i = 9;
	int j = 3;
	int k = 1;
	iequals(__LINE__, i>>=j>>=k, 4);
	iequals(__LINE__, i, 4);
	iequals(__LINE__, j, 1);
	k = 7;
	iequals(__LINE__, i>>=j&=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 1);
	i = 0x100;
	iequals(__LINE__, i>>=j^=k, 4);
	iequals(__LINE__, i, 4);
	iequals(__LINE__, j, 6);
	j = 0; k = 2;
	iequals(__LINE__, i>>=j|=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 2);
	i = 7;
	iequals(__LINE__, (i>>=j,k), k);
	iequals(__LINE__, i, 1);
	}

/*  X&=YopZ */
static void p1_44()
	{
	int i = 7;
	int j = 2;
	int k = 3;
	iequals(__LINE__, i&=j&=k, 2);
	iequals(__LINE__, i, 2);
	iequals(__LINE__, j, 2);
	i = 7;
	iequals(__LINE__, i&=j^=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 1);
	k = 4;
	iequals(__LINE__, i&=j|=k, 1);
	iequals(__LINE__, i, 1);
	iequals(__LINE__, j, 5);
	i = 7;
	iequals(__LINE__, (i&=j,k), k);
	iequals(__LINE__, i, 5);
	}

/*  X^=YopZ */
static void p1_45()
	{
	int i = 2;
	int j = 2;
	int k = 3;
	iequals(__LINE__, i^=j^=k, 3);
	iequals(__LINE__, i, 3);
	iequals(__LINE__, j, 1);
	k = 4;
	iequals(__LINE__, i^=j|=k, 6);
	iequals(__LINE__, i, 6);
	iequals(__LINE__, j, 5);
	iequals(__LINE__, (i^=j,k), 4);
	iequals(__LINE__, i, 3);
	}

/*  X|=YopZ */
static void p1_46()
	{
	int i = 1;
	int j = 2;
	int k = 4;
	iequals(__LINE__, i|=j|=k, 7);
	iequals(__LINE__, i, 7);
	iequals(__LINE__, j, 6);
	iequals(__LINE__, (i|=j,k), 4);
	iequals(__LINE__, i, 7);
	}

/*  X,YopZ */
static void p1_47()
	{
	int i = 1;
	int j = 2;
	int k = 3;
	iequals(__LINE__, (i,j,k), k);
	}
