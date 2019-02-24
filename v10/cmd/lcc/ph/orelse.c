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
		} L, *pL = &L;
	pL->b1 = 1;
	pL->b2 = 2;
	pL->b3 = 3;
	pL->b4 = 4;
	pL->b5 = 5;
	pL->b6 = 6;
	pL->b7 = 7;
	pL->b8 = 5;
	pL->b9 = 4;
	pL->b10 = 3;
	pL->b11 = 2;
	pL->b12 = 1;
	{
	int true = 1, false = 0;
	Filename =  " auto scalar auto pbits orelse ";
	iequals(__LINE__, Ac || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Ac, 1);
	if (Ac || pL->b1); else complain(__LINE__);
	if (pL->b1 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Ac, 1);
	if (Ac || pL->b2); else complain(__LINE__);
	if (pL->b2 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Ac, 1);
	if (Ac || pL->b3); else complain(__LINE__);
	if (pL->b3 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Ac, 1);
	if (Ac || pL->b4); else complain(__LINE__);
	if (pL->b4 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Ac, 1);
	if (Ac || pL->b5); else complain(__LINE__);
	if (pL->b5 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Ac, 1);
	if (Ac || pL->b6); else complain(__LINE__);
	if (pL->b6 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Ac, 1);
	if (Ac || pL->b7); else complain(__LINE__);
	if (pL->b7 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Ac, 1);
	if (Ac || pL->b8); else complain(__LINE__);
	if (pL->b8 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Ac, 1);
	if (Ac || pL->b9); else complain(__LINE__);
	if (pL->b9 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Ac, 1);
	if (Ac || pL->b10); else complain(__LINE__);
	if (pL->b10 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Ac, 1);
	if (Ac || pL->b11); else complain(__LINE__);
	if (pL->b11 || Ac); else complain(__LINE__);
	iequals(__LINE__, Ac || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Ac, 1);
	if (Ac || pL->b12); else complain(__LINE__);
	if (pL->b12 || Ac); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, Asc || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Asc, 1);
	if (Asc || pL->b1); else complain(__LINE__);
	if (pL->b1 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Asc, 1);
	if (Asc || pL->b2); else complain(__LINE__);
	if (pL->b2 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Asc, 1);
	if (Asc || pL->b3); else complain(__LINE__);
	if (pL->b3 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Asc, 1);
	if (Asc || pL->b4); else complain(__LINE__);
	if (pL->b4 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Asc, 1);
	if (Asc || pL->b5); else complain(__LINE__);
	if (pL->b5 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Asc, 1);
	if (Asc || pL->b6); else complain(__LINE__);
	if (pL->b6 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Asc, 1);
	if (Asc || pL->b7); else complain(__LINE__);
	if (pL->b7 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Asc, 1);
	if (Asc || pL->b8); else complain(__LINE__);
	if (pL->b8 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Asc, 1);
	if (Asc || pL->b9); else complain(__LINE__);
	if (pL->b9 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Asc, 1);
	if (Asc || pL->b10); else complain(__LINE__);
	if (pL->b10 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Asc, 1);
	if (Asc || pL->b11); else complain(__LINE__);
	if (pL->b11 || Asc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Asc || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Asc, 1);
	if (Asc || pL->b12); else complain(__LINE__);
	if (pL->b12 || Asc); else complain(__LINE__);
