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
	auto UCHAR Auc = 11;
	auto USHORT Aus = 12;
	auto UINT Aui = 13;
	auto LONG Al = 14;
	auto ULONG Aul = 15;
	auto FLOAT Af = 16;
	auto DOUBLE Ad = 17;
#if ANSI
	auto LDOUBLE Ald = 18;
#endif
	{
	typedef union {
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
		} D;
	auto D D1;
#if ANSI
	auto D D2;
#endif
	auto D D3;
	auto D D4;
	auto D D5;
	auto D D6;
	auto D D7;
	auto D D8;
	auto D D9;
	auto D D10;
	auto D D11;
#if ANSI
	auto D D12;
#endif
	D1.c = 7;
#if ANSI
	D2.sc = 8;
#endif
	D3.s = 9;
	D4.i = 10;
	D5.uc = 11;
	D6.us = 12;
	D7.ui = 13;
	D8.l = 14;
	D9.ul = 15;
	D10.f = 16;
	D11.d = 17;
#if ANSI
	D12.ld = 18;
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto scalar auto union timeq2 ";
	As = 9; D7.ui = 13;
	As *= D7.ui;
	iequals(__LINE__, As, (SHORT)117);
	As = 9;
	iequals(__LINE__, As *= D7.ui, (SHORT)117);
	D7.ui = 13; As = 9;
	D7.ui *= As;
	iequals(__LINE__, D7.ui, (UINT)117);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= As, (UINT)117);
	As = 9; D8.l = 14;
	As *= D8.l;
	iequals(__LINE__, As, (SHORT)126);
	As = 9;
	iequals(__LINE__, As *= D8.l, (SHORT)126);
	D8.l = 14; As = 9;
	D8.l *= As;
	lequals(__LINE__, D8.l, (LONG)126L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= As, (LONG)126L);
	As = 9; D9.ul = 15;
	As *= D9.ul;
	iequals(__LINE__, As, (SHORT)135);
	As = 9;
	iequals(__LINE__, As *= D9.ul, (SHORT)135);
	D9.ul = 15; As = 9;
	D9.ul *= As;
	lequals(__LINE__, D9.ul, (ULONG)135L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= As, (ULONG)135L);
	As = 9; D10.f = 16;
	As *= D10.f;
	iequals(__LINE__, As, (SHORT)144);
	As = 9;
	iequals(__LINE__, As *= D10.f, (SHORT)144);
	D10.f = 16; As = 9;
	D10.f *= As;
	dequals(__LINE__, D10.f, (FLOAT)144.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= As, (FLOAT)144.);
	As = 9; D11.d = 17;
	As *= D11.d;
	iequals(__LINE__, As, (SHORT)153);
	As = 9;
	iequals(__LINE__, As *= D11.d, (SHORT)153);
	D11.d = 17; As = 9;
	D11.d *= As;
	dequals(__LINE__, D11.d, (DOUBLE)153.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= As, (DOUBLE)153.);
#if ANSI
	As = 9; D12.ld = 18;
	As *= D12.ld;
	iequals(__LINE__, As, (SHORT)162);
	As = 9;
	iequals(__LINE__, As *= D12.ld, (SHORT)162);
	D12.ld = 18; As = 9;
	D12.ld *= As;
	ldequals(__LINE__, D12.ld, (LDOUBLE)162.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= As, (LDOUBLE)162.L);
#endif
	Ai = 10; D1.c = 7;
	Ai *= D1.c;
	iequals(__LINE__, Ai, (INT)70);
	Ai = 10;
	iequals(__LINE__, Ai *= D1.c, (INT)70);
	D1.c = 7; Ai = 10;
	D1.c *= Ai;
	iequals(__LINE__, D1.c, (CHAR)70);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Ai, (CHAR)70);
#if ANSI
	Ai = 10; D2.sc = 8;
	Ai *= D2.sc;
	iequals(__LINE__, Ai, (INT)80);
	Ai = 10;
	iequals(__LINE__, Ai *= D2.sc, (INT)80);
	D2.sc = 8; Ai = 10;
	D2.sc *= Ai;
	iequals(__LINE__, D2.sc, (SCHAR)80);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Ai, (SCHAR)80);
