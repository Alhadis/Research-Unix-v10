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
	Filename =  " auto scalar auto union timeq1 ";
	Ac = 7; D1.c = 7;
	Ac *= D1.c;
	iequals(__LINE__, Ac, (CHAR)49);
	Ac = 7;
	iequals(__LINE__, Ac *= D1.c, (CHAR)49);
	D1.c = 7; Ac = 7;
	D1.c *= Ac;
	iequals(__LINE__, D1.c, (CHAR)49);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Ac, (CHAR)49);
#if ANSI
	Ac = 7; D2.sc = 8;
	Ac *= D2.sc;
	iequals(__LINE__, Ac, (CHAR)56);
	Ac = 7;
	iequals(__LINE__, Ac *= D2.sc, (CHAR)56);
	D2.sc = 8; Ac = 7;
	D2.sc *= Ac;
	iequals(__LINE__, D2.sc, (SCHAR)56);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Ac, (SCHAR)56);
#endif
	Ac = 7; D3.s = 9;
	Ac *= D3.s;
	iequals(__LINE__, Ac, (CHAR)63);
	Ac = 7;
	iequals(__LINE__, Ac *= D3.s, (CHAR)63);
	D3.s = 9; Ac = 7;
	D3.s *= Ac;
	iequals(__LINE__, D3.s, (SHORT)63);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Ac, (SHORT)63);
	Ac = 7; D4.i = 10;
	Ac *= D4.i;
	iequals(__LINE__, Ac, (CHAR)70);
	Ac = 7;
	iequals(__LINE__, Ac *= D4.i, (CHAR)70);
	D4.i = 10; Ac = 7;
	D4.i *= Ac;
	iequals(__LINE__, D4.i, (INT)70);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Ac, (INT)70);
	Ac = 7; D5.uc = 11;
	Ac *= D5.uc;
	iequals(__LINE__, Ac, (CHAR)77);
	Ac = 7;
	iequals(__LINE__, Ac *= D5.uc, (CHAR)77);
	D5.uc = 11; Ac = 7;
	D5.uc *= Ac;
	iequals(__LINE__, D5.uc, (UCHAR)77);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Ac, (UCHAR)77);
	Ac = 7; D6.us = 12;
	Ac *= D6.us;
	iequals(__LINE__, Ac, (CHAR)84);
	Ac = 7;
	iequals(__LINE__, Ac *= D6.us, (CHAR)84);
	D6.us = 12; Ac = 7;
	D6.us *= Ac;
	iequals(__LINE__, D6.us, (USHORT)84);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Ac, (USHORT)84);
	Ac = 7; D7.ui = 13;
	Ac *= D7.ui;
	iequals(__LINE__, Ac, (CHAR)91);
	Ac = 7;
	iequals(__LINE__, Ac *= D7.ui, (CHAR)91);
	D7.ui = 13; Ac = 7;
	D7.ui *= Ac;
	iequals(__LINE__, D7.ui, (UINT)91);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Ac, (UINT)91);
	Ac = 7; D8.l = 14;
	Ac *= D8.l;
	iequals(__LINE__, Ac, (CHAR)98);
	Ac = 7;
	iequals(__LINE__, Ac *= D8.l, (CHAR)98);
	D8.l = 14; Ac = 7;
	D8.l *= Ac;
	lequals(__LINE__, D8.l, (LONG)98L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Ac, (LONG)98L);
	Ac = 7; D9.ul = 15;
	Ac *= D9.ul;
	iequals(__LINE__, Ac, (CHAR)105);
	Ac = 7;
	iequals(__LINE__, Ac *= D9.ul, (CHAR)105);
	D9.ul = 15; Ac = 7;
	D9.ul *= Ac;
	lequals(__LINE__, D9.ul, (ULONG)105L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Ac, (ULONG)105L);
	Ac = 7; D10.f = 16;
	Ac *= D10.f;
	iequals(__LINE__, Ac, (CHAR)112);
	Ac = 7;
	iequals(__LINE__, Ac *= D10.f, (CHAR)112);
	D10.f = 16; Ac = 7;
	D10.f *= Ac;
	dequals(__LINE__, D10.f, (FLOAT)112.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Ac, (FLOAT)112.);
	Ac = 7; D11.d = 17;
	Ac *= D11.d;
	iequals(__LINE__, Ac, (CHAR)119);
	Ac = 7;
	iequals(__LINE__, Ac *= D11.d, (CHAR)119);
	D11.d = 17; Ac = 7;
	D11.d *= Ac;
	dequals(__LINE__, D11.d, (DOUBLE)119.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Ac, (DOUBLE)119.);
