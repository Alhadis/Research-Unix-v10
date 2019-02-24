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
		} G, *pG = &G;
	pG->c = 7;
#if ANSI
	pG->sc = 8;
#endif
	pG->s = 9;
	pG->i = 10;
	pG->uc = 11;
	pG->us = 12;
	pG->ui = 13;
	pG->l = 14;
	pG->ul = 15;
	pG->f = 16;
	pG->d = 17;
#if ANSI
	pG->ld = 18;
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
	Filename =  " auto pstruct1 auto array2 plus ";
	iequals(__LINE__, pG->c + Jc[x_0][y_0], 14);
	iequals(__LINE__, Jc[x_0][y_0] + pG->c, 14);
#if ANSI
	iequals(__LINE__, pG->c + Jsc[x_1][y_1], 15);
	iequals(__LINE__, Jsc[x_1][y_1] + pG->c, 15);
#endif
	iequals(__LINE__, pG->c + Js[x_2][y_2], 16);
	iequals(__LINE__, Js[x_2][y_2] + pG->c, 16);
	iequals(__LINE__, pG->c + Ji[x_3][y_3], 17);
	iequals(__LINE__, Ji[x_3][y_3] + pG->c, 17);
	iequals(__LINE__, pG->c + Juc[x_4][y_4], 18);
	iequals(__LINE__, Juc[x_4][y_4] + pG->c, 18);
	iequals(__LINE__, pG->c + Jus[x_5][y_5], 19);
	iequals(__LINE__, Jus[x_5][y_5] + pG->c, 19);
	iequals(__LINE__, pG->c + Jui[x_6][y_6], 20);
	iequals(__LINE__, Jui[x_6][y_6] + pG->c, 20);
	lequals(__LINE__, pG->c + Jl[x_7][y_7], 21L);
	lequals(__LINE__, Jl[x_7][y_7] + pG->c, 21L);
	lequals(__LINE__, pG->c + Jul[x_8][y_8], 22L);
	lequals(__LINE__, Jul[x_8][y_8] + pG->c, 22L);
	dequals(__LINE__, pG->c + Jf[x_9][y_9], 23.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->c, 23.);
	dequals(__LINE__, pG->c + Jd[x_10][y_10], 24.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->c, 24.);
#if ANSI
	ldequals(__LINE__, pG->c + Jld[x_11][y_11], 25.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->c, 25.L);
#endif
#if ANSI
	iequals(__LINE__, pG->sc + Jc[x_0][y_0], 15);
	iequals(__LINE__, Jc[x_0][y_0] + pG->sc, 15);
#endif
#if ANSI
	iequals(__LINE__, pG->sc + Jsc[x_1][y_1], 16);
	iequals(__LINE__, Jsc[x_1][y_1] + pG->sc, 16);
#endif
#if ANSI
	iequals(__LINE__, pG->sc + Js[x_2][y_2], 17);
	iequals(__LINE__, Js[x_2][y_2] + pG->sc, 17);
#endif
#if ANSI
	iequals(__LINE__, pG->sc + Ji[x_3][y_3], 18);
	iequals(__LINE__, Ji[x_3][y_3] + pG->sc, 18);
#endif
#if ANSI
	iequals(__LINE__, pG->sc + Juc[x_4][y_4], 19);
	iequals(__LINE__, Juc[x_4][y_4] + pG->sc, 19);
#endif
#if ANSI
	iequals(__LINE__, pG->sc + Jus[x_5][y_5], 20);
	iequals(__LINE__, Jus[x_5][y_5] + pG->sc, 20);
#endif
#if ANSI
	iequals(__LINE__, pG->sc + Jui[x_6][y_6], 21);
	iequals(__LINE__, Jui[x_6][y_6] + pG->sc, 21);
#endif
#if ANSI
	lequals(__LINE__, pG->sc + Jl[x_7][y_7], 22L);
	lequals(__LINE__, Jl[x_7][y_7] + pG->sc, 22L);
