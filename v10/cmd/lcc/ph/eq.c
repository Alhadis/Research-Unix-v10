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
	auto CHAR Bc = 7;
	auto CHAR *pBc = &Bc;
#if ANSI
	auto SCHAR Bsc = 8;
	auto SCHAR *pBsc = &Bsc;
#endif
	auto SHORT Bs = 9;
	auto SHORT *pBs = &Bs;
	auto INT Bi = 10;
	auto INT *pBi = &Bi;
	auto UCHAR Buc = 11;
	auto UCHAR *pBuc = &Buc;
	auto USHORT Bus = 12;
	auto USHORT *pBus = &Bus;
	auto UINT Bui = 13;
	auto UINT *pBui = &Bui;
	auto LONG Bl = 14;
	auto LONG *pBl = &Bl;
	auto ULONG Bul = 15;
	auto ULONG *pBul = &Bul;
	auto FLOAT Bf = 16;
	auto FLOAT *pBf = &Bf;
	auto DOUBLE Bd = 17;
	auto DOUBLE *pBd = &Bd;
#if ANSI
	auto LDOUBLE Bld = 18;
	auto LDOUBLE *pBld = &Bld;
#endif
	{
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
	int true = 1, false = 0;
	Filename =  " auto pscalar1 auto array1 eq ";
	iequals(__LINE__, *pBc == Ic[x_0], 1);
	iequals(__LINE__, Ic[x_0] == *pBc, 1);
	if (*pBc == Ic[x_0]); else complain(__LINE__);
	if (Ic[x_0] == *pBc); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBc == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBc, 0);
	if (*pBc == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBc) complain(__LINE__);
#endif
	iequals(__LINE__, *pBc == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBc, 0);
	if (*pBc == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBc) complain(__LINE__);
	iequals(__LINE__, *pBc == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBc, 0);
	if (*pBc == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBc) complain(__LINE__);
	iequals(__LINE__, *pBc == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBc, 0);
	if (*pBc == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBc) complain(__LINE__);
	iequals(__LINE__, *pBc == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBc, 0);
	if (*pBc == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBc) complain(__LINE__);
	iequals(__LINE__, *pBc == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBc, 0);
	if (*pBc == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBc) complain(__LINE__);
	iequals(__LINE__, *pBc == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBc, 0);
	if (*pBc == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBc) complain(__LINE__);
	iequals(__LINE__, *pBc == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBc, 0);
	if (*pBc == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBc) complain(__LINE__);
	iequals(__LINE__, *pBc == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBc, 0);
	if (*pBc == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBc) complain(__LINE__);
	iequals(__LINE__, *pBc == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBc, 0);
	if (*pBc == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBc) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBc == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBc, 0);
	if (*pBc == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBsc, 0);
	if (*pBsc == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Isc[x_1], 1);
	iequals(__LINE__, Isc[x_1] == *pBsc, 1);
	if (*pBsc == Isc[x_1]); else complain(__LINE__);
	if (Isc[x_1] == *pBsc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBsc, 0);
	if (*pBsc == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBsc, 0);
	if (*pBsc == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBsc, 0);
	if (*pBsc == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBsc, 0);
	if (*pBsc == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBsc, 0);
	if (*pBsc == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBsc, 0);
	if (*pBsc == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBsc, 0);
	if (*pBsc == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBsc, 0);
	if (*pBsc == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBsc, 0);
	if (*pBsc == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBsc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBsc == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBsc, 0);
	if (*pBsc == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBsc) complain(__LINE__);
#endif
	iequals(__LINE__, *pBs == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBs, 0);
	if (*pBs == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBs) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBs == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBs, 0);
	if (*pBs == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBs) complain(__LINE__);
