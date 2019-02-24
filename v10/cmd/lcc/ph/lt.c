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
	Filename =  " auto pscalar2 auto func lt ";
	iequals(__LINE__, **ppCc < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCc, 0);
	if (**ppCc < Mc()) complain(__LINE__);
	if (Mc() < **ppCc) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCc < Msc(), 1);
	iequals(__LINE__, Msc() < **ppCc, 0);
	if (**ppCc < Msc()); else complain(__LINE__);
	if (Msc() < **ppCc) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCc < Ms(), 1);
	iequals(__LINE__, Ms() < **ppCc, 0);
	if (**ppCc < Ms()); else complain(__LINE__);
	if (Ms() < **ppCc) complain(__LINE__);
	iequals(__LINE__, **ppCc < Mi(), 1);
	iequals(__LINE__, Mi() < **ppCc, 0);
	if (**ppCc < Mi()); else complain(__LINE__);
	if (Mi() < **ppCc) complain(__LINE__);
	iequals(__LINE__, **ppCc < Muc(), 1);
	iequals(__LINE__, Muc() < **ppCc, 0);
	if (**ppCc < Muc()); else complain(__LINE__);
	if (Muc() < **ppCc) complain(__LINE__);
	iequals(__LINE__, **ppCc < Mus(), 1);
	iequals(__LINE__, Mus() < **ppCc, 0);
	if (**ppCc < Mus()); else complain(__LINE__);
	if (Mus() < **ppCc) complain(__LINE__);
	iequals(__LINE__, **ppCc < Mui(), 1);
	iequals(__LINE__, Mui() < **ppCc, 0);
	if (**ppCc < Mui()); else complain(__LINE__);
	if (Mui() < **ppCc) complain(__LINE__);
	iequals(__LINE__, **ppCc < Ml(), 1);
	iequals(__LINE__, Ml() < **ppCc, 0);
	if (**ppCc < Ml()); else complain(__LINE__);
	if (Ml() < **ppCc) complain(__LINE__);
	iequals(__LINE__, **ppCc < Mul(), 1);
	iequals(__LINE__, Mul() < **ppCc, 0);
	if (**ppCc < Mul()); else complain(__LINE__);
	if (Mul() < **ppCc) complain(__LINE__);
	iequals(__LINE__, **ppCc < Mf(), 1);
	iequals(__LINE__, Mf() < **ppCc, 0);
	if (**ppCc < Mf()); else complain(__LINE__);
	if (Mf() < **ppCc) complain(__LINE__);
	iequals(__LINE__, **ppCc < Md(), 1);
	iequals(__LINE__, Md() < **ppCc, 0);
	if (**ppCc < Md()); else complain(__LINE__);
	if (Md() < **ppCc) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCc < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCc, 0);
	if (**ppCc < Mld()); else complain(__LINE__);
	if (Mld() < **ppCc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCsc, 1);
	if (**ppCsc < Mc()) complain(__LINE__);
	if (Mc() < **ppCsc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCsc, 0);
	if (**ppCsc < Msc()) complain(__LINE__);
	if (Msc() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Ms(), 1);
	iequals(__LINE__, Ms() < **ppCsc, 0);
	if (**ppCsc < Ms()); else complain(__LINE__);
	if (Ms() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Mi(), 1);
	iequals(__LINE__, Mi() < **ppCsc, 0);
	if (**ppCsc < Mi()); else complain(__LINE__);
	if (Mi() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Muc(), 1);
	iequals(__LINE__, Muc() < **ppCsc, 0);
	if (**ppCsc < Muc()); else complain(__LINE__);
	if (Muc() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Mus(), 1);
	iequals(__LINE__, Mus() < **ppCsc, 0);
	if (**ppCsc < Mus()); else complain(__LINE__);
	if (Mus() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Mui(), 1);
	iequals(__LINE__, Mui() < **ppCsc, 0);
	if (**ppCsc < Mui()); else complain(__LINE__);
	if (Mui() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Ml(), 1);
	iequals(__LINE__, Ml() < **ppCsc, 0);
	if (**ppCsc < Ml()); else complain(__LINE__);
	if (Ml() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Mul(), 1);
	iequals(__LINE__, Mul() < **ppCsc, 0);
	if (**ppCsc < Mul()); else complain(__LINE__);
	if (Mul() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Mf(), 1);
	iequals(__LINE__, Mf() < **ppCsc, 0);
	if (**ppCsc < Mf()); else complain(__LINE__);
	if (Mf() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Md(), 1);
	iequals(__LINE__, Md() < **ppCsc, 0);
	if (**ppCsc < Md()); else complain(__LINE__);
	if (Md() < **ppCsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCsc, 0);
	if (**ppCsc < Mld()); else complain(__LINE__);
	if (Mld() < **ppCsc) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCs < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCs, 1);
	if (**ppCs < Mc()) complain(__LINE__);
	if (Mc() < **ppCs); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCs < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCs, 1);
	if (**ppCs < Msc()) complain(__LINE__);
	if (Msc() < **ppCs); else complain(__LINE__);
