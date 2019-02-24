/* print_self_default.C -- bailout versions of print_self and format_self */
/*
	$Source: /usr2/odi/lib.src/RCS/print_self_default.C,v $ $RCSfile: print_self_default.C,v $
	$Revision: 1.4 $		$Date: 89/09/19 11:49:58 $
	$Author: benson $		$Locker:  $
	$State: Exp $
*/

#include <print_self.H>

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

static char rcsinfo[] = "$Header: /usr2/odi/lib.src/RCS/print_self_default.C,v 1.4 89/09/19 11:49:58 benson Exp $";


/*
   $Log:	print_self_default.C,v $
Revision 1.4  89/09/19  11:49:58  benson
rename print_self_class to _Print_self.

Revision 1.3  89/09/19  11:41:40  benson
add consts.

Revision 1.2  89/09/18  15:28:21  benson
cosmetic improvements.


   end_log
*/
