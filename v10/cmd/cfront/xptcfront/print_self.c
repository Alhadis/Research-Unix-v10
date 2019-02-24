/* ident "@(#)ctrans:src/print_self.c	1.2" */
/* print_self_default.C -- bailout versions of print_self and format_self */
/*
	$Source: /usr2/odi/lib.src/RCS/print_self_default.C,v $ $RCSfile: print_self_default.C,v $
	$Revision: 1.4 $		$Date: 89/09/19 11:49:58 $
	$Author: benson $		$Locker:  $
	$State: Exp $
*/

#include "print_self.h"

int ostream_printf (ostream&, const char * ...);

int _Print_self::print_self (ostream& stream) const
{
    return ostream_printf (stream, "<%s at 0x%p>", 
			   this->type_name(), (void *) this);
}

int _Print_self::format_self (ostream& stream) const
{
    return ostream_printf (stream, "<%s at 0x%p>", 
			   this->type_name(), (void *) this);
}
