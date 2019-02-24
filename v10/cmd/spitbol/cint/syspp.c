/*
/   syspp - obtain print parameters
*/

#include "osint.h"
#include "spitblks.h"

extern WORD	lnsppage;
extern WORD	pagewdth;
extern WORD	spitflag;

syspp()

{
	/*
	/   Set page width, lines per page, and compiler flags.
	*/
	SET_WA( pagewdth );
	SET_WB( lnsppage );
	SET_WC( spitflag );

	return NORMAL_RETURN;
}
