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
	Filename =  " auto union auto punion div ";
	pE1->c = 14;
	D1.c = 7;
	iequals(__LINE__, pE1->c / D1.c, 2);
	D1.c = 14;
	pE1->c = 7;
	iequals(__LINE__, D1.c / pE1->c, 2);
#if ANSI
	pE2->sc = 14;
	D1.c = 7;
	iequals(__LINE__, pE2->sc / D1.c, 2);
	D1.c = 16;
	pE2->sc = 8;
	iequals(__LINE__, D1.c / pE2->sc, 2);
#endif
	pE3->s = 14;
	D1.c = 7;
	iequals(__LINE__, pE3->s / D1.c, 2);
	D1.c = 18;
	pE3->s = 9;
	iequals(__LINE__, D1.c / pE3->s, 2);
	pE4->i = 14;
	D1.c = 7;
	iequals(__LINE__, pE4->i / D1.c, 2);
	D1.c = 20;
	pE4->i = 10;
	iequals(__LINE__, D1.c / pE4->i, 2);
	pE5->uc = 14;
	D1.c = 7;
	iequals(__LINE__, pE5->uc / D1.c, 2);
	D1.c = 22;
	pE5->uc = 11;
	iequals(__LINE__, D1.c / pE5->uc, 2);
	pE6->us = 14;
	D1.c = 7;
	iequals(__LINE__, pE6->us / D1.c, 2);
	D1.c = 24;
	pE6->us = 12;
	iequals(__LINE__, D1.c / pE6->us, 2);
	pE7->ui = 14;
	D1.c = 7;
	iequals(__LINE__, pE7->ui / D1.c, 2);
	D1.c = 26;
	pE7->ui = 13;
	iequals(__LINE__, D1.c / pE7->ui, 2);
	pE8->l = 14;
	D1.c = 7;
	lequals(__LINE__, pE8->l / D1.c, 2L);
	D1.c = 28;
	pE8->l = 14;
	lequals(__LINE__, D1.c / pE8->l, 2L);
	pE9->ul = 14;
	D1.c = 7;
	lequals(__LINE__, pE9->ul / D1.c, 2L);
	D1.c = 30;
	pE9->ul = 15;
	lequals(__LINE__, D1.c / pE9->ul, 2L);
	pE10->f = 14;
	D1.c = 7;
	fequals(__LINE__, pE10->f / D1.c, (float)2.);
	D1.c = 32;
	pE10->f = 16;
	fequals(__LINE__, D1.c / pE10->f, (float)2.);
	pE11->d = 14;
	D1.c = 7;
	dequals(__LINE__, pE11->d / D1.c, 2.);
	D1.c = 34;
	pE11->d = 17;
	dequals(__LINE__, D1.c / pE11->d, 2.);
#if ANSI
	pE12->ld = 14;
	D1.c = 7;
	ldequals(__LINE__, pE12->ld / D1.c, 2.L);
	D1.c = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D1.c / pE12->ld, 2.L);
#endif
#if ANSI
	pE1->c = 16;
	D2.sc = 8;
	iequals(__LINE__, pE1->c / D2.sc, 2);
	D2.sc = 14;
	pE1->c = 7;
	iequals(__LINE__, D2.sc / pE1->c, 2);
#endif
#if ANSI
	pE2->sc = 16;
	D2.sc = 8;
	iequals(__LINE__, pE2->sc / D2.sc, 2);
	D2.sc = 16;
	pE2->sc = 8;
	iequals(__LINE__, D2.sc / pE2->sc, 2);
#endif
#if ANSI
	pE3->s = 16;
	D2.sc = 8;
	iequals(__LINE__, pE3->s / D2.sc, 2);
	D2.sc = 18;
	pE3->s = 9;
	iequals(__LINE__, D2.sc / pE3->s, 2);
#endif
#if ANSI
	pE4->i = 16;
	D2.sc = 8;
	iequals(__LINE__, pE4->i / D2.sc, 2);
	D2.sc = 20;
	pE4->i = 10;
	iequals(__LINE__, D2.sc / pE4->i, 2);
#endif
#if ANSI
	pE5->uc = 16;
	D2.sc = 8;
	iequals(__LINE__, pE5->uc / D2.sc, 2);
	D2.sc = 22;
	pE5->uc = 11;
	iequals(__LINE__, D2.sc / pE5->uc, 2);
#endif
#if ANSI
	pE6->us = 16;
	D2.sc = 8;
	iequals(__LINE__, pE6->us / D2.sc, 2);
	D2.sc = 24;
	pE6->us = 12;
	iequals(__LINE__, D2.sc / pE6->us, 2);
