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
		} F;
	F.c = 7;
#if ANSI
	F.sc = 8;
#endif
	F.s = 9;
	F.i = 10;
	F.uc = 11;
	F.us = 12;
	F.ui = 13;
	F.l = 14;
	F.ul = 15;
	F.f = 16;
	F.d = 17;
#if ANSI
	F.ld = 18;
#endif
	{
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
	int true = 1, false = 0;
	Filename =  " auto struct auto scalar le ";
	iequals(__LINE__, F.c <= Ac, 1);
	iequals(__LINE__, Ac <= F.c, 1);
	if (F.c <= Ac); else complain(__LINE__);
	if (Ac <= F.c); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.c <= Asc, 1);
	iequals(__LINE__, Asc <= F.c, 0);
	if (F.c <= Asc); else complain(__LINE__);
	if (Asc <= F.c) complain(__LINE__);
#endif
	iequals(__LINE__, F.c <= As, 1);
	iequals(__LINE__, As <= F.c, 0);
	if (F.c <= As); else complain(__LINE__);
	if (As <= F.c) complain(__LINE__);
	iequals(__LINE__, F.c <= Ai, 1);
	iequals(__LINE__, Ai <= F.c, 0);
	if (F.c <= Ai); else complain(__LINE__);
	if (Ai <= F.c) complain(__LINE__);
	iequals(__LINE__, F.c <= Auc, 1);
	iequals(__LINE__, Auc <= F.c, 0);
	if (F.c <= Auc); else complain(__LINE__);
	if (Auc <= F.c) complain(__LINE__);
	iequals(__LINE__, F.c <= Aus, 1);
	iequals(__LINE__, Aus <= F.c, 0);
	if (F.c <= Aus); else complain(__LINE__);
	if (Aus <= F.c) complain(__LINE__);
	iequals(__LINE__, F.c <= Aui, 1);
	iequals(__LINE__, Aui <= F.c, 0);
	if (F.c <= Aui); else complain(__LINE__);
	if (Aui <= F.c) complain(__LINE__);
	iequals(__LINE__, F.c <= Al, 1);
	iequals(__LINE__, Al <= F.c, 0);
	if (F.c <= Al); else complain(__LINE__);
	if (Al <= F.c) complain(__LINE__);
	iequals(__LINE__, F.c <= Aul, 1);
	iequals(__LINE__, Aul <= F.c, 0);
	if (F.c <= Aul); else complain(__LINE__);
	if (Aul <= F.c) complain(__LINE__);
	iequals(__LINE__, F.c <= Af, 1);
	iequals(__LINE__, Af <= F.c, 0);
	if (F.c <= Af); else complain(__LINE__);
	if (Af <= F.c) complain(__LINE__);
	iequals(__LINE__, F.c <= Ad, 1);
	iequals(__LINE__, Ad <= F.c, 0);
	if (F.c <= Ad); else complain(__LINE__);
	if (Ad <= F.c) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.c <= Ald, 1);
	iequals(__LINE__, Ald <= F.c, 0);
	if (F.c <= Ald); else complain(__LINE__);
	if (Ald <= F.c) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Ac, 0);
	iequals(__LINE__, Ac <= F.sc, 1);
	if (F.sc <= Ac) complain(__LINE__);
	if (Ac <= F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Asc, 1);
	iequals(__LINE__, Asc <= F.sc, 1);
	if (F.sc <= Asc); else complain(__LINE__);
	if (Asc <= F.sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= As, 1);
	iequals(__LINE__, As <= F.sc, 0);
	if (F.sc <= As); else complain(__LINE__);
	if (As <= F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Ai, 1);
	iequals(__LINE__, Ai <= F.sc, 0);
	if (F.sc <= Ai); else complain(__LINE__);
	if (Ai <= F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Auc, 1);
	iequals(__LINE__, Auc <= F.sc, 0);
	if (F.sc <= Auc); else complain(__LINE__);
	if (Auc <= F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Aus, 1);
	iequals(__LINE__, Aus <= F.sc, 0);
	if (F.sc <= Aus); else complain(__LINE__);
	if (Aus <= F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Aui, 1);
	iequals(__LINE__, Aui <= F.sc, 0);
	if (F.sc <= Aui); else complain(__LINE__);
	if (Aui <= F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Al, 1);
	iequals(__LINE__, Al <= F.sc, 0);
	if (F.sc <= Al); else complain(__LINE__);
	if (Al <= F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Aul, 1);
	iequals(__LINE__, Aul <= F.sc, 0);
	if (F.sc <= Aul); else complain(__LINE__);
	if (Aul <= F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Af, 1);
	iequals(__LINE__, Af <= F.sc, 0);
	if (F.sc <= Af); else complain(__LINE__);
	if (Af <= F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Ad, 1);
	iequals(__LINE__, Ad <= F.sc, 0);
	if (F.sc <= Ad); else complain(__LINE__);
	if (Ad <= F.sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.sc <= Ald, 1);
	iequals(__LINE__, Ald <= F.sc, 0);
	if (F.sc <= Ald); else complain(__LINE__);
	if (Ald <= F.sc) complain(__LINE__);
