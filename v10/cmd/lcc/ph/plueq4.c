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
#include "n.h"
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
	CHAR *Nc();
#if ANSI
	SCHAR *Nsc();
#endif
	SHORT *Ns();
	INT *Ni();
	UCHAR *Nuc();
	USHORT *Nus();
	UINT *Nui();
	LONG *Nl();
	ULONG *Nul();
	FLOAT *Nf();
	DOUBLE *Nd();
#if ANSI
	LDOUBLE *Nld();
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto pscalar2 auto funcrp plueq4 ";
	**ppCf = 16; *Nc() = 7;
	**ppCf += *Nc();
	dequals(__LINE__, **ppCf, (FLOAT)23.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nc(), (FLOAT)23.);
	*Nc() = 7; **ppCf = 16;
	*Nc() += **ppCf;
	iequals(__LINE__, *Nc(), (CHAR)23);
	*Nc() = 7;
	iequals(__LINE__, *Nc() += **ppCf, (CHAR)23);
#if ANSI
	**ppCf = 16; *Nsc() = 8;
	**ppCf += *Nsc();
	dequals(__LINE__, **ppCf, (FLOAT)24.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nsc(), (FLOAT)24.);
	*Nsc() = 8; **ppCf = 16;
	*Nsc() += **ppCf;
	iequals(__LINE__, *Nsc(), (SCHAR)24);
	*Nsc() = 8;
	iequals(__LINE__, *Nsc() += **ppCf, (SCHAR)24);
#endif
	**ppCf = 16; *Ns() = 9;
	**ppCf += *Ns();
	dequals(__LINE__, **ppCf, (FLOAT)25.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Ns(), (FLOAT)25.);
	*Ns() = 9; **ppCf = 16;
	*Ns() += **ppCf;
	iequals(__LINE__, *Ns(), (SHORT)25);
	*Ns() = 9;
	iequals(__LINE__, *Ns() += **ppCf, (SHORT)25);
	**ppCf = 16; *Ni() = 10;
	**ppCf += *Ni();
	dequals(__LINE__, **ppCf, (FLOAT)26.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Ni(), (FLOAT)26.);
	*Ni() = 10; **ppCf = 16;
	*Ni() += **ppCf;
	iequals(__LINE__, *Ni(), (INT)26);
	*Ni() = 10;
	iequals(__LINE__, *Ni() += **ppCf, (INT)26);
	**ppCf = 16; *Nuc() = 11;
	**ppCf += *Nuc();
	dequals(__LINE__, **ppCf, (FLOAT)27.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nuc(), (FLOAT)27.);
	*Nuc() = 11; **ppCf = 16;
	*Nuc() += **ppCf;
	iequals(__LINE__, *Nuc(), (UCHAR)27);
	*Nuc() = 11;
	iequals(__LINE__, *Nuc() += **ppCf, (UCHAR)27);
	**ppCf = 16; *Nus() = 12;
	**ppCf += *Nus();
	dequals(__LINE__, **ppCf, (FLOAT)28.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nus(), (FLOAT)28.);
	*Nus() = 12; **ppCf = 16;
	*Nus() += **ppCf;
	iequals(__LINE__, *Nus(), (USHORT)28);
	*Nus() = 12;
	iequals(__LINE__, *Nus() += **ppCf, (USHORT)28);
	**ppCf = 16; *Nui() = 13;
	**ppCf += *Nui();
	dequals(__LINE__, **ppCf, (FLOAT)29.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nui(), (FLOAT)29.);
	*Nui() = 13; **ppCf = 16;
	*Nui() += **ppCf;
	iequals(__LINE__, *Nui(), (UINT)29);
	*Nui() = 13;
	iequals(__LINE__, *Nui() += **ppCf, (UINT)29);
	**ppCf = 16; *Nl() = 14;
	**ppCf += *Nl();
	dequals(__LINE__, **ppCf, (FLOAT)30.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nl(), (FLOAT)30.);
	*Nl() = 14; **ppCf = 16;
	*Nl() += **ppCf;
	lequals(__LINE__, *Nl(), (LONG)30L);
	*Nl() = 14;
	lequals(__LINE__, *Nl() += **ppCf, (LONG)30L);
	**ppCf = 16; *Nul() = 15;
	**ppCf += *Nul();
	dequals(__LINE__, **ppCf, (FLOAT)31.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nul(), (FLOAT)31.);
	*Nul() = 15; **ppCf = 16;
	*Nul() += **ppCf;
	lequals(__LINE__, *Nul(), (ULONG)31L);
	*Nul() = 15;
	lequals(__LINE__, *Nul() += **ppCf, (ULONG)31L);
	**ppCf = 16; *Nf() = 16;
	**ppCf += *Nf();
	dequals(__LINE__, **ppCf, (FLOAT)32.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nf(), (FLOAT)32.);
	*Nf() = 16; **ppCf = 16;
	*Nf() += **ppCf;
	dequals(__LINE__, *Nf(), (FLOAT)32.);
	*Nf() = 16;
	dequals(__LINE__, *Nf() += **ppCf, (FLOAT)32.);
	**ppCf = 16; *Nd() = 17;
	**ppCf += *Nd();
	dequals(__LINE__, **ppCf, (FLOAT)33.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nd(), (FLOAT)33.);
	*Nd() = 17; **ppCf = 16;
	*Nd() += **ppCf;
	dequals(__LINE__, *Nd(), (DOUBLE)33.);
	*Nd() = 17;
	dequals(__LINE__, *Nd() += **ppCf, (DOUBLE)33.);
