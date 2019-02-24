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
#include "m.h"
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
	CHAR Mc();
#if ANSI
	SCHAR Msc();
#endif
	SHORT Ms();
	INT Mi();
	UCHAR Muc();
	USHORT Mus();
	UINT Mui();
	LONG Ml();
	ULONG Mul();
	FLOAT Mf();
	DOUBLE Md();
#if ANSI
	LDOUBLE Mld();
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto punion auto func rem ";
	iequals(__LINE__, pE1->c % Mc(), 0);
	iequals(__LINE__, Mc() % pE1->c, 0);
#if ANSI
	iequals(__LINE__, pE1->c % Msc(), 7);
	iequals(__LINE__, Msc() % pE1->c, 1);
#endif
	iequals(__LINE__, pE1->c % Ms(), 7);
	iequals(__LINE__, Ms() % pE1->c, 2);
	iequals(__LINE__, pE1->c % Mi(), 7);
	iequals(__LINE__, Mi() % pE1->c, 3);
	iequals(__LINE__, pE1->c % Muc(), 7);
	iequals(__LINE__, Muc() % pE1->c, 4);
	iequals(__LINE__, pE1->c % Mus(), 7);
	iequals(__LINE__, Mus() % pE1->c, 5);
	iequals(__LINE__, pE1->c % Mui(), 7);
	iequals(__LINE__, Mui() % pE1->c, 6);
	lequals(__LINE__, pE1->c % Ml(), 7L);
	lequals(__LINE__, Ml() % pE1->c, 0L);
	lequals(__LINE__, pE1->c % Mul(), 7L);
	lequals(__LINE__, Mul() % pE1->c, 1L);
#if ANSI
	iequals(__LINE__, pE2->sc % Mc(), 1);
	iequals(__LINE__, Mc() % pE2->sc, 7);
#endif
#if ANSI
	iequals(__LINE__, pE2->sc % Msc(), 0);
	iequals(__LINE__, Msc() % pE2->sc, 0);
#endif
#if ANSI
	iequals(__LINE__, pE2->sc % Ms(), 8);
	iequals(__LINE__, Ms() % pE2->sc, 1);
#endif
#if ANSI
	iequals(__LINE__, pE2->sc % Mi(), 8);
	iequals(__LINE__, Mi() % pE2->sc, 2);
#endif
#if ANSI
	iequals(__LINE__, pE2->sc % Muc(), 8);
	iequals(__LINE__, Muc() % pE2->sc, 3);
#endif
#if ANSI
	iequals(__LINE__, pE2->sc % Mus(), 8);
	iequals(__LINE__, Mus() % pE2->sc, 4);
#endif
#if ANSI
	iequals(__LINE__, pE2->sc % Mui(), 8);
	iequals(__LINE__, Mui() % pE2->sc, 5);
#endif
#if ANSI
	lequals(__LINE__, pE2->sc % Ml(), 8L);
	lequals(__LINE__, Ml() % pE2->sc, 6L);
#endif
#if ANSI
	lequals(__LINE__, pE2->sc % Mul(), 8L);
	lequals(__LINE__, Mul() % pE2->sc, 7L);
#endif
	iequals(__LINE__, pE3->s % Mc(), 2);
	iequals(__LINE__, Mc() % pE3->s, 7);
#if ANSI
	iequals(__LINE__, pE3->s % Msc(), 1);
	iequals(__LINE__, Msc() % pE3->s, 8);
#endif
	iequals(__LINE__, pE3->s % Ms(), 0);
	iequals(__LINE__, Ms() % pE3->s, 0);
	iequals(__LINE__, pE3->s % Mi(), 9);
	iequals(__LINE__, Mi() % pE3->s, 1);
	iequals(__LINE__, pE3->s % Muc(), 9);
	iequals(__LINE__, Muc() % pE3->s, 2);
	iequals(__LINE__, pE3->s % Mus(), 9);
	iequals(__LINE__, Mus() % pE3->s, 3);
	iequals(__LINE__, pE3->s % Mui(), 9);
	iequals(__LINE__, Mui() % pE3->s, 4);
	lequals(__LINE__, pE3->s % Ml(), 9L);
	lequals(__LINE__, Ml() % pE3->s, 5L);
	lequals(__LINE__, pE3->s % Mul(), 9L);
	lequals(__LINE__, Mul() % pE3->s, 6L);
	iequals(__LINE__, pE4->i % Mc(), 3);
	iequals(__LINE__, Mc() % pE4->i, 7);