#endif
	iequals(__LINE__, *pBs == Is[x_2], 1);
	iequals(__LINE__, Is[x_2] == *pBs, 1);
	if (*pBs == Is[x_2]); else complain(__LINE__);
	if (Is[x_2] == *pBs); else complain(__LINE__);
	iequals(__LINE__, *pBs == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBs, 0);
	if (*pBs == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBs) complain(__LINE__);
	iequals(__LINE__, *pBs == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBs, 0);
	if (*pBs == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBs) complain(__LINE__);
	iequals(__LINE__, *pBs == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBs, 0);
	if (*pBs == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBs) complain(__LINE__);
	iequals(__LINE__, *pBs == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBs, 0);
	if (*pBs == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBs) complain(__LINE__);
	iequals(__LINE__, *pBs == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBs, 0);
	if (*pBs == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBs) complain(__LINE__);
	iequals(__LINE__, *pBs == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBs, 0);
	if (*pBs == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBs) complain(__LINE__);
	iequals(__LINE__, *pBs == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBs, 0);
	if (*pBs == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBs) complain(__LINE__);
	iequals(__LINE__, *pBs == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBs, 0);
	if (*pBs == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBs) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBs == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBs, 0);
	if (*pBs == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBs) complain(__LINE__);
#endif
	iequals(__LINE__, *pBi == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBi, 0);
	if (*pBi == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBi) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBi == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBi, 0);
	if (*pBi == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBi) complain(__LINE__);
#endif
	iequals(__LINE__, *pBi == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBi, 0);
	if (*pBi == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBi) complain(__LINE__);
	iequals(__LINE__, *pBi == Ii[x_3], 1);
	iequals(__LINE__, Ii[x_3] == *pBi, 1);
	if (*pBi == Ii[x_3]); else complain(__LINE__);
	if (Ii[x_3] == *pBi); else complain(__LINE__);
	iequals(__LINE__, *pBi == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBi, 0);
	if (*pBi == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBi) complain(__LINE__);
	iequals(__LINE__, *pBi == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBi, 0);
	if (*pBi == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBi) complain(__LINE__);
	iequals(__LINE__, *pBi == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBi, 0);
	if (*pBi == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBi) complain(__LINE__);
	iequals(__LINE__, *pBi == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBi, 0);
	if (*pBi == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBi) complain(__LINE__);
	iequals(__LINE__, *pBi == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBi, 0);
	if (*pBi == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBi) complain(__LINE__);
	iequals(__LINE__, *pBi == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBi, 0);
	if (*pBi == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBi) complain(__LINE__);
	iequals(__LINE__, *pBi == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBi, 0);
	if (*pBi == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBi) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBi == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBi, 0);
	if (*pBi == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBi) complain(__LINE__);
#endif
	iequals(__LINE__, *pBuc == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBuc, 0);
	if (*pBuc == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBuc) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBuc == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBuc, 0);
	if (*pBuc == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBuc) complain(__LINE__);
#endif
	iequals(__LINE__, *pBuc == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBuc, 0);
	if (*pBuc == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBuc) complain(__LINE__);
	iequals(__LINE__, *pBuc == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBuc, 0);
	if (*pBuc == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBuc) complain(__LINE__);
	iequals(__LINE__, *pBuc == Iuc[x_4], 1);
	iequals(__LINE__, Iuc[x_4] == *pBuc, 1);
	if (*pBuc == Iuc[x_4]); else complain(__LINE__);
	if (Iuc[x_4] == *pBuc); else complain(__LINE__);
	iequals(__LINE__, *pBuc == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBuc, 0);
	if (*pBuc == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBuc) complain(__LINE__);
	iequals(__LINE__, *pBuc == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBuc, 0);
	if (*pBuc == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBuc) complain(__LINE__);
	iequals(__LINE__, *pBuc == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBuc, 0);
	if (*pBuc == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBuc) complain(__LINE__);
	iequals(__LINE__, *pBuc == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBuc, 0);
	if (*pBuc == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBuc) complain(__LINE__);
	iequals(__LINE__, *pBuc == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBuc, 0);
	if (*pBuc == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBuc) complain(__LINE__);
	iequals(__LINE__, *pBuc == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBuc, 0);
	if (*pBuc == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBuc) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBuc == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBuc, 0);
	if (*pBuc == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBuc) complain(__LINE__);
