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
	auto CHAR Ic[12];
	int x_0 = 0;
#if ANSI
	auto SCHAR Isc[12];
	int x_1 = 1;
#endif
	auto SHORT Is[12];
	int x_2 = 2;
	auto INT Ii[12];
	int x_3 = 3;
	auto UCHAR Iuc[12];
	int x_4 = 4;
	auto USHORT Ius[12];
	int x_5 = 5;
	auto UINT Iui[12];
	int x_6 = 6;
	auto LONG Il[12];
	int x_7 = 7;
	auto ULONG Iul[12];
	int x_8 = 8;
	auto FLOAT If[12];
	int x_9 = 9;
	auto DOUBLE Id[12];
	int x_10 = 10;
#if ANSI
	auto LDOUBLE Ild[12];
	int x_11 = 11;
#endif
	Ic[x_0] = 7;
#if ANSI
	Isc[x_1] = 8;
#endif
	Is[x_2] = 9;
	Ii[x_3] = 10;
	Iuc[x_4] = 11;
	Ius[x_5] = 12;
	Iui[x_6] = 13;
	Il[x_7] = 14;
	Iul[x_8] = 15;
	If[x_9] = 16;
	Id[x_10] = 17;
#if ANSI
	Ild[x_11] = 18;
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
	Filename =  " auto array1 auto arrarr or ";
	iequals(__LINE__, Ic[x_0] | Pc[x[x_0]], 7);
	iequals(__LINE__, Pc[x[x_0]] | Ic[x_0], 7);
#if ANSI
	iequals(__LINE__, Ic[x_0] | Psc[x[x_1]], 15);
	iequals(__LINE__, Psc[x[x_1]] | Ic[x_0], 15);
#endif
	iequals(__LINE__, Ic[x_0] | Ps[x[x_2]], 15);
	iequals(__LINE__, Ps[x[x_2]] | Ic[x_0], 15);
	iequals(__LINE__, Ic[x_0] | Pi[x[x_3]], 15);
	iequals(__LINE__, Pi[x[x_3]] | Ic[x_0], 15);
	iequals(__LINE__, Ic[x_0] | Puc[x[x_4]], 15);
	iequals(__LINE__, Puc[x[x_4]] | Ic[x_0], 15);
	iequals(__LINE__, Ic[x_0] | Pus[x[x_5]], 15);
	iequals(__LINE__, Pus[x[x_5]] | Ic[x_0], 15);
	iequals(__LINE__, Ic[x_0] | Pui[x[x_6]], 15);
	iequals(__LINE__, Pui[x[x_6]] | Ic[x_0], 15);
	lequals(__LINE__, Ic[x_0] | Pl[x[x_7]], 15L);
	lequals(__LINE__, Pl[x[x_7]] | Ic[x_0], 15L);
	lequals(__LINE__, Ic[x_0] | Pul[x[x_8]], 15L);
	lequals(__LINE__, Pul[x[x_8]] | Ic[x_0], 15L);
#if ANSI
	iequals(__LINE__, Isc[x_1] | Pc[x[x_0]], 15);
	iequals(__LINE__, Pc[x[x_0]] | Isc[x_1], 15);
#endif
#if ANSI
	iequals(__LINE__, Isc[x_1] | Psc[x[x_1]], 8);
	iequals(__LINE__, Psc[x[x_1]] | Isc[x_1], 8);
#endif
#if ANSI
	iequals(__LINE__, Isc[x_1] | Ps[x[x_2]], 9);
	iequals(__LINE__, Ps[x[x_2]] | Isc[x_1], 9);
#endif
#if ANSI
	iequals(__LINE__, Isc[x_1] | Pi[x[x_3]], 10);
	iequals(__LINE__, Pi[x[x_3]] | Isc[x_1], 10);
#endif
#if ANSI
	iequals(__LINE__, Isc[x_1] | Puc[x[x_4]], 11);
	iequals(__LINE__, Puc[x[x_4]] | Isc[x_1], 11);
