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
	auto CHAR Ac = 7;
#if ANSI
	auto SCHAR Asc = 8;
#endif
	auto SHORT As = 9;
	auto INT Ai = 10;
	auto UCHAR Auc = 21;
	auto USHORT Aus = 22;
	auto UINT Aui = 23;
	auto LONG Al = 14;
	auto ULONG Aul = 25;
	auto FLOAT Af = 16;
	auto DOUBLE Ad = 17;
#if ANSI
	auto LDOUBLE Ald = 18;
#endif
	{
	auto CHAR Bc = 7;
	auto CHAR *pBc = &Bc;
#if ANSI
	auto SCHAR Bsc = 8;
	auto SCHAR *pBsc = &Bsc;
#endif
	auto SHORT Bs = 9;
	auto SHORT *pBs = &Bs;
	auto INT Bi = 10;
	auto INT *pBi = &Bi;
	auto UCHAR Buc = 21;
	auto UCHAR *pBuc = &Buc;
	auto USHORT Bus = 22;
	auto USHORT *pBus = &Bus;
	auto UINT Bui = 23;
	auto UINT *pBui = &Bui;
	auto LONG Bl = 14;
	auto LONG *pBl = &Bl;
	auto ULONG Bul = 25;
	auto ULONG *pBul = &Bul;
	auto FLOAT Bf = 16;
	auto FLOAT *pBf = &Bf;
	auto DOUBLE Bd = 17;
	auto DOUBLE *pBd = &Bd;
#if ANSI
	auto LDOUBLE Bld = 18;
	auto LDOUBLE *pBld = &Bld;
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto scalar auto pscalar1 mineq1 ";
	Ac = 7; *pBc = 7;
	Ac -= *pBc;
	iequals(__LINE__, Ac, (CHAR)0);
	Ac = 7;
	iequals(__LINE__, Ac -= *pBc, (CHAR)0);
	*pBc = 7; Ac = 7;
	*pBc -= Ac;
	iequals(__LINE__, *pBc, (CHAR)0);
	*pBc = 7;
	iequals(__LINE__, *pBc -= Ac, (CHAR)0);
#if ANSI
	Ac = 7; *pBsc = 8;
	Ac -= *pBsc;
	iequals(__LINE__, Ac, (CHAR)-1);
	Ac = 7;
	iequals(__LINE__, Ac -= *pBsc, (CHAR)-1);
	*pBsc = 8; Ac = 7;
	*pBsc -= Ac;
	iequals(__LINE__, *pBsc, (SCHAR)1);
	*pBsc = 8;
	iequals(__LINE__, *pBsc -= Ac, (SCHAR)1);
#endif
	Ac = 7; *pBs = 9;
	Ac -= *pBs;
	iequals(__LINE__, Ac, (CHAR)-2);
	Ac = 7;
	iequals(__LINE__, Ac -= *pBs, (CHAR)-2);
	*pBs = 9; Ac = 7;
	*pBs -= Ac;
	iequals(__LINE__, *pBs, (SHORT)2);
	*pBs = 9;
	iequals(__LINE__, *pBs -= Ac, (SHORT)2);
	Ac = 7; *pBi = 10;
	Ac -= *pBi;
	iequals(__LINE__, Ac, (CHAR)-3);
	Ac = 7;
	iequals(__LINE__, Ac -= *pBi, (CHAR)-3);
	*pBi = 10; Ac = 7;
	*pBi -= Ac;
	iequals(__LINE__, *pBi, (INT)3);
	*pBi = 10;
	iequals(__LINE__, *pBi -= Ac, (INT)3);
	Ac = 7; *pBuc = 21;
	Ac -= *pBuc;
	iequals(__LINE__, Ac, (CHAR)-14);
	Ac = 7;
	iequals(__LINE__, Ac -= *pBuc, (CHAR)-14);
	*pBuc = 21; Ac = 7;
	*pBuc -= Ac;
	iequals(__LINE__, *pBuc, (UCHAR)14);
	*pBuc = 21;
	iequals(__LINE__, *pBuc -= Ac, (UCHAR)14);
	Ac = 7; *pBus = 22;
	Ac -= *pBus;
	iequals(__LINE__, Ac, (CHAR)-15);
	Ac = 7;
	iequals(__LINE__, Ac -= *pBus, (CHAR)-15);
	*pBus = 22; Ac = 7;
	*pBus -= Ac;
	iequals(__LINE__, *pBus, (USHORT)15);
	*pBus = 22;
	iequals(__LINE__, *pBus -= Ac, (USHORT)15);
	Ac = 7; *pBui = 23;
	Ac -= *pBui;
	iequals(__LINE__, Ac, (CHAR)-16);
	Ac = 7;
	iequals(__LINE__, Ac -= *pBui, (CHAR)-16);
	*pBui = 23; Ac = 7;
	*pBui -= Ac;
	iequals(__LINE__, *pBui, (UINT)16);
	*pBui = 23;
	iequals(__LINE__, *pBui -= Ac, (UINT)16);
	Ac = 7; *pBl = 14;
	Ac -= *pBl;
	iequals(__LINE__, Ac, (CHAR)-7);
	Ac = 7;
	iequals(__LINE__, Ac -= *pBl, (CHAR)-7);
	*pBl = 14; Ac = 7;
	*pBl -= Ac;
	lequals(__LINE__, *pBl, (LONG)7L);
	*pBl = 14;
	lequals(__LINE__, *pBl -= Ac, (LONG)7L);
	Ac = 7; *pBul = 25;
	Ac -= *pBul;
	iequals(__LINE__, Ac, (CHAR)-18);
	Ac = 7;
	iequals(__LINE__, Ac -= *pBul, (CHAR)-18);
	*pBul = 25; Ac = 7;
	*pBul -= Ac;
	lequals(__LINE__, *pBul, (ULONG)18L);
	*pBul = 25;
	lequals(__LINE__, *pBul -= Ac, (ULONG)18L);
	Ac = 7; *pBf = 16;
/* 	Ac -= *pBf; DELETED: NON-PORTABLE */
/* 	iequals(__LINE__, Ac, (CHAR)-9); DELETED: NON-PORTABLE */
	Ac = 7;
/* 	iequals(__LINE__, Ac -= *pBf, (CHAR)-9); DELETED: NON-PORTABLE */
	*pBf = 16; Ac = 7;
	*pBf -= Ac;
	dequals(__LINE__, *pBf, (FLOAT)9.);
	*pBf = 16;
	dequals(__LINE__, *pBf -= Ac, (FLOAT)9.);
	Ac = 7; *pBd = 17;
/* 	Ac -= *pBd; DELETED: NON-PORTABLE */
/* 	iequals(__LINE__, Ac, (CHAR)-10); DELETED: NON-PORTABLE */
	Ac = 7;
/*	iequals(__LINE__, Ac -= *pBd, (CHAR)-10); DELETED: NON-PORTABLE */
	*pBd = 17; Ac = 7;
	*pBd -= Ac;
	dequals(__LINE__, *pBd, (DOUBLE)10.);
	*pBd = 17;
	dequals(__LINE__, *pBd -= Ac, (DOUBLE)10.);
#if ANSI
	Ac = 7; *pBld = 18;
/* 	Ac -= *pBld; DELETED: NON-PORTABLE */
/* 	iequals(__LINE__, Ac, (CHAR)-11); DELETED: NON-PORTABLE */
	Ac = 7;
/* 	iequals(__LINE__, Ac -= *pBld, (CHAR)-11); DELETED: NON-PORTABLE */
	*pBld = 18; Ac = 7;
	*pBld -= Ac;
	ldequals(__LINE__, *pBld, (LDOUBLE)11.L);
	*pBld = 18;
	ldequals(__LINE__, *pBld -= Ac, (LDOUBLE)11.L);
#endif
#if ANSI
	Asc = 8; *pBc = 7;
	Asc -= *pBc;
	iequals(__LINE__, Asc, (SCHAR)1);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBc, (SCHAR)1);
	*pBc = 7; Asc = 8;
	*pBc -= Asc;
	iequals(__LINE__, *pBc, (CHAR)-1);
	*pBc = 7;
	iequals(__LINE__, *pBc -= Asc, (CHAR)-1);
