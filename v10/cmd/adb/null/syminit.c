#include "defs.h"
#include "sym.h"

/*
 * syminit goes here, to build the symbol table
 * the null version never bothers to call it.
 */

/*
 * compare a name in the symbol table
 * with one the user typed
 * hacks like leading _ and length restrictions go here
 */
eqsym(sp, n)
struct sym *sp;
char *n;
{

	return (strcmp(sp->y_name, n) == 0);
}
