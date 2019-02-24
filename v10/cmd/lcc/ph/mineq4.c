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
	Filename =  " auto scalar auto pscalar1 mineq4 ";
#if ANSI
	Af = 16; *pBsc = 8;
	Af -= *pBsc;
	dequals(__LINE__, Af, (FLOAT)8.);
	Af = 16;
	dequals(__LINE__, Af -= *pBsc, (FLOAT)8.);
	*pBsc = 8; Af = 16;
	*pBsc -= Af;
	iequals(__LINE__, *pBsc, (SCHAR)-8);
	*pBsc = 8;
	iequals(__LINE__, *pBsc -= Af, (SCHAR)-8);
#endif
	Af = 16; *pBs = 9;
	Af -= *pBs;
	dequals(__LINE__, Af, (FLOAT)7.);
	Af = 16;
	dequals(__LINE__, Af -= *pBs, (FLOAT)7.);
	*pBs = 9; Af = 16;
	*pBs -= Af;
	iequals(__LINE__, *pBs, (SHORT)-7);
	*pBs = 9;
	iequals(__LINE__, *pBs -= Af, (SHORT)-7);
	Af = 16; *pBi = 10;
	Af -= *pBi;
	dequals(__LINE__, Af, (FLOAT)6.);
	Af = 16;
	dequals(__LINE__, Af -= *pBi, (FLOAT)6.);
	*pBi = 10; Af = 16;
	*pBi -= Af;
	iequals(__LINE__, *pBi, (INT)-6);
	*pBi = 10;
	iequals(__LINE__, *pBi -= Af, (INT)-6);
	Af = 16; *pBuc = 21;
	Af -= *pBuc;
	dequals(__LINE__, Af, (FLOAT)-5.);
	Af = 16;
	dequals(__LINE__, Af -= *pBuc, (FLOAT)-5.);
	*pBuc = 21; Af = 16;
	*pBuc -= Af;
	iequals(__LINE__, *pBuc, (UCHAR)5);
	*pBuc = 21;
	iequals(__LINE__, *pBuc -= Af, (UCHAR)5);
	Af = 16; *pBus = 22;
	Af -= *pBus;
	dequals(__LINE__, Af, (FLOAT)-6.);
	Af = 16;
	dequals(__LINE__, Af -= *pBus, (FLOAT)-6.);
	*pBus = 22; Af = 16;
	*pBus -= Af;
	iequals(__LINE__, *pBus, (USHORT)6);
	*pBus = 22;
	iequals(__LINE__, *pBus -= Af, (USHORT)6);
	Af = 16; *pBui = 23;
	Af -= *pBui;
	dequals(__LINE__, Af, (FLOAT)-7.);
	Af = 16;
	dequals(__LINE__, Af -= *pBui, (FLOAT)-7.);
	*pBui = 23; Af = 16;
	*pBui -= Af;
	iequals(__LINE__, *pBui, (UINT)7);
	*pBui = 23;
	iequals(__LINE__, *pBui -= Af, (UINT)7);
	Af = 16; *pBl = 14;
	Af -= *pBl;
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 16;
	dequals(__LINE__, Af -= *pBl, (FLOAT)2.);
	*pBl = 14; Af = 16;
	*pBl -= Af;
	lequals(__LINE__, *pBl, (LONG)-2L);
	*pBl = 14;
	lequals(__LINE__, *pBl -= Af, (LONG)-2L);
	Af = 16; *pBul = 25;
	Af -= *pBul;
	dequals(__LINE__, Af, (FLOAT)-9.);
	Af = 16;
	dequals(__LINE__, Af -= *pBul, (FLOAT)-9.);
	*pBul = 25; Af = 16;
	*pBul -= Af;
	lequals(__LINE__, *pBul, (ULONG)9L);
	*pBul = 25;
	lequals(__LINE__, *pBul -= Af, (ULONG)9L);
	Af = 16; *pBf = 16;
	Af -= *pBf;
	dequals(__LINE__, Af, (FLOAT)0.);
	Af = 16;
	dequals(__LINE__, Af -= *pBf, (FLOAT)0.);
	*pBf = 16; Af = 16;
	*pBf -= Af;
	dequals(__LINE__, *pBf, (FLOAT)0.);
	*pBf = 16;
	dequals(__LINE__, *pBf -= Af, (FLOAT)0.);
	Af = 16; *pBd = 17;
	Af -= *pBd;
	dequals(__LINE__, Af, (FLOAT)-1.);
	Af = 16;
	dequals(__LINE__, Af -= *pBd, (FLOAT)-1.);
	*pBd = 17; Af = 16;
	*pBd -= Af;
	dequals(__LINE__, *pBd, (DOUBLE)1.);
	*pBd = 17;
	dequals(__LINE__, *pBd -= Af, (DOUBLE)1.);
