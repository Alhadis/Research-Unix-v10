#include "playdefs.h"

/* 'error' is used almost nowhere to show fatal error messages and
 *	exit.
 */
error (str)
char	*str;
{
	string(&defont, str, &display, Drect.origin, F_XOR);
	exit (1);
}