#if ANSI
	Ac = 7; D12.ld = 18;
	Ac *= D12.ld;
	iequals(__LINE__, Ac, (CHAR)126);
	Ac = 7;
	iequals(__LINE__, Ac *= D12.ld, (CHAR)126);
	D12.ld = 18; Ac = 7;
	D12.ld *= Ac;
	ldequals(__LINE__, D12.ld, (LDOUBLE)126.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Ac, (LDOUBLE)126.L);
#endif
#if ANSI
	Asc = 8; D1.c = 7;
	Asc *= D1.c;
	iequals(__LINE__, Asc, (SCHAR)56);
	Asc = 8;
	iequals(__LINE__, Asc *= D1.c, (SCHAR)56);
	D1.c = 7; Asc = 8;
	D1.c *= Asc;
	iequals(__LINE__, D1.c, (CHAR)56);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Asc, (CHAR)56);
#endif
#if ANSI
	Asc = 8; D2.sc = 8;
	Asc *= D2.sc;
	iequals(__LINE__, Asc, (SCHAR)64);
	Asc = 8;
	iequals(__LINE__, Asc *= D2.sc, (SCHAR)64);
	D2.sc = 8; Asc = 8;
	D2.sc *= Asc;
	iequals(__LINE__, D2.sc, (SCHAR)64);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Asc, (SCHAR)64);
#endif
#if ANSI
	Asc = 8; D3.s = 9;
	Asc *= D3.s;
	iequals(__LINE__, Asc, (SCHAR)72);
	Asc = 8;
	iequals(__LINE__, Asc *= D3.s, (SCHAR)72);
	D3.s = 9; Asc = 8;
	D3.s *= Asc;
	iequals(__LINE__, D3.s, (SHORT)72);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Asc, (SHORT)72);
#endif
#if ANSI
	Asc = 8; D4.i = 10;
	Asc *= D4.i;
	iequals(__LINE__, Asc, (SCHAR)80);
	Asc = 8;
	iequals(__LINE__, Asc *= D4.i, (SCHAR)80);
	D4.i = 10; Asc = 8;
	D4.i *= Asc;
	iequals(__LINE__, D4.i, (INT)80);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Asc, (INT)80);
#endif
#if ANSI
	Asc = 8; D5.uc = 11;
	Asc *= D5.uc;
	iequals(__LINE__, Asc, (SCHAR)88);
	Asc = 8;
	iequals(__LINE__, Asc *= D5.uc, (SCHAR)88);
	D5.uc = 11; Asc = 8;
	D5.uc *= Asc;
	iequals(__LINE__, D5.uc, (UCHAR)88);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Asc, (UCHAR)88);
#endif
#if ANSI
	Asc = 8; D6.us = 12;
	Asc *= D6.us;
	iequals(__LINE__, Asc, (SCHAR)96);
	Asc = 8;
	iequals(__LINE__, Asc *= D6.us, (SCHAR)96);
	D6.us = 12; Asc = 8;
	D6.us *= Asc;
	iequals(__LINE__, D6.us, (USHORT)96);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Asc, (USHORT)96);
#endif
#if ANSI
	Asc = 8; D7.ui = 13;
	Asc *= D7.ui;
	iequals(__LINE__, Asc, (SCHAR)104);
	Asc = 8;
	iequals(__LINE__, Asc *= D7.ui, (SCHAR)104);
	D7.ui = 13; Asc = 8;
	D7.ui *= Asc;
	iequals(__LINE__, D7.ui, (UINT)104);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Asc, (UINT)104);
#endif
#if ANSI
	Asc = 8; D8.l = 14;
	Asc *= D8.l;
	iequals(__LINE__, Asc, (SCHAR)112);
	Asc = 8;
	iequals(__LINE__, Asc *= D8.l, (SCHAR)112);
	D8.l = 14; Asc = 8;
	D8.l *= Asc;
	lequals(__LINE__, D8.l, (LONG)112L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Asc, (LONG)112L);
