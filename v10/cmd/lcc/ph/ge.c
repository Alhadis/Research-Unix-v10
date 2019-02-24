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
	static struct H{
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
		struct H *pH;
		} H, *ppH = &H;
	H.pH = &H;
	ppH->pH->c = 7;
#if ANSI
	ppH->pH->sc = 8;
#endif
	ppH->pH->s = 9;
	ppH->pH->i = 10;
	ppH->pH->uc = 11;
	ppH->pH->us = 12;
	ppH->pH->ui = 13;
	ppH->pH->l = 14;
	ppH->pH->ul = 15;
	ppH->pH->f = 16;
	ppH->pH->d = 17;
#if ANSI
	ppH->pH->ld = 18;
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto pstruct1 auto pstruct2 ge ";
	iequals(__LINE__, pG->c >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->c, 1);
	if (pG->c >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->c); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->c >= ppH->pH->sc, 0);
	iequals(__LINE__, ppH->pH->sc >= pG->c, 1);
	if (pG->c >= ppH->pH->sc) complain(__LINE__);
	if (ppH->pH->sc >= pG->c); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->c >= ppH->pH->s, 0);
	iequals(__LINE__, ppH->pH->s >= pG->c, 1);
	if (pG->c >= ppH->pH->s) complain(__LINE__);
	if (ppH->pH->s >= pG->c); else complain(__LINE__);
	iequals(__LINE__, pG->c >= ppH->pH->i, 0);
	iequals(__LINE__, ppH->pH->i >= pG->c, 1);
	if (pG->c >= ppH->pH->i) complain(__LINE__);
	if (ppH->pH->i >= pG->c); else complain(__LINE__);
	iequals(__LINE__, pG->c >= ppH->pH->uc, 0);
	iequals(__LINE__, ppH->pH->uc >= pG->c, 1);
	if (pG->c >= ppH->pH->uc) complain(__LINE__);
	if (ppH->pH->uc >= pG->c); else complain(__LINE__);
	iequals(__LINE__, pG->c >= ppH->pH->us, 0);
	iequals(__LINE__, ppH->pH->us >= pG->c, 1);
	if (pG->c >= ppH->pH->us) complain(__LINE__);
	if (ppH->pH->us >= pG->c); else complain(__LINE__);
	iequals(__LINE__, pG->c >= ppH->pH->ui, 0);
	iequals(__LINE__, ppH->pH->ui >= pG->c, 1);
	if (pG->c >= ppH->pH->ui) complain(__LINE__);
	if (ppH->pH->ui >= pG->c); else complain(__LINE__);
	iequals(__LINE__, pG->c >= ppH->pH->l, 0);
	iequals(__LINE__, ppH->pH->l >= pG->c, 1);
	if (pG->c >= ppH->pH->l) complain(__LINE__);
	if (ppH->pH->l >= pG->c); else complain(__LINE__);
	iequals(__LINE__, pG->c >= ppH->pH->ul, 0);
	iequals(__LINE__, ppH->pH->ul >= pG->c, 1);
	if (pG->c >= ppH->pH->ul) complain(__LINE__);
	if (ppH->pH->ul >= pG->c); else complain(__LINE__);
	iequals(__LINE__, pG->c >= ppH->pH->f, 0);
	iequals(__LINE__, ppH->pH->f >= pG->c, 1);
	if (pG->c >= ppH->pH->f) complain(__LINE__);
	if (ppH->pH->f >= pG->c); else complain(__LINE__);
	iequals(__LINE__, pG->c >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->c, 1);
	if (pG->c >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->c); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->c >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->c, 1);
	if (pG->c >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->c); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->sc, 0);
	if (pG->sc >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->sc) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->sc, 1);
	if (pG->sc >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->s, 0);
	iequals(__LINE__, ppH->pH->s >= pG->sc, 1);
	if (pG->sc >= ppH->pH->s) complain(__LINE__);
	if (ppH->pH->s >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->i, 0);
	iequals(__LINE__, ppH->pH->i >= pG->sc, 1);
	if (pG->sc >= ppH->pH->i) complain(__LINE__);
	if (ppH->pH->i >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->uc, 0);
	iequals(__LINE__, ppH->pH->uc >= pG->sc, 1);
	if (pG->sc >= ppH->pH->uc) complain(__LINE__);
	if (ppH->pH->uc >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->us, 0);
	iequals(__LINE__, ppH->pH->us >= pG->sc, 1);
	if (pG->sc >= ppH->pH->us) complain(__LINE__);
	if (ppH->pH->us >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->ui, 0);
	iequals(__LINE__, ppH->pH->ui >= pG->sc, 1);
	if (pG->sc >= ppH->pH->ui) complain(__LINE__);
	if (ppH->pH->ui >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->l, 0);
	iequals(__LINE__, ppH->pH->l >= pG->sc, 1);
	if (pG->sc >= ppH->pH->l) complain(__LINE__);
	if (ppH->pH->l >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->ul, 0);
	iequals(__LINE__, ppH->pH->ul >= pG->sc, 1);
	if (pG->sc >= ppH->pH->ul) complain(__LINE__);
	if (ppH->pH->ul >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->f, 0);
	iequals(__LINE__, ppH->pH->f >= pG->sc, 1);
	if (pG->sc >= ppH->pH->f) complain(__LINE__);
	if (ppH->pH->f >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->sc, 1);
	if (pG->sc >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->sc); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->sc >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->sc, 1);
	if (pG->sc >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->sc); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->s >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->s, 0);
	if (pG->s >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->s) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->s >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->s, 0);
	if (pG->s >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->s) complain(__LINE__);
