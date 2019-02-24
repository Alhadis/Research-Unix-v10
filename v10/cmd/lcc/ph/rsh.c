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
	Filename =  " auto scalar auto funcrs rsh ";
	Ac = 7; 
	O_c = 1;
	iequals(__LINE__, Ac >> Oc().c, 3);
	O_c = 7;
	Ac = 1;
	iequals(__LINE__, Oc().c >> Ac, 3);
#if ANSI
	Ac = 7; 
	O_sc = 1;
	iequals(__LINE__, Ac >> Osc().sc, 3);
	O_sc = 8;
	Ac = 1;
	iequals(__LINE__, Osc().sc >> Ac, 4);
#endif
	Ac = 7; 
	O_s = 1;
	iequals(__LINE__, Ac >> Os().s, 3);
	O_s = 9;
	Ac = 1;
	iequals(__LINE__, Os().s >> Ac, 4);
	Ac = 7; 
	O_i = 1;
	iequals(__LINE__, Ac >> Oi().i, 3);
	O_i = 10;
	Ac = 1;
	iequals(__LINE__, Oi().i >> Ac, 5);
	Ac = 7; 
	O_uc = 1;
	iequals(__LINE__, Ac >> Ouc().uc, 3);
	O_uc = 11;
	Ac = 1;
	iequals(__LINE__, Ouc().uc >> Ac, 5);
	Ac = 7; 
	O_us = 1;
	iequals(__LINE__, Ac >> Ous().us, 3);
	O_us = 12;
	Ac = 1;
	iequals(__LINE__, Ous().us >> Ac, 6);
	Ac = 7; 
	O_ui = 1;
	iequals(__LINE__, Ac >> Oui().ui, 3);
	O_ui = 13;
	Ac = 1;
	iequals(__LINE__, Oui().ui >> Ac, 6);
	Ac = 7; 
	O_l = 1;
#if ANSI
	iequals(__LINE__, Ac >> Ol().l, 3);
#else
	lequals(__LINE__, Ac >> Ol().l, 3L);
#endif
	O_l = 14;
	Ac = 1;
#if ANSI
	lequals(__LINE__, Ol().l >> Ac, 7L);
#else
	lequals(__LINE__, Ol().l >> Ac, 7L);
#endif
	Ac = 7; 
	O_ul = 1;
#if ANSI
	iequals(__LINE__, Ac >> Oul().ul, 3);
#else
	lequals(__LINE__, Ac >> Oul().ul, 3L);
#endif
	O_ul = 15;
	Ac = 1;
#if ANSI
	lequals(__LINE__, Oul().ul >> Ac, 7L);
#else
	lequals(__LINE__, Oul().ul >> Ac, 7L);
#endif
#if ANSI
	Asc = 8; 
	O_c = 1;
	iequals(__LINE__, Asc >> Oc().c, 4);
	O_c = 7;
	Asc = 1;
	iequals(__LINE__, Oc().c >> Asc, 3);
#endif
#if ANSI
	Asc = 8; 
	O_sc = 1;
	iequals(__LINE__, Asc >> Osc().sc, 4);
	O_sc = 8;
	Asc = 1;
	iequals(__LINE__, Osc().sc >> Asc, 4);
#endif
#if ANSI
	Asc = 8; 
	O_s = 1;
	iequals(__LINE__, Asc >> Os().s, 4);
	O_s = 9;
	Asc = 1;
	iequals(__LINE__, Os().s >> Asc, 4);
#endif
#if ANSI
	Asc = 8; 
	O_i = 1;
	iequals(__LINE__, Asc >> Oi().i, 4);
	O_i = 10;
	Asc = 1;
	iequals(__LINE__, Oi().i >> Asc, 5);
#endif
#if ANSI
	Asc = 8; 
	O_uc = 1;
	iequals(__LINE__, Asc >> Ouc().uc, 4);
	O_uc = 11;
	Asc = 1;
	iequals(__LINE__, Ouc().uc >> Asc, 5);
#endif
#if ANSI
	Asc = 8; 
	O_us = 1;
	iequals(__LINE__, Asc >> Ous().us, 4);
	O_us = 12;
	Asc = 1;
	iequals(__LINE__, Ous().us >> Asc, 6);
#endif
#if ANSI
	Asc = 8; 
	O_ui = 1;
	iequals(__LINE__, Asc >> Oui().ui, 4);
	O_ui = 13;
	Asc = 1;
	iequals(__LINE__, Oui().ui >> Asc, 6);
