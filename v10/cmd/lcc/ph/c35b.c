/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/*
 * BITFIELDS - make sure that all operators work with bitfields.
 * Called at or about line 48 in c3_5.
 */

#include "flags.h"
#ifndef SKIP35B
#include "defs.h"

static int SideEffect = 0;
struct fields
	{
	unsigned x1:1;
	unsigned x2:2;
	unsigned x3:3;
	unsigned   :1;		/* just because it can be done */
	unsigned x4:4;
	};

void bitfields()
	{
	struct fields b, b2, *p = &b2, *bf();

	Filename = "c35b.c";
	b.x2 = 2;
	p->x3 = 7;

	iequals(__LINE__, b.x2++, 2);
	iequals(__LINE__, b.x2--, 3);
	iequals(__LINE__, ++b.x2, 3);
	iequals(__LINE__, --b.x2, 2);
	/* &bitfield is illegal */
#if ANSI
	iequals(__LINE__, +b.x2, 2);
#endif
	iequals(__LINE__, -b.x2, -2);
	iequals(__LINE__, ~b.x2, ~2);
	iequals(__LINE__, !b.x2, 0);
	dequals(__LINE__, (double)b.x2, 2.0);
	iequals(__LINE__, b.x2*p->x3, 14);
	iequals(__LINE__, p->x3/b.x2, 3);


	iequals(__LINE__, p->x3%b.x2, 1);
	iequals(__LINE__, p->x3+b.x2, 9);
	iequals(__LINE__, p->x3-b.x2, 5);
	iequals(__LINE__, p->x3>>b.x2, 7>>2);
	iequals(__LINE__, p->x3<<b.x2, 7<<2);
	iequals(__LINE__, p->x3>b.x2, 1);
	iequals(__LINE__, p->x3<b.x2, 0);
	iequals(__LINE__, p->x3>=b.x2, 1);
	iequals(__LINE__, p->x3<=b.x2, 0);
	iequals(__LINE__, p->x3==b.x2, 0);
	iequals(__LINE__, p->x3!=b.x2, 1);
	iequals(__LINE__, p->x3&b.x2, 2);
	iequals(__LINE__, p->x3^b.x2, 5);
	iequals(__LINE__, p->x3|b.x2, 7);
	iequals(__LINE__, p->x3&&b.x2, 1);
	iequals(__LINE__, p->x3||b.x2, 1);
	iequals(__LINE__, p->x3?b.x2:0, 2);

	/* the first two are tricky -- make sure that the expression value is
	 * the same as the left hand side after assignment (wraparound occurs).
	 */
	iequals(__LINE__, p->x3=15, 7);
	iequals(__LINE__, p->x3*=b.x2, 6);
	iequals(__LINE__, p->x3/=b.x2, 3);
	iequals(__LINE__, p->x3%=b.x2, 1);
	iequals(__LINE__, p->x3+=b.x2, 3);
	iequals(__LINE__, p->x3-=b.x2, 1);
	iequals(__LINE__, p->x3<<=b.x2, 4);
	iequals(__LINE__, p->x3>>=b.x2, 1);
	b.x2 = 3;
	iequals(__LINE__, p->x3&=b.x2, 1);
	iequals(__LINE__, p->x3^=b.x2, 2);
	iequals(__LINE__, p->x3|=b.x2, 3);
	iequals(__LINE__, (p->x3,b.x2), 3);

	/* check for incorrect side effects */
	bf()->x3 = 3;
	bf()->x2 = 2;
	iequals(__LINE__, bf()->x3 *= bf()->x2, 6);
	iequals(__LINE__, bf()->x3 /= bf()->x2, 3);
	iequals(__LINE__, bf()->x3 %= bf()->x2, 1);
	iequals(__LINE__, bf()->x3 += (SideEffect++, bf())->x2, 3);
	iequals(__LINE__, bf()->x3 -= (SideEffect++, bf())->x2, 1);
	iequals(__LINE__, (SideEffect++, bf())->x3 <<= bf()->x2, 4);
	iequals(__LINE__, (SideEffect++, bf())->x3 >>= bf()->x2, 1);
	iequals(__LINE__, SideEffect, 20);
	}




struct fields *bf()
	{
	static struct fields f;
	++SideEffect;
	return(&f);
	}

#else /* if SKIP35B */
void bitfields() { pr_skip("bitfields (c35b): SKIPPED ENTIRELY\n"); }
#endif /* SKIP35B */