#endif
#if ANSI
	Asc = 8; *pBsc = 8;
	Asc -= *pBsc;
	iequals(__LINE__, Asc, (SCHAR)0);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBsc, (SCHAR)0);
	*pBsc = 8; Asc = 8;
	*pBsc -= Asc;
	iequals(__LINE__, *pBsc, (SCHAR)0);
	*pBsc = 8;
	iequals(__LINE__, *pBsc -= Asc, (SCHAR)0);
#endif
#if ANSI
	Asc = 8; *pBs = 9;
	Asc -= *pBs;
	iequals(__LINE__, Asc, (SCHAR)-1);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBs, (SCHAR)-1);
	*pBs = 9; Asc = 8;
	*pBs -= Asc;
	iequals(__LINE__, *pBs, (SHORT)1);
	*pBs = 9;
	iequals(__LINE__, *pBs -= Asc, (SHORT)1);
#endif
#if ANSI
	Asc = 8; *pBi = 10;
	Asc -= *pBi;
	iequals(__LINE__, Asc, (SCHAR)-2);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBi, (SCHAR)-2);
	*pBi = 10; Asc = 8;
	*pBi -= Asc;
	iequals(__LINE__, *pBi, (INT)2);
	*pBi = 10;
	iequals(__LINE__, *pBi -= Asc, (INT)2);
