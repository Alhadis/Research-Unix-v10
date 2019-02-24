/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

CHAR M_c = 0;
CHAR Mc(){ int i; if (M_c){i=M_c;M_c=0;} else i=7; return (CHAR)i;}
#if ANSI
SCHAR M_sc = 0;
SCHAR Msc(){ int i; if (M_sc){i=M_sc;M_sc=0;} else i=8; return (SCHAR)i;}
#endif
SHORT M_s = 0;
SHORT Ms(){ int i; if (M_s){i=M_s;M_s=0;} else i=9; return (SHORT)i;}
INT M_i = 0;
INT Mi(){ int i; if (M_i){i=M_i;M_i=0;} else i=10; return (INT)i;}
UCHAR M_uc = 0;
UCHAR Muc(){ int i; if (M_uc){i=M_uc;M_uc=0;} else i=11; return (UCHAR)i;}
USHORT M_us = 0;
USHORT Mus(){ int i; if (M_us){i=M_us;M_us=0;} else i=12; return (USHORT)i;}
UINT M_ui = 0;
UINT Mui(){ int i; if (M_ui){i=M_ui;M_ui=0;} else i=13; return (UINT)i;}
LONG M_l = 0;
LONG Ml(){ int i; if (M_l){i=M_l;M_l=0;} else i=14; return (LONG)i;}
ULONG M_ul = 0;
ULONG Mul(){ int i; if (M_ul){i=M_ul;M_ul=0;} else i=15; return (ULONG)i;}
FLOAT M_f = 0;
FLOAT Mf(){ int i; if (M_f){i=M_f;M_f=0;} else i=16; return (FLOAT)i;}
DOUBLE M_d = 0;
DOUBLE Md(){ int i; if (M_d){i=M_d;M_d=0;} else i=17; return (DOUBLE)i;}
#if ANSI
LDOUBLE M_ld = 0;
LDOUBLE Mld(){ int i; if (M_ld){i=M_ld;M_ld=0;} else i=18; return (LDOUBLE)i;}
#endif
