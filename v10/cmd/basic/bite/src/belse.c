/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	////////////////////////////////////////
 *	////////////////////////////////////////
 *	/// ELSE and ENDIF for STRUCTURED IF ///
 *	//// J. P. Hawkins WH X4610 8C-001 /////
 *	/////// Mon Sep 17 21:28:50 1979 ///////
 *	////////////////////////////////////////
 *	////////////////////////////////////////

 * These statements, like "next" are markers whose codes
 * are searched by an "if" statement with nothing following
 * the "then" to implement a structured "if".
 *
 * These Commands are NOT EXECUTIBLE, and return the appropriate
 * error if an attempt is made to do so.
 */
/*   "@(#) belse.c:  V 1.1  12/21/80" */

#include	"bas.h"
_else()
{
	error(inst.thing.linno, 44); /* ELSE WITH NO IF */
	return(-1);
}

_endif()
{
	error(inst.thing.linno, 45); /* ENDIF WITH NO IF */
	return(-1);
}
