/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * universe common data
 */

#include "univlib.h"

#ifndef UNIV_MAX

char		univ_env[] = "__UNIVERSE__";

#else

#ifndef NUMUNIV

char*		univ_name[] = { "att", "ucb" };

int		univ_max = sizeof(univ_name) / sizeof(univ_name[0]);

#endif

char		univ_cond[] = "$(UNIVERSE)";

int		univ_size = sizeof(univ_cond) - 1;

#endif