#endif
#if ANSI
	Asc = 8; D9.ul = 15;
	Asc *= D9.ul;
	iequals(__LINE__, Asc, (SCHAR)120);
	Asc = 8;
	iequals(__LINE__, Asc *= D9.ul, (SCHAR)120);
	D9.ul = 15; Asc = 8;
	D9.ul *= Asc;
	lequals(__LINE__, D9.ul, (ULONG)120L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Asc, (ULONG)120L);
#endif
#if ANSI
	Asc = 8; D10.f = 15;
	Asc *= D10.f;
	iequals(__LINE__, Asc, (SCHAR)120);
	Asc = 8;
	iequals(__LINE__, Asc *= D10.f, (SCHAR)120);
	D10.f = 15; Asc = 8;
	D10.f *= Asc;
	dequals(__LINE__, D10.f, (FLOAT)120.);
	D10.f = 15;
	dequals(__LINE__, D10.f *= Asc, (FLOAT)120.);
#endif
#if ANSI
	Asc = 8; D11.d = 15;
	Asc *= D11.d;
	iequals(__LINE__, Asc, (SCHAR)120);
	Asc = 8;
	iequals(__LINE__, Asc *= D11.d, (SCHAR)120);
	D11.d = 15; Asc = 8;
	D11.d *= Asc;
	dequals(__LINE__, D11.d, (DOUBLE)120.);
	D11.d = 15;
	dequals(__LINE__, D11.d *= Asc, (DOUBLE)120.);
#endif
#if ANSI
	Asc = 8; D12.ld = 15;
	Asc *= D12.ld;
	iequals(__LINE__, Asc, (SCHAR)120);
	Asc = 8;
	iequals(__LINE__, Asc *= D12.ld, (SCHAR)120);
	D12.ld = 15; Asc = 8;
	D12.ld *= Asc;
	ldequals(__LINE__, D12.ld, (LDOUBLE)120.L);
	D12.ld = 15;
	ldequals(__LINE__, D12.ld *= Asc, (LDOUBLE)120.L);
#endif
	As = 9; D1.c = 7;
	As *= D1.c;
	iequals(__LINE__, As, (SHORT)63);
	As = 9;
	iequals(__LINE__, As *= D1.c, (SHORT)63);
	D1.c = 7; As = 9;
	D1.c *= As;
	iequals(__LINE__, D1.c, (CHAR)63);
	D1.c = 7;
	iequals(__LINE__, D1.c *= As, (CHAR)63);
#if ANSI
	As = 9; D2.sc = 8;
	As *= D2.sc;
	iequals(__LINE__, As, (SHORT)72);
	As = 9;
	iequals(__LINE__, As *= D2.sc, (SHORT)72);
	D2.sc = 8; As = 9;
	D2.sc *= As;
	iequals(__LINE__, D2.sc, (SCHAR)72);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= As, (SCHAR)72);
#endif
	As = 9; D3.s = 9;
	As *= D3.s;
	iequals(__LINE__, As, (SHORT)81);
	As = 9;
	iequals(__LINE__, As *= D3.s, (SHORT)81);
	D3.s = 9; As = 9;
	D3.s *= As;
	iequals(__LINE__, D3.s, (SHORT)81);
	D3.s = 9;
	iequals(__LINE__, D3.s *= As, (SHORT)81);
	As = 9; D4.i = 10;
	As *= D4.i;
	iequals(__LINE__, As, (SHORT)90);
	As = 9;
	iequals(__LINE__, As *= D4.i, (SHORT)90);
	D4.i = 10; As = 9;
	D4.i *= As;
	iequals(__LINE__, D4.i, (INT)90);
	D4.i = 10;
	iequals(__LINE__, D4.i *= As, (INT)90);
	As = 9; D5.uc = 11;
	As *= D5.uc;
	iequals(__LINE__, As, (SHORT)99);
	As = 9;
	iequals(__LINE__, As *= D5.uc, (SHORT)99);
	D5.uc = 11; As = 9;
	D5.uc *= As;
	iequals(__LINE__, D5.uc, (UCHAR)99);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= As, (UCHAR)99);
	As = 9; D6.us = 12;
	As *= D6.us;
	iequals(__LINE__, As, (SHORT)108);
	As = 9;
	iequals(__LINE__, As *= D6.us, (SHORT)108);
	D6.us = 12; As = 9;
	D6.us *= As;
	iequals(__LINE__, D6.us, (USHORT)108);
	D6.us = 12;
	iequals(__LINE__, D6.us *= As, (USHORT)108);
	}}
	return 	report(Filename);
	}