#endif
	iequals(__LINE__, **ppCs < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCs, 0);
	if (**ppCs < Ms()) complain(__LINE__);
	if (Ms() < **ppCs) complain(__LINE__);
	iequals(__LINE__, **ppCs < Mi(), 1);
	iequals(__LINE__, Mi() < **ppCs, 0);
	if (**ppCs < Mi()); else complain(__LINE__);
	if (Mi() < **ppCs) complain(__LINE__);
	iequals(__LINE__, **ppCs < Muc(), 1);
	iequals(__LINE__, Muc() < **ppCs, 0);
	if (**ppCs < Muc()); else complain(__LINE__);
	if (Muc() < **ppCs) complain(__LINE__);
	iequals(__LINE__, **ppCs < Mus(), 1);
	iequals(__LINE__, Mus() < **ppCs, 0);
	if (**ppCs < Mus()); else complain(__LINE__);
	if (Mus() < **ppCs) complain(__LINE__);
	iequals(__LINE__, **ppCs < Mui(), 1);
	iequals(__LINE__, Mui() < **ppCs, 0);
	if (**ppCs < Mui()); else complain(__LINE__);
	if (Mui() < **ppCs) complain(__LINE__);
	iequals(__LINE__, **ppCs < Ml(), 1);
	iequals(__LINE__, Ml() < **ppCs, 0);
	if (**ppCs < Ml()); else complain(__LINE__);
	if (Ml() < **ppCs) complain(__LINE__);
	iequals(__LINE__, **ppCs < Mul(), 1);
	iequals(__LINE__, Mul() < **ppCs, 0);
	if (**ppCs < Mul()); else complain(__LINE__);
	if (Mul() < **ppCs) complain(__LINE__);
	iequals(__LINE__, **ppCs < Mf(), 1);
	iequals(__LINE__, Mf() < **ppCs, 0);
	if (**ppCs < Mf()); else complain(__LINE__);
	if (Mf() < **ppCs) complain(__LINE__);
	iequals(__LINE__, **ppCs < Md(), 1);
	iequals(__LINE__, Md() < **ppCs, 0);
	if (**ppCs < Md()); else complain(__LINE__);
	if (Md() < **ppCs) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCs < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCs, 0);
	if (**ppCs < Mld()); else complain(__LINE__);
	if (Mld() < **ppCs) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCi < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCi, 1);
	if (**ppCi < Mc()) complain(__LINE__);
	if (Mc() < **ppCi); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCi < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCi, 1);
	if (**ppCi < Msc()) complain(__LINE__);
	if (Msc() < **ppCi); else complain(__LINE__);