#endif
#if ANSI
	pE7->ui = 16;
	D2.sc = 8;
	iequals(__LINE__, pE7->ui / D2.sc, 2);
	D2.sc = 26;
	pE7->ui = 13;
	iequals(__LINE__, D2.sc / pE7->ui, 2);
#endif
#if ANSI
	pE8->l = 16;
	D2.sc = 8;
	lequals(__LINE__, pE8->l / D2.sc, 2L);
	D2.sc = 28;
	pE8->l = 14;
	lequals(__LINE__, D2.sc / pE8->l, 2L);
#endif
#if ANSI
	pE9->ul = 16;
	D2.sc = 8;
	lequals(__LINE__, pE9->ul / D2.sc, 2L);
	D2.sc = 30;
	pE9->ul = 15;
	lequals(__LINE__, D2.sc / pE9->ul, 2L);
#endif
#if ANSI
	pE10->f = 16;
	D2.sc = 8;
	fequals(__LINE__, pE10->f / D2.sc, (float)2.);
	D2.sc = 32;
	pE10->f = 16;
	fequals(__LINE__, D2.sc / pE10->f, (float)2.);
#endif
#if ANSI
	pE11->d = 16;
	D2.sc = 8;
	dequals(__LINE__, pE11->d / D2.sc, 2.);
	D2.sc = 34;
	pE11->d = 17;
	dequals(__LINE__, D2.sc / pE11->d, 2.);
#endif
#if ANSI
	pE12->ld = 16;
	D2.sc = 8;
	ldequals(__LINE__, pE12->ld / D2.sc, 2.L);
	D2.sc = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D2.sc / pE12->ld, 2.L);
#endif
	pE1->c = 18;
	D3.s = 9;
	iequals(__LINE__, pE1->c / D3.s, 2);
	D3.s = 14;
	pE1->c = 7;
	iequals(__LINE__, D3.s / pE1->c, 2);
#if ANSI
	pE2->sc = 18;
	D3.s = 9;
	iequals(__LINE__, pE2->sc / D3.s, 2);
	D3.s = 16;
	pE2->sc = 8;
	iequals(__LINE__, D3.s / pE2->sc, 2);
#endif
	pE3->s = 18;
	D3.s = 9;
	iequals(__LINE__, pE3->s / D3.s, 2);
	D3.s = 18;
	pE3->s = 9;
	iequals(__LINE__, D3.s / pE3->s, 2);
	pE4->i = 18;
	D3.s = 9;
	iequals(__LINE__, pE4->i / D3.s, 2);
	D3.s = 20;
	pE4->i = 10;
	iequals(__LINE__, D3.s / pE4->i, 2);
	pE5->uc = 18;
	D3.s = 9;
	iequals(__LINE__, pE5->uc / D3.s, 2);
	D3.s = 22;
	pE5->uc = 11;
	iequals(__LINE__, D3.s / pE5->uc, 2);
	pE6->us = 18;
	D3.s = 9;
	iequals(__LINE__, pE6->us / D3.s, 2);
	D3.s = 24;
	pE6->us = 12;
	iequals(__LINE__, D3.s / pE6->us, 2);
	pE7->ui = 18;
	D3.s = 9;
	iequals(__LINE__, pE7->ui / D3.s, 2);
	D3.s = 26;
	pE7->ui = 13;
	iequals(__LINE__, D3.s / pE7->ui, 2);
	pE8->l = 18;
	D3.s = 9;
	lequals(__LINE__, pE8->l / D3.s, 2L);
	D3.s = 28;
	pE8->l = 14;
	lequals(__LINE__, D3.s / pE8->l, 2L);
	pE9->ul = 18;
	D3.s = 9;
	lequals(__LINE__, pE9->ul / D3.s, 2L);
	D3.s = 30;
	pE9->ul = 15;
	lequals(__LINE__, D3.s / pE9->ul, 2L);
	pE10->f = 18;
	D3.s = 9;
	fequals(__LINE__, pE10->f / D3.s, (float)2.);
	D3.s = 32;
	pE10->f = 16;
	fequals(__LINE__, D3.s / pE10->f, (float)2.);
	pE11->d = 18;
	D3.s = 9;
	dequals(__LINE__, pE11->d / D3.s, 2.);
	D3.s = 34;
	pE11->d = 17;
	dequals(__LINE__, D3.s / pE11->d, 2.);