#endif
	iequals(__LINE__, *pBus == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBus, 0);
	if (*pBus == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBus) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBus == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBus, 0);
	if (*pBus == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBus) complain(__LINE__);
#endif
	iequals(__LINE__, *pBus == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBus, 0);
	if (*pBus == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBus) complain(__LINE__);
	iequals(__LINE__, *pBus == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBus, 0);
	if (*pBus == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBus) complain(__LINE__);
	iequals(__LINE__, *pBus == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBus, 0);
	if (*pBus == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBus) complain(__LINE__);
	iequals(__LINE__, *pBus == Ius[x_5], 1);
	iequals(__LINE__, Ius[x_5] == *pBus, 1);
	if (*pBus == Ius[x_5]); else complain(__LINE__);
	if (Ius[x_5] == *pBus); else complain(__LINE__);
	iequals(__LINE__, *pBus == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBus, 0);
	if (*pBus == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBus) complain(__LINE__);
	iequals(__LINE__, *pBus == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBus, 0);
	if (*pBus == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBus) complain(__LINE__);
	iequals(__LINE__, *pBus == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBus, 0);
	if (*pBus == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBus) complain(__LINE__);
	iequals(__LINE__, *pBus == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBus, 0);
	if (*pBus == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBus) complain(__LINE__);
	iequals(__LINE__, *pBus == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBus, 0);
	if (*pBus == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBus) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBus == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBus, 0);
	if (*pBus == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBus) complain(__LINE__);
#endif
	iequals(__LINE__, *pBui == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBui, 0);
	if (*pBui == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBui) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBui == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBui, 0);
	if (*pBui == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBui) complain(__LINE__);
#endif
	iequals(__LINE__, *pBui == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBui, 0);
	if (*pBui == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBui) complain(__LINE__);
	iequals(__LINE__, *pBui == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBui, 0);
	if (*pBui == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBui) complain(__LINE__);
	iequals(__LINE__, *pBui == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBui, 0);
	if (*pBui == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBui) complain(__LINE__);
	iequals(__LINE__, *pBui == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBui, 0);
	if (*pBui == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBui) complain(__LINE__);
	iequals(__LINE__, *pBui == Iui[x_6], 1);
	iequals(__LINE__, Iui[x_6] == *pBui, 1);
	if (*pBui == Iui[x_6]); else complain(__LINE__);
	if (Iui[x_6] == *pBui); else complain(__LINE__);
	iequals(__LINE__, *pBui == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBui, 0);
	if (*pBui == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBui) complain(__LINE__);
	iequals(__LINE__, *pBui == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBui, 0);
	if (*pBui == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBui) complain(__LINE__);
	iequals(__LINE__, *pBui == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBui, 0);
	if (*pBui == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBui) complain(__LINE__);
	iequals(__LINE__, *pBui == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBui, 0);
	if (*pBui == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBui) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBui == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBui, 0);
	if (*pBui == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBui) complain(__LINE__);
#endif
	iequals(__LINE__, *pBl == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBl, 0);
	if (*pBl == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBl) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBl == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBl, 0);
	if (*pBl == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBl) complain(__LINE__);