#endif
	iequals(__LINE__, F.s <= Ac, 0);
	iequals(__LINE__, Ac <= F.s, 1);
	if (F.s <= Ac) complain(__LINE__);
	if (Ac <= F.s); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.s <= Asc, 0);
	iequals(__LINE__, Asc <= F.s, 1);
	if (F.s <= Asc) complain(__LINE__);
	if (Asc <= F.s); else complain(__LINE__);
#endif
	iequals(__LINE__, F.s <= As, 1);
	iequals(__LINE__, As <= F.s, 1);
	if (F.s <= As); else complain(__LINE__);
	if (As <= F.s); else complain(__LINE__);
	iequals(__LINE__, F.s <= Ai, 1);
	iequals(__LINE__, Ai <= F.s, 0);
	if (F.s <= Ai); else complain(__LINE__);
	if (Ai <= F.s) complain(__LINE__);
	iequals(__LINE__, F.s <= Auc, 1);
	iequals(__LINE__, Auc <= F.s, 0);
	if (F.s <= Auc); else complain(__LINE__);
	if (Auc <= F.s) complain(__LINE__);
	iequals(__LINE__, F.s <= Aus, 1);
	iequals(__LINE__, Aus <= F.s, 0);
	if (F.s <= Aus); else complain(__LINE__);
	if (Aus <= F.s) complain(__LINE__);
	iequals(__LINE__, F.s <= Aui, 1);
	iequals(__LINE__, Aui <= F.s, 0);
	if (F.s <= Aui); else complain(__LINE__);
	if (Aui <= F.s) complain(__LINE__);
	iequals(__LINE__, F.s <= Al, 1);
	iequals(__LINE__, Al <= F.s, 0);
	if (F.s <= Al); else complain(__LINE__);
	if (Al <= F.s) complain(__LINE__);
	iequals(__LINE__, F.s <= Aul, 1);
	iequals(__LINE__, Aul <= F.s, 0);
	if (F.s <= Aul); else complain(__LINE__);
	if (Aul <= F.s) complain(__LINE__);
	iequals(__LINE__, F.s <= Af, 1);
	iequals(__LINE__, Af <= F.s, 0);
	if (F.s <= Af); else complain(__LINE__);
	if (Af <= F.s) complain(__LINE__);
	iequals(__LINE__, F.s <= Ad, 1);
	iequals(__LINE__, Ad <= F.s, 0);
	if (F.s <= Ad); else complain(__LINE__);
	if (Ad <= F.s) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.s <= Ald, 1);
	iequals(__LINE__, Ald <= F.s, 0);
	if (F.s <= Ald); else complain(__LINE__);
	if (Ald <= F.s) complain(__LINE__);
#endif
	iequals(__LINE__, F.i <= Ac, 0);
	iequals(__LINE__, Ac <= F.i, 1);
	if (F.i <= Ac) complain(__LINE__);
	if (Ac <= F.i); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.i <= Asc, 0);
	iequals(__LINE__, Asc <= F.i, 1);
	if (F.i <= Asc) complain(__LINE__);
	if (Asc <= F.i); else complain(__LINE__);
