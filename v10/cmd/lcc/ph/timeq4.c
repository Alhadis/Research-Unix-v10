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
	Filename =  " auto scalar auto union timeq4 ";
#if ANSI
	D12.ld = 18; Aul = 15;
	D12.ld *= Aul;
	ldequals(__LINE__, D12.ld, (LDOUBLE)270.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Aul, (LDOUBLE)270.L);
#endif
	Af = 16; D1.c = 7;
	Af *= D1.c;
	dequals(__LINE__, Af, (FLOAT)112.);
	Af = 16;
	dequals(__LINE__, Af *= D1.c, (FLOAT)112.);
	D1.c = 7; Af = 16;
	D1.c *= Af;
	iequals(__LINE__, D1.c, (CHAR)112);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Af, (CHAR)112);
#if ANSI
	Af = 15; D2.sc = 8;
	Af *= D2.sc;
	dequals(__LINE__, Af, (FLOAT)120.);
	Af = 15;
	dequals(__LINE__, Af *= D2.sc, (FLOAT)120.);
	D2.sc = 8; Af = 15;
	D2.sc *= Af;
	iequals(__LINE__, D2.sc, (SCHAR)120);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Af, (SCHAR)120);
#endif
	Af = 16; D3.s = 9;
	Af *= D3.s;
	dequals(__LINE__, Af, (FLOAT)144.);
	Af = 16;
	dequals(__LINE__, Af *= D3.s, (FLOAT)144.);
	D3.s = 9; Af = 16;
	D3.s *= Af;
	iequals(__LINE__, D3.s, (SHORT)144);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Af, (SHORT)144);
	Af = 16; D4.i = 10;
	Af *= D4.i;
	dequals(__LINE__, Af, (FLOAT)160.);
	Af = 16;
	dequals(__LINE__, Af *= D4.i, (FLOAT)160.);
	D4.i = 10; Af = 16;
	D4.i *= Af;
	iequals(__LINE__, D4.i, (INT)160);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Af, (INT)160);
	Af = 16; D5.uc = 11;
	Af *= D5.uc;
	dequals(__LINE__, Af, (FLOAT)176.);
	Af = 16;
	dequals(__LINE__, Af *= D5.uc, (FLOAT)176.);
	D5.uc = 11; Af = 16;
	D5.uc *= Af;
	iequals(__LINE__, D5.uc, (UCHAR)176);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Af, (UCHAR)176);
	Af = 16; D6.us = 12;
	Af *= D6.us;
	dequals(__LINE__, Af, (FLOAT)192.);
	Af = 16;
	dequals(__LINE__, Af *= D6.us, (FLOAT)192.);
	D6.us = 12; Af = 16;
	D6.us *= Af;
	iequals(__LINE__, D6.us, (USHORT)192);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Af, (USHORT)192);
	Af = 16; D7.ui = 13;
	Af *= D7.ui;
	dequals(__LINE__, Af, (FLOAT)208.);
	Af = 16;
	dequals(__LINE__, Af *= D7.ui, (FLOAT)208.);
	D7.ui = 13; Af = 16;
	D7.ui *= Af;
	iequals(__LINE__, D7.ui, (UINT)208);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Af, (UINT)208);
	Af = 16; D8.l = 14;
	Af *= D8.l;
	dequals(__LINE__, Af, (FLOAT)224.);
	Af = 16;
	dequals(__LINE__, Af *= D8.l, (FLOAT)224.);
	D8.l = 14; Af = 16;
	D8.l *= Af;
	lequals(__LINE__, D8.l, (LONG)224L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Af, (LONG)224L);
	Af = 16; D9.ul = 15;
	Af *= D9.ul;
	dequals(__LINE__, Af, (FLOAT)240.);
	Af = 16;
	dequals(__LINE__, Af *= D9.ul, (FLOAT)240.);
	D9.ul = 15; Af = 16;
	D9.ul *= Af;
	lequals(__LINE__, D9.ul, (ULONG)240L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Af, (ULONG)240L);
	Af = 16; D10.f = 16;
	Af *= D10.f;
	dequals(__LINE__, Af, (FLOAT)256.);
	Af = 16;
	dequals(__LINE__, Af *= D10.f, (FLOAT)256.);
	D10.f = 16; Af = 16;
	D10.f *= Af;
	dequals(__LINE__, D10.f, (FLOAT)256.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Af, (FLOAT)256.);
	Af = 16; D11.d = 17;
	Af *= D11.d;
	dequals(__LINE__, Af, (FLOAT)272.);
	Af = 16;
	dequals(__LINE__, Af *= D11.d, (FLOAT)272.);
	D11.d = 17; Af = 16;
	D11.d *= Af;
	dequals(__LINE__, D11.d, (DOUBLE)272.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Af, (DOUBLE)272.);
#if ANSI
	Af = 16; D12.ld = 18;
	Af *= D12.ld;
	dequals(__LINE__, Af, (FLOAT)288.);
	Af = 16;
	dequals(__LINE__, Af *= D12.ld, (FLOAT)288.);
	D12.ld = 18; Af = 16;
	D12.ld *= Af;
	ldequals(__LINE__, D12.ld, (LDOUBLE)288.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Af, (LDOUBLE)288.L);
