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
	Filename =  " auto pscalar2 auto funcrp plueq1 ";
	**ppCc = 7; *Nc() = 7;
	**ppCc += *Nc();
	iequals(__LINE__, **ppCc, (CHAR)14);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nc(), (CHAR)14);
	*Nc() = 7; **ppCc = 7;
	*Nc() += **ppCc;
	iequals(__LINE__, *Nc(), (CHAR)14);
	*Nc() = 7;
	iequals(__LINE__, *Nc() += **ppCc, (CHAR)14);
#if ANSI
	**ppCc = 7; *Nsc() = 8;
	**ppCc += *Nsc();
	iequals(__LINE__, **ppCc, (CHAR)15);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nsc(), (CHAR)15);
	*Nsc() = 8; **ppCc = 7;
	*Nsc() += **ppCc;
	iequals(__LINE__, *Nsc(), (SCHAR)15);
	*Nsc() = 8;
	iequals(__LINE__, *Nsc() += **ppCc, (SCHAR)15);
#endif
	**ppCc = 7; *Ns() = 9;
	**ppCc += *Ns();
	iequals(__LINE__, **ppCc, (CHAR)16);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Ns(), (CHAR)16);
	*Ns() = 9; **ppCc = 7;
	*Ns() += **ppCc;
	iequals(__LINE__, *Ns(), (SHORT)16);
	*Ns() = 9;
	iequals(__LINE__, *Ns() += **ppCc, (SHORT)16);
	**ppCc = 7; *Ni() = 10;
	**ppCc += *Ni();
	iequals(__LINE__, **ppCc, (CHAR)17);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Ni(), (CHAR)17);
	*Ni() = 10; **ppCc = 7;
	*Ni() += **ppCc;
	iequals(__LINE__, *Ni(), (INT)17);
	*Ni() = 10;
	iequals(__LINE__, *Ni() += **ppCc, (INT)17);
	**ppCc = 7; *Nuc() = 11;
	**ppCc += *Nuc();
	iequals(__LINE__, **ppCc, (CHAR)18);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nuc(), (CHAR)18);
	*Nuc() = 11; **ppCc = 7;
	*Nuc() += **ppCc;
	iequals(__LINE__, *Nuc(), (UCHAR)18);
	*Nuc() = 11;
	iequals(__LINE__, *Nuc() += **ppCc, (UCHAR)18);
	**ppCc = 7; *Nus() = 12;
	**ppCc += *Nus();
	iequals(__LINE__, **ppCc, (CHAR)19);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nus(), (CHAR)19);
	*Nus() = 12; **ppCc = 7;
	*Nus() += **ppCc;
	iequals(__LINE__, *Nus(), (USHORT)19);
	*Nus() = 12;
	iequals(__LINE__, *Nus() += **ppCc, (USHORT)19);
	**ppCc = 7; *Nui() = 13;
	**ppCc += *Nui();
	iequals(__LINE__, **ppCc, (CHAR)20);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nui(), (CHAR)20);
	*Nui() = 13; **ppCc = 7;
	*Nui() += **ppCc;
	iequals(__LINE__, *Nui(), (UINT)20);
	*Nui() = 13;
	iequals(__LINE__, *Nui() += **ppCc, (UINT)20);
	**ppCc = 7; *Nl() = 14;
	**ppCc += *Nl();
	iequals(__LINE__, **ppCc, (CHAR)21);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nl(), (CHAR)21);
	*Nl() = 14; **ppCc = 7;
	*Nl() += **ppCc;
	lequals(__LINE__, *Nl(), (LONG)21L);
	*Nl() = 14;
	lequals(__LINE__, *Nl() += **ppCc, (LONG)21L);
	**ppCc = 7; *Nul() = 15;
	**ppCc += *Nul();
	iequals(__LINE__, **ppCc, (CHAR)22);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nul(), (CHAR)22);
	*Nul() = 15; **ppCc = 7;
	*Nul() += **ppCc;
	lequals(__LINE__, *Nul(), (ULONG)22L);
	*Nul() = 15;
	lequals(__LINE__, *Nul() += **ppCc, (ULONG)22L);
	**ppCc = 7; *Nf() = 16;
	**ppCc += *Nf();
	iequals(__LINE__, **ppCc, (CHAR)23);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nf(), (CHAR)23);
	*Nf() = 16; **ppCc = 7;
	*Nf() += **ppCc;
	dequals(__LINE__, *Nf(), (FLOAT)23.);
	*Nf() = 16;
	dequals(__LINE__, *Nf() += **ppCc, (FLOAT)23.);
	**ppCc = 7; *Nd() = 17;
	**ppCc += *Nd();
	iequals(__LINE__, **ppCc, (CHAR)24);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nd(), (CHAR)24);
	*Nd() = 17; **ppCc = 7;
	*Nd() += **ppCc;
	dequals(__LINE__, *Nd(), (DOUBLE)24.);
	*Nd() = 17;
	dequals(__LINE__, *Nd() += **ppCc, (DOUBLE)24.);