#endif
	iequals(__LINE__, As || pL->b1, 1);
	iequals(__LINE__, pL->b1 || As, 1);
	if (As || pL->b1); else complain(__LINE__);
	if (pL->b1 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b2, 1);
	iequals(__LINE__, pL->b2 || As, 1);
	if (As || pL->b2); else complain(__LINE__);
	if (pL->b2 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b3, 1);
	iequals(__LINE__, pL->b3 || As, 1);
	if (As || pL->b3); else complain(__LINE__);
	if (pL->b3 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b4, 1);
	iequals(__LINE__, pL->b4 || As, 1);
	if (As || pL->b4); else complain(__LINE__);
	if (pL->b4 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b5, 1);
	iequals(__LINE__, pL->b5 || As, 1);
	if (As || pL->b5); else complain(__LINE__);
	if (pL->b5 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b6, 1);
	iequals(__LINE__, pL->b6 || As, 1);
	if (As || pL->b6); else complain(__LINE__);
	if (pL->b6 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b7, 1);
	iequals(__LINE__, pL->b7 || As, 1);
	if (As || pL->b7); else complain(__LINE__);
	if (pL->b7 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b8, 1);
	iequals(__LINE__, pL->b8 || As, 1);
	if (As || pL->b8); else complain(__LINE__);
	if (pL->b8 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b9, 1);
	iequals(__LINE__, pL->b9 || As, 1);
	if (As || pL->b9); else complain(__LINE__);
	if (pL->b9 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b10, 1);
	iequals(__LINE__, pL->b10 || As, 1);
	if (As || pL->b10); else complain(__LINE__);
	if (pL->b10 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b11, 1);
	iequals(__LINE__, pL->b11 || As, 1);
	if (As || pL->b11); else complain(__LINE__);
	if (pL->b11 || As); else complain(__LINE__);
	iequals(__LINE__, As || pL->b12, 1);
	iequals(__LINE__, pL->b12 || As, 1);
	if (As || pL->b12); else complain(__LINE__);
	if (pL->b12 || As); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Ai, 1);
	if (Ai || pL->b1); else complain(__LINE__);
	if (pL->b1 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Ai, 1);
	if (Ai || pL->b2); else complain(__LINE__);
	if (pL->b2 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Ai, 1);
	if (Ai || pL->b3); else complain(__LINE__);
	if (pL->b3 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Ai, 1);
	if (Ai || pL->b4); else complain(__LINE__);
	if (pL->b4 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Ai, 1);
	if (Ai || pL->b5); else complain(__LINE__);
	if (pL->b5 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Ai, 1);
	if (Ai || pL->b6); else complain(__LINE__);
	if (pL->b6 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Ai, 1);
	if (Ai || pL->b7); else complain(__LINE__);
	if (pL->b7 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Ai, 1);
	if (Ai || pL->b8); else complain(__LINE__);
	if (pL->b8 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Ai, 1);
	if (Ai || pL->b9); else complain(__LINE__);
	if (pL->b9 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Ai, 1);
	if (Ai || pL->b10); else complain(__LINE__);
	if (pL->b10 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Ai, 1);
	if (Ai || pL->b11); else complain(__LINE__);
	if (pL->b11 || Ai); else complain(__LINE__);
	iequals(__LINE__, Ai || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Ai, 1);
	if (Ai || pL->b12); else complain(__LINE__);
	if (pL->b12 || Ai); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Auc, 1);
	if (Auc || pL->b1); else complain(__LINE__);
	if (pL->b1 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Auc, 1);
	if (Auc || pL->b2); else complain(__LINE__);
	if (pL->b2 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Auc, 1);
	if (Auc || pL->b3); else complain(__LINE__);
	if (pL->b3 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Auc, 1);
	if (Auc || pL->b4); else complain(__LINE__);
	if (pL->b4 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Auc, 1);
	if (Auc || pL->b5); else complain(__LINE__);
	if (pL->b5 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Auc, 1);
	if (Auc || pL->b6); else complain(__LINE__);
	if (pL->b6 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Auc, 1);
	if (Auc || pL->b7); else complain(__LINE__);
	if (pL->b7 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Auc, 1);
	if (Auc || pL->b8); else complain(__LINE__);
	if (pL->b8 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Auc, 1);
	if (Auc || pL->b9); else complain(__LINE__);
	if (pL->b9 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Auc, 1);
	if (Auc || pL->b10); else complain(__LINE__);
	if (pL->b10 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Auc, 1);
	if (Auc || pL->b11); else complain(__LINE__);
	if (pL->b11 || Auc); else complain(__LINE__);
	iequals(__LINE__, Auc || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Auc, 1);
	if (Auc || pL->b12); else complain(__LINE__);
	if (pL->b12 || Auc); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Aus, 1);
	if (Aus || pL->b1); else complain(__LINE__);
	if (pL->b1 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Aus, 1);
	if (Aus || pL->b2); else complain(__LINE__);
	if (pL->b2 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Aus, 1);
	if (Aus || pL->b3); else complain(__LINE__);
	if (pL->b3 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Aus, 1);
	if (Aus || pL->b4); else complain(__LINE__);
	if (pL->b4 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Aus, 1);
	if (Aus || pL->b5); else complain(__LINE__);
	if (pL->b5 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Aus, 1);
	if (Aus || pL->b6); else complain(__LINE__);
	if (pL->b6 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Aus, 1);
	if (Aus || pL->b7); else complain(__LINE__);
	if (pL->b7 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Aus, 1);
	if (Aus || pL->b8); else complain(__LINE__);
	if (pL->b8 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Aus, 1);
	if (Aus || pL->b9); else complain(__LINE__);
	if (pL->b9 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Aus, 1);
	if (Aus || pL->b10); else complain(__LINE__);
	if (pL->b10 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Aus, 1);
	if (Aus || pL->b11); else complain(__LINE__);
	if (pL->b11 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aus || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Aus, 1);
	if (Aus || pL->b12); else complain(__LINE__);
	if (pL->b12 || Aus); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Aui, 1);
	if (Aui || pL->b1); else complain(__LINE__);
	if (pL->b1 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Aui, 1);
	if (Aui || pL->b2); else complain(__LINE__);
	if (pL->b2 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Aui, 1);
	if (Aui || pL->b3); else complain(__LINE__);
	if (pL->b3 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Aui, 1);
	if (Aui || pL->b4); else complain(__LINE__);
	if (pL->b4 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Aui, 1);
	if (Aui || pL->b5); else complain(__LINE__);
	if (pL->b5 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Aui, 1);
	if (Aui || pL->b6); else complain(__LINE__);
	if (pL->b6 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Aui, 1);
	if (Aui || pL->b7); else complain(__LINE__);
	if (pL->b7 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Aui, 1);
	if (Aui || pL->b8); else complain(__LINE__);
	if (pL->b8 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Aui, 1);
	if (Aui || pL->b9); else complain(__LINE__);
	if (pL->b9 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Aui, 1);
	if (Aui || pL->b10); else complain(__LINE__);
	if (pL->b10 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Aui, 1);
	if (Aui || pL->b11); else complain(__LINE__);
	if (pL->b11 || Aui); else complain(__LINE__);
	iequals(__LINE__, Aui || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Aui, 1);
	if (Aui || pL->b12); else complain(__LINE__);
	if (pL->b12 || Aui); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Al, 1);
	if (Al || pL->b1); else complain(__LINE__);
	if (pL->b1 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Al, 1);
	if (Al || pL->b2); else complain(__LINE__);
	if (pL->b2 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Al, 1);
	if (Al || pL->b3); else complain(__LINE__);
	if (pL->b3 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Al, 1);
	if (Al || pL->b4); else complain(__LINE__);
	if (pL->b4 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Al, 1);
	if (Al || pL->b5); else complain(__LINE__);
	if (pL->b5 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Al, 1);
	if (Al || pL->b6); else complain(__LINE__);
	if (pL->b6 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Al, 1);
	if (Al || pL->b7); else complain(__LINE__);
	if (pL->b7 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Al, 1);
	if (Al || pL->b8); else complain(__LINE__);
	if (pL->b8 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Al, 1);
	if (Al || pL->b9); else complain(__LINE__);
	if (pL->b9 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Al, 1);
	if (Al || pL->b10); else complain(__LINE__);
	if (pL->b10 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Al, 1);
	if (Al || pL->b11); else complain(__LINE__);
	if (pL->b11 || Al); else complain(__LINE__);
	iequals(__LINE__, Al || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Al, 1);
	if (Al || pL->b12); else complain(__LINE__);
	if (pL->b12 || Al); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Aul, 1);
	if (Aul || pL->b1); else complain(__LINE__);
	if (pL->b1 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Aul, 1);
	if (Aul || pL->b2); else complain(__LINE__);
	if (pL->b2 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Aul, 1);
	if (Aul || pL->b3); else complain(__LINE__);
	if (pL->b3 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Aul, 1);
	if (Aul || pL->b4); else complain(__LINE__);
	if (pL->b4 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Aul, 1);
	if (Aul || pL->b5); else complain(__LINE__);
	if (pL->b5 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Aul, 1);
	if (Aul || pL->b6); else complain(__LINE__);
	if (pL->b6 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Aul, 1);
	if (Aul || pL->b7); else complain(__LINE__);
	if (pL->b7 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Aul, 1);
	if (Aul || pL->b8); else complain(__LINE__);
	if (pL->b8 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Aul, 1);
	if (Aul || pL->b9); else complain(__LINE__);
	if (pL->b9 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Aul, 1);
	if (Aul || pL->b10); else complain(__LINE__);
	if (pL->b10 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Aul, 1);
	if (Aul || pL->b11); else complain(__LINE__);
	if (pL->b11 || Aul); else complain(__LINE__);
	iequals(__LINE__, Aul || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Aul, 1);
	if (Aul || pL->b12); else complain(__LINE__);
	if (pL->b12 || Aul); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Af, 1);
	if (Af || pL->b1); else complain(__LINE__);
	if (pL->b1 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Af, 1);
	if (Af || pL->b2); else complain(__LINE__);
	if (pL->b2 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Af, 1);
	if (Af || pL->b3); else complain(__LINE__);
	if (pL->b3 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Af, 1);
	if (Af || pL->b4); else complain(__LINE__);
	if (pL->b4 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Af, 1);
	if (Af || pL->b5); else complain(__LINE__);
	if (pL->b5 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Af, 1);
	if (Af || pL->b6); else complain(__LINE__);
	if (pL->b6 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Af, 1);
	if (Af || pL->b7); else complain(__LINE__);
	if (pL->b7 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Af, 1);
	if (Af || pL->b8); else complain(__LINE__);
	if (pL->b8 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Af, 1);
	if (Af || pL->b9); else complain(__LINE__);
	if (pL->b9 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Af, 1);
	if (Af || pL->b10); else complain(__LINE__);
	if (pL->b10 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Af, 1);
	if (Af || pL->b11); else complain(__LINE__);
	if (pL->b11 || Af); else complain(__LINE__);
	iequals(__LINE__, Af || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Af, 1);
	if (Af || pL->b12); else complain(__LINE__);
	if (pL->b12 || Af); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Ad, 1);
	if (Ad || pL->b1); else complain(__LINE__);
	if (pL->b1 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Ad, 1);
	if (Ad || pL->b2); else complain(__LINE__);
	if (pL->b2 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Ad, 1);
	if (Ad || pL->b3); else complain(__LINE__);
	if (pL->b3 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Ad, 1);
	if (Ad || pL->b4); else complain(__LINE__);
	if (pL->b4 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Ad, 1);
	if (Ad || pL->b5); else complain(__LINE__);
	if (pL->b5 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Ad, 1);
	if (Ad || pL->b6); else complain(__LINE__);
	if (pL->b6 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Ad, 1);
	if (Ad || pL->b7); else complain(__LINE__);
	if (pL->b7 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Ad, 1);
	if (Ad || pL->b8); else complain(__LINE__);
	if (pL->b8 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Ad, 1);
	if (Ad || pL->b9); else complain(__LINE__);
	if (pL->b9 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Ad, 1);
	if (Ad || pL->b10); else complain(__LINE__);
	if (pL->b10 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Ad, 1);
	if (Ad || pL->b11); else complain(__LINE__);
	if (pL->b11 || Ad); else complain(__LINE__);
	iequals(__LINE__, Ad || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Ad, 1);
	if (Ad || pL->b12); else complain(__LINE__);
	if (pL->b12 || Ad); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, Ald || pL->b1, 1);
	iequals(__LINE__, pL->b1 || Ald, 1);
	if (Ald || pL->b1); else complain(__LINE__);
	if (pL->b1 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b2, 1);
	iequals(__LINE__, pL->b2 || Ald, 1);
	if (Ald || pL->b2); else complain(__LINE__);
	if (pL->b2 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b3, 1);
	iequals(__LINE__, pL->b3 || Ald, 1);
	if (Ald || pL->b3); else complain(__LINE__);
	if (pL->b3 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b4, 1);
	iequals(__LINE__, pL->b4 || Ald, 1);
	if (Ald || pL->b4); else complain(__LINE__);
	if (pL->b4 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b5, 1);
	iequals(__LINE__, pL->b5 || Ald, 1);
	if (Ald || pL->b5); else complain(__LINE__);
	if (pL->b5 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b6, 1);
	iequals(__LINE__, pL->b6 || Ald, 1);
	if (Ald || pL->b6); else complain(__LINE__);
	if (pL->b6 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b7, 1);
	iequals(__LINE__, pL->b7 || Ald, 1);
	if (Ald || pL->b7); else complain(__LINE__);
	if (pL->b7 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b8, 1);
	iequals(__LINE__, pL->b8 || Ald, 1);
	if (Ald || pL->b8); else complain(__LINE__);
	if (pL->b8 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b9, 1);
	iequals(__LINE__, pL->b9 || Ald, 1);
	if (Ald || pL->b9); else complain(__LINE__);
	if (pL->b9 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b10, 1);
	iequals(__LINE__, pL->b10 || Ald, 1);
	if (Ald || pL->b10); else complain(__LINE__);
	if (pL->b10 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b11, 1);
	iequals(__LINE__, pL->b11 || Ald, 1);
	if (Ald || pL->b11); else complain(__LINE__);
	if (pL->b11 || Ald); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, Ald || pL->b12, 1);
	iequals(__LINE__, pL->b12 || Ald, 1);
	if (Ald || pL->b12); else complain(__LINE__);
	if (pL->b12 || Ald); else complain(__LINE__);
#endif
	}}
	return 	report(Filename);
	}