#endif
#if ANSI
	Asc = 8; 
	O_l = 1;
#if ANSI
	iequals(__LINE__, Asc >> Ol().l, 4);
#else
	lequals(__LINE__, Asc >> Ol().l, 4L);
#endif
	O_l = 14;
	Asc = 1;
#if ANSI
	lequals(__LINE__, Ol().l >> Asc, 7L);
#else
	lequals(__LINE__, Ol().l >> Asc, 7L);
#endif
#endif
#if ANSI
	Asc = 8; 
	O_ul = 1;
#if ANSI
	iequals(__LINE__, Asc >> Oul().ul, 4);
#else
	lequals(__LINE__, Asc >> Oul().ul, 4L);
#endif
	O_ul = 15;
	Asc = 1;
#if ANSI
	lequals(__LINE__, Oul().ul >> Asc, 7L);
#else
	lequals(__LINE__, Oul().ul >> Asc, 7L);
#endif
#endif
	As = 9; 
	O_c = 1;
	iequals(__LINE__, As >> Oc().c, 4);
	O_c = 7;
	As = 1;
	iequals(__LINE__, Oc().c >> As, 3);
#if ANSI
	As = 9; 
	O_sc = 1;
	iequals(__LINE__, As >> Osc().sc, 4);
	O_sc = 8;
	As = 1;
	iequals(__LINE__, Osc().sc >> As, 4);
#endif
	As = 9; 
	O_s = 1;
	iequals(__LINE__, As >> Os().s, 4);
	O_s = 9;
	As = 1;
	iequals(__LINE__, Os().s >> As, 4);
	As = 9; 
	O_i = 1;
	iequals(__LINE__, As >> Oi().i, 4);
	O_i = 10;
	As = 1;
	iequals(__LINE__, Oi().i >> As, 5);
	As = 9; 
	O_uc = 1;
	iequals(__LINE__, As >> Ouc().uc, 4);
	O_uc = 11;
	As = 1;
	iequals(__LINE__, Ouc().uc >> As, 5);
	As = 9; 
	O_us = 1;
	iequals(__LINE__, As >> Ous().us, 4);
	O_us = 12;
	As = 1;
	iequals(__LINE__, Ous().us >> As, 6);
	As = 9; 
	O_ui = 1;
	iequals(__LINE__, As >> Oui().ui, 4);
	O_ui = 13;
	As = 1;
	iequals(__LINE__, Oui().ui >> As, 6);
	As = 9; 
	O_l = 1;
#if ANSI
	iequals(__LINE__, As >> Ol().l, 4);
#else
	lequals(__LINE__, As >> Ol().l, 4L);
#endif
	O_l = 14;
	As = 1;
#if ANSI
	lequals(__LINE__, Ol().l >> As, 7L);
#else
	lequals(__LINE__, Ol().l >> As, 7L);
#endif
	As = 9; 
	O_ul = 1;
#if ANSI
	iequals(__LINE__, As >> Oul().ul, 4);
#else
	lequals(__LINE__, As >> Oul().ul, 4L);
#endif
	O_ul = 15;
	As = 1;
#if ANSI
	lequals(__LINE__, Oul().ul >> As, 7L);
#else
	lequals(__LINE__, Oul().ul >> As, 7L);
#endif
	Ai = 10; 
	O_c = 1;
	iequals(__LINE__, Ai >> Oc().c, 5);
	O_c = 7;
	Ai = 1;
	iequals(__LINE__, Oc().c >> Ai, 3);
#if ANSI
	Ai = 10; 
	O_sc = 1;
	iequals(__LINE__, Ai >> Osc().sc, 5);
	O_sc = 8;
	Ai = 1;
	iequals(__LINE__, Osc().sc >> Ai, 4);
