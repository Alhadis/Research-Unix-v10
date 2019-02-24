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
	{
	int true = 1, false = 0;
	Filename =  " auto pbits auto funcrs gt ";
	iequals(__LINE__, pL->b1 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b1, 1);
	if (pL->b1 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b1); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b1 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b1, 1);
	if (pL->b1 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b1); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b1 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b1, 1);
	if (pL->b1 > Os().s) complain(__LINE__);
	if (Os().s > pL->b1); else complain(__LINE__);
	iequals(__LINE__, pL->b1 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b1, 1);
	if (pL->b1 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b1); else complain(__LINE__);
	iequals(__LINE__, pL->b1 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b1, 1);
	if (pL->b1 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b1); else complain(__LINE__);
	iequals(__LINE__, pL->b1 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b1, 1);
	if (pL->b1 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b1); else complain(__LINE__);
	iequals(__LINE__, pL->b1 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b1, 1);
	if (pL->b1 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b1); else complain(__LINE__);
	iequals(__LINE__, pL->b1 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b1, 1);
	if (pL->b1 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b1); else complain(__LINE__);
	iequals(__LINE__, pL->b1 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b1, 1);
	if (pL->b1 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b1); else complain(__LINE__);
	iequals(__LINE__, pL->b1 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b1, 1);
	if (pL->b1 > Of().f) complain(__LINE__);
	if (Of().f > pL->b1); else complain(__LINE__);
	iequals(__LINE__, pL->b1 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b1, 1);
	if (pL->b1 > Od().d) complain(__LINE__);
	if (Od().d > pL->b1); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b1 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b1, 1);
	if (pL->b1 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b1); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b2 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b2, 1);
	if (pL->b2 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b2); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b2 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b2, 1);
	if (pL->b2 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b2); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b2 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b2, 1);
	if (pL->b2 > Os().s) complain(__LINE__);
	if (Os().s > pL->b2); else complain(__LINE__);
	iequals(__LINE__, pL->b2 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b2, 1);
	if (pL->b2 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b2); else complain(__LINE__);
	iequals(__LINE__, pL->b2 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b2, 1);
	if (pL->b2 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b2); else complain(__LINE__);
	iequals(__LINE__, pL->b2 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b2, 1);
	if (pL->b2 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b2); else complain(__LINE__);
	iequals(__LINE__, pL->b2 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b2, 1);
	if (pL->b2 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b2); else complain(__LINE__);
	iequals(__LINE__, pL->b2 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b2, 1);
	if (pL->b2 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b2); else complain(__LINE__);
	iequals(__LINE__, pL->b2 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b2, 1);
	if (pL->b2 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b2); else complain(__LINE__);
	iequals(__LINE__, pL->b2 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b2, 1);
	if (pL->b2 > Of().f) complain(__LINE__);
	if (Of().f > pL->b2); else complain(__LINE__);
	iequals(__LINE__, pL->b2 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b2, 1);
	if (pL->b2 > Od().d) complain(__LINE__);
	if (Od().d > pL->b2); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b2 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b2, 1);
	if (pL->b2 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b2); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b3 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b3, 1);
	if (pL->b3 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b3); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b3 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b3, 1);
	if (pL->b3 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b3); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b3 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b3, 1);
	if (pL->b3 > Os().s) complain(__LINE__);
	if (Os().s > pL->b3); else complain(__LINE__);
	iequals(__LINE__, pL->b3 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b3, 1);
	if (pL->b3 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b3); else complain(__LINE__);
	iequals(__LINE__, pL->b3 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b3, 1);
	if (pL->b3 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b3); else complain(__LINE__);
	iequals(__LINE__, pL->b3 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b3, 1);
	if (pL->b3 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b3); else complain(__LINE__);
	iequals(__LINE__, pL->b3 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b3, 1);
	if (pL->b3 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b3); else complain(__LINE__);
	iequals(__LINE__, pL->b3 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b3, 1);
	if (pL->b3 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b3); else complain(__LINE__);
	iequals(__LINE__, pL->b3 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b3, 1);
	if (pL->b3 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b3); else complain(__LINE__);
	iequals(__LINE__, pL->b3 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b3, 1);
	if (pL->b3 > Of().f) complain(__LINE__);
	if (Of().f > pL->b3); else complain(__LINE__);
	iequals(__LINE__, pL->b3 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b3, 1);
	if (pL->b3 > Od().d) complain(__LINE__);
	if (Od().d > pL->b3); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b3 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b3, 1);
	if (pL->b3 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b3); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b4 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b4, 1);
	if (pL->b4 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b4); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b4 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b4, 1);
	if (pL->b4 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b4); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b4 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b4, 1);
	if (pL->b4 > Os().s) complain(__LINE__);
	if (Os().s > pL->b4); else complain(__LINE__);
	iequals(__LINE__, pL->b4 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b4, 1);
	if (pL->b4 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b4); else complain(__LINE__);
	iequals(__LINE__, pL->b4 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b4, 1);
	if (pL->b4 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b4); else complain(__LINE__);
	iequals(__LINE__, pL->b4 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b4, 1);
	if (pL->b4 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b4); else complain(__LINE__);
	iequals(__LINE__, pL->b4 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b4, 1);
	if (pL->b4 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b4); else complain(__LINE__);
	iequals(__LINE__, pL->b4 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b4, 1);
	if (pL->b4 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b4); else complain(__LINE__);
	iequals(__LINE__, pL->b4 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b4, 1);
	if (pL->b4 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b4); else complain(__LINE__);
	iequals(__LINE__, pL->b4 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b4, 1);
	if (pL->b4 > Of().f) complain(__LINE__);
	if (Of().f > pL->b4); else complain(__LINE__);
	iequals(__LINE__, pL->b4 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b4, 1);
	if (pL->b4 > Od().d) complain(__LINE__);
	if (Od().d > pL->b4); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b4 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b4, 1);
	if (pL->b4 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b4); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b5 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b5, 1);
	if (pL->b5 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b5); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b5 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b5, 1);
	if (pL->b5 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b5); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b5 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b5, 1);
	if (pL->b5 > Os().s) complain(__LINE__);
	if (Os().s > pL->b5); else complain(__LINE__);
	iequals(__LINE__, pL->b5 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b5, 1);
	if (pL->b5 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b5); else complain(__LINE__);
	iequals(__LINE__, pL->b5 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b5, 1);
	if (pL->b5 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b5); else complain(__LINE__);
	iequals(__LINE__, pL->b5 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b5, 1);
	if (pL->b5 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b5); else complain(__LINE__);
	iequals(__LINE__, pL->b5 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b5, 1);
	if (pL->b5 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b5); else complain(__LINE__);
	iequals(__LINE__, pL->b5 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b5, 1);
	if (pL->b5 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b5); else complain(__LINE__);
	iequals(__LINE__, pL->b5 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b5, 1);
	if (pL->b5 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b5); else complain(__LINE__);
	iequals(__LINE__, pL->b5 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b5, 1);
	if (pL->b5 > Of().f) complain(__LINE__);
	if (Of().f > pL->b5); else complain(__LINE__);
	iequals(__LINE__, pL->b5 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b5, 1);
	if (pL->b5 > Od().d) complain(__LINE__);
	if (Od().d > pL->b5); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b5 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b5, 1);
	if (pL->b5 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b5); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b6 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b6, 1);
	if (pL->b6 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b6); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b6 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b6, 1);
	if (pL->b6 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b6); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b6 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b6, 1);
	if (pL->b6 > Os().s) complain(__LINE__);
	if (Os().s > pL->b6); else complain(__LINE__);
	iequals(__LINE__, pL->b6 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b6, 1);
	if (pL->b6 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b6); else complain(__LINE__);
	iequals(__LINE__, pL->b6 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b6, 1);
	if (pL->b6 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b6); else complain(__LINE__);
	iequals(__LINE__, pL->b6 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b6, 1);
	if (pL->b6 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b6); else complain(__LINE__);
	iequals(__LINE__, pL->b6 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b6, 1);
	if (pL->b6 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b6); else complain(__LINE__);
	iequals(__LINE__, pL->b6 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b6, 1);
	if (pL->b6 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b6); else complain(__LINE__);
	iequals(__LINE__, pL->b6 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b6, 1);
	if (pL->b6 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b6); else complain(__LINE__);
	iequals(__LINE__, pL->b6 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b6, 1);
	if (pL->b6 > Of().f) complain(__LINE__);
	if (Of().f > pL->b6); else complain(__LINE__);
	iequals(__LINE__, pL->b6 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b6, 1);
	if (pL->b6 > Od().d) complain(__LINE__);
	if (Od().d > pL->b6); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b6 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b6, 1);
	if (pL->b6 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b6); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b7 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b7, 0);
	if (pL->b7 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b7) complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b7 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b7, 1);
	if (pL->b7 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b7); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b7 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b7, 1);
	if (pL->b7 > Os().s) complain(__LINE__);
	if (Os().s > pL->b7); else complain(__LINE__);
	iequals(__LINE__, pL->b7 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b7, 1);
	if (pL->b7 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b7); else complain(__LINE__);
	iequals(__LINE__, pL->b7 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b7, 1);
	if (pL->b7 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b7); else complain(__LINE__);
	iequals(__LINE__, pL->b7 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b7, 1);
	if (pL->b7 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b7); else complain(__LINE__);
	iequals(__LINE__, pL->b7 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b7, 1);
	if (pL->b7 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b7); else complain(__LINE__);
	iequals(__LINE__, pL->b7 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b7, 1);
	if (pL->b7 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b7); else complain(__LINE__);
	iequals(__LINE__, pL->b7 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b7, 1);
	if (pL->b7 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b7); else complain(__LINE__);
	iequals(__LINE__, pL->b7 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b7, 1);
	if (pL->b7 > Of().f) complain(__LINE__);
	if (Of().f > pL->b7); else complain(__LINE__);
	iequals(__LINE__, pL->b7 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b7, 1);
	if (pL->b7 > Od().d) complain(__LINE__);
	if (Od().d > pL->b7); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b7 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b7, 1);
	if (pL->b7 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b7); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b8 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b8, 1);
	if (pL->b8 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b8); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b8 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b8, 1);
	if (pL->b8 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b8); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b8 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b8, 1);
	if (pL->b8 > Os().s) complain(__LINE__);
	if (Os().s > pL->b8); else complain(__LINE__);
	iequals(__LINE__, pL->b8 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b8, 1);
	if (pL->b8 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b8); else complain(__LINE__);
	iequals(__LINE__, pL->b8 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b8, 1);
	if (pL->b8 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b8); else complain(__LINE__);
	iequals(__LINE__, pL->b8 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b8, 1);
	if (pL->b8 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b8); else complain(__LINE__);
	iequals(__LINE__, pL->b8 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b8, 1);
	if (pL->b8 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b8); else complain(__LINE__);
	iequals(__LINE__, pL->b8 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b8, 1);
	if (pL->b8 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b8); else complain(__LINE__);
	iequals(__LINE__, pL->b8 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b8, 1);
	if (pL->b8 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b8); else complain(__LINE__);
	iequals(__LINE__, pL->b8 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b8, 1);
	if (pL->b8 > Of().f) complain(__LINE__);
	if (Of().f > pL->b8); else complain(__LINE__);
	iequals(__LINE__, pL->b8 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b8, 1);
	if (pL->b8 > Od().d) complain(__LINE__);
	if (Od().d > pL->b8); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b8 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b8, 1);
	if (pL->b8 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b8); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b9 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b9, 1);
	if (pL->b9 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b9); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b9 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b9, 1);
	if (pL->b9 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b9); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b9 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b9, 1);
	if (pL->b9 > Os().s) complain(__LINE__);
	if (Os().s > pL->b9); else complain(__LINE__);
	iequals(__LINE__, pL->b9 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b9, 1);
	if (pL->b9 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b9); else complain(__LINE__);
	iequals(__LINE__, pL->b9 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b9, 1);
	if (pL->b9 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b9); else complain(__LINE__);
	iequals(__LINE__, pL->b9 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b9, 1);
	if (pL->b9 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b9); else complain(__LINE__);
	iequals(__LINE__, pL->b9 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b9, 1);
	if (pL->b9 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b9); else complain(__LINE__);
	iequals(__LINE__, pL->b9 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b9, 1);
	if (pL->b9 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b9); else complain(__LINE__);
	iequals(__LINE__, pL->b9 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b9, 1);
	if (pL->b9 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b9); else complain(__LINE__);
	iequals(__LINE__, pL->b9 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b9, 1);
	if (pL->b9 > Of().f) complain(__LINE__);
	if (Of().f > pL->b9); else complain(__LINE__);
	iequals(__LINE__, pL->b9 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b9, 1);
	if (pL->b9 > Od().d) complain(__LINE__);
	if (Od().d > pL->b9); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b9 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b9, 1);
	if (pL->b9 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b9); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b10 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b10, 1);
	if (pL->b10 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b10); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b10 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b10, 1);
	if (pL->b10 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b10); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b10 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b10, 1);
	if (pL->b10 > Os().s) complain(__LINE__);
	if (Os().s > pL->b10); else complain(__LINE__);
	iequals(__LINE__, pL->b10 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b10, 1);
	if (pL->b10 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b10); else complain(__LINE__);
	iequals(__LINE__, pL->b10 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b10, 1);
	if (pL->b10 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b10); else complain(__LINE__);
	iequals(__LINE__, pL->b10 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b10, 1);
	if (pL->b10 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b10); else complain(__LINE__);
	iequals(__LINE__, pL->b10 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b10, 1);
	if (pL->b10 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b10); else complain(__LINE__);
	iequals(__LINE__, pL->b10 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b10, 1);
	if (pL->b10 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b10); else complain(__LINE__);
	iequals(__LINE__, pL->b10 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b10, 1);
	if (pL->b10 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b10); else complain(__LINE__);
	iequals(__LINE__, pL->b10 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b10, 1);
	if (pL->b10 > Of().f) complain(__LINE__);
	if (Of().f > pL->b10); else complain(__LINE__);
	iequals(__LINE__, pL->b10 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b10, 1);
	if (pL->b10 > Od().d) complain(__LINE__);
	if (Od().d > pL->b10); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b10 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b10, 1);
	if (pL->b10 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b10); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b11 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b11, 1);
	if (pL->b11 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b11); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b11 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b11, 1);
	if (pL->b11 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b11); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b11 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b11, 1);
	if (pL->b11 > Os().s) complain(__LINE__);
	if (Os().s > pL->b11); else complain(__LINE__);
	iequals(__LINE__, pL->b11 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b11, 1);
	if (pL->b11 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b11); else complain(__LINE__);
	iequals(__LINE__, pL->b11 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b11, 1);
	if (pL->b11 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b11); else complain(__LINE__);
	iequals(__LINE__, pL->b11 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b11, 1);
	if (pL->b11 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b11); else complain(__LINE__);
	iequals(__LINE__, pL->b11 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b11, 1);
	if (pL->b11 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b11); else complain(__LINE__);
	iequals(__LINE__, pL->b11 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b11, 1);
	if (pL->b11 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b11); else complain(__LINE__);
	iequals(__LINE__, pL->b11 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b11, 1);
	if (pL->b11 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b11); else complain(__LINE__);
	iequals(__LINE__, pL->b11 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b11, 1);
	if (pL->b11 > Of().f) complain(__LINE__);
	if (Of().f > pL->b11); else complain(__LINE__);
	iequals(__LINE__, pL->b11 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b11, 1);
	if (pL->b11 > Od().d) complain(__LINE__);
	if (Od().d > pL->b11); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b11 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b11, 1);
	if (pL->b11 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b11); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b12 > Oc().c, 0);
	iequals(__LINE__, Oc().c > pL->b12, 1);
	if (pL->b12 > Oc().c) complain(__LINE__);
	if (Oc().c > pL->b12); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b12 > Osc().sc, 0);
	iequals(__LINE__, Osc().sc > pL->b12, 1);
	if (pL->b12 > Osc().sc) complain(__LINE__);
	if (Osc().sc > pL->b12); else complain(__LINE__);
