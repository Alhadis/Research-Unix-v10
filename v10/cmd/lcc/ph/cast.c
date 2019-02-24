/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of the Suite corresponds to the official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

#define ANSI 1  /* This file follows ANSI arithmetic rules, which may not work in non-ANSI modes */
#include "types.h"
int main()
	{
	extern char *Filename;
	auto CHAR Cc = 7;
	auto CHAR *pCc = &Cc;
	auto CHAR **ppCc = &pCc;
#if ANSI
	auto SCHAR Csc = 8;
	auto SCHAR *pCsc = &Csc;
	auto SCHAR **ppCsc = &pCsc;
#endif
	auto SHORT Cs = 9;
	auto SHORT *pCs = &Cs;
	auto SHORT **ppCs = &pCs;
	auto INT Ci = 10;
	auto INT *pCi = &Ci;
	auto INT **ppCi = &pCi;
	auto UCHAR Cuc = 11;
	auto UCHAR *pCuc = &Cuc;
	auto UCHAR **ppCuc = &pCuc;
	auto USHORT Cus = 12;
	auto USHORT *pCus = &Cus;
	auto USHORT **ppCus = &pCus;
	auto UINT Cui = 13;
	auto UINT *pCui = &Cui;
	auto UINT **ppCui = &pCui;
	auto LONG Cl = 14;
	auto LONG *pCl = &Cl;
	auto LONG **ppCl = &pCl;
	auto ULONG Cul = 15;
	auto ULONG *pCul = &Cul;
	auto ULONG **ppCul = &pCul;
	auto FLOAT Cf = 16;
	auto FLOAT *pCf = &Cf;
	auto FLOAT **ppCf = &pCf;
	auto DOUBLE Cd = 17;
	auto DOUBLE *pCd = &Cd;
	auto DOUBLE **ppCd = &pCd;
#if ANSI
	auto LDOUBLE Cld = 18;
	auto LDOUBLE *pCld = &Cld;
	auto LDOUBLE **ppCld = &pCld;
#endif
	{
	static struct H{
		CHAR c;
		SHORT s;
		INT i;
		UCHAR uc;
		USHORT us;
		UINT ui;
		LONG l;
		ULONG ul;
		FLOAT f;
		DOUBLE d;
#if ANSI
		SCHAR sc;
		LDOUBLE ld;
#endif
		struct H *pH;
		} H, *ppH = &H;
	H.pH = &H;
	ppH->pH->c = 7;
#if ANSI
	ppH->pH->sc = 8;
#endif
	ppH->pH->s = 9;
	ppH->pH->i = 10;
	ppH->pH->uc = 11;
	ppH->pH->us = 12;
	ppH->pH->ui = 13;
	ppH->pH->l = 14;
	ppH->pH->ul = 15;
	ppH->pH->f = 16;
	ppH->pH->d = 17;
#if ANSI
	ppH->pH->ld = 18;
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto pscalar2 auto pstruct2 cast ";
	iequals(__LINE__,  (CHAR)ppH->pH->c, 7);
	iequals(__LINE__,  (CHAR)**ppCc, 7);
#if ANSI
	iequals(__LINE__,  (CHAR)ppH->pH->sc, 8);
	iequals(__LINE__,  (SCHAR)**ppCc, 7);
#endif
	iequals(__LINE__,  (CHAR)ppH->pH->s, 9);
	iequals(__LINE__,  (SHORT)**ppCc, 7);
	iequals(__LINE__,  (CHAR)ppH->pH->i, 10);
	iequals(__LINE__,  (INT)**ppCc, 7);
	iequals(__LINE__,  (CHAR)ppH->pH->uc, 11);
	iequals(__LINE__,  (UCHAR)**ppCc, 7);
	iequals(__LINE__,  (CHAR)ppH->pH->us, 12);
	iequals(__LINE__,  (USHORT)**ppCc, 7);
	iequals(__LINE__,  (CHAR)ppH->pH->ui, 13);
	iequals(__LINE__,  (UINT)**ppCc, 7);
	iequals(__LINE__,  (CHAR)ppH->pH->l, 14);
	lequals(__LINE__,  (LONG)**ppCc, 7L);
	iequals(__LINE__,  (CHAR)ppH->pH->ul, 15);
	lequals(__LINE__,  (ULONG)**ppCc, 7L);
	iequals(__LINE__,  (CHAR)ppH->pH->f, 16);
	dequals(__LINE__,  (FLOAT)**ppCc, 7.);
	iequals(__LINE__,  (CHAR)ppH->pH->d, 17);
	dequals(__LINE__,  (DOUBLE)**ppCc, 7.);
#if ANSI
	iequals(__LINE__,  (CHAR)ppH->pH->ld, 18);
	ldequals(__LINE__,  (LDOUBLE)**ppCc, 7.L);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->c, 7);
	iequals(__LINE__,  (CHAR)**ppCsc, 8);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->sc, 8);
	iequals(__LINE__,  (SCHAR)**ppCsc, 8);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->s, 9);
	iequals(__LINE__,  (SHORT)**ppCsc, 8);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->i, 10);
	iequals(__LINE__,  (INT)**ppCsc, 8);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->uc, 11);
	iequals(__LINE__,  (UCHAR)**ppCsc, 8);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->us, 12);
	iequals(__LINE__,  (USHORT)**ppCsc, 8);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->ui, 13);
	iequals(__LINE__,  (UINT)**ppCsc, 8);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->l, 14);
	lequals(__LINE__,  (LONG)**ppCsc, 8L);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->ul, 15);
	lequals(__LINE__,  (ULONG)**ppCsc, 8L);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->f, 16);
	dequals(__LINE__,  (FLOAT)**ppCsc, 8.);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->d, 17);
	dequals(__LINE__,  (DOUBLE)**ppCsc, 8.);
