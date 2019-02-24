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
		} E;
	auto E E1, *pE1 = &E1;
#if ANSI
	auto E E2, *pE2 = &E2;
#endif
	auto E E3, *pE3 = &E3;
	auto E E4, *pE4 = &E4;
	auto E E5, *pE5 = &E5;
	auto E E6, *pE6 = &E6;
	auto E E7, *pE7 = &E7;
	auto E E8, *pE8 = &E8;
	auto E E9, *pE9 = &E9;
	auto E E10, *pE10 = &E10;
	auto E E11, *pE11 = &E11;
#if ANSI
	auto E E12, *pE12 = &E12;
#endif
	pE1->c = 7;
#if ANSI
	pE2->sc = 8;
#endif
	pE3->s = 9;
	pE4->i = 10;
	pE5->uc = 11;
	pE6->us = 12;
	pE7->ui = 13;
	pE8->l = 14;
	pE9->ul = 15;
	pE10->f = 16;
	pE11->d = 17;
#if ANSI
	pE12->ld = 18;
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
	Filename =  " auto punion auto array2 postdec ";
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE1->c = 7;
	iequals(__LINE__,  (pE1->c) --, 7);
	iequals(__LINE__, pE1->c, 6);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#endif
#if ANSI
	pE2->sc = 8;
	iequals(__LINE__,  (pE2->sc) --, 8);
	iequals(__LINE__, pE2->sc, 7);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE3->s = 9;
	iequals(__LINE__,  (pE3->s) --, 9);
	iequals(__LINE__, pE3->s, 8);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE4->i = 10;
	iequals(__LINE__,  (pE4->i) --, 10);
	iequals(__LINE__, pE4->i, 9);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE5->uc = 11;
	iequals(__LINE__,  (pE5->uc) --, 11);
	iequals(__LINE__, pE5->uc, 10);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE6->us = 12;
	iequals(__LINE__,  (pE6->us) --, 12);
	iequals(__LINE__, pE6->us, 11);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE7->ui = 13;
	iequals(__LINE__,  (pE7->ui) --, 13);
	iequals(__LINE__, pE7->ui, 12);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE8->l = 14;
	lequals(__LINE__,  (pE8->l) --, 14L);
	lequals(__LINE__, pE8->l, 13L);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE9->ul = 15;
	lequals(__LINE__,  (pE9->ul) --, 15L);
	lequals(__LINE__, pE9->ul, 14L);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE10->f = 16;
	dequals(__LINE__,  (pE10->f) --, 16.);
	dequals(__LINE__, pE10->f, 15.);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#if ANSI
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#if ANSI
	pE11->d = 17;
	dequals(__LINE__,  (pE11->d) --, 17.);
	dequals(__LINE__, pE11->d, 16.);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Jc[x_0][y_0] = 7;
	iequals(__LINE__,  (Jc[x_0][y_0]) --, 7);
	iequals(__LINE__, Jc[x_0][y_0], 6);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Jsc[x_1][y_1] = 8;
	iequals(__LINE__,  (Jsc[x_1][y_1]) --, 8);
	iequals(__LINE__, Jsc[x_1][y_1], 7);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Js[x_2][y_2] = 9;
	iequals(__LINE__,  (Js[x_2][y_2]) --, 9);
	iequals(__LINE__, Js[x_2][y_2], 8);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Ji[x_3][y_3] = 10;
	iequals(__LINE__,  (Ji[x_3][y_3]) --, 10);
	iequals(__LINE__, Ji[x_3][y_3], 9);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Juc[x_4][y_4] = 11;
	iequals(__LINE__,  (Juc[x_4][y_4]) --, 11);
	iequals(__LINE__, Juc[x_4][y_4], 10);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Jus[x_5][y_5] = 12;
	iequals(__LINE__,  (Jus[x_5][y_5]) --, 12);
	iequals(__LINE__, Jus[x_5][y_5], 11);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Jui[x_6][y_6] = 13;
	iequals(__LINE__,  (Jui[x_6][y_6]) --, 13);
	iequals(__LINE__, Jui[x_6][y_6], 12);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Jl[x_7][y_7] = 14;
	lequals(__LINE__,  (Jl[x_7][y_7]) --, 14L);
	lequals(__LINE__, Jl[x_7][y_7], 13L);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Jul[x_8][y_8] = 15;
	lequals(__LINE__,  (Jul[x_8][y_8]) --, 15L);
	lequals(__LINE__, Jul[x_8][y_8], 14L);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Jf[x_9][y_9] = 16;
	dequals(__LINE__,  (Jf[x_9][y_9]) --, 16.);
	dequals(__LINE__, Jf[x_9][y_9], 15.);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Jd[x_10][y_10] = 17;
	dequals(__LINE__,  (Jd[x_10][y_10]) --, 17.);
	dequals(__LINE__, Jd[x_10][y_10], 16.);
#endif
#if ANSI
	pE12->ld = 18;
	ldequals(__LINE__,  (pE12->ld) --, 18.L);
	ldequals(__LINE__, pE12->ld, 17.L);
	Jld[x_11][y_11] = 18;
	ldequals(__LINE__,  (Jld[x_11][y_11]) --, 18.L);
	ldequals(__LINE__, Jld[x_11][y_11], 17.L);
#endif
	}}
	return 	report(Filename);
	}
