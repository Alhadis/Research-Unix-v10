/* ident "@(#)ctrans:src/print_self.h	1.2" */
/* print_self.H -- base class for objects that want to support
   the 'official' print self for \T in printf. */

/*
    $Header: /usr2/odi/include/RCS/print_self.H,v 1.4 89/09/19 11:49:19 benson Exp $

    Copyright (c) 1989 by Object Design, Inc., Burlington, Mass.
    All rights reserved.

*/

#ifndef _PRINT_SELF_CLASS
#define _PRINT_SELF_CLASS
#include <iostream.h>

class _Print_self {
  public:
    virtual char * type_name () const { return "OBJECT"; }
    virtual int print_self (ostream&) const; // returns count of characters printed.
    virtual int format_self (ostream&) const; // returns count of characters printed.
};
#endif
