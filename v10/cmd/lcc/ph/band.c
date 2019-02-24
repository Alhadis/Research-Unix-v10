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
#include "n.h"
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
	CHAR *Nc();
#if ANSI
	SCHAR *Nsc();
#endif
	SHORT *Ns();
	INT *Ni();
	UCHAR *Nuc();
	USHORT *Nus();
	UINT *Nui();
	LONG *Nl();
	ULONG *Nul();
	FLOAT *Nf();
	DOUBLE *Nd();
#if ANSI
	LDOUBLE *Nld();
#endif
	{
	int true = 1, false = 0;
	Filename =  " auto pstruct1 auto funcrp band ";
	iequals(__LINE__, pG->c & *Nc(), 7);
	iequals(__LINE__, *Nc() & pG->c, 7);
#if ANSI
	iequals(__LINE__, pG->c & *Nsc(), 0);
	iequals(__LINE__, *Nsc() & pG->c, 0);
#endif
	iequals(__LINE__, pG->c & *Ns(), 1);
	iequals(__LINE__, *Ns() & pG->c, 1);
	iequals(__LINE__, pG->c & *Ni(), 2);
	iequals(__LINE__, *Ni() & pG->c, 2);
	iequals(__LINE__, pG->c & *Nuc(), 3);
	iequals(__LINE__, *Nuc() & pG->c, 3);
	iequals(__LINE__, pG->c & *Nus(), 4);
	iequals(__LINE__, *Nus() & pG->c, 4);
	iequals(__LINE__, pG->c & *Nui(), 5);
	iequals(__LINE__, *Nui() & pG->c, 5);
	lequals(__LINE__, pG->c & *Nl(), 6L);
	lequals(__LINE__, *Nl() & pG->c, 6L);
	lequals(__LINE__, pG->c & *Nul(), 7L);
	lequals(__LINE__, *Nul() & pG->c, 7L);
#if ANSI
	iequals(__LINE__, pG->sc & *Nc(), 0);
	iequals(__LINE__, *Nc() & pG->sc, 0);
#endif
#if ANSI
	iequals(__LINE__, pG->sc & *Nsc(), 8);
	iequals(__LINE__, *Nsc() & pG->sc, 8);
#endif
#if ANSI
	iequals(__LINE__, pG->sc & *Ns(), 8);
	iequals(__LINE__, *Ns() & pG->sc, 8);
#endif
#if ANSI
	iequals(__LINE__, pG->sc & *Ni(), 8);
	iequals(__LINE__, *Ni() & pG->sc, 8);
#endif
#if ANSI
	iequals(__LINE__, pG->sc & *Nuc(), 8);
	iequals(__LINE__, *Nuc() & pG->sc, 8);
#endif
#if ANSI
	iequals(__LINE__, pG->sc & *Nus(), 8);
	iequals(__LINE__, *Nus() & pG->sc, 8);
#endif
#if ANSI
	iequals(__LINE__, pG->sc & *Nui(), 8);
	iequals(__LINE__, *Nui() & pG->sc, 8);
#endif
#if ANSI
	lequals(__LINE__, pG->sc & *Nl(), 8L);
	lequals(__LINE__, *Nl() & pG->sc, 8L);
#endif
#if ANSI
	lequals(__LINE__, pG->sc & *Nul(), 8L);
	lequals(__LINE__, *Nul() & pG->sc, 8L);
#endif
	iequals(__LINE__, pG->s & *Nc(), 1);
	iequals(__LINE__, *Nc() & pG->s, 1);
#if ANSI
	iequals(__LINE__, pG->s & *Nsc(), 8);
	iequals(__LINE__, *Nsc() & pG->s, 8);
#endif
	iequals(__LINE__, pG->s & *Ns(), 9);
	iequals(__LINE__, *Ns() & pG->s, 9);
	iequals(__LINE__, pG->s & *Ni(), 8);
	iequals(__LINE__, *Ni() & pG->s, 8);
	iequals(__LINE__, pG->s & *Nuc(), 9);
	iequals(__LINE__, *Nuc() & pG->s, 9);
	iequals(__LINE__, pG->s & *Nus(), 8);
	iequals(__LINE__, *Nus() & pG->s, 8);
	iequals(__LINE__, pG->s & *Nui(), 9);
	iequals(__LINE__, *Nui() & pG->s, 9);
	lequals(__LINE__, pG->s & *Nl(), 8L);
	lequals(__LINE__, *Nl() & pG->s, 8L);
	lequals(__LINE__, pG->s & *Nul(), 9L);
	lequals(__LINE__, *Nul() & pG->s, 9L);
	iequals(__LINE__, pG->i & *Nc(), 2);
	iequals(__LINE__, *Nc() & pG->i, 2);
#if ANSI
	iequals(__LINE__, pG->i & *Nsc(), 8);
	iequals(__LINE__, *Nsc() & pG->i, 8);
#endif
	iequals(__LINE__, pG->i & *Ns(), 8);
	iequals(__LINE__, *Ns() & pG->i, 8);
	iequals(__LINE__, pG->i & *Ni(), 10);
	iequals(__LINE__, *Ni() & pG->i, 10);
	iequals(__LINE__, pG->i & *Nuc(), 10);
	iequals(__LINE__, *Nuc() & pG->i, 10);
	iequals(__LINE__, pG->i & *Nus(), 8);
	iequals(__LINE__, *Nus() & pG->i, 8);
	iequals(__LINE__, pG->i & *Nui(), 8);
	iequals(__LINE__, *Nui() & pG->i, 8);
	lequals(__LINE__, pG->i & *Nl(), 10L);
	lequals(__LINE__, *Nl() & pG->i, 10L);
	lequals(__LINE__, pG->i & *Nul(), 10L);
	lequals(__LINE__, *Nul() & pG->i, 10L);
	iequals(__LINE__, pG->uc & *Nc(), 3);
	iequals(__LINE__, *Nc() & pG->uc, 3);
#if ANSI
	iequals(__LINE__, pG->uc & *Nsc(), 8);
	iequals(__LINE__, *Nsc() & pG->uc, 8);
#endif
	iequals(__LINE__, pG->uc & *Ns(), 9);
	iequals(__LINE__, *Ns() & pG->uc, 9);
	iequals(__LINE__, pG->uc & *Ni(), 10);
	iequals(__LINE__, *Ni() & pG->uc, 10);
	iequals(__LINE__, pG->uc & *Nuc(), 11);
	iequals(__LINE__, *Nuc() & pG->uc, 11);
	iequals(__LINE__, pG->uc & *Nus(), 8);
	iequals(__LINE__, *Nus() & pG->uc, 8);
	iequals(__LINE__, pG->uc & *Nui(), 9);
	iequals(__LINE__, *Nui() & pG->uc, 9);
	lequals(__LINE__, pG->uc & *Nl(), 10L);
	lequals(__LINE__, *Nl() & pG->uc, 10L);
	lequals(__LINE__, pG->uc & *Nul(), 11L);
	lequals(__LINE__, *Nul() & pG->uc, 11L);
	iequals(__LINE__, pG->us & *Nc(), 4);
	iequals(__LINE__, *Nc() & pG->us, 4);
#if ANSI
	iequals(__LINE__, pG->us & *Nsc(), 8);
	iequals(__LINE__, *Nsc() & pG->us, 8);
#endif
	iequals(__LINE__, pG->us & *Ns(), 8);
	iequals(__LINE__, *Ns() & pG->us, 8);
	iequals(__LINE__, pG->us & *Ni(), 8);
	iequals(__LINE__, *Ni() & pG->us, 8);
	iequals(__LINE__, pG->us & *Nuc(), 8);
	iequals(__LINE__, *Nuc() & pG->us, 8);
	iequals(__LINE__, pG->us & *Nus(), 12);
	iequals(__LINE__, *Nus() & pG->us, 12);
	iequals(__LINE__, pG->us & *Nui(), 12);
	iequals(__LINE__, *Nui() & pG->us, 12);
	lequals(__LINE__, pG->us & *Nl(), 12L);
	lequals(__LINE__, *Nl() & pG->us, 12L);
	lequals(__LINE__, pG->us & *Nul(), 12L);
	lequals(__LINE__, *Nul() & pG->us, 12L);
	iequals(__LINE__, pG->ui & *Nc(), 5);
	iequals(__LINE__, *Nc() & pG->ui, 5);
#if ANSI
	iequals(__LINE__, pG->ui & *Nsc(), 8);
	iequals(__LINE__, *Nsc() & pG->ui, 8);
#endif
	iequals(__LINE__, pG->ui & *Ns(), 9);
	iequals(__LINE__, *Ns() & pG->ui, 9);
	iequals(__LINE__, pG->ui & *Ni(), 8);
	iequals(__LINE__, *Ni() & pG->ui, 8);
	iequals(__LINE__, pG->ui & *Nuc(), 9);
	iequals(__LINE__, *Nuc() & pG->ui, 9);
	iequals(__LINE__, pG->ui & *Nus(), 12);
	iequals(__LINE__, *Nus() & pG->ui, 12);
	iequals(__LINE__, pG->ui & *Nui(), 13);
	iequals(__LINE__, *Nui() & pG->ui, 13);
	lequals(__LINE__, pG->ui & *Nl(), 12L);
	lequals(__LINE__, *Nl() & pG->ui, 12L);
	lequals(__LINE__, pG->ui & *Nul(), 13L);
	lequals(__LINE__, *Nul() & pG->ui, 13L);
	lequals(__LINE__, pG->l & *Nc(), 6L);
	lequals(__LINE__, *Nc() & pG->l, 6L);
#if ANSI
	lequals(__LINE__, pG->l & *Nsc(), 8L);
	lequals(__LINE__, *Nsc() & pG->l, 8L);
#endif
	lequals(__LINE__, pG->l & *Ns(), 8L);
	lequals(__LINE__, *Ns() & pG->l, 8L);
	lequals(__LINE__, pG->l & *Ni(), 10L);
	lequals(__LINE__, *Ni() & pG->l, 10L);
	lequals(__LINE__, pG->l & *Nuc(), 10L);
	lequals(__LINE__, *Nuc() & pG->l, 10L);
	lequals(__LINE__, pG->l & *Nus(), 12L);
	lequals(__LINE__, *Nus() & pG->l, 12L);
	lequals(__LINE__, pG->l & *Nui(), 12L);
	lequals(__LINE__, *Nui() & pG->l, 12L);
	lequals(__LINE__, pG->l & *Nl(), 14L);
	lequals(__LINE__, *Nl() & pG->l, 14L);
	lequals(__LINE__, pG->l & *Nul(), 14L);
	lequals(__LINE__, *Nul() & pG->l, 14L);
	lequals(__LINE__, pG->ul & *Nc(), 7L);
	lequals(__LINE__, *Nc() & pG->ul, 7L);
#if ANSI
	lequals(__LINE__, pG->ul & *Nsc(), 8L);
	lequals(__LINE__, *Nsc() & pG->ul, 8L);
#endif
	lequals(__LINE__, pG->ul & *Ns(), 9L);
	lequals(__LINE__, *Ns() & pG->ul, 9L);
	lequals(__LINE__, pG->ul & *Ni(), 10L);
	lequals(__LINE__, *Ni() & pG->ul, 10L);
	lequals(__LINE__, pG->ul & *Nuc(), 11L);
	lequals(__LINE__, *Nuc() & pG->ul, 11L);
	lequals(__LINE__, pG->ul & *Nus(), 12L);
	lequals(__LINE__, *Nus() & pG->ul, 12L);
	lequals(__LINE__, pG->ul & *Nui(), 13L);
	lequals(__LINE__, *Nui() & pG->ul, 13L);
	lequals(__LINE__, pG->ul & *Nl(), 14L);
	lequals(__LINE__, *Nl() & pG->ul, 14L);
	lequals(__LINE__, pG->ul & *Nul(), 15L);
	lequals(__LINE__, *Nul() & pG->ul, 15L);
	}}
	return 	report(Filename);
	}