#if ANSI
	**ppCf = 16; *Nld() = 18;
	**ppCf += *Nld();
	dequals(__LINE__, **ppCf, (FLOAT)34.);
	**ppCf = 16;
	dequals(__LINE__, **ppCf += *Nld(), (FLOAT)34.);
	*Nld() = 18; **ppCf = 16;
	*Nld() += **ppCf;
	ldequals(__LINE__, *Nld(), (LDOUBLE)34.L);
	*Nld() = 18;
	ldequals(__LINE__, *Nld() += **ppCf, (LDOUBLE)34.L);
#endif
	**ppCd = 17; *Nc() = 7;
	**ppCd += *Nc();
	dequals(__LINE__, **ppCd, (DOUBLE)24.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nc(), (DOUBLE)24.);
	*Nc() = 7; **ppCd = 17;
	*Nc() += **ppCd;
	iequals(__LINE__, *Nc(), (CHAR)24);
	*Nc() = 7;
	iequals(__LINE__, *Nc() += **ppCd, (CHAR)24);
#if ANSI
	**ppCd = 17; *Nsc() = 8;
	**ppCd += *Nsc();
	dequals(__LINE__, **ppCd, (DOUBLE)25.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nsc(), (DOUBLE)25.);
	*Nsc() = 8; **ppCd = 17;
	*Nsc() += **ppCd;
	iequals(__LINE__, *Nsc(), (SCHAR)25);
	*Nsc() = 8;
	iequals(__LINE__, *Nsc() += **ppCd, (SCHAR)25);