#endif
#if ANSI
	iequals(__LINE__,  (SCHAR)ppH->pH->ld, 18);
	ldequals(__LINE__,  (LDOUBLE)**ppCsc, 8.L);
#endif
	iequals(__LINE__,  (SHORT)ppH->pH->c, 7);
	iequals(__LINE__,  (CHAR)**ppCs, 9);
#if ANSI
	iequals(__LINE__,  (SHORT)ppH->pH->sc, 8);
	iequals(__LINE__,  (SCHAR)**ppCs, 9);
#endif
	iequals(__LINE__,  (SHORT)ppH->pH->s, 9);
	iequals(__LINE__,  (SHORT)**ppCs, 9);
	iequals(__LINE__,  (SHORT)ppH->pH->i, 10);
	iequals(__LINE__,  (INT)**ppCs, 9);
	iequals(__LINE__,  (SHORT)ppH->pH->uc, 11);
	iequals(__LINE__,  (UCHAR)**ppCs, 9);
	iequals(__LINE__,  (SHORT)ppH->pH->us, 12);
	iequals(__LINE__,  (USHORT)**ppCs, 9);
	iequals(__LINE__,  (SHORT)ppH->pH->ui, 13);
	iequals(__LINE__,  (UINT)**ppCs, 9);
	iequals(__LINE__,  (SHORT)ppH->pH->l, 14);
	lequals(__LINE__,  (LONG)**ppCs, 9L);
	iequals(__LINE__,  (SHORT)ppH->pH->ul, 15);
	lequals(__LINE__,  (ULONG)**ppCs, 9L);
	iequals(__LINE__,  (SHORT)ppH->pH->f, 16);
	dequals(__LINE__,  (FLOAT)**ppCs, 9.);
	iequals(__LINE__,  (SHORT)ppH->pH->d, 17);
	dequals(__LINE__,  (DOUBLE)**ppCs, 9.);
#if ANSI
	iequals(__LINE__,  (SHORT)ppH->pH->ld, 18);
	ldequals(__LINE__,  (LDOUBLE)**ppCs, 9.L);
#endif
	iequals(__LINE__,  (INT)ppH->pH->c, 7);
	iequals(__LINE__,  (CHAR)**ppCi, 10);