#endif
	Ai = 10; D3.s = 9;
	Ai *= D3.s;
	iequals(__LINE__, Ai, (INT)90);
	Ai = 10;
	iequals(__LINE__, Ai *= D3.s, (INT)90);
	D3.s = 9; Ai = 10;
	D3.s *= Ai;
	iequals(__LINE__, D3.s, (SHORT)90);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Ai, (SHORT)90);
	Ai = 10; D4.i = 10;
	Ai *= D4.i;
	iequals(__LINE__, Ai, (INT)100);
	Ai = 10;
	iequals(__LINE__, Ai *= D4.i, (INT)100);
	D4.i = 10; Ai = 10;
	D4.i *= Ai;
	iequals(__LINE__, D4.i, (INT)100);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Ai, (INT)100);
	Ai = 10; D5.uc = 11;
	Ai *= D5.uc;
	iequals(__LINE__, Ai, (INT)110);
	Ai = 10;
	iequals(__LINE__, Ai *= D5.uc, (INT)110);
	D5.uc = 11; Ai = 10;
	D5.uc *= Ai;
	iequals(__LINE__, D5.uc, (UCHAR)110);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Ai, (UCHAR)110);
	Ai = 10; D6.us = 12;
	Ai *= D6.us;
	iequals(__LINE__, Ai, (INT)120);
	Ai = 10;
	iequals(__LINE__, Ai *= D6.us, (INT)120);
	D6.us = 12; Ai = 10;
	D6.us *= Ai;
	iequals(__LINE__, D6.us, (USHORT)120);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Ai, (USHORT)120);
	Ai = 10; D7.ui = 13;
	Ai *= D7.ui;
	iequals(__LINE__, Ai, (INT)130);
	Ai = 10;
	iequals(__LINE__, Ai *= D7.ui, (INT)130);
	D7.ui = 13; Ai = 10;
	D7.ui *= Ai;
	iequals(__LINE__, D7.ui, (UINT)130);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Ai, (UINT)130);
	Ai = 10; D8.l = 14;
	Ai *= D8.l;
	iequals(__LINE__, Ai, (INT)140);
	Ai = 10;
	iequals(__LINE__, Ai *= D8.l, (INT)140);
	D8.l = 14; Ai = 10;
	D8.l *= Ai;
	lequals(__LINE__, D8.l, (LONG)140L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Ai, (LONG)140L);
	Ai = 10; D9.ul = 15;
	Ai *= D9.ul;
	iequals(__LINE__, Ai, (INT)150);
	Ai = 10;
	iequals(__LINE__, Ai *= D9.ul, (INT)150);
	D9.ul = 15; Ai = 10;
	D9.ul *= Ai;
	lequals(__LINE__, D9.ul, (ULONG)150L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Ai, (ULONG)150L);
	Ai = 10; D10.f = 16;
	Ai *= D10.f;
	iequals(__LINE__, Ai, (INT)160);
	Ai = 10;
	iequals(__LINE__, Ai *= D10.f, (INT)160);
	D10.f = 16; Ai = 10;
	D10.f *= Ai;
	dequals(__LINE__, D10.f, (FLOAT)160.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Ai, (FLOAT)160.);
	Ai = 10; D11.d = 17;
	Ai *= D11.d;
	iequals(__LINE__, Ai, (INT)170);
	Ai = 10;
	iequals(__LINE__, Ai *= D11.d, (INT)170);
	D11.d = 17; Ai = 10;
	D11.d *= Ai;
	dequals(__LINE__, D11.d, (DOUBLE)170.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Ai, (DOUBLE)170.);
#if ANSI
	Ai = 10; D12.ld = 18;
	Ai *= D12.ld;
	iequals(__LINE__, Ai, (INT)180);
	Ai = 10;
	iequals(__LINE__, Ai *= D12.ld, (INT)180);
	D12.ld = 18; Ai = 10;
	D12.ld *= Ai;
	ldequals(__LINE__, D12.ld, (LDOUBLE)180.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Ai, (LDOUBLE)180.L);