#endif
	iequals(__LINE__, pG->s >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->s, 1);
	if (pG->s >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->s); else complain(__LINE__);
	iequals(__LINE__, pG->s >= ppH->pH->i, 0);
	iequals(__LINE__, ppH->pH->i >= pG->s, 1);
	if (pG->s >= ppH->pH->i) complain(__LINE__);
	if (ppH->pH->i >= pG->s); else complain(__LINE__);
	iequals(__LINE__, pG->s >= ppH->pH->uc, 0);
	iequals(__LINE__, ppH->pH->uc >= pG->s, 1);
	if (pG->s >= ppH->pH->uc) complain(__LINE__);
	if (ppH->pH->uc >= pG->s); else complain(__LINE__);
	iequals(__LINE__, pG->s >= ppH->pH->us, 0);
	iequals(__LINE__, ppH->pH->us >= pG->s, 1);
	if (pG->s >= ppH->pH->us) complain(__LINE__);
	if (ppH->pH->us >= pG->s); else complain(__LINE__);
	iequals(__LINE__, pG->s >= ppH->pH->ui, 0);
	iequals(__LINE__, ppH->pH->ui >= pG->s, 1);
	if (pG->s >= ppH->pH->ui) complain(__LINE__);
	if (ppH->pH->ui >= pG->s); else complain(__LINE__);
	iequals(__LINE__, pG->s >= ppH->pH->l, 0);
	iequals(__LINE__, ppH->pH->l >= pG->s, 1);
	if (pG->s >= ppH->pH->l) complain(__LINE__);
	if (ppH->pH->l >= pG->s); else complain(__LINE__);
	iequals(__LINE__, pG->s >= ppH->pH->ul, 0);
	iequals(__LINE__, ppH->pH->ul >= pG->s, 1);
	if (pG->s >= ppH->pH->ul) complain(__LINE__);
	if (ppH->pH->ul >= pG->s); else complain(__LINE__);
	iequals(__LINE__, pG->s >= ppH->pH->f, 0);
	iequals(__LINE__, ppH->pH->f >= pG->s, 1);
	if (pG->s >= ppH->pH->f) complain(__LINE__);
	if (ppH->pH->f >= pG->s); else complain(__LINE__);
	iequals(__LINE__, pG->s >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->s, 1);
	if (pG->s >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->s); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->s >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->s, 1);
	if (pG->s >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->s); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->i >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->i, 0);
	if (pG->i >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->i) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->i >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->i, 0);
	if (pG->i >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->i) complain(__LINE__);