#endif
	iequals(__LINE__, **ppCi < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCi, 1);
	if (**ppCi < Ms()) complain(__LINE__);
	if (Ms() < **ppCi); else complain(__LINE__);
	iequals(__LINE__, **ppCi < Mi(), 0);
	iequals(__LINE__, Mi() < **ppCi, 0);
	if (**ppCi < Mi()) complain(__LINE__);
	if (Mi() < **ppCi) complain(__LINE__);
	iequals(__LINE__, **ppCi < Muc(), 1);
	iequals(__LINE__, Muc() < **ppCi, 0);
	if (**ppCi < Muc()); else complain(__LINE__);
	if (Muc() < **ppCi) complain(__LINE__);
	iequals(__LINE__, **ppCi < Mus(), 1);
	iequals(__LINE__, Mus() < **ppCi, 0);
	if (**ppCi < Mus()); else complain(__LINE__);
	if (Mus() < **ppCi) complain(__LINE__);
	iequals(__LINE__, **ppCi < Mui(), 1);
	iequals(__LINE__, Mui() < **ppCi, 0);
	if (**ppCi < Mui()); else complain(__LINE__);
	if (Mui() < **ppCi) complain(__LINE__);
	iequals(__LINE__, **ppCi < Ml(), 1);
	iequals(__LINE__, Ml() < **ppCi, 0);
	if (**ppCi < Ml()); else complain(__LINE__);
	if (Ml() < **ppCi) complain(__LINE__);
	iequals(__LINE__, **ppCi < Mul(), 1);
	iequals(__LINE__, Mul() < **ppCi, 0);
	if (**ppCi < Mul()); else complain(__LINE__);
	if (Mul() < **ppCi) complain(__LINE__);
	iequals(__LINE__, **ppCi < Mf(), 1);
	iequals(__LINE__, Mf() < **ppCi, 0);
	if (**ppCi < Mf()); else complain(__LINE__);
	if (Mf() < **ppCi) complain(__LINE__);
	iequals(__LINE__, **ppCi < Md(), 1);
	iequals(__LINE__, Md() < **ppCi, 0);
	if (**ppCi < Md()); else complain(__LINE__);
	if (Md() < **ppCi) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCi < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCi, 0);
	if (**ppCi < Mld()); else complain(__LINE__);
	if (Mld() < **ppCi) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCuc < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCuc, 1);
	if (**ppCuc < Mc()) complain(__LINE__);
	if (Mc() < **ppCuc); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCuc < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCuc, 1);
	if (**ppCuc < Msc()) complain(__LINE__);
	if (Msc() < **ppCuc); else complain(__LINE__);
#endif
	iequals(__LINE__, **ppCuc < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCuc, 1);
	if (**ppCuc < Ms()) complain(__LINE__);
	if (Ms() < **ppCuc); else complain(__LINE__);
	iequals(__LINE__, **ppCuc < Mi(), 0);
	iequals(__LINE__, Mi() < **ppCuc, 1);
	if (**ppCuc < Mi()) complain(__LINE__);
	if (Mi() < **ppCuc); else complain(__LINE__);
	iequals(__LINE__, **ppCuc < Muc(), 0);
	iequals(__LINE__, Muc() < **ppCuc, 0);
	if (**ppCuc < Muc()) complain(__LINE__);
	if (Muc() < **ppCuc) complain(__LINE__);
	iequals(__LINE__, **ppCuc < Mus(), 1);
	iequals(__LINE__, Mus() < **ppCuc, 0);
	if (**ppCuc < Mus()); else complain(__LINE__);
	if (Mus() < **ppCuc) complain(__LINE__);
	iequals(__LINE__, **ppCuc < Mui(), 1);
	iequals(__LINE__, Mui() < **ppCuc, 0);
	if (**ppCuc < Mui()); else complain(__LINE__);
	if (Mui() < **ppCuc) complain(__LINE__);
	iequals(__LINE__, **ppCuc < Ml(), 1);
	iequals(__LINE__, Ml() < **ppCuc, 0);
	if (**ppCuc < Ml()); else complain(__LINE__);
	if (Ml() < **ppCuc) complain(__LINE__);
	iequals(__LINE__, **ppCuc < Mul(), 1);
	iequals(__LINE__, Mul() < **ppCuc, 0);
	if (**ppCuc < Mul()); else complain(__LINE__);
	if (Mul() < **ppCuc) complain(__LINE__);
	iequals(__LINE__, **ppCuc < Mf(), 1);
	iequals(__LINE__, Mf() < **ppCuc, 0);
	if (**ppCuc < Mf()); else complain(__LINE__);
	if (Mf() < **ppCuc) complain(__LINE__);
	iequals(__LINE__, **ppCuc < Md(), 1);
	iequals(__LINE__, Md() < **ppCuc, 0);
	if (**ppCuc < Md()); else complain(__LINE__);
	if (Md() < **ppCuc) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCuc < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCuc, 0);
	if (**ppCuc < Mld()); else complain(__LINE__);
	if (Mld() < **ppCuc) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCus < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCus, 1);
	if (**ppCus < Mc()) complain(__LINE__);
	if (Mc() < **ppCus); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCus < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCus, 1);
	if (**ppCus < Msc()) complain(__LINE__);
	if (Msc() < **ppCus); else complain(__LINE__);
