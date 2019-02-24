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
#include "m.h"
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
	CHAR Mc();
#if ANSI
	SCHAR Msc();
#endif
	SHORT Ms();
	INT Mi();
	UCHAR Muc();
	USHORT Mus();
	UINT Mui();
	LONG Ml();
	ULONG Mul();
	FLOAT Mf();
	DOUBLE Md();
#if ANSI
	LDOUBLE Mld();
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto pscalar2 auto func compl ";
	iequals(__LINE__, ~ **ppCc, ~7);
	iequals(__LINE__, ~ Mc(), ~7);
#if ANSI
	iequals(__LINE__, ~ **ppCc, ~7);
	iequals(__LINE__, ~ Msc(), ~8);
#endif
	iequals(__LINE__, ~ **ppCc, ~7);
	iequals(__LINE__, ~ Ms(), ~9);
	iequals(__LINE__, ~ **ppCc, ~7);
	iequals(__LINE__, ~ Mi(), ~10);
	iequals(__LINE__, ~ **ppCc, ~7);
	iequals(__LINE__, ~ (int)Muc(), ~11);
	iequals(__LINE__, ~ **ppCc, ~7);
	iequals(__LINE__, ~ (int)Mus(), ~12);
	iequals(__LINE__, ~ **ppCc, ~7);
	iequals(__LINE__, ~ (int)Mui(), ~13);
	iequals(__LINE__, ~ **ppCc, ~7);
	lequals(__LINE__, ~ Ml(), ~14L);
	iequals(__LINE__, ~ **ppCc, ~7);
	lequals(__LINE__, ~ (long)Mul(), ~15L);
#if ANSI
	iequals(__LINE__, ~ **ppCsc, ~8);
	iequals(__LINE__, ~ Mc(), ~7);
#endif
#if ANSI
	iequals(__LINE__, ~ **ppCsc, ~8);
	iequals(__LINE__, ~ Msc(), ~8);
#endif
#if ANSI
	iequals(__LINE__, ~ **ppCsc, ~8);
	iequals(__LINE__, ~ Ms(), ~9);
#endif
#if ANSI
	iequals(__LINE__, ~ **ppCsc, ~8);
	iequals(__LINE__, ~ Mi(), ~10);
#endif
#if ANSI
	iequals(__LINE__, ~ **ppCsc, ~8);
	iequals(__LINE__, ~ (int)Muc(), ~11);
#endif
#if ANSI
	iequals(__LINE__, ~ **ppCsc, ~8);
	iequals(__LINE__, ~ (int)Mus(), ~12);
#endif
#if ANSI
	iequals(__LINE__, ~ **ppCsc, ~8);
	iequals(__LINE__, ~ (int)Mui(), ~13);
#endif
#if ANSI
	iequals(__LINE__, ~ **ppCsc, ~8);
	lequals(__LINE__, ~ Ml(), ~14L);
#endif
#if ANSI
	iequals(__LINE__, ~ **ppCsc, ~8);
	lequals(__LINE__, ~ (long)Mul(), ~15L);
#endif
	iequals(__LINE__, ~ **ppCs, ~9);
	iequals(__LINE__, ~ Mc(), ~7);
#if ANSI
	iequals(__LINE__, ~ **ppCs, ~9);
	iequals(__LINE__, ~ Msc(), ~8);
#endif
	iequals(__LINE__, ~ **ppCs, ~9);
	iequals(__LINE__, ~ Ms(), ~9);
	iequals(__LINE__, ~ **ppCs, ~9);
	iequals(__LINE__, ~ Mi(), ~10);
	iequals(__LINE__, ~ **ppCs, ~9);
	iequals(__LINE__, ~ (int)Muc(), ~11);
	iequals(__LINE__, ~ **ppCs, ~9);
	iequals(__LINE__, ~ (int)Mus(), ~12);
	iequals(__LINE__, ~ **ppCs, ~9);
	iequals(__LINE__, ~ (int)Mui(), ~13);
	iequals(__LINE__, ~ **ppCs, ~9);
	lequals(__LINE__, ~ Ml(), ~14L);
	iequals(__LINE__, ~ **ppCs, ~9);
	lequals(__LINE__, ~ (long)Mul(), ~15L);
	iequals(__LINE__, ~ **ppCi, ~10);
	iequals(__LINE__, ~ Mc(), ~7);
