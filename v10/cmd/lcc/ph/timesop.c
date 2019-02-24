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
	auto CHAR Cc = 7;
	auto CHAR *pCc = &Cc;
	auto CHAR **ppCc = &pCc;
#if ANSI
	auto SCHAR Csc = 8;
	auto SCHAR *pCsc = &Csc;
	auto SCHAR **ppCsc = &pCsc;
#endif
	auto SHORT Cs = 9;
	auto SHORT *pCs = &Cs;
	auto SHORT **ppCs = &pCs;
	auto INT Ci = 10;
	auto INT *pCi = &Ci;
	auto INT **ppCi = &pCi;
	auto UCHAR Cuc = 11;
	auto UCHAR *pCuc = &Cuc;
	auto UCHAR **ppCuc = &pCuc;
	auto USHORT Cus = 12;
	auto USHORT *pCus = &Cus;
	auto USHORT **ppCus = &pCus;
	auto UINT Cui = 13;
	auto UINT *pCui = &Cui;
	auto UINT **ppCui = &pCui;
	auto LONG Cl = 14;
	auto LONG *pCl = &Cl;
	auto LONG **ppCl = &pCl;
	auto ULONG Cul = 15;
	auto ULONG *pCul = &Cul;
	auto ULONG **ppCul = &pCul;
	auto FLOAT Cf = 16;
	auto FLOAT *pCf = &Cf;
	auto FLOAT **ppCf = &pCf;
	auto DOUBLE Cd = 17;
	auto DOUBLE *pCd = &Cd;
	auto DOUBLE **ppCd = &pCd;
#if ANSI
	auto LDOUBLE Cld = 18;
	auto LDOUBLE *pCld = &Cld;
	auto LDOUBLE **ppCld = &pCld;
#endif
	{
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
	int true = 1, false = 0;
	Filename =  " auto pscalar2 auto union times ";
	iequals(__LINE__, **ppCc * D1.c, 49);
	iequals(__LINE__, D1.c * **ppCc, 49);
#if ANSI
	iequals(__LINE__, **ppCc * D2.sc, 56);
	iequals(__LINE__, D2.sc * **ppCc, 56);
#endif
	iequals(__LINE__, **ppCc * D3.s, 63);
	iequals(__LINE__, D3.s * **ppCc, 63);
	iequals(__LINE__, **ppCc * D4.i, 70);
	iequals(__LINE__, D4.i * **ppCc, 70);
	iequals(__LINE__, **ppCc * D5.uc, 77);
	iequals(__LINE__, D5.uc * **ppCc, 77);
	iequals(__LINE__, **ppCc * D6.us, 84);
	iequals(__LINE__, D6.us * **ppCc, 84);
	iequals(__LINE__, **ppCc * D7.ui, 91);
	iequals(__LINE__, D7.ui * **ppCc, 91);
	lequals(__LINE__, **ppCc * D8.l, 98L);
	lequals(__LINE__, D8.l * **ppCc, 98L);
	lequals(__LINE__, **ppCc * D9.ul, 105L);
	lequals(__LINE__, D9.ul * **ppCc, 105L);
	dequals(__LINE__, **ppCc * D10.f, 112.);
	dequals(__LINE__, D10.f * **ppCc, 112.);
	dequals(__LINE__, **ppCc * D11.d, 119.);
	dequals(__LINE__, D11.d * **ppCc, 119.);
#if ANSI
	ldequals(__LINE__, **ppCc * D12.ld, 126.L);
	ldequals(__LINE__, D12.ld * **ppCc, 126.L);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc * D1.c, 56);
	iequals(__LINE__, D1.c * **ppCsc, 56);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc * D2.sc, 64);
	iequals(__LINE__, D2.sc * **ppCsc, 64);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc * D3.s, 72);
	iequals(__LINE__, D3.s * **ppCsc, 72);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc * D4.i, 80);
	iequals(__LINE__, D4.i * **ppCsc, 80);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc * D5.uc, 88);
	iequals(__LINE__, D5.uc * **ppCsc, 88);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc * D6.us, 96);
	iequals(__LINE__, D6.us * **ppCsc, 96);
#endif
#if ANSI
	iequals(__LINE__, **ppCsc * D7.ui, 104);
	iequals(__LINE__, D7.ui * **ppCsc, 104);