#endif
	Auc = 11; D1.c = 7;
	Auc *= D1.c;
	iequals(__LINE__, Auc, (UCHAR)77);
	Auc = 11;
	iequals(__LINE__, Auc *= D1.c, (UCHAR)77);
	D1.c = 7; Auc = 11;
	D1.c *= Auc;
	iequals(__LINE__, D1.c, (CHAR)77);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Auc, (CHAR)77);
#if ANSI
	Auc = 11; D2.sc = 8;
	Auc *= D2.sc;
	iequals(__LINE__, Auc, (UCHAR)88);
	Auc = 11;
	iequals(__LINE__, Auc *= D2.sc, (UCHAR)88);
	D2.sc = 8; Auc = 11;
	D2.sc *= Auc;
	iequals(__LINE__, D2.sc, (SCHAR)88);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Auc, (SCHAR)88);
#endif
	Auc = 11; D3.s = 9;
	Auc *= D3.s;
	iequals(__LINE__, Auc, (UCHAR)99);
	Auc = 11;
	iequals(__LINE__, Auc *= D3.s, (UCHAR)99);
	D3.s = 9; Auc = 11;
	D3.s *= Auc;
	iequals(__LINE__, D3.s, (SHORT)99);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Auc, (SHORT)99);
	Auc = 11; D4.i = 10;
	Auc *= D4.i;
	iequals(__LINE__, Auc, (UCHAR)110);
	Auc = 11;
	iequals(__LINE__, Auc *= D4.i, (UCHAR)110);
	D4.i = 10; Auc = 11;
	D4.i *= Auc;
	iequals(__LINE__, D4.i, (INT)110);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Auc, (INT)110);
	Auc = 11; D5.uc = 11;
	Auc *= D5.uc;
	iequals(__LINE__, Auc, (UCHAR)121);
	Auc = 11;
	iequals(__LINE__, Auc *= D5.uc, (UCHAR)121);
	D5.uc = 11; Auc = 11;
	D5.uc *= Auc;
	iequals(__LINE__, D5.uc, (UCHAR)121);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Auc, (UCHAR)121);
	Auc = 11; D6.us = 12;
	Auc *= D6.us;
	iequals(__LINE__, Auc, (UCHAR)132);
	Auc = 11;
	iequals(__LINE__, Auc *= D6.us, (UCHAR)132);
	D6.us = 12; Auc = 11;
	D6.us *= Auc;
	iequals(__LINE__, D6.us, (USHORT)132);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Auc, (USHORT)132);
	Auc = 11; D7.ui = 13;
	Auc *= D7.ui;
	iequals(__LINE__, Auc, (UCHAR)143);
	Auc = 11;
	iequals(__LINE__, Auc *= D7.ui, (UCHAR)143);
	D7.ui = 13; Auc = 11;
	D7.ui *= Auc;
	iequals(__LINE__, D7.ui, (UINT)143);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Auc, (UINT)143);
	Auc = 11; D8.l = 14;
	Auc *= D8.l;
	iequals(__LINE__, Auc, (UCHAR)154);
	Auc = 11;
	iequals(__LINE__, Auc *= D8.l, (UCHAR)154);
	D8.l = 14; Auc = 11;
	D8.l *= Auc;
	lequals(__LINE__, D8.l, (LONG)154L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Auc, (LONG)154L);
	Auc = 11; D9.ul = 15;
	Auc *= D9.ul;
	iequals(__LINE__, Auc, (UCHAR)165);
	Auc = 11;
	iequals(__LINE__, Auc *= D9.ul, (UCHAR)165);
	D9.ul = 15; Auc = 11;
	D9.ul *= Auc;
	lequals(__LINE__, D9.ul, (ULONG)165L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Auc, (ULONG)165L);
	Auc = 11; D10.f = 16;
	Auc *= D10.f;
	iequals(__LINE__, Auc, (UCHAR)176);
	Auc = 11;
	iequals(__LINE__, Auc *= D10.f, (UCHAR)176);
	D10.f = 16; Auc = 11;
	D10.f *= Auc;
	dequals(__LINE__, D10.f, (FLOAT)176.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Auc, (FLOAT)176.);
	Auc = 11; D11.d = 17;
	Auc *= D11.d;
	iequals(__LINE__, Auc, (UCHAR)187);
	Auc = 11;
	iequals(__LINE__, Auc *= D11.d, (UCHAR)187);
	D11.d = 17; Auc = 11;
	D11.d *= Auc;
	dequals(__LINE__, D11.d, (DOUBLE)187.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Auc, (DOUBLE)187.);
