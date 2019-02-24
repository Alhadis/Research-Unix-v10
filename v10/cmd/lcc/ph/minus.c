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
	auto struct {
		UINT b1: 1;
		UINT b2: 2;
		UINT b3: 3;
		UINT b4: 4;
		UINT b5: 5;
		UINT b6: 6;
		UINT b7: 7;
		UINT b8: 5;
		UINT b9: 4;
		UINT b10: 3;
		UINT b11: 2;
		UINT b12: 1;
		} K;
	K.b1 = 1;
	K.b2 = 2;
	K.b3 = 3;
	K.b4 = 4;
	K.b5 = 5;
	K.b6 = 6;
	K.b7 = 7;
	K.b8 = 5;
	K.b9 = 4;
	K.b10 = 3;
	K.b11 = 2;
	K.b12 = 1;
	{
	int true = 1, false = 0;
	Filename =  " auto pscalar1 auto bits minus ";
	iequals(__LINE__, *pBc - K.b1, 6);
	iequals(__LINE__, K.b1 - *pBc, -6);
	iequals(__LINE__, *pBc - K.b2, 5);
	iequals(__LINE__, K.b2 - *pBc, -5);
	iequals(__LINE__, *pBc - K.b3, 4);
	iequals(__LINE__, K.b3 - *pBc, -4);
	iequals(__LINE__, *pBc - K.b4, 3);
	iequals(__LINE__, K.b4 - *pBc, -3);
	iequals(__LINE__, *pBc - K.b5, 2);
	iequals(__LINE__, K.b5 - *pBc, -2);
	iequals(__LINE__, *pBc - K.b6, 1);
	iequals(__LINE__, K.b6 - *pBc, -1);
	iequals(__LINE__, *pBc - K.b7, 0);
	iequals(__LINE__, K.b7 - *pBc, 0);
	iequals(__LINE__, *pBc - K.b8, 2);
	iequals(__LINE__, K.b8 - *pBc, -2);
	iequals(__LINE__, *pBc - K.b9, 3);
	iequals(__LINE__, K.b9 - *pBc, -3);
	iequals(__LINE__, *pBc - K.b10, 4);
	iequals(__LINE__, K.b10 - *pBc, -4);
	iequals(__LINE__, *pBc - K.b11, 5);
	iequals(__LINE__, K.b11 - *pBc, -5);
	iequals(__LINE__, *pBc - K.b12, 6);
	iequals(__LINE__, K.b12 - *pBc, -6);
#if ANSI
	iequals(__LINE__, *pBsc - K.b1, 7);
	iequals(__LINE__, K.b1 - *pBsc, -7);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b2, 6);
	iequals(__LINE__, K.b2 - *pBsc, -6);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b3, 5);
	iequals(__LINE__, K.b3 - *pBsc, -5);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b4, 4);
	iequals(__LINE__, K.b4 - *pBsc, -4);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b5, 3);
	iequals(__LINE__, K.b5 - *pBsc, -3);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b6, 2);
	iequals(__LINE__, K.b6 - *pBsc, -2);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b7, 1);
	iequals(__LINE__, K.b7 - *pBsc, -1);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b8, 3);
	iequals(__LINE__, K.b8 - *pBsc, -3);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b9, 4);
	iequals(__LINE__, K.b9 - *pBsc, -4);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b10, 5);
	iequals(__LINE__, K.b10 - *pBsc, -5);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b11, 6);
	iequals(__LINE__, K.b11 - *pBsc, -6);
#endif
#if ANSI
	iequals(__LINE__, *pBsc - K.b12, 7);
	iequals(__LINE__, K.b12 - *pBsc, -7);
