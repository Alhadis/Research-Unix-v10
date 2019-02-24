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
	Filename =  " auto scalar auto arrarr diveq1 ";
	Ac = 14;
	Pc[x[x_0]] = 7;Ac /= Pc[x[x_0]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 14;
	iequals(__LINE__, Ac /= Pc[x[x_0]], (CHAR)2);
	Pc[x[x_0]] = 14;
	Ac = 7;Pc[x[x_0]] /= Ac;
	iequals(__LINE__, Pc[x[x_0]], (CHAR)2);
	Pc[x[x_0]] = 14;
	iequals(__LINE__, Pc[x[x_0]] /= Ac, (CHAR)2);
#if ANSI
	Ac = 16;
	Psc[x[x_1]] = 8;Ac /= Psc[x[x_1]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 16;
	iequals(__LINE__, Ac /= Psc[x[x_1]], (CHAR)2);
	Psc[x[x_1]] = 14;
	Ac = 7;Psc[x[x_1]] /= Ac;
	iequals(__LINE__, Psc[x[x_1]], (SCHAR)2);
	Psc[x[x_1]] = 14;
	iequals(__LINE__, Psc[x[x_1]] /= Ac, (SCHAR)2);
#endif
	Ac = 18;
	Ps[x[x_2]] = 9;Ac /= Ps[x[x_2]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 18;
	iequals(__LINE__, Ac /= Ps[x[x_2]], (CHAR)2);
	Ps[x[x_2]] = 14;
	Ac = 7;Ps[x[x_2]] /= Ac;
	iequals(__LINE__, Ps[x[x_2]], (SHORT)2);
	Ps[x[x_2]] = 14;
	iequals(__LINE__, Ps[x[x_2]] /= Ac, (SHORT)2);
	Ac = 20;
	Pi[x[x_3]] = 10;Ac /= Pi[x[x_3]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 20;
	iequals(__LINE__, Ac /= Pi[x[x_3]], (CHAR)2);
	Pi[x[x_3]] = 14;
	Ac = 7;Pi[x[x_3]] /= Ac;
	iequals(__LINE__, Pi[x[x_3]], (INT)2);
	Pi[x[x_3]] = 14;
	iequals(__LINE__, Pi[x[x_3]] /= Ac, (INT)2);
	Ac = 22;
	Puc[x[x_4]] = 11;Ac /= Puc[x[x_4]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 22;
	iequals(__LINE__, Ac /= Puc[x[x_4]], (CHAR)2);
	Puc[x[x_4]] = 14;
	Ac = 7;Puc[x[x_4]] /= Ac;
	iequals(__LINE__, Puc[x[x_4]], (UCHAR)2);
	Puc[x[x_4]] = 14;
	iequals(__LINE__, Puc[x[x_4]] /= Ac, (UCHAR)2);
	Ac = 24;
	Pus[x[x_5]] = 12;Ac /= Pus[x[x_5]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 24;
	iequals(__LINE__, Ac /= Pus[x[x_5]], (CHAR)2);
	Pus[x[x_5]] = 14;
	Ac = 7;Pus[x[x_5]] /= Ac;
	iequals(__LINE__, Pus[x[x_5]], (USHORT)2);
	Pus[x[x_5]] = 14;
	iequals(__LINE__, Pus[x[x_5]] /= Ac, (USHORT)2);
	Ac = 26;
	Pui[x[x_6]] = 13;Ac /= Pui[x[x_6]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 26;
	iequals(__LINE__, Ac /= Pui[x[x_6]], (CHAR)2);
	Pui[x[x_6]] = 14;
	Ac = 7;Pui[x[x_6]] /= Ac;
	iequals(__LINE__, Pui[x[x_6]], (UINT)2);
	Pui[x[x_6]] = 14;
	iequals(__LINE__, Pui[x[x_6]] /= Ac, (UINT)2);
	Ac = 28;
	Pl[x[x_7]] = 14;Ac /= Pl[x[x_7]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 28;
	iequals(__LINE__, Ac /= Pl[x[x_7]], (CHAR)2);
	Pl[x[x_7]] = 14;
	Ac = 7;Pl[x[x_7]] /= Ac;
	lequals(__LINE__, Pl[x[x_7]], (LONG)2L);
	Pl[x[x_7]] = 14;
	lequals(__LINE__, Pl[x[x_7]] /= Ac, (LONG)2L);
	Ac = 30;
	Pul[x[x_8]] = 15;Ac /= Pul[x[x_8]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 30;
	iequals(__LINE__, Ac /= Pul[x[x_8]], (CHAR)2);
	Pul[x[x_8]] = 14;
	Ac = 7;Pul[x[x_8]] /= Ac;
	lequals(__LINE__, Pul[x[x_8]], (ULONG)2L);
	Pul[x[x_8]] = 14;
	lequals(__LINE__, Pul[x[x_8]] /= Ac, (ULONG)2L);
	Ac = 32;
	Pf[x[x_9]] = 16;Ac /= Pf[x[x_9]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 32;
	iequals(__LINE__, Ac /= Pf[x[x_9]], (CHAR)2);
	Pf[x[x_9]] = 14;
	Ac = 7;Pf[x[x_9]] /= Ac;
	dequals(__LINE__, Pf[x[x_9]], (FLOAT)2.);
	Pf[x[x_9]] = 14;
	dequals(__LINE__, Pf[x[x_9]] /= Ac, (FLOAT)2.);
	Ac = 34;
	Pd[x[x_10]] = 17;Ac /= Pd[x[x_10]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 34;
	iequals(__LINE__, Ac /= Pd[x[x_10]], (CHAR)2);
	Pd[x[x_10]] = 14;
	Ac = 7;Pd[x[x_10]] /= Ac;
	dequals(__LINE__, Pd[x[x_10]], (DOUBLE)2.);
	Pd[x[x_10]] = 14;
	dequals(__LINE__, Pd[x[x_10]] /= Ac, (DOUBLE)2.);
#if ANSI
	Ac = 36;
	Pld[x[x_11]] = 18;Ac /= Pld[x[x_11]];
	iequals(__LINE__, Ac, (CHAR)2);
	Ac = 36;
	iequals(__LINE__, Ac /= Pld[x[x_11]], (CHAR)2);
	Pld[x[x_11]] = 14;
	Ac = 7;Pld[x[x_11]] /= Ac;
	ldequals(__LINE__, Pld[x[x_11]], (LDOUBLE)2.L);
	Pld[x[x_11]] = 14;
	ldequals(__LINE__, Pld[x[x_11]] /= Ac, (LDOUBLE)2.L);
#endif
#if ANSI
	Asc = 14;
	Pc[x[x_0]] = 7;Asc /= Pc[x[x_0]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 14;
	iequals(__LINE__, Asc /= Pc[x[x_0]], (SCHAR)2);
	Pc[x[x_0]] = 16;
	Asc = 8;Pc[x[x_0]] /= Asc;
	iequals(__LINE__, Pc[x[x_0]], (CHAR)2);
	Pc[x[x_0]] = 16;
	iequals(__LINE__, Pc[x[x_0]] /= Asc, (CHAR)2);
#endif
#if ANSI
	Asc = 16;
	Psc[x[x_1]] = 8;Asc /= Psc[x[x_1]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 16;
	iequals(__LINE__, Asc /= Psc[x[x_1]], (SCHAR)2);
	Psc[x[x_1]] = 16;
	Asc = 8;Psc[x[x_1]] /= Asc;
	iequals(__LINE__, Psc[x[x_1]], (SCHAR)2);
	Psc[x[x_1]] = 16;
	iequals(__LINE__, Psc[x[x_1]] /= Asc, (SCHAR)2);
#endif
#if ANSI
	Asc = 18;
	Ps[x[x_2]] = 9;Asc /= Ps[x[x_2]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 18;
	iequals(__LINE__, Asc /= Ps[x[x_2]], (SCHAR)2);
	Ps[x[x_2]] = 16;
	Asc = 8;Ps[x[x_2]] /= Asc;
	iequals(__LINE__, Ps[x[x_2]], (SHORT)2);
	Ps[x[x_2]] = 16;
	iequals(__LINE__, Ps[x[x_2]] /= Asc, (SHORT)2);
#endif
#if ANSI
	Asc = 20;
	Pi[x[x_3]] = 10;Asc /= Pi[x[x_3]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 20;
	iequals(__LINE__, Asc /= Pi[x[x_3]], (SCHAR)2);
	Pi[x[x_3]] = 16;
	Asc = 8;Pi[x[x_3]] /= Asc;
	iequals(__LINE__, Pi[x[x_3]], (INT)2);
	Pi[x[x_3]] = 16;
	iequals(__LINE__, Pi[x[x_3]] /= Asc, (INT)2);
#endif
#if ANSI
	Asc = 22;
	Puc[x[x_4]] = 11;Asc /= Puc[x[x_4]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 22;
	iequals(__LINE__, Asc /= Puc[x[x_4]], (SCHAR)2);
	Puc[x[x_4]] = 16;
	Asc = 8;Puc[x[x_4]] /= Asc;
	iequals(__LINE__, Puc[x[x_4]], (UCHAR)2);
	Puc[x[x_4]] = 16;
	iequals(__LINE__, Puc[x[x_4]] /= Asc, (UCHAR)2);
#endif
#if ANSI
	Asc = 24;
	Pus[x[x_5]] = 12;Asc /= Pus[x[x_5]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 24;
	iequals(__LINE__, Asc /= Pus[x[x_5]], (SCHAR)2);
	Pus[x[x_5]] = 16;
	Asc = 8;Pus[x[x_5]] /= Asc;
	iequals(__LINE__, Pus[x[x_5]], (USHORT)2);
	Pus[x[x_5]] = 16;
	iequals(__LINE__, Pus[x[x_5]] /= Asc, (USHORT)2);
#endif
#if ANSI
	Asc = 26;
	Pui[x[x_6]] = 13;Asc /= Pui[x[x_6]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 26;
	iequals(__LINE__, Asc /= Pui[x[x_6]], (SCHAR)2);
	Pui[x[x_6]] = 16;
	Asc = 8;Pui[x[x_6]] /= Asc;
	iequals(__LINE__, Pui[x[x_6]], (UINT)2);
	Pui[x[x_6]] = 16;
	iequals(__LINE__, Pui[x[x_6]] /= Asc, (UINT)2);
#endif
#if ANSI
	Asc = 28;
	Pl[x[x_7]] = 14;Asc /= Pl[x[x_7]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 28;
	iequals(__LINE__, Asc /= Pl[x[x_7]], (SCHAR)2);
	Pl[x[x_7]] = 16;
	Asc = 8;Pl[x[x_7]] /= Asc;
	lequals(__LINE__, Pl[x[x_7]], (LONG)2L);
	Pl[x[x_7]] = 16;
	lequals(__LINE__, Pl[x[x_7]] /= Asc, (LONG)2L);
#endif
#if ANSI
	Asc = 30;
	Pul[x[x_8]] = 15;Asc /= Pul[x[x_8]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 30;
	iequals(__LINE__, Asc /= Pul[x[x_8]], (SCHAR)2);
	Pul[x[x_8]] = 16;
	Asc = 8;Pul[x[x_8]] /= Asc;
	lequals(__LINE__, Pul[x[x_8]], (ULONG)2L);
	Pul[x[x_8]] = 16;
	lequals(__LINE__, Pul[x[x_8]] /= Asc, (ULONG)2L);
#endif
#if ANSI
	Asc = 32;
	Pf[x[x_9]] = 16;Asc /= Pf[x[x_9]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 32;
	iequals(__LINE__, Asc /= Pf[x[x_9]], (SCHAR)2);
	Pf[x[x_9]] = 16;
	Asc = 8;Pf[x[x_9]] /= Asc;
	dequals(__LINE__, Pf[x[x_9]], (FLOAT)2.);
	Pf[x[x_9]] = 16;
	dequals(__LINE__, Pf[x[x_9]] /= Asc, (FLOAT)2.);
#endif
#if ANSI
	Asc = 34;
	Pd[x[x_10]] = 17;Asc /= Pd[x[x_10]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 34;
	iequals(__LINE__, Asc /= Pd[x[x_10]], (SCHAR)2);
	Pd[x[x_10]] = 16;
	Asc = 8;Pd[x[x_10]] /= Asc;
	dequals(__LINE__, Pd[x[x_10]], (DOUBLE)2.);
	Pd[x[x_10]] = 16;
	dequals(__LINE__, Pd[x[x_10]] /= Asc, (DOUBLE)2.);
#endif
#if ANSI
	Asc = 36;
	Pld[x[x_11]] = 18;Asc /= Pld[x[x_11]];
	iequals(__LINE__, Asc, (SCHAR)2);
	Asc = 36;
	iequals(__LINE__, Asc /= Pld[x[x_11]], (SCHAR)2);
	Pld[x[x_11]] = 16;
	Asc = 8;Pld[x[x_11]] /= Asc;
	ldequals(__LINE__, Pld[x[x_11]], (LDOUBLE)2.L);
	Pld[x[x_11]] = 16;
	ldequals(__LINE__, Pld[x[x_11]] /= Asc, (LDOUBLE)2.L);
#endif
	As = 14;
	Pc[x[x_0]] = 7;As /= Pc[x[x_0]];
	iequals(__LINE__, As, (SHORT)2);
	As = 14;
	iequals(__LINE__, As /= Pc[x[x_0]], (SHORT)2);
	Pc[x[x_0]] = 18;
	As = 9;Pc[x[x_0]] /= As;
	iequals(__LINE__, Pc[x[x_0]], (CHAR)2);
	Pc[x[x_0]] = 18;
	iequals(__LINE__, Pc[x[x_0]] /= As, (CHAR)2);
#if ANSI
	As = 16;
	Psc[x[x_1]] = 8;As /= Psc[x[x_1]];
	iequals(__LINE__, As, (SHORT)2);
	As = 16;
	iequals(__LINE__, As /= Psc[x[x_1]], (SHORT)2);
	Psc[x[x_1]] = 18;
	As = 9;Psc[x[x_1]] /= As;
	iequals(__LINE__, Psc[x[x_1]], (SCHAR)2);
	Psc[x[x_1]] = 18;
	iequals(__LINE__, Psc[x[x_1]] /= As, (SCHAR)2);
#endif
	As = 18;
	Ps[x[x_2]] = 9;As /= Ps[x[x_2]];
	iequals(__LINE__, As, (SHORT)2);
	As = 18;
	iequals(__LINE__, As /= Ps[x[x_2]], (SHORT)2);
	Ps[x[x_2]] = 18;
	As = 9;Ps[x[x_2]] /= As;
	iequals(__LINE__, Ps[x[x_2]], (SHORT)2);
	Ps[x[x_2]] = 18;
	iequals(__LINE__, Ps[x[x_2]] /= As, (SHORT)2);
	As = 20;
	Pi[x[x_3]] = 10;As /= Pi[x[x_3]];
	iequals(__LINE__, As, (SHORT)2);
	As = 20;
	iequals(__LINE__, As /= Pi[x[x_3]], (SHORT)2);
	Pi[x[x_3]] = 18;
	As = 9;Pi[x[x_3]] /= As;
	iequals(__LINE__, Pi[x[x_3]], (INT)2);
	Pi[x[x_3]] = 18;
	iequals(__LINE__, Pi[x[x_3]] /= As, (INT)2);
	As = 22;
	Puc[x[x_4]] = 11;As /= Puc[x[x_4]];
	iequals(__LINE__, As, (SHORT)2);
	As = 22;
	iequals(__LINE__, As /= Puc[x[x_4]], (SHORT)2);
	Puc[x[x_4]] = 18;
	As = 9;Puc[x[x_4]] /= As;
	iequals(__LINE__, Puc[x[x_4]], (UCHAR)2);
	Puc[x[x_4]] = 18;
	iequals(__LINE__, Puc[x[x_4]] /= As, (UCHAR)2);
	As = 24;
	Pus[x[x_5]] = 12;As /= Pus[x[x_5]];
	iequals(__LINE__, As, (SHORT)2);
	As = 24;
	iequals(__LINE__, As /= Pus[x[x_5]], (SHORT)2);
	Pus[x[x_5]] = 18;
	As = 9;Pus[x[x_5]] /= As;
	iequals(__LINE__, Pus[x[x_5]], (USHORT)2);
	Pus[x[x_5]] = 18;
	iequals(__LINE__, Pus[x[x_5]] /= As, (USHORT)2);
	}}
	return 	report(Filename);
	}