#endif
	iequals(__LINE__, F.i <= As, 0);
	iequals(__LINE__, As <= F.i, 1);
	if (F.i <= As) complain(__LINE__);
	if (As <= F.i); else complain(__LINE__);
	iequals(__LINE__, F.i <= Ai, 1);
	iequals(__LINE__, Ai <= F.i, 1);
	if (F.i <= Ai); else complain(__LINE__);
	if (Ai <= F.i); else complain(__LINE__);
	iequals(__LINE__, F.i <= Auc, 1);
	iequals(__LINE__, Auc <= F.i, 0);
	if (F.i <= Auc); else complain(__LINE__);
	if (Auc <= F.i) complain(__LINE__);
	iequals(__LINE__, F.i <= Aus, 1);
	iequals(__LINE__, Aus <= F.i, 0);
	if (F.i <= Aus); else complain(__LINE__);
	if (Aus <= F.i) complain(__LINE__);
	iequals(__LINE__, F.i <= Aui, 1);
	iequals(__LINE__, Aui <= F.i, 0);
	if (F.i <= Aui); else complain(__LINE__);
	if (Aui <= F.i) complain(__LINE__);
	iequals(__LINE__, F.i <= Al, 1);
	iequals(__LINE__, Al <= F.i, 0);
	if (F.i <= Al); else complain(__LINE__);
	if (Al <= F.i) complain(__LINE__);
	iequals(__LINE__, F.i <= Aul, 1);
	iequals(__LINE__, Aul <= F.i, 0);
	if (F.i <= Aul); else complain(__LINE__);
	if (Aul <= F.i) complain(__LINE__);
	iequals(__LINE__, F.i <= Af, 1);
	iequals(__LINE__, Af <= F.i, 0);
	if (F.i <= Af); else complain(__LINE__);
	if (Af <= F.i) complain(__LINE__);
	iequals(__LINE__, F.i <= Ad, 1);
	iequals(__LINE__, Ad <= F.i, 0);
	if (F.i <= Ad); else complain(__LINE__);
	if (Ad <= F.i) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.i <= Ald, 1);
	iequals(__LINE__, Ald <= F.i, 0);
	if (F.i <= Ald); else complain(__LINE__);
	if (Ald <= F.i) complain(__LINE__);
#endif
	iequals(__LINE__, F.uc <= Ac, 0);
	iequals(__LINE__, Ac <= F.uc, 1);
	if (F.uc <= Ac) complain(__LINE__);
	if (Ac <= F.uc); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.uc <= Asc, 0);
	iequals(__LINE__, Asc <= F.uc, 1);
	if (F.uc <= Asc) complain(__LINE__);
	if (Asc <= F.uc); else complain(__LINE__);
#endif
	iequals(__LINE__, F.uc <= As, 0);
	iequals(__LINE__, As <= F.uc, 1);
	if (F.uc <= As) complain(__LINE__);
	if (As <= F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc <= Ai, 0);
	iequals(__LINE__, Ai <= F.uc, 1);
	if (F.uc <= Ai) complain(__LINE__);
	if (Ai <= F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc <= Auc, 1);
	iequals(__LINE__, Auc <= F.uc, 1);
	if (F.uc <= Auc); else complain(__LINE__);
	if (Auc <= F.uc); else complain(__LINE__);
	iequals(__LINE__, F.uc <= Aus, 1);
	iequals(__LINE__, Aus <= F.uc, 0);
	if (F.uc <= Aus); else complain(__LINE__);
	if (Aus <= F.uc) complain(__LINE__);
	iequals(__LINE__, F.uc <= Aui, 1);
	iequals(__LINE__, Aui <= F.uc, 0);
	if (F.uc <= Aui); else complain(__LINE__);
	if (Aui <= F.uc) complain(__LINE__);
	iequals(__LINE__, F.uc <= Al, 1);
	iequals(__LINE__, Al <= F.uc, 0);
	if (F.uc <= Al); else complain(__LINE__);
	if (Al <= F.uc) complain(__LINE__);
	iequals(__LINE__, F.uc <= Aul, 1);
	iequals(__LINE__, Aul <= F.uc, 0);
	if (F.uc <= Aul); else complain(__LINE__);
	if (Aul <= F.uc) complain(__LINE__);
	iequals(__LINE__, F.uc <= Af, 1);
	iequals(__LINE__, Af <= F.uc, 0);
	if (F.uc <= Af); else complain(__LINE__);
	if (Af <= F.uc) complain(__LINE__);
	iequals(__LINE__, F.uc <= Ad, 1);
	iequals(__LINE__, Ad <= F.uc, 0);
	if (F.uc <= Ad); else complain(__LINE__);
	if (Ad <= F.uc) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.uc <= Ald, 1);
	iequals(__LINE__, Ald <= F.uc, 0);
	if (F.uc <= Ald); else complain(__LINE__);
	if (Ald <= F.uc) complain(__LINE__);