#endif
	iequals(__LINE__, **ppCus < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCus, 1);
	if (**ppCus < Ms()) complain(__LINE__);
	if (Ms() < **ppCus); else complain(__LINE__);
	iequals(__LINE__, **ppCus < Mi(), 0);
	iequals(__LINE__, Mi() < **ppCus, 1);
	if (**ppCus < Mi()) complain(__LINE__);
	if (Mi() < **ppCus); else complain(__LINE__);
	iequals(__LINE__, **ppCus < Muc(), 0);
	iequals(__LINE__, Muc() < **ppCus, 1);
	if (**ppCus < Muc()) complain(__LINE__);
	if (Muc() < **ppCus); else complain(__LINE__);
	iequals(__LINE__, **ppCus < Mus(), 0);
	iequals(__LINE__, Mus() < **ppCus, 0);
	if (**ppCus < Mus()) complain(__LINE__);
	if (Mus() < **ppCus) complain(__LINE__);
	iequals(__LINE__, **ppCus < Mui(), 1);
	iequals(__LINE__, Mui() < **ppCus, 0);
	if (**ppCus < Mui()); else complain(__LINE__);
	if (Mui() < **ppCus) complain(__LINE__);
	iequals(__LINE__, **ppCus < Ml(), 1);
	iequals(__LINE__, Ml() < **ppCus, 0);
	if (**ppCus < Ml()); else complain(__LINE__);
	if (Ml() < **ppCus) complain(__LINE__);
	iequals(__LINE__, **ppCus < Mul(), 1);
	iequals(__LINE__, Mul() < **ppCus, 0);
	if (**ppCus < Mul()); else complain(__LINE__);
	if (Mul() < **ppCus) complain(__LINE__);
	iequals(__LINE__, **ppCus < Mf(), 1);
	iequals(__LINE__, Mf() < **ppCus, 0);
	if (**ppCus < Mf()); else complain(__LINE__);
	if (Mf() < **ppCus) complain(__LINE__);
	iequals(__LINE__, **ppCus < Md(), 1);
	iequals(__LINE__, Md() < **ppCus, 0);
	if (**ppCus < Md()); else complain(__LINE__);
	if (Md() < **ppCus) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCus < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCus, 0);
	if (**ppCus < Mld()); else complain(__LINE__);
	if (Mld() < **ppCus) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCui < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCui, 1);
	if (**ppCui < Mc()) complain(__LINE__);
	if (Mc() < **ppCui); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCui < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCui, 1);
	if (**ppCui < Msc()) complain(__LINE__);
	if (Msc() < **ppCui); else complain(__LINE__);
#endif
	iequals(__LINE__, **ppCui < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCui, 1);
	if (**ppCui < Ms()) complain(__LINE__);
	if (Ms() < **ppCui); else complain(__LINE__);
	iequals(__LINE__, **ppCui < Mi(), 0);
	iequals(__LINE__, Mi() < **ppCui, 1);
	if (**ppCui < Mi()) complain(__LINE__);
	if (Mi() < **ppCui); else complain(__LINE__);
	iequals(__LINE__, **ppCui < Muc(), 0);
	iequals(__LINE__, Muc() < **ppCui, 1);
	if (**ppCui < Muc()) complain(__LINE__);
	if (Muc() < **ppCui); else complain(__LINE__);
	iequals(__LINE__, **ppCui < Mus(), 0);
	iequals(__LINE__, Mus() < **ppCui, 1);
	if (**ppCui < Mus()) complain(__LINE__);
	if (Mus() < **ppCui); else complain(__LINE__);
	iequals(__LINE__, **ppCui < Mui(), 0);
	iequals(__LINE__, Mui() < **ppCui, 0);
	if (**ppCui < Mui()) complain(__LINE__);
	if (Mui() < **ppCui) complain(__LINE__);
	iequals(__LINE__, **ppCui < Ml(), 1);
	iequals(__LINE__, Ml() < **ppCui, 0);
	if (**ppCui < Ml()); else complain(__LINE__);
	if (Ml() < **ppCui) complain(__LINE__);
	iequals(__LINE__, **ppCui < Mul(), 1);
	iequals(__LINE__, Mul() < **ppCui, 0);
	if (**ppCui < Mul()); else complain(__LINE__);
	if (Mul() < **ppCui) complain(__LINE__);
	iequals(__LINE__, **ppCui < Mf(), 1);
	iequals(__LINE__, Mf() < **ppCui, 0);
	if (**ppCui < Mf()); else complain(__LINE__);
	if (Mf() < **ppCui) complain(__LINE__);
	iequals(__LINE__, **ppCui < Md(), 1);
	iequals(__LINE__, Md() < **ppCui, 0);
	if (**ppCui < Md()); else complain(__LINE__);
	if (Md() < **ppCui) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCui < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCui, 0);
	if (**ppCui < Mld()); else complain(__LINE__);
	if (Mld() < **ppCui) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCl < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCl, 1);
	if (**ppCl < Mc()) complain(__LINE__);
	if (Mc() < **ppCl); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCl < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCl, 1);
	if (**ppCl < Msc()) complain(__LINE__);
	if (Msc() < **ppCl); else complain(__LINE__);