#endif
#if ANSI
	lequals(__LINE__, pG->sc + Jul[x_8][y_8], 23L);
	lequals(__LINE__, Jul[x_8][y_8] + pG->sc, 23L);
#endif
#if ANSI
	dequals(__LINE__, pG->sc + Jf[x_9][y_9], 24.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->sc, 24.);
#endif
#if ANSI
	dequals(__LINE__, pG->sc + Jd[x_10][y_10], 25.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->sc, 25.);
#endif
#if ANSI
	ldequals(__LINE__, pG->sc + Jld[x_11][y_11], 26.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->sc, 26.L);
#endif
	iequals(__LINE__, pG->s + Jc[x_0][y_0], 16);
	iequals(__LINE__, Jc[x_0][y_0] + pG->s, 16);
#if ANSI
	iequals(__LINE__, pG->s + Jsc[x_1][y_1], 17);
	iequals(__LINE__, Jsc[x_1][y_1] + pG->s, 17);
#endif
	iequals(__LINE__, pG->s + Js[x_2][y_2], 18);
	iequals(__LINE__, Js[x_2][y_2] + pG->s, 18);
	iequals(__LINE__, pG->s + Ji[x_3][y_3], 19);
	iequals(__LINE__, Ji[x_3][y_3] + pG->s, 19);
	iequals(__LINE__, pG->s + Juc[x_4][y_4], 20);
	iequals(__LINE__, Juc[x_4][y_4] + pG->s, 20);
	iequals(__LINE__, pG->s + Jus[x_5][y_5], 21);
	iequals(__LINE__, Jus[x_5][y_5] + pG->s, 21);
	iequals(__LINE__, pG->s + Jui[x_6][y_6], 22);
	iequals(__LINE__, Jui[x_6][y_6] + pG->s, 22);
	lequals(__LINE__, pG->s + Jl[x_7][y_7], 23L);
	lequals(__LINE__, Jl[x_7][y_7] + pG->s, 23L);
	lequals(__LINE__, pG->s + Jul[x_8][y_8], 24L);
	lequals(__LINE__, Jul[x_8][y_8] + pG->s, 24L);
	dequals(__LINE__, pG->s + Jf[x_9][y_9], 25.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->s, 25.);
	dequals(__LINE__, pG->s + Jd[x_10][y_10], 26.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->s, 26.);
#if ANSI
	ldequals(__LINE__, pG->s + Jld[x_11][y_11], 27.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->s, 27.L);
#endif
	iequals(__LINE__, pG->i + Jc[x_0][y_0], 17);
	iequals(__LINE__, Jc[x_0][y_0] + pG->i, 17);
#if ANSI
	iequals(__LINE__, pG->i + Jsc[x_1][y_1], 18);
	iequals(__LINE__, Jsc[x_1][y_1] + pG->i, 18);
#endif
	iequals(__LINE__, pG->i + Js[x_2][y_2], 19);
	iequals(__LINE__, Js[x_2][y_2] + pG->i, 19);
	iequals(__LINE__, pG->i + Ji[x_3][y_3], 20);
	iequals(__LINE__, Ji[x_3][y_3] + pG->i, 20);
	iequals(__LINE__, pG->i + Juc[x_4][y_4], 21);
	iequals(__LINE__, Juc[x_4][y_4] + pG->i, 21);
	iequals(__LINE__, pG->i + Jus[x_5][y_5], 22);
	iequals(__LINE__, Jus[x_5][y_5] + pG->i, 22);
	iequals(__LINE__, pG->i + Jui[x_6][y_6], 23);
	iequals(__LINE__, Jui[x_6][y_6] + pG->i, 23);
	lequals(__LINE__, pG->i + Jl[x_7][y_7], 24L);
	lequals(__LINE__, Jl[x_7][y_7] + pG->i, 24L);
	lequals(__LINE__, pG->i + Jul[x_8][y_8], 25L);
	lequals(__LINE__, Jul[x_8][y_8] + pG->i, 25L);
	dequals(__LINE__, pG->i + Jf[x_9][y_9], 26.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->i, 26.);
	dequals(__LINE__, pG->i + Jd[x_10][y_10], 27.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->i, 27.);
