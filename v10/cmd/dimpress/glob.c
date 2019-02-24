
/*
 *
 * Definition and initialization of most of the global variables.
 *
 */


#include <stdio.h>			/* just for definition of NULL */


#include "gen.h"			/* general purpose definitions */
#include "init.h"			/* printer and system definitions */


char	**argv;				/* global so everyone can use them */
int	argc;

char	*prog_name = "";		/* really just for error messages */

int	x_stat = 0;			/* program exit status */
int	debug = OFF;			/* debug flag */
int	ignore = OFF;			/* what we do with FATAL errors */
long	lineno = 0;			/* really just for post-processor */

char	*fontdir = FONTDIR;		/* troff's binary font table directory */
char	*bitdir = NULL;			/* rast* directories found here */
char	*rastdir = ".";			/* raster files are found right here */
char	*resfile = NULL;		/* control file in *bitdir/RESIDENT */

