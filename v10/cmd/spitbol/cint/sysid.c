/*
/   sysid - return system id
*/

#include "osint.h"
#include "spitblks.h"

extern struct scblk	id1;
extern struct scblk	id2;

sysid()

{
	SET_XR( &id1 );
	SET_XL( &id2 );
	return NORMAL_RETURN;
}