#if ANSI
	ldequals(__LINE__, pG->i + Jld[x_11][y_11], 28.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->i, 28.L);
#endif
	iequals(__LINE__, pG->uc + Jc[x_0][y_0], 18);
	iequals(__LINE__, Jc[x_0][y_0] + pG->uc, 18);
#if ANSI
	iequals(__LINE__, pG->uc + Jsc[x_1][y_1], 19);
	iequals(__LINE__, Jsc[x_1][y_1] + pG->uc, 19);
#endif
	iequals(__LINE__, pG->uc + Js[x_2][y_2], 20);
	iequals(__LINE__, Js[x_2][y_2] + pG->uc, 20);
	iequals(__LINE__, pG->uc + Ji[x_3][y_3], 21);
	iequals(__LINE__, Ji[x_3][y_3] + pG->uc, 21);
	iequals(__LINE__, pG->uc + Juc[x_4][y_4], 22);
	iequals(__LINE__, Juc[x_4][y_4] + pG->uc, 22);
	iequals(__LINE__, pG->uc + Jus[x_5][y_5], 23);
	iequals(__LINE__, Jus[x_5][y_5] + pG->uc, 23);
	iequals(__LINE__, pG->uc + Jui[x_6][y_6], 24);
	iequals(__LINE__, Jui[x_6][y_6] + pG->uc, 24);
	lequals(__LINE__, pG->uc + Jl[x_7][y_7], 25L);
	lequals(__LINE__, Jl[x_7][y_7] + pG->uc, 25L);
	lequals(__LINE__, pG->uc + Jul[x_8][y_8], 26L);
	lequals(__LINE__, Jul[x_8][y_8] + pG->uc, 26L);
	dequals(__LINE__, pG->uc + Jf[x_9][y_9], 27.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->uc, 27.);
	dequals(__LINE__, pG->uc + Jd[x_10][y_10], 28.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->uc, 28.);
#if ANSI
	ldequals(__LINE__, pG->uc + Jld[x_11][y_11], 29.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->uc, 29.L);
#endif
	iequals(__LINE__, pG->us + Jc[x_0][y_0], 19);
	iequals(__LINE__, Jc[x_0][y_0] + pG->us, 19);
#if ANSI
	iequals(__LINE__, pG->us + Jsc[x_1][y_1], 20);
	iequals(__LINE__, Jsc[x_1][y_1] + pG->us, 20);
#endif
	iequals(__LINE__, pG->us + Js[x_2][y_2], 21);
	iequals(__LINE__, Js[x_2][y_2] + pG->us, 21);
	iequals(__LINE__, pG->us + Ji[x_3][y_3], 22);
	iequals(__LINE__, Ji[x_3][y_3] + pG->us, 22);
	iequals(__LINE__, pG->us + Juc[x_4][y_4], 23);
	iequals(__LINE__, Juc[x_4][y_4] + pG->us, 23);
	iequals(__LINE__, pG->us + Jus[x_5][y_5], 24);
	iequals(__LINE__, Jus[x_5][y_5] + pG->us, 24);
	iequals(__LINE__, pG->us + Jui[x_6][y_6], 25);
	iequals(__LINE__, Jui[x_6][y_6] + pG->us, 25);
	lequals(__LINE__, pG->us + Jl[x_7][y_7], 26L);
	lequals(__LINE__, Jl[x_7][y_7] + pG->us, 26L);
	lequals(__LINE__, pG->us + Jul[x_8][y_8], 27L);
	lequals(__LINE__, Jul[x_8][y_8] + pG->us, 27L);
	dequals(__LINE__, pG->us + Jf[x_9][y_9], 28.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->us, 28.);
	dequals(__LINE__, pG->us + Jd[x_10][y_10], 29.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->us, 29.);
