/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#include "flags.h"
#ifndef SKIP34
/*
 * 3.4 - Constant expressions
 */

#include "defs.h"


#if ANSI
static int i1 = +1;
static int i4  = !4;
static int i13 = 1 && 3;
static int i14 = 0 || 3;
#else
static int i1 = 1;
static int i4  = 0;
static int i13 = 1;
static int i14 = 1;
#endif

static int i2  = -2;
static int i3  = ~3;
static int i5  = ((((((2*5) / 2) % 3) + 1) - 2) << 2) >> 2 ;
static int i6 = 4 < 5;
static int i7 = 4 > 5;
static int i8 = 4 <= 5;
static int i9 = 4 >= 5;
static int i10 = 4 == 5;
static int i11 = 4 != 5;
static int i12 = ((1 & 3) ^ 2) | 4; 
static int i15 = 1 ? 1 : 2;

static double d1 = 1.3;
static double d2 = (double)1;

static char array[] = "abcd";
static char *pc1 = array;
static char *pc2 = array + (1 + 3 * 2) / 7;
static char *pc3 = array - (-2 *(1 + 3 * 2)) / 7;
static char *pc4 = &array[3];


static struct b {																											/* 3.4 (cont.) */
	int i;
	unsigned int a : (1 + 3 * 2) / 7;
	unsigned int b : (1 + 3 * 2) / 7;
	} b, *pb = &b;

#if ANSI8712
static int *pi1 = &b.i;
static int *pi2 = &(&b)->i;
static int *pi3 = (int *)&b;
static int i16 = 1 || 2/0;
#endif

#if (V7 || ANSI)
static enum { e2 = (4 + 3 * 2) / 5, e1 = (1 + 3 * 2) / 7} e; 
#endif

static char carray[4 + 3 * 2];

void c3_4()
	{
	int count, i;
	static void (*pf)() = c3_4;

	Filename = "c34.c";

	/* check out constant expressions from initializers */
#if ANSI
	iequals(__LINE__, i1, 1); 
#endif
	iequals(__LINE__, i2, -2); 
	iequals(__LINE__, i3, ~3); 
	iequals(__LINE__, i4, 0); 
	iequals(__LINE__, i5, 1); 
	iequals(__LINE__, i6, 1); 
	iequals(__LINE__, i7, 0); 
	iequals(__LINE__, i8, 1); 
	iequals(__LINE__, i9, 0); 
	iequals(__LINE__, i10, 0); 
	iequals(__LINE__, i11, 1); 
	iequals(__LINE__, i12, 7); 
	iequals(__LINE__, i13, 1); 
	iequals(__LINE__, i14, 1); 
	iequals(__LINE__, i15, 1); 

	iequals(__LINE__, *pc1, 'a');
	iequals(__LINE__, *pc2, 'b');
	iequals(__LINE__, *pc3, 'c');
	iequals(__LINE__, *pc4, 'd');

	dequals(__LINE__, d1, 1.3);																							/* 3.4 (cont.) */
	dequals(__LINE__, d2, 1.0);

#if ANSI8712
	iequals(__LINE__, *pi1, 0);
	iequals(__LINE__, *pi2, 0);
	iequals(__LINE__, *pi3, 0);
	iequals(__LINE__, i16, 1);
#endif

	checkthat(__LINE__, pf == c3_4);


	/* check out constant expressions contexts : bitfields, enums, sizeof array, case. */
	b.b = 2;
	iequals(__LINE__, b.b, 0);
#if (V7 || ANSI)
	iequals(__LINE__, e2, 2);
	iequals(__LINE__, e1, 1);
#endif
	iequals(__LINE__, sizeof(carray), 10);

	count = -1;
	for (i = 0; i < 4; ++i)
		switch (i)
			{
			case (6*2) - (3 * 4):
				count = 0;
				break;
			case 15 % 14:
				count += 1;
				break;
			case 1<<1:
				count += 2;
				break;
			case 0?17:3:
				count += 3;
				break;
			default :
				count = -1;
				break;
			}
	iequals(__LINE__, count, 6);
	}

#else /* if SKIP34 */

void c3_4() { pr_skip("c3_4: SKIPPED ENTIRELY\n"); }
#endif /* SKIP34 */