#if ANSI
	iequals(__LINE__,  (INT)ppH->pH->sc, 8);
	iequals(__LINE__,  (SCHAR)**ppCi, 10);
#endif
	iequals(__LINE__,  (INT)ppH->pH->s, 9);
	iequals(__LINE__,  (SHORT)**ppCi, 10);
	iequals(__LINE__,  (INT)ppH->pH->i, 10);
	iequals(__LINE__,  (INT)**ppCi, 10);
	iequals(__LINE__,  (INT)ppH->pH->uc, 11);
	iequals(__LINE__,  (UCHAR)**ppCi, 10);
	iequals(__LINE__,  (INT)ppH->pH->us, 12);
	iequals(__LINE__,  (USHORT)**ppCi, 10);
	iequals(__LINE__,  (INT)ppH->pH->ui, 13);
	iequals(__LINE__,  (UINT)**ppCi, 10);
	iequals(__LINE__,  (INT)ppH->pH->l, 14);
	lequals(__LINE__,  (LONG)**ppCi, 10L);
	iequals(__LINE__,  (INT)ppH->pH->ul, 15);
	lequals(__LINE__,  (ULONG)**ppCi, 10L);
	iequals(__LINE__,  (INT)ppH->pH->f, 16);
	dequals(__LINE__,  (FLOAT)**ppCi, 10.);
	iequals(__LINE__,  (INT)ppH->pH->d, 17);
	dequals(__LINE__,  (DOUBLE)**ppCi, 10.);
#if ANSI
	iequals(__LINE__,  (INT)ppH->pH->ld, 18);
	ldequals(__LINE__,  (LDOUBLE)**ppCi, 10.L);
#endif
	iequals(__LINE__,  (UCHAR)ppH->pH->c, 7);
	iequals(__LINE__,  (CHAR)**ppCuc, 11);
#if ANSI
	iequals(__LINE__,  (UCHAR)ppH->pH->sc, 8);
	iequals(__LINE__,  (SCHAR)**ppCuc, 11);
#endif
	iequals(__LINE__,  (UCHAR)ppH->pH->s, 9);
	iequals(__LINE__,  (SHORT)**ppCuc, 11);
	iequals(__LINE__,  (UCHAR)ppH->pH->i, 10);
	iequals(__LINE__,  (INT)**ppCuc, 11);
	iequals(__LINE__,  (UCHAR)ppH->pH->uc, 11);
	iequals(__LINE__,  (UCHAR)**ppCuc, 11);
	iequals(__LINE__,  (UCHAR)ppH->pH->us, 12);
	iequals(__LINE__,  (USHORT)**ppCuc, 11);
	iequals(__LINE__,  (UCHAR)ppH->pH->ui, 13);
	iequals(__LINE__,  (UINT)**ppCuc, 11);
	iequals(__LINE__,  (UCHAR)ppH->pH->l, 14);
	lequals(__LINE__,  (LONG)**ppCuc, 11L);
	iequals(__LINE__,  (UCHAR)ppH->pH->ul, 15);
	lequals(__LINE__,  (ULONG)**ppCuc, 11L);
	iequals(__LINE__,  (UCHAR)ppH->pH->f, 16);
	dequals(__LINE__,  (FLOAT)**ppCuc, 11.);
	iequals(__LINE__,  (UCHAR)ppH->pH->d, 17);
	dequals(__LINE__,  (DOUBLE)**ppCuc, 11.);
#if ANSI
	iequals(__LINE__,  (UCHAR)ppH->pH->ld, 18);
	ldequals(__LINE__,  (LDOUBLE)**ppCuc, 11.L);
#endif
	iequals(__LINE__,  (USHORT)ppH->pH->c, 7);
	iequals(__LINE__,  (CHAR)**ppCus, 12);
#if ANSI
	iequals(__LINE__,  (USHORT)ppH->pH->sc, 8);
	iequals(__LINE__,  (SCHAR)**ppCus, 12);