#if ANSI
	iequals(__LINE__, pE4->i % Msc(), 2);
	iequals(__LINE__, Msc() % pE4->i, 8);
#endif
	iequals(__LINE__, pE4->i % Ms(), 1);
	iequals(__LINE__, Ms() % pE4->i, 9);
	iequals(__LINE__, pE4->i % Mi(), 0);
	iequals(__LINE__, Mi() % pE4->i, 0);
	iequals(__LINE__, pE4->i % Muc(), 10);
	iequals(__LINE__, Muc() % pE4->i, 1);
	iequals(__LINE__, pE4->i % Mus(), 10);
	iequals(__LINE__, Mus() % pE4->i, 2);
	iequals(__LINE__, pE4->i % Mui(), 10);
	iequals(__LINE__, Mui() % pE4->i, 3);
	lequals(__LINE__, pE4->i % Ml(), 10L);
	lequals(__LINE__, Ml() % pE4->i, 4L);
	lequals(__LINE__, pE4->i % Mul(), 10L);
	lequals(__LINE__, Mul() % pE4->i, 5L);
	iequals(__LINE__, pE5->uc % Mc(), 4);
	iequals(__LINE__, Mc() % pE5->uc, 7);
#if ANSI
	iequals(__LINE__, pE5->uc % Msc(), 3);
	iequals(__LINE__, Msc() % pE5->uc, 8);
#endif
	iequals(__LINE__, pE5->uc % Ms(), 2);
	iequals(__LINE__, Ms() % pE5->uc, 9);
	iequals(__LINE__, pE5->uc % Mi(), 1);
	iequals(__LINE__, Mi() % pE5->uc, 10);
	iequals(__LINE__, pE5->uc % Muc(), 0);
	iequals(__LINE__, Muc() % pE5->uc, 0);
	iequals(__LINE__, pE5->uc % Mus(), 11);
	iequals(__LINE__, Mus() % pE5->uc, 1);
	iequals(__LINE__, pE5->uc % Mui(), 11);
	iequals(__LINE__, Mui() % pE5->uc, 2);
	lequals(__LINE__, pE5->uc % Ml(), 11L);
	lequals(__LINE__, Ml() % pE5->uc, 3L);
	lequals(__LINE__, pE5->uc % Mul(), 11L);
	lequals(__LINE__, Mul() % pE5->uc, 4L);
	iequals(__LINE__, pE6->us % Mc(), 5);
	iequals(__LINE__, Mc() % pE6->us, 7);
#if ANSI
	iequals(__LINE__, pE6->us % Msc(), 4);
	iequals(__LINE__, Msc() % pE6->us, 8);
#endif
	iequals(__LINE__, pE6->us % Ms(), 3);
	iequals(__LINE__, Ms() % pE6->us, 9);
	iequals(__LINE__, pE6->us % Mi(), 2);
	iequals(__LINE__, Mi() % pE6->us, 10);
	iequals(__LINE__, pE6->us % Muc(), 1);
	iequals(__LINE__, Muc() % pE6->us, 11);
	iequals(__LINE__, pE6->us % Mus(), 0);
	iequals(__LINE__, Mus() % pE6->us, 0);
	iequals(__LINE__, pE6->us % Mui(), 12);
	iequals(__LINE__, Mui() % pE6->us, 1);
	lequals(__LINE__, pE6->us % Ml(), 12L);
	lequals(__LINE__, Ml() % pE6->us, 2L);
	lequals(__LINE__, pE6->us % Mul(), 12L);
	lequals(__LINE__, Mul() % pE6->us, 3L);
	iequals(__LINE__, pE7->ui % Mc(), 6);
	iequals(__LINE__, Mc() % pE7->ui, 7);