#endif
#if ANSI
	Asc = 8; *pBuc = 21;
	Asc -= *pBuc;
	iequals(__LINE__, Asc, (SCHAR)-13);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBuc, (SCHAR)-13);
	*pBuc = 21; Asc = 8;
	*pBuc -= Asc;
	iequals(__LINE__, *pBuc, (UCHAR)13);
	*pBuc = 21;
	iequals(__LINE__, *pBuc -= Asc, (UCHAR)13);
#endif
#if ANSI
	Asc = 8; *pBus = 22;
	Asc -= *pBus;
	iequals(__LINE__, Asc, (SCHAR)-14);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBus, (SCHAR)-14);
	*pBus = 22; Asc = 8;
	*pBus -= Asc;
	iequals(__LINE__, *pBus, (USHORT)14);
	*pBus = 22;
	iequals(__LINE__, *pBus -= Asc, (USHORT)14);
#endif
#if ANSI
	Asc = 8; *pBui = 23;
	Asc -= *pBui;
	iequals(__LINE__, Asc, (SCHAR)-15);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBui, (SCHAR)-15);
	*pBui = 23; Asc = 8;
	*pBui -= Asc;
	iequals(__LINE__, *pBui, (UINT)15);
	*pBui = 23;
	iequals(__LINE__, *pBui -= Asc, (UINT)15);
#endif
#if ANSI
	Asc = 8; *pBl = 14;
	Asc -= *pBl;
	iequals(__LINE__, Asc, (SCHAR)-6);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBl, (SCHAR)-6);
	*pBl = 14; Asc = 8;
	*pBl -= Asc;
	lequals(__LINE__, *pBl, (LONG)6L);
	*pBl = 14;
	lequals(__LINE__, *pBl -= Asc, (LONG)6L);
#endif
#if ANSI
	Asc = 8; *pBul = 25;
	Asc -= *pBul;
	iequals(__LINE__, Asc, (SCHAR)-17);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBul, (SCHAR)-17);
	*pBul = 25; Asc = 8;
	*pBul -= Asc;
	lequals(__LINE__, *pBul, (ULONG)17L);
	*pBul = 25;
	lequals(__LINE__, *pBul -= Asc, (ULONG)17L);
#endif
#if ANSI
	Asc = 8; *pBf = 16;
	Asc -= *pBf;
	iequals(__LINE__, Asc, (SCHAR)-8);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBf, (SCHAR)-8);
	*pBf = 16; Asc = 8;
	*pBf -= Asc;
	dequals(__LINE__, *pBf, (FLOAT)8.);
	*pBf = 16;
	dequals(__LINE__, *pBf -= Asc, (FLOAT)8.);
#endif
#if ANSI
	Asc = 8; *pBd = 17;
	Asc -= *pBd;
	iequals(__LINE__, Asc, (SCHAR)-9);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBd, (SCHAR)-9);
	*pBd = 17; Asc = 8;
	*pBd -= Asc;
	dequals(__LINE__, *pBd, (DOUBLE)9.);
	*pBd = 17;
	dequals(__LINE__, *pBd -= Asc, (DOUBLE)9.);