#endif
	iequals(__LINE__, F.us <= Ac, 0);
	iequals(__LINE__, Ac <= F.us, 1);
	if (F.us <= Ac) complain(__LINE__);
	if (Ac <= F.us); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.us <= Asc, 0);
	iequals(__LINE__, Asc <= F.us, 1);
	if (F.us <= Asc) complain(__LINE__);
	if (Asc <= F.us); else complain(__LINE__);
#endif
	iequals(__LINE__, F.us <= As, 0);
	iequals(__LINE__, As <= F.us, 1);
	if (F.us <= As) complain(__LINE__);
	if (As <= F.us); else complain(__LINE__);
	iequals(__LINE__, F.us <= Ai, 0);
	iequals(__LINE__, Ai <= F.us, 1);
	if (F.us <= Ai) complain(__LINE__);
	if (Ai <= F.us); else complain(__LINE__);
	iequals(__LINE__, F.us <= Auc, 0);
	iequals(__LINE__, Auc <= F.us, 1);
	if (F.us <= Auc) complain(__LINE__);
	if (Auc <= F.us); else complain(__LINE__);
	iequals(__LINE__, F.us <= Aus, 1);
	iequals(__LINE__, Aus <= F.us, 1);
	if (F.us <= Aus); else complain(__LINE__);
	if (Aus <= F.us); else complain(__LINE__);
	iequals(__LINE__, F.us <= Aui, 1);
	iequals(__LINE__, Aui <= F.us, 0);
	if (F.us <= Aui); else complain(__LINE__);
	if (Aui <= F.us) complain(__LINE__);
	iequals(__LINE__, F.us <= Al, 1);
	iequals(__LINE__, Al <= F.us, 0);
	if (F.us <= Al); else complain(__LINE__);
	if (Al <= F.us) complain(__LINE__);
	iequals(__LINE__, F.us <= Aul, 1);
	iequals(__LINE__, Aul <= F.us, 0);
	if (F.us <= Aul); else complain(__LINE__);
	if (Aul <= F.us) complain(__LINE__);
	iequals(__LINE__, F.us <= Af, 1);
	iequals(__LINE__, Af <= F.us, 0);
	if (F.us <= Af); else complain(__LINE__);
	if (Af <= F.us) complain(__LINE__);
	iequals(__LINE__, F.us <= Ad, 1);
	iequals(__LINE__, Ad <= F.us, 0);
	if (F.us <= Ad); else complain(__LINE__);
	if (Ad <= F.us) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.us <= Ald, 1);
	iequals(__LINE__, Ald <= F.us, 0);
	if (F.us <= Ald); else complain(__LINE__);
	if (Ald <= F.us) complain(__LINE__);
#endif
	iequals(__LINE__, F.ui <= Ac, 0);
	iequals(__LINE__, Ac <= F.ui, 1);
	if (F.ui <= Ac) complain(__LINE__);
	if (Ac <= F.ui); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.ui <= Asc, 0);
	iequals(__LINE__, Asc <= F.ui, 1);
	if (F.ui <= Asc) complain(__LINE__);
	if (Asc <= F.ui); else complain(__LINE__);
#endif
	iequals(__LINE__, F.ui <= As, 0);
	iequals(__LINE__, As <= F.ui, 1);
	if (F.ui <= As) complain(__LINE__);
	if (As <= F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui <= Ai, 0);
	iequals(__LINE__, Ai <= F.ui, 1);
	if (F.ui <= Ai) complain(__LINE__);
	if (Ai <= F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui <= Auc, 0);
	iequals(__LINE__, Auc <= F.ui, 1);
	if (F.ui <= Auc) complain(__LINE__);
	if (Auc <= F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui <= Aus, 0);
	iequals(__LINE__, Aus <= F.ui, 1);
	if (F.ui <= Aus) complain(__LINE__);
	if (Aus <= F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui <= Aui, 1);
	iequals(__LINE__, Aui <= F.ui, 1);
	if (F.ui <= Aui); else complain(__LINE__);
	if (Aui <= F.ui); else complain(__LINE__);
	iequals(__LINE__, F.ui <= Al, 1);
	iequals(__LINE__, Al <= F.ui, 0);
	if (F.ui <= Al); else complain(__LINE__);
	if (Al <= F.ui) complain(__LINE__);
	iequals(__LINE__, F.ui <= Aul, 1);
	iequals(__LINE__, Aul <= F.ui, 0);
	if (F.ui <= Aul); else complain(__LINE__);
	if (Aul <= F.ui) complain(__LINE__);
	iequals(__LINE__, F.ui <= Af, 1);
	iequals(__LINE__, Af <= F.ui, 0);
	if (F.ui <= Af); else complain(__LINE__);
	if (Af <= F.ui) complain(__LINE__);
	iequals(__LINE__, F.ui <= Ad, 1);
	iequals(__LINE__, Ad <= F.ui, 0);
	if (F.ui <= Ad); else complain(__LINE__);
	if (Ad <= F.ui) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.ui <= Ald, 1);
	iequals(__LINE__, Ald <= F.ui, 0);
	if (F.ui <= Ald); else complain(__LINE__);
	if (Ald <= F.ui) complain(__LINE__);