#endif
	iequals(__LINE__, pG->i >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->i, 0);
	if (pG->i >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->i) complain(__LINE__);
	iequals(__LINE__, pG->i >= ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i >= pG->i, 1);
	if (pG->i >= ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i >= pG->i); else complain(__LINE__);
	iequals(__LINE__, pG->i >= ppH->pH->uc, 0);
	iequals(__LINE__, ppH->pH->uc >= pG->i, 1);
	if (pG->i >= ppH->pH->uc) complain(__LINE__);
	if (ppH->pH->uc >= pG->i); else complain(__LINE__);
	iequals(__LINE__, pG->i >= ppH->pH->us, 0);
	iequals(__LINE__, ppH->pH->us >= pG->i, 1);
	if (pG->i >= ppH->pH->us) complain(__LINE__);
	if (ppH->pH->us >= pG->i); else complain(__LINE__);
	iequals(__LINE__, pG->i >= ppH->pH->ui, 0);
	iequals(__LINE__, ppH->pH->ui >= pG->i, 1);
	if (pG->i >= ppH->pH->ui) complain(__LINE__);
	if (ppH->pH->ui >= pG->i); else complain(__LINE__);
	iequals(__LINE__, pG->i >= ppH->pH->l, 0);
	iequals(__LINE__, ppH->pH->l >= pG->i, 1);
	if (pG->i >= ppH->pH->l) complain(__LINE__);
	if (ppH->pH->l >= pG->i); else complain(__LINE__);
	iequals(__LINE__, pG->i >= ppH->pH->ul, 0);
	iequals(__LINE__, ppH->pH->ul >= pG->i, 1);
	if (pG->i >= ppH->pH->ul) complain(__LINE__);
	if (ppH->pH->ul >= pG->i); else complain(__LINE__);
	iequals(__LINE__, pG->i >= ppH->pH->f, 0);
	iequals(__LINE__, ppH->pH->f >= pG->i, 1);
	if (pG->i >= ppH->pH->f) complain(__LINE__);
	if (ppH->pH->f >= pG->i); else complain(__LINE__);
	iequals(__LINE__, pG->i >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->i, 1);
	if (pG->i >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->i); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->i >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->i, 1);
	if (pG->i >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->i); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->uc >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->uc, 0);
	if (pG->uc >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->uc) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->uc >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->uc, 0);
	if (pG->uc >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->uc) complain(__LINE__);
#endif
	iequals(__LINE__, pG->uc >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->uc, 0);
	if (pG->uc >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->uc) complain(__LINE__);
	iequals(__LINE__, pG->uc >= ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i >= pG->uc, 0);
	if (pG->uc >= ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i >= pG->uc) complain(__LINE__);
	iequals(__LINE__, pG->uc >= ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc >= pG->uc, 1);
	if (pG->uc >= ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc >= pG->uc); else complain(__LINE__);
	iequals(__LINE__, pG->uc >= ppH->pH->us, 0);
	iequals(__LINE__, ppH->pH->us >= pG->uc, 1);
	if (pG->uc >= ppH->pH->us) complain(__LINE__);
	if (ppH->pH->us >= pG->uc); else complain(__LINE__);
	iequals(__LINE__, pG->uc >= ppH->pH->ui, 0);
	iequals(__LINE__, ppH->pH->ui >= pG->uc, 1);
	if (pG->uc >= ppH->pH->ui) complain(__LINE__);
	if (ppH->pH->ui >= pG->uc); else complain(__LINE__);
	iequals(__LINE__, pG->uc >= ppH->pH->l, 0);
	iequals(__LINE__, ppH->pH->l >= pG->uc, 1);
	if (pG->uc >= ppH->pH->l) complain(__LINE__);
	if (ppH->pH->l >= pG->uc); else complain(__LINE__);
	iequals(__LINE__, pG->uc >= ppH->pH->ul, 0);
	iequals(__LINE__, ppH->pH->ul >= pG->uc, 1);
	if (pG->uc >= ppH->pH->ul) complain(__LINE__);
	if (ppH->pH->ul >= pG->uc); else complain(__LINE__);
	iequals(__LINE__, pG->uc >= ppH->pH->f, 0);
	iequals(__LINE__, ppH->pH->f >= pG->uc, 1);
	if (pG->uc >= ppH->pH->f) complain(__LINE__);
	if (ppH->pH->f >= pG->uc); else complain(__LINE__);
	iequals(__LINE__, pG->uc >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->uc, 1);
	if (pG->uc >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->uc); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->uc >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->uc, 1);
	if (pG->uc >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->uc); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->us >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->us, 0);
	if (pG->us >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->us) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->us >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->us, 0);
	if (pG->us >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->us) complain(__LINE__);