#endif
	Ai = 10; 
	O_s = 1;
	iequals(__LINE__, Ai >> Os().s, 5);
	O_s = 9;
	Ai = 1;
	iequals(__LINE__, Os().s >> Ai, 4);
	Ai = 10; 
	O_i = 1;
	iequals(__LINE__, Ai >> Oi().i, 5);
	O_i = 10;
	Ai = 1;
	iequals(__LINE__, Oi().i >> Ai, 5);
	Ai = 10; 
	O_uc = 1;
	iequals(__LINE__, Ai >> Ouc().uc, 5);
	O_uc = 11;
	Ai = 1;
	iequals(__LINE__, Ouc().uc >> Ai, 5);
	Ai = 10; 
	O_us = 1;
	iequals(__LINE__, Ai >> Ous().us, 5);
	O_us = 12;
	Ai = 1;
	iequals(__LINE__, Ous().us >> Ai, 6);
	Ai = 10; 
	O_ui = 1;
	iequals(__LINE__, Ai >> Oui().ui, 5);
	O_ui = 13;
	Ai = 1;
	iequals(__LINE__, Oui().ui >> Ai, 6);
	Ai = 10; 
	O_l = 1;
#if ANSI
	iequals(__LINE__, Ai >> Ol().l, 5);
#else
	lequals(__LINE__, Ai >> Ol().l, 5L);
#endif
	O_l = 14;
	Ai = 1;
#if ANSI
	lequals(__LINE__, Ol().l >> Ai, 7L);
#else
	lequals(__LINE__, Ol().l >> Ai, 7L);
#endif
	Ai = 10; 
	O_ul = 1;
#if ANSI
	iequals(__LINE__, Ai >> Oul().ul, 5);
#else
	lequals(__LINE__, Ai >> Oul().ul, 5L);
#endif
	O_ul = 15;
	Ai = 1;
#if ANSI
	lequals(__LINE__, Oul().ul >> Ai, 7L);
#else
	lequals(__LINE__, Oul().ul >> Ai, 7L);
#endif
	Auc = 11; 
	O_c = 1;
	iequals(__LINE__, Auc >> Oc().c, 5);
	O_c = 7;
	Auc = 1;
	iequals(__LINE__, Oc().c >> Auc, 3);
#if ANSI
	Auc = 11; 
	O_sc = 1;
	iequals(__LINE__, Auc >> Osc().sc, 5);
	O_sc = 8;
	Auc = 1;
	iequals(__LINE__, Osc().sc >> Auc, 4);
#endif
	Auc = 11; 
	O_s = 1;
	iequals(__LINE__, Auc >> Os().s, 5);
	O_s = 9;
	Auc = 1;
	iequals(__LINE__, Os().s >> Auc, 4);
	Auc = 11; 
	O_i = 1;
	iequals(__LINE__, Auc >> Oi().i, 5);
	O_i = 10;
	Auc = 1;
	iequals(__LINE__, Oi().i >> Auc, 5);
	Auc = 11; 
	O_uc = 1;
	iequals(__LINE__, Auc >> Ouc().uc, 5);
	O_uc = 11;
	Auc = 1;
	iequals(__LINE__, Ouc().uc >> Auc, 5);
	Auc = 11; 
	O_us = 1;
	iequals(__LINE__, Auc >> Ous().us, 5);
	O_us = 12;
	Auc = 1;
	iequals(__LINE__, Ous().us >> Auc, 6);
	Auc = 11; 
	O_ui = 1;
	iequals(__LINE__, Auc >> Oui().ui, 5);
	O_ui = 13;
	Auc = 1;
	iequals(__LINE__, Oui().ui >> Auc, 6);
	Auc = 11; 
	O_l = 1;
#if ANSI
	iequals(__LINE__, Auc >> Ol().l, 5);
#else
	lequals(__LINE__, Auc >> Ol().l, 5L);
#endif
	O_l = 14;
	Auc = 1;
#if ANSI
	lequals(__LINE__, Ol().l >> Auc, 7L);
#else
	lequals(__LINE__, Ol().l >> Auc, 7L);
#endif
	Auc = 11; 
	O_ul = 1;
#if ANSI
	iequals(__LINE__, Auc >> Oul().ul, 5);
#else
	lequals(__LINE__, Auc >> Oul().ul, 5L);
#endif
	O_ul = 15;
	Auc = 1;
#if ANSI
	lequals(__LINE__, Oul().ul >> Auc, 7L);
#else
	lequals(__LINE__, Oul().ul >> Auc, 7L);
#endif
	Aus = 12; 
	O_c = 1;
	iequals(__LINE__, Aus >> Oc().c, 6);
	O_c = 7;
	Aus = 1;
	iequals(__LINE__, Oc().c >> Aus, 3);
#if ANSI
	Aus = 12; 
	O_sc = 1;
	iequals(__LINE__, Aus >> Osc().sc, 6);
	O_sc = 8;
	Aus = 1;
	iequals(__LINE__, Osc().sc >> Aus, 4);
