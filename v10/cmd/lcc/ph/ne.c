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
	auto struct {
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
		} F;
	F.c = 7;
#if ANSI
	F.sc = 8;
#endif
	F.s = 9;
	F.i = 10;
	F.uc = 11;
	F.us = 12;
	F.ui = 13;
	F.l = 14;
	F.ul = 15;
	F.f = 16;
	F.d = 17;
#if ANSI
	F.ld = 18;
#endif
	{
	auto CHAR Jc[12][12];
	int x_0 = 0;
	int y_0 = 0;
#if ANSI
	auto SCHAR Jsc[12][12];
	int x_1 = 1;
	int y_1 = 1;
#endif
	auto SHORT Js[12][12];
	int x_2 = 2;
	int y_2 = 2;
	auto INT Ji[12][12];
	int x_3 = 3;
	int y_3 = 3;
	auto UCHAR Juc[12][12];
	int x_4 = 4;
	int y_4 = 4;
	auto USHORT Jus[12][12];
	int x_5 = 5;
	int y_5 = 5;
	auto UINT Jui[12][12];
	int x_6 = 6;
	int y_6 = 6;
	auto LONG Jl[12][12];
	int x_7 = 7;
	int y_7 = 7;
	auto ULONG Jul[12][12];
	int x_8 = 8;
	int y_8 = 8;
	auto FLOAT Jf[12][12];
	int x_9 = 9;
	int y_9 = 9;
	auto DOUBLE Jd[12][12];
	int x_10 = 10;
	int y_10 = 10;
#if ANSI
	auto LDOUBLE Jld[12][12];
	int x_11 = 11;
	int y_11 = 11;
#endif
	Jc[x_0][y_0] = 7;
#if ANSI
	Jsc[x_1][y_1] = 8;
#endif
	Js[x_2][y_2] = 9;
	Ji[x_3][y_3] = 10;
	Juc[x_4][y_4] = 11;
	Jus[x_5][y_5] = 12;
	Jui[x_6][y_6] = 13;
	Jl[x_7][y_7] = 14;
	Jul[x_8][y_8] = 15;
	Jf[x_9][y_9] = 16;
	Jd[x_10][y_10] = 17;
#if ANSI
	Jld[x_11][y_11] = 18;
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto struct auto array2 ne ";
	iequals(__LINE__, F.c != Jc[x_0][y_0], 0);
	iequals(__LINE__, Jc[x_0][y_0] != F.c, 0);
	if (F.c != Jc[x_0][y_0]) complain(__LINE__);
	if (Jc[x_0][y_0] != F.c) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.c != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.c, 1);
	if (F.c != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.c); else complain(__LINE__);
#endif
	iequals(__LINE__, F.c != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.c, 1);
	if (F.c != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.c); else complain(__LINE__);
	iequals(__LINE__, F.c != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.c, 1);
	if (F.c != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.c); else complain(__LINE__);
	iequals(__LINE__, F.c != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.c, 1);
	if (F.c != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.c); else complain(__LINE__);
	iequals(__LINE__, F.c != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.c, 1);
	if (F.c != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.c); else complain(__LINE__);
	iequals(__LINE__, F.c != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.c, 1);
	if (F.c != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.c); else complain(__LINE__);
	iequals(__LINE__, F.c != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.c, 1);
	if (F.c != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.c); else complain(__LINE__);
	iequals(__LINE__, F.c != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.c, 1);
	if (F.c != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.c); else complain(__LINE__);
	iequals(__LINE__, F.c != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.c, 1);
	if (F.c != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.c); else complain(__LINE__);
	iequals(__LINE__, F.c != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.c, 1);
	if (F.c != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.c); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.c != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.c, 1);
	if (F.c != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.c); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.sc, 1);
	if (F.sc != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Jsc[x_1][y_1], 0);
	iequals(__LINE__, Jsc[x_1][y_1] != F.sc, 0);
	if (F.sc != Jsc[x_1][y_1]) complain(__LINE__);
	if (Jsc[x_1][y_1] != F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.sc, 1);
	if (F.sc != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.sc, 1);
	if (F.sc != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.sc, 1);
	if (F.sc != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.sc, 1);
	if (F.sc != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.sc, 1);
	if (F.sc != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.sc, 1);
	if (F.sc != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.sc, 1);
	if (F.sc != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.sc, 1);
	if (F.sc != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.sc, 1);
	if (F.sc != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.sc, 1);
	if (F.sc != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.sc); else complain(__LINE__);
