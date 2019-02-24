/* getlogin() on SGI systems is currently unreliable so this routine
	is added to the library to avoid the problem.  WMC
*/

#include <stdio.h>

char *
getlogin()
{
	return NULL;
}
