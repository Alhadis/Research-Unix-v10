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
	Filename =  " auto scalar auto arrarr diveq4 ";
	Af = 14;
	Pc[x[x_0]] = 7;Af /= Pc[x[x_0]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 14;
	dequals(__LINE__, Af /= Pc[x[x_0]], (FLOAT)2.);
	Pc[x[x_0]] = 32;
	Af = 16;Pc[x[x_0]] /= Af;
	iequals(__LINE__, Pc[x[x_0]], (CHAR)2);
	Pc[x[x_0]] = 32;
	iequals(__LINE__, Pc[x[x_0]] /= Af, (CHAR)2);
#if ANSI
	Af = 16;
	Psc[x[x_1]] = 8;Af /= Psc[x[x_1]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 16;
	dequals(__LINE__, Af /= Psc[x[x_1]], (FLOAT)2.);
	Psc[x[x_1]] = 32;
	Af = 16;Psc[x[x_1]] /= Af;
	iequals(__LINE__, Psc[x[x_1]], (SCHAR)2);
	Psc[x[x_1]] = 32;
	iequals(__LINE__, Psc[x[x_1]] /= Af, (SCHAR)2);
#endif
	Af = 18;
	Ps[x[x_2]] = 9;Af /= Ps[x[x_2]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 18;
	dequals(__LINE__, Af /= Ps[x[x_2]], (FLOAT)2.);
	Ps[x[x_2]] = 32;
	Af = 16;Ps[x[x_2]] /= Af;
	iequals(__LINE__, Ps[x[x_2]], (SHORT)2);
	Ps[x[x_2]] = 32;
	iequals(__LINE__, Ps[x[x_2]] /= Af, (SHORT)2);
	Af = 20;
	Pi[x[x_3]] = 10;Af /= Pi[x[x_3]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 20;
	dequals(__LINE__, Af /= Pi[x[x_3]], (FLOAT)2.);
	Pi[x[x_3]] = 32;
	Af = 16;Pi[x[x_3]] /= Af;
	iequals(__LINE__, Pi[x[x_3]], (INT)2);
	Pi[x[x_3]] = 32;
	iequals(__LINE__, Pi[x[x_3]] /= Af, (INT)2);
	Af = 22;
	Puc[x[x_4]] = 11;Af /= Puc[x[x_4]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 22;
	dequals(__LINE__, Af /= Puc[x[x_4]], (FLOAT)2.);
	Puc[x[x_4]] = 32;
	Af = 16;Puc[x[x_4]] /= Af;
	iequals(__LINE__, Puc[x[x_4]], (UCHAR)2);
	Puc[x[x_4]] = 32;
	iequals(__LINE__, Puc[x[x_4]] /= Af, (UCHAR)2);
	Af = 24;
	Pus[x[x_5]] = 12;Af /= Pus[x[x_5]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 24;
	dequals(__LINE__, Af /= Pus[x[x_5]], (FLOAT)2.);
	Pus[x[x_5]] = 32;
	Af = 16;Pus[x[x_5]] /= Af;
	iequals(__LINE__, Pus[x[x_5]], (USHORT)2);
	Pus[x[x_5]] = 32;
	iequals(__LINE__, Pus[x[x_5]] /= Af, (USHORT)2);
	Af = 26;
	Pui[x[x_6]] = 13;Af /= Pui[x[x_6]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 26;
	dequals(__LINE__, Af /= Pui[x[x_6]], (FLOAT)2.);
	Pui[x[x_6]] = 32;
	Af = 16;Pui[x[x_6]] /= Af;
	iequals(__LINE__, Pui[x[x_6]], (UINT)2);
	Pui[x[x_6]] = 32;
	iequals(__LINE__, Pui[x[x_6]] /= Af, (UINT)2);
	Af = 28;
	Pl[x[x_7]] = 14;Af /= Pl[x[x_7]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 28;
	dequals(__LINE__, Af /= Pl[x[x_7]], (FLOAT)2.);
	Pl[x[x_7]] = 32;
	Af = 16;Pl[x[x_7]] /= Af;
	lequals(__LINE__, Pl[x[x_7]], (LONG)2L);
	Pl[x[x_7]] = 32;
	lequals(__LINE__, Pl[x[x_7]] /= Af, (LONG)2L);
	Af = 30;
	Pul[x[x_8]] = 15;Af /= Pul[x[x_8]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 30;
	dequals(__LINE__, Af /= Pul[x[x_8]], (FLOAT)2.);
	Pul[x[x_8]] = 32;
	Af = 16;Pul[x[x_8]] /= Af;
	lequals(__LINE__, Pul[x[x_8]], (ULONG)2L);
	Pul[x[x_8]] = 32;
	lequals(__LINE__, Pul[x[x_8]] /= Af, (ULONG)2L);
	Af = 32;
	Pf[x[x_9]] = 16;Af /= Pf[x[x_9]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 32;
	dequals(__LINE__, Af /= Pf[x[x_9]], (FLOAT)2.);
	Pf[x[x_9]] = 32;
	Af = 16;Pf[x[x_9]] /= Af;
	dequals(__LINE__, Pf[x[x_9]], (FLOAT)2.);
	Pf[x[x_9]] = 32;
	dequals(__LINE__, Pf[x[x_9]] /= Af, (FLOAT)2.);
	Af = 34;
	Pd[x[x_10]] = 17;Af /= Pd[x[x_10]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 34;
	dequals(__LINE__, Af /= Pd[x[x_10]], (FLOAT)2.);
	Pd[x[x_10]] = 32;
	Af = 16;Pd[x[x_10]] /= Af;
	dequals(__LINE__, Pd[x[x_10]], (DOUBLE)2.);
	Pd[x[x_10]] = 32;
	dequals(__LINE__, Pd[x[x_10]] /= Af, (DOUBLE)2.);
#if ANSI
	Af = 36;
	Pld[x[x_11]] = 18;Af /= Pld[x[x_11]];
	dequals(__LINE__, Af, (FLOAT)2.);
	Af = 36;
	dequals(__LINE__, Af /= Pld[x[x_11]], (FLOAT)2.);
	Pld[x[x_11]] = 32;
	Af = 16;Pld[x[x_11]] /= Af;
	ldequals(__LINE__, Pld[x[x_11]], (LDOUBLE)2.L);
	Pld[x[x_11]] = 32;
	ldequals(__LINE__, Pld[x[x_11]] /= Af, (LDOUBLE)2.L);
#endif
	Ad = 14;
	Pc[x[x_0]] = 7;Ad /= Pc[x[x_0]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 14;
	dequals(__LINE__, Ad /= Pc[x[x_0]], (DOUBLE)2.);
	Pc[x[x_0]] = 34;
	Ad = 17;Pc[x[x_0]] /= Ad;
	iequals(__LINE__, Pc[x[x_0]], (CHAR)2);
	Pc[x[x_0]] = 34;
	iequals(__LINE__, Pc[x[x_0]] /= Ad, (CHAR)2);
#if ANSI
	Ad = 16;
	Psc[x[x_1]] = 8;Ad /= Psc[x[x_1]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 16;
	dequals(__LINE__, Ad /= Psc[x[x_1]], (DOUBLE)2.);
	Psc[x[x_1]] = 34;
	Ad = 17;Psc[x[x_1]] /= Ad;
	iequals(__LINE__, Psc[x[x_1]], (SCHAR)2);
	Psc[x[x_1]] = 34;
	iequals(__LINE__, Psc[x[x_1]] /= Ad, (SCHAR)2);
#endif
	Ad = 18;
	Ps[x[x_2]] = 9;Ad /= Ps[x[x_2]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 18;
	dequals(__LINE__, Ad /= Ps[x[x_2]], (DOUBLE)2.);
	Ps[x[x_2]] = 34;
	Ad = 17;Ps[x[x_2]] /= Ad;
	iequals(__LINE__, Ps[x[x_2]], (SHORT)2);
	Ps[x[x_2]] = 34;
	iequals(__LINE__, Ps[x[x_2]] /= Ad, (SHORT)2);
	Ad = 20;
	Pi[x[x_3]] = 10;Ad /= Pi[x[x_3]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 20;
	dequals(__LINE__, Ad /= Pi[x[x_3]], (DOUBLE)2.);
	Pi[x[x_3]] = 34;
	Ad = 17;Pi[x[x_3]] /= Ad;
	iequals(__LINE__, Pi[x[x_3]], (INT)2);
	Pi[x[x_3]] = 34;
	iequals(__LINE__, Pi[x[x_3]] /= Ad, (INT)2);
	Ad = 22;
	Puc[x[x_4]] = 11;Ad /= Puc[x[x_4]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 22;
	dequals(__LINE__, Ad /= Puc[x[x_4]], (DOUBLE)2.);
	Puc[x[x_4]] = 34;
	Ad = 17;Puc[x[x_4]] /= Ad;
	iequals(__LINE__, Puc[x[x_4]], (UCHAR)2);
	Puc[x[x_4]] = 34;
	iequals(__LINE__, Puc[x[x_4]] /= Ad, (UCHAR)2);
	Ad = 24;
	Pus[x[x_5]] = 12;Ad /= Pus[x[x_5]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 24;
	dequals(__LINE__, Ad /= Pus[x[x_5]], (DOUBLE)2.);
	Pus[x[x_5]] = 34;
	Ad = 17;Pus[x[x_5]] /= Ad;
	iequals(__LINE__, Pus[x[x_5]], (USHORT)2);
	Pus[x[x_5]] = 34;
	iequals(__LINE__, Pus[x[x_5]] /= Ad, (USHORT)2);
	Ad = 26;
	Pui[x[x_6]] = 13;Ad /= Pui[x[x_6]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 26;
	dequals(__LINE__, Ad /= Pui[x[x_6]], (DOUBLE)2.);
	Pui[x[x_6]] = 34;
	Ad = 17;Pui[x[x_6]] /= Ad;
	iequals(__LINE__, Pui[x[x_6]], (UINT)2);
	Pui[x[x_6]] = 34;
	iequals(__LINE__, Pui[x[x_6]] /= Ad, (UINT)2);
	Ad = 28;
	Pl[x[x_7]] = 14;Ad /= Pl[x[x_7]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 28;
	dequals(__LINE__, Ad /= Pl[x[x_7]], (DOUBLE)2.);
	Pl[x[x_7]] = 34;
	Ad = 17;Pl[x[x_7]] /= Ad;
	lequals(__LINE__, Pl[x[x_7]], (LONG)2L);
	Pl[x[x_7]] = 34;
	lequals(__LINE__, Pl[x[x_7]] /= Ad, (LONG)2L);
	Ad = 30;
	Pul[x[x_8]] = 15;Ad /= Pul[x[x_8]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 30;
	dequals(__LINE__, Ad /= Pul[x[x_8]], (DOUBLE)2.);
	Pul[x[x_8]] = 34;
	Ad = 17;Pul[x[x_8]] /= Ad;
	lequals(__LINE__, Pul[x[x_8]], (ULONG)2L);
	Pul[x[x_8]] = 34;
	lequals(__LINE__, Pul[x[x_8]] /= Ad, (ULONG)2L);
	Ad = 32;
	Pf[x[x_9]] = 16;Ad /= Pf[x[x_9]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 32;
	dequals(__LINE__, Ad /= Pf[x[x_9]], (DOUBLE)2.);
	Pf[x[x_9]] = 34;
	Ad = 17;Pf[x[x_9]] /= Ad;
	dequals(__LINE__, Pf[x[x_9]], (FLOAT)2.);
	Pf[x[x_9]] = 34;
	dequals(__LINE__, Pf[x[x_9]] /= Ad, (FLOAT)2.);
	Ad = 34;
	Pd[x[x_10]] = 17;Ad /= Pd[x[x_10]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 34;
	dequals(__LINE__, Ad /= Pd[x[x_10]], (DOUBLE)2.);
	Pd[x[x_10]] = 34;
	Ad = 17;Pd[x[x_10]] /= Ad;
	dequals(__LINE__, Pd[x[x_10]], (DOUBLE)2.);
	Pd[x[x_10]] = 34;
	dequals(__LINE__, Pd[x[x_10]] /= Ad, (DOUBLE)2.);
#if ANSI
	Ad = 36;
	Pld[x[x_11]] = 18;Ad /= Pld[x[x_11]];
	dequals(__LINE__, Ad, (DOUBLE)2.);
	Ad = 36;
	dequals(__LINE__, Ad /= Pld[x[x_11]], (DOUBLE)2.);
	Pld[x[x_11]] = 34;
	Ad = 17;Pld[x[x_11]] /= Ad;
	ldequals(__LINE__, Pld[x[x_11]], (LDOUBLE)2.L);
	Pld[x[x_11]] = 34;
	ldequals(__LINE__, Pld[x[x_11]] /= Ad, (LDOUBLE)2.L);
#endif
#if ANSI
	Ald = 14;
	Pc[x[x_0]] = 7;Ald /= Pc[x[x_0]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 14;
	ldequals(__LINE__, Ald /= Pc[x[x_0]], (LDOUBLE)2.L);
	Pc[x[x_0]] = 36;
	Ald = 18;Pc[x[x_0]] /= Ald;
	iequals(__LINE__, Pc[x[x_0]], (CHAR)2);
	Pc[x[x_0]] = 36;
	iequals(__LINE__, Pc[x[x_0]] /= Ald, (CHAR)2);
#endif
#if ANSI
	Ald = 16;
	Psc[x[x_1]] = 8;Ald /= Psc[x[x_1]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 16;
	ldequals(__LINE__, Ald /= Psc[x[x_1]], (LDOUBLE)2.L);
	Psc[x[x_1]] = 36;
	Ald = 18;Psc[x[x_1]] /= Ald;
	iequals(__LINE__, Psc[x[x_1]], (SCHAR)2);
	Psc[x[x_1]] = 36;
	iequals(__LINE__, Psc[x[x_1]] /= Ald, (SCHAR)2);
#endif
#if ANSI
	Ald = 18;
	Ps[x[x_2]] = 9;Ald /= Ps[x[x_2]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 18;
	ldequals(__LINE__, Ald /= Ps[x[x_2]], (LDOUBLE)2.L);
	Ps[x[x_2]] = 36;
	Ald = 18;Ps[x[x_2]] /= Ald;
	iequals(__LINE__, Ps[x[x_2]], (SHORT)2);
	Ps[x[x_2]] = 36;
	iequals(__LINE__, Ps[x[x_2]] /= Ald, (SHORT)2);
#endif
#if ANSI
	Ald = 20;
	Pi[x[x_3]] = 10;Ald /= Pi[x[x_3]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 20;
	ldequals(__LINE__, Ald /= Pi[x[x_3]], (LDOUBLE)2.L);
	Pi[x[x_3]] = 36;
	Ald = 18;Pi[x[x_3]] /= Ald;
	iequals(__LINE__, Pi[x[x_3]], (INT)2);
	Pi[x[x_3]] = 36;
	iequals(__LINE__, Pi[x[x_3]] /= Ald, (INT)2);
#endif
#if ANSI
	Ald = 22;
	Puc[x[x_4]] = 11;Ald /= Puc[x[x_4]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 22;
	ldequals(__LINE__, Ald /= Puc[x[x_4]], (LDOUBLE)2.L);
	Puc[x[x_4]] = 36;
	Ald = 18;Puc[x[x_4]] /= Ald;
	iequals(__LINE__, Puc[x[x_4]], (UCHAR)2);
	Puc[x[x_4]] = 36;
	iequals(__LINE__, Puc[x[x_4]] /= Ald, (UCHAR)2);
#endif
#if ANSI
	Ald = 24;
	Pus[x[x_5]] = 12;Ald /= Pus[x[x_5]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 24;
	ldequals(__LINE__, Ald /= Pus[x[x_5]], (LDOUBLE)2.L);
	Pus[x[x_5]] = 36;
	Ald = 18;Pus[x[x_5]] /= Ald;
	iequals(__LINE__, Pus[x[x_5]], (USHORT)2);
	Pus[x[x_5]] = 36;
	iequals(__LINE__, Pus[x[x_5]] /= Ald, (USHORT)2);
#endif
#if ANSI
	Ald = 26;
	Pui[x[x_6]] = 13;Ald /= Pui[x[x_6]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 26;
	ldequals(__LINE__, Ald /= Pui[x[x_6]], (LDOUBLE)2.L);
	Pui[x[x_6]] = 36;
	Ald = 18;Pui[x[x_6]] /= Ald;
	iequals(__LINE__, Pui[x[x_6]], (UINT)2);
	Pui[x[x_6]] = 36;
	iequals(__LINE__, Pui[x[x_6]] /= Ald, (UINT)2);
#endif
#if ANSI
	Ald = 28;
	Pl[x[x_7]] = 14;Ald /= Pl[x[x_7]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 28;
	ldequals(__LINE__, Ald /= Pl[x[x_7]], (LDOUBLE)2.L);
	Pl[x[x_7]] = 36;
	Ald = 18;Pl[x[x_7]] /= Ald;
	lequals(__LINE__, Pl[x[x_7]], (LONG)2L);
	Pl[x[x_7]] = 36;
	lequals(__LINE__, Pl[x[x_7]] /= Ald, (LONG)2L);
#endif
#if ANSI
	Ald = 30;
	Pul[x[x_8]] = 15;Ald /= Pul[x[x_8]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 30;
	ldequals(__LINE__, Ald /= Pul[x[x_8]], (LDOUBLE)2.L);
	Pul[x[x_8]] = 36;
	Ald = 18;Pul[x[x_8]] /= Ald;
	lequals(__LINE__, Pul[x[x_8]], (ULONG)2L);
	Pul[x[x_8]] = 36;
	lequals(__LINE__, Pul[x[x_8]] /= Ald, (ULONG)2L);
#endif
#if ANSI
	Ald = 32;
	Pf[x[x_9]] = 16;Ald /= Pf[x[x_9]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 32;
	ldequals(__LINE__, Ald /= Pf[x[x_9]], (LDOUBLE)2.L);
	Pf[x[x_9]] = 36;
	Ald = 18;Pf[x[x_9]] /= Ald;
	dequals(__LINE__, Pf[x[x_9]], (FLOAT)2.);
	Pf[x[x_9]] = 36;
	dequals(__LINE__, Pf[x[x_9]] /= Ald, (FLOAT)2.);
#endif
#if ANSI
	Ald = 34;
	Pd[x[x_10]] = 17;Ald /= Pd[x[x_10]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 34;
	ldequals(__LINE__, Ald /= Pd[x[x_10]], (LDOUBLE)2.L);
	Pd[x[x_10]] = 36;
	Ald = 18;Pd[x[x_10]] /= Ald;
	dequals(__LINE__, Pd[x[x_10]], (DOUBLE)2.);
	Pd[x[x_10]] = 36;
	dequals(__LINE__, Pd[x[x_10]] /= Ald, (DOUBLE)2.);
#endif
#if ANSI
	Ald = 36;
	Pld[x[x_11]] = 18;Ald /= Pld[x[x_11]];
	ldequals(__LINE__, Ald, (LDOUBLE)2.L);
	Ald = 36;
	ldequals(__LINE__, Ald /= Pld[x[x_11]], (LDOUBLE)2.L);
	Pld[x[x_11]] = 36;
	Ald = 18;Pld[x[x_11]] /= Ald;
	ldequals(__LINE__, Pld[x[x_11]], (LDOUBLE)2.L);
	Pld[x[x_11]] = 36;
	ldequals(__LINE__, Pld[x[x_11]] /= Ald, (LDOUBLE)2.L);
#endif
	}}
	return 	report(Filename);
	}