#endif
#if ANSI
	Asc = 8; *pBld = 18;
	Asc -= *pBld;
	iequals(__LINE__, Asc, (SCHAR)-10);
	Asc = 8;
	iequals(__LINE__, Asc -= *pBld, (SCHAR)-10);
	*pBld = 18; Asc = 8;
	*pBld -= Asc;
	ldequals(__LINE__, *pBld, (LDOUBLE)10.L);
	*pBld = 18;
	ldequals(__LINE__, *pBld -= Asc, (LDOUBLE)10.L);
#endif
	As = 9; *pBc = 7;
	As -= *pBc;
	iequals(__LINE__, As, (SHORT)2);
	As = 9;
	iequals(__LINE__, As -= *pBc, (SHORT)2);
	*pBc = 7; As = 9;
	*pBc -= As;
	iequals(__LINE__, *pBc, (CHAR)-2);
	*pBc = 7;
	iequals(__LINE__, *pBc -= As, (CHAR)-2);
#if ANSI
	As = 9; *pBsc = 8;
	As -= *pBsc;
	iequals(__LINE__, As, (SHORT)1);
	As = 9;
	iequals(__LINE__, As -= *pBsc, (SHORT)1);
	*pBsc = 8; As = 9;
	*pBsc -= As;
	iequals(__LINE__, *pBsc, (SCHAR)-1);
	*pBsc = 8;
	iequals(__LINE__, *pBsc -= As, (SCHAR)-1);
#endif
	As = 9; *pBs = 9;
	As -= *pBs;
	iequals(__LINE__, As, (SHORT)0);
	As = 9;
	iequals(__LINE__, As -= *pBs, (SHORT)0);
	*pBs = 9; As = 9;
	*pBs -= As;
	iequals(__LINE__, *pBs, (SHORT)0);
	*pBs = 9;
	iequals(__LINE__, *pBs -= As, (SHORT)0);
	As = 9; *pBi = 10;
	As -= *pBi;
	iequals(__LINE__, As, (SHORT)-1);
	As = 9;
	iequals(__LINE__, As -= *pBi, (SHORT)-1);
	*pBi = 10; As = 9;
	*pBi -= As;
	iequals(__LINE__, *pBi, (INT)1);
	*pBi = 10;
	iequals(__LINE__, *pBi -= As, (INT)1);
	As = 9; *pBuc = 21;
	As -= *pBuc;
	iequals(__LINE__, As, (SHORT)-12);
	As = 9;
	iequals(__LINE__, As -= *pBuc, (SHORT)-12);
	*pBuc = 21; As = 9;
	*pBuc -= As;
	iequals(__LINE__, *pBuc, (UCHAR)12);
	*pBuc = 21;
	iequals(__LINE__, *pBuc -= As, (UCHAR)12);
	As = 9; *pBus = 22;
	As -= *pBus;
	iequals(__LINE__, As, (SHORT)-13);
	As = 9;
	iequals(__LINE__, As -= *pBus, (SHORT)-13);
	*pBus = 22; As = 9;
	*pBus -= As;
	iequals(__LINE__, *pBus, (USHORT)13);
	*pBus = 22;
	iequals(__LINE__, *pBus -= As, (USHORT)13);
	As = 9; *pBui = 23;
	As -= *pBui;
	iequals(__LINE__, As, (SHORT)(UINT)-14);
	As = 9;
	iequals(__LINE__, As -= *pBui, (SHORT)(UINT)-14);
	*pBui = 23; As = 9;
	*pBui -= As;
	iequals(__LINE__, *pBui, (UINT)14);
	*pBui = 23;
	iequals(__LINE__, *pBui -= As, (UINT)14);
	As = 9; *pBl = 14;
	As -= *pBl;
	iequals(__LINE__, As, (SHORT)-5);
	As = 9;
	iequals(__LINE__, As -= *pBl, (SHORT)-5);
	*pBl = 14; As = 9;
	*pBl -= As;
	lequals(__LINE__, *pBl, (LONG)5L);
	*pBl = 14;
	lequals(__LINE__, *pBl -= As, (LONG)5L);
	}}
	return 	report(Filename);
	}