#endif
	iequals(__LINE__, F.s != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.s, 1);
	if (F.s != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.s); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.s != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.s, 1);
	if (F.s != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.s); else complain(__LINE__);
#endif
	iequals(__LINE__, F.s != Js[x_2][y_2], 0);
	iequals(__LINE__, Js[x_2][y_2] != F.s, 0);
	if (F.s != Js[x_2][y_2]) complain(__LINE__);
	if (Js[x_2][y_2] != F.s) complain(__LINE__);
	iequals(__LINE__, F.s != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.s, 1);
	if (F.s != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.s); else complain(__LINE__);
	iequals(__LINE__, F.s != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.s, 1);
	if (F.s != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.s); else complain(__LINE__);
	iequals(__LINE__, F.s != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.s, 1);
	if (F.s != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.s); else complain(__LINE__);
	iequals(__LINE__, F.s != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.s, 1);
	if (F.s != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.s); else complain(__LINE__);
	iequals(__LINE__, F.s != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.s, 1);
	if (F.s != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.s); else complain(__LINE__);
	iequals(__LINE__, F.s != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.s, 1);
	if (F.s != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.s); else complain(__LINE__);
	iequals(__LINE__, F.s != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.s, 1);
	if (F.s != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.s); else complain(__LINE__);
	iequals(__LINE__, F.s != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.s, 1);
	if (F.s != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.s); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.s != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.s, 1);
	if (F.s != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.s); else complain(__LINE__);
#endif
	iequals(__LINE__, F.i != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.i, 1);
	if (F.i != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.i); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.i != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.i, 1);
	if (F.i != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.i); else complain(__LINE__);
#endif
	iequals(__LINE__, F.i != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.i, 1);
	if (F.i != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.i); else complain(__LINE__);
	iequals(__LINE__, F.i != Ji[x_3][y_3], 0);
	iequals(__LINE__, Ji[x_3][y_3] != F.i, 0);
	if (F.i != Ji[x_3][y_3]) complain(__LINE__);
	if (Ji[x_3][y_3] != F.i) complain(__LINE__);
	iequals(__LINE__, F.i != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.i, 1);
	if (F.i != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.i); else complain(__LINE__);
	iequals(__LINE__, F.i != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.i, 1);
	if (F.i != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.i); else complain(__LINE__);
	iequals(__LINE__, F.i != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.i, 1);
	if (F.i != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.i); else complain(__LINE__);
	iequals(__LINE__, F.i != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.i, 1);
	if (F.i != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.i); else complain(__LINE__);
	iequals(__LINE__, F.i != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.i, 1);
	if (F.i != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.i); else complain(__LINE__);
	iequals(__LINE__, F.i != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.i, 1);
	if (F.i != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.i); else complain(__LINE__);
	iequals(__LINE__, F.i != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.i, 1);
	if (F.i != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.i); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.i != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.i, 1);
	if (F.i != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.i); else complain(__LINE__);
#endif
	iequals(__LINE__, F.uc != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.uc, 1);
	if (F.uc != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.uc); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.uc != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.uc, 1);
	if (F.uc != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.uc); else complain(__LINE__);