#endif
	iequals(__LINE__, *pBs - K.b1, 8);
	iequals(__LINE__, K.b1 - *pBs, -8);
	iequals(__LINE__, *pBs - K.b2, 7);
	iequals(__LINE__, K.b2 - *pBs, -7);
	iequals(__LINE__, *pBs - K.b3, 6);
	iequals(__LINE__, K.b3 - *pBs, -6);
	iequals(__LINE__, *pBs - K.b4, 5);
	iequals(__LINE__, K.b4 - *pBs, -5);
	iequals(__LINE__, *pBs - K.b5, 4);
	iequals(__LINE__, K.b5 - *pBs, -4);
	iequals(__LINE__, *pBs - K.b6, 3);
	iequals(__LINE__, K.b6 - *pBs, -3);
	iequals(__LINE__, *pBs - K.b7, 2);
	iequals(__LINE__, K.b7 - *pBs, -2);
	iequals(__LINE__, *pBs - K.b8, 4);
	iequals(__LINE__, K.b8 - *pBs, -4);
	iequals(__LINE__, *pBs - K.b9, 5);
	iequals(__LINE__, K.b9 - *pBs, -5);
	iequals(__LINE__, *pBs - K.b10, 6);
	iequals(__LINE__, K.b10 - *pBs, -6);
	iequals(__LINE__, *pBs - K.b11, 7);
	iequals(__LINE__, K.b11 - *pBs, -7);
	iequals(__LINE__, *pBs - K.b12, 8);
	iequals(__LINE__, K.b12 - *pBs, -8);
	iequals(__LINE__, *pBi - K.b1, 9);
	iequals(__LINE__, K.b1 - *pBi, -9);
	iequals(__LINE__, *pBi - K.b2, 8);
	iequals(__LINE__, K.b2 - *pBi, -8);
	iequals(__LINE__, *pBi - K.b3, 7);
	iequals(__LINE__, K.b3 - *pBi, -7);
	iequals(__LINE__, *pBi - K.b4, 6);
	iequals(__LINE__, K.b4 - *pBi, -6);
	iequals(__LINE__, *pBi - K.b5, 5);
	iequals(__LINE__, K.b5 - *pBi, -5);
	iequals(__LINE__, *pBi - K.b6, 4);
	iequals(__LINE__, K.b6 - *pBi, -4);
	iequals(__LINE__, *pBi - K.b7, 3);
	iequals(__LINE__, K.b7 - *pBi, -3);
	iequals(__LINE__, *pBi - K.b8, 5);
	iequals(__LINE__, K.b8 - *pBi, -5);
	iequals(__LINE__, *pBi - K.b9, 6);
	iequals(__LINE__, K.b9 - *pBi, -6);
	iequals(__LINE__, *pBi - K.b10, 7);
	iequals(__LINE__, K.b10 - *pBi, -7);
	iequals(__LINE__, *pBi - K.b11, 8);
	iequals(__LINE__, K.b11 - *pBi, -8);
	iequals(__LINE__, *pBi - K.b12, 9);
	iequals(__LINE__, K.b12 - *pBi, -9);
	iequals(__LINE__, (int)*pBuc - K.b1, 10);
	iequals(__LINE__, K.b1 - (int)*pBuc, -10);
	iequals(__LINE__, (int)*pBuc - K.b2, 9);
	iequals(__LINE__, K.b2 - (int)*pBuc, -9);
	iequals(__LINE__, (int)*pBuc - K.b3, 8);
	iequals(__LINE__, K.b3 - (int)*pBuc, -8);
	iequals(__LINE__, (int)*pBuc - K.b4, 7);
	iequals(__LINE__, K.b4 - (int)*pBuc, -7);
	iequals(__LINE__, (int)*pBuc - K.b5, 6);
	iequals(__LINE__, K.b5 - (int)*pBuc, -6);
	iequals(__LINE__, (int)*pBuc - K.b6, 5);
	iequals(__LINE__, K.b6 - (int)*pBuc, -5);
	iequals(__LINE__, (int)*pBuc - K.b7, 4);
	iequals(__LINE__, K.b7 - (int)*pBuc, -4);
	iequals(__LINE__, (int)*pBuc - K.b8, 6);
	iequals(__LINE__, K.b8 - (int)*pBuc, -6);
	iequals(__LINE__, (int)*pBuc - K.b9, 7);
	iequals(__LINE__, K.b9 - (int)*pBuc, -7);
	iequals(__LINE__, (int)*pBuc - K.b10, 8);
	iequals(__LINE__, K.b10 - (int)*pBuc, -8);
	iequals(__LINE__, (int)*pBuc - K.b11, 9);
	iequals(__LINE__, K.b11 - (int)*pBuc, -9);
	iequals(__LINE__, (int)*pBuc - K.b12, 10);
	iequals(__LINE__, K.b12 - (int)*pBuc, -10);
	iequals(__LINE__, (int)*pBus - K.b1, 11);
	iequals(__LINE__, K.b1 - (int)*pBus, -11);
	iequals(__LINE__, (int)*pBus - K.b2, 10);
	iequals(__LINE__, K.b2 - (int)*pBus, -10);
	iequals(__LINE__, (int)*pBus - K.b3, 9);
	iequals(__LINE__, K.b3 - (int)*pBus, -9);
	iequals(__LINE__, (int)*pBus - K.b4, 8);
	iequals(__LINE__, K.b4 - (int)*pBus, -8);
	iequals(__LINE__, (int)*pBus - K.b5, 7);
	iequals(__LINE__, K.b5 - (int)*pBus, -7);
	iequals(__LINE__, (int)*pBus - K.b6, 6);
	iequals(__LINE__, K.b6 - (int)*pBus, -6);
	iequals(__LINE__, (int)*pBus - K.b7, 5);
	iequals(__LINE__, K.b7 - (int)*pBus, -5);
	iequals(__LINE__, (int)*pBus - K.b8, 7);
	iequals(__LINE__, K.b8 - (int)*pBus, -7);
	iequals(__LINE__, (int)*pBus - K.b9, 8);
	iequals(__LINE__, K.b9 - (int)*pBus, -8);
	iequals(__LINE__, (int)*pBus - K.b10, 9);
	iequals(__LINE__, K.b10 - (int)*pBus, -9);
	iequals(__LINE__, (int)*pBus - K.b11, 10);
	iequals(__LINE__, K.b11 - (int)*pBus, -10);
	iequals(__LINE__, (int)*pBus - K.b12, 11);
	iequals(__LINE__, K.b12 - (int)*pBus, -11);
	iequals(__LINE__, (int)*pBui - K.b1, 12);
	iequals(__LINE__, K.b1 - (int)*pBui, -12);
	iequals(__LINE__, (int)*pBui - K.b2, 11);
	iequals(__LINE__, K.b2 - (int)*pBui, -11);
	iequals(__LINE__, (int)*pBui - K.b3, 10);
	iequals(__LINE__, K.b3 - (int)*pBui, -10);
	iequals(__LINE__, (int)*pBui - K.b4, 9);
	iequals(__LINE__, K.b4 - (int)*pBui, -9);
	iequals(__LINE__, (int)*pBui - K.b5, 8);
	iequals(__LINE__, K.b5 - (int)*pBui, -8);
	iequals(__LINE__, (int)*pBui - K.b6, 7);
	iequals(__LINE__, K.b6 - (int)*pBui, -7);
	iequals(__LINE__, (int)*pBui - K.b7, 6);
	iequals(__LINE__, K.b7 - (int)*pBui, -6);
	iequals(__LINE__, (int)*pBui - K.b8, 8);
	iequals(__LINE__, K.b8 - (int)*pBui, -8);
	iequals(__LINE__, (int)*pBui - K.b9, 9);
	iequals(__LINE__, K.b9 - (int)*pBui, -9);
	iequals(__LINE__, (int)*pBui - K.b10, 10);
	iequals(__LINE__, K.b10 - (int)*pBui, -10);
	iequals(__LINE__, (int)*pBui - K.b11, 11);
	iequals(__LINE__, K.b11 - (int)*pBui, -11);
	iequals(__LINE__, (int)*pBui - K.b12, 12);
	iequals(__LINE__, K.b12 - (int)*pBui, -12);
	lequals(__LINE__, *pBl - K.b1, 13L);
	lequals(__LINE__, K.b1 - *pBl, -13L);
	lequals(__LINE__, *pBl - K.b2, 12L);
	lequals(__LINE__, K.b2 - *pBl, -12L);
	lequals(__LINE__, *pBl - K.b3, 11L);
	lequals(__LINE__, K.b3 - *pBl, -11L);
	lequals(__LINE__, *pBl - K.b4, 10L);
	lequals(__LINE__, K.b4 - *pBl, -10L);
	lequals(__LINE__, *pBl - K.b5, 9L);
	lequals(__LINE__, K.b5 - *pBl, -9L);
	lequals(__LINE__, *pBl - K.b6, 8L);
	lequals(__LINE__, K.b6 - *pBl, -8L);
	lequals(__LINE__, *pBl - K.b7, 7L);
	lequals(__LINE__, K.b7 - *pBl, -7L);
	lequals(__LINE__, *pBl - K.b8, 9L);
	lequals(__LINE__, K.b8 - *pBl, -9L);
	lequals(__LINE__, *pBl - K.b9, 10L);
	lequals(__LINE__, K.b9 - *pBl, -10L);
	lequals(__LINE__, *pBl - K.b10, 11L);
	lequals(__LINE__, K.b10 - *pBl, -11L);
	lequals(__LINE__, *pBl - K.b11, 12L);
	lequals(__LINE__, K.b11 - *pBl, -12L);
	lequals(__LINE__, *pBl - K.b12, 13L);
	lequals(__LINE__, K.b12 - *pBl, -13L);
	lequals(__LINE__, (long)*pBul - K.b1, 14L);
	lequals(__LINE__, K.b1 - (long)*pBul, -14L);
	lequals(__LINE__, (long)*pBul - K.b2, 13L);
	lequals(__LINE__, K.b2 - (long)*pBul, -13L);
	lequals(__LINE__, (long)*pBul - K.b3, 12L);
	lequals(__LINE__, K.b3 - (long)*pBul, -12L);
	lequals(__LINE__, (long)*pBul - K.b4, 11L);
	lequals(__LINE__, K.b4 - (long)*pBul, -11L);
	lequals(__LINE__, (long)*pBul - K.b5, 10L);
	lequals(__LINE__, K.b5 - (long)*pBul, -10L);
	lequals(__LINE__, (long)*pBul - K.b6, 9L);
	lequals(__LINE__, K.b6 - (long)*pBul, -9L);
	lequals(__LINE__, (long)*pBul - K.b7, 8L);
	lequals(__LINE__, K.b7 - (long)*pBul, -8L);
	lequals(__LINE__, (long)*pBul - K.b8, 10L);
	lequals(__LINE__, K.b8 - (long)*pBul, -10L);
	lequals(__LINE__, (long)*pBul - K.b9, 11L);
	lequals(__LINE__, K.b9 - (long)*pBul, -11L);
	lequals(__LINE__, (long)*pBul - K.b10, 12L);
	lequals(__LINE__, K.b10 - (long)*pBul, -12L);
	lequals(__LINE__, (long)*pBul - K.b11, 13L);
	lequals(__LINE__, K.b11 - (long)*pBul, -13L);
	lequals(__LINE__, (long)*pBul - K.b12, 14L);
	lequals(__LINE__, K.b12 - (long)*pBul, -14L);
	dequals(__LINE__, *pBf - K.b1, 15.);
	dequals(__LINE__, K.b1 - *pBf, -15.);
	dequals(__LINE__, *pBf - K.b2, 14.);
	dequals(__LINE__, K.b2 - *pBf, -14.);
	dequals(__LINE__, *pBf - K.b3, 13.);
	dequals(__LINE__, K.b3 - *pBf, -13.);
	dequals(__LINE__, *pBf - K.b4, 12.);
	dequals(__LINE__, K.b4 - *pBf, -12.);
	dequals(__LINE__, *pBf - K.b5, 11.);
	dequals(__LINE__, K.b5 - *pBf, -11.);
	dequals(__LINE__, *pBf - K.b6, 10.);
	dequals(__LINE__, K.b6 - *pBf, -10.);
	dequals(__LINE__, *pBf - K.b7, 9.);
	dequals(__LINE__, K.b7 - *pBf, -9.);
	dequals(__LINE__, *pBf - K.b8, 11.);
	dequals(__LINE__, K.b8 - *pBf, -11.);
	dequals(__LINE__, *pBf - K.b9, 12.);
	dequals(__LINE__, K.b9 - *pBf, -12.);
	dequals(__LINE__, *pBf - K.b10, 13.);
	dequals(__LINE__, K.b10 - *pBf, -13.);
	dequals(__LINE__, *pBf - K.b11, 14.);
	dequals(__LINE__, K.b11 - *pBf, -14.);
	dequals(__LINE__, *pBf - K.b12, 15.);
	dequals(__LINE__, K.b12 - *pBf, -15.);
	dequals(__LINE__, *pBd - K.b1, 16.);
	dequals(__LINE__, K.b1 - *pBd, -16.);
	dequals(__LINE__, *pBd - K.b2, 15.);
	dequals(__LINE__, K.b2 - *pBd, -15.);
	dequals(__LINE__, *pBd - K.b3, 14.);
	dequals(__LINE__, K.b3 - *pBd, -14.);
	dequals(__LINE__, *pBd - K.b4, 13.);
	dequals(__LINE__, K.b4 - *pBd, -13.);
	dequals(__LINE__, *pBd - K.b5, 12.);
	dequals(__LINE__, K.b5 - *pBd, -12.);
	dequals(__LINE__, *pBd - K.b6, 11.);
	dequals(__LINE__, K.b6 - *pBd, -11.);
	dequals(__LINE__, *pBd - K.b7, 10.);
	dequals(__LINE__, K.b7 - *pBd, -10.);
	dequals(__LINE__, *pBd - K.b8, 12.);
	dequals(__LINE__, K.b8 - *pBd, -12.);
	dequals(__LINE__, *pBd - K.b9, 13.);
	dequals(__LINE__, K.b9 - *pBd, -13.);
	dequals(__LINE__, *pBd - K.b10, 14.);
	dequals(__LINE__, K.b10 - *pBd, -14.);
	dequals(__LINE__, *pBd - K.b11, 15.);
	dequals(__LINE__, K.b11 - *pBd, -15.);
	dequals(__LINE__, *pBd - K.b12, 16.);
	dequals(__LINE__, K.b12 - *pBd, -16.);
