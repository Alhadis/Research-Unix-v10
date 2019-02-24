/* C compiler: UNIX code generator
 *
 * Outputs `stab' entries for the UNIX dbx debugger,
 * if -g is specified.
 */
#include "c.h"
#if mc68020 || mc68030 || NeXT
#include <stab.h>
#include "stabbsd.c"
#else
#include "stabstub.c"
#endif