#endif
#if ANSI
	iequals(__LINE__, Isc[x_1] | Pus[x[x_5]], 12);
	iequals(__LINE__, Pus[x[x_5]] | Isc[x_1], 12);
#endif
#if ANSI
	iequals(__LINE__, Isc[x_1] | Pui[x[x_6]], 13);
	iequals(__LINE__, Pui[x[x_6]] | Isc[x_1], 13);
#endif
#if ANSI
	lequals(__LINE__, Isc[x_1] | Pl[x[x_7]], 14L);
	lequals(__LINE__, Pl[x[x_7]] | Isc[x_1], 14L);
#endif
#if ANSI
	lequals(__LINE__, Isc[x_1] | Pul[x[x_8]], 15L);
	lequals(__LINE__, Pul[x[x_8]] | Isc[x_1], 15L);
#endif
	iequals(__LINE__, Is[x_2] | Pc[x[x_0]], 15);
	iequals(__LINE__, Pc[x[x_0]] | Is[x_2], 15);
#if ANSI
	iequals(__LINE__, Is[x_2] | Psc[x[x_1]], 9);
	iequals(__LINE__, Psc[x[x_1]] | Is[x_2], 9);
#endif
	iequals(__LINE__, Is[x_2] | Ps[x[x_2]], 9);
	iequals(__LINE__, Ps[x[x_2]] | Is[x_2], 9);
	iequals(__LINE__, Is[x_2] | Pi[x[x_3]], 11);
	iequals(__LINE__, Pi[x[x_3]] | Is[x_2], 11);
	iequals(__LINE__, Is[x_2] | Puc[x[x_4]], 11);
	iequals(__LINE__, Puc[x[x_4]] | Is[x_2], 11);
	iequals(__LINE__, Is[x_2] | Pus[x[x_5]], 13);
	iequals(__LINE__, Pus[x[x_5]] | Is[x_2], 13);
	iequals(__LINE__, Is[x_2] | Pui[x[x_6]], 13);
	iequals(__LINE__, Pui[x[x_6]] | Is[x_2], 13);
	lequals(__LINE__, Is[x_2] | Pl[x[x_7]], 15L);
	lequals(__LINE__, Pl[x[x_7]] | Is[x_2], 15L);
	lequals(__LINE__, Is[x_2] | Pul[x[x_8]], 15L);
	lequals(__LINE__, Pul[x[x_8]] | Is[x_2], 15L);
	iequals(__LINE__, Ii[x_3] | Pc[x[x_0]], 15);
	iequals(__LINE__, Pc[x[x_0]] | Ii[x_3], 15);
#if ANSI
	iequals(__LINE__, Ii[x_3] | Psc[x[x_1]], 10);
	iequals(__LINE__, Psc[x[x_1]] | Ii[x_3], 10);
#endif
	iequals(__LINE__, Ii[x_3] | Ps[x[x_2]], 11);
	iequals(__LINE__, Ps[x[x_2]] | Ii[x_3], 11);
	iequals(__LINE__, Ii[x_3] | Pi[x[x_3]], 10);
	iequals(__LINE__, Pi[x[x_3]] | Ii[x_3], 10);
	iequals(__LINE__, Ii[x_3] | Puc[x[x_4]], 11);
	iequals(__LINE__, Puc[x[x_4]] | Ii[x_3], 11);
	iequals(__LINE__, Ii[x_3] | Pus[x[x_5]], 14);
	iequals(__LINE__, Pus[x[x_5]] | Ii[x_3], 14);
	iequals(__LINE__, Ii[x_3] | Pui[x[x_6]], 15);
	iequals(__LINE__, Pui[x[x_6]] | Ii[x_3], 15);
	lequals(__LINE__, Ii[x_3] | Pl[x[x_7]], 14L);
	lequals(__LINE__, Pl[x[x_7]] | Ii[x_3], 14L);
	lequals(__LINE__, Ii[x_3] | Pul[x[x_8]], 15L);
	lequals(__LINE__, Pul[x[x_8]] | Ii[x_3], 15L);
	iequals(__LINE__, Iuc[x_4] | Pc[x[x_0]], 15);
	iequals(__LINE__, Pc[x[x_0]] | Iuc[x_4], 15);