#endif
	Aus = 12; 
	O_s = 1;
	iequals(__LINE__, Aus >> Os().s, 6);
	O_s = 9;
	Aus = 1;
	iequals(__LINE__, Os().s >> Aus, 4);
	Aus = 12; 
	O_i = 1;
	iequals(__LINE__, Aus >> Oi().i, 6);
	O_i = 10;
	Aus = 1;
	iequals(__LINE__, Oi().i >> Aus, 5);
	Aus = 12; 
	O_uc = 1;
	iequals(__LINE__, Aus >> Ouc().uc, 6);
	O_uc = 11;
	Aus = 1;
	iequals(__LINE__, Ouc().uc >> Aus, 5);
	Aus = 12; 
	O_us = 1;
	iequals(__LINE__, Aus >> Ous().us, 6);
	O_us = 12;
	Aus = 1;
	iequals(__LINE__, Ous().us >> Aus, 6);
	Aus = 12; 
	O_ui = 1;
	iequals(__LINE__, Aus >> Oui().ui, 6);
	O_ui = 13;
	Aus = 1;
	iequals(__LINE__, Oui().ui >> Aus, 6);
	Aus = 12; 
	O_l = 1;
#if ANSI
	iequals(__LINE__, Aus >> Ol().l, 6);
#else
	lequals(__LINE__, Aus >> Ol().l, 6L);
#endif
	O_l = 14;
	Aus = 1;
#if ANSI
	lequals(__LINE__, Ol().l >> Aus, 7L);
#else
	lequals(__LINE__, Ol().l >> Aus, 7L);
#endif
	Aus = 12; 
	O_ul = 1;
#if ANSI
	iequals(__LINE__, Aus >> Oul().ul, 6);
#else
	lequals(__LINE__, Aus >> Oul().ul, 6L);
#endif
	O_ul = 15;
	Aus = 1;
#if ANSI
	lequals(__LINE__, Oul().ul >> Aus, 7L);
#else
	lequals(__LINE__, Oul().ul >> Aus, 7L);
#endif
	Aui = 13; 
	O_c = 1;
	iequals(__LINE__, Aui >> Oc().c, 6);
	O_c = 7;
	Aui = 1;
	iequals(__LINE__, Oc().c >> Aui, 3);
#if ANSI
	Aui = 13; 
	O_sc = 1;
	iequals(__LINE__, Aui >> Osc().sc, 6);
	O_sc = 8;
	Aui = 1;
	iequals(__LINE__, Osc().sc >> Aui, 4);
#endif
	Aui = 13; 
	O_s = 1;
	iequals(__LINE__, Aui >> Os().s, 6);
	O_s = 9;
	Aui = 1;
	iequals(__LINE__, Os().s >> Aui, 4);
	Aui = 13; 
	O_i = 1;
	iequals(__LINE__, Aui >> Oi().i, 6);
	O_i = 10;
	Aui = 1;
	iequals(__LINE__, Oi().i >> Aui, 5);
	Aui = 13; 
	O_uc = 1;
	iequals(__LINE__, Aui >> Ouc().uc, 6);
	O_uc = 11;
	Aui = 1;
	iequals(__LINE__, Ouc().uc >> Aui, 5);
	Aui = 13; 
	O_us = 1;
	iequals(__LINE__, Aui >> Ous().us, 6);
	O_us = 12;
	Aui = 1;
	iequals(__LINE__, Ous().us >> Aui, 6);
	Aui = 13; 
	O_ui = 1;
	iequals(__LINE__, Aui >> Oui().ui, 6);
	O_ui = 13;
	Aui = 1;
	iequals(__LINE__, Oui().ui >> Aui, 6);
	Aui = 13; 
	O_l = 1;
#if ANSI
	iequals(__LINE__, Aui >> Ol().l, 6);
#else
	lequals(__LINE__, Aui >> Ol().l, 6L);
#endif
	O_l = 14;
	Aui = 1;
#if ANSI
	lequals(__LINE__, Ol().l >> Aui, 7L);
#else
	lequals(__LINE__, Ol().l >> Aui, 7L);
#endif
	Aui = 13; 
	O_ul = 1;
#if ANSI
	iequals(__LINE__, Aui >> Oul().ul, 6);
#else
	lequals(__LINE__, Aui >> Oul().ul, 6L);
#endif
	O_ul = 15;
	Aui = 1;