#endif
	iequals(__LINE__, F.uc != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.uc, 1);
	if (F.uc != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.uc, 1);
	if (F.uc != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc != Juc[x_4][y_4], 0);
	iequals(__LINE__, Juc[x_4][y_4] != F.uc, 0);
	if (F.uc != Juc[x_4][y_4]) complain(__LINE__);
	if (Juc[x_4][y_4] != F.uc) complain(__LINE__);
	iequals(__LINE__, F.uc != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.uc, 1);
	if (F.uc != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.uc, 1);
	if (F.uc != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.uc, 1);
	if (F.uc != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.uc, 1);
	if (F.uc != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.uc, 1);
	if (F.uc != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.uc, 1);
	if (F.uc != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.uc); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.uc != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.uc, 1);
	if (F.uc != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.uc); else complain(__LINE__);
#endif
	iequals(__LINE__, F.us != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.us, 1);
	if (F.us != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.us); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.us != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.us, 1);
	if (F.us != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.us); else complain(__LINE__);
#endif
	iequals(__LINE__, F.us != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.us, 1);
	if (F.us != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.us); else complain(__LINE__);
	iequals(__LINE__, F.us != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.us, 1);
	if (F.us != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.us); else complain(__LINE__);
	iequals(__LINE__, F.us != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.us, 1);
	if (F.us != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.us); else complain(__LINE__);
	iequals(__LINE__, F.us != Jus[x_5][y_5], 0);
	iequals(__LINE__, Jus[x_5][y_5] != F.us, 0);
	if (F.us != Jus[x_5][y_5]) complain(__LINE__);
	if (Jus[x_5][y_5] != F.us) complain(__LINE__);
	iequals(__LINE__, F.us != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.us, 1);
	if (F.us != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.us); else complain(__LINE__);
	iequals(__LINE__, F.us != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.us, 1);
	if (F.us != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.us); else complain(__LINE__);
	iequals(__LINE__, F.us != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.us, 1);
	if (F.us != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.us); else complain(__LINE__);
	iequals(__LINE__, F.us != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.us, 1);
	if (F.us != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.us); else complain(__LINE__);
	iequals(__LINE__, F.us != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.us, 1);
	if (F.us != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.us); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.us != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.us, 1);
	if (F.us != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.us); else complain(__LINE__);
#endif
	iequals(__LINE__, F.ui != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.ui, 1);
	if (F.ui != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.ui); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.ui != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.ui, 1);
	if (F.ui != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.ui); else complain(__LINE__);
#endif
	iequals(__LINE__, F.ui != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.ui, 1);
	if (F.ui != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.ui, 1);
	if (F.ui != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.ui, 1);
	if (F.ui != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.ui, 1);
	if (F.ui != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui != Jui[x_6][y_6], 0);
	iequals(__LINE__, Jui[x_6][y_6] != F.ui, 0);
	if (F.ui != Jui[x_6][y_6]) complain(__LINE__);
	if (Jui[x_6][y_6] != F.ui) complain(__LINE__);
	iequals(__LINE__, F.ui != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.ui, 1);
	if (F.ui != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.ui, 1);
	if (F.ui != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.ui, 1);
	if (F.ui != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.ui, 1);
	if (F.ui != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.ui); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.ui != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.ui, 1);
	if (F.ui != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.ui); else complain(__LINE__);
#endif
	iequals(__LINE__, F.l != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.l, 1);
	if (F.l != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.l); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.l != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.l, 1);
	if (F.l != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.l); else complain(__LINE__);