#if ANSI
	iequals(__LINE__, ~ **ppCi, ~10);
	iequals(__LINE__, ~ Msc(), ~8);
#endif
	iequals(__LINE__, ~ **ppCi, ~10);
	iequals(__LINE__, ~ Ms(), ~9);
	iequals(__LINE__, ~ **ppCi, ~10);
	iequals(__LINE__, ~ Mi(), ~10);
	iequals(__LINE__, ~ **ppCi, ~10);
	iequals(__LINE__, ~ (int)Muc(), ~11);
	iequals(__LINE__, ~ **ppCi, ~10);
	iequals(__LINE__, ~ (int)Mus(), ~12);
	iequals(__LINE__, ~ **ppCi, ~10);
	iequals(__LINE__, ~ (int)Mui(), ~13);
	iequals(__LINE__, ~ **ppCi, ~10);
	lequals(__LINE__, ~ Ml(), ~14L);
	iequals(__LINE__, ~ **ppCi, ~10);
	lequals(__LINE__, ~ (long)Mul(), ~15L);
	iequals(__LINE__, ~ (int)**ppCuc, ~11);
	iequals(__LINE__, ~ Mc(), ~7);
#if ANSI
	iequals(__LINE__, ~ (int)**ppCuc, ~11);
	iequals(__LINE__, ~ Msc(), ~8);
#endif
	iequals(__LINE__, ~ (int)**ppCuc, ~11);
	iequals(__LINE__, ~ Ms(), ~9);
	iequals(__LINE__, ~ (int)**ppCuc, ~11);
	iequals(__LINE__, ~ Mi(), ~10);
	iequals(__LINE__, ~ (int)**ppCuc, ~11);
	iequals(__LINE__, ~ (int)Muc(), ~11);
	iequals(__LINE__, ~ (int)**ppCuc, ~11);
	iequals(__LINE__, ~ (int)Mus(), ~12);
	iequals(__LINE__, ~ (int)**ppCuc, ~11);
	iequals(__LINE__, ~ (int)Mui(), ~13);
	iequals(__LINE__, ~ (int)**ppCuc, ~11);
	lequals(__LINE__, ~ Ml(), ~14L);
	iequals(__LINE__, ~ (int)**ppCuc, ~11);
	lequals(__LINE__, ~ (long)Mul(), ~15L);
	iequals(__LINE__, ~ (int)**ppCus, ~12);
	iequals(__LINE__, ~ Mc(), ~7);
#if ANSI
	iequals(__LINE__, ~ (int)**ppCus, ~12);
	iequals(__LINE__, ~ Msc(), ~8);
#endif
	iequals(__LINE__, ~ (int)**ppCus, ~12);
	iequals(__LINE__, ~ Ms(), ~9);
	iequals(__LINE__, ~ (int)**ppCus, ~12);
	iequals(__LINE__, ~ Mi(), ~10);
	iequals(__LINE__, ~ (int)**ppCus, ~12);
	iequals(__LINE__, ~ (int)Muc(), ~11);
	iequals(__LINE__, ~ (int)**ppCus, ~12);
	iequals(__LINE__, ~ (int)Mus(), ~12);
	iequals(__LINE__, ~ (int)**ppCus, ~12);
	iequals(__LINE__, ~ (int)Mui(), ~13);
	iequals(__LINE__, ~ (int)**ppCus, ~12);
	lequals(__LINE__, ~ Ml(), ~14L);
	iequals(__LINE__, ~ (int)**ppCus, ~12);
	lequals(__LINE__, ~ (long)Mul(), ~15L);
	iequals(__LINE__, ~ (int)**ppCui, ~13);
	iequals(__LINE__, ~ Mc(), ~7);
