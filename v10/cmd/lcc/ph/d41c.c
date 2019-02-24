/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#define LIB_TEST 1
#include "defs.h"
#if !ANSI || !HAS_PROTOTYPES
#define SKIP41 1
#endif

#include "flags.h"
#ifndef SKIP41
#include <float.h>
/*
 * 4.1.4 - Limits <float.h>
 */
void d41_4b()
	{
	Filename = "d41c.c";

	/* FLT_RADIX must be a restricted constant expression.
	 * All the others need not be constant expressions (ANSI8706).
	 */
#if FLT_RADIX < 2
	complain(__LINE__);
#endif

	checkthat( - __LINE__, FLT_ROUNDS >= -1);	/* other values of FLT_ROUNDS may be implem-def */
	{
		double d1, d2, d3;

		d1 = dvalue(1.);
		d2 = dvalue(2 * DBL_EPSILON / 3);
		d3 = d1 + d2;	/* should cause rounding into lowest-order digit */
		     if (FLT_ROUNDS == 0 /* toward zero    */ && d3  > d1) complain(__LINE__);
		else if (FLT_ROUNDS == 1 /* to nearest     */ && d3 <= d1) complain(__LINE__);
		else if (FLT_ROUNDS == 2 /* toward pos inf */ && d3 <= d1) complain(__LINE__);
		else if (FLT_ROUNDS == 3 /* to neg inf     */ && d3  > d1) complain(__LINE__);

		d1 = -dvalue(1.);
		d2 = -dvalue(2 * DBL_EPSILON / 3);
		d3 = d1 + d2;	/* should cause rounding into lowest-order digit */
		     if (FLT_ROUNDS == 0 /* toward zero    */ && d3  < d1) complain(__LINE__);
		else if (FLT_ROUNDS == 1 /* to nearest     */ && d3 >= d1) complain(__LINE__);
		else if (FLT_ROUNDS == 2 /* toward pos inf */ && d3  < d1) complain(__LINE__);
		else if (FLT_ROUNDS == 3 /* to neg inf     */ && d3 >= d1) complain(__LINE__);
	}

	checkthat(__LINE__, FLT_MANT_DIG > 0);
	checkthat(__LINE__, DBL_MANT_DIG > 0);
	checkthat(__LINE__, LDBL_MANT_DIG > 0);
	checkthat(__LINE__, FLT_DIG >= 6);
	checkthat(__LINE__, DBL_DIG >= (ANSI8703 ? 10 : 6));
	checkthat(__LINE__, LDBL_DIG >= (ANSI8703 ? 10 : 6));
	checkthat(__LINE__, FLT_MIN_EXP < 0);
	checkthat(__LINE__, DBL_MIN_EXP < 0);
	checkthat(__LINE__, LDBL_MIN_EXP < 0);
	checkthat(__LINE__, FLT_MIN_10_EXP <= -37);
	checkthat(__LINE__, DBL_MIN_10_EXP <= -37);
	checkthat(__LINE__, LDBL_MIN_10_EXP <= -37);
	checkthat(__LINE__, FLT_MAX_EXP > 0);
	checkthat(__LINE__, DBL_MAX_EXP > 0);
	checkthat(__LINE__, LDBL_MAX_EXP > 0);
	checkthat(__LINE__, FLT_MAX_10_EXP >= 37);
	checkthat(__LINE__, DBL_MAX_10_EXP >= 37);
	checkthat(__LINE__, LDBL_MAX_10_EXP >= 37);
	checkthat(__LINE__, FLT_MAX >= 1E+37);
	checkthat(__LINE__, DBL_MAX >= 1E+37);
	checkthat(__LINE__, LDBL_MAX >= 1E+37);
	checkthat(__LINE__, FLT_EPSILON <= 1E-5);
	checkthat(__LINE__, DBL_EPSILON <= (ANSI8703 ? 1E-9 : 1E-5));
	checkthat(__LINE__, LDBL_EPSILON <= (ANSI8703 ? 1E-9 : 1E-5));
	checkthat(__LINE__, FLT_MIN <= 1E-37);
	checkthat(__LINE__, DBL_MIN <= 1E-37);
	checkthat(__LINE__, LDBL_MIN <= 1E-37);

	/* check that all names are really macros */
#if !defined(FLT_MANT_DIG) || !defined(DBL_MANT_DIG) || !defined(LDBL_MANT_DIG)
	complain(__LINE__);
#endif
#if !defined(FLT_DIG) || !defined(DBL_DIG) || !defined(LDBL_DIG)
	complain(__LINE__);
#endif
#if !defined(FLT_MIN_EXP) || !defined(DBL_MIN_EXP) || !defined(LDBL_MIN_EXP)
	complain(__LINE__);
#endif
#if !defined(FLT_MIN_10_EXP) || !defined(DBL_MIN_10_EXP) || !defined(LDBL_MIN_10_EXP)
	complain(__LINE__);
#endif
#if !defined(FLT_MAX_EXP) || !defined(DBL_MAX_EXP) || !defined(LDBL_MAX_EXP)
	complain(__LINE__);
#endif
#if !defined(FLT_MAX_10_EXP) || !defined(DBL_MAX_10_EXP) || !defined(LDBL_MAX_10_EXP)
	complain(__LINE__);
#endif


#if !defined(FLT_MAX) || !defined(DBL_MAX) || !defined(LDBL_MAX)
	complain(__LINE__);
#endif
#if !defined(FLT_EPSILON) || !defined(DBL_EPSILON) || !defined(LDBL_EPSILON)
	complain(__LINE__);
#endif
#if !defined(FLT_MIN) || !defined(DBL_MIN) || !defined(LDBL_MIN)
	complain(__LINE__);
#endif
	}
#else
int skp41c = 1;	/* must have one def */
#endif	/* SKIP41 */