#endif
	iequals(__LINE__, F.l <= Ac, 0);
	iequals(__LINE__, Ac <= F.l, 1);
	if (F.l <= Ac) complain(__LINE__);
	if (Ac <= F.l); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.l <= Asc, 0);
	iequals(__LINE__, Asc <= F.l, 1);
	if (F.l <= Asc) complain(__LINE__);
	if (Asc <= F.l); else complain(__LINE__);
#endif
	iequals(__LINE__, F.l <= As, 0);
	iequals(__LINE__, As <= F.l, 1);
	if (F.l <= As) complain(__LINE__);
	if (As <= F.l); else complain(__LINE__);
	iequals(__LINE__, F.l <= Ai, 0);
	iequals(__LINE__, Ai <= F.l, 1);
	if (F.l <= Ai) complain(__LINE__);
	if (Ai <= F.l); else complain(__LINE__);
	iequals(__LINE__, F.l <= Auc, 0);
	iequals(__LINE__, Auc <= F.l, 1);
	if (F.l <= Auc) complain(__LINE__);
	if (Auc <= F.l); else complain(__LINE__);
	iequals(__LINE__, F.l <= Aus, 0);
	iequals(__LINE__, Aus <= F.l, 1);
	if (F.l <= Aus) complain(__LINE__);
	if (Aus <= F.l); else complain(__LINE__);
	iequals(__LINE__, F.l <= Aui, 0);
	iequals(__LINE__, Aui <= F.l, 1);
	if (F.l <= Aui) complain(__LINE__);
	if (Aui <= F.l); else complain(__LINE__);
	iequals(__LINE__, F.l <= Al, 1);
	iequals(__LINE__, Al <= F.l, 1);
	if (F.l <= Al); else complain(__LINE__);
	if (Al <= F.l); else complain(__LINE__);
	iequals(__LINE__, F.l <= Aul, 1);
	iequals(__LINE__, Aul <= F.l, 0);
	if (F.l <= Aul); else complain(__LINE__);
	if (Aul <= F.l) complain(__LINE__);
	iequals(__LINE__, F.l <= Af, 1);
	iequals(__LINE__, Af <= F.l, 0);
	if (F.l <= Af); else complain(__LINE__);
	if (Af <= F.l) complain(__LINE__);
	iequals(__LINE__, F.l <= Ad, 1);
	iequals(__LINE__, Ad <= F.l, 0);
	if (F.l <= Ad); else complain(__LINE__);
	if (Ad <= F.l) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.l <= Ald, 1);
	iequals(__LINE__, Ald <= F.l, 0);
	if (F.l <= Ald); else complain(__LINE__);
	if (Ald <= F.l) complain(__LINE__);
#endif
	iequals(__LINE__, F.ul <= Ac, 0);
	iequals(__LINE__, Ac <= F.ul, 1);
	if (F.ul <= Ac) complain(__LINE__);
	if (Ac <= F.ul); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.ul <= Asc, 0);
	iequals(__LINE__, Asc <= F.ul, 1);
	if (F.ul <= Asc) complain(__LINE__);
	if (Asc <= F.ul); else complain(__LINE__);