#endif
	**ppCd = 17; *Ns() = 9;
	**ppCd += *Ns();
	dequals(__LINE__, **ppCd, (DOUBLE)26.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Ns(), (DOUBLE)26.);
	*Ns() = 9; **ppCd = 17;
	*Ns() += **ppCd;
	iequals(__LINE__, *Ns(), (SHORT)26);
	*Ns() = 9;
	iequals(__LINE__, *Ns() += **ppCd, (SHORT)26);
	**ppCd = 17; *Ni() = 10;
	**ppCd += *Ni();
	dequals(__LINE__, **ppCd, (DOUBLE)27.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Ni(), (DOUBLE)27.);
	*Ni() = 10; **ppCd = 17;
	*Ni() += **ppCd;
	iequals(__LINE__, *Ni(), (INT)27);
	*Ni() = 10;
	iequals(__LINE__, *Ni() += **ppCd, (INT)27);
	**ppCd = 17; *Nuc() = 11;
	**ppCd += *Nuc();
	dequals(__LINE__, **ppCd, (DOUBLE)28.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nuc(), (DOUBLE)28.);
	*Nuc() = 11; **ppCd = 17;
	*Nuc() += **ppCd;
	iequals(__LINE__, *Nuc(), (UCHAR)28);
	*Nuc() = 11;
	iequals(__LINE__, *Nuc() += **ppCd, (UCHAR)28);
	**ppCd = 17; *Nus() = 12;
	**ppCd += *Nus();
	dequals(__LINE__, **ppCd, (DOUBLE)29.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nus(), (DOUBLE)29.);
	*Nus() = 12; **ppCd = 17;
	*Nus() += **ppCd;
	iequals(__LINE__, *Nus(), (USHORT)29);
	*Nus() = 12;
	iequals(__LINE__, *Nus() += **ppCd, (USHORT)29);
	**ppCd = 17; *Nui() = 13;
	**ppCd += *Nui();
	dequals(__LINE__, **ppCd, (DOUBLE)30.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nui(), (DOUBLE)30.);
	*Nui() = 13; **ppCd = 17;
	*Nui() += **ppCd;
	iequals(__LINE__, *Nui(), (UINT)30);
	*Nui() = 13;
	iequals(__LINE__, *Nui() += **ppCd, (UINT)30);
	**ppCd = 17; *Nl() = 14;
	**ppCd += *Nl();
	dequals(__LINE__, **ppCd, (DOUBLE)31.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nl(), (DOUBLE)31.);
	*Nl() = 14; **ppCd = 17;
	*Nl() += **ppCd;
	lequals(__LINE__, *Nl(), (LONG)31L);
	*Nl() = 14;
	lequals(__LINE__, *Nl() += **ppCd, (LONG)31L);
	**ppCd = 17; *Nul() = 15;
	**ppCd += *Nul();
	dequals(__LINE__, **ppCd, (DOUBLE)32.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nul(), (DOUBLE)32.);
	*Nul() = 15; **ppCd = 17;
	*Nul() += **ppCd;
	lequals(__LINE__, *Nul(), (ULONG)32L);
	*Nul() = 15;
	lequals(__LINE__, *Nul() += **ppCd, (ULONG)32L);
	**ppCd = 17; *Nf() = 16;
	**ppCd += *Nf();
	dequals(__LINE__, **ppCd, (DOUBLE)33.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nf(), (DOUBLE)33.);
	*Nf() = 16; **ppCd = 17;
	*Nf() += **ppCd;
	dequals(__LINE__, *Nf(), (FLOAT)33.);
	*Nf() = 16;
	dequals(__LINE__, *Nf() += **ppCd, (FLOAT)33.);
	**ppCd = 17; *Nd() = 17;
	**ppCd += *Nd();
	dequals(__LINE__, **ppCd, (DOUBLE)34.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nd(), (DOUBLE)34.);
	*Nd() = 17; **ppCd = 17;
	*Nd() += **ppCd;
	dequals(__LINE__, *Nd(), (DOUBLE)34.);
	*Nd() = 17;
	dequals(__LINE__, *Nd() += **ppCd, (DOUBLE)34.);
#if ANSI
	**ppCd = 17; *Nld() = 18;
	**ppCd += *Nld();
	dequals(__LINE__, **ppCd, (DOUBLE)35.);
	**ppCd = 17;
	dequals(__LINE__, **ppCd += *Nld(), (DOUBLE)35.);
	*Nld() = 18; **ppCd = 17;
	*Nld() += **ppCd;
	ldequals(__LINE__, *Nld(), (LDOUBLE)35.L);
	*Nld() = 18;
	ldequals(__LINE__, *Nld() += **ppCd, (LDOUBLE)35.L);
#endif
#if ANSI
	**ppCld = 18; *Nc() = 7;
	**ppCld += *Nc();
	ldequals(__LINE__, **ppCld, (LDOUBLE)25.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nc(), (LDOUBLE)25.L);
	*Nc() = 7; **ppCld = 18;
	*Nc() += **ppCld;
	iequals(__LINE__, *Nc(), (CHAR)25);
	*Nc() = 7;
	iequals(__LINE__, *Nc() += **ppCld, (CHAR)25);
#endif
#if ANSI
	**ppCld = 18; *Nsc() = 8;
	**ppCld += *Nsc();
	ldequals(__LINE__, **ppCld, (LDOUBLE)26.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nsc(), (LDOUBLE)26.L);
	*Nsc() = 8; **ppCld = 18;
	*Nsc() += **ppCld;
	iequals(__LINE__, *Nsc(), (SCHAR)26);
	*Nsc() = 8;
	iequals(__LINE__, *Nsc() += **ppCld, (SCHAR)26);
#endif
#if ANSI
	**ppCld = 18; *Ns() = 9;
	**ppCld += *Ns();
	ldequals(__LINE__, **ppCld, (LDOUBLE)27.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Ns(), (LDOUBLE)27.L);
	*Ns() = 9; **ppCld = 18;
	*Ns() += **ppCld;
	iequals(__LINE__, *Ns(), (SHORT)27);
	*Ns() = 9;
	iequals(__LINE__, *Ns() += **ppCld, (SHORT)27);