#if ANSI
	ldequals(__LINE__, pG->us + Jld[x_11][y_11], 30.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->us, 30.L);
#endif
	iequals(__LINE__, pG->ui + Jc[x_0][y_0], 20);
	iequals(__LINE__, Jc[x_0][y_0] + pG->ui, 20);
#if ANSI
	iequals(__LINE__, pG->ui + Jsc[x_1][y_1], 21);
	iequals(__LINE__, Jsc[x_1][y_1] + pG->ui, 21);
#endif
	iequals(__LINE__, pG->ui + Js[x_2][y_2], 22);
	iequals(__LINE__, Js[x_2][y_2] + pG->ui, 22);
	iequals(__LINE__, pG->ui + Ji[x_3][y_3], 23);
	iequals(__LINE__, Ji[x_3][y_3] + pG->ui, 23);
	iequals(__LINE__, pG->ui + Juc[x_4][y_4], 24);
	iequals(__LINE__, Juc[x_4][y_4] + pG->ui, 24);
	iequals(__LINE__, pG->ui + Jus[x_5][y_5], 25);
	iequals(__LINE__, Jus[x_5][y_5] + pG->ui, 25);
	iequals(__LINE__, pG->ui + Jui[x_6][y_6], 26);
	iequals(__LINE__, Jui[x_6][y_6] + pG->ui, 26);
	lequals(__LINE__, pG->ui + Jl[x_7][y_7], 27L);
	lequals(__LINE__, Jl[x_7][y_7] + pG->ui, 27L);
	lequals(__LINE__, pG->ui + Jul[x_8][y_8], 28L);
	lequals(__LINE__, Jul[x_8][y_8] + pG->ui, 28L);
	dequals(__LINE__, pG->ui + Jf[x_9][y_9], 29.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->ui, 29.);
	dequals(__LINE__, pG->ui + Jd[x_10][y_10], 30.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->ui, 30.);
#if ANSI
	ldequals(__LINE__, pG->ui + Jld[x_11][y_11], 31.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->ui, 31.L);
#endif
	lequals(__LINE__, pG->l + Jc[x_0][y_0], 21L);
	lequals(__LINE__, Jc[x_0][y_0] + pG->l, 21L);
#if ANSI
	lequals(__LINE__, pG->l + Jsc[x_1][y_1], 22L);
	lequals(__LINE__, Jsc[x_1][y_1] + pG->l, 22L);
#endif
	lequals(__LINE__, pG->l + Js[x_2][y_2], 23L);
	lequals(__LINE__, Js[x_2][y_2] + pG->l, 23L);
	lequals(__LINE__, pG->l + Ji[x_3][y_3], 24L);
	lequals(__LINE__, Ji[x_3][y_3] + pG->l, 24L);
	lequals(__LINE__, pG->l + Juc[x_4][y_4], 25L);
	lequals(__LINE__, Juc[x_4][y_4] + pG->l, 25L);
	lequals(__LINE__, pG->l + Jus[x_5][y_5], 26L);
	lequals(__LINE__, Jus[x_5][y_5] + pG->l, 26L);
	lequals(__LINE__, pG->l + Jui[x_6][y_6], 27L);
	lequals(__LINE__, Jui[x_6][y_6] + pG->l, 27L);
	lequals(__LINE__, pG->l + Jl[x_7][y_7], 28L);
	lequals(__LINE__, Jl[x_7][y_7] + pG->l, 28L);
	lequals(__LINE__, pG->l + Jul[x_8][y_8], 29L);
	lequals(__LINE__, Jul[x_8][y_8] + pG->l, 29L);
	dequals(__LINE__, pG->l + Jf[x_9][y_9], 30.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->l, 30.);
	dequals(__LINE__, pG->l + Jd[x_10][y_10], 31.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->l, 31.);
