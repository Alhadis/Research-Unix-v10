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
	static int x[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
	auto CHAR Pc[12];
	int x_0 = 0;
#if ANSI
	auto SCHAR Psc[12];
	int x_1 = 1;
#endif
	auto SHORT Ps[12];
	int x_2 = 2;
	auto INT Pi[12];
	int x_3 = 3;
	auto UCHAR Puc[12];
	int x_4 = 4;
	auto USHORT Pus[12];
	int x_5 = 5;
	auto UINT Pui[12];
	int x_6 = 6;
	auto LONG Pl[12];
	int x_7 = 7;
	auto ULONG Pul[12];
	int x_8 = 8;
	auto FLOAT Pf[12];
	int x_9 = 9;
	auto DOUBLE Pd[12];
	int x_10 = 10;
#if ANSI
	auto LDOUBLE Pld[12];
	int x_11 = 11;
#endif
	Pc[x[x_0]] = 7;
#if ANSI
	Psc[x[x_1]] = 8;
#endif
	Ps[x[x_2]] = 9;
	Pi[x[x_3]] = 10;
	Puc[x[x_4]] = 11;
	Pus[x[x_5]] = 12;
	Pui[x[x_6]] = 13;
	Pl[x[x_7]] = 14;
	Pul[x[x_8]] = 15;
	Pf[x[x_9]] = 16;
	Pd[x[x_10]] = 17;
#if ANSI
	Pld[x[x_11]] = 18;
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto pstruct1 auto arrarr lsh ";
	pG->c = 7; 
	Pc[x[x_0]] = 1;
	iequals(__LINE__, pG->c << Pc[x[x_0]], 14);
	Pc[x[x_0]] = 7;
	pG->c = 1;
	iequals(__LINE__, Pc[x[x_0]] << pG->c, 14);
#if ANSI
	pG->c = 7; 
	Psc[x[x_1]] = 1;
	iequals(__LINE__, pG->c << Psc[x[x_1]], 14);
	Psc[x[x_1]] = 8;
	pG->c = 1;
	iequals(__LINE__, Psc[x[x_1]] << pG->c, 16);
#endif
	pG->c = 7; 
	Ps[x[x_2]] = 1;
	iequals(__LINE__, pG->c << Ps[x[x_2]], 14);
	Ps[x[x_2]] = 9;
	pG->c = 1;
	iequals(__LINE__, Ps[x[x_2]] << pG->c, 18);
	pG->c = 7; 
	Pi[x[x_3]] = 1;
	iequals(__LINE__, pG->c << Pi[x[x_3]], 14);
	Pi[x[x_3]] = 10;
	pG->c = 1;
	iequals(__LINE__, Pi[x[x_3]] << pG->c, 20);
	pG->c = 7; 
	Puc[x[x_4]] = 1;
	iequals(__LINE__, pG->c << Puc[x[x_4]], 14);
	Puc[x[x_4]] = 11;
	pG->c = 1;
	iequals(__LINE__, Puc[x[x_4]] << pG->c, 22);
	pG->c = 7; 
	Pus[x[x_5]] = 1;
	iequals(__LINE__, pG->c << Pus[x[x_5]], 14);
	Pus[x[x_5]] = 12;
	pG->c = 1;
	iequals(__LINE__, Pus[x[x_5]] << pG->c, 24);
	pG->c = 7; 
	Pui[x[x_6]] = 1;
	iequals(__LINE__, pG->c << Pui[x[x_6]], 14);
	Pui[x[x_6]] = 13;
	pG->c = 1;
	iequals(__LINE__, Pui[x[x_6]] << pG->c, 26);
	pG->c = 7; 
	Pl[x[x_7]] = 1;
#if ANSI
	iequals(__LINE__, pG->c << Pl[x[x_7]], 14);
#else
	lequals(__LINE__, pG->c << Pl[x[x_7]], 14L);
#endif
	Pl[x[x_7]] = 14;
	pG->c = 1;
#if ANSI
	lequals(__LINE__, Pl[x[x_7]] << pG->c, 28L);
#else
	lequals(__LINE__, Pl[x[x_7]] << pG->c, 28L);
#endif
	pG->c = 7; 
	Pul[x[x_8]] = 1;
#if ANSI
	iequals(__LINE__, pG->c << Pul[x[x_8]], 14);
#else
	lequals(__LINE__, pG->c << Pul[x[x_8]], 14L);
#endif
	Pul[x[x_8]] = 15;
	pG->c = 1;
#if ANSI
	lequals(__LINE__, Pul[x[x_8]] << pG->c, 30L);
#else
	lequals(__LINE__, Pul[x[x_8]] << pG->c, 30L);
#endif
#if ANSI
	pG->sc = 8; 
	Pc[x[x_0]] = 1;
	iequals(__LINE__, pG->sc << Pc[x[x_0]], 16);
	Pc[x[x_0]] = 7;
	pG->sc = 1;
	iequals(__LINE__, Pc[x[x_0]] << pG->sc, 14);
#endif
#if ANSI
	pG->sc = 8; 
	Psc[x[x_1]] = 1;
	iequals(__LINE__, pG->sc << Psc[x[x_1]], 16);
	Psc[x[x_1]] = 8;
	pG->sc = 1;
	iequals(__LINE__, Psc[x[x_1]] << pG->sc, 16);
#endif
#if ANSI
	pG->sc = 8; 
	Ps[x[x_2]] = 1;
	iequals(__LINE__, pG->sc << Ps[x[x_2]], 16);
	Ps[x[x_2]] = 9;
	pG->sc = 1;
	iequals(__LINE__, Ps[x[x_2]] << pG->sc, 18);
#endif
#if ANSI
	pG->sc = 8; 
	Pi[x[x_3]] = 1;
	iequals(__LINE__, pG->sc << Pi[x[x_3]], 16);
	Pi[x[x_3]] = 10;
	pG->sc = 1;
	iequals(__LINE__, Pi[x[x_3]] << pG->sc, 20);
#endif
#if ANSI
	pG->sc = 8; 
	Puc[x[x_4]] = 1;
	iequals(__LINE__, pG->sc << Puc[x[x_4]], 16);
	Puc[x[x_4]] = 11;
	pG->sc = 1;
	iequals(__LINE__, Puc[x[x_4]] << pG->sc, 22);
#endif
#if ANSI
	pG->sc = 8; 
	Pus[x[x_5]] = 1;
	iequals(__LINE__, pG->sc << Pus[x[x_5]], 16);
	Pus[x[x_5]] = 12;
	pG->sc = 1;
	iequals(__LINE__, Pus[x[x_5]] << pG->sc, 24);
#endif
#if ANSI
	pG->sc = 8; 
	Pui[x[x_6]] = 1;
	iequals(__LINE__, pG->sc << Pui[x[x_6]], 16);
	Pui[x[x_6]] = 13;
	pG->sc = 1;
	iequals(__LINE__, Pui[x[x_6]] << pG->sc, 26);
#endif
#if ANSI
	pG->sc = 8; 
	Pl[x[x_7]] = 1;
#if ANSI
	iequals(__LINE__, pG->sc << Pl[x[x_7]], 16);
#else
	lequals(__LINE__, pG->sc << Pl[x[x_7]], 16L);
#endif
	Pl[x[x_7]] = 14;
	pG->sc = 1;
#if ANSI
	lequals(__LINE__, Pl[x[x_7]] << pG->sc, 28L);
#else
	lequals(__LINE__, Pl[x[x_7]] << pG->sc, 28L);
#endif
#endif
#if ANSI
	pG->sc = 8; 
	Pul[x[x_8]] = 1;
#if ANSI
	iequals(__LINE__, pG->sc << Pul[x[x_8]], 16);
#else
	lequals(__LINE__, pG->sc << Pul[x[x_8]], 16L);
#endif
	Pul[x[x_8]] = 15;
	pG->sc = 1;
#if ANSI
	lequals(__LINE__, Pul[x[x_8]] << pG->sc, 30L);
#else
	lequals(__LINE__, Pul[x[x_8]] << pG->sc, 30L);
#endif
#endif
	pG->s = 9; 
	Pc[x[x_0]] = 1;
	iequals(__LINE__, pG->s << Pc[x[x_0]], 18);
	Pc[x[x_0]] = 7;
	pG->s = 1;
	iequals(__LINE__, Pc[x[x_0]] << pG->s, 14);
#if ANSI
	pG->s = 9; 
	Psc[x[x_1]] = 1;
	iequals(__LINE__, pG->s << Psc[x[x_1]], 18);
	Psc[x[x_1]] = 8;
	pG->s = 1;
	iequals(__LINE__, Psc[x[x_1]] << pG->s, 16);
#endif
	pG->s = 9; 
	Ps[x[x_2]] = 1;
	iequals(__LINE__, pG->s << Ps[x[x_2]], 18);
	Ps[x[x_2]] = 9;
	pG->s = 1;
	iequals(__LINE__, Ps[x[x_2]] << pG->s, 18);
	pG->s = 9; 
	Pi[x[x_3]] = 1;
	iequals(__LINE__, pG->s << Pi[x[x_3]], 18);
	Pi[x[x_3]] = 10;
	pG->s = 1;
	iequals(__LINE__, Pi[x[x_3]] << pG->s, 20);
	pG->s = 9; 
	Puc[x[x_4]] = 1;
	iequals(__LINE__, pG->s << Puc[x[x_4]], 18);
	Puc[x[x_4]] = 11;
	pG->s = 1;
	iequals(__LINE__, Puc[x[x_4]] << pG->s, 22);
	pG->s = 9; 
	Pus[x[x_5]] = 1;
	iequals(__LINE__, pG->s << Pus[x[x_5]], 18);
	Pus[x[x_5]] = 12;
	pG->s = 1;
	iequals(__LINE__, Pus[x[x_5]] << pG->s, 24);
	pG->s = 9; 
	Pui[x[x_6]] = 1;
	iequals(__LINE__, pG->s << Pui[x[x_6]], 18);
	Pui[x[x_6]] = 13;
	pG->s = 1;
	iequals(__LINE__, Pui[x[x_6]] << pG->s, 26);
	pG->s = 9; 
	Pl[x[x_7]] = 1;
#if ANSI
	iequals(__LINE__, pG->s << Pl[x[x_7]], 18);
#else
	lequals(__LINE__, pG->s << Pl[x[x_7]], 18L);
#endif
	Pl[x[x_7]] = 14;
	pG->s = 1;
#if ANSI
	lequals(__LINE__, Pl[x[x_7]] << pG->s, 28L);
#else
	lequals(__LINE__, Pl[x[x_7]] << pG->s, 28L);
#endif
	pG->s = 9; 
	Pul[x[x_8]] = 1;
#if ANSI
	iequals(__LINE__, pG->s << Pul[x[x_8]], 18);
#else
	lequals(__LINE__, pG->s << Pul[x[x_8]], 18L);
#endif
	Pul[x[x_8]] = 15;
	pG->s = 1;
#if ANSI
	lequals(__LINE__, Pul[x[x_8]] << pG->s, 30L);
#else
	lequals(__LINE__, Pul[x[x_8]] << pG->s, 30L);
#endif
	pG->i = 10; 
	Pc[x[x_0]] = 1;
	iequals(__LINE__, pG->i << Pc[x[x_0]], 20);
	Pc[x[x_0]] = 7;
	pG->i = 1;
	iequals(__LINE__, Pc[x[x_0]] << pG->i, 14);
#if ANSI
	pG->i = 10; 
	Psc[x[x_1]] = 1;
	iequals(__LINE__, pG->i << Psc[x[x_1]], 20);
	Psc[x[x_1]] = 8;
	pG->i = 1;
	iequals(__LINE__, Psc[x[x_1]] << pG->i, 16);
#endif
	pG->i = 10; 
	Ps[x[x_2]] = 1;
	iequals(__LINE__, pG->i << Ps[x[x_2]], 20);
	Ps[x[x_2]] = 9;
	pG->i = 1;
	iequals(__LINE__, Ps[x[x_2]] << pG->i, 18);
	pG->i = 10; 
	Pi[x[x_3]] = 1;
	iequals(__LINE__, pG->i << Pi[x[x_3]], 20);
	Pi[x[x_3]] = 10;
	pG->i = 1;
	iequals(__LINE__, Pi[x[x_3]] << pG->i, 20);
	pG->i = 10; 
	Puc[x[x_4]] = 1;
	iequals(__LINE__, pG->i << Puc[x[x_4]], 20);
	Puc[x[x_4]] = 11;
	pG->i = 1;
	iequals(__LINE__, Puc[x[x_4]] << pG->i, 22);
	pG->i = 10; 
	Pus[x[x_5]] = 1;
	iequals(__LINE__, pG->i << Pus[x[x_5]], 20);
	Pus[x[x_5]] = 12;
	pG->i = 1;
	iequals(__LINE__, Pus[x[x_5]] << pG->i, 24);
	pG->i = 10; 
	Pui[x[x_6]] = 1;
	iequals(__LINE__, pG->i << Pui[x[x_6]], 20);
	Pui[x[x_6]] = 13;
	pG->i = 1;
	iequals(__LINE__, Pui[x[x_6]] << pG->i, 26);
	pG->i = 10; 
	Pl[x[x_7]] = 1;
#if ANSI
	iequals(__LINE__, pG->i << Pl[x[x_7]], 20);
#else
	lequals(__LINE__, pG->i << Pl[x[x_7]], 20L);
#endif
	Pl[x[x_7]] = 14;
	pG->i = 1;
#if ANSI
	lequals(__LINE__, Pl[x[x_7]] << pG->i, 28L);
#else
	lequals(__LINE__, Pl[x[x_7]] << pG->i, 28L);
#endif
	pG->i = 10; 
	Pul[x[x_8]] = 1;
#if ANSI
	iequals(__LINE__, pG->i << Pul[x[x_8]], 20);
#else
	lequals(__LINE__, pG->i << Pul[x[x_8]], 20L);
#endif
	Pul[x[x_8]] = 15;
	pG->i = 1;
#if ANSI
	lequals(__LINE__, Pul[x[x_8]] << pG->i, 30L);
#else
	lequals(__LINE__, Pul[x[x_8]] << pG->i, 30L);
#endif
	pG->uc = 11; 
	Pc[x[x_0]] = 1;
	iequals(__LINE__, pG->uc << Pc[x[x_0]], 22);
	Pc[x[x_0]] = 7;
	pG->uc = 1;
	iequals(__LINE__, Pc[x[x_0]] << pG->uc, 14);
#if ANSI
	pG->uc = 11; 
	Psc[x[x_1]] = 1;
	iequals(__LINE__, pG->uc << Psc[x[x_1]], 22);
	Psc[x[x_1]] = 8;
	pG->uc = 1;
	iequals(__LINE__, Psc[x[x_1]] << pG->uc, 16);
#endif
	pG->uc = 11; 
	Ps[x[x_2]] = 1;
	iequals(__LINE__, pG->uc << Ps[x[x_2]], 22);
	Ps[x[x_2]] = 9;
	pG->uc = 1;
	iequals(__LINE__, Ps[x[x_2]] << pG->uc, 18);
	pG->uc = 11; 
	Pi[x[x_3]] = 1;
	iequals(__LINE__, pG->uc << Pi[x[x_3]], 22);
	Pi[x[x_3]] = 10;
	pG->uc = 1;
	iequals(__LINE__, Pi[x[x_3]] << pG->uc, 20);
	pG->uc = 11; 
	Puc[x[x_4]] = 1;
	iequals(__LINE__, pG->uc << Puc[x[x_4]], 22);
	Puc[x[x_4]] = 11;
	pG->uc = 1;
	iequals(__LINE__, Puc[x[x_4]] << pG->uc, 22);
	pG->uc = 11; 
	Pus[x[x_5]] = 1;
	iequals(__LINE__, pG->uc << Pus[x[x_5]], 22);
	Pus[x[x_5]] = 12;
	pG->uc = 1;
	iequals(__LINE__, Pus[x[x_5]] << pG->uc, 24);
	pG->uc = 11; 
	Pui[x[x_6]] = 1;
	iequals(__LINE__, pG->uc << Pui[x[x_6]], 22);
	Pui[x[x_6]] = 13;
	pG->uc = 1;
	iequals(__LINE__, Pui[x[x_6]] << pG->uc, 26);
	pG->uc = 11; 
	Pl[x[x_7]] = 1;
#if ANSI
	iequals(__LINE__, pG->uc << Pl[x[x_7]], 22);
#else
	lequals(__LINE__, pG->uc << Pl[x[x_7]], 22L);
#endif
	Pl[x[x_7]] = 14;
	pG->uc = 1;
#if ANSI
	lequals(__LINE__, Pl[x[x_7]] << pG->uc, 28L);
#else
	lequals(__LINE__, Pl[x[x_7]] << pG->uc, 28L);
#endif
	pG->uc = 11; 
	Pul[x[x_8]] = 1;
#if ANSI
	iequals(__LINE__, pG->uc << Pul[x[x_8]], 22);
#else
	lequals(__LINE__, pG->uc << Pul[x[x_8]], 22L);
#endif
	Pul[x[x_8]] = 15;
	pG->uc = 1;
#if ANSI
	lequals(__LINE__, Pul[x[x_8]] << pG->uc, 30L);
#else
	lequals(__LINE__, Pul[x[x_8]] << pG->uc, 30L);
#endif
	pG->us = 12; 
	Pc[x[x_0]] = 1;
	iequals(__LINE__, pG->us << Pc[x[x_0]], 24);
	Pc[x[x_0]] = 7;
	pG->us = 1;
	iequals(__LINE__, Pc[x[x_0]] << pG->us, 14);
#if ANSI
	pG->us = 12; 
	Psc[x[x_1]] = 1;
	iequals(__LINE__, pG->us << Psc[x[x_1]], 24);
	Psc[x[x_1]] = 8;
	pG->us = 1;
	iequals(__LINE__, Psc[x[x_1]] << pG->us, 16);
#endif
	pG->us = 12; 
	Ps[x[x_2]] = 1;
	iequals(__LINE__, pG->us << Ps[x[x_2]], 24);
	Ps[x[x_2]] = 9;
	pG->us = 1;
	iequals(__LINE__, Ps[x[x_2]] << pG->us, 18);
	pG->us = 12; 
	Pi[x[x_3]] = 1;
	iequals(__LINE__, pG->us << Pi[x[x_3]], 24);
	Pi[x[x_3]] = 10;
	pG->us = 1;
	iequals(__LINE__, Pi[x[x_3]] << pG->us, 20);
	pG->us = 12; 
	Puc[x[x_4]] = 1;
	iequals(__LINE__, pG->us << Puc[x[x_4]], 24);
	Puc[x[x_4]] = 11;
	pG->us = 1;
	iequals(__LINE__, Puc[x[x_4]] << pG->us, 22);
	pG->us = 12; 
	Pus[x[x_5]] = 1;
	iequals(__LINE__, pG->us << Pus[x[x_5]], 24);
	Pus[x[x_5]] = 12;
	pG->us = 1;
	iequals(__LINE__, Pus[x[x_5]] << pG->us, 24);
	pG->us = 12; 
	Pui[x[x_6]] = 1;
	iequals(__LINE__, pG->us << Pui[x[x_6]], 24);
	Pui[x[x_6]] = 13;
	pG->us = 1;
	iequals(__LINE__, Pui[x[x_6]] << pG->us, 26);
	pG->us = 12; 
	Pl[x[x_7]] = 1;
#if ANSI
	iequals(__LINE__, pG->us << Pl[x[x_7]], 24);
#else
	lequals(__LINE__, pG->us << Pl[x[x_7]], 24L);
#endif
	Pl[x[x_7]] = 14;
	pG->us = 1;
#if ANSI
	lequals(__LINE__, Pl[x[x_7]] << pG->us, 28L);
#else
	lequals(__LINE__, Pl[x[x_7]] << pG->us, 28L);
#endif
	pG->us = 12; 
	Pul[x[x_8]] = 1;
#if ANSI
	iequals(__LINE__, pG->us << Pul[x[x_8]], 24);
#else
	lequals(__LINE__, pG->us << Pul[x[x_8]], 24L);
#endif
	Pul[x[x_8]] = 15;
	pG->us = 1;
#if ANSI
	lequals(__LINE__, Pul[x[x_8]] << pG->us, 30L);
#else
	lequals(__LINE__, Pul[x[x_8]] << pG->us, 30L);
#endif
	pG->ui = 13; 
	Pc[x[x_0]] = 1;
	iequals(__LINE__, pG->ui << Pc[x[x_0]], 26);
	Pc[x[x_0]] = 7;
	pG->ui = 1;
	iequals(__LINE__, Pc[x[x_0]] << pG->ui, 14);
#if ANSI
	pG->ui = 13; 
	Psc[x[x_1]] = 1;
	iequals(__LINE__, pG->ui << Psc[x[x_1]], 26);
	Psc[x[x_1]] = 8;
	pG->ui = 1;
	iequals(__LINE__, Psc[x[x_1]] << pG->ui, 16);
#endif
	pG->ui = 13; 
	Ps[x[x_2]] = 1;
	iequals(__LINE__, pG->ui << Ps[x[x_2]], 26);
	Ps[x[x_2]] = 9;
	pG->ui = 1;
	iequals(__LINE__, Ps[x[x_2]] << pG->ui, 18);
	pG->ui = 13; 
	Pi[x[x_3]] = 1;
	iequals(__LINE__, pG->ui << Pi[x[x_3]], 26);
	Pi[x[x_3]] = 10;
	pG->ui = 1;
	iequals(__LINE__, Pi[x[x_3]] << pG->ui, 20);
	pG->ui = 13; 
	Puc[x[x_4]] = 1;
	iequals(__LINE__, pG->ui << Puc[x[x_4]], 26);
	Puc[x[x_4]] = 11;
	pG->ui = 1;
	iequals(__LINE__, Puc[x[x_4]] << pG->ui, 22);
	pG->ui = 13; 
	Pus[x[x_5]] = 1;
	iequals(__LINE__, pG->ui << Pus[x[x_5]], 26);
	Pus[x[x_5]] = 12;
	pG->ui = 1;
	iequals(__LINE__, Pus[x[x_5]] << pG->ui, 24);
	pG->ui = 13; 
	Pui[x[x_6]] = 1;
	iequals(__LINE__, pG->ui << Pui[x[x_6]], 26);
	Pui[x[x_6]] = 13;
	pG->ui = 1;
	iequals(__LINE__, Pui[x[x_6]] << pG->ui, 26);
	pG->ui = 13; 
	Pl[x[x_7]] = 1;
#if ANSI
	iequals(__LINE__, pG->ui << Pl[x[x_7]], 26);
#else
	lequals(__LINE__, pG->ui << Pl[x[x_7]], 26L);
#endif
	Pl[x[x_7]] = 14;
	pG->ui = 1;
#if ANSI
	lequals(__LINE__, Pl[x[x_7]] << pG->ui, 28L);
#else
	lequals(__LINE__, Pl[x[x_7]] << pG->ui, 28L);
#endif
	pG->ui = 13; 
	Pul[x[x_8]] = 1;
#if ANSI
	iequals(__LINE__, pG->ui << Pul[x[x_8]], 26);
#else
	lequals(__LINE__, pG->ui << Pul[x[x_8]], 26L);
#endif
	Pul[x[x_8]] = 15;
	pG->ui = 1;
#if ANSI
	lequals(__LINE__, Pul[x[x_8]] << pG->ui, 30L);
#else
	lequals(__LINE__, Pul[x[x_8]] << pG->ui, 30L);
#endif
	pG->l = 14; 
	Pc[x[x_0]] = 1;
	lequals(__LINE__, pG->l << Pc[x[x_0]], 28L);
	Pc[x[x_0]] = 7;
	pG->l = 1;
	lequals(__LINE__, Pc[x[x_0]] << pG->l, 14L);
#if ANSI
	pG->l = 14; 
	Psc[x[x_1]] = 1;
	lequals(__LINE__, pG->l << Psc[x[x_1]], 28L);
	Psc[x[x_1]] = 8;
	pG->l = 1;
	lequals(__LINE__, Psc[x[x_1]] << pG->l, 16L);
#endif
	pG->l = 14; 
	Ps[x[x_2]] = 1;
	lequals(__LINE__, pG->l << Ps[x[x_2]], 28L);
	Ps[x[x_2]] = 9;
	pG->l = 1;
	lequals(__LINE__, Ps[x[x_2]] << pG->l, 18L);
	pG->l = 14; 
	Pi[x[x_3]] = 1;
	lequals(__LINE__, pG->l << Pi[x[x_3]], 28L);
	Pi[x[x_3]] = 10;
	pG->l = 1;
	lequals(__LINE__, Pi[x[x_3]] << pG->l, 20L);
	pG->l = 14; 
	Puc[x[x_4]] = 1;
	lequals(__LINE__, pG->l << Puc[x[x_4]], 28L);
	Puc[x[x_4]] = 11;
	pG->l = 1;
	lequals(__LINE__, Puc[x[x_4]] << pG->l, 22L);
	pG->l = 14; 
	Pus[x[x_5]] = 1;
	lequals(__LINE__, pG->l << Pus[x[x_5]], 28L);
	Pus[x[x_5]] = 12;
	pG->l = 1;
	lequals(__LINE__, Pus[x[x_5]] << pG->l, 24L);
	pG->l = 14; 
	Pui[x[x_6]] = 1;
	lequals(__LINE__, pG->l << Pui[x[x_6]], 28L);
	Pui[x[x_6]] = 13;
	pG->l = 1;
	lequals(__LINE__, Pui[x[x_6]] << pG->l, 26L);
	pG->l = 14; 
	Pl[x[x_7]] = 1;
	lequals(__LINE__, pG->l << Pl[x[x_7]], 28L);
	Pl[x[x_7]] = 14;
	pG->l = 1;
	lequals(__LINE__, Pl[x[x_7]] << pG->l, 28L);
	pG->l = 14; 
	Pul[x[x_8]] = 1;
	lequals(__LINE__, pG->l << Pul[x[x_8]], 28L);
	Pul[x[x_8]] = 15;
	pG->l = 1;
	lequals(__LINE__, Pul[x[x_8]] << pG->l, 30L);
	pG->ul = 15; 
	Pc[x[x_0]] = 1;
	lequals(__LINE__, pG->ul << Pc[x[x_0]], 30L);
	Pc[x[x_0]] = 7;
	pG->ul = 1;
	lequals(__LINE__, Pc[x[x_0]] << pG->ul, 14L);
#if ANSI
	pG->ul = 15; 
	Psc[x[x_1]] = 1;
	lequals(__LINE__, pG->ul << Psc[x[x_1]], 30L);
	Psc[x[x_1]] = 8;
	pG->ul = 1;
	lequals(__LINE__, Psc[x[x_1]] << pG->ul, 16L);
#endif
	pG->ul = 15; 
	Ps[x[x_2]] = 1;
	lequals(__LINE__, pG->ul << Ps[x[x_2]], 30L);
	Ps[x[x_2]] = 9;
	pG->ul = 1;
	lequals(__LINE__, Ps[x[x_2]] << pG->ul, 18L);
	pG->ul = 15; 
	Pi[x[x_3]] = 1;
	lequals(__LINE__, pG->ul << Pi[x[x_3]], 30L);
	Pi[x[x_3]] = 10;
	pG->ul = 1;
	lequals(__LINE__, Pi[x[x_3]] << pG->ul, 20L);
	pG->ul = 15; 
	Puc[x[x_4]] = 1;
	lequals(__LINE__, pG->ul << Puc[x[x_4]], 30L);
	Puc[x[x_4]] = 11;
	pG->ul = 1;
	lequals(__LINE__, Puc[x[x_4]] << pG->ul, 22L);
	pG->ul = 15; 
	Pus[x[x_5]] = 1;
	lequals(__LINE__, pG->ul << Pus[x[x_5]], 30L);
	Pus[x[x_5]] = 12;
	pG->ul = 1;
	lequals(__LINE__, Pus[x[x_5]] << pG->ul, 24L);
	pG->ul = 15; 
	Pui[x[x_6]] = 1;
	lequals(__LINE__, pG->ul << Pui[x[x_6]], 30L);
	Pui[x[x_6]] = 13;
	pG->ul = 1;
	lequals(__LINE__, Pui[x[x_6]] << pG->ul, 26L);
	pG->ul = 15; 
	Pl[x[x_7]] = 1;
	lequals(__LINE__, pG->ul << Pl[x[x_7]], 30L);
	Pl[x[x_7]] = 14;
	pG->ul = 1;
	lequals(__LINE__, Pl[x[x_7]] << pG->ul, 28L);
	pG->ul = 15; 
	Pul[x[x_8]] = 1;
	lequals(__LINE__, pG->ul << Pul[x[x_8]], 30L);
	Pul[x[x_8]] = 15;
	pG->ul = 1;
	lequals(__LINE__, Pul[x[x_8]] << pG->ul, 30L);
	}}
	return 	report(Filename);
	}