#endif
#if ANSI
	**ppCld = 18; *Ni() = 10;
	**ppCld += *Ni();
	ldequals(__LINE__, **ppCld, (LDOUBLE)28.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Ni(), (LDOUBLE)28.L);
	*Ni() = 10; **ppCld = 18;
	*Ni() += **ppCld;
	iequals(__LINE__, *Ni(), (INT)28);
	*Ni() = 10;
	iequals(__LINE__, *Ni() += **ppCld, (INT)28);
#endif
#if ANSI
	**ppCld = 18; *Nuc() = 11;
	**ppCld += *Nuc();
	ldequals(__LINE__, **ppCld, (LDOUBLE)29.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nuc(), (LDOUBLE)29.L);
	*Nuc() = 11; **ppCld = 18;
	*Nuc() += **ppCld;
	iequals(__LINE__, *Nuc(), (UCHAR)29);
	*Nuc() = 11;
	iequals(__LINE__, *Nuc() += **ppCld, (UCHAR)29);
#endif
#if ANSI
	**ppCld = 18; *Nus() = 12;
	**ppCld += *Nus();
	ldequals(__LINE__, **ppCld, (LDOUBLE)30.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nus(), (LDOUBLE)30.L);
	*Nus() = 12; **ppCld = 18;
	*Nus() += **ppCld;
	iequals(__LINE__, *Nus(), (USHORT)30);
	*Nus() = 12;
	iequals(__LINE__, *Nus() += **ppCld, (USHORT)30);
#endif
#if ANSI
	**ppCld = 18; *Nui() = 13;
	**ppCld += *Nui();
	ldequals(__LINE__, **ppCld, (LDOUBLE)31.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nui(), (LDOUBLE)31.L);
	*Nui() = 13; **ppCld = 18;
	*Nui() += **ppCld;
	iequals(__LINE__, *Nui(), (UINT)31);
	*Nui() = 13;
	iequals(__LINE__, *Nui() += **ppCld, (UINT)31);
#endif
#if ANSI
	**ppCld = 18; *Nl() = 14;
	**ppCld += *Nl();
	ldequals(__LINE__, **ppCld, (LDOUBLE)32.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nl(), (LDOUBLE)32.L);
	*Nl() = 14; **ppCld = 18;
	*Nl() += **ppCld;
	lequals(__LINE__, *Nl(), (LONG)32L);
	*Nl() = 14;
	lequals(__LINE__, *Nl() += **ppCld, (LONG)32L);
#endif
#if ANSI
	**ppCld = 18; *Nul() = 15;
	**ppCld += *Nul();
	ldequals(__LINE__, **ppCld, (LDOUBLE)33.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nul(), (LDOUBLE)33.L);
	*Nul() = 15; **ppCld = 18;
	*Nul() += **ppCld;
	lequals(__LINE__, *Nul(), (ULONG)33L);
	*Nul() = 15;
	lequals(__LINE__, *Nul() += **ppCld, (ULONG)33L);
#endif
#if ANSI
	**ppCld = 18; *Nf() = 16;
	**ppCld += *Nf();
	ldequals(__LINE__, **ppCld, (LDOUBLE)34.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nf(), (LDOUBLE)34.L);
	*Nf() = 16; **ppCld = 18;
	*Nf() += **ppCld;
	dequals(__LINE__, *Nf(), (FLOAT)34.);
	*Nf() = 16;
	dequals(__LINE__, *Nf() += **ppCld, (FLOAT)34.);
#endif
#if ANSI
	**ppCld = 18; *Nd() = 17;
	**ppCld += *Nd();
	ldequals(__LINE__, **ppCld, (LDOUBLE)35.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nd(), (LDOUBLE)35.L);
	*Nd() = 17; **ppCld = 18;
	*Nd() += **ppCld;
	dequals(__LINE__, *Nd(), (DOUBLE)35.);
	*Nd() = 17;
	dequals(__LINE__, *Nd() += **ppCld, (DOUBLE)35.);
#endif
#if ANSI
	**ppCld = 18; *Nld() = 18;
	**ppCld += *Nld();
	ldequals(__LINE__, **ppCld, (LDOUBLE)36.L);
	**ppCld = 18;
	ldequals(__LINE__, **ppCld += *Nld(), (LDOUBLE)36.L);
	*Nld() = 18; **ppCld = 18;
	*Nld() += **ppCld;
	ldequals(__LINE__, *Nld(), (LDOUBLE)36.L);
	*Nld() = 18;
	ldequals(__LINE__, *Nld() += **ppCld, (LDOUBLE)36.L);
#endif
	}}
	return 	report(Filename);
	}