#endif
	iequals(__LINE__, **ppCl < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCl, 1);
	if (**ppCl < Ms()) complain(__LINE__);
	if (Ms() < **ppCl); else complain(__LINE__);
	iequals(__LINE__, **ppCl < Mi(), 0);
	iequals(__LINE__, Mi() < **ppCl, 1);
	if (**ppCl < Mi()) complain(__LINE__);
	if (Mi() < **ppCl); else complain(__LINE__);
	iequals(__LINE__, **ppCl < Muc(), 0);
	iequals(__LINE__, Muc() < **ppCl, 1);
	if (**ppCl < Muc()) complain(__LINE__);
	if (Muc() < **ppCl); else complain(__LINE__);
	iequals(__LINE__, **ppCl < Mus(), 0);
	iequals(__LINE__, Mus() < **ppCl, 1);
	if (**ppCl < Mus()) complain(__LINE__);
	if (Mus() < **ppCl); else complain(__LINE__);
	iequals(__LINE__, **ppCl < Mui(), 0);
	iequals(__LINE__, Mui() < **ppCl, 1);
	if (**ppCl < Mui()) complain(__LINE__);
	if (Mui() < **ppCl); else complain(__LINE__);
	iequals(__LINE__, **ppCl < Ml(), 0);
	iequals(__LINE__, Ml() < **ppCl, 0);
	if (**ppCl < Ml()) complain(__LINE__);
	if (Ml() < **ppCl) complain(__LINE__);
	iequals(__LINE__, **ppCl < Mul(), 1);
	iequals(__LINE__, Mul() < **ppCl, 0);
	if (**ppCl < Mul()); else complain(__LINE__);
	if (Mul() < **ppCl) complain(__LINE__);
	iequals(__LINE__, **ppCl < Mf(), 1);
	iequals(__LINE__, Mf() < **ppCl, 0);
	if (**ppCl < Mf()); else complain(__LINE__);
	if (Mf() < **ppCl) complain(__LINE__);
	iequals(__LINE__, **ppCl < Md(), 1);
	iequals(__LINE__, Md() < **ppCl, 0);
	if (**ppCl < Md()); else complain(__LINE__);
	if (Md() < **ppCl) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCl < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCl, 0);
	if (**ppCl < Mld()); else complain(__LINE__);
	if (Mld() < **ppCl) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCul < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCul, 1);
	if (**ppCul < Mc()) complain(__LINE__);
	if (Mc() < **ppCul); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCul < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCul, 1);
	if (**ppCul < Msc()) complain(__LINE__);
	if (Msc() < **ppCul); else complain(__LINE__);
