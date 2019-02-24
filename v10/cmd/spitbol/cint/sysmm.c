/*
/   sysmm- get more memory
/
/   Parameters:
/	None
/   Returns:
/	Number of additional words obtained (or 0 if none) in XR.
*/

#include "osint.h"
#include "spitblks.h"

extern WORD	meminc;
extern WORD	memincb;
extern char	*topmem;
extern char	*maxmem;

extern char	*sbrk();

sysmm()

{
	/*
	/    Default situation is that no more memory is available.
	*/
	SET_XR( 0 );

	/*
	/   If not already at maximum allocation, try to get more memory.
	*/
	if ( topmem < maxmem )
	{
	    if ( (long) sbrk( memincb ) != -1 )
	    {
		topmem += memincb;	/*  adjust current top address	*/
		SET_XR( meminc );	/*  set # of words obtained	*/
	    }
	}

	return NORMAL_RETURN;
}