#endif
#if ANSI
	lequals(__LINE__, **ppCsc * D8.l, 112L);
	lequals(__LINE__, D8.l * **ppCsc, 112L);
#endif
#if ANSI
	lequals(__LINE__, **ppCsc * D9.ul, 120L);
	lequals(__LINE__, D9.ul * **ppCsc, 120L);
#endif
#if ANSI
	dequals(__LINE__, **ppCsc * D10.f, 128.);
	dequals(__LINE__, D10.f * **ppCsc, 128.);
#endif
#if ANSI
	dequals(__LINE__, **ppCsc * D11.d, 136.);
	dequals(__LINE__, D11.d * **ppCsc, 136.);
#endif
#if ANSI
	ldequals(__LINE__, **ppCsc * D12.ld, 144.L);
	ldequals(__LINE__, D12.ld * **ppCsc, 144.L);
#endif
	iequals(__LINE__, **ppCs * D1.c, 63);
	iequals(__LINE__, D1.c * **ppCs, 63);
#if ANSI
	iequals(__LINE__, **ppCs * D2.sc, 72);
	iequals(__LINE__, D2.sc * **ppCs, 72);
#endif
	iequals(__LINE__, **ppCs * D3.s, 81);
	iequals(__LINE__, D3.s * **ppCs, 81);
	iequals(__LINE__, **ppCs * D4.i, 90);
	iequals(__LINE__, D4.i * **ppCs, 90);
	iequals(__LINE__, **ppCs * D5.uc, 99);
	iequals(__LINE__, D5.uc * **ppCs, 99);
	iequals(__LINE__, **ppCs * D6.us, 108);
	iequals(__LINE__, D6.us * **ppCs, 108);
	iequals(__LINE__, **ppCs * D7.ui, 117);
	iequals(__LINE__, D7.ui * **ppCs, 117);
	lequals(__LINE__, **ppCs * D8.l, 126L);
	lequals(__LINE__, D8.l * **ppCs, 126L);
	lequals(__LINE__, **ppCs * D9.ul, 135L);
	lequals(__LINE__, D9.ul * **ppCs, 135L);
	dequals(__LINE__, **ppCs * D10.f, 144.);
	dequals(__LINE__, D10.f * **ppCs, 144.);
	dequals(__LINE__, **ppCs * D11.d, 153.);
	dequals(__LINE__, D11.d * **ppCs, 153.);
#if ANSI
	ldequals(__LINE__, **ppCs * D12.ld, 162.L);
	ldequals(__LINE__, D12.ld * **ppCs, 162.L);
#endif
	iequals(__LINE__, **ppCi * D1.c, 70);
	iequals(__LINE__, D1.c * **ppCi, 70);
#if ANSI
	iequals(__LINE__, **ppCi * D2.sc, 80);
	iequals(__LINE__, D2.sc * **ppCi, 80);
#endif
	iequals(__LINE__, **ppCi * D3.s, 90);
	iequals(__LINE__, D3.s * **ppCi, 90);
	iequals(__LINE__, **ppCi * D4.i, 100);
	iequals(__LINE__, D4.i * **ppCi, 100);
	iequals(__LINE__, **ppCi * D5.uc, 110);
	iequals(__LINE__, D5.uc * **ppCi, 110);
	iequals(__LINE__, **ppCi * D6.us, 120);
	iequals(__LINE__, D6.us * **ppCi, 120);
	iequals(__LINE__, **ppCi * D7.ui, 130);
	iequals(__LINE__, D7.ui * **ppCi, 130);
	lequals(__LINE__, **ppCi * D8.l, 140L);
	lequals(__LINE__, D8.l * **ppCi, 140L);
	lequals(__LINE__, **ppCi * D9.ul, 150L);
	lequals(__LINE__, D9.ul * **ppCi, 150L);
	dequals(__LINE__, **ppCi * D10.f, 160.);
	dequals(__LINE__, D10.f * **ppCi, 160.);
	dequals(__LINE__, **ppCi * D11.d, 170.);
	dequals(__LINE__, D11.d * **ppCi, 170.);
#if ANSI
	ldequals(__LINE__, **ppCi * D12.ld, 180.L);
	ldequals(__LINE__, D12.ld * **ppCi, 180.L);
#endif
	iequals(__LINE__, **ppCuc * D1.c, 77);
	iequals(__LINE__, D1.c * **ppCuc, 77);