#if ANSI
	Af = 16; *pBld = 18;
	Af -= *pBld;
	dequals(__LINE__, Af, (FLOAT)-2.);
	Af = 16;
	dequals(__LINE__, Af -= *pBld, (FLOAT)-2.);
	*pBld = 18; Af = 16;
	*pBld -= Af;
	ldequals(__LINE__, *pBld, (LDOUBLE)2.L);
	*pBld = 18;
	ldequals(__LINE__, *pBld -= Af, (LDOUBLE)2.L);
#endif
	Ad = 17; *pBc = 7;
	Ad -= *pBc;
	dequals(__LINE__, Ad, (DOUBLE)10.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBc, (DOUBLE)10.);
	*pBc = 7; Ad = 17;
/* 	*pBc -= Ad; DELETED: NON-PORTABLE */
/* 	iequals(__LINE__, *pBc, (CHAR)-10); DELETED: NON-PORTABLE */
	*pBc = 7;
/* 	iequals(__LINE__, *pBc -= Ad, (CHAR)-10); DELETED: NON-PORTABLE */
#if ANSI
	Ad = 17; *pBsc = 8;
	Ad -= *pBsc;
	dequals(__LINE__, Ad, (DOUBLE)9.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBsc, (DOUBLE)9.);
	*pBsc = 8; Ad = 17;
	*pBsc -= Ad;
	iequals(__LINE__, *pBsc, (SCHAR)-9);
	*pBsc = 8;
	iequals(__LINE__, *pBsc -= Ad, (SCHAR)-9);