#endif
	iequals(__LINE__, pL->b12 > Os().s, 0);
	iequals(__LINE__, Os().s > pL->b12, 1);
	if (pL->b12 > Os().s) complain(__LINE__);
	if (Os().s > pL->b12); else complain(__LINE__);
	iequals(__LINE__, pL->b12 > Oi().i, 0);
	iequals(__LINE__, Oi().i > pL->b12, 1);
	if (pL->b12 > Oi().i) complain(__LINE__);
	if (Oi().i > pL->b12); else complain(__LINE__);
	iequals(__LINE__, pL->b12 > Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc > pL->b12, 1);
	if (pL->b12 > Ouc().uc) complain(__LINE__);
	if (Ouc().uc > pL->b12); else complain(__LINE__);
	iequals(__LINE__, pL->b12 > Ous().us, 0);
	iequals(__LINE__, Ous().us > pL->b12, 1);
	if (pL->b12 > Ous().us) complain(__LINE__);
	if (Ous().us > pL->b12); else complain(__LINE__);
	iequals(__LINE__, pL->b12 > Oui().ui, 0);
	iequals(__LINE__, Oui().ui > pL->b12, 1);
	if (pL->b12 > Oui().ui) complain(__LINE__);
	if (Oui().ui > pL->b12); else complain(__LINE__);
	iequals(__LINE__, pL->b12 > Ol().l, 0);
	iequals(__LINE__, Ol().l > pL->b12, 1);
	if (pL->b12 > Ol().l) complain(__LINE__);
	if (Ol().l > pL->b12); else complain(__LINE__);
	iequals(__LINE__, pL->b12 > Oul().ul, 0);
	iequals(__LINE__, Oul().ul > pL->b12, 1);
	if (pL->b12 > Oul().ul) complain(__LINE__);
	if (Oul().ul > pL->b12); else complain(__LINE__);
	iequals(__LINE__, pL->b12 > Of().f, 0);
	iequals(__LINE__, Of().f > pL->b12, 1);
	if (pL->b12 > Of().f) complain(__LINE__);
	if (Of().f > pL->b12); else complain(__LINE__);
	iequals(__LINE__, pL->b12 > Od().d, 0);
	iequals(__LINE__, Od().d > pL->b12, 1);
	if (pL->b12 > Od().d) complain(__LINE__);
	if (Od().d > pL->b12); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, pL->b12 > Old().ld, 0);
	iequals(__LINE__, Old().ld > pL->b12, 1);
	if (pL->b12 > Old().ld) complain(__LINE__);
	if (Old().ld > pL->b12); else complain(__LINE__);
#endif
	}}
	return 	report(Filename);
	}