#endif
	iequals(__LINE__,  (USHORT)ppH->pH->s, 9);
	iequals(__LINE__,  (SHORT)**ppCus, 12);
	iequals(__LINE__,  (USHORT)ppH->pH->i, 10);
	iequals(__LINE__,  (INT)**ppCus, 12);
	iequals(__LINE__,  (USHORT)ppH->pH->uc, 11);
	iequals(__LINE__,  (UCHAR)**ppCus, 12);
	iequals(__LINE__,  (USHORT)ppH->pH->us, 12);
	iequals(__LINE__,  (USHORT)**ppCus, 12);
	iequals(__LINE__,  (USHORT)ppH->pH->ui, 13);
	iequals(__LINE__,  (UINT)**ppCus, 12);
	iequals(__LINE__,  (USHORT)ppH->pH->l, 14);
	lequals(__LINE__,  (LONG)**ppCus, 12L);
	iequals(__LINE__,  (USHORT)ppH->pH->ul, 15);
	lequals(__LINE__,  (ULONG)**ppCus, 12L);
	iequals(__LINE__,  (USHORT)ppH->pH->f, 16);
	dequals(__LINE__,  (FLOAT)**ppCus, 12.);
	iequals(__LINE__,  (USHORT)ppH->pH->d, 17);
	dequals(__LINE__,  (DOUBLE)**ppCus, 12.);
#if ANSI
	iequals(__LINE__,  (USHORT)ppH->pH->ld, 18);
	ldequals(__LINE__,  (LDOUBLE)**ppCus, 12.L);
#endif
	iequals(__LINE__,  (UINT)ppH->pH->c, 7);
	iequals(__LINE__,  (CHAR)**ppCui, 13);
#if ANSI
	iequals(__LINE__,  (UINT)ppH->pH->sc, 8);
	iequals(__LINE__,  (SCHAR)**ppCui, 13);
#endif
	iequals(__LINE__,  (UINT)ppH->pH->s, 9);
	iequals(__LINE__,  (SHORT)**ppCui, 13);
	iequals(__LINE__,  (UINT)ppH->pH->i, 10);
	iequals(__LINE__,  (INT)**ppCui, 13);
	iequals(__LINE__,  (UINT)ppH->pH->uc, 11);
	iequals(__LINE__,  (UCHAR)**ppCui, 13);
	iequals(__LINE__,  (UINT)ppH->pH->us, 12);
	iequals(__LINE__,  (USHORT)**ppCui, 13);
	iequals(__LINE__,  (UINT)ppH->pH->ui, 13);
	iequals(__LINE__,  (UINT)**ppCui, 13);
	iequals(__LINE__,  (UINT)ppH->pH->l, 14);
	lequals(__LINE__,  (LONG)**ppCui, 13L);
	iequals(__LINE__,  (UINT)ppH->pH->ul, 15);
	lequals(__LINE__,  (ULONG)**ppCui, 13L);
	iequals(__LINE__,  (UINT)ppH->pH->f, 16);
	dequals(__LINE__,  (FLOAT)**ppCui, 13.);
	iequals(__LINE__,  (UINT)ppH->pH->d, 17);
	dequals(__LINE__,  (DOUBLE)**ppCui, 13.);
#if ANSI
	iequals(__LINE__,  (UINT)ppH->pH->ld, 18);
	ldequals(__LINE__,  (LDOUBLE)**ppCui, 13.L);
#endif
	lequals(__LINE__,  (LONG)ppH->pH->c, 7L);
	iequals(__LINE__,  (CHAR)**ppCl, 14);
#if ANSI
	lequals(__LINE__,  (LONG)ppH->pH->sc, 8L);
	iequals(__LINE__,  (SCHAR)**ppCl, 14);