#endif
	iequals(__LINE__, pG->us >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->us, 0);
	if (pG->us >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->us) complain(__LINE__);
	iequals(__LINE__, pG->us >= ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i >= pG->us, 0);
	if (pG->us >= ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i >= pG->us) complain(__LINE__);
	iequals(__LINE__, pG->us >= ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc >= pG->us, 0);
	if (pG->us >= ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc >= pG->us) complain(__LINE__);
	iequals(__LINE__, pG->us >= ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us >= pG->us, 1);
	if (pG->us >= ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us >= pG->us); else complain(__LINE__);
	iequals(__LINE__, pG->us >= ppH->pH->ui, 0);
	iequals(__LINE__, ppH->pH->ui >= pG->us, 1);
	if (pG->us >= ppH->pH->ui) complain(__LINE__);
	if (ppH->pH->ui >= pG->us); else complain(__LINE__);
	iequals(__LINE__, pG->us >= ppH->pH->l, 0);
	iequals(__LINE__, ppH->pH->l >= pG->us, 1);
	if (pG->us >= ppH->pH->l) complain(__LINE__);
	if (ppH->pH->l >= pG->us); else complain(__LINE__);
	iequals(__LINE__, pG->us >= ppH->pH->ul, 0);
	iequals(__LINE__, ppH->pH->ul >= pG->us, 1);
	if (pG->us >= ppH->pH->ul) complain(__LINE__);
	if (ppH->pH->ul >= pG->us); else complain(__LINE__);
	iequals(__LINE__, pG->us >= ppH->pH->f, 0);
	iequals(__LINE__, ppH->pH->f >= pG->us, 1);
	if (pG->us >= ppH->pH->f) complain(__LINE__);
	if (ppH->pH->f >= pG->us); else complain(__LINE__);
	iequals(__LINE__, pG->us >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->us, 1);
	if (pG->us >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->us); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->us >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->us, 1);
	if (pG->us >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->us); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->ui >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->ui, 0);
	if (pG->ui >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->ui) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->ui >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->ui, 0);
	if (pG->ui >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->ui) complain(__LINE__);
#endif
	iequals(__LINE__, pG->ui >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->ui, 0);
	if (pG->ui >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->ui) complain(__LINE__);
	iequals(__LINE__, pG->ui >= ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i >= pG->ui, 0);
	if (pG->ui >= ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i >= pG->ui) complain(__LINE__);
	iequals(__LINE__, pG->ui >= ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc >= pG->ui, 0);
	if (pG->ui >= ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc >= pG->ui) complain(__LINE__);
	iequals(__LINE__, pG->ui >= ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us >= pG->ui, 0);
	if (pG->ui >= ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us >= pG->ui) complain(__LINE__);
	iequals(__LINE__, pG->ui >= ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui >= pG->ui, 1);
	if (pG->ui >= ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui >= pG->ui); else complain(__LINE__);
	iequals(__LINE__, pG->ui >= ppH->pH->l, 0);
	iequals(__LINE__, ppH->pH->l >= pG->ui, 1);
	if (pG->ui >= ppH->pH->l) complain(__LINE__);
	if (ppH->pH->l >= pG->ui); else complain(__LINE__);
	iequals(__LINE__, pG->ui >= ppH->pH->ul, 0);
	iequals(__LINE__, ppH->pH->ul >= pG->ui, 1);
	if (pG->ui >= ppH->pH->ul) complain(__LINE__);
	if (ppH->pH->ul >= pG->ui); else complain(__LINE__);
	iequals(__LINE__, pG->ui >= ppH->pH->f, 0);
	iequals(__LINE__, ppH->pH->f >= pG->ui, 1);
	if (pG->ui >= ppH->pH->f) complain(__LINE__);
	if (ppH->pH->f >= pG->ui); else complain(__LINE__);
	iequals(__LINE__, pG->ui >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->ui, 1);
	if (pG->ui >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->ui); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->ui >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->ui, 1);
	if (pG->ui >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->ui); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->l >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->l, 0);
	if (pG->l >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->l) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->l >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->l, 0);
	if (pG->l >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->l) complain(__LINE__);
