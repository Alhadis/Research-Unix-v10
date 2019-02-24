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
	Filename =  " auto scalar auto union timeq3 ";
	D9.ul = 15; Aus = 12;
	D9.ul *= Aus;
	lequals(__LINE__, D9.ul, (ULONG)180L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Aus, (ULONG)180L);
	Aus = 12; D10.f = 16;
	Aus *= D10.f;
	iequals(__LINE__, Aus, (USHORT)192);
	Aus = 12;
	iequals(__LINE__, Aus *= D10.f, (USHORT)192);
	D10.f = 16; Aus = 12;
	D10.f *= Aus;
	dequals(__LINE__, D10.f, (FLOAT)192.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Aus, (FLOAT)192.);
	Aus = 12; D11.d = 17;
	Aus *= D11.d;
	iequals(__LINE__, Aus, (USHORT)204);
	Aus = 12;
	iequals(__LINE__, Aus *= D11.d, (USHORT)204);
	D11.d = 17; Aus = 12;
	D11.d *= Aus;
	dequals(__LINE__, D11.d, (DOUBLE)204.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Aus, (DOUBLE)204.);
#if ANSI
	Aus = 12; D12.ld = 18;
	Aus *= D12.ld;
	iequals(__LINE__, Aus, (USHORT)216);
	Aus = 12;
	iequals(__LINE__, Aus *= D12.ld, (USHORT)216);
	D12.ld = 18; Aus = 12;
	D12.ld *= Aus;
	ldequals(__LINE__, D12.ld, (LDOUBLE)216.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Aus, (LDOUBLE)216.L);
#endif
	Aui = 13; D1.c = 7;
	Aui *= D1.c;
	iequals(__LINE__, Aui, (UINT)91);
	Aui = 13;
	iequals(__LINE__, Aui *= D1.c, (UINT)91);
	D1.c = 7; Aui = 13;
	D1.c *= Aui;
	iequals(__LINE__, D1.c, (CHAR)91);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Aui, (CHAR)91);
#if ANSI
	Aui = 13; D2.sc = 8;
	Aui *= D2.sc;
	iequals(__LINE__, Aui, (UINT)104);
	Aui = 13;
	iequals(__LINE__, Aui *= D2.sc, (UINT)104);
	D2.sc = 8; Aui = 13;
	D2.sc *= Aui;
	iequals(__LINE__, D2.sc, (SCHAR)104);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Aui, (SCHAR)104);
#endif
	Aui = 13; D3.s = 9;
	Aui *= D3.s;
	iequals(__LINE__, Aui, (UINT)117);
	Aui = 13;
	iequals(__LINE__, Aui *= D3.s, (UINT)117);
	D3.s = 9; Aui = 13;
	D3.s *= Aui;
	iequals(__LINE__, D3.s, (SHORT)117);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Aui, (SHORT)117);
	Aui = 13; D4.i = 10;
	Aui *= D4.i;
	iequals(__LINE__, Aui, (UINT)130);
	Aui = 13;
	iequals(__LINE__, Aui *= D4.i, (UINT)130);
	D4.i = 10; Aui = 13;
	D4.i *= Aui;
	iequals(__LINE__, D4.i, (INT)130);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Aui, (INT)130);
	Aui = 13; D5.uc = 11;
	Aui *= D5.uc;
	iequals(__LINE__, Aui, (UINT)143);
	Aui = 13;
	iequals(__LINE__, Aui *= D5.uc, (UINT)143);
	D5.uc = 11; Aui = 13;
	D5.uc *= Aui;
	iequals(__LINE__, D5.uc, (UCHAR)143);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Aui, (UCHAR)143);
	Aui = 13; D6.us = 12;
	Aui *= D6.us;
	iequals(__LINE__, Aui, (UINT)156);
	Aui = 13;
	iequals(__LINE__, Aui *= D6.us, (UINT)156);
	D6.us = 12; Aui = 13;
	D6.us *= Aui;
	iequals(__LINE__, D6.us, (USHORT)156);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Aui, (USHORT)156);
	Aui = 13; D7.ui = 13;
	Aui *= D7.ui;
	iequals(__LINE__, Aui, (UINT)169);
	Aui = 13;
	iequals(__LINE__, Aui *= D7.ui, (UINT)169);
	D7.ui = 13; Aui = 13;
	D7.ui *= Aui;
	iequals(__LINE__, D7.ui, (UINT)169);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Aui, (UINT)169);
	Aui = 13; D8.l = 14;
	Aui *= D8.l;
	iequals(__LINE__, Aui, (UINT)182);
	Aui = 13;
	iequals(__LINE__, Aui *= D8.l, (UINT)182);
	D8.l = 14; Aui = 13;
	D8.l *= Aui;
	lequals(__LINE__, D8.l, (LONG)182L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Aui, (LONG)182L);
	Aui = 13; D9.ul = 15;
	Aui *= D9.ul;
	iequals(__LINE__, Aui, (UINT)195);
	Aui = 13;
	iequals(__LINE__, Aui *= D9.ul, (UINT)195);
	D9.ul = 15; Aui = 13;
	D9.ul *= Aui;
	lequals(__LINE__, D9.ul, (ULONG)195L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Aui, (ULONG)195L);
	Aui = 13; D10.f = 16;
	Aui *= D10.f;
	iequals(__LINE__, Aui, (UINT)208);
	Aui = 13;
	iequals(__LINE__, Aui *= D10.f, (UINT)208);
	D10.f = 16; Aui = 13;
	D10.f *= Aui;
	dequals(__LINE__, D10.f, (FLOAT)208.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Aui, (FLOAT)208.);
	Aui = 13; D11.d = 17;
	Aui *= D11.d;
	iequals(__LINE__, Aui, (UINT)221);
	Aui = 13;
	iequals(__LINE__, Aui *= D11.d, (UINT)221);
	D11.d = 17; Aui = 13;
	D11.d *= Aui;
	dequals(__LINE__, D11.d, (DOUBLE)221.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Aui, (DOUBLE)221.);