#endif
	iequals(__LINE__, *pBl == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBl, 0);
	if (*pBl == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBl) complain(__LINE__);
	iequals(__LINE__, *pBl == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBl, 0);
	if (*pBl == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBl) complain(__LINE__);
	iequals(__LINE__, *pBl == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBl, 0);
	if (*pBl == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBl) complain(__LINE__);
	iequals(__LINE__, *pBl == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBl, 0);
	if (*pBl == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBl) complain(__LINE__);
	iequals(__LINE__, *pBl == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBl, 0);
	if (*pBl == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBl) complain(__LINE__);
	iequals(__LINE__, *pBl == Il[x_7], 1);
	iequals(__LINE__, Il[x_7] == *pBl, 1);
	if (*pBl == Il[x_7]); else complain(__LINE__);
	if (Il[x_7] == *pBl); else complain(__LINE__);
	iequals(__LINE__, *pBl == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBl, 0);
	if (*pBl == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBl) complain(__LINE__);
	iequals(__LINE__, *pBl == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBl, 0);
	if (*pBl == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBl) complain(__LINE__);
	iequals(__LINE__, *pBl == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBl, 0);
	if (*pBl == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBl) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBl == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBl, 0);
	if (*pBl == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBl) complain(__LINE__);
#endif
	iequals(__LINE__, *pBul == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBul, 0);
	if (*pBul == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBul) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBul == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBul, 0);
	if (*pBul == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBul) complain(__LINE__);
#endif
	iequals(__LINE__, *pBul == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBul, 0);
	if (*pBul == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBul) complain(__LINE__);
	iequals(__LINE__, *pBul == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBul, 0);
	if (*pBul == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBul) complain(__LINE__);
	iequals(__LINE__, *pBul == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBul, 0);
	if (*pBul == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBul) complain(__LINE__);
	iequals(__LINE__, *pBul == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBul, 0);
	if (*pBul == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBul) complain(__LINE__);
	iequals(__LINE__, *pBul == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBul, 0);
	if (*pBul == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBul) complain(__LINE__);
	iequals(__LINE__, *pBul == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBul, 0);
	if (*pBul == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBul) complain(__LINE__);
	iequals(__LINE__, *pBul == Iul[x_8], 1);
	iequals(__LINE__, Iul[x_8] == *pBul, 1);
	if (*pBul == Iul[x_8]); else complain(__LINE__);
	if (Iul[x_8] == *pBul); else complain(__LINE__);
	iequals(__LINE__, *pBul == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBul, 0);
	if (*pBul == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBul) complain(__LINE__);
	iequals(__LINE__, *pBul == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBul, 0);
	if (*pBul == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBul) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBul == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBul, 0);
	if (*pBul == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBul) complain(__LINE__);
#endif
	iequals(__LINE__, *pBf == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBf, 0);
	if (*pBf == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBf) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBf == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBf, 0);
	if (*pBf == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBf) complain(__LINE__);
#endif
	iequals(__LINE__, *pBf == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBf, 0);
	if (*pBf == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBf) complain(__LINE__);
	iequals(__LINE__, *pBf == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBf, 0);
	if (*pBf == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBf) complain(__LINE__);
	iequals(__LINE__, *pBf == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBf, 0);
	if (*pBf == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBf) complain(__LINE__);
	iequals(__LINE__, *pBf == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBf, 0);
	if (*pBf == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBf) complain(__LINE__);
	iequals(__LINE__, *pBf == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBf, 0);
	if (*pBf == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBf) complain(__LINE__);
	iequals(__LINE__, *pBf == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBf, 0);
	if (*pBf == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBf) complain(__LINE__);
	iequals(__LINE__, *pBf == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBf, 0);
	if (*pBf == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBf) complain(__LINE__);
	iequals(__LINE__, *pBf == If[x_9], 1);
	iequals(__LINE__, If[x_9] == *pBf, 1);
	if (*pBf == If[x_9]); else complain(__LINE__);
	if (If[x_9] == *pBf); else complain(__LINE__);
	iequals(__LINE__, *pBf == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBf, 0);
	if (*pBf == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBf) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBf == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBf, 0);
	if (*pBf == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBf) complain(__LINE__);
