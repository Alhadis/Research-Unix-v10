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
	Filename =  " auto union auto arrarr not ";
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D1.c, !7);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#endif
#if ANSI
	iequals(__LINE__, ! D2.sc, !8);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D3.s, !9);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D4.i, !10);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D5.uc, !11);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D6.us, !12);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D7.ui, !13);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D8.l, !14);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D9.ul, !15);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D10.f, !16);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#if ANSI
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#if ANSI
	iequals(__LINE__, ! D11.d, !17);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Pc[x[x_0]], !7);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Psc[x[x_1]], !8);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Ps[x[x_2]], !9);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Pi[x[x_3]], !10);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Puc[x[x_4]], !11);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Pus[x[x_5]], !12);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Pui[x[x_6]], !13);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Pl[x[x_7]], !14);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Pul[x[x_8]], !15);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Pf[x[x_9]], !16);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Pd[x[x_10]], !17);
#endif
#if ANSI
	iequals(__LINE__, ! D12.ld, !18);
	iequals(__LINE__, ! Pld[x[x_11]], !18);
#endif
	}}
	return 	report(Filename);
	}