#if ANSI
	iequals(__LINE__, **ppCuc * D2.sc, 88);
	iequals(__LINE__, D2.sc * **ppCuc, 88);
#endif
	iequals(__LINE__, **ppCuc * D3.s, 99);
	iequals(__LINE__, D3.s * **ppCuc, 99);
	iequals(__LINE__, **ppCuc * D4.i, 110);
	iequals(__LINE__, D4.i * **ppCuc, 110);
	iequals(__LINE__, **ppCuc * D5.uc, 121);
	iequals(__LINE__, D5.uc * **ppCuc, 121);
	iequals(__LINE__, **ppCuc * D6.us, 132);
	iequals(__LINE__, D6.us * **ppCuc, 132);
	iequals(__LINE__, **ppCuc * D7.ui, 143);
	iequals(__LINE__, D7.ui * **ppCuc, 143);
	lequals(__LINE__, **ppCuc * D8.l, 154L);
	lequals(__LINE__, D8.l * **ppCuc, 154L);
	lequals(__LINE__, **ppCuc * D9.ul, 165L);
	lequals(__LINE__, D9.ul * **ppCuc, 165L);
	dequals(__LINE__, **ppCuc * D10.f, 176.);
	dequals(__LINE__, D10.f * **ppCuc, 176.);
	dequals(__LINE__, **ppCuc * D11.d, 187.);
	dequals(__LINE__, D11.d * **ppCuc, 187.);
#if ANSI
	ldequals(__LINE__, **ppCuc * D12.ld, 198.L);
	ldequals(__LINE__, D12.ld * **ppCuc, 198.L);
#endif
	iequals(__LINE__, **ppCus * D1.c, 84);
	iequals(__LINE__, D1.c * **ppCus, 84);
#if ANSI
	iequals(__LINE__, **ppCus * D2.sc, 96);
	iequals(__LINE__, D2.sc * **ppCus, 96);
#endif
	iequals(__LINE__, **ppCus * D3.s, 108);
	iequals(__LINE__, D3.s * **ppCus, 108);
	iequals(__LINE__, **ppCus * D4.i, 120);
	iequals(__LINE__, D4.i * **ppCus, 120);
	iequals(__LINE__, **ppCus * D5.uc, 132);
	iequals(__LINE__, D5.uc * **ppCus, 132);
	iequals(__LINE__, **ppCus * D6.us, 144);
	iequals(__LINE__, D6.us * **ppCus, 144);
	iequals(__LINE__, **ppCus * D7.ui, 156);
	iequals(__LINE__, D7.ui * **ppCus, 156);
	lequals(__LINE__, **ppCus * D8.l, 168L);
	lequals(__LINE__, D8.l * **ppCus, 168L);
	lequals(__LINE__, **ppCus * D9.ul, 180L);
	lequals(__LINE__, D9.ul * **ppCus, 180L);
	dequals(__LINE__, **ppCus * D10.f, 192.);
	dequals(__LINE__, D10.f * **ppCus, 192.);
	dequals(__LINE__, **ppCus * D11.d, 204.);
	dequals(__LINE__, D11.d * **ppCus, 204.);
#if ANSI
	ldequals(__LINE__, **ppCus * D12.ld, 216.L);
	ldequals(__LINE__, D12.ld * **ppCus, 216.L);
#endif
	iequals(__LINE__, **ppCui * D1.c, 91);
	iequals(__LINE__, D1.c * **ppCui, 91);
#if ANSI
	iequals(__LINE__, **ppCui * D2.sc, 104);
	iequals(__LINE__, D2.sc * **ppCui, 104);
#endif
	iequals(__LINE__, **ppCui * D3.s, 117);
	iequals(__LINE__, D3.s * **ppCui, 117);
	iequals(__LINE__, **ppCui * D4.i, 130);
	iequals(__LINE__, D4.i * **ppCui, 130);
	iequals(__LINE__, **ppCui * D5.uc, 143);
	iequals(__LINE__, D5.uc * **ppCui, 143);
	iequals(__LINE__, **ppCui * D6.us, 156);
	iequals(__LINE__, D6.us * **ppCui, 156);
	iequals(__LINE__, **ppCui * D7.ui, 169);
	iequals(__LINE__, D7.ui * **ppCui, 169);
	lequals(__LINE__, **ppCui * D8.l, 182L);
	lequals(__LINE__, D8.l * **ppCui, 182L);
	lequals(__LINE__, **ppCui * D9.ul, 195L);
	lequals(__LINE__, D9.ul * **ppCui, 195L);
	dequals(__LINE__, **ppCui * D10.f, 208.);
	dequals(__LINE__, D10.f * **ppCui, 208.);
	dequals(__LINE__, **ppCui * D11.d, 221.);
	dequals(__LINE__, D11.d * **ppCui, 221.);