#if ANSI
	**ppCc = 7; *Nld() = 18;
	**ppCc += *Nld();
	iequals(__LINE__, **ppCc, (CHAR)25);
	**ppCc = 7;
	iequals(__LINE__, **ppCc += *Nld(), (CHAR)25);
	*Nld() = 18; **ppCc = 7;
	*Nld() += **ppCc;
	ldequals(__LINE__, *Nld(), (LDOUBLE)25.L);
	*Nld() = 18;
	ldequals(__LINE__, *Nld() += **ppCc, (LDOUBLE)25.L);
#endif
#if ANSI
	**ppCsc = 8; *Nc() = 7;
	**ppCsc += *Nc();
	iequals(__LINE__, **ppCsc, (SCHAR)15);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nc(), (SCHAR)15);
	*Nc() = 7; **ppCsc = 8;
	*Nc() += **ppCsc;
	iequals(__LINE__, *Nc(), (CHAR)15);
	*Nc() = 7;
	iequals(__LINE__, *Nc() += **ppCsc, (CHAR)15);
#endif
#if ANSI
	**ppCsc = 8; *Nsc() = 8;
	**ppCsc += *Nsc();
	iequals(__LINE__, **ppCsc, (SCHAR)16);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nsc(), (SCHAR)16);
	*Nsc() = 8; **ppCsc = 8;
	*Nsc() += **ppCsc;
	iequals(__LINE__, *Nsc(), (SCHAR)16);
	*Nsc() = 8;
	iequals(__LINE__, *Nsc() += **ppCsc, (SCHAR)16);
#endif
#if ANSI
	**ppCsc = 8; *Ns() = 9;
	**ppCsc += *Ns();
	iequals(__LINE__, **ppCsc, (SCHAR)17);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Ns(), (SCHAR)17);
	*Ns() = 9; **ppCsc = 8;
	*Ns() += **ppCsc;
	iequals(__LINE__, *Ns(), (SHORT)17);
	*Ns() = 9;
	iequals(__LINE__, *Ns() += **ppCsc, (SHORT)17);
#endif
#if ANSI
	**ppCsc = 8; *Ni() = 10;
	**ppCsc += *Ni();
	iequals(__LINE__, **ppCsc, (SCHAR)18);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Ni(), (SCHAR)18);
	*Ni() = 10; **ppCsc = 8;
	*Ni() += **ppCsc;
	iequals(__LINE__, *Ni(), (INT)18);
	*Ni() = 10;
	iequals(__LINE__, *Ni() += **ppCsc, (INT)18);
#endif
#if ANSI
	**ppCsc = 8; *Nuc() = 11;
	**ppCsc += *Nuc();
	iequals(__LINE__, **ppCsc, (SCHAR)19);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nuc(), (SCHAR)19);
	*Nuc() = 11; **ppCsc = 8;
	*Nuc() += **ppCsc;
	iequals(__LINE__, *Nuc(), (UCHAR)19);
	*Nuc() = 11;
	iequals(__LINE__, *Nuc() += **ppCsc, (UCHAR)19);
#endif
#if ANSI
	**ppCsc = 8; *Nus() = 12;
	**ppCsc += *Nus();
	iequals(__LINE__, **ppCsc, (SCHAR)20);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nus(), (SCHAR)20);
	*Nus() = 12; **ppCsc = 8;
	*Nus() += **ppCsc;
	iequals(__LINE__, *Nus(), (USHORT)20);
	*Nus() = 12;
	iequals(__LINE__, *Nus() += **ppCsc, (USHORT)20);
#endif
#if ANSI
	**ppCsc = 8; *Nui() = 13;
	**ppCsc += *Nui();
	iequals(__LINE__, **ppCsc, (SCHAR)21);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nui(), (SCHAR)21);
	*Nui() = 13; **ppCsc = 8;
	*Nui() += **ppCsc;
	iequals(__LINE__, *Nui(), (UINT)21);
	*Nui() = 13;
	iequals(__LINE__, *Nui() += **ppCsc, (UINT)21);
#endif
#if ANSI
	**ppCsc = 8; *Nl() = 14;
	**ppCsc += *Nl();
	iequals(__LINE__, **ppCsc, (SCHAR)22);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nl(), (SCHAR)22);
	*Nl() = 14; **ppCsc = 8;
	*Nl() += **ppCsc;
	lequals(__LINE__, *Nl(), (LONG)22L);
	*Nl() = 14;
	lequals(__LINE__, *Nl() += **ppCsc, (LONG)22L);
#endif
#if ANSI
	**ppCsc = 8; *Nul() = 15;
	**ppCsc += *Nul();
	iequals(__LINE__, **ppCsc, (SCHAR)23);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nul(), (SCHAR)23);
	*Nul() = 15; **ppCsc = 8;
	*Nul() += **ppCsc;
	lequals(__LINE__, *Nul(), (ULONG)23L);
	*Nul() = 15;
	lequals(__LINE__, *Nul() += **ppCsc, (ULONG)23L);