#endif
	lequals(__LINE__,  (LONG)ppH->pH->s, 9L);
	iequals(__LINE__,  (SHORT)**ppCl, 14);
	lequals(__LINE__,  (LONG)ppH->pH->i, 10L);
	iequals(__LINE__,  (INT)**ppCl, 14);
	lequals(__LINE__,  (LONG)ppH->pH->uc, 11L);
	iequals(__LINE__,  (UCHAR)**ppCl, 14);
	lequals(__LINE__,  (LONG)ppH->pH->us, 12L);
	iequals(__LINE__,  (USHORT)**ppCl, 14);
	lequals(__LINE__,  (LONG)ppH->pH->ui, 13L);
	iequals(__LINE__,  (UINT)**ppCl, 14);
	lequals(__LINE__,  (LONG)ppH->pH->l, 14L);
	lequals(__LINE__,  (LONG)**ppCl, 14L);
	lequals(__LINE__,  (LONG)ppH->pH->ul, 15L);
	lequals(__LINE__,  (ULONG)**ppCl, 14L);
	lequals(__LINE__,  (LONG)ppH->pH->f, 16L);
	dequals(__LINE__,  (FLOAT)**ppCl, 14.);
	lequals(__LINE__,  (LONG)ppH->pH->d, 17L);
	dequals(__LINE__,  (DOUBLE)**ppCl, 14.);
#if ANSI
	lequals(__LINE__,  (LONG)ppH->pH->ld, 18L);
	ldequals(__LINE__,  (LDOUBLE)**ppCl, 14.L);
#endif
	lequals(__LINE__,  (ULONG)ppH->pH->c, 7L);
	iequals(__LINE__,  (CHAR)**ppCul, 15);
#if ANSI
	lequals(__LINE__,  (ULONG)ppH->pH->sc, 8L);
	iequals(__LINE__,  (SCHAR)**ppCul, 15);
#endif
	lequals(__LINE__,  (ULONG)ppH->pH->s, 9L);
	iequals(__LINE__,  (SHORT)**ppCul, 15);
	lequals(__LINE__,  (ULONG)ppH->pH->i, 10L);
	iequals(__LINE__,  (INT)**ppCul, 15);
	lequals(__LINE__,  (ULONG)ppH->pH->uc, 11L);
	iequals(__LINE__,  (UCHAR)**ppCul, 15);
	lequals(__LINE__,  (ULONG)ppH->pH->us, 12L);
	iequals(__LINE__,  (USHORT)**ppCul, 15);
	lequals(__LINE__,  (ULONG)ppH->pH->ui, 13L);
	iequals(__LINE__,  (UINT)**ppCul, 15);
	lequals(__LINE__,  (ULONG)ppH->pH->l, 14L);
	lequals(__LINE__,  (LONG)**ppCul, 15L);
	lequals(__LINE__,  (ULONG)ppH->pH->ul, 15L);
	lequals(__LINE__,  (ULONG)**ppCul, 15L);
	lequals(__LINE__,  (ULONG)ppH->pH->f, 16L);
	dequals(__LINE__,  (FLOAT)**ppCul, 15.);
	lequals(__LINE__,  (ULONG)ppH->pH->d, 17L);
	dequals(__LINE__,  (DOUBLE)**ppCul, 15.);
#if ANSI
	lequals(__LINE__,  (ULONG)ppH->pH->ld, 18L);
	ldequals(__LINE__,  (LDOUBLE)**ppCul, 15.L);
#endif
	dequals(__LINE__,  (FLOAT)ppH->pH->c, 7.);
	iequals(__LINE__,  (CHAR)**ppCf, 16);
#if ANSI
	dequals(__LINE__,  (FLOAT)ppH->pH->sc, 8.);
	iequals(__LINE__,  (SCHAR)**ppCf, 16);
#endif
	dequals(__LINE__,  (FLOAT)ppH->pH->s, 9.);
	iequals(__LINE__,  (SHORT)**ppCf, 16);
	dequals(__LINE__,  (FLOAT)ppH->pH->i, 10.);
	iequals(__LINE__,  (INT)**ppCf, 16);
	dequals(__LINE__,  (FLOAT)ppH->pH->uc, 11.);
	iequals(__LINE__,  (UCHAR)**ppCf, 16);
	dequals(__LINE__,  (FLOAT)ppH->pH->us, 12.);
	iequals(__LINE__,  (USHORT)**ppCf, 16);
	dequals(__LINE__,  (FLOAT)ppH->pH->ui, 13.);
	iequals(__LINE__,  (UINT)**ppCf, 16);
	dequals(__LINE__,  (FLOAT)ppH->pH->l, 14.);
	lequals(__LINE__,  (LONG)**ppCf, 16L);
	dequals(__LINE__,  (FLOAT)ppH->pH->ul, 15.);
	lequals(__LINE__,  (ULONG)**ppCf, 16L);
	dequals(__LINE__,  (FLOAT)ppH->pH->f, 16.);
	dequals(__LINE__,  (FLOAT)**ppCf, 16.);
	dequals(__LINE__,  (FLOAT)ppH->pH->d, 17.);
	dequals(__LINE__,  (DOUBLE)**ppCf, 16.);
