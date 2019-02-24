

/*
 *
 * Reads the raster files listed as arguments and prints the glyphs, along
 * with information about character widths and glyph numbers, on an Imagen
 * printer. The output is written in Impress. If no glyphs are selected by
 * using the -o option the entire file or files will be printed. An Impress
 * document header is the first thing written to the output file provided
 * want_header == TRUE. Use the -H option to force the header. The target
 * printer is assumed to have a resolution of res dots per inch, which by
 * default is set to RAST_RES (probably 300). If that's not the case use
 * the -r option to select a new resolution. It's really only important
 * in determining the pixel length of a page.
 *
 * Text in the printout is written using font *font in size list_size.
 * By default it's resident font cour10. You can change the requested font
 * and size using the -f and -s options respectively.
 *
 * All the raster files are assumed to be in directory *rastdir, which by
 * default is set to ".". *rastdir can be changed by using the -R option.
 * but in all cases the string "*rastdir/" is prepended to the raster file
 * names to get the complete pathname.
 *
 * As an example the command line,
 *
 *	printrast -R /usr/lib/raster/rasti300 -H -r 300 R.10 I.10 >xxx
 *
 * will write the Impress commands needed to print the glyphs in raster
 * file /usr/lib/raster/rasti300/R.10 and /usr/lib/raster/rasti300/I.10 out
 * to file xxx. The target printer is assumed to have a resolution of 300
 * dots per inch. The output file xxx begins with a document header defining
 * the language to be Impress and can therefore be set directly to an 8/300.
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
#include "impcodes.h"			/* Impress opcode definitions */


FILE	*fp_out = stdout;		/* output file - probably won't change */

char	*font = "cour";			/* use this font for printing text */
int	list_size = 10;			/* and this point size */

float	xoffset = .25;			/* start this far from left edge */
float	yoffset = .50;			/* and this far down the page - inches */

int	p_xoffset;			/* same as last two but in pixels */
int	p_yoffset;

float	height = 10.50;			/* height of the paper in inches */
float	width = 8.75;			/* same but for paper width */

int	p_height;			/* paper dimensions in pixels */
int	p_width;

int	res = RAST_RES;			/* resolution of the target printer */

int	hpos;				/* current horizontal position - pixels */
int	vpos;				/* same but for vertical position */

int	linesp;				/* interline spacing */
int	spacewid;			/* interword spacing */

int	want_header = FALSE;		/* write document header first */


/*****************************************************************************/


main(agc, agv)


    int		agc;
    char	*agv[];


{


/*
 *
 * Reads Imagen formatted raster files and writes the Impress commands
 * needed to print the glyphs, along with width and index data, to fp_out.
 * Text is written using resident the resident font whose name is built
 * by concatenating *font and list_size. By default the font is cour10,
 * but both font and list_size can be changed by options. Individual glyphs
 * in the raster files can be selected, rather than the whole file, by
 * using the -o option (although I haven't tested this yet).
 *
 */


    argc = agc;				/* other routines may want them */
    argv = agv;

    prog_name = argv[0];		/* really just for error messages */
    rastdir = ".";			/* current directory by default */

    options();				/* first get command line options */
    doc_header();			/* make sure this comes first */
    initialize();			/* must be done after options */
    arguments();			/* then process non-option arguments */

    exit(x_stat);			/* everything probably went OK */

}   /* End of main */


/*****************************************************************************/


options()