#if ANSI
	iequals(__LINE__, pE7->ui % Msc(), 5);
	iequals(__LINE__, Msc() % pE7->ui, 8);
#endif
	iequals(__LINE__, pE7->ui % Ms(), 4);
	iequals(__LINE__, Ms() % pE7->ui, 9);
	iequals(__LINE__, pE7->ui % Mi(), 3);
	iequals(__LINE__, Mi() % pE7->ui, 10);
	iequals(__LINE__, pE7->ui % Muc(), 2);
	iequals(__LINE__, Muc() % pE7->ui, 11);
	iequals(__LINE__, pE7->ui % Mus(), 1);
	iequals(__LINE__, Mus() % pE7->ui, 12);
	iequals(__LINE__, pE7->ui % Mui(), 0);
	iequals(__LINE__, Mui() % pE7->ui, 0);
	lequals(__LINE__, pE7->ui % Ml(), 13L);
	lequals(__LINE__, Ml() % pE7->ui, 1L);
	lequals(__LINE__, pE7->ui % Mul(), 13L);
	lequals(__LINE__, Mul() % pE7->ui, 2L);
	lequals(__LINE__, pE8->l % Mc(), 0L);
	lequals(__LINE__, Mc() % pE8->l, 7L);
#if ANSI
	lequals(__LINE__, pE8->l % Msc(), 6L);
	lequals(__LINE__, Msc() % pE8->l, 8L);
#endif
	lequals(__LINE__, pE8->l % Ms(), 5L);
	lequals(__LINE__, Ms() % pE8->l, 9L);
	lequals(__LINE__, pE8->l % Mi(), 4L);
	lequals(__LINE__, Mi() % pE8->l, 10L);
	lequals(__LINE__, pE8->l % Muc(), 3L);
	lequals(__LINE__, Muc() % pE8->l, 11L);
	lequals(__LINE__, pE8->l % Mus(), 2L);
	lequals(__LINE__, Mus() % pE8->l, 12L);
	lequals(__LINE__, pE8->l % Mui(), 1L);
	lequals(__LINE__, Mui() % pE8->l, 13L);
	lequals(__LINE__, pE8->l % Ml(), 0L);
	lequals(__LINE__, Ml() % pE8->l, 0L);
	lequals(__LINE__, pE8->l % Mul(), 14L);
	lequals(__LINE__, Mul() % pE8->l, 1L);
	lequals(__LINE__, pE9->ul % Mc(), 1L);
	lequals(__LINE__, Mc() % pE9->ul, 7L);
#if ANSI
	lequals(__LINE__, pE9->ul % Msc(), 7L);
	lequals(__LINE__, Msc() % pE9->ul, 8L);
#endif
	lequals(__LINE__, pE9->ul % Ms(), 6L);
	lequals(__LINE__, Ms() % pE9->ul, 9L);
	lequals(__LINE__, pE9->ul % Mi(), 5L);
	lequals(__LINE__, Mi() % pE9->ul, 10L);
	lequals(__LINE__, pE9->ul % Muc(), 4L);
	lequals(__LINE__, Muc() % pE9->ul, 11L);
	lequals(__LINE__, pE9->ul % Mus(), 3L);
	lequals(__LINE__, Mus() % pE9->ul, 12L);
	lequals(__LINE__, pE9->ul % Mui(), 2L);
	lequals(__LINE__, Mui() % pE9->ul, 13L);
	lequals(__LINE__, pE9->ul % Ml(), 1L);
	lequals(__LINE__, Ml() % pE9->ul, 14L);
	lequals(__LINE__, pE9->ul % Mul(), 0L);
	lequals(__LINE__, Mul() % pE9->ul, 0L);
	}}
	return 	report(Filename);
	}