#if ANSI
	lequals(__LINE__, Oul().ul >> Aui, 7L);
#else
	lequals(__LINE__, Oul().ul >> Aui, 7L);
#endif
	Al = 14; 
	O_c = 1;
	lequals(__LINE__, Al >> Oc().c, 7L);
	O_c = 7;
	Al = 1;
	lequals(__LINE__, Oc().c >> Al, 3L);
#if ANSI
	Al = 14; 
	O_sc = 1;
	lequals(__LINE__, Al >> Osc().sc, 7L);
	O_sc = 8;
	Al = 1;
	lequals(__LINE__, Osc().sc >> Al, 4L);
#endif
	Al = 14; 
	O_s = 1;
	lequals(__LINE__, Al >> Os().s, 7L);
	O_s = 9;
	Al = 1;
	lequals(__LINE__, Os().s >> Al, 4L);
	Al = 14; 
	O_i = 1;
	lequals(__LINE__, Al >> Oi().i, 7L);
	O_i = 10;
	Al = 1;
	lequals(__LINE__, Oi().i >> Al, 5L);
	Al = 14; 
	O_uc = 1;
	lequals(__LINE__, Al >> Ouc().uc, 7L);
	O_uc = 11;
	Al = 1;
	lequals(__LINE__, Ouc().uc >> Al, 5L);
	Al = 14; 
	O_us = 1;
	lequals(__LINE__, Al >> Ous().us, 7L);
	O_us = 12;
	Al = 1;
	lequals(__LINE__, Ous().us >> Al, 6L);
	Al = 14; 
	O_ui = 1;
	lequals(__LINE__, Al >> Oui().ui, 7L);
	O_ui = 13;
	Al = 1;
	lequals(__LINE__, Oui().ui >> Al, 6L);
	Al = 14; 
	O_l = 1;
	lequals(__LINE__, Al >> Ol().l, 7L);
	O_l = 14;
	Al = 1;
	lequals(__LINE__, Ol().l >> Al, 7L);
	Al = 14; 
	O_ul = 1;
	lequals(__LINE__, Al >> Oul().ul, 7L);
	O_ul = 15;
	Al = 1;
	lequals(__LINE__, Oul().ul >> Al, 7L);
	Aul = 15; 
	O_c = 1;
	lequals(__LINE__, Aul >> Oc().c, 7L);
	O_c = 7;
	Aul = 1;
	lequals(__LINE__, Oc().c >> Aul, 3L);
#if ANSI
	Aul = 15; 
	O_sc = 1;
	lequals(__LINE__, Aul >> Osc().sc, 7L);
	O_sc = 8;
	Aul = 1;
	lequals(__LINE__, Osc().sc >> Aul, 4L);
#endif
	Aul = 15; 
	O_s = 1;
	lequals(__LINE__, Aul >> Os().s, 7L);
	O_s = 9;
	Aul = 1;
	lequals(__LINE__, Os().s >> Aul, 4L);
	Aul = 15; 
	O_i = 1;
	lequals(__LINE__, Aul >> Oi().i, 7L);
	O_i = 10;
	Aul = 1;
	lequals(__LINE__, Oi().i >> Aul, 5L);
	Aul = 15; 
	O_uc = 1;
	lequals(__LINE__, Aul >> Ouc().uc, 7L);
	O_uc = 11;
	Aul = 1;
	lequals(__LINE__, Ouc().uc >> Aul, 5L);
	Aul = 15; 
	O_us = 1;
	lequals(__LINE__, Aul >> Ous().us, 7L);
	O_us = 12;
	Aul = 1;
	lequals(__LINE__, Ous().us >> Aul, 6L);
	Aul = 15; 
	O_ui = 1;
	lequals(__LINE__, Aul >> Oui().ui, 7L);
	O_ui = 13;
	Aul = 1;
	lequals(__LINE__, Oui().ui >> Aul, 6L);
	Aul = 15; 
	O_l = 1;
	lequals(__LINE__, Aul >> Ol().l, 7L);
	O_l = 14;
	Aul = 1;
	lequals(__LINE__, Ol().l >> Aul, 7L);
	Aul = 15; 
	O_ul = 1;
	lequals(__LINE__, Aul >> Oul().ul, 7L);
	O_ul = 15;
	Aul = 1;
	lequals(__LINE__, Oul().ul >> Aul, 7L);
	}}
	return 	report(Filename);
	}