{


    int		ch;			/* return value from getopt() */
    char	*names = "DIFHR:o:x:y:r:h:w:s:";

    extern char	*optarg;		/* used by getopt() */
    extern int	optind;

    float	atof();


/*
 *
 * Reads and processes the command line options. Right now the recognized
 * options are,
 *
 */


    while ( (ch = getopt(argc, argv, names)) != EOF )  {

	switch ( ch )  {

	    case 'r':			/* target printer resolution */
		    res = atoi(optarg);
		    break;

	    case 'f':			/* use this font (omit size) */
		    font = optarg;
		    break;

	    case 's':			/* use this point size */
		    list_size = atoi(optarg);
		    break;

	    case 'h':			/* set the paper height - inches */
		    height = atof(optarg);
		    break;

	    case 'w':			/* same but for the width */
		    width = atof(optarg);
		    break;

	    case 'x':			/* set the xoffset - inches */
		    xoffset = atof(optarg);
		    break;

	    case 'y':			/* same but for yoffset */
		    yoffset = atof(optarg);
		    break;

	    case 'o':			/* processing range list */
		    out_list(optarg);
		    break;

	    case 'H':			/* want the document header */
		    want_header = TRUE;
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


doc_header()


{


/*
 *
 * If want_header is TRUE we'll make sure a document header is the first
 * thing written to the output file.
 *
 */


    if ( want_header == TRUE )
	fprintf(fp_out, "@document(language impress)");

}   /* End of doc_header */


/*****************************************************************************/


initialize()


{


/*
 *
 * Does the run time initialization of variables that depend on the target
 * printer resolution. Also handles printer initialization of things like
 * the space width and creating the listing font family table.
 *
 */


    p_xoffset = xoffset * res;
    p_yoffset = yoffset * res;

    p_height = height * res;
    p_width = width * res;

    spacewid = 2.2 * list_size;		/* a reasonable value for spaces */

    putc(ASETSP, fp_out);
    putint(spacewid, fp_out);

    putc(ACFT, fp_out);			/* use resident font */
    putc(MAXFAMILY, fp_out);
    putc(1, fp_out);
    putc(0, fp_out);
    fprintf(fp_out, "%s%d", font, list_size);
    putc('\0', fp_out);

}   /* End of initialize */


/*****************************************************************************/


arguments()


{


    char	name[100];		/* make sure we can acces file */
    char	*font;			/* name of the font we're dumping */
    char	*size;			/* in this point size */

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
		error(FATAL, "bad raster file name %s", *argv);

	    if ( *(size = font + strlen(font) + 1) == 'r' ) size++;

	    printrast(font, atoi(size));
	    argc--;
	    argv++;
	}   /* End while */
    }   /* End else */

}   /* End of arguments */


/*****************************************************************************/


printrast(name, size)


    char	*name;			/* name of the font */
    int		size;			/* in this point size */


{


    int		start, stop;		/* glyph number end points */
    int		i;			/* loop index */
    int		cwid;			/* character width */
    int		xref, yref;		/* reference point for current glyph */
    char	buff[200];		/* text string buffer */


/*
 *
 * Prints glyphs from the raster file associated with font *name in point
 * size 'size'. If no -o option was given all the glyphs will be printed.
 * The listing for each new font begins on a new page.
 *
 */


    getrastdata(name, size);		/* read the font's raster file */

    start = getvalue(P_FIRSTGLY);	/* first glyph number in the font */
    stop = getvalue(P_LASTGLY);		/* number of the last glyph */

    linesp = ((MAX(size, list_size) + .2 * MAX(size, list_size)) / 72.27) * RAST_RES + 1.0;

    newpage();				/* dump starts on a new page */

    sprintf(buff, "FONT DUMP FOR %s:   SIZE %d\n\n\n", name, size);
    printstring(buff);

    for ( i = start; i <= stop; i++ )
	if ( in_olist(i) == ON )  {
	    printchar(i, name, size, -1, fp_out);
	    cwid = PIXEL_WIDTH(getvalue(G_CHWIDTH, i), rast_res);
	    xref = getvalue(G_XREF, i);
	    yref = getvalue(G_YREF, i);
	    hpos = 6 * spacewid + p_xoffset;
	    xymove();
	    sprintf(buff, "width = %d   code = %d    xref = %d    yref = %d\n", cwid, i, xref, yref);
	    printstring(buff);
	}   /* End if */

}   /* End of printrast */


/*****************************************************************************/


printchar(c, font, size, advance, fp)


    int		c;			/* print glyph for this character */
    char	*font;			/* in this font */
    int		size;			/* and this point size */
    int		advance;		/* using this as the advance width */
    FILE	*fp;			/* Impress commands go here */


{


/*
 *
 * Does everything needed to get character c printed using the raster file
 * for *font in point size 'size' and using advance as the character advance.
 *
 */


    if ( fam == NULL || fam->size != size || strcmp(font, fam->name) != 0 || fam->rst == NULL )
	getrastdata(font, size);

    putc(download(c, advance, ROT_0, fp), fp);

}   /* End of printchar */


/*****************************************************************************/


newline()


{


/*
 *
 * Makes sure everything is properly set up to start a new line. If the
 * vertical position on the page has gone past p_height (ie. the height
 * of the paper in pixels) we'll start a new page.
 *
 */


    hpos = p_xoffset;			/* left margin */
    if ( (vpos += linesp) >= p_height )
	 newpage();
    else xymove();

}   /* End of newline */


/*****************************************************************************/


newpage()


{


/*
 *
 * Ends the previous page and makes sure we're positioned at the upper left
 * corner of a new page. The way things are done results in a blank page at
 * the beginning of every job - no big deal because the program isn't used
 * much.
 *
 */


    putc(AENDP, fp_out);

    hpos = p_xoffset;
    vpos = p_yoffset;

    xymove();

}   /* End of newpage */


/*****************************************************************************/


xymove()


{


/*
 *
 * Makes sure the current position for Impress is set to (hpos, vpos).
 *
 */


    putc(ASETAH, fp_out);
    putint(hpos, fp_out);

    putc(ASETAV, fp_out);
    putint(vpos, fp_out);


}   /* End of xymove */


/*****************************************************************************/


printstring(str)


    char	*str;			/* want to print this string */


{


/*
 *
 * Arranges to have string *str printed using the resident font that's been
 * assigned the family number MAXFAMILY. Spaces and newlines are handled
 * separately, although that probably isn't necessary for resident fonts.
 *
 * Some of the glyphs in the raster files we got from Imagen are bad. It
 * didn't seem to matter much for version 1.9, but a glyph having 0 for its
 * raster height hung version 2.0 and nothing was printed. Anyway I put a
 * check in routine download() to make sure glyphs having no height weren't
 * downloaded. At least that made sure the printer didn't crash. All we
 * get for those glyphs is a harmless undefined glyph message and a
 * special character got printed to mark where the problem occurred.
 *
 */


    if ( last_fam != MAXFAMILY )  {	/* using this family for listing */
	putc(ASF, fp_out);		/* need to tell printer about it */
	putc(MAXFAMILY, fp_out);
	last_fam = MAXFAMILY;
    }	/* End if */

    while ( *str != '\0' )  {		/* print the whole string */
	if ( *str == ' ' )
	     putc(ASP, fp_out);
	else if ( *str == '\n' )
	     newline();
	else putc(*str, fp_out);
	str++;
    }   /* End while */

}   /* End of printstring */


/*****************************************************************************/


