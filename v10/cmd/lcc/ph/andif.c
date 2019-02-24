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
		} K;
	K.b1 = 1;
	K.b2 = 2;
	K.b3 = 3;
	K.b4 = 4;
	K.b5 = 5;
	K.b6 = 6;
	K.b7 = 7;
	K.b8 = 5;
	K.b9 = 4;
	K.b10 = 3;
	K.b11 = 2;
	K.b12 = 1;
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
	Filename =  " auto bits auto pstruct2 andif ";
	iequals(__LINE__, K.b1 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b1, 1);
	if (K.b1 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b1); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b1 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b1, 1);
	if (K.b1 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b1); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b1 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b1, 1);
	if (K.b1 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b1); else complain(__LINE__);
	iequals(__LINE__, K.b1 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b1, 1);
	if (K.b1 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b1); else complain(__LINE__);
	iequals(__LINE__, K.b1 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b1, 1);
	if (K.b1 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b1); else complain(__LINE__);
	iequals(__LINE__, K.b1 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b1, 1);
	if (K.b1 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b1); else complain(__LINE__);
	iequals(__LINE__, K.b1 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b1, 1);
	if (K.b1 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b1); else complain(__LINE__);
	iequals(__LINE__, K.b1 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b1, 1);
	if (K.b1 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b1); else complain(__LINE__);
	iequals(__LINE__, K.b1 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b1, 1);
	if (K.b1 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b1); else complain(__LINE__);
	iequals(__LINE__, K.b1 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b1, 1);
	if (K.b1 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b1); else complain(__LINE__);
	iequals(__LINE__, K.b1 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b1, 1);
	if (K.b1 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b1); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b1 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b1, 1);
	if (K.b1 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b1); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b2 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b2, 1);
	if (K.b2 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b2); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b2 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b2, 1);
	if (K.b2 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b2); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b2 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b2, 1);
	if (K.b2 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b2); else complain(__LINE__);
	iequals(__LINE__, K.b2 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b2, 1);
	if (K.b2 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b2); else complain(__LINE__);
	iequals(__LINE__, K.b2 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b2, 1);
	if (K.b2 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b2); else complain(__LINE__);
	iequals(__LINE__, K.b2 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b2, 1);
	if (K.b2 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b2); else complain(__LINE__);
	iequals(__LINE__, K.b2 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b2, 1);
	if (K.b2 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b2); else complain(__LINE__);
	iequals(__LINE__, K.b2 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b2, 1);
	if (K.b2 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b2); else complain(__LINE__);
	iequals(__LINE__, K.b2 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b2, 1);
	if (K.b2 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b2); else complain(__LINE__);
	iequals(__LINE__, K.b2 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b2, 1);
	if (K.b2 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b2); else complain(__LINE__);
	iequals(__LINE__, K.b2 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b2, 1);
	if (K.b2 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b2); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b2 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b2, 1);
	if (K.b2 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b2); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b3 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b3, 1);
	if (K.b3 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b3); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b3 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b3, 1);
	if (K.b3 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b3); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b3 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b3, 1);
	if (K.b3 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b3); else complain(__LINE__);
	iequals(__LINE__, K.b3 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b3, 1);
	if (K.b3 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b3); else complain(__LINE__);
	iequals(__LINE__, K.b3 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b3, 1);
	if (K.b3 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b3); else complain(__LINE__);
	iequals(__LINE__, K.b3 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b3, 1);
	if (K.b3 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b3); else complain(__LINE__);
	iequals(__LINE__, K.b3 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b3, 1);
	if (K.b3 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b3); else complain(__LINE__);
	iequals(__LINE__, K.b3 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b3, 1);
	if (K.b3 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b3); else complain(__LINE__);
	iequals(__LINE__, K.b3 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b3, 1);
	if (K.b3 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b3); else complain(__LINE__);
	iequals(__LINE__, K.b3 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b3, 1);
	if (K.b3 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b3); else complain(__LINE__);
	iequals(__LINE__, K.b3 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b3, 1);
	if (K.b3 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b3); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b3 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b3, 1);
	if (K.b3 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b3); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b4 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b4, 1);
	if (K.b4 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b4); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b4 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b4, 1);
	if (K.b4 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b4); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b4 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b4, 1);
	if (K.b4 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b4); else complain(__LINE__);
	iequals(__LINE__, K.b4 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b4, 1);
	if (K.b4 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b4); else complain(__LINE__);
	iequals(__LINE__, K.b4 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b4, 1);
	if (K.b4 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b4); else complain(__LINE__);
	iequals(__LINE__, K.b4 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b4, 1);
	if (K.b4 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b4); else complain(__LINE__);
	iequals(__LINE__, K.b4 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b4, 1);
	if (K.b4 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b4); else complain(__LINE__);
	iequals(__LINE__, K.b4 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b4, 1);
	if (K.b4 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b4); else complain(__LINE__);
	iequals(__LINE__, K.b4 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b4, 1);
	if (K.b4 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b4); else complain(__LINE__);
	iequals(__LINE__, K.b4 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b4, 1);
	if (K.b4 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b4); else complain(__LINE__);
	iequals(__LINE__, K.b4 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b4, 1);
	if (K.b4 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b4); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b4 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b4, 1);
	if (K.b4 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b4); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b5 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b5, 1);
	if (K.b5 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b5); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b5 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b5, 1);
	if (K.b5 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b5); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b5 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b5, 1);
	if (K.b5 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b5); else complain(__LINE__);
	iequals(__LINE__, K.b5 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b5, 1);
	if (K.b5 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b5); else complain(__LINE__);
	iequals(__LINE__, K.b5 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b5, 1);
	if (K.b5 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b5); else complain(__LINE__);
	iequals(__LINE__, K.b5 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b5, 1);
	if (K.b5 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b5); else complain(__LINE__);
	iequals(__LINE__, K.b5 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b5, 1);
	if (K.b5 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b5); else complain(__LINE__);
	iequals(__LINE__, K.b5 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b5, 1);
	if (K.b5 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b5); else complain(__LINE__);
	iequals(__LINE__, K.b5 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b5, 1);
	if (K.b5 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b5); else complain(__LINE__);
	iequals(__LINE__, K.b5 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b5, 1);
	if (K.b5 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b5); else complain(__LINE__);
	iequals(__LINE__, K.b5 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b5, 1);
	if (K.b5 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b5); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b5 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b5, 1);
	if (K.b5 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b5); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b6 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b6, 1);
	if (K.b6 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b6); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b6 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b6, 1);
	if (K.b6 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b6); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b6 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b6, 1);
	if (K.b6 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b6); else complain(__LINE__);
	iequals(__LINE__, K.b6 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b6, 1);
	if (K.b6 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b6); else complain(__LINE__);
	iequals(__LINE__, K.b6 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b6, 1);
	if (K.b6 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b6); else complain(__LINE__);
	iequals(__LINE__, K.b6 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b6, 1);
	if (K.b6 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b6); else complain(__LINE__);
	iequals(__LINE__, K.b6 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b6, 1);
	if (K.b6 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b6); else complain(__LINE__);
	iequals(__LINE__, K.b6 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b6, 1);
	if (K.b6 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b6); else complain(__LINE__);
	iequals(__LINE__, K.b6 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b6, 1);
	if (K.b6 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b6); else complain(__LINE__);
	iequals(__LINE__, K.b6 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b6, 1);
	if (K.b6 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b6); else complain(__LINE__);
	iequals(__LINE__, K.b6 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b6, 1);
	if (K.b6 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b6); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b6 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b6, 1);
	if (K.b6 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b6); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b7 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b7, 1);
	if (K.b7 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b7); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b7 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b7, 1);
	if (K.b7 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b7); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b7 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b7, 1);
	if (K.b7 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b7); else complain(__LINE__);
	iequals(__LINE__, K.b7 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b7, 1);
	if (K.b7 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b7); else complain(__LINE__);
	iequals(__LINE__, K.b7 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b7, 1);
	if (K.b7 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b7); else complain(__LINE__);
	iequals(__LINE__, K.b7 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b7, 1);
	if (K.b7 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b7); else complain(__LINE__);
	iequals(__LINE__, K.b7 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b7, 1);
	if (K.b7 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b7); else complain(__LINE__);
	iequals(__LINE__, K.b7 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b7, 1);
	if (K.b7 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b7); else complain(__LINE__);
	iequals(__LINE__, K.b7 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b7, 1);
	if (K.b7 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b7); else complain(__LINE__);
	iequals(__LINE__, K.b7 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b7, 1);
	if (K.b7 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b7); else complain(__LINE__);
	iequals(__LINE__, K.b7 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b7, 1);
	if (K.b7 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b7); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b7 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b7, 1);
	if (K.b7 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b7); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b8 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b8, 1);
	if (K.b8 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b8); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b8 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b8, 1);
	if (K.b8 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b8); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b8 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b8, 1);
	if (K.b8 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b8); else complain(__LINE__);
	iequals(__LINE__, K.b8 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b8, 1);
	if (K.b8 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b8); else complain(__LINE__);
	iequals(__LINE__, K.b8 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b8, 1);
	if (K.b8 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b8); else complain(__LINE__);
	iequals(__LINE__, K.b8 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b8, 1);
	if (K.b8 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b8); else complain(__LINE__);
	iequals(__LINE__, K.b8 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b8, 1);
	if (K.b8 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b8); else complain(__LINE__);
	iequals(__LINE__, K.b8 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b8, 1);
	if (K.b8 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b8); else complain(__LINE__);
	iequals(__LINE__, K.b8 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b8, 1);
	if (K.b8 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b8); else complain(__LINE__);
	iequals(__LINE__, K.b8 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b8, 1);
	if (K.b8 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b8); else complain(__LINE__);
	iequals(__LINE__, K.b8 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b8, 1);
	if (K.b8 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b8); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b8 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b8, 1);
	if (K.b8 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b8); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b9 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b9, 1);
	if (K.b9 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b9); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b9 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b9, 1);
	if (K.b9 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b9); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b9 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b9, 1);
	if (K.b9 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b9); else complain(__LINE__);
	iequals(__LINE__, K.b9 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b9, 1);
	if (K.b9 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b9); else complain(__LINE__);
	iequals(__LINE__, K.b9 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b9, 1);
	if (K.b9 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b9); else complain(__LINE__);
	iequals(__LINE__, K.b9 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b9, 1);
	if (K.b9 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b9); else complain(__LINE__);
	iequals(__LINE__, K.b9 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b9, 1);
	if (K.b9 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b9); else complain(__LINE__);
	iequals(__LINE__, K.b9 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b9, 1);
	if (K.b9 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b9); else complain(__LINE__);
	iequals(__LINE__, K.b9 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b9, 1);
	if (K.b9 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b9); else complain(__LINE__);
	iequals(__LINE__, K.b9 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b9, 1);
	if (K.b9 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b9); else complain(__LINE__);
	iequals(__LINE__, K.b9 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b9, 1);
	if (K.b9 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b9); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b9 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b9, 1);
	if (K.b9 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b9); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b10 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b10, 1);
	if (K.b10 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b10); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b10 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b10, 1);
	if (K.b10 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b10); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b10 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b10, 1);
	if (K.b10 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b10); else complain(__LINE__);
	iequals(__LINE__, K.b10 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b10, 1);
	if (K.b10 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b10); else complain(__LINE__);
	iequals(__LINE__, K.b10 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b10, 1);
	if (K.b10 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b10); else complain(__LINE__);
	iequals(__LINE__, K.b10 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b10, 1);
	if (K.b10 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b10); else complain(__LINE__);
	iequals(__LINE__, K.b10 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b10, 1);
	if (K.b10 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b10); else complain(__LINE__);
	iequals(__LINE__, K.b10 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b10, 1);
	if (K.b10 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b10); else complain(__LINE__);
	iequals(__LINE__, K.b10 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b10, 1);
	if (K.b10 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b10); else complain(__LINE__);
	iequals(__LINE__, K.b10 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b10, 1);
	if (K.b10 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b10); else complain(__LINE__);
	iequals(__LINE__, K.b10 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b10, 1);
	if (K.b10 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b10); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b10 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b10, 1);
	if (K.b10 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b10); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b11 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b11, 1);
	if (K.b11 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b11); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b11 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b11, 1);
	if (K.b11 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b11); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b11 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b11, 1);
	if (K.b11 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b11); else complain(__LINE__);
	iequals(__LINE__, K.b11 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b11, 1);
	if (K.b11 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b11); else complain(__LINE__);
	iequals(__LINE__, K.b11 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b11, 1);
	if (K.b11 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b11); else complain(__LINE__);
	iequals(__LINE__, K.b11 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b11, 1);
	if (K.b11 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b11); else complain(__LINE__);
	iequals(__LINE__, K.b11 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b11, 1);
	if (K.b11 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b11); else complain(__LINE__);
	iequals(__LINE__, K.b11 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b11, 1);
	if (K.b11 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b11); else complain(__LINE__);
	iequals(__LINE__, K.b11 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b11, 1);
	if (K.b11 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b11); else complain(__LINE__);
	iequals(__LINE__, K.b11 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b11, 1);
	if (K.b11 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b11); else complain(__LINE__);
	iequals(__LINE__, K.b11 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b11, 1);
	if (K.b11 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b11); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b11 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b11, 1);
	if (K.b11 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b11); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b12 && ppH->pH->c, 1);
	iequals(__LINE__, ppH->pH->c && K.b12, 1);
	if (K.b12 && ppH->pH->c); else complain(__LINE__);
	if (ppH->pH->c && K.b12); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b12 && ppH->pH->sc, 1);
	iequals(__LINE__, ppH->pH->sc && K.b12, 1);
	if (K.b12 && ppH->pH->sc); else complain(__LINE__);
	if (ppH->pH->sc && K.b12); else complain(__LINE__);
