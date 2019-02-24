#include "term.h"
#include "../protocol.h"

Alloc()
{
	register bytes = RcvShort();
	register char *a = alloc(bytes);

	journal( "Alloc", "%d %x", bytes, a );
	SendLong(a);
}

Free()
{
	register long a = RcvLong();
	journal( "Free", "%x", a );
	free(a);
}

typedef struct Args {
	char filler[100];	/* what size should this be? */
} Args;
typedef long (*Fptr)();
long Returned;
#define SAVE_INST 0x10
	
Call()
{
	register Fptr fptr = (Fptr) RcvLong();
	register Args *aptr = (Args*)RcvLong();
	register char *error;
	journal( "Call", "f=%x a=%x", fptr, aptr );
	if( !(error = ReadErr(fptr,1)) )
		if( *(char*)fptr != SAVE_INST )
			error = "function prolog not found";
		else
			Returned = (*fptr)(*aptr);
	SendLong(&Returned);
	SendError(error);
}