#if ANSI
	Aui = 13; D12.ld = 18;
	Aui *= D12.ld;
	iequals(__LINE__, Aui, (UINT)234);
	Aui = 13;
	iequals(__LINE__, Aui *= D12.ld, (UINT)234);
	D12.ld = 18; Aui = 13;
	D12.ld *= Aui;
	ldequals(__LINE__, D12.ld, (LDOUBLE)234.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Aui, (LDOUBLE)234.L);
#endif
	Al = 14; D1.c = 7;
	Al *= D1.c;
	lequals(__LINE__, Al, (LONG)98L);
	Al = 14;
	lequals(__LINE__, Al *= D1.c, (LONG)98L);
	D1.c = 7; Al = 14;
	D1.c *= Al;
	iequals(__LINE__, D1.c, (CHAR)98);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Al, (CHAR)98);
#if ANSI
	Al = 14; D2.sc = 8;
	Al *= D2.sc;
	lequals(__LINE__, Al, (LONG)112L);
	Al = 14;
	lequals(__LINE__, Al *= D2.sc, (LONG)112L);
	D2.sc = 8; Al = 14;
	D2.sc *= Al;
	iequals(__LINE__, D2.sc, (SCHAR)112);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Al, (SCHAR)112);
#endif
	Al = 14; D3.s = 9;
	Al *= D3.s;
	lequals(__LINE__, Al, (LONG)126L);
	Al = 14;
	lequals(__LINE__, Al *= D3.s, (LONG)126L);
	D3.s = 9; Al = 14;
	D3.s *= Al;
	iequals(__LINE__, D3.s, (SHORT)126);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Al, (SHORT)126);
	Al = 14; D4.i = 10;
	Al *= D4.i;
	lequals(__LINE__, Al, (LONG)140L);
	Al = 14;
	lequals(__LINE__, Al *= D4.i, (LONG)140L);
	D4.i = 10; Al = 14;
	D4.i *= Al;
	iequals(__LINE__, D4.i, (INT)140);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Al, (INT)140);
	Al = 14; D5.uc = 11;
	Al *= D5.uc;
	lequals(__LINE__, Al, (LONG)154L);
	Al = 14;
	lequals(__LINE__, Al *= D5.uc, (LONG)154L);
	D5.uc = 11; Al = 14;
	D5.uc *= Al;
	iequals(__LINE__, D5.uc, (UCHAR)154);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Al, (UCHAR)154);
	Al = 14; D6.us = 12;
	Al *= D6.us;
	lequals(__LINE__, Al, (LONG)168L);
	Al = 14;
	lequals(__LINE__, Al *= D6.us, (LONG)168L);
	D6.us = 12; Al = 14;
	D6.us *= Al;
	iequals(__LINE__, D6.us, (USHORT)168);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Al, (USHORT)168);
	Al = 14; D7.ui = 13;
	Al *= D7.ui;
	lequals(__LINE__, Al, (LONG)182L);
	Al = 14;
	lequals(__LINE__, Al *= D7.ui, (LONG)182L);
	D7.ui = 13; Al = 14;
	D7.ui *= Al;
	iequals(__LINE__, D7.ui, (UINT)182);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Al, (UINT)182);
	Al = 14; D8.l = 14;
	Al *= D8.l;
	lequals(__LINE__, Al, (LONG)196L);
	Al = 14;
	lequals(__LINE__, Al *= D8.l, (LONG)196L);
	D8.l = 14; Al = 14;
	D8.l *= Al;
	lequals(__LINE__, D8.l, (LONG)196L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Al, (LONG)196L);
	Al = 14; D9.ul = 15;
	Al *= D9.ul;
	lequals(__LINE__, Al, (LONG)210L);
	Al = 14;
	lequals(__LINE__, Al *= D9.ul, (LONG)210L);
	D9.ul = 15; Al = 14;
	D9.ul *= Al;
	lequals(__LINE__, D9.ul, (ULONG)210L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Al, (ULONG)210L);
	Al = 14; D10.f = 16;
	Al *= D10.f;
	lequals(__LINE__, Al, (LONG)224L);
	Al = 14;
	lequals(__LINE__, Al *= D10.f, (LONG)224L);
	D10.f = 16; Al = 14;
	D10.f *= Al;
	dequals(__LINE__, D10.f, (FLOAT)224.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Al, (FLOAT)224.);
	Al = 14; D11.d = 17;
	Al *= D11.d;
	lequals(__LINE__, Al, (LONG)238L);
	Al = 14;
	lequals(__LINE__, Al *= D11.d, (LONG)238L);
	D11.d = 17; Al = 14;
	D11.d *= Al;
	dequals(__LINE__, D11.d, (DOUBLE)238.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Al, (DOUBLE)238.);
