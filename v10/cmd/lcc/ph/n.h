/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

CHAR *Nc(){ static CHAR i = 7; return &i;}
#if ANSI
SCHAR *Nsc(){ static SCHAR i = 8; return &i;}
#endif
SHORT *Ns(){ static SHORT i = 9; return &i;}
INT *Ni(){ static INT i = 10; return &i;}
UCHAR *Nuc(){ static UCHAR i = 11; return &i;}
USHORT *Nus(){ static USHORT i = 12; return &i;}
UINT *Nui(){ static UINT i = 13; return &i;}
LONG *Nl(){ static LONG i = 14; return &i;}
ULONG *Nul(){ static ULONG i = 15; return &i;}
FLOAT *Nf(){ static FLOAT i = 16; return &i;}
DOUBLE *Nd(){ static DOUBLE i = 17; return &i;}
#if ANSI
LDOUBLE *Nld(){ static LDOUBLE i = 18; return &i;}
#endif