#endif
	iequals(__LINE__, F.l != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.l, 1);
	if (F.l != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.l); else complain(__LINE__);
	iequals(__LINE__, F.l != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.l, 1);
	if (F.l != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.l); else complain(__LINE__);
	iequals(__LINE__, F.l != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.l, 1);
	if (F.l != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.l); else complain(__LINE__);
	iequals(__LINE__, F.l != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.l, 1);
	if (F.l != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.l); else complain(__LINE__);
	iequals(__LINE__, F.l != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.l, 1);
	if (F.l != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.l); else complain(__LINE__);
	iequals(__LINE__, F.l != Jl[x_7][y_7], 0);
	iequals(__LINE__, Jl[x_7][y_7] != F.l, 0);
	if (F.l != Jl[x_7][y_7]) complain(__LINE__);
	if (Jl[x_7][y_7] != F.l) complain(__LINE__);
	iequals(__LINE__, F.l != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.l, 1);
	if (F.l != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.l); else complain(__LINE__);
	iequals(__LINE__, F.l != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.l, 1);
	if (F.l != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.l); else complain(__LINE__);
	iequals(__LINE__, F.l != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.l, 1);
	if (F.l != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.l); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.l != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.l, 1);
	if (F.l != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.l); else complain(__LINE__);
#endif
	iequals(__LINE__, F.ul != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.ul, 1);
	if (F.ul != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.ul); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.ul != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.ul, 1);
	if (F.ul != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.ul); else complain(__LINE__);
#endif
	iequals(__LINE__, F.ul != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.ul, 1);
	if (F.ul != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.ul, 1);
	if (F.ul != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.ul, 1);
	if (F.ul != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.ul, 1);
	if (F.ul != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.ul, 1);
	if (F.ul != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.ul, 1);
	if (F.ul != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul != Jul[x_8][y_8], 0);
	iequals(__LINE__, Jul[x_8][y_8] != F.ul, 0);
	if (F.ul != Jul[x_8][y_8]) complain(__LINE__);
	if (Jul[x_8][y_8] != F.ul) complain(__LINE__);
	iequals(__LINE__, F.ul != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.ul, 1);
	if (F.ul != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.ul, 1);
	if (F.ul != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.ul); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.ul != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.ul, 1);
	if (F.ul != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.ul); else complain(__LINE__);
#endif
	iequals(__LINE__, F.f != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.f, 1);
	if (F.f != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.f); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.f != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.f, 1);
	if (F.f != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.f); else complain(__LINE__);
#endif
	iequals(__LINE__, F.f != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.f, 1);
	if (F.f != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.f); else complain(__LINE__);
	iequals(__LINE__, F.f != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.f, 1);
	if (F.f != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.f); else complain(__LINE__);
	iequals(__LINE__, F.f != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.f, 1);
	if (F.f != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.f); else complain(__LINE__);
	iequals(__LINE__, F.f != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.f, 1);
	if (F.f != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.f); else complain(__LINE__);
	iequals(__LINE__, F.f != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.f, 1);
	if (F.f != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.f); else complain(__LINE__);
	iequals(__LINE__, F.f != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.f, 1);
	if (F.f != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.f); else complain(__LINE__);
	iequals(__LINE__, F.f != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.f, 1);
	if (F.f != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.f); else complain(__LINE__);
	iequals(__LINE__, F.f != Jf[x_9][y_9], 0);
	iequals(__LINE__, Jf[x_9][y_9] != F.f, 0);
	if (F.f != Jf[x_9][y_9]) complain(__LINE__);
	if (Jf[x_9][y_9] != F.f) complain(__LINE__);
	iequals(__LINE__, F.f != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.f, 1);
	if (F.f != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.f); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.f != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.f, 1);
	if (F.f != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.f); else complain(__LINE__);
#endif
	iequals(__LINE__, F.d != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.d, 1);
	if (F.d != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.d); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.d != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.d, 1);
	if (F.d != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.d); else complain(__LINE__);
