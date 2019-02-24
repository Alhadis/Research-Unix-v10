/*ident	"@(#)ctrans:src/typedef.h	1.1.1.1" */
/**************************************************************************

        C++ source for cfront, the C++ compiler front-end
        written in the computer science research center of Bell Labs

        Copyright (c) 1984 AT&T, Inc. All Rights Reserved
        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

typedef.h:

*****************************************************************************/

typedef unsigned char TOK;
typedef unsigned char bit;	// sometimes used as bits
typedef class node * PP;
typedef int (*PFI)();
typedef void (*PFV)();
typedef class node * Pnode;
typedef struct key * Pkey;
typedef class name * Pname;
typedef class basetype * Pbase;
typedef class basecl* Pbcl;
typedef class type * Ptype;
typedef class fct  * Pfct;
typedef class field * Pfield;
typedef class expr * Pexpr;
typedef class qexpr * Pqexpr;
typedef class texpr * Ptexpr;
typedef class classdef * Pclass;
typedef class enumdef * Penum;
typedef class stmt * Pstmt;
typedef class estmt * Pestmt;
typedef class tstmt * Ptstmt;
typedef class vec * Pvec;
typedef class ptr * Pptr;
typedef class block * Pblock;
typedef class table * Ptable;
typedef class loc Loc;
typedef class call * Pcall;
typedef class gen* Pgen;
typedef class ref * Pref;
typedef class name_list * Plist;
typedef class iline * Pin;
typedef class nlist * Pnlist;
typedef class slist * Pslist;
typedef class elist * Pelist;
typedef class virt * Pvirt;
typedef char* Pchar;