#endif
	iequals(__LINE__, **ppCul < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCul, 1);
	if (**ppCul < Ms()) complain(__LINE__);
	if (Ms() < **ppCul); else complain(__LINE__);
	iequals(__LINE__, **ppCul < Mi(), 0);
	iequals(__LINE__, Mi() < **ppCul, 1);
	if (**ppCul < Mi()) complain(__LINE__);
	if (Mi() < **ppCul); else complain(__LINE__);
	iequals(__LINE__, **ppCul < Muc(), 0);
	iequals(__LINE__, Muc() < **ppCul, 1);
	if (**ppCul < Muc()) complain(__LINE__);
	if (Muc() < **ppCul); else complain(__LINE__);
	iequals(__LINE__, **ppCul < Mus(), 0);
	iequals(__LINE__, Mus() < **ppCul, 1);
	if (**ppCul < Mus()) complain(__LINE__);
	if (Mus() < **ppCul); else complain(__LINE__);
	iequals(__LINE__, **ppCul < Mui(), 0);
	iequals(__LINE__, Mui() < **ppCul, 1);
	if (**ppCul < Mui()) complain(__LINE__);
	if (Mui() < **ppCul); else complain(__LINE__);
	iequals(__LINE__, **ppCul < Ml(), 0);
	iequals(__LINE__, Ml() < **ppCul, 1);
	if (**ppCul < Ml()) complain(__LINE__);
	if (Ml() < **ppCul); else complain(__LINE__);
	iequals(__LINE__, **ppCul < Mul(), 0);
	iequals(__LINE__, Mul() < **ppCul, 0);
	if (**ppCul < Mul()) complain(__LINE__);
	if (Mul() < **ppCul) complain(__LINE__);
	iequals(__LINE__, **ppCul < Mf(), 1);
	iequals(__LINE__, Mf() < **ppCul, 0);
	if (**ppCul < Mf()); else complain(__LINE__);
	if (Mf() < **ppCul) complain(__LINE__);
	iequals(__LINE__, **ppCul < Md(), 1);
	iequals(__LINE__, Md() < **ppCul, 0);
	if (**ppCul < Md()); else complain(__LINE__);
	if (Md() < **ppCul) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCul < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCul, 0);
	if (**ppCul < Mld()); else complain(__LINE__);
	if (Mld() < **ppCul) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCf < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCf, 1);
	if (**ppCf < Mc()) complain(__LINE__);
	if (Mc() < **ppCf); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCf < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCf, 1);
	if (**ppCf < Msc()) complain(__LINE__);
	if (Msc() < **ppCf); else complain(__LINE__);
#endif
	iequals(__LINE__, **ppCf < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCf, 1);
	if (**ppCf < Ms()) complain(__LINE__);
	if (Ms() < **ppCf); else complain(__LINE__);
	iequals(__LINE__, **ppCf < Mi(), 0);
	iequals(__LINE__, Mi() < **ppCf, 1);
	if (**ppCf < Mi()) complain(__LINE__);
	if (Mi() < **ppCf); else complain(__LINE__);
	iequals(__LINE__, **ppCf < Muc(), 0);
	iequals(__LINE__, Muc() < **ppCf, 1);
	if (**ppCf < Muc()) complain(__LINE__);
	if (Muc() < **ppCf); else complain(__LINE__);
	iequals(__LINE__, **ppCf < Mus(), 0);
	iequals(__LINE__, Mus() < **ppCf, 1);
	if (**ppCf < Mus()) complain(__LINE__);
	if (Mus() < **ppCf); else complain(__LINE__);
	iequals(__LINE__, **ppCf < Mui(), 0);
	iequals(__LINE__, Mui() < **ppCf, 1);
	if (**ppCf < Mui()) complain(__LINE__);
	if (Mui() < **ppCf); else complain(__LINE__);
	iequals(__LINE__, **ppCf < Ml(), 0);
	iequals(__LINE__, Ml() < **ppCf, 1);
	if (**ppCf < Ml()) complain(__LINE__);
	if (Ml() < **ppCf); else complain(__LINE__);
	iequals(__LINE__, **ppCf < Mul(), 0);
	iequals(__LINE__, Mul() < **ppCf, 1);
	if (**ppCf < Mul()) complain(__LINE__);
	if (Mul() < **ppCf); else complain(__LINE__);
	iequals(__LINE__, **ppCf < Mf(), 0);
	iequals(__LINE__, Mf() < **ppCf, 0);
	if (**ppCf < Mf()) complain(__LINE__);
	if (Mf() < **ppCf) complain(__LINE__);
	iequals(__LINE__, **ppCf < Md(), 1);
	iequals(__LINE__, Md() < **ppCf, 0);
	if (**ppCf < Md()); else complain(__LINE__);
	if (Md() < **ppCf) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCf < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCf, 0);
	if (**ppCf < Mld()); else complain(__LINE__);
	if (Mld() < **ppCf) complain(__LINE__);
#endif
	iequals(__LINE__, **ppCd < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCd, 1);
	if (**ppCd < Mc()) complain(__LINE__);
	if (Mc() < **ppCd); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCd < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCd, 1);
	if (**ppCd < Msc()) complain(__LINE__);
	if (Msc() < **ppCd); else complain(__LINE__);