#endif
	iequals(__LINE__, pG->l >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->l, 0);
	if (pG->l >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->l) complain(__LINE__);
	iequals(__LINE__, pG->l >= ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i >= pG->l, 0);
	if (pG->l >= ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i >= pG->l) complain(__LINE__);
	iequals(__LINE__, pG->l >= ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc >= pG->l, 0);
	if (pG->l >= ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc >= pG->l) complain(__LINE__);
	iequals(__LINE__, pG->l >= ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us >= pG->l, 0);
	if (pG->l >= ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us >= pG->l) complain(__LINE__);
	iequals(__LINE__, pG->l >= ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui >= pG->l, 0);
	if (pG->l >= ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui >= pG->l) complain(__LINE__);
	iequals(__LINE__, pG->l >= ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l >= pG->l, 1);
	if (pG->l >= ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l >= pG->l); else complain(__LINE__);
	iequals(__LINE__, pG->l >= ppH->pH->ul, 0);
	iequals(__LINE__, ppH->pH->ul >= pG->l, 1);
	if (pG->l >= ppH->pH->ul) complain(__LINE__);
	if (ppH->pH->ul >= pG->l); else complain(__LINE__);
	iequals(__LINE__, pG->l >= ppH->pH->f, 0);
	iequals(__LINE__, ppH->pH->f >= pG->l, 1);
	if (pG->l >= ppH->pH->f) complain(__LINE__);
	if (ppH->pH->f >= pG->l); else complain(__LINE__);
	iequals(__LINE__, pG->l >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->l, 1);
	if (pG->l >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->l); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->l >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->l, 1);
	if (pG->l >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->l); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->ul >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->ul, 0);
	if (pG->ul >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->ul) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->ul >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->ul, 0);
	if (pG->ul >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->ul) complain(__LINE__);
#endif
	iequals(__LINE__, pG->ul >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->ul, 0);
	if (pG->ul >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->ul) complain(__LINE__);
	iequals(__LINE__, pG->ul >= ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i >= pG->ul, 0);
	if (pG->ul >= ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i >= pG->ul) complain(__LINE__);
	iequals(__LINE__, pG->ul >= ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc >= pG->ul, 0);
	if (pG->ul >= ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc >= pG->ul) complain(__LINE__);
	iequals(__LINE__, pG->ul >= ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us >= pG->ul, 0);
	if (pG->ul >= ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us >= pG->ul) complain(__LINE__);
	iequals(__LINE__, pG->ul >= ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui >= pG->ul, 0);
	if (pG->ul >= ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui >= pG->ul) complain(__LINE__);
	iequals(__LINE__, pG->ul >= ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l >= pG->ul, 0);
	if (pG->ul >= ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l >= pG->ul) complain(__LINE__);
	iequals(__LINE__, pG->ul >= ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul >= pG->ul, 1);
	if (pG->ul >= ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul >= pG->ul); else complain(__LINE__);
	iequals(__LINE__, pG->ul >= ppH->pH->f, 0);
	iequals(__LINE__, ppH->pH->f >= pG->ul, 1);
	if (pG->ul >= ppH->pH->f) complain(__LINE__);
	if (ppH->pH->f >= pG->ul); else complain(__LINE__);
	iequals(__LINE__, pG->ul >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->ul, 1);
	if (pG->ul >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->ul); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->ul >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->ul, 1);
	if (pG->ul >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->ul); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->f >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->f, 0);
	if (pG->f >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->f) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->f >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->f, 0);
	if (pG->f >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->f) complain(__LINE__);