#if ANSI
	ldequals(__LINE__, pG->l + Jld[x_11][y_11], 32.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->l, 32.L);
#endif
	lequals(__LINE__, pG->ul + Jc[x_0][y_0], 22L);
	lequals(__LINE__, Jc[x_0][y_0] + pG->ul, 22L);
#if ANSI
	lequals(__LINE__, pG->ul + Jsc[x_1][y_1], 23L);
	lequals(__LINE__, Jsc[x_1][y_1] + pG->ul, 23L);
#endif
	lequals(__LINE__, pG->ul + Js[x_2][y_2], 24L);
	lequals(__LINE__, Js[x_2][y_2] + pG->ul, 24L);
	lequals(__LINE__, pG->ul + Ji[x_3][y_3], 25L);
	lequals(__LINE__, Ji[x_3][y_3] + pG->ul, 25L);
	lequals(__LINE__, pG->ul + Juc[x_4][y_4], 26L);
	lequals(__LINE__, Juc[x_4][y_4] + pG->ul, 26L);
	lequals(__LINE__, pG->ul + Jus[x_5][y_5], 27L);
	lequals(__LINE__, Jus[x_5][y_5] + pG->ul, 27L);
	lequals(__LINE__, pG->ul + Jui[x_6][y_6], 28L);
	lequals(__LINE__, Jui[x_6][y_6] + pG->ul, 28L);
	lequals(__LINE__, pG->ul + Jl[x_7][y_7], 29L);
	lequals(__LINE__, Jl[x_7][y_7] + pG->ul, 29L);
	lequals(__LINE__, pG->ul + Jul[x_8][y_8], 30L);
	lequals(__LINE__, Jul[x_8][y_8] + pG->ul, 30L);
	dequals(__LINE__, pG->ul + Jf[x_9][y_9], 31.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->ul, 31.);
	dequals(__LINE__, pG->ul + Jd[x_10][y_10], 32.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->ul, 32.);
#if ANSI
	ldequals(__LINE__, pG->ul + Jld[x_11][y_11], 33.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->ul, 33.L);
#endif
	dequals(__LINE__, pG->f + Jc[x_0][y_0], 23.);
	dequals(__LINE__, Jc[x_0][y_0] + pG->f, 23.);
#if ANSI
	dequals(__LINE__, pG->f + Jsc[x_1][y_1], 24.);
	dequals(__LINE__, Jsc[x_1][y_1] + pG->f, 24.);
#endif
	dequals(__LINE__, pG->f + Js[x_2][y_2], 25.);
	dequals(__LINE__, Js[x_2][y_2] + pG->f, 25.);
	dequals(__LINE__, pG->f + Ji[x_3][y_3], 26.);
	dequals(__LINE__, Ji[x_3][y_3] + pG->f, 26.);
	dequals(__LINE__, pG->f + Juc[x_4][y_4], 27.);
	dequals(__LINE__, Juc[x_4][y_4] + pG->f, 27.);
	dequals(__LINE__, pG->f + Jus[x_5][y_5], 28.);
	dequals(__LINE__, Jus[x_5][y_5] + pG->f, 28.);
	dequals(__LINE__, pG->f + Jui[x_6][y_6], 29.);
	dequals(__LINE__, Jui[x_6][y_6] + pG->f, 29.);
	dequals(__LINE__, pG->f + Jl[x_7][y_7], 30.);
	dequals(__LINE__, Jl[x_7][y_7] + pG->f, 30.);
	dequals(__LINE__, pG->f + Jul[x_8][y_8], 31.);
	dequals(__LINE__, Jul[x_8][y_8] + pG->f, 31.);
	dequals(__LINE__, pG->f + Jf[x_9][y_9], 32.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->f, 32.);
	dequals(__LINE__, pG->f + Jd[x_10][y_10], 33.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->f, 33.);