#endif
	iequals(__LINE__, **ppCd < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCd, 1);
	if (**ppCd < Ms()) complain(__LINE__);
	if (Ms() < **ppCd); else complain(__LINE__);
	iequals(__LINE__, **ppCd < Mi(), 0);
	iequals(__LINE__, Mi() < **ppCd, 1);
	if (**ppCd < Mi()) complain(__LINE__);
	if (Mi() < **ppCd); else complain(__LINE__);
	iequals(__LINE__, **ppCd < Muc(), 0);
	iequals(__LINE__, Muc() < **ppCd, 1);
	if (**ppCd < Muc()) complain(__LINE__);
	if (Muc() < **ppCd); else complain(__LINE__);
	iequals(__LINE__, **ppCd < Mus(), 0);
	iequals(__LINE__, Mus() < **ppCd, 1);
	if (**ppCd < Mus()) complain(__LINE__);
	if (Mus() < **ppCd); else complain(__LINE__);
	iequals(__LINE__, **ppCd < Mui(), 0);
	iequals(__LINE__, Mui() < **ppCd, 1);
	if (**ppCd < Mui()) complain(__LINE__);
	if (Mui() < **ppCd); else complain(__LINE__);
	iequals(__LINE__, **ppCd < Ml(), 0);
	iequals(__LINE__, Ml() < **ppCd, 1);
	if (**ppCd < Ml()) complain(__LINE__);
	if (Ml() < **ppCd); else complain(__LINE__);
	iequals(__LINE__, **ppCd < Mul(), 0);
	iequals(__LINE__, Mul() < **ppCd, 1);
	if (**ppCd < Mul()) complain(__LINE__);
	if (Mul() < **ppCd); else complain(__LINE__);
	iequals(__LINE__, **ppCd < Mf(), 0);
	iequals(__LINE__, Mf() < **ppCd, 1);
	if (**ppCd < Mf()) complain(__LINE__);
	if (Mf() < **ppCd); else complain(__LINE__);
	iequals(__LINE__, **ppCd < Md(), 0);
	iequals(__LINE__, Md() < **ppCd, 0);
	if (**ppCd < Md()) complain(__LINE__);
	if (Md() < **ppCd) complain(__LINE__);
#if ANSI
	iequals(__LINE__, **ppCd < Mld(), 1);
	iequals(__LINE__, Mld() < **ppCd, 0);
	if (**ppCd < Mld()); else complain(__LINE__);
	if (Mld() < **ppCd) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Mc(), 0);
	iequals(__LINE__, Mc() < **ppCld, 1);
	if (**ppCld < Mc()) complain(__LINE__);
	if (Mc() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Msc(), 0);
	iequals(__LINE__, Msc() < **ppCld, 1);
	if (**ppCld < Msc()) complain(__LINE__);
	if (Msc() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Ms(), 0);
	iequals(__LINE__, Ms() < **ppCld, 1);
	if (**ppCld < Ms()) complain(__LINE__);
	if (Ms() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Mi(), 0);
	iequals(__LINE__, Mi() < **ppCld, 1);
	if (**ppCld < Mi()) complain(__LINE__);
	if (Mi() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Muc(), 0);
	iequals(__LINE__, Muc() < **ppCld, 1);
	if (**ppCld < Muc()) complain(__LINE__);
	if (Muc() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Mus(), 0);
	iequals(__LINE__, Mus() < **ppCld, 1);
	if (**ppCld < Mus()) complain(__LINE__);
	if (Mus() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Mui(), 0);
	iequals(__LINE__, Mui() < **ppCld, 1);
	if (**ppCld < Mui()) complain(__LINE__);
	if (Mui() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Ml(), 0);
	iequals(__LINE__, Ml() < **ppCld, 1);
	if (**ppCld < Ml()) complain(__LINE__);
	if (Ml() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Mul(), 0);
	iequals(__LINE__, Mul() < **ppCld, 1);
	if (**ppCld < Mul()) complain(__LINE__);
	if (Mul() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Mf(), 0);
	iequals(__LINE__, Mf() < **ppCld, 1);
	if (**ppCld < Mf()) complain(__LINE__);
	if (Mf() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Md(), 0);
	iequals(__LINE__, Md() < **ppCld, 1);
	if (**ppCld < Md()) complain(__LINE__);
	if (Md() < **ppCld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, **ppCld < Mld(), 0);
	iequals(__LINE__, Mld() < **ppCld, 0);
	if (**ppCld < Mld()) complain(__LINE__);
	if (Mld() < **ppCld) complain(__LINE__);
#endif
	}}
	return 	report(Filename);
	}
