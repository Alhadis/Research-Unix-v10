#include <stdio.h>
#include <sys/types.h>
#include <dir.h>
#include "defs.h"
#include "scanmail.h"

/* global */
struct scanmail sm;

extern char *smnext();

main()
{
    char *p;

    sminit (&sm, NULL, getlogin());
    while (1) {
	    if(p=smnext(&sm))
		fprintf (stderr, "mail from %s\n", p);
	    sleep (5);
    }
}