#if ANSI
	ldequals(__LINE__, **ppCui * D12.ld, 234.L);
	ldequals(__LINE__, D12.ld * **ppCui, 234.L);
#endif
	lequals(__LINE__, **ppCl * D1.c, 98L);
	lequals(__LINE__, D1.c * **ppCl, 98L);
#if ANSI
	lequals(__LINE__, **ppCl * D2.sc, 112L);
	lequals(__LINE__, D2.sc * **ppCl, 112L);
#endif
	lequals(__LINE__, **ppCl * D3.s, 126L);
	lequals(__LINE__, D3.s * **ppCl, 126L);
	lequals(__LINE__, **ppCl * D4.i, 140L);
	lequals(__LINE__, D4.i * **ppCl, 140L);
	lequals(__LINE__, **ppCl * D5.uc, 154L);
	lequals(__LINE__, D5.uc * **ppCl, 154L);
	lequals(__LINE__, **ppCl * D6.us, 168L);
	lequals(__LINE__, D6.us * **ppCl, 168L);
	lequals(__LINE__, **ppCl * D7.ui, 182L);
	lequals(__LINE__, D7.ui * **ppCl, 182L);
	lequals(__LINE__, **ppCl * D8.l, 196L);
	lequals(__LINE__, D8.l * **ppCl, 196L);
	lequals(__LINE__, **ppCl * D9.ul, 210L);
	lequals(__LINE__, D9.ul * **ppCl, 210L);
	dequals(__LINE__, **ppCl * D10.f, 224.);
	dequals(__LINE__, D10.f * **ppCl, 224.);
	dequals(__LINE__, **ppCl * D11.d, 238.);
	dequals(__LINE__, D11.d * **ppCl, 238.);
#if ANSI
	ldequals(__LINE__, **ppCl * D12.ld, 252.L);
	ldequals(__LINE__, D12.ld * **ppCl, 252.L);
#endif
	lequals(__LINE__, **ppCul * D1.c, 105L);
	lequals(__LINE__, D1.c * **ppCul, 105L);
#if ANSI
	lequals(__LINE__, **ppCul * D2.sc, 120L);
	lequals(__LINE__, D2.sc * **ppCul, 120L);
#endif
	lequals(__LINE__, **ppCul * D3.s, 135L);
	lequals(__LINE__, D3.s * **ppCul, 135L);
	lequals(__LINE__, **ppCul * D4.i, 150L);
	lequals(__LINE__, D4.i * **ppCul, 150L);
	lequals(__LINE__, **ppCul * D5.uc, 165L);
	lequals(__LINE__, D5.uc * **ppCul, 165L);
	lequals(__LINE__, **ppCul * D6.us, 180L);
	lequals(__LINE__, D6.us * **ppCul, 180L);
	lequals(__LINE__, **ppCul * D7.ui, 195L);
	lequals(__LINE__, D7.ui * **ppCul, 195L);
	lequals(__LINE__, **ppCul * D8.l, 210L);
	lequals(__LINE__, D8.l * **ppCul, 210L);
	lequals(__LINE__, **ppCul * D9.ul, 225L);
	lequals(__LINE__, D9.ul * **ppCul, 225L);
	dequals(__LINE__, **ppCul * D10.f, 240.);
	dequals(__LINE__, D10.f * **ppCul, 240.);
	dequals(__LINE__, **ppCul * D11.d, 255.);
	dequals(__LINE__, D11.d * **ppCul, 255.);
#if ANSI
	ldequals(__LINE__, **ppCul * D12.ld, 270.L);
	ldequals(__LINE__, D12.ld * **ppCul, 270.L);
#endif
	dequals(__LINE__, **ppCf * D1.c, 112.);
	dequals(__LINE__, D1.c * **ppCf, 112.);
#if ANSI
	dequals(__LINE__, **ppCf * D2.sc, 128.);
	dequals(__LINE__, D2.sc * **ppCf, 128.);
