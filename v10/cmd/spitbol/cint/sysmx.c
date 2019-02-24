/*
/   sysmx - return maximum size in bytes of created object
*/

#include "osint.h"
#include "spitblks.h"

extern WORD	maxsize;

sysmx()

{
	SET_WA( maxsize * 4 );
	return NORMAL_RETURN;
}