#endif
	iequals(__LINE__, F.d != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.d, 1);
	if (F.d != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.d); else complain(__LINE__);
	iequals(__LINE__, F.d != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.d, 1);
	if (F.d != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.d); else complain(__LINE__);
	iequals(__LINE__, F.d != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.d, 1);
	if (F.d != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.d); else complain(__LINE__);
	iequals(__LINE__, F.d != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.d, 1);
	if (F.d != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.d); else complain(__LINE__);
	iequals(__LINE__, F.d != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.d, 1);
	if (F.d != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.d); else complain(__LINE__);
	iequals(__LINE__, F.d != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.d, 1);
	if (F.d != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.d); else complain(__LINE__);
	iequals(__LINE__, F.d != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.d, 1);
	if (F.d != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.d); else complain(__LINE__);
	iequals(__LINE__, F.d != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.d, 1);
	if (F.d != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.d); else complain(__LINE__);
	iequals(__LINE__, F.d != Jd[x_10][y_10], 0);
	iequals(__LINE__, Jd[x_10][y_10] != F.d, 0);
	if (F.d != Jd[x_10][y_10]) complain(__LINE__);
	if (Jd[x_10][y_10] != F.d) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.d != Jld[x_11][y_11], 1);
	iequals(__LINE__, Jld[x_11][y_11] != F.d, 1);
	if (F.d != Jld[x_11][y_11]); else complain(__LINE__);
	if (Jld[x_11][y_11] != F.d); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Jc[x_0][y_0], 1);
	iequals(__LINE__, Jc[x_0][y_0] != F.ld, 1);
	if (F.ld != Jc[x_0][y_0]); else complain(__LINE__);
	if (Jc[x_0][y_0] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Jsc[x_1][y_1], 1);
	iequals(__LINE__, Jsc[x_1][y_1] != F.ld, 1);
	if (F.ld != Jsc[x_1][y_1]); else complain(__LINE__);
	if (Jsc[x_1][y_1] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Js[x_2][y_2], 1);
	iequals(__LINE__, Js[x_2][y_2] != F.ld, 1);
	if (F.ld != Js[x_2][y_2]); else complain(__LINE__);
	if (Js[x_2][y_2] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Ji[x_3][y_3], 1);
	iequals(__LINE__, Ji[x_3][y_3] != F.ld, 1);
	if (F.ld != Ji[x_3][y_3]); else complain(__LINE__);
	if (Ji[x_3][y_3] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Juc[x_4][y_4], 1);
	iequals(__LINE__, Juc[x_4][y_4] != F.ld, 1);
	if (F.ld != Juc[x_4][y_4]); else complain(__LINE__);
	if (Juc[x_4][y_4] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Jus[x_5][y_5], 1);
	iequals(__LINE__, Jus[x_5][y_5] != F.ld, 1);
	if (F.ld != Jus[x_5][y_5]); else complain(__LINE__);
	if (Jus[x_5][y_5] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Jui[x_6][y_6], 1);
	iequals(__LINE__, Jui[x_6][y_6] != F.ld, 1);
	if (F.ld != Jui[x_6][y_6]); else complain(__LINE__);
	if (Jui[x_6][y_6] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Jl[x_7][y_7], 1);
	iequals(__LINE__, Jl[x_7][y_7] != F.ld, 1);
	if (F.ld != Jl[x_7][y_7]); else complain(__LINE__);
	if (Jl[x_7][y_7] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Jul[x_8][y_8], 1);
	iequals(__LINE__, Jul[x_8][y_8] != F.ld, 1);
	if (F.ld != Jul[x_8][y_8]); else complain(__LINE__);
	if (Jul[x_8][y_8] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Jf[x_9][y_9], 1);
	iequals(__LINE__, Jf[x_9][y_9] != F.ld, 1);
	if (F.ld != Jf[x_9][y_9]); else complain(__LINE__);
	if (Jf[x_9][y_9] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Jd[x_10][y_10], 1);
	iequals(__LINE__, Jd[x_10][y_10] != F.ld, 1);
	if (F.ld != Jd[x_10][y_10]); else complain(__LINE__);
	if (Jd[x_10][y_10] != F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld != Jld[x_11][y_11], 0);
	iequals(__LINE__, Jld[x_11][y_11] != F.ld, 0);
	if (F.ld != Jld[x_11][y_11]) complain(__LINE__);
	if (Jld[x_11][y_11] != F.ld) complain(__LINE__);
#endif
	}}
	return 	report(Filename);
	}