#if ANSI
	pE12->ld = 18;
	D3.s = 9;
	ldequals(__LINE__, pE12->ld / D3.s, 2.L);
	D3.s = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D3.s / pE12->ld, 2.L);
#endif
	pE1->c = 20;
	D4.i = 10;
	iequals(__LINE__, pE1->c / D4.i, 2);
	D4.i = 14;
	pE1->c = 7;
	iequals(__LINE__, D4.i / pE1->c, 2);
#if ANSI
	pE2->sc = 20;
	D4.i = 10;
	iequals(__LINE__, pE2->sc / D4.i, 2);
	D4.i = 16;
	pE2->sc = 8;
	iequals(__LINE__, D4.i / pE2->sc, 2);
#endif
	pE3->s = 20;
	D4.i = 10;
	iequals(__LINE__, pE3->s / D4.i, 2);
	D4.i = 18;
	pE3->s = 9;
	iequals(__LINE__, D4.i / pE3->s, 2);
	pE4->i = 20;
	D4.i = 10;
	iequals(__LINE__, pE4->i / D4.i, 2);
	D4.i = 20;
	pE4->i = 10;
	iequals(__LINE__, D4.i / pE4->i, 2);
	pE5->uc = 20;
	D4.i = 10;
	iequals(__LINE__, pE5->uc / D4.i, 2);
	D4.i = 22;
	pE5->uc = 11;
	iequals(__LINE__, D4.i / pE5->uc, 2);
	pE6->us = 20;
	D4.i = 10;
	iequals(__LINE__, pE6->us / D4.i, 2);
	D4.i = 24;
	pE6->us = 12;
	iequals(__LINE__, D4.i / pE6->us, 2);
	pE7->ui = 20;
	D4.i = 10;
	iequals(__LINE__, pE7->ui / D4.i, 2);
	D4.i = 26;
	pE7->ui = 13;
	iequals(__LINE__, D4.i / pE7->ui, 2);
	pE8->l = 20;
	D4.i = 10;
	lequals(__LINE__, pE8->l / D4.i, 2L);
	D4.i = 28;
	pE8->l = 14;
	lequals(__LINE__, D4.i / pE8->l, 2L);
	pE9->ul = 20;
	D4.i = 10;
	lequals(__LINE__, pE9->ul / D4.i, 2L);
	D4.i = 30;
	pE9->ul = 15;
	lequals(__LINE__, D4.i / pE9->ul, 2L);
	pE10->f = 20;
	D4.i = 10;
	fequals(__LINE__, pE10->f / D4.i, (float)2.);
	D4.i = 32;
	pE10->f = 16;
	fequals(__LINE__, D4.i / pE10->f, (float)2.);
	pE11->d = 20;
	D4.i = 10;
	dequals(__LINE__, pE11->d / D4.i, 2.);
	D4.i = 34;
	pE11->d = 17;
	dequals(__LINE__, D4.i / pE11->d, 2.);
#if ANSI
	pE12->ld = 20;
	D4.i = 10;
	ldequals(__LINE__, pE12->ld / D4.i, 2.L);
	D4.i = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D4.i / pE12->ld, 2.L);
#endif
	pE1->c = 22;
	D5.uc = 11;
	iequals(__LINE__, pE1->c / D5.uc, 2);
	D5.uc = 14;
	pE1->c = 7;
	iequals(__LINE__, D5.uc / pE1->c, 2);
#if ANSI
	pE2->sc = 22;
	D5.uc = 11;
	iequals(__LINE__, pE2->sc / D5.uc, 2);
	D5.uc = 16;
	pE2->sc = 8;
	iequals(__LINE__, D5.uc / pE2->sc, 2);
