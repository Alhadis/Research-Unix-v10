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
		} E;
	auto E E1, *pE1 = &E1;
#if ANSI
	auto E E2, *pE2 = &E2;
#endif
	auto E E3, *pE3 = &E3;
	auto E E4, *pE4 = &E4;
	auto E E5, *pE5 = &E5;
	auto E E6, *pE6 = &E6;
	auto E E7, *pE7 = &E7;
	auto E E8, *pE8 = &E8;
	auto E E9, *pE9 = &E9;
	auto E E10, *pE10 = &E10;
	auto E E11, *pE11 = &E11;
#if ANSI
	auto E E12, *pE12 = &E12;
#endif
	pE1->c = 7;
#if ANSI
	pE2->sc = 8;
#endif
	pE3->s = 9;
	pE4->i = 10;
	pE5->uc = 11;
	pE6->us = 12;
	pE7->ui = 13;
	pE8->l = 14;
	pE9->ul = 15;
	pE10->f = 16;
	pE11->d = 17;
#if ANSI
	pE12->ld = 18;
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto pscalar1 auto punion preinc ";
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBc = 7;
	iequals(__LINE__, ++ (*pBc) , 8);
	iequals(__LINE__, *pBc, 8);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#endif
#if ANSI
	*pBsc = 8;
	iequals(__LINE__, ++ (*pBsc) , 9);
	iequals(__LINE__, *pBsc, 9);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBs = 9;
	iequals(__LINE__, ++ (*pBs) , 10);
	iequals(__LINE__, *pBs, 10);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBi = 10;
	iequals(__LINE__, ++ (*pBi) , 11);
	iequals(__LINE__, *pBi, 11);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBuc = 11;
	iequals(__LINE__, ++ (*pBuc) , 12);
	iequals(__LINE__, *pBuc, 12);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBus = 12;
	iequals(__LINE__, ++ (*pBus) , 13);
	iequals(__LINE__, *pBus, 13);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBui = 13;
	iequals(__LINE__, ++ (*pBui) , 14);
	iequals(__LINE__, *pBui, 14);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBl = 14;
	lequals(__LINE__, ++ (*pBl) , 15L);
	lequals(__LINE__, *pBl, 15L);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBul = 15;
	lequals(__LINE__, ++ (*pBul) , 16L);
	lequals(__LINE__, *pBul, 16L);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBf = 16;
	dequals(__LINE__, ++ (*pBf) , 17.);
	dequals(__LINE__, *pBf, 17.);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#if ANSI
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#if ANSI
	*pBd = 17;
	dequals(__LINE__, ++ (*pBd) , 18.);
	dequals(__LINE__, *pBd, 18.);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE1->c = 7;
	iequals(__LINE__, ++ (pE1->c) , 8);
	iequals(__LINE__, pE1->c, 8);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE2->sc = 8;
	iequals(__LINE__, ++ (pE2->sc) , 9);
	iequals(__LINE__, pE2->sc, 9);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE3->s = 9;
	iequals(__LINE__, ++ (pE3->s) , 10);
	iequals(__LINE__, pE3->s, 10);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE4->i = 10;
	iequals(__LINE__, ++ (pE4->i) , 11);
	iequals(__LINE__, pE4->i, 11);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE5->uc = 11;
	iequals(__LINE__, ++ (pE5->uc) , 12);
	iequals(__LINE__, pE5->uc, 12);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE6->us = 12;
	iequals(__LINE__, ++ (pE6->us) , 13);
	iequals(__LINE__, pE6->us, 13);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE7->ui = 13;
	iequals(__LINE__, ++ (pE7->ui) , 14);
	iequals(__LINE__, pE7->ui, 14);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE8->l = 14;
	lequals(__LINE__, ++ (pE8->l) , 15L);
	lequals(__LINE__, pE8->l, 15L);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE9->ul = 15;
	lequals(__LINE__, ++ (pE9->ul) , 16L);
	lequals(__LINE__, pE9->ul, 16L);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE10->f = 16;
	dequals(__LINE__, ++ (pE10->f) , 17.);
	dequals(__LINE__, pE10->f, 17.);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE11->d = 17;
	dequals(__LINE__, ++ (pE11->d) , 18.);
	dequals(__LINE__, pE11->d, 18.);
#endif
#if ANSI
	*pBld = 18;
	ldequals(__LINE__, ++ (*pBld) , 19.L);
	ldequals(__LINE__, *pBld, 19.L);
	pE12->ld = 18;
	ldequals(__LINE__, ++ (pE12->ld) , 19.L);
	ldequals(__LINE__, pE12->ld, 19.L);
#endif
	}}
	return 	report(Filename);
	}