#endif
	iequals(__LINE__, *pBd == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBd, 0);
	if (*pBd == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBd) complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBd == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBd, 0);
	if (*pBd == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBd) complain(__LINE__);
#endif
	iequals(__LINE__, *pBd == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBd, 0);
	if (*pBd == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBd) complain(__LINE__);
	iequals(__LINE__, *pBd == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBd, 0);
	if (*pBd == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBd) complain(__LINE__);
	iequals(__LINE__, *pBd == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBd, 0);
	if (*pBd == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBd) complain(__LINE__);
	iequals(__LINE__, *pBd == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBd, 0);
	if (*pBd == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBd) complain(__LINE__);
	iequals(__LINE__, *pBd == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBd, 0);
	if (*pBd == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBd) complain(__LINE__);
	iequals(__LINE__, *pBd == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBd, 0);
	if (*pBd == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBd) complain(__LINE__);
	iequals(__LINE__, *pBd == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBd, 0);
	if (*pBd == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBd) complain(__LINE__);
	iequals(__LINE__, *pBd == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBd, 0);
	if (*pBd == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBd) complain(__LINE__);
	iequals(__LINE__, *pBd == Id[x_10], 1);
	iequals(__LINE__, Id[x_10] == *pBd, 1);
	if (*pBd == Id[x_10]); else complain(__LINE__);
	if (Id[x_10] == *pBd); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, *pBd == Ild[x_11], 0);
	iequals(__LINE__, Ild[x_11] == *pBd, 0);
	if (*pBd == Ild[x_11]) complain(__LINE__);
	if (Ild[x_11] == *pBd) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Ic[x_0], 0);
	iequals(__LINE__, Ic[x_0] == *pBld, 0);
	if (*pBld == Ic[x_0]) complain(__LINE__);
	if (Ic[x_0] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Isc[x_1], 0);
	iequals(__LINE__, Isc[x_1] == *pBld, 0);
	if (*pBld == Isc[x_1]) complain(__LINE__);
	if (Isc[x_1] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Is[x_2], 0);
	iequals(__LINE__, Is[x_2] == *pBld, 0);
	if (*pBld == Is[x_2]) complain(__LINE__);
	if (Is[x_2] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Ii[x_3], 0);
	iequals(__LINE__, Ii[x_3] == *pBld, 0);
	if (*pBld == Ii[x_3]) complain(__LINE__);
	if (Ii[x_3] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Iuc[x_4], 0);
	iequals(__LINE__, Iuc[x_4] == *pBld, 0);
	if (*pBld == Iuc[x_4]) complain(__LINE__);
	if (Iuc[x_4] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Ius[x_5], 0);
	iequals(__LINE__, Ius[x_5] == *pBld, 0);
	if (*pBld == Ius[x_5]) complain(__LINE__);
	if (Ius[x_5] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Iui[x_6], 0);
	iequals(__LINE__, Iui[x_6] == *pBld, 0);
	if (*pBld == Iui[x_6]) complain(__LINE__);
	if (Iui[x_6] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Il[x_7], 0);
	iequals(__LINE__, Il[x_7] == *pBld, 0);
	if (*pBld == Il[x_7]) complain(__LINE__);
	if (Il[x_7] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Iul[x_8], 0);
	iequals(__LINE__, Iul[x_8] == *pBld, 0);
	if (*pBld == Iul[x_8]) complain(__LINE__);
	if (Iul[x_8] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == If[x_9], 0);
	iequals(__LINE__, If[x_9] == *pBld, 0);
	if (*pBld == If[x_9]) complain(__LINE__);
	if (If[x_9] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Id[x_10], 0);
	iequals(__LINE__, Id[x_10] == *pBld, 0);
	if (*pBld == Id[x_10]) complain(__LINE__);
	if (Id[x_10] == *pBld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, *pBld == Ild[x_11], 1);
	iequals(__LINE__, Ild[x_11] == *pBld, 1);
	if (*pBld == Ild[x_11]); else complain(__LINE__);
	if (Ild[x_11] == *pBld); else complain(__LINE__);
#endif
	}}
	return 	report(Filename);
	}