#endif
	pE3->s = 22;
	D5.uc = 11;
	iequals(__LINE__, pE3->s / D5.uc, 2);
	D5.uc = 18;
	pE3->s = 9;
	iequals(__LINE__, D5.uc / pE3->s, 2);
	pE4->i = 22;
	D5.uc = 11;
	iequals(__LINE__, pE4->i / D5.uc, 2);
	D5.uc = 20;
	pE4->i = 10;
	iequals(__LINE__, D5.uc / pE4->i, 2);
	pE5->uc = 22;
	D5.uc = 11;
	iequals(__LINE__, pE5->uc / D5.uc, 2);
	D5.uc = 22;
	pE5->uc = 11;
	iequals(__LINE__, D5.uc / pE5->uc, 2);
	pE6->us = 22;
	D5.uc = 11;
	iequals(__LINE__, pE6->us / D5.uc, 2);
	D5.uc = 24;
	pE6->us = 12;
	iequals(__LINE__, D5.uc / pE6->us, 2);
	pE7->ui = 22;
	D5.uc = 11;
	iequals(__LINE__, pE7->ui / D5.uc, 2);
	D5.uc = 26;
	pE7->ui = 13;
	iequals(__LINE__, D5.uc / pE7->ui, 2);
	pE8->l = 22;
	D5.uc = 11;
	lequals(__LINE__, pE8->l / D5.uc, 2L);
	D5.uc = 28;
	pE8->l = 14;
	lequals(__LINE__, D5.uc / pE8->l, 2L);
	pE9->ul = 22;
	D5.uc = 11;
	lequals(__LINE__, pE9->ul / D5.uc, 2L);
	D5.uc = 30;
	pE9->ul = 15;
	lequals(__LINE__, D5.uc / pE9->ul, 2L);
	pE10->f = 22;
	D5.uc = 11;
	fequals(__LINE__, pE10->f / D5.uc, (float)2.);
	D5.uc = 32;
	pE10->f = 16;
	fequals(__LINE__, D5.uc / pE10->f, (float)2.);
	pE11->d = 22;
	D5.uc = 11;
	dequals(__LINE__, pE11->d / D5.uc, 2.);
	D5.uc = 34;
	pE11->d = 17;
	dequals(__LINE__, D5.uc / pE11->d, 2.);
#if ANSI
	pE12->ld = 22;
	D5.uc = 11;
	ldequals(__LINE__, pE12->ld / D5.uc, 2.L);
	D5.uc = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D5.uc / pE12->ld, 2.L);
#endif
	pE1->c = 24;
	D6.us = 12;
	iequals(__LINE__, pE1->c / D6.us, 2);
	D6.us = 14;
	pE1->c = 7;
	iequals(__LINE__, D6.us / pE1->c, 2);
#if ANSI
	pE2->sc = 24;
	D6.us = 12;
	iequals(__LINE__, pE2->sc / D6.us, 2);
	D6.us = 16;
	pE2->sc = 8;
	iequals(__LINE__, D6.us / pE2->sc, 2);
#endif
	pE3->s = 24;
	D6.us = 12;
	iequals(__LINE__, pE3->s / D6.us, 2);
	D6.us = 18;
	pE3->s = 9;
	iequals(__LINE__, D6.us / pE3->s, 2);
	pE4->i = 24;
	D6.us = 12;
	iequals(__LINE__, pE4->i / D6.us, 2);
	D6.us = 20;
	pE4->i = 10;
	iequals(__LINE__, D6.us / pE4->i, 2);
	pE5->uc = 24;
	D6.us = 12;
	iequals(__LINE__, pE5->uc / D6.us, 2);
	D6.us = 22;
	pE5->uc = 11;
	iequals(__LINE__, D6.us / pE5->uc, 2);
	pE6->us = 24;
	D6.us = 12;
	iequals(__LINE__, pE6->us / D6.us, 2);
	D6.us = 24;
	pE6->us = 12;
	iequals(__LINE__, D6.us / pE6->us, 2);
	pE7->ui = 24;
	D6.us = 12;
	iequals(__LINE__, pE7->ui / D6.us, 2);
	D6.us = 26;
	pE7->ui = 13;
	iequals(__LINE__, D6.us / pE7->ui, 2);
	pE8->l = 24;
	D6.us = 12;
	lequals(__LINE__, pE8->l / D6.us, 2L);
	D6.us = 28;
	pE8->l = 14;
	lequals(__LINE__, D6.us / pE8->l, 2L);
	pE9->ul = 24;
	D6.us = 12;
	lequals(__LINE__, pE9->ul / D6.us, 2L);
	D6.us = 30;
	pE9->ul = 15;
	lequals(__LINE__, D6.us / pE9->ul, 2L);
	pE10->f = 24;
	D6.us = 12;
	fequals(__LINE__, pE10->f / D6.us, (float)2.);
	D6.us = 32;
	pE10->f = 16;
	fequals(__LINE__, D6.us / pE10->f, (float)2.);
	pE11->d = 24;
	D6.us = 12;
	dequals(__LINE__, pE11->d / D6.us, 2.);
	D6.us = 34;
	pE11->d = 17;
	dequals(__LINE__, D6.us / pE11->d, 2.);
#if ANSI
	pE12->ld = 24;
	D6.us = 12;
	ldequals(__LINE__, pE12->ld / D6.us, 2.L);
	D6.us = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D6.us / pE12->ld, 2.L);