#endif
	Ad = 17; D1.c = 7;
	Ad *= D1.c;
	dequals(__LINE__, Ad, (DOUBLE)119.);
	Ad = 17;
	dequals(__LINE__, Ad *= D1.c, (DOUBLE)119.);
	D1.c = 7; Ad = 17;
	D1.c *= Ad;
	iequals(__LINE__, D1.c, (CHAR)119);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Ad, (CHAR)119);
#if ANSI
	Ad = 15; D2.sc = 8;
	Ad *= D2.sc;
	dequals(__LINE__, Ad, (DOUBLE)120.);
	Ad = 15;
	dequals(__LINE__, Ad *= D2.sc, (DOUBLE)120.);
	D2.sc = 8; Ad = 15;
	D2.sc *= Ad;
	iequals(__LINE__, D2.sc, (SCHAR)120);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Ad, (SCHAR)120);
#endif
	Ad = 17; D3.s = 9;
	Ad *= D3.s;
	dequals(__LINE__, Ad, (DOUBLE)153.);
	Ad = 17;
	dequals(__LINE__, Ad *= D3.s, (DOUBLE)153.);
	D3.s = 9; Ad = 17;
	D3.s *= Ad;
	iequals(__LINE__, D3.s, (SHORT)153);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Ad, (SHORT)153);
	Ad = 17; D4.i = 10;
	Ad *= D4.i;
	dequals(__LINE__, Ad, (DOUBLE)170.);
	Ad = 17;
	dequals(__LINE__, Ad *= D4.i, (DOUBLE)170.);
	D4.i = 10; Ad = 17;
	D4.i *= Ad;
	iequals(__LINE__, D4.i, (INT)170);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Ad, (INT)170);
	Ad = 17; D5.uc = 11;
	Ad *= D5.uc;
	dequals(__LINE__, Ad, (DOUBLE)187.);
	Ad = 17;
	dequals(__LINE__, Ad *= D5.uc, (DOUBLE)187.);
	D5.uc = 11; Ad = 17;
	D5.uc *= Ad;
	iequals(__LINE__, D5.uc, (UCHAR)187);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Ad, (UCHAR)187);
	Ad = 17; D6.us = 12;
	Ad *= D6.us;
	dequals(__LINE__, Ad, (DOUBLE)204.);
	Ad = 17;
	dequals(__LINE__, Ad *= D6.us, (DOUBLE)204.);
	D6.us = 12; Ad = 17;
	D6.us *= Ad;
	iequals(__LINE__, D6.us, (USHORT)204);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Ad, (USHORT)204);
	Ad = 17; D7.ui = 13;
	Ad *= D7.ui;
	dequals(__LINE__, Ad, (DOUBLE)221.);
	Ad = 17;
	dequals(__LINE__, Ad *= D7.ui, (DOUBLE)221.);
	D7.ui = 13; Ad = 17;
	D7.ui *= Ad;
	iequals(__LINE__, D7.ui, (UINT)221);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Ad, (UINT)221);
	Ad = 17; D8.l = 14;
	Ad *= D8.l;
	dequals(__LINE__, Ad, (DOUBLE)238.);
	Ad = 17;
	dequals(__LINE__, Ad *= D8.l, (DOUBLE)238.);
	D8.l = 14; Ad = 17;
	D8.l *= Ad;
	lequals(__LINE__, D8.l, (LONG)238L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Ad, (LONG)238L);
	Ad = 17; D9.ul = 15;
	Ad *= D9.ul;
	dequals(__LINE__, Ad, (DOUBLE)255.);
	Ad = 17;
	dequals(__LINE__, Ad *= D9.ul, (DOUBLE)255.);
	D9.ul = 15; Ad = 17;
	D9.ul *= Ad;
	lequals(__LINE__, D9.ul, (ULONG)255L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Ad, (ULONG)255L);
	Ad = 17; D10.f = 16;
	Ad *= D10.f;
	dequals(__LINE__, Ad, (DOUBLE)272.);
	Ad = 17;
	dequals(__LINE__, Ad *= D10.f, (DOUBLE)272.);
	D10.f = 16; Ad = 17;
	D10.f *= Ad;
	dequals(__LINE__, D10.f, (FLOAT)272.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Ad, (FLOAT)272.);
	Ad = 17; D11.d = 17;
	Ad *= D11.d;
	dequals(__LINE__, Ad, (DOUBLE)289.);
	Ad = 17;
	dequals(__LINE__, Ad *= D11.d, (DOUBLE)289.);
	D11.d = 17; Ad = 17;
	D11.d *= Ad;
	dequals(__LINE__, D11.d, (DOUBLE)289.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Ad, (DOUBLE)289.);
#if ANSI
	Ad = 17; D12.ld = 18;
	Ad *= D12.ld;
	dequals(__LINE__, Ad, (DOUBLE)306.);
	Ad = 17;
	dequals(__LINE__, Ad *= D12.ld, (DOUBLE)306.);
	D12.ld = 18; Ad = 17;
	D12.ld *= Ad;
	ldequals(__LINE__, D12.ld, (LDOUBLE)306.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Ad, (LDOUBLE)306.L);
