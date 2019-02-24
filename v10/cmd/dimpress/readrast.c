

/*
 *
 * Reads raster files in Imagen's format and dumps all the glyphs, or
 * just the ones selected by the -o option, to stdout. The non-option
 * arguments are the names of the raster files that you want to display.
 * For example to dump glyphs 12, 23, and 24 in raster file cmasc10.r10
 * just type,
 *
 *                  readrast -o12,23,24 cmasc10.r10
 *
 * The raster files are all assumed to be in directory *rastdir, which by
 * default is set to ".", but it can be changed by using the -R option.
 *
 * A word of warning is in order here. Because I've written readrastfile()
 * so it accepts two different raster file naming conventions there isn't
 * a one to one mapping of file names to font name, size pairs. That means
 * if you have two similiarly named raster files, differing only in the
 * naming convention used, and you keep them in the same directory you'll
 * always display glyphs from the preferred file no matter which file
 * you list on the command line. I don't think it will cause any real
 * problems so I'm not going to change any of this stuff.
 *
 */


#include <stdio.h>

#include "gen.h"			/* general purpose definitions */
#include "ext.h"			/* external variable declarations */
#include "rast.h"			/* raster file definitions */


/*****************************************************************************/


main(agc, agv)


    int		agc;
    char	*agv[];


{


/*
 *
 * As I mentioned before this program is used to display glyph bitmaps
 * from Imagen formatted raster files on stdout. Use the -o option to
 * select one or more glyphs.
 *
 */


    argc = agc;				/* other routines may want them */
    argv = agv;

    prog_name = argv[0];		/* really just for error messages */
    rastdir = ".";			/* current directory by default */

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


    char	name[100];		/* raster file pathname */
    char	*font;			/* includes up to the '.' in *argv */
    char	*size;			/* number after '.' and possibly 'r' */
    int		i;			/* just a loop index */

    char	*strtok();


/*
 *
 * All the remaining arguments are the names of raster files in *rastdir
 * that we want to display. Because two raster file naming conventions
 * are used in rast.c we can't be absolutely sure we're really displaying
 * the raster file that the user wanted. That's why the pathname is built
 * up in name[] and access to the file is checked. That's probably good
 * enough for our purposes, but it still doesn't guarantee you get what
 * you asked for. It shouldn't cause any real problems as long as you
 * don't try and keep raster files with the two different naming conventions
 * in the same directory.
 *
 */


    if ( argc < 1 )
	error(FATAL, "can't read standard input");
    else  {				/* have at least one argument */
	while ( argc > 0 )  {
	    sprintf(name, "%s/%s", rastdir, *argv);
	    if ( access(name, 04) == -1 )
		error(FATAL, "can't read file %s", name);

	    if ( (font = strtok(*argv, ".")) == NULL )
		error(FATAL, "Bad raster file name %s", *argv);

	    if ( *(size = font + strlen(font) + 1) == 'r' ) size++;

	    fprintf(stdout, "Get raster file for font %s, size %s\n", font, size);
	    getrastdata(font, atoi(size));

	    for ( i = fam->first; i <= fam->last; i++ )
		if ( in_olist(i) == ON )
		    dump_glyph(i, stdout);

	    argc--;
	    argv++;
	}   /* End while */
    }   /* End else */

}   /* End of arguments */


/*****************************************************************************/