#endif
	pE1->c = 26;
	D7.ui = 13;
	iequals(__LINE__, pE1->c / D7.ui, 2);
	D7.ui = 14;
	pE1->c = 7;
	iequals(__LINE__, D7.ui / pE1->c, 2);
#if ANSI
	pE2->sc = 26;
	D7.ui = 13;
	iequals(__LINE__, pE2->sc / D7.ui, 2);
	D7.ui = 16;
	pE2->sc = 8;
	iequals(__LINE__, D7.ui / pE2->sc, 2);
#endif
	pE3->s = 26;
	D7.ui = 13;
	iequals(__LINE__, pE3->s / D7.ui, 2);
	D7.ui = 18;
	pE3->s = 9;
	iequals(__LINE__, D7.ui / pE3->s, 2);
	pE4->i = 26;
	D7.ui = 13;
	iequals(__LINE__, pE4->i / D7.ui, 2);
	D7.ui = 20;
	pE4->i = 10;
	iequals(__LINE__, D7.ui / pE4->i, 2);
	pE5->uc = 26;
	D7.ui = 13;
	iequals(__LINE__, pE5->uc / D7.ui, 2);
	D7.ui = 22;
	pE5->uc = 11;
	iequals(__LINE__, D7.ui / pE5->uc, 2);
	pE6->us = 26;
	D7.ui = 13;
	iequals(__LINE__, pE6->us / D7.ui, 2);
	D7.ui = 24;
	pE6->us = 12;
	iequals(__LINE__, D7.ui / pE6->us, 2);
	pE7->ui = 26;
	D7.ui = 13;
	iequals(__LINE__, pE7->ui / D7.ui, 2);
	D7.ui = 26;
	pE7->ui = 13;
	iequals(__LINE__, D7.ui / pE7->ui, 2);
	pE8->l = 26;
	D7.ui = 13;
	lequals(__LINE__, pE8->l / D7.ui, 2L);
	D7.ui = 28;
	pE8->l = 14;
	lequals(__LINE__, D7.ui / pE8->l, 2L);
	pE9->ul = 26;
	D7.ui = 13;
	lequals(__LINE__, pE9->ul / D7.ui, 2L);
	D7.ui = 30;
	pE9->ul = 15;
	lequals(__LINE__, D7.ui / pE9->ul, 2L);
	pE10->f = 26;
	D7.ui = 13;
	fequals(__LINE__, pE10->f / D7.ui, (float)2.);
	D7.ui = 32;
	pE10->f = 16;
	fequals(__LINE__, D7.ui / pE10->f, (float)2.);
	pE11->d = 26;
	D7.ui = 13;
	dequals(__LINE__, pE11->d / D7.ui, 2.);
	D7.ui = 34;
	pE11->d = 17;
	dequals(__LINE__, D7.ui / pE11->d, 2.);
#if ANSI
	pE12->ld = 26;
	D7.ui = 13;
	ldequals(__LINE__, pE12->ld / D7.ui, 2.L);
	D7.ui = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D7.ui / pE12->ld, 2.L);
#endif
	pE1->c = 28;
	D8.l = 14;
	lequals(__LINE__, pE1->c / D8.l, 2L);
	D8.l = 14;
	pE1->c = 7;
	lequals(__LINE__, D8.l / pE1->c, 2L);
#if ANSI
	pE2->sc = 28;
	D8.l = 14;
	lequals(__LINE__, pE2->sc / D8.l, 2L);
	D8.l = 16;
	pE2->sc = 8;
	lequals(__LINE__, D8.l / pE2->sc, 2L);