#if ANSI
	ldequals(__LINE__, *pBld - K.b1, 17.L);
	ldequals(__LINE__, K.b1 - *pBld, -17.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b2, 16.L);
	ldequals(__LINE__, K.b2 - *pBld, -16.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b3, 15.L);
	ldequals(__LINE__, K.b3 - *pBld, -15.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b4, 14.L);
	ldequals(__LINE__, K.b4 - *pBld, -14.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b5, 13.L);
	ldequals(__LINE__, K.b5 - *pBld, -13.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b6, 12.L);
	ldequals(__LINE__, K.b6 - *pBld, -12.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b7, 11.L);
	ldequals(__LINE__, K.b7 - *pBld, -11.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b8, 13.L);
	ldequals(__LINE__, K.b8 - *pBld, -13.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b9, 14.L);
	ldequals(__LINE__, K.b9 - *pBld, -14.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b10, 15.L);
	ldequals(__LINE__, K.b10 - *pBld, -15.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b11, 16.L);
	ldequals(__LINE__, K.b11 - *pBld, -16.L);
#endif
#if ANSI
	ldequals(__LINE__, *pBld - K.b12, 17.L);
	ldequals(__LINE__, K.b12 - *pBld, -17.L);
#endif
	}}
	return 	report(Filename);
	}
