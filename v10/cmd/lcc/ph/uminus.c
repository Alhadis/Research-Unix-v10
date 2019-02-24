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
#include "o.h"
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
		} G, *pG = &G;
	pG->c = 7;
#if ANSI
	pG->sc = 8;
#endif
	pG->s = 9;
	pG->i = 10;
	pG->uc = 11;
	pG->us = 12;
	pG->ui = 13;
	pG->l = 14;
	pG->ul = 15;
	pG->f = 16;
	pG->d = 17;
#if ANSI
	pG->ld = 18;
#endif
	{
	{
	int true = 1, false = 0;
	Filename =  " auto pstruct1 auto funcrs uminus ";
	iequals(__LINE__, - pG->c, -7);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	iequals(__LINE__, - pG->c, -7);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	iequals(__LINE__, - pG->c, -7);
	iequals(__LINE__, - Os().s, -9);
	iequals(__LINE__, - pG->c, -7);
	iequals(__LINE__, - Oi().i, -10);
	iequals(__LINE__, - pG->c, -7);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	iequals(__LINE__, - pG->c, -7);
	iequals(__LINE__, - (int)Ous().us, -12);
	iequals(__LINE__, - pG->c, -7);
	iequals(__LINE__, - (int)Oui().ui, -13);
	iequals(__LINE__, - pG->c, -7);
	lequals(__LINE__, - Ol().l, -14L);
	iequals(__LINE__, - pG->c, -7);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	iequals(__LINE__, - pG->c, -7);
	dequals(__LINE__, - Of().f, -16.);
	iequals(__LINE__, - pG->c, -7);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	iequals(__LINE__, - pG->c, -7);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	iequals(__LINE__, - Oc().c, -7);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	iequals(__LINE__, - Osc().sc, -8);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	iequals(__LINE__, - Os().s, -9);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	iequals(__LINE__, - Oi().i, -10);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	iequals(__LINE__, - (int)Ouc().uc, -11);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	iequals(__LINE__, - (int)Ous().us, -12);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	iequals(__LINE__, - (int)Oui().ui, -13);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	lequals(__LINE__, - Ol().l, -14L);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	lequals(__LINE__, - (long)Oul().ul, -15L);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	dequals(__LINE__, - Of().f, -16.);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	dequals(__LINE__, - Od().d, -17.);
#endif
#if ANSI
	iequals(__LINE__, - pG->sc, -8);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	iequals(__LINE__, - pG->s, -9);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	iequals(__LINE__, - pG->s, -9);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	iequals(__LINE__, - pG->s, -9);
	iequals(__LINE__, - Os().s, -9);
	iequals(__LINE__, - pG->s, -9);
	iequals(__LINE__, - Oi().i, -10);
	iequals(__LINE__, - pG->s, -9);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	iequals(__LINE__, - pG->s, -9);
	iequals(__LINE__, - (int)Ous().us, -12);
	iequals(__LINE__, - pG->s, -9);
	iequals(__LINE__, - (int)Oui().ui, -13);
	iequals(__LINE__, - pG->s, -9);
	lequals(__LINE__, - Ol().l, -14L);
	iequals(__LINE__, - pG->s, -9);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	iequals(__LINE__, - pG->s, -9);
	dequals(__LINE__, - Of().f, -16.);
	iequals(__LINE__, - pG->s, -9);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	iequals(__LINE__, - pG->s, -9);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	iequals(__LINE__, - pG->i, -10);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	iequals(__LINE__, - pG->i, -10);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	iequals(__LINE__, - pG->i, -10);
	iequals(__LINE__, - Os().s, -9);
	iequals(__LINE__, - pG->i, -10);
	iequals(__LINE__, - Oi().i, -10);
	iequals(__LINE__, - pG->i, -10);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	iequals(__LINE__, - pG->i, -10);
	iequals(__LINE__, - (int)Ous().us, -12);
	iequals(__LINE__, - pG->i, -10);
	iequals(__LINE__, - (int)Oui().ui, -13);
	iequals(__LINE__, - pG->i, -10);
	lequals(__LINE__, - Ol().l, -14L);
	iequals(__LINE__, - pG->i, -10);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	iequals(__LINE__, - pG->i, -10);
	dequals(__LINE__, - Of().f, -16.);
	iequals(__LINE__, - pG->i, -10);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	iequals(__LINE__, - pG->i, -10);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	iequals(__LINE__, - (int)pG->uc, -11);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	iequals(__LINE__, - (int)pG->uc, -11);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	iequals(__LINE__, - (int)pG->uc, -11);
	iequals(__LINE__, - Os().s, -9);
	iequals(__LINE__, - (int)pG->uc, -11);
	iequals(__LINE__, - Oi().i, -10);
	iequals(__LINE__, - (int)pG->uc, -11);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	iequals(__LINE__, - (int)pG->uc, -11);
	iequals(__LINE__, - (int)Ous().us, -12);
	iequals(__LINE__, - (int)pG->uc, -11);
	iequals(__LINE__, - (int)Oui().ui, -13);
	iequals(__LINE__, - (int)pG->uc, -11);
	lequals(__LINE__, - Ol().l, -14L);
	iequals(__LINE__, - (int)pG->uc, -11);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	iequals(__LINE__, - (int)pG->uc, -11);
	dequals(__LINE__, - Of().f, -16.);
	iequals(__LINE__, - (int)pG->uc, -11);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	iequals(__LINE__, - (int)pG->uc, -11);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	iequals(__LINE__, - (int)pG->us, -12);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	iequals(__LINE__, - (int)pG->us, -12);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	iequals(__LINE__, - (int)pG->us, -12);
	iequals(__LINE__, - Os().s, -9);
	iequals(__LINE__, - (int)pG->us, -12);
	iequals(__LINE__, - Oi().i, -10);
	iequals(__LINE__, - (int)pG->us, -12);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	iequals(__LINE__, - (int)pG->us, -12);
	iequals(__LINE__, - (int)Ous().us, -12);
	iequals(__LINE__, - (int)pG->us, -12);
	iequals(__LINE__, - (int)Oui().ui, -13);
	iequals(__LINE__, - (int)pG->us, -12);
	lequals(__LINE__, - Ol().l, -14L);
	iequals(__LINE__, - (int)pG->us, -12);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	iequals(__LINE__, - (int)pG->us, -12);
	dequals(__LINE__, - Of().f, -16.);
	iequals(__LINE__, - (int)pG->us, -12);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	iequals(__LINE__, - (int)pG->us, -12);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	iequals(__LINE__, - (int)pG->ui, -13);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	iequals(__LINE__, - (int)pG->ui, -13);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	iequals(__LINE__, - (int)pG->ui, -13);
	iequals(__LINE__, - Os().s, -9);
	iequals(__LINE__, - (int)pG->ui, -13);
	iequals(__LINE__, - Oi().i, -10);
	iequals(__LINE__, - (int)pG->ui, -13);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	iequals(__LINE__, - (int)pG->ui, -13);
	iequals(__LINE__, - (int)Ous().us, -12);
	iequals(__LINE__, - (int)pG->ui, -13);
	iequals(__LINE__, - (int)Oui().ui, -13);
	iequals(__LINE__, - (int)pG->ui, -13);
	lequals(__LINE__, - Ol().l, -14L);
	iequals(__LINE__, - (int)pG->ui, -13);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	iequals(__LINE__, - (int)pG->ui, -13);
	dequals(__LINE__, - Of().f, -16.);
	iequals(__LINE__, - (int)pG->ui, -13);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	iequals(__LINE__, - (int)pG->ui, -13);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	lequals(__LINE__, - pG->l, -14L);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	lequals(__LINE__, - pG->l, -14L);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	lequals(__LINE__, - pG->l, -14L);
	iequals(__LINE__, - Os().s, -9);
	lequals(__LINE__, - pG->l, -14L);
	iequals(__LINE__, - Oi().i, -10);
	lequals(__LINE__, - pG->l, -14L);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	lequals(__LINE__, - pG->l, -14L);
	iequals(__LINE__, - (int)Ous().us, -12);
	lequals(__LINE__, - pG->l, -14L);
	iequals(__LINE__, - (int)Oui().ui, -13);
	lequals(__LINE__, - pG->l, -14L);
	lequals(__LINE__, - Ol().l, -14L);
	lequals(__LINE__, - pG->l, -14L);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	lequals(__LINE__, - pG->l, -14L);
	dequals(__LINE__, - Of().f, -16.);
	lequals(__LINE__, - pG->l, -14L);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	lequals(__LINE__, - pG->l, -14L);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	lequals(__LINE__, - (long)pG->ul, -15L);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	lequals(__LINE__, - (long)pG->ul, -15L);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	lequals(__LINE__, - (long)pG->ul, -15L);
	iequals(__LINE__, - Os().s, -9);
	lequals(__LINE__, - (long)pG->ul, -15L);
	iequals(__LINE__, - Oi().i, -10);
	lequals(__LINE__, - (long)pG->ul, -15L);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	lequals(__LINE__, - (long)pG->ul, -15L);
	iequals(__LINE__, - (int)Ous().us, -12);
	lequals(__LINE__, - (long)pG->ul, -15L);
	iequals(__LINE__, - (int)Oui().ui, -13);
	lequals(__LINE__, - (long)pG->ul, -15L);
	lequals(__LINE__, - Ol().l, -14L);
	lequals(__LINE__, - (long)pG->ul, -15L);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	lequals(__LINE__, - (long)pG->ul, -15L);
	dequals(__LINE__, - Of().f, -16.);
	lequals(__LINE__, - (long)pG->ul, -15L);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	lequals(__LINE__, - (long)pG->ul, -15L);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	dequals(__LINE__, - pG->f, -16.);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	dequals(__LINE__, - pG->f, -16.);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	dequals(__LINE__, - pG->f, -16.);
	iequals(__LINE__, - Os().s, -9);
	dequals(__LINE__, - pG->f, -16.);
	iequals(__LINE__, - Oi().i, -10);
	dequals(__LINE__, - pG->f, -16.);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	dequals(__LINE__, - pG->f, -16.);
	iequals(__LINE__, - (int)Ous().us, -12);
	dequals(__LINE__, - pG->f, -16.);
	iequals(__LINE__, - (int)Oui().ui, -13);
	dequals(__LINE__, - pG->f, -16.);
	lequals(__LINE__, - Ol().l, -14L);
	dequals(__LINE__, - pG->f, -16.);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	dequals(__LINE__, - pG->f, -16.);
	dequals(__LINE__, - Of().f, -16.);
	dequals(__LINE__, - pG->f, -16.);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	dequals(__LINE__, - pG->f, -16.);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	dequals(__LINE__, - pG->d, -17.);
	iequals(__LINE__, - Oc().c, -7);
#if ANSI
	dequals(__LINE__, - pG->d, -17.);
	iequals(__LINE__, - Osc().sc, -8);
#endif
	dequals(__LINE__, - pG->d, -17.);
	iequals(__LINE__, - Os().s, -9);
	dequals(__LINE__, - pG->d, -17.);
	iequals(__LINE__, - Oi().i, -10);
	dequals(__LINE__, - pG->d, -17.);
	iequals(__LINE__, - (int)Ouc().uc, -11);
	dequals(__LINE__, - pG->d, -17.);
	iequals(__LINE__, - (int)Ous().us, -12);
	dequals(__LINE__, - pG->d, -17.);
	iequals(__LINE__, - (int)Oui().ui, -13);
	dequals(__LINE__, - pG->d, -17.);
	lequals(__LINE__, - Ol().l, -14L);
	dequals(__LINE__, - pG->d, -17.);
	lequals(__LINE__, - (long)Oul().ul, -15L);
	dequals(__LINE__, - pG->d, -17.);
	dequals(__LINE__, - Of().f, -16.);
	dequals(__LINE__, - pG->d, -17.);
	dequals(__LINE__, - Od().d, -17.);
#if ANSI
	dequals(__LINE__, - pG->d, -17.);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	iequals(__LINE__, - Oc().c, -7);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	iequals(__LINE__, - Osc().sc, -8);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	iequals(__LINE__, - Os().s, -9);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	iequals(__LINE__, - Oi().i, -10);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	iequals(__LINE__, - (int)Ouc().uc, -11);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	iequals(__LINE__, - (int)Ous().us, -12);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	iequals(__LINE__, - (int)Oui().ui, -13);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	lequals(__LINE__, - Ol().l, -14L);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	lequals(__LINE__, - (long)Oul().ul, -15L);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	dequals(__LINE__, - Of().f, -16.);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	dequals(__LINE__, - Od().d, -17.);
#endif
#if ANSI
	ldequals(__LINE__, - pG->ld, -18.L);
	ldequals(__LINE__, - Old().ld, -18.L);
#endif
	}}
	return 	report(Filename);
	}