#endif
#if ANSI
	Ald = 18; D1.c = 7;
	Ald *= D1.c;
	ldequals(__LINE__, Ald, (LDOUBLE)126.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D1.c, (LDOUBLE)126.L);
	D1.c = 7; Ald = 18;
	D1.c *= Ald;
	iequals(__LINE__, D1.c, (CHAR)126);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Ald, (CHAR)126);
#endif
#if ANSI
	Ald = 15; D2.sc = 8;
	Ald *= D2.sc;
	ldequals(__LINE__, Ald, (LDOUBLE)120.L);
	Ald = 15;
	ldequals(__LINE__, Ald *= D2.sc, (LDOUBLE)120.L);
	D2.sc = 8; Ald = 15;
	D2.sc *= Ald;
	iequals(__LINE__, D2.sc, (SCHAR)120);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Ald, (SCHAR)120);
#endif
#if ANSI
	Ald = 18; D3.s = 9;
	Ald *= D3.s;
	ldequals(__LINE__, Ald, (LDOUBLE)162.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D3.s, (LDOUBLE)162.L);
	D3.s = 9; Ald = 18;
	D3.s *= Ald;
	iequals(__LINE__, D3.s, (SHORT)162);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Ald, (SHORT)162);
#endif
#if ANSI
	Ald = 18; D4.i = 10;
	Ald *= D4.i;
	ldequals(__LINE__, Ald, (LDOUBLE)180.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D4.i, (LDOUBLE)180.L);
	D4.i = 10; Ald = 18;
	D4.i *= Ald;
	iequals(__LINE__, D4.i, (INT)180);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Ald, (INT)180);
#endif
#if ANSI
	Ald = 18; D5.uc = 11;
	Ald *= D5.uc;
	ldequals(__LINE__, Ald, (LDOUBLE)198.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D5.uc, (LDOUBLE)198.L);
	D5.uc = 11; Ald = 18;
	D5.uc *= Ald;
	iequals(__LINE__, D5.uc, (UCHAR)198);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Ald, (UCHAR)198);
#endif
#if ANSI
	Ald = 18; D6.us = 12;
	Ald *= D6.us;
	ldequals(__LINE__, Ald, (LDOUBLE)216.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D6.us, (LDOUBLE)216.L);
	D6.us = 12; Ald = 18;
	D6.us *= Ald;
	iequals(__LINE__, D6.us, (USHORT)216);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Ald, (USHORT)216);
#endif
#if ANSI
	Ald = 18; D7.ui = 13;
	Ald *= D7.ui;
	ldequals(__LINE__, Ald, (LDOUBLE)234.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D7.ui, (LDOUBLE)234.L);
	D7.ui = 13; Ald = 18;
	D7.ui *= Ald;
	iequals(__LINE__, D7.ui, (UINT)234);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Ald, (UINT)234);
#endif
#if ANSI
	Ald = 18; D8.l = 14;
	Ald *= D8.l;
	ldequals(__LINE__, Ald, (LDOUBLE)252.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D8.l, (LDOUBLE)252.L);
	D8.l = 14; Ald = 18;
	D8.l *= Ald;
	lequals(__LINE__, D8.l, (LONG)252L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Ald, (LONG)252L);
#endif
#if ANSI
	Ald = 18; D9.ul = 15;
	Ald *= D9.ul;
	ldequals(__LINE__, Ald, (LDOUBLE)270.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D9.ul, (LDOUBLE)270.L);
	D9.ul = 15; Ald = 18;
	D9.ul *= Ald;
	lequals(__LINE__, D9.ul, (ULONG)270L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Ald, (ULONG)270L);
#endif
#if ANSI
	Ald = 18; D10.f = 16;
	Ald *= D10.f;
	ldequals(__LINE__, Ald, (LDOUBLE)288.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D10.f, (LDOUBLE)288.L);
	D10.f = 16; Ald = 18;
	D10.f *= Ald;
	dequals(__LINE__, D10.f, (FLOAT)288.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Ald, (FLOAT)288.);
#endif
#if ANSI
	Ald = 18; D11.d = 17;
	Ald *= D11.d;
	ldequals(__LINE__, Ald, (LDOUBLE)306.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D11.d, (LDOUBLE)306.L);
	D11.d = 17; Ald = 18;
	D11.d *= Ald;
	dequals(__LINE__, D11.d, (DOUBLE)306.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Ald, (DOUBLE)306.);
#endif
#if ANSI
	Ald = 18; D12.ld = 18;
	Ald *= D12.ld;
	ldequals(__LINE__, Ald, (LDOUBLE)324.L);
	Ald = 18;
	ldequals(__LINE__, Ald *= D12.ld, (LDOUBLE)324.L);
	D12.ld = 18; Ald = 18;
	D12.ld *= Ald;
	ldequals(__LINE__, D12.ld, (LDOUBLE)324.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Ald, (LDOUBLE)324.L);
#endif
	}}
	return 	report(Filename);
	}