#endif
	Ad = 17; *pBs = 9;
	Ad -= *pBs;
	dequals(__LINE__, Ad, (DOUBLE)8.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBs, (DOUBLE)8.);
	*pBs = 9; Ad = 17;
	*pBs -= Ad;
	iequals(__LINE__, *pBs, (SHORT)-8);
	*pBs = 9;
	iequals(__LINE__, *pBs -= Ad, (SHORT)-8);
	Ad = 17; *pBi = 10;
	Ad -= *pBi;
	dequals(__LINE__, Ad, (DOUBLE)7.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBi, (DOUBLE)7.);
	*pBi = 10; Ad = 17;
	*pBi -= Ad;
	iequals(__LINE__, *pBi, (INT)-7);
	*pBi = 10;
	iequals(__LINE__, *pBi -= Ad, (INT)-7);
	Ad = 17; *pBuc = 21;
	Ad -= *pBuc;
	dequals(__LINE__, Ad, (DOUBLE)-4.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBuc, (DOUBLE)-4.);
	*pBuc = 21; Ad = 17;
	*pBuc -= Ad;
	iequals(__LINE__, *pBuc, (UCHAR)4);
	*pBuc = 21;
	iequals(__LINE__, *pBuc -= Ad, (UCHAR)4);
	Ad = 17; *pBus = 22;
	Ad -= *pBus;
	dequals(__LINE__, Ad, (DOUBLE)-5.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBus, (DOUBLE)-5.);
	*pBus = 22; Ad = 17;
	*pBus -= Ad;
	iequals(__LINE__, *pBus, (USHORT)5);
	*pBus = 22;
	iequals(__LINE__, *pBus -= Ad, (USHORT)5);
	Ad = 17; *pBui = 23;
	Ad -= *pBui;
	dequals(__LINE__, Ad, (DOUBLE)-6.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBui, (DOUBLE)-6.);
	*pBui = 23; Ad = 17;
	*pBui -= Ad;
	iequals(__LINE__, *pBui, (UINT)6);
	*pBui = 23;
	iequals(__LINE__, *pBui -= Ad, (UINT)6);
	Ad = 17; *pBl = 14;
	Ad -= *pBl;
	dequals(__LINE__, Ad, (DOUBLE)3.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBl, (DOUBLE)3.);
	*pBl = 14; Ad = 17;
	*pBl -= Ad;
	lequals(__LINE__, *pBl, (LONG)-3L);
	*pBl = 14;
	lequals(__LINE__, *pBl -= Ad, (LONG)-3L);
	Ad = 17; *pBul = 25;
	Ad -= *pBul;
	dequals(__LINE__, Ad, (DOUBLE)-8.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBul, (DOUBLE)-8.);
	*pBul = 25; Ad = 17;
	*pBul -= Ad;
	lequals(__LINE__, *pBul, (ULONG)8L);
	*pBul = 25;
	lequals(__LINE__, *pBul -= Ad, (ULONG)8L);
	Ad = 17; *pBf = 16;
	Ad -= *pBf;
	dequals(__LINE__, Ad, (DOUBLE)1.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBf, (DOUBLE)1.);
	*pBf = 16; Ad = 17;
	*pBf -= Ad;
	dequals(__LINE__, *pBf, (FLOAT)-1.);
	*pBf = 16;
	dequals(__LINE__, *pBf -= Ad, (FLOAT)-1.);
	Ad = 17; *pBd = 17;
	Ad -= *pBd;
	dequals(__LINE__, Ad, (DOUBLE)0.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBd, (DOUBLE)0.);
	*pBd = 17; Ad = 17;
	*pBd -= Ad;
	dequals(__LINE__, *pBd, (DOUBLE)0.);
	*pBd = 17;
	dequals(__LINE__, *pBd -= Ad, (DOUBLE)0.);
#if ANSI
	Ad = 17; *pBld = 18;
	Ad -= *pBld;
	dequals(__LINE__, Ad, (DOUBLE)-1.);
	Ad = 17;
	dequals(__LINE__, Ad -= *pBld, (DOUBLE)-1.);
	*pBld = 18; Ad = 17;
	*pBld -= Ad;
	ldequals(__LINE__, *pBld, (LDOUBLE)1.L);
	*pBld = 18;
	ldequals(__LINE__, *pBld -= Ad, (LDOUBLE)1.L);
#endif
#if ANSI
	Ald = 18; *pBc = 7;
	Ald -= *pBc;
	ldequals(__LINE__, Ald, (LDOUBLE)11.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBc, (LDOUBLE)11.L);
	*pBc = 7; Ald = 18;
/* 	*pBc -= Ald; DELETED: NON-PORTABLE */
/* 	iequals(__LINE__, *pBc, (CHAR)-11); DELETED: NON-PORTABLE */
	*pBc = 7;
/* 	iequals(__LINE__, *pBc -= Ald, (CHAR)-11); DELETED: NON-PORTABLE */
#endif
#if ANSI
	Ald = 18; *pBsc = 8;
	Ald -= *pBsc;
	ldequals(__LINE__, Ald, (LDOUBLE)10.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBsc, (LDOUBLE)10.L);
	*pBsc = 8; Ald = 18;
	*pBsc -= Ald;
	iequals(__LINE__, *pBsc, (SCHAR)-10);
	*pBsc = 8;
	iequals(__LINE__, *pBsc -= Ald, (SCHAR)-10);
#endif
#if ANSI
	Ald = 18; *pBs = 9;
	Ald -= *pBs;
	ldequals(__LINE__, Ald, (LDOUBLE)9.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBs, (LDOUBLE)9.L);
	*pBs = 9; Ald = 18;
	*pBs -= Ald;
	iequals(__LINE__, *pBs, (SHORT)-9);
	*pBs = 9;
	iequals(__LINE__, *pBs -= Ald, (SHORT)-9);