#endif
	iequals(__LINE__, F.ul <= As, 0);
	iequals(__LINE__, As <= F.ul, 1);
	if (F.ul <= As) complain(__LINE__);
	if (As <= F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul <= Ai, 0);
	iequals(__LINE__, Ai <= F.ul, 1);
	if (F.ul <= Ai) complain(__LINE__);
	if (Ai <= F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul <= Auc, 0);
	iequals(__LINE__, Auc <= F.ul, 1);
	if (F.ul <= Auc) complain(__LINE__);
	if (Auc <= F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul <= Aus, 0);
	iequals(__LINE__, Aus <= F.ul, 1);
	if (F.ul <= Aus) complain(__LINE__);
	if (Aus <= F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul <= Aui, 0);
	iequals(__LINE__, Aui <= F.ul, 1);
	if (F.ul <= Aui) complain(__LINE__);
	if (Aui <= F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul <= Al, 0);
	iequals(__LINE__, Al <= F.ul, 1);
	if (F.ul <= Al) complain(__LINE__);
	if (Al <= F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul <= Aul, 1);
	iequals(__LINE__, Aul <= F.ul, 1);
	if (F.ul <= Aul); else complain(__LINE__);
	if (Aul <= F.ul); else complain(__LINE__);
	iequals(__LINE__, F.ul <= Af, 1);
	iequals(__LINE__, Af <= F.ul, 0);
	if (F.ul <= Af); else complain(__LINE__);
	if (Af <= F.ul) complain(__LINE__);
	iequals(__LINE__, F.ul <= Ad, 1);
	iequals(__LINE__, Ad <= F.ul, 0);
	if (F.ul <= Ad); else complain(__LINE__);
	if (Ad <= F.ul) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.ul <= Ald, 1);
	iequals(__LINE__, Ald <= F.ul, 0);
	if (F.ul <= Ald); else complain(__LINE__);
	if (Ald <= F.ul) complain(__LINE__);
#endif
	iequals(__LINE__, F.f <= Ac, 0);
	iequals(__LINE__, Ac <= F.f, 1);
	if (F.f <= Ac) complain(__LINE__);
	if (Ac <= F.f); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.f <= Asc, 0);
	iequals(__LINE__, Asc <= F.f, 1);
	if (F.f <= Asc) complain(__LINE__);
	if (Asc <= F.f); else complain(__LINE__);
#endif
	iequals(__LINE__, F.f <= As, 0);
	iequals(__LINE__, As <= F.f, 1);
	if (F.f <= As) complain(__LINE__);
	if (As <= F.f); else complain(__LINE__);
	iequals(__LINE__, F.f <= Ai, 0);
	iequals(__LINE__, Ai <= F.f, 1);
	if (F.f <= Ai) complain(__LINE__);
	if (Ai <= F.f); else complain(__LINE__);
	iequals(__LINE__, F.f <= Auc, 0);
	iequals(__LINE__, Auc <= F.f, 1);
	if (F.f <= Auc) complain(__LINE__);
	if (Auc <= F.f); else complain(__LINE__);
	iequals(__LINE__, F.f <= Aus, 0);
	iequals(__LINE__, Aus <= F.f, 1);
	if (F.f <= Aus) complain(__LINE__);
	if (Aus <= F.f); else complain(__LINE__);
	iequals(__LINE__, F.f <= Aui, 0);
	iequals(__LINE__, Aui <= F.f, 1);
	if (F.f <= Aui) complain(__LINE__);
	if (Aui <= F.f); else complain(__LINE__);
	iequals(__LINE__, F.f <= Al, 0);
	iequals(__LINE__, Al <= F.f, 1);
	if (F.f <= Al) complain(__LINE__);
	if (Al <= F.f); else complain(__LINE__);
	iequals(__LINE__, F.f <= Aul, 0);
	iequals(__LINE__, Aul <= F.f, 1);
	if (F.f <= Aul) complain(__LINE__);
	if (Aul <= F.f); else complain(__LINE__);
	iequals(__LINE__, F.f <= Af, 1);
	iequals(__LINE__, Af <= F.f, 1);
	if (F.f <= Af); else complain(__LINE__);
	if (Af <= F.f); else complain(__LINE__);
	iequals(__LINE__, F.f <= Ad, 1);
	iequals(__LINE__, Ad <= F.f, 0);
	if (F.f <= Ad); else complain(__LINE__);
	if (Ad <= F.f) complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.f <= Ald, 1);
	iequals(__LINE__, Ald <= F.f, 0);
	if (F.f <= Ald); else complain(__LINE__);
	if (Ald <= F.f) complain(__LINE__);
#endif
	iequals(__LINE__, F.d <= Ac, 0);
	iequals(__LINE__, Ac <= F.d, 1);
	if (F.d <= Ac) complain(__LINE__);
	if (Ac <= F.d); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.d <= Asc, 0);
	iequals(__LINE__, Asc <= F.d, 1);
	if (F.d <= Asc) complain(__LINE__);
	if (Asc <= F.d); else complain(__LINE__);