#endif
	iequals(__LINE__, pG->f >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->f, 0);
	if (pG->f >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->f) complain(__LINE__);
	iequals(__LINE__, pG->f >= ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i >= pG->f, 0);
	if (pG->f >= ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i >= pG->f) complain(__LINE__);
	iequals(__LINE__, pG->f >= ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc >= pG->f, 0);
	if (pG->f >= ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc >= pG->f) complain(__LINE__);
	iequals(__LINE__, pG->f >= ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us >= pG->f, 0);
	if (pG->f >= ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us >= pG->f) complain(__LINE__);
	iequals(__LINE__, pG->f >= ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui >= pG->f, 0);
	if (pG->f >= ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui >= pG->f) complain(__LINE__);
	iequals(__LINE__, pG->f >= ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l >= pG->f, 0);
	if (pG->f >= ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l >= pG->f) complain(__LINE__);
	iequals(__LINE__, pG->f >= ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul >= pG->f, 0);
	if (pG->f >= ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul >= pG->f) complain(__LINE__);
	iequals(__LINE__, pG->f >= ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f >= pG->f, 1);
	if (pG->f >= ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f >= pG->f); else complain(__LINE__);
	iequals(__LINE__, pG->f >= ppH->pH->d, 0);
	iequals(__LINE__, ppH->pH->d >= pG->f, 1);
	if (pG->f >= ppH->pH->d) complain(__LINE__);
	if (ppH->pH->d >= pG->f); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->f >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->f, 1);
	if (pG->f >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->f); else complain(__LINE__);
#endif
	iequals(__LINE__, pG->d >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->d, 0);
	if (pG->d >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->d) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->d >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->d, 0);
	if (pG->d >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->d) complain(__LINE__);
#endif
	iequals(__LINE__, pG->d >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->d, 0);
	if (pG->d >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->d) complain(__LINE__);
	iequals(__LINE__, pG->d >= ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i >= pG->d, 0);
	if (pG->d >= ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i >= pG->d) complain(__LINE__);
	iequals(__LINE__, pG->d >= ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc >= pG->d, 0);
	if (pG->d >= ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc >= pG->d) complain(__LINE__);
	iequals(__LINE__, pG->d >= ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us >= pG->d, 0);
	if (pG->d >= ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us >= pG->d) complain(__LINE__);
	iequals(__LINE__, pG->d >= ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui >= pG->d, 0);
	if (pG->d >= ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui >= pG->d) complain(__LINE__);
	iequals(__LINE__, pG->d >= ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l >= pG->d, 0);
	if (pG->d >= ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l >= pG->d) complain(__LINE__);
	iequals(__LINE__, pG->d >= ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul >= pG->d, 0);
	if (pG->d >= ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul >= pG->d) complain(__LINE__);
	iequals(__LINE__, pG->d >= ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f >= pG->d, 0);
	if (pG->d >= ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f >= pG->d) complain(__LINE__);
	iequals(__LINE__, pG->d >= ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d >= pG->d, 1);
	if (pG->d >= ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d >= pG->d); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pG->d >= ppH->pH->ld, 0);
	iequals(__LINE__, ppH->pH->ld >= pG->d, 1);
	if (pG->d >= ppH->pH->ld) complain(__LINE__);
	if (ppH->pH->ld >= pG->d); else complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c >= pG->ld, 0);
	if (pG->ld >= ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc >= pG->ld, 0);
	if (pG->ld >= ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s >= pG->ld, 0);
	if (pG->ld >= ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i >= pG->ld, 0);
	if (pG->ld >= ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc >= pG->ld, 0);
	if (pG->ld >= ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us >= pG->ld, 0);
	if (pG->ld >= ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui >= pG->ld, 0);
	if (pG->ld >= ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l >= pG->ld, 0);
	if (pG->ld >= ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul >= pG->ld, 0);
	if (pG->ld >= ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f >= pG->ld, 0);
	if (pG->ld >= ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d >= pG->ld, 0);
	if (pG->ld >= ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d >= pG->ld) complain(__LINE__);
#endif
#if ANSI
	iequals(__LINE__, pG->ld >= ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld >= pG->ld, 1);
	if (pG->ld >= ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld >= pG->ld); else complain(__LINE__);
#endif
	}}
	return 	report(Filename);
	}