#if ANSI
	ldequals(__LINE__, pG->f + Jld[x_11][y_11], 34.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->f, 34.L);
#endif
	dequals(__LINE__, pG->d + Jc[x_0][y_0], 24.);
	dequals(__LINE__, Jc[x_0][y_0] + pG->d, 24.);
#if ANSI
	dequals(__LINE__, pG->d + Jsc[x_1][y_1], 25.);
	dequals(__LINE__, Jsc[x_1][y_1] + pG->d, 25.);
#endif
	dequals(__LINE__, pG->d + Js[x_2][y_2], 26.);
	dequals(__LINE__, Js[x_2][y_2] + pG->d, 26.);
	dequals(__LINE__, pG->d + Ji[x_3][y_3], 27.);
	dequals(__LINE__, Ji[x_3][y_3] + pG->d, 27.);
	dequals(__LINE__, pG->d + Juc[x_4][y_4], 28.);
	dequals(__LINE__, Juc[x_4][y_4] + pG->d, 28.);
	dequals(__LINE__, pG->d + Jus[x_5][y_5], 29.);
	dequals(__LINE__, Jus[x_5][y_5] + pG->d, 29.);
	dequals(__LINE__, pG->d + Jui[x_6][y_6], 30.);
	dequals(__LINE__, Jui[x_6][y_6] + pG->d, 30.);
	dequals(__LINE__, pG->d + Jl[x_7][y_7], 31.);
	dequals(__LINE__, Jl[x_7][y_7] + pG->d, 31.);
	dequals(__LINE__, pG->d + Jul[x_8][y_8], 32.);
	dequals(__LINE__, Jul[x_8][y_8] + pG->d, 32.);
	dequals(__LINE__, pG->d + Jf[x_9][y_9], 33.);
	dequals(__LINE__, Jf[x_9][y_9] + pG->d, 33.);
	dequals(__LINE__, pG->d + Jd[x_10][y_10], 34.);
	dequals(__LINE__, Jd[x_10][y_10] + pG->d, 34.);
#if ANSI
	ldequals(__LINE__, pG->d + Jld[x_11][y_11], 35.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->d, 35.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Jc[x_0][y_0], 25.L);
	ldequals(__LINE__, Jc[x_0][y_0] + pG->ld, 25.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Jsc[x_1][y_1], 26.L);
	ldequals(__LINE__, Jsc[x_1][y_1] + pG->ld, 26.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Js[x_2][y_2], 27.L);
	ldequals(__LINE__, Js[x_2][y_2] + pG->ld, 27.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Ji[x_3][y_3], 28.L);
	ldequals(__LINE__, Ji[x_3][y_3] + pG->ld, 28.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Juc[x_4][y_4], 29.L);
	ldequals(__LINE__, Juc[x_4][y_4] + pG->ld, 29.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Jus[x_5][y_5], 30.L);
	ldequals(__LINE__, Jus[x_5][y_5] + pG->ld, 30.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Jui[x_6][y_6], 31.L);
	ldequals(__LINE__, Jui[x_6][y_6] + pG->ld, 31.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Jl[x_7][y_7], 32.L);
	ldequals(__LINE__, Jl[x_7][y_7] + pG->ld, 32.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Jul[x_8][y_8], 33.L);
	ldequals(__LINE__, Jul[x_8][y_8] + pG->ld, 33.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Jf[x_9][y_9], 34.L);
	ldequals(__LINE__, Jf[x_9][y_9] + pG->ld, 34.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Jd[x_10][y_10], 35.L);
	ldequals(__LINE__, Jd[x_10][y_10] + pG->ld, 35.L);
#endif
#if ANSI
	ldequals(__LINE__, pG->ld + Jld[x_11][y_11], 36.L);
	ldequals(__LINE__, Jld[x_11][y_11] + pG->ld, 36.L);
#endif
	}}
	return 	report(Filename);
	}
