/* C compiler: SPARC code generator
 *
 * Outputs `stab' entries for the UNIX dbx debugger,
 * if -g is specified.
 */
#include "c.h"
#ifdef sparc
#include </usr/include/stab.h>
#include "stabbsd.c"
#else
#include "stabstub.c"
#endif