#endif
	dequals(__LINE__, **ppCf * D3.s, 144.);
	dequals(__LINE__, D3.s * **ppCf, 144.);
	dequals(__LINE__, **ppCf * D4.i, 160.);
	dequals(__LINE__, D4.i * **ppCf, 160.);
	dequals(__LINE__, **ppCf * D5.uc, 176.);
	dequals(__LINE__, D5.uc * **ppCf, 176.);
	dequals(__LINE__, **ppCf * D6.us, 192.);
	dequals(__LINE__, D6.us * **ppCf, 192.);
	dequals(__LINE__, **ppCf * D7.ui, 208.);
	dequals(__LINE__, D7.ui * **ppCf, 208.);
	dequals(__LINE__, **ppCf * D8.l, 224.);
	dequals(__LINE__, D8.l * **ppCf, 224.);
	dequals(__LINE__, **ppCf * D9.ul, 240.);
	dequals(__LINE__, D9.ul * **ppCf, 240.);
	dequals(__LINE__, **ppCf * D10.f, 256.);
	dequals(__LINE__, D10.f * **ppCf, 256.);
	dequals(__LINE__, **ppCf * D11.d, 272.);
	dequals(__LINE__, D11.d * **ppCf, 272.);
#if ANSI
	ldequals(__LINE__, **ppCf * D12.ld, 288.L);
	ldequals(__LINE__, D12.ld * **ppCf, 288.L);
#endif
	dequals(__LINE__, **ppCd * D1.c, 119.);
	dequals(__LINE__, D1.c * **ppCd, 119.);
#if ANSI
	dequals(__LINE__, **ppCd * D2.sc, 136.);
	dequals(__LINE__, D2.sc * **ppCd, 136.);
#endif
	dequals(__LINE__, **ppCd * D3.s, 153.);
	dequals(__LINE__, D3.s * **ppCd, 153.);
	dequals(__LINE__, **ppCd * D4.i, 170.);
	dequals(__LINE__, D4.i * **ppCd, 170.);
	dequals(__LINE__, **ppCd * D5.uc, 187.);
	dequals(__LINE__, D5.uc * **ppCd, 187.);
	dequals(__LINE__, **ppCd * D6.us, 204.);
	dequals(__LINE__, D6.us * **ppCd, 204.);
	dequals(__LINE__, **ppCd * D7.ui, 221.);
	dequals(__LINE__, D7.ui * **ppCd, 221.);
	dequals(__LINE__, **ppCd * D8.l, 238.);
	dequals(__LINE__, D8.l * **ppCd, 238.);
	dequals(__LINE__, **ppCd * D9.ul, 255.);
	dequals(__LINE__, D9.ul * **ppCd, 255.);
	dequals(__LINE__, **ppCd * D10.f, 272.);
	dequals(__LINE__, D10.f * **ppCd, 272.);
	dequals(__LINE__, **ppCd * D11.d, 289.);
	dequals(__LINE__, D11.d * **ppCd, 289.);
#if ANSI
	ldequals(__LINE__, **ppCd * D12.ld, 306.L);
	ldequals(__LINE__, D12.ld * **ppCd, 306.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D1.c, 126.L);
	ldequals(__LINE__, D1.c * **ppCld, 126.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D2.sc, 144.L);
	ldequals(__LINE__, D2.sc * **ppCld, 144.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D3.s, 162.L);
	ldequals(__LINE__, D3.s * **ppCld, 162.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D4.i, 180.L);
	ldequals(__LINE__, D4.i * **ppCld, 180.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D5.uc, 198.L);
	ldequals(__LINE__, D5.uc * **ppCld, 198.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D6.us, 216.L);
	ldequals(__LINE__, D6.us * **ppCld, 216.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D7.ui, 234.L);
	ldequals(__LINE__, D7.ui * **ppCld, 234.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D8.l, 252.L);
	ldequals(__LINE__, D8.l * **ppCld, 252.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D9.ul, 270.L);
	ldequals(__LINE__, D9.ul * **ppCld, 270.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D10.f, 288.L);
	ldequals(__LINE__, D10.f * **ppCld, 288.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D11.d, 306.L);
	ldequals(__LINE__, D11.d * **ppCld, 306.L);
#endif
#if ANSI
	ldequals(__LINE__, **ppCld * D12.ld, 324.L);
	ldequals(__LINE__, D12.ld * **ppCld, 324.L);
#endif
	}}
	return 	report(Filename);
	}