#if ANSI
	Auc = 11; D12.ld = 18;
	Auc *= D12.ld;
	iequals(__LINE__, Auc, (UCHAR)198);
	Auc = 11;
	iequals(__LINE__, Auc *= D12.ld, (UCHAR)198);
	D12.ld = 18; Auc = 11;
	D12.ld *= Auc;
	ldequals(__LINE__, D12.ld, (LDOUBLE)198.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Auc, (LDOUBLE)198.L);
#endif
	Aus = 12; D1.c = 7;
	Aus *= D1.c;
	iequals(__LINE__, Aus, (USHORT)84);
	Aus = 12;
	iequals(__LINE__, Aus *= D1.c, (USHORT)84);
	D1.c = 7; Aus = 12;
	D1.c *= Aus;
	iequals(__LINE__, D1.c, (CHAR)84);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Aus, (CHAR)84);
#if ANSI
	Aus = 12; D2.sc = 8;
	Aus *= D2.sc;
	iequals(__LINE__, Aus, (USHORT)96);
	Aus = 12;
	iequals(__LINE__, Aus *= D2.sc, (USHORT)96);
	D2.sc = 8; Aus = 12;
	D2.sc *= Aus;
	iequals(__LINE__, D2.sc, (SCHAR)96);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Aus, (SCHAR)96);
#endif
	Aus = 12; D3.s = 9;
	Aus *= D3.s;
	iequals(__LINE__, Aus, (USHORT)108);
	Aus = 12;
	iequals(__LINE__, Aus *= D3.s, (USHORT)108);
	D3.s = 9; Aus = 12;
	D3.s *= Aus;
	iequals(__LINE__, D3.s, (SHORT)108);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Aus, (SHORT)108);
	Aus = 12; D4.i = 10;
	Aus *= D4.i;
	iequals(__LINE__, Aus, (USHORT)120);
	Aus = 12;
	iequals(__LINE__, Aus *= D4.i, (USHORT)120);
	D4.i = 10; Aus = 12;
	D4.i *= Aus;
	iequals(__LINE__, D4.i, (INT)120);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Aus, (INT)120);
	Aus = 12; D5.uc = 11;
	Aus *= D5.uc;
	iequals(__LINE__, Aus, (USHORT)132);
	Aus = 12;
	iequals(__LINE__, Aus *= D5.uc, (USHORT)132);
	D5.uc = 11; Aus = 12;
	D5.uc *= Aus;
	iequals(__LINE__, D5.uc, (UCHAR)132);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Aus, (UCHAR)132);
	Aus = 12; D6.us = 12;
	Aus *= D6.us;
	iequals(__LINE__, Aus, (USHORT)144);
	Aus = 12;
	iequals(__LINE__, Aus *= D6.us, (USHORT)144);
	D6.us = 12; Aus = 12;
	D6.us *= Aus;
	iequals(__LINE__, D6.us, (USHORT)144);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Aus, (USHORT)144);
	Aus = 12; D7.ui = 13;
	Aus *= D7.ui;
	iequals(__LINE__, Aus, (USHORT)156);
	Aus = 12;
	iequals(__LINE__, Aus *= D7.ui, (USHORT)156);
	D7.ui = 13; Aus = 12;
	D7.ui *= Aus;
	iequals(__LINE__, D7.ui, (UINT)156);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Aus, (UINT)156);
	Aus = 12; D8.l = 14;
	Aus *= D8.l;
	iequals(__LINE__, Aus, (USHORT)168);
	Aus = 12;
	iequals(__LINE__, Aus *= D8.l, (USHORT)168);
	D8.l = 14; Aus = 12;
	D8.l *= Aus;
	lequals(__LINE__, D8.l, (LONG)168L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Aus, (LONG)168L);
	Aus = 12; D9.ul = 15;
	Aus *= D9.ul;
	iequals(__LINE__, Aus, (USHORT)180);
	Aus = 12;
	iequals(__LINE__, Aus *= D9.ul, (USHORT)180);
	}}
	return 	report(Filename);
	}