#if ANSI
	dequals(__LINE__,  (FLOAT)ppH->pH->ld, 18.);
	ldequals(__LINE__,  (LDOUBLE)**ppCf, 16.L);
#endif
	dequals(__LINE__,  (DOUBLE)ppH->pH->c, 7.);
	iequals(__LINE__,  (CHAR)**ppCd, 17);
#if ANSI
	dequals(__LINE__,  (DOUBLE)ppH->pH->sc, 8.);
	iequals(__LINE__,  (SCHAR)**ppCd, 17);
#endif
	dequals(__LINE__,  (DOUBLE)ppH->pH->s, 9.);
	iequals(__LINE__,  (SHORT)**ppCd, 17);
	dequals(__LINE__,  (DOUBLE)ppH->pH->i, 10.);
	iequals(__LINE__,  (INT)**ppCd, 17);
	dequals(__LINE__,  (DOUBLE)ppH->pH->uc, 11.);
	iequals(__LINE__,  (UCHAR)**ppCd, 17);
	dequals(__LINE__,  (DOUBLE)ppH->pH->us, 12.);
	iequals(__LINE__,  (USHORT)**ppCd, 17);
	dequals(__LINE__,  (DOUBLE)ppH->pH->ui, 13.);
	iequals(__LINE__,  (UINT)**ppCd, 17);
	dequals(__LINE__,  (DOUBLE)ppH->pH->l, 14.);
	lequals(__LINE__,  (LONG)**ppCd, 17L);
	dequals(__LINE__,  (DOUBLE)ppH->pH->ul, 15.);
	lequals(__LINE__,  (ULONG)**ppCd, 17L);
	dequals(__LINE__,  (DOUBLE)ppH->pH->f, 16.);
	dequals(__LINE__,  (FLOAT)**ppCd, 17.);
	dequals(__LINE__,  (DOUBLE)ppH->pH->d, 17.);
	dequals(__LINE__,  (DOUBLE)**ppCd, 17.);
#if ANSI
	dequals(__LINE__,  (DOUBLE)ppH->pH->ld, 18.);
	ldequals(__LINE__,  (LDOUBLE)**ppCd, 17.L);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->c, 7.L);
	iequals(__LINE__,  (CHAR)**ppCld, 18);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->sc, 8.L);
	iequals(__LINE__,  (SCHAR)**ppCld, 18);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->s, 9.L);
	iequals(__LINE__,  (SHORT)**ppCld, 18);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->i, 10.L);
	iequals(__LINE__,  (INT)**ppCld, 18);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->uc, 11.L);
	iequals(__LINE__,  (UCHAR)**ppCld, 18);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->us, 12.L);
	iequals(__LINE__,  (USHORT)**ppCld, 18);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->ui, 13.L);
	iequals(__LINE__,  (UINT)**ppCld, 18);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->l, 14.L);
	lequals(__LINE__,  (LONG)**ppCld, 18L);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->ul, 15.L);
	lequals(__LINE__,  (ULONG)**ppCld, 18L);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->f, 16.L);
	dequals(__LINE__,  (FLOAT)**ppCld, 18.);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->d, 17.L);
	dequals(__LINE__,  (DOUBLE)**ppCld, 18.);
#endif
#if ANSI
	ldequals(__LINE__,  (LDOUBLE)ppH->pH->ld, 18.L);
	ldequals(__LINE__,  (LDOUBLE)**ppCld, 18.L);
#endif
	}}
	return 	report(Filename);
	}