#endif
#if ANSI
	Ald = 18; *pBi = 10;
	Ald -= *pBi;
	ldequals(__LINE__, Ald, (LDOUBLE)8.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBi, (LDOUBLE)8.L);
	*pBi = 10; Ald = 18;
	*pBi -= Ald;
	iequals(__LINE__, *pBi, (INT)-8);
	*pBi = 10;
	iequals(__LINE__, *pBi -= Ald, (INT)-8);
#endif
#if ANSI
	Ald = 18; *pBuc = 21;
	Ald -= *pBuc;
	ldequals(__LINE__, Ald, (LDOUBLE)-3.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBuc, (LDOUBLE)-3.L);
	*pBuc = 21; Ald = 18;
	*pBuc -= Ald;
	iequals(__LINE__, *pBuc, (UCHAR)3);
	*pBuc = 21;
	iequals(__LINE__, *pBuc -= Ald, (UCHAR)3);
#endif
#if ANSI
	Ald = 18; *pBus = 22;
	Ald -= *pBus;
	ldequals(__LINE__, Ald, (LDOUBLE)-4.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBus, (LDOUBLE)-4.L);
	*pBus = 22; Ald = 18;
	*pBus -= Ald;
	iequals(__LINE__, *pBus, (USHORT)4);
	*pBus = 22;
	iequals(__LINE__, *pBus -= Ald, (USHORT)4);
#endif
#if ANSI
	Ald = 18; *pBui = 23;
	Ald -= *pBui;
	ldequals(__LINE__, Ald, (LDOUBLE)-5.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBui, (LDOUBLE)-5.L);
	*pBui = 23; Ald = 18;
	*pBui -= Ald;
	iequals(__LINE__, *pBui, (UINT)5);
	*pBui = 23;
	iequals(__LINE__, *pBui -= Ald, (UINT)5);
#endif
#if ANSI
	Ald = 18; *pBl = 14;
	Ald -= *pBl;
	ldequals(__LINE__, Ald, (LDOUBLE)4.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBl, (LDOUBLE)4.L);
	*pBl = 14; Ald = 18;
	*pBl -= Ald;
	lequals(__LINE__, *pBl, (LONG)-4L);
	*pBl = 14;
	lequals(__LINE__, *pBl -= Ald, (LONG)-4L);
#endif
#if ANSI
	Ald = 18; *pBul = 25;
	Ald -= *pBul;
	ldequals(__LINE__, Ald, (LDOUBLE)-7.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBul, (LDOUBLE)-7.L);
	*pBul = 25; Ald = 18;
	*pBul -= Ald;
	lequals(__LINE__, *pBul, (ULONG)7L);
	*pBul = 25;
	lequals(__LINE__, *pBul -= Ald, (ULONG)7L);
#endif
#if ANSI
	Ald = 18; *pBf = 16;
	Ald -= *pBf;
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBf, (LDOUBLE)2.L);
	*pBf = 16; Ald = 18;
	*pBf -= Ald;
	dequals(__LINE__, *pBf, (FLOAT)-2.);
	*pBf = 16;
	dequals(__LINE__, *pBf -= Ald, (FLOAT)-2.);
#endif
#if ANSI
	Ald = 18; *pBd = 17;
	Ald -= *pBd;
	ldequals(__LINE__, Ald, (LDOUBLE)1.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBd, (LDOUBLE)1.L);
	*pBd = 17; Ald = 18;
	*pBd -= Ald;
	dequals(__LINE__, *pBd, (DOUBLE)-1.);
	*pBd = 17;
	dequals(__LINE__, *pBd -= Ald, (DOUBLE)-1.);
#endif
#if ANSI
	Ald = 18; *pBld = 18;
	Ald -= *pBld;
	ldequals(__LINE__, Ald, (LDOUBLE)0.L);
	Ald = 18;
	ldequals(__LINE__, Ald -= *pBld, (LDOUBLE)0.L);
	*pBld = 18; Ald = 18;
	*pBld -= Ald;
	ldequals(__LINE__, *pBld, (LDOUBLE)0.L);
	*pBld = 18;
	ldequals(__LINE__, *pBld -= Ald, (LDOUBLE)0.L);
#endif
	}}
	return 	report(Filename);
	}