#if ANSI
	iequals(__LINE__, ~ (int)**ppCui, ~13);
	iequals(__LINE__, ~ Msc(), ~8);
#endif
	iequals(__LINE__, ~ (int)**ppCui, ~13);
	iequals(__LINE__, ~ Ms(), ~9);
	iequals(__LINE__, ~ (int)**ppCui, ~13);
	iequals(__LINE__, ~ Mi(), ~10);
	iequals(__LINE__, ~ (int)**ppCui, ~13);
	iequals(__LINE__, ~ (int)Muc(), ~11);
	iequals(__LINE__, ~ (int)**ppCui, ~13);
	iequals(__LINE__, ~ (int)Mus(), ~12);
	iequals(__LINE__, ~ (int)**ppCui, ~13);
	iequals(__LINE__, ~ (int)Mui(), ~13);
	iequals(__LINE__, ~ (int)**ppCui, ~13);
	lequals(__LINE__, ~ Ml(), ~14L);
	iequals(__LINE__, ~ (int)**ppCui, ~13);
	lequals(__LINE__, ~ (long)Mul(), ~15L);
	lequals(__LINE__, ~ **ppCl, ~14L);
	iequals(__LINE__, ~ Mc(), ~7);
#if ANSI
	lequals(__LINE__, ~ **ppCl, ~14L);
	iequals(__LINE__, ~ Msc(), ~8);
#endif
	lequals(__LINE__, ~ **ppCl, ~14L);
	iequals(__LINE__, ~ Ms(), ~9);
	lequals(__LINE__, ~ **ppCl, ~14L);
	iequals(__LINE__, ~ Mi(), ~10);
	lequals(__LINE__, ~ **ppCl, ~14L);
	iequals(__LINE__, ~ (int)Muc(), ~11);
	lequals(__LINE__, ~ **ppCl, ~14L);
	iequals(__LINE__, ~ (int)Mus(), ~12);
	lequals(__LINE__, ~ **ppCl, ~14L);
	iequals(__LINE__, ~ (int)Mui(), ~13);
	lequals(__LINE__, ~ **ppCl, ~14L);
	lequals(__LINE__, ~ Ml(), ~14L);
	lequals(__LINE__, ~ **ppCl, ~14L);
	lequals(__LINE__, ~ (long)Mul(), ~15L);
	lequals(__LINE__, ~ (long)**ppCul, ~15L);
	iequals(__LINE__, ~ Mc(), ~7);
#if ANSI
	lequals(__LINE__, ~ (long)**ppCul, ~15L);
	iequals(__LINE__, ~ Msc(), ~8);
#endif
	lequals(__LINE__, ~ (long)**ppCul, ~15L);
	iequals(__LINE__, ~ Ms(), ~9);
	lequals(__LINE__, ~ (long)**ppCul, ~15L);
	iequals(__LINE__, ~ Mi(), ~10);
	lequals(__LINE__, ~ (long)**ppCul, ~15L);
	iequals(__LINE__, ~ (int)Muc(), ~11);
	lequals(__LINE__, ~ (long)**ppCul, ~15L);
	iequals(__LINE__, ~ (int)Mus(), ~12);
	lequals(__LINE__, ~ (long)**ppCul, ~15L);
	iequals(__LINE__, ~ (int)Mui(), ~13);
	lequals(__LINE__, ~ (long)**ppCul, ~15L);
	lequals(__LINE__, ~ Ml(), ~14L);
	lequals(__LINE__, ~ (long)**ppCul, ~15L);
	lequals(__LINE__, ~ (long)Mul(), ~15L);
	}}
	return 	report(Filename);
	}