#endif
	iequals(__LINE__, K.b12 && ppH->pH->s, 1);
	iequals(__LINE__, ppH->pH->s && K.b12, 1);
	if (K.b12 && ppH->pH->s); else complain(__LINE__);
	if (ppH->pH->s && K.b12); else complain(__LINE__);
	iequals(__LINE__, K.b12 && ppH->pH->i, 1);
	iequals(__LINE__, ppH->pH->i && K.b12, 1);
	if (K.b12 && ppH->pH->i); else complain(__LINE__);
	if (ppH->pH->i && K.b12); else complain(__LINE__);
	iequals(__LINE__, K.b12 && ppH->pH->uc, 1);
	iequals(__LINE__, ppH->pH->uc && K.b12, 1);
	if (K.b12 && ppH->pH->uc); else complain(__LINE__);
	if (ppH->pH->uc && K.b12); else complain(__LINE__);
	iequals(__LINE__, K.b12 && ppH->pH->us, 1);
	iequals(__LINE__, ppH->pH->us && K.b12, 1);
	if (K.b12 && ppH->pH->us); else complain(__LINE__);
	if (ppH->pH->us && K.b12); else complain(__LINE__);
	iequals(__LINE__, K.b12 && ppH->pH->ui, 1);
	iequals(__LINE__, ppH->pH->ui && K.b12, 1);
	if (K.b12 && ppH->pH->ui); else complain(__LINE__);
	if (ppH->pH->ui && K.b12); else complain(__LINE__);
	iequals(__LINE__, K.b12 && ppH->pH->l, 1);
	iequals(__LINE__, ppH->pH->l && K.b12, 1);
	if (K.b12 && ppH->pH->l); else complain(__LINE__);
	if (ppH->pH->l && K.b12); else complain(__LINE__);
	iequals(__LINE__, K.b12 && ppH->pH->ul, 1);
	iequals(__LINE__, ppH->pH->ul && K.b12, 1);
	if (K.b12 && ppH->pH->ul); else complain(__LINE__);
	if (ppH->pH->ul && K.b12); else complain(__LINE__);
	iequals(__LINE__, K.b12 && ppH->pH->f, 1);
	iequals(__LINE__, ppH->pH->f && K.b12, 1);
	if (K.b12 && ppH->pH->f); else complain(__LINE__);
	if (ppH->pH->f && K.b12); else complain(__LINE__);
	iequals(__LINE__, K.b12 && ppH->pH->d, 1);
	iequals(__LINE__, ppH->pH->d && K.b12, 1);
	if (K.b12 && ppH->pH->d); else complain(__LINE__);
	if (ppH->pH->d && K.b12); else complain(__LINE__);
#if ANSI
	iequals(__LINE__, K.b12 && ppH->pH->ld, 1);
	iequals(__LINE__, ppH->pH->ld && K.b12, 1);
	if (K.b12 && ppH->pH->ld); else complain(__LINE__);
	if (ppH->pH->ld && K.b12); else complain(__LINE__);
#endif
	}}
	return 	report(Filename);
	}