#endif
	iequals(__LINE__, F.d <= As, 0);
	iequals(__LINE__, As <= F.d, 1);
	if (F.d <= As) complain(__LINE__);
	if (As <= F.d); else complain(__LINE__);
	iequals(__LINE__, F.d <= Ai, 0);
	iequals(__LINE__, Ai <= F.d, 1);
	if (F.d <= Ai) complain(__LINE__);
	if (Ai <= F.d); else complain(__LINE__);
	iequals(__LINE__, F.d <= Auc, 0);
	iequals(__LINE__, Auc <= F.d, 1);
	if (F.d <= Auc) complain(__LINE__);
	if (Auc <= F.d); else complain(__LINE__);
	iequals(__LINE__, F.d <= Aus, 0);
	iequals(__LINE__, Aus <= F.d, 1);
	if (F.d <= Aus) complain(__LINE__);
	if (Aus <= F.d); else complain(__LINE__);
	iequals(__LINE__, F.d <= Aui, 0);
	iequals(__LINE__, Aui <= F.d, 1);
	if (F.d <= Aui) complain(__LINE__);
	if (Aui <= F.d); else complain(__LINE__);
	iequals(__LINE__, F.d <= Al, 0);
	iequals(__LINE__, Al <= F.d, 1);
	if (F.d <= Al) complain(__LINE__);
	if (Al <= F.d); else complain(__LINE__);
	iequals(__LINE__, F.d <= Aul, 0);
	iequals(__LINE__, Aul <= F.d, 1);
	if (F.d <= Aul) complain(__LINE__);
	if (Aul <= F.d); else complain(__LINE__);
	iequals(__LINE__, F.d <= Af, 0);
	iequals(__LINE__, Af <= F.d, 1);
	if (F.d <= Af) complain(__LINE__);
	if (Af <= F.d); else complain(__LINE__);
	iequals(__LINE__, F.d <= Ad, 1);
	iequals(__LINE__, Ad <= F.d, 1);
	if (F.d <= Ad); else complain(__LINE__);
	if (Ad <= F.d); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, F.d <= Ald, 1);
	iequals(__LINE__, Ald <= F.d, 0);
	if (F.d <= Ald); else complain(__LINE__);
	if (Ald <= F.d) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Ac, 0);
	iequals(__LINE__, Ac <= F.ld, 1);
	if (F.ld <= Ac) complain(__LINE__);
	if (Ac <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Asc, 0);
	iequals(__LINE__, Asc <= F.ld, 1);
	if (F.ld <= Asc) complain(__LINE__);
	if (Asc <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= As, 0);
	iequals(__LINE__, As <= F.ld, 1);
	if (F.ld <= As) complain(__LINE__);
	if (As <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Ai, 0);
	iequals(__LINE__, Ai <= F.ld, 1);
	if (F.ld <= Ai) complain(__LINE__);
	if (Ai <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Auc, 0);
	iequals(__LINE__, Auc <= F.ld, 1);
	if (F.ld <= Auc) complain(__LINE__);
	if (Auc <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Aus, 0);
	iequals(__LINE__, Aus <= F.ld, 1);
	if (F.ld <= Aus) complain(__LINE__);
	if (Aus <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Aui, 0);
	iequals(__LINE__, Aui <= F.ld, 1);
	if (F.ld <= Aui) complain(__LINE__);
	if (Aui <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Al, 0);
	iequals(__LINE__, Al <= F.ld, 1);
	if (F.ld <= Al) complain(__LINE__);
	if (Al <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Aul, 0);
	iequals(__LINE__, Aul <= F.ld, 1);
	if (F.ld <= Aul) complain(__LINE__);
	if (Aul <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Af, 0);
	iequals(__LINE__, Af <= F.ld, 1);
	if (F.ld <= Af) complain(__LINE__);
	if (Af <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Ad, 0);
	iequals(__LINE__, Ad <= F.ld, 1);
	if (F.ld <= Ad) complain(__LINE__);
	if (Ad <= F.ld); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, F.ld <= Ald, 1);
	iequals(__LINE__, Ald <= F.ld, 1);
	if (F.ld <= Ald); else complain(__LINE__);
	if (Ald <= F.ld); else complain(__LINE__);
#endif
	}}
	return 	report(Filename);
	}