#endif
#if ANSI
	**ppCsc = 8; *Nf() = 16;
	**ppCsc += *Nf();
	iequals(__LINE__, **ppCsc, (SCHAR)24);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nf(), (SCHAR)24);
	*Nf() = 16; **ppCsc = 8;
	*Nf() += **ppCsc;
	dequals(__LINE__, *Nf(), (FLOAT)24.);
	*Nf() = 16;
	dequals(__LINE__, *Nf() += **ppCsc, (FLOAT)24.);
#endif
#if ANSI
	**ppCsc = 8; *Nd() = 17;
	**ppCsc += *Nd();
	iequals(__LINE__, **ppCsc, (SCHAR)25);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nd(), (SCHAR)25);
	*Nd() = 17; **ppCsc = 8;
	*Nd() += **ppCsc;
	dequals(__LINE__, *Nd(), (DOUBLE)25.);
	*Nd() = 17;
	dequals(__LINE__, *Nd() += **ppCsc, (DOUBLE)25.);
#endif
#if ANSI
	**ppCsc = 8; *Nld() = 18;
	**ppCsc += *Nld();
	iequals(__LINE__, **ppCsc, (SCHAR)26);
	**ppCsc = 8;
	iequals(__LINE__, **ppCsc += *Nld(), (SCHAR)26);
	*Nld() = 18; **ppCsc = 8;
	*Nld() += **ppCsc;
	ldequals(__LINE__, *Nld(), (LDOUBLE)26.L);
	*Nld() = 18;
	ldequals(__LINE__, *Nld() += **ppCsc, (LDOUBLE)26.L);
#endif
	**ppCs = 9; *Nc() = 7;
	**ppCs += *Nc();
	iequals(__LINE__, **ppCs, (SHORT)16);
	**ppCs = 9;
	iequals(__LINE__, **ppCs += *Nc(), (SHORT)16);
	*Nc() = 7; **ppCs = 9;
	*Nc() += **ppCs;
	iequals(__LINE__, *Nc(), (CHAR)16);
	*Nc() = 7;
	iequals(__LINE__, *Nc() += **ppCs, (CHAR)16);
#if ANSI
	**ppCs = 9; *Nsc() = 8;
	**ppCs += *Nsc();
	iequals(__LINE__, **ppCs, (SHORT)17);
	**ppCs = 9;
	iequals(__LINE__, **ppCs += *Nsc(), (SHORT)17);
	*Nsc() = 8; **ppCs = 9;
	*Nsc() += **ppCs;
	iequals(__LINE__, *Nsc(), (SCHAR)17);
	*Nsc() = 8;
	iequals(__LINE__, *Nsc() += **ppCs, (SCHAR)17);
#endif
	**ppCs = 9; *Ns() = 9;
	**ppCs += *Ns();
	iequals(__LINE__, **ppCs, (SHORT)18);
	**ppCs = 9;
	iequals(__LINE__, **ppCs += *Ns(), (SHORT)18);
	*Ns() = 9; **ppCs = 9;
	*Ns() += **ppCs;
	iequals(__LINE__, *Ns(), (SHORT)18);
	*Ns() = 9;
	iequals(__LINE__, *Ns() += **ppCs, (SHORT)18);
	**ppCs = 9; *Ni() = 10;
	**ppCs += *Ni();
	iequals(__LINE__, **ppCs, (SHORT)19);
	**ppCs = 9;
	iequals(__LINE__, **ppCs += *Ni(), (SHORT)19);
	*Ni() = 10; **ppCs = 9;
	*Ni() += **ppCs;
	iequals(__LINE__, *Ni(), (INT)19);
	*Ni() = 10;
	iequals(__LINE__, *Ni() += **ppCs, (INT)19);
	**ppCs = 9; *Nuc() = 11;
	**ppCs += *Nuc();
	iequals(__LINE__, **ppCs, (SHORT)20);
	**ppCs = 9;
	iequals(__LINE__, **ppCs += *Nuc(), (SHORT)20);
	*Nuc() = 11; **ppCs = 9;
	*Nuc() += **ppCs;
	iequals(__LINE__, *Nuc(), (UCHAR)20);
	*Nuc() = 11;
	iequals(__LINE__, *Nuc() += **ppCs, (UCHAR)20);
	**ppCs = 9; *Nus() = 12;
	**ppCs += *Nus();
	iequals(__LINE__, **ppCs, (SHORT)21);
	**ppCs = 9;
	iequals(__LINE__, **ppCs += *Nus(), (SHORT)21);
	*Nus() = 12; **ppCs = 9;
	*Nus() += **ppCs;
	iequals(__LINE__, *Nus(), (USHORT)21);
	*Nus() = 12;
	iequals(__LINE__, *Nus() += **ppCs, (USHORT)21);
	**ppCs = 9; *Nui() = 13;
	**ppCs += *Nui();
	iequals(__LINE__, **ppCs, (SHORT)22);
	**ppCs = 9;
	iequals(__LINE__, **ppCs += *Nui(), (SHORT)22);
	}}
	return 	report(Filename);
	}