#if ANSI
	iequals(__LINE__, Iuc[x_4] | Psc[x[x_1]], 11);
	iequals(__LINE__, Psc[x[x_1]] | Iuc[x_4], 11);
#endif
	iequals(__LINE__, Iuc[x_4] | Ps[x[x_2]], 11);
	iequals(__LINE__, Ps[x[x_2]] | Iuc[x_4], 11);
	iequals(__LINE__, Iuc[x_4] | Pi[x[x_3]], 11);
	iequals(__LINE__, Pi[x[x_3]] | Iuc[x_4], 11);
	iequals(__LINE__, Iuc[x_4] | Puc[x[x_4]], 11);
	iequals(__LINE__, Puc[x[x_4]] | Iuc[x_4], 11);
	iequals(__LINE__, Iuc[x_4] | Pus[x[x_5]], 15);
	iequals(__LINE__, Pus[x[x_5]] | Iuc[x_4], 15);
	iequals(__LINE__, Iuc[x_4] | Pui[x[x_6]], 15);
	iequals(__LINE__, Pui[x[x_6]] | Iuc[x_4], 15);
	lequals(__LINE__, Iuc[x_4] | Pl[x[x_7]], 15L);
	lequals(__LINE__, Pl[x[x_7]] | Iuc[x_4], 15L);
	lequals(__LINE__, Iuc[x_4] | Pul[x[x_8]], 15L);
	lequals(__LINE__, Pul[x[x_8]] | Iuc[x_4], 15L);
	iequals(__LINE__, Ius[x_5] | Pc[x[x_0]], 15);
	iequals(__LINE__, Pc[x[x_0]] | Ius[x_5], 15);
#if ANSI
	iequals(__LINE__, Ius[x_5] | Psc[x[x_1]], 12);
	iequals(__LINE__, Psc[x[x_1]] | Ius[x_5], 12);
#endif
	iequals(__LINE__, Ius[x_5] | Ps[x[x_2]], 13);
	iequals(__LINE__, Ps[x[x_2]] | Ius[x_5], 13);
	iequals(__LINE__, Ius[x_5] | Pi[x[x_3]], 14);
	iequals(__LINE__, Pi[x[x_3]] | Ius[x_5], 14);
	iequals(__LINE__, Ius[x_5] | Puc[x[x_4]], 15);
	iequals(__LINE__, Puc[x[x_4]] | Ius[x_5], 15);
	iequals(__LINE__, Ius[x_5] | Pus[x[x_5]], 12);
	iequals(__LINE__, Pus[x[x_5]] | Ius[x_5], 12);
	iequals(__LINE__, Ius[x_5] | Pui[x[x_6]], 13);
	iequals(__LINE__, Pui[x[x_6]] | Ius[x_5], 13);
	lequals(__LINE__, Ius[x_5] | Pl[x[x_7]], 14L);
	lequals(__LINE__, Pl[x[x_7]] | Ius[x_5], 14L);
	lequals(__LINE__, Ius[x_5] | Pul[x[x_8]], 15L);
	lequals(__LINE__, Pul[x[x_8]] | Ius[x_5], 15L);
	iequals(__LINE__, Iui[x_6] | Pc[x[x_0]], 15);
	iequals(__LINE__, Pc[x[x_0]] | Iui[x_6], 15);
#if ANSI
	iequals(__LINE__, Iui[x_6] | Psc[x[x_1]], 13);
	iequals(__LINE__, Psc[x[x_1]] | Iui[x_6], 13);
