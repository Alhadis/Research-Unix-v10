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
	{
	int true = 1, false = 0;
	Filename =  " auto scalar auto funcrs xor ";
	iequals(__LINE__, Ac ^ Oc().c, 0);
	iequals(__LINE__, Oc().c ^ Ac, 0);
#if ANSI
	iequals(__LINE__, Ac ^ Osc().sc, 15);
	iequals(__LINE__, Osc().sc ^ Ac, 15);
#endif
	iequals(__LINE__, Ac ^ Os().s, 14);
	iequals(__LINE__, Os().s ^ Ac, 14);
	iequals(__LINE__, Ac ^ Oi().i, 13);
	iequals(__LINE__, Oi().i ^ Ac, 13);
	iequals(__LINE__, Ac ^ Ouc().uc, 12);
	iequals(__LINE__, Ouc().uc ^ Ac, 12);
	iequals(__LINE__, Ac ^ Ous().us, 11);
	iequals(__LINE__, Ous().us ^ Ac, 11);
	iequals(__LINE__, Ac ^ Oui().ui, 10);
	iequals(__LINE__, Oui().ui ^ Ac, 10);
	lequals(__LINE__, Ac ^ Ol().l, 9L);
	lequals(__LINE__, Ol().l ^ Ac, 9L);
	lequals(__LINE__, Ac ^ Oul().ul, 8L);
	lequals(__LINE__, Oul().ul ^ Ac, 8L);
#if ANSI
	iequals(__LINE__, Asc ^ Oc().c, 15);
	iequals(__LINE__, Oc().c ^ Asc, 15);
#endif
#if ANSI
	iequals(__LINE__, Asc ^ Osc().sc, 0);
	iequals(__LINE__, Osc().sc ^ Asc, 0);
#endif
#if ANSI
	iequals(__LINE__, Asc ^ Os().s, 1);
	iequals(__LINE__, Os().s ^ Asc, 1);
#endif
#if ANSI
	iequals(__LINE__, Asc ^ Oi().i, 2);
	iequals(__LINE__, Oi().i ^ Asc, 2);
#endif
#if ANSI
	iequals(__LINE__, Asc ^ Ouc().uc, 3);
	iequals(__LINE__, Ouc().uc ^ Asc, 3);
#endif
#if ANSI
	iequals(__LINE__, Asc ^ Ous().us, 4);
	iequals(__LINE__, Ous().us ^ Asc, 4);
#endif
#if ANSI
	iequals(__LINE__, Asc ^ Oui().ui, 5);
	iequals(__LINE__, Oui().ui ^ Asc, 5);
#endif
#if ANSI
	lequals(__LINE__, Asc ^ Ol().l, 6L);
	lequals(__LINE__, Ol().l ^ Asc, 6L);
#endif
#if ANSI
	lequals(__LINE__, Asc ^ Oul().ul, 7L);
	lequals(__LINE__, Oul().ul ^ Asc, 7L);
#endif
	iequals(__LINE__, As ^ Oc().c, 14);
	iequals(__LINE__, Oc().c ^ As, 14);
#if ANSI
	iequals(__LINE__, As ^ Osc().sc, 1);
	iequals(__LINE__, Osc().sc ^ As, 1);
#endif
	iequals(__LINE__, As ^ Os().s, 0);
	iequals(__LINE__, Os().s ^ As, 0);
	iequals(__LINE__, As ^ Oi().i, 3);
	iequals(__LINE__, Oi().i ^ As, 3);
	iequals(__LINE__, As ^ Ouc().uc, 2);
	iequals(__LINE__, Ouc().uc ^ As, 2);
	iequals(__LINE__, As ^ Ous().us, 5);
	iequals(__LINE__, Ous().us ^ As, 5);
	iequals(__LINE__, As ^ Oui().ui, 4);
	iequals(__LINE__, Oui().ui ^ As, 4);
	lequals(__LINE__, As ^ Ol().l, 7L);
	lequals(__LINE__, Ol().l ^ As, 7L);
	lequals(__LINE__, As ^ Oul().ul, 6L);
	lequals(__LINE__, Oul().ul ^ As, 6L);
	iequals(__LINE__, Ai ^ Oc().c, 13);
	iequals(__LINE__, Oc().c ^ Ai, 13);
#if ANSI
	iequals(__LINE__, Ai ^ Osc().sc, 2);
	iequals(__LINE__, Osc().sc ^ Ai, 2);
#endif
	iequals(__LINE__, Ai ^ Os().s, 3);
	iequals(__LINE__, Os().s ^ Ai, 3);
	iequals(__LINE__, Ai ^ Oi().i, 0);
	iequals(__LINE__, Oi().i ^ Ai, 0);
	iequals(__LINE__, Ai ^ Ouc().uc, 1);
	iequals(__LINE__, Ouc().uc ^ Ai, 1);
	iequals(__LINE__, Ai ^ Ous().us, 6);
	iequals(__LINE__, Ous().us ^ Ai, 6);
	iequals(__LINE__, Ai ^ Oui().ui, 7);
	iequals(__LINE__, Oui().ui ^ Ai, 7);
	lequals(__LINE__, Ai ^ Ol().l, 4L);
	lequals(__LINE__, Ol().l ^ Ai, 4L);
	lequals(__LINE__, Ai ^ Oul().ul, 5L);
	lequals(__LINE__, Oul().ul ^ Ai, 5L);
	iequals(__LINE__, Auc ^ Oc().c, 12);
	iequals(__LINE__, Oc().c ^ Auc, 12);
#if ANSI
	iequals(__LINE__, Auc ^ Osc().sc, 3);
	iequals(__LINE__, Osc().sc ^ Auc, 3);
#endif
	iequals(__LINE__, Auc ^ Os().s, 2);
	iequals(__LINE__, Os().s ^ Auc, 2);
	iequals(__LINE__, Auc ^ Oi().i, 1);
	iequals(__LINE__, Oi().i ^ Auc, 1);
	iequals(__LINE__, Auc ^ Ouc().uc, 0);
	iequals(__LINE__, Ouc().uc ^ Auc, 0);
	iequals(__LINE__, Auc ^ Ous().us, 7);
	iequals(__LINE__, Ous().us ^ Auc, 7);
	iequals(__LINE__, Auc ^ Oui().ui, 6);
	iequals(__LINE__, Oui().ui ^ Auc, 6);
	lequals(__LINE__, Auc ^ Ol().l, 5L);
	lequals(__LINE__, Ol().l ^ Auc, 5L);
	lequals(__LINE__, Auc ^ Oul().ul, 4L);
	lequals(__LINE__, Oul().ul ^ Auc, 4L);
	iequals(__LINE__, Aus ^ Oc().c, 11);
	iequals(__LINE__, Oc().c ^ Aus, 11);
#if ANSI
	iequals(__LINE__, Aus ^ Osc().sc, 4);
	iequals(__LINE__, Osc().sc ^ Aus, 4);
#endif
	iequals(__LINE__, Aus ^ Os().s, 5);
	iequals(__LINE__, Os().s ^ Aus, 5);
	iequals(__LINE__, Aus ^ Oi().i, 6);
	iequals(__LINE__, Oi().i ^ Aus, 6);
	iequals(__LINE__, Aus ^ Ouc().uc, 7);
	iequals(__LINE__, Ouc().uc ^ Aus, 7);
	iequals(__LINE__, Aus ^ Ous().us, 0);
	iequals(__LINE__, Ous().us ^ Aus, 0);
	iequals(__LINE__, Aus ^ Oui().ui, 1);
	iequals(__LINE__, Oui().ui ^ Aus, 1);
	lequals(__LINE__, Aus ^ Ol().l, 2L);
	lequals(__LINE__, Ol().l ^ Aus, 2L);
	lequals(__LINE__, Aus ^ Oul().ul, 3L);
	lequals(__LINE__, Oul().ul ^ Aus, 3L);
	iequals(__LINE__, Aui ^ Oc().c, 10);
	iequals(__LINE__, Oc().c ^ Aui, 10);
#if ANSI
	iequals(__LINE__, Aui ^ Osc().sc, 5);
	iequals(__LINE__, Osc().sc ^ Aui, 5);
#endif
	iequals(__LINE__, Aui ^ Os().s, 4);
	iequals(__LINE__, Os().s ^ Aui, 4);
	iequals(__LINE__, Aui ^ Oi().i, 7);
	iequals(__LINE__, Oi().i ^ Aui, 7);
	iequals(__LINE__, Aui ^ Ouc().uc, 6);
	iequals(__LINE__, Ouc().uc ^ Aui, 6);
	iequals(__LINE__, Aui ^ Ous().us, 1);
	iequals(__LINE__, Ous().us ^ Aui, 1);
	iequals(__LINE__, Aui ^ Oui().ui, 0);
	iequals(__LINE__, Oui().ui ^ Aui, 0);
	lequals(__LINE__, Aui ^ Ol().l, 3L);
	lequals(__LINE__, Ol().l ^ Aui, 3L);
	lequals(__LINE__, Aui ^ Oul().ul, 2L);
	lequals(__LINE__, Oul().ul ^ Aui, 2L);
	lequals(__LINE__, Al ^ Oc().c, 9L);
	lequals(__LINE__, Oc().c ^ Al, 9L);
#if ANSI
	lequals(__LINE__, Al ^ Osc().sc, 6L);
	lequals(__LINE__, Osc().sc ^ Al, 6L);
#endif
	lequals(__LINE__, Al ^ Os().s, 7L);
	lequals(__LINE__, Os().s ^ Al, 7L);
	lequals(__LINE__, Al ^ Oi().i, 4L);
	lequals(__LINE__, Oi().i ^ Al, 4L);
	lequals(__LINE__, Al ^ Ouc().uc, 5L);
	lequals(__LINE__, Ouc().uc ^ Al, 5L);
	lequals(__LINE__, Al ^ Ous().us, 2L);
	lequals(__LINE__, Ous().us ^ Al, 2L);
	lequals(__LINE__, Al ^ Oui().ui, 3L);
	lequals(__LINE__, Oui().ui ^ Al, 3L);
	lequals(__LINE__, Al ^ Ol().l, 0L);
	lequals(__LINE__, Ol().l ^ Al, 0L);
	lequals(__LINE__, Al ^ Oul().ul, 1L);
	lequals(__LINE__, Oul().ul ^ Al, 1L);
	lequals(__LINE__, Aul ^ Oc().c, 8L);
	lequals(__LINE__, Oc().c ^ Aul, 8L);
#if ANSI
	lequals(__LINE__, Aul ^ Osc().sc, 7L);
	lequals(__LINE__, Osc().sc ^ Aul, 7L);
#endif
	lequals(__LINE__, Aul ^ Os().s, 6L);
	lequals(__LINE__, Os().s ^ Aul, 6L);
	lequals(__LINE__, Aul ^ Oi().i, 5L);
	lequals(__LINE__, Oi().i ^ Aul, 5L);
	lequals(__LINE__, Aul ^ Ouc().uc, 4L);
	lequals(__LINE__, Ouc().uc ^ Aul, 4L);
	lequals(__LINE__, Aul ^ Ous().us, 3L);
	lequals(__LINE__, Ous().us ^ Aul, 3L);
	lequals(__LINE__, Aul ^ Oui().ui, 2L);
	lequals(__LINE__, Oui().ui ^ Aul, 2L);
	lequals(__LINE__, Aul ^ Ol().l, 1L);
	lequals(__LINE__, Ol().l ^ Aul, 1L);
	lequals(__LINE__, Aul ^ Oul().ul, 0L);
	lequals(__LINE__, Oul().ul ^ Aul, 0L);
	}}
	return 	report(Filename);
	}
