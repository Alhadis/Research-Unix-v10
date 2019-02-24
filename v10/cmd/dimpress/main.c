

/*
 *
 * Template for normal C programs. All the code was written with standard
 * tab settings, ie. every eighth column. Options are processed in routine
 * options() using getopt(). The non-option arguments are handled in
 * arguments(). To add a new option add its name to string *name and an
 * appropriate case to the switch in options() (see getopt in section 3
 * of the UNIX manual). You'll also need to add a bunch of code to handle
 * the non-option arguments.
 *
 */


#include <stdio.h>

#include "gen.h"			/* general purpose definitions */
#include "ext.h"			/* external variable declarations */


/*****************************************************************************/


main(agc, agv)


    int		agc;
    char	*agv[];


{


/*
 *
 *
 */


    argc = agc;				/* other routines may want them */
    argv = agv;

    prog_name = argv[0];		/* really just for error messages */

    options();				/* first get command line options */
    arguments();			/* then process non-option arguments */

    exit(x_stat);			/* everything probably went OK */

}   /* End of main */


/*****************************************************************************/


options()


{


    int		ch;			/* return value from getopt() */
    char	*names = "DIF:R:o:";

    extern char	*optarg;		/* used by getopt() */
    extern int	optind;


/*
 *
 * Reads and processes the command line options. Right now the recognized
 * options are,
 *
 */


    while ( (ch = getopt(argc, argv, names)) != EOF )  {

	switch ( ch )  {

	    case 'o':			/* processing range list */
		    out_list(optarg);
		    break;

	    case 'F':			/* new troff font directory */
		    fontdir = optarg;
		    break;

	    case 'R':			/* set raster table directory */
		    rastdir = optarg;
		    break;

	    case 'D':			/* debug flag */
		    debug = ON;
		    break;

	    case 'I':			/* ignore FATAL errors */
		    ignore = ON;
		    break;

	    case '?':			/* don't understand the option */
		    error(FATAL, "");
		    break;

	    default:			/* don't know what to do for ch */
		    error(FATAL, "missing case for option %c\n", ch);
		    break;

	}   /* End switch */

    }   /* End while */

    argc -= optind;			/* get ready for non-option args */
    argv += optind;

}   /* End of options */


/*****************************************************************************/


arguments()


{


    char	*file;
    int		size;
    int		i;


/*
 *
 * Makes sure all the non-option command line arguments are processed.
 *
 */


    if ( argc < 1 )
	printf("No non-option arguments\n");
    else  {				/* have at least one argument */
	while ( argc > 0 )  {
	    file = *argv++;
	    argc--;
	    size = atoi(*argv);
	    fprintf(stderr, "Get raster file for %s.r%d\n", file, size);
	    getrastdata(file, size);
	    for ( i = 0; i < 127; i++ ) dump_glyph(i);
	    argc--;
	    argv++;
	}   /* End while */
    }   /* End else */

}   /* End of arguments */


/*****************************************************************************/