#endif
	iequals(__LINE__, Iui[x_6] | Ps[x[x_2]], 13);
	iequals(__LINE__, Ps[x[x_2]] | Iui[x_6], 13);
	iequals(__LINE__, Iui[x_6] | Pi[x[x_3]], 15);
	iequals(__LINE__, Pi[x[x_3]] | Iui[x_6], 15);
	iequals(__LINE__, Iui[x_6] | Puc[x[x_4]], 15);
	iequals(__LINE__, Puc[x[x_4]] | Iui[x_6], 15);
	iequals(__LINE__, Iui[x_6] | Pus[x[x_5]], 13);
	iequals(__LINE__, Pus[x[x_5]] | Iui[x_6], 13);
	iequals(__LINE__, Iui[x_6] | Pui[x[x_6]], 13);
	iequals(__LINE__, Pui[x[x_6]] | Iui[x_6], 13);
	lequals(__LINE__, Iui[x_6] | Pl[x[x_7]], 15L);
	lequals(__LINE__, Pl[x[x_7]] | Iui[x_6], 15L);
	lequals(__LINE__, Iui[x_6] | Pul[x[x_8]], 15L);
	lequals(__LINE__, Pul[x[x_8]] | Iui[x_6], 15L);
	lequals(__LINE__, Il[x_7] | Pc[x[x_0]], 15L);
	lequals(__LINE__, Pc[x[x_0]] | Il[x_7], 15L);
#if ANSI
	lequals(__LINE__, Il[x_7] | Psc[x[x_1]], 14L);
	lequals(__LINE__, Psc[x[x_1]] | Il[x_7], 14L);
#endif
	lequals(__LINE__, Il[x_7] | Ps[x[x_2]], 15L);
	lequals(__LINE__, Ps[x[x_2]] | Il[x_7], 15L);
	lequals(__LINE__, Il[x_7] | Pi[x[x_3]], 14L);
	lequals(__LINE__, Pi[x[x_3]] | Il[x_7], 14L);
	lequals(__LINE__, Il[x_7] | Puc[x[x_4]], 15L);
	lequals(__LINE__, Puc[x[x_4]] | Il[x_7], 15L);
	lequals(__LINE__, Il[x_7] | Pus[x[x_5]], 14L);
	lequals(__LINE__, Pus[x[x_5]] | Il[x_7], 14L);
	lequals(__LINE__, Il[x_7] | Pui[x[x_6]], 15L);
	lequals(__LINE__, Pui[x[x_6]] | Il[x_7], 15L);
	lequals(__LINE__, Il[x_7] | Pl[x[x_7]], 14L);
	lequals(__LINE__, Pl[x[x_7]] | Il[x_7], 14L);
	lequals(__LINE__, Il[x_7] | Pul[x[x_8]], 15L);
	lequals(__LINE__, Pul[x[x_8]] | Il[x_7], 15L);
	lequals(__LINE__, Iul[x_8] | Pc[x[x_0]], 15L);
	lequals(__LINE__, Pc[x[x_0]] | Iul[x_8], 15L);
#if ANSI
	lequals(__LINE__, Iul[x_8] | Psc[x[x_1]], 15L);
	lequals(__LINE__, Psc[x[x_1]] | Iul[x_8], 15L);
#endif
	lequals(__LINE__, Iul[x_8] | Ps[x[x_2]], 15L);
	lequals(__LINE__, Ps[x[x_2]] | Iul[x_8], 15L);
	lequals(__LINE__, Iul[x_8] | Pi[x[x_3]], 15L);
	lequals(__LINE__, Pi[x[x_3]] | Iul[x_8], 15L);
	lequals(__LINE__, Iul[x_8] | Puc[x[x_4]], 15L);
	lequals(__LINE__, Puc[x[x_4]] | Iul[x_8], 15L);
	lequals(__LINE__, Iul[x_8] | Pus[x[x_5]], 15L);
	lequals(__LINE__, Pus[x[x_5]] | Iul[x_8], 15L);
	lequals(__LINE__, Iul[x_8] | Pui[x[x_6]], 15L);
	lequals(__LINE__, Pui[x[x_6]] | Iul[x_8], 15L);
	lequals(__LINE__, Iul[x_8] | Pl[x[x_7]], 15L);
	lequals(__LINE__, Pl[x[x_7]] | Iul[x_8], 15L);
	lequals(__LINE__, Iul[x_8] | Pul[x[x_8]], 15L);
	lequals(__LINE__, Pul[x[x_8]] | Iul[x_8], 15L);
	}}
	return 	report(Filename);
	}