#if ANSI
	Al = 14; D12.ld = 18;
	Al *= D12.ld;
	lequals(__LINE__, Al, (LONG)252L);
	Al = 14;
	lequals(__LINE__, Al *= D12.ld, (LONG)252L);
	D12.ld = 18; Al = 14;
	D12.ld *= Al;
	ldequals(__LINE__, D12.ld, (LDOUBLE)252.L);
	D12.ld = 18;
	ldequals(__LINE__, D12.ld *= Al, (LDOUBLE)252.L);
#endif
	Aul = 15; D1.c = 7;
	Aul *= D1.c;
	lequals(__LINE__, Aul, (ULONG)105L);
	Aul = 15;
	lequals(__LINE__, Aul *= D1.c, (ULONG)105L);
	D1.c = 7; Aul = 15;
	D1.c *= Aul;
	iequals(__LINE__, D1.c, (CHAR)105);
	D1.c = 7;
	iequals(__LINE__, D1.c *= Aul, (CHAR)105);
#if ANSI
	Aul = 15; D2.sc = 8;
	Aul *= D2.sc;
	lequals(__LINE__, Aul, (ULONG)120L);
	Aul = 15;
	lequals(__LINE__, Aul *= D2.sc, (ULONG)120L);
	D2.sc = 8; Aul = 15;
	D2.sc *= Aul;
	iequals(__LINE__, D2.sc, (SCHAR)120);
	D2.sc = 8;
	iequals(__LINE__, D2.sc *= Aul, (SCHAR)120);