#endif
	pE3->s = 28;
	D8.l = 14;
	lequals(__LINE__, pE3->s / D8.l, 2L);
	D8.l = 18;
	pE3->s = 9;
	lequals(__LINE__, D8.l / pE3->s, 2L);
	pE4->i = 28;
	D8.l = 14;
	lequals(__LINE__, pE4->i / D8.l, 2L);
	D8.l = 20;
	pE4->i = 10;
	lequals(__LINE__, D8.l / pE4->i, 2L);
	pE5->uc = 28;
	D8.l = 14;
	lequals(__LINE__, pE5->uc / D8.l, 2L);
	D8.l = 22;
	pE5->uc = 11;
	lequals(__LINE__, D8.l / pE5->uc, 2L);
	pE6->us = 28;
	D8.l = 14;
	lequals(__LINE__, pE6->us / D8.l, 2L);
	D8.l = 24;
	pE6->us = 12;
	lequals(__LINE__, D8.l / pE6->us, 2L);
	pE7->ui = 28;
	D8.l = 14;
	lequals(__LINE__, pE7->ui / D8.l, 2L);
	D8.l = 26;
	pE7->ui = 13;
	lequals(__LINE__, D8.l / pE7->ui, 2L);
	pE8->l = 28;
	D8.l = 14;
	lequals(__LINE__, pE8->l / D8.l, 2L);
	D8.l = 28;
	pE8->l = 14;
	lequals(__LINE__, D8.l / pE8->l, 2L);
	pE9->ul = 28;
	D8.l = 14;
	lequals(__LINE__, pE9->ul / D8.l, 2L);
	D8.l = 30;
	pE9->ul = 15;
	lequals(__LINE__, D8.l / pE9->ul, 2L);
	pE10->f = 28;
	D8.l = 14;
	fequals(__LINE__, pE10->f / D8.l, (float)2.);
	D8.l = 32;
	pE10->f = 16;
	fequals(__LINE__, D8.l / pE10->f, (float)2.);
	pE11->d = 28;
	D8.l = 14;
	dequals(__LINE__, pE11->d / D8.l, 2.);
	D8.l = 34;
	pE11->d = 17;
	dequals(__LINE__, D8.l / pE11->d, 2.);
#if ANSI
	pE12->ld = 28;
	D8.l = 14;
	ldequals(__LINE__, pE12->ld / D8.l, 2.L);
	D8.l = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D8.l / pE12->ld, 2.L);
#endif
	pE1->c = 30;
	D9.ul = 15;
	lequals(__LINE__, pE1->c / D9.ul, 2L);
	D9.ul = 14;
	pE1->c = 7;
	lequals(__LINE__, D9.ul / pE1->c, 2L);
#if ANSI
	pE2->sc = 30;
	D9.ul = 15;
	lequals(__LINE__, pE2->sc / D9.ul, 2L);
	D9.ul = 16;
	pE2->sc = 8;
	lequals(__LINE__, D9.ul / pE2->sc, 2L);
#endif
	pE3->s = 30;
	D9.ul = 15;
	lequals(__LINE__, pE3->s / D9.ul, 2L);
	D9.ul = 18;
	pE3->s = 9;
	lequals(__LINE__, D9.ul / pE3->s, 2L);
	pE4->i = 30;
	D9.ul = 15;
	lequals(__LINE__, pE4->i / D9.ul, 2L);
	D9.ul = 20;
	pE4->i = 10;
	lequals(__LINE__, D9.ul / pE4->i, 2L);
	pE5->uc = 30;
	D9.ul = 15;
	lequals(__LINE__, pE5->uc / D9.ul, 2L);
	D9.ul = 22;
	pE5->uc = 11;
	lequals(__LINE__, D9.ul / pE5->uc, 2L);
	pE6->us = 30;
	D9.ul = 15;
	lequals(__LINE__, pE6->us / D9.ul, 2L);
	D9.ul = 24;
	pE6->us = 12;
	lequals(__LINE__, D9.ul / pE6->us, 2L);
	pE7->ui = 30;
	D9.ul = 15;
	lequals(__LINE__, pE7->ui / D9.ul, 2L);
	D9.ul = 26;
	pE7->ui = 13;
	lequals(__LINE__, D9.ul / pE7->ui, 2L);
	pE8->l = 30;
	D9.ul = 15;
	lequals(__LINE__, pE8->l / D9.ul, 2L);
	D9.ul = 28;
	pE8->l = 14;
	lequals(__LINE__, D9.ul / pE8->l, 2L);
	pE9->ul = 30;
	D9.ul = 15;
	lequals(__LINE__, pE9->ul / D9.ul, 2L);
	D9.ul = 30;
	pE9->ul = 15;
	lequals(__LINE__, D9.ul / pE9->ul, 2L);
	pE10->f = 30;
	D9.ul = 15;
	fequals(__LINE__, pE10->f / D9.ul, (float)2.);
	D9.ul = 32;
	pE10->f = 16;
	fequals(__LINE__, D9.ul / pE10->f, (float)2.);
	pE11->d = 30;
	D9.ul = 15;
	dequals(__LINE__, pE11->d / D9.ul, 2.);
	D9.ul = 34;
	pE11->d = 17;
	dequals(__LINE__, D9.ul / pE11->d, 2.);
#if ANSI
	pE12->ld = 30;
	D9.ul = 15;
	ldequals(__LINE__, pE12->ld / D9.ul, 2.L);
	D9.ul = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D9.ul / pE12->ld, 2.L);
#endif
	pE1->c = 32;
	D10.f = 16;
	fequals(__LINE__, pE1->c / D10.f, (float)2.);
	D10.f = 14;
	pE1->c = 7;
	fequals(__LINE__, D10.f / pE1->c, (float)2.);
#if ANSI
	pE2->sc = 32;
	D10.f = 16;
	fequals(__LINE__, pE2->sc / D10.f, (float)2.);
	D10.f = 16;
	pE2->sc = 8;
	fequals(__LINE__, D10.f / pE2->sc, (float)2.);
#endif
	pE3->s = 32;
	D10.f = 16;
	fequals(__LINE__, pE3->s / D10.f, (float)2.);
	D10.f = 18;
	pE3->s = 9;
	fequals(__LINE__, D10.f / pE3->s, (float)2.);
	pE4->i = 32;
	D10.f = 16;
	fequals(__LINE__, pE4->i / D10.f, (float)2.);
	D10.f = 20;
	pE4->i = 10;
	fequals(__LINE__, D10.f / pE4->i, (float)2.);
	pE5->uc = 32;
	D10.f = 16;
	fequals(__LINE__, pE5->uc / D10.f, (float)2.);
	D10.f = 22;
	pE5->uc = 11;
	fequals(__LINE__, D10.f / pE5->uc, (float)2.);
	pE6->us = 32;
	D10.f = 16;
	fequals(__LINE__, pE6->us / D10.f, (float)2.);
	D10.f = 24;
	pE6->us = 12;
	fequals(__LINE__, D10.f / pE6->us, (float)2.);
	pE7->ui = 32;
	D10.f = 16;
	fequals(__LINE__, pE7->ui / D10.f, (float)2.);
	D10.f = 26;
	pE7->ui = 13;
	fequals(__LINE__, D10.f / pE7->ui, (float)2.);
	pE8->l = 32;
	D10.f = 16;
	fequals(__LINE__, pE8->l / D10.f, (float)2.);
	D10.f = 28;
	pE8->l = 14;
	fequals(__LINE__, D10.f / pE8->l, (float)2.);
	pE9->ul = 32;
	D10.f = 16;
	fequals(__LINE__, pE9->ul / D10.f, (float)2.);
	D10.f = 30;
	pE9->ul = 15;
	fequals(__LINE__, D10.f / pE9->ul, (float)2.);
	pE10->f = 32;
	D10.f = 16;
	fequals(__LINE__, pE10->f / D10.f, (float)2.);
	D10.f = 32;
	pE10->f = 16;
	fequals(__LINE__, D10.f / pE10->f, (float)2.);
	pE11->d = 32;
	D10.f = 16;
	fequals(__LINE__, pE11->d / D10.f, (float)2.);
	D10.f = 34;
	pE11->d = 17;
	fequals(__LINE__, D10.f / pE11->d, (float)2.);
#if ANSI
	pE12->ld = 32;
	D10.f = 16;
	ldequals(__LINE__, pE12->ld / D10.f, 2.L);
	D10.f = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D10.f / pE12->ld, 2.L);
#endif
	pE1->c = 34;
	D11.d = 17;
	dequals(__LINE__, pE1->c / D11.d, 2.);
	D11.d = 14;
	pE1->c = 7;
	dequals(__LINE__, D11.d / pE1->c, 2.);
#if ANSI
	pE2->sc = 34;
	D11.d = 17;
	dequals(__LINE__, pE2->sc / D11.d, 2.);
	D11.d = 16;
	pE2->sc = 8;
	dequals(__LINE__, D11.d / pE2->sc, 2.);