#endif
	Aul = 15; D3.s = 9;
	Aul *= D3.s;
	lequals(__LINE__, Aul, (ULONG)135L);
	Aul = 15;
	lequals(__LINE__, Aul *= D3.s, (ULONG)135L);
	D3.s = 9; Aul = 15;
	D3.s *= Aul;
	iequals(__LINE__, D3.s, (SHORT)135);
	D3.s = 9;
	iequals(__LINE__, D3.s *= Aul, (SHORT)135);
	Aul = 15; D4.i = 10;
	Aul *= D4.i;
	lequals(__LINE__, Aul, (ULONG)150L);
	Aul = 15;
	lequals(__LINE__, Aul *= D4.i, (ULONG)150L);
	D4.i = 10; Aul = 15;
	D4.i *= Aul;
	iequals(__LINE__, D4.i, (INT)150);
	D4.i = 10;
	iequals(__LINE__, D4.i *= Aul, (INT)150);
	Aul = 15; D5.uc = 11;
	Aul *= D5.uc;
	lequals(__LINE__, Aul, (ULONG)165L);
	Aul = 15;
	lequals(__LINE__, Aul *= D5.uc, (ULONG)165L);
	D5.uc = 11; Aul = 15;
	D5.uc *= Aul;
	iequals(__LINE__, D5.uc, (UCHAR)165);
	D5.uc = 11;
	iequals(__LINE__, D5.uc *= Aul, (UCHAR)165);
	Aul = 15; D6.us = 12;
	Aul *= D6.us;
	lequals(__LINE__, Aul, (ULONG)180L);
	Aul = 15;
	lequals(__LINE__, Aul *= D6.us, (ULONG)180L);
	D6.us = 12; Aul = 15;
	D6.us *= Aul;
	iequals(__LINE__, D6.us, (USHORT)180);
	D6.us = 12;
	iequals(__LINE__, D6.us *= Aul, (USHORT)180);
	Aul = 15; D7.ui = 13;
	Aul *= D7.ui;
	lequals(__LINE__, Aul, (ULONG)195L);
	Aul = 15;
	lequals(__LINE__, Aul *= D7.ui, (ULONG)195L);
	D7.ui = 13; Aul = 15;
	D7.ui *= Aul;
	iequals(__LINE__, D7.ui, (UINT)195);
	D7.ui = 13;
	iequals(__LINE__, D7.ui *= Aul, (UINT)195);
	Aul = 15; D8.l = 14;
	Aul *= D8.l;
	lequals(__LINE__, Aul, (ULONG)210L);
	Aul = 15;
	lequals(__LINE__, Aul *= D8.l, (ULONG)210L);
	D8.l = 14; Aul = 15;
	D8.l *= Aul;
	lequals(__LINE__, D8.l, (LONG)210L);
	D8.l = 14;
	lequals(__LINE__, D8.l *= Aul, (LONG)210L);
	Aul = 15; D9.ul = 15;
	Aul *= D9.ul;
	lequals(__LINE__, Aul, (ULONG)225L);
	Aul = 15;
	lequals(__LINE__, Aul *= D9.ul, (ULONG)225L);
	D9.ul = 15; Aul = 15;
	D9.ul *= Aul;
	lequals(__LINE__, D9.ul, (ULONG)225L);
	D9.ul = 15;
	lequals(__LINE__, D9.ul *= Aul, (ULONG)225L);
	Aul = 15; D10.f = 16;
	Aul *= D10.f;
	lequals(__LINE__, Aul, (ULONG)240L);
	Aul = 15;
	lequals(__LINE__, Aul *= D10.f, (ULONG)240L);
	D10.f = 16; Aul = 15;
	D10.f *= Aul;
	dequals(__LINE__, D10.f, (FLOAT)240.);
	D10.f = 16;
	dequals(__LINE__, D10.f *= Aul, (FLOAT)240.);
	Aul = 15; D11.d = 17;
	Aul *= D11.d;
	lequals(__LINE__, Aul, (ULONG)255L);
	Aul = 15;
	lequals(__LINE__, Aul *= D11.d, (ULONG)255L);
	D11.d = 17; Aul = 15;
	D11.d *= Aul;
	dequals(__LINE__, D11.d, (DOUBLE)255.);
	D11.d = 17;
	dequals(__LINE__, D11.d *= Aul, (DOUBLE)255.);
#if ANSI
	Aul = 15; D12.ld = 18;
	Aul *= D12.ld;
	lequals(__LINE__, Aul, (ULONG)270L);
	Aul = 15;
	lequals(__LINE__, Aul *= D12.ld, (ULONG)270L);
#endif
	}}
	return 	report(Filename);
	}