#endif
	pE3->s = 34;
	D11.d = 17;
	dequals(__LINE__, pE3->s / D11.d, 2.);
	D11.d = 18;
	pE3->s = 9;
	dequals(__LINE__, D11.d / pE3->s, 2.);
	pE4->i = 34;
	D11.d = 17;
	dequals(__LINE__, pE4->i / D11.d, 2.);
	D11.d = 20;
	pE4->i = 10;
	dequals(__LINE__, D11.d / pE4->i, 2.);
	pE5->uc = 34;
	D11.d = 17;
	dequals(__LINE__, pE5->uc / D11.d, 2.);
	D11.d = 22;
	pE5->uc = 11;
	dequals(__LINE__, D11.d / pE5->uc, 2.);
	pE6->us = 34;
	D11.d = 17;
	dequals(__LINE__, pE6->us / D11.d, 2.);
	D11.d = 24;
	pE6->us = 12;
	dequals(__LINE__, D11.d / pE6->us, 2.);
	pE7->ui = 34;
	D11.d = 17;
	dequals(__LINE__, pE7->ui / D11.d, 2.);
	D11.d = 26;
	pE7->ui = 13;
	dequals(__LINE__, D11.d / pE7->ui, 2.);
	pE8->l = 34;
	D11.d = 17;
	dequals(__LINE__, pE8->l / D11.d, 2.);
	D11.d = 28;
	pE8->l = 14;
	dequals(__LINE__, D11.d / pE8->l, 2.);
	pE9->ul = 34;
	D11.d = 17;
	dequals(__LINE__, pE9->ul / D11.d, 2.);
	D11.d = 30;
	pE9->ul = 15;
	dequals(__LINE__, D11.d / pE9->ul, 2.);
	pE10->f = 34;
	D11.d = 17;
	dequals(__LINE__, pE10->f / D11.d, 2.);
	D11.d = 32;
	pE10->f = 16;
	dequals(__LINE__, D11.d / pE10->f, 2.);
	pE11->d = 34;
	D11.d = 17;
	dequals(__LINE__, pE11->d / D11.d, 2.);
	D11.d = 34;
	pE11->d = 17;
	dequals(__LINE__, D11.d / pE11->d, 2.);
#if ANSI
	pE12->ld = 34;
	D11.d = 17;
	ldequals(__LINE__, pE12->ld / D11.d, 2.L);
	D11.d = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D11.d / pE12->ld, 2.L);
#endif
#if ANSI
	pE1->c = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE1->c / D12.ld, 2.L);
	D12.ld = 14;
	pE1->c = 7;
	ldequals(__LINE__, D12.ld / pE1->c, 2.L);
#endif
#if ANSI
	pE2->sc = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE2->sc / D12.ld, 2.L);
	D12.ld = 16;
	pE2->sc = 8;
	ldequals(__LINE__, D12.ld / pE2->sc, 2.L);
#endif
#if ANSI
	pE3->s = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE3->s / D12.ld, 2.L);
	D12.ld = 18;
	pE3->s = 9;
	ldequals(__LINE__, D12.ld / pE3->s, 2.L);
#endif
#if ANSI
	pE4->i = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE4->i / D12.ld, 2.L);
	D12.ld = 20;
	pE4->i = 10;
	ldequals(__LINE__, D12.ld / pE4->i, 2.L);
#endif
#if ANSI
	pE5->uc = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE5->uc / D12.ld, 2.L);
	D12.ld = 22;
	pE5->uc = 11;
	ldequals(__LINE__, D12.ld / pE5->uc, 2.L);
#endif
#if ANSI
	pE6->us = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE6->us / D12.ld, 2.L);
	D12.ld = 24;
	pE6->us = 12;
	ldequals(__LINE__, D12.ld / pE6->us, 2.L);
#endif
#if ANSI
	pE7->ui = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE7->ui / D12.ld, 2.L);
	D12.ld = 26;
	pE7->ui = 13;
	ldequals(__LINE__, D12.ld / pE7->ui, 2.L);
#endif
#if ANSI
	pE8->l = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE8->l / D12.ld, 2.L);
	D12.ld = 28;
	pE8->l = 14;
	ldequals(__LINE__, D12.ld / pE8->l, 2.L);
#endif
#if ANSI
	pE9->ul = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE9->ul / D12.ld, 2.L);
	D12.ld = 30;
	pE9->ul = 15;
	ldequals(__LINE__, D12.ld / pE9->ul, 2.L);
#endif
#if ANSI
	pE10->f = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE10->f / D12.ld, 2.L);
	D12.ld = 32;
	pE10->f = 16;
	ldequals(__LINE__, D12.ld / pE10->f, 2.L);
#endif
#if ANSI
	pE11->d = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE11->d / D12.ld, 2.L);
	D12.ld = 34;
	pE11->d = 17;
	ldequals(__LINE__, D12.ld / pE11->d, 2.L);
#endif
#if ANSI
	pE12->ld = 36;
	D12.ld = 18;
	ldequals(__LINE__, pE12->ld / D12.ld, 2.L);
	D12.ld = 36;
	pE12->ld = 18;
	ldequals(__LINE__, D12.ld / pE12->ld, 2.L);
#endif
	}}
	return 	report(Filename);
	}
