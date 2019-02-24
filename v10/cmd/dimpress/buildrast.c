

/*
 *
 * Reads specially formatted ASCII files, called control files, and uses
 * them to build raster and font files that can be used by troff and its
 * post-processors. The program reads raster files in Imagen's format from
 * directory *rastdir and uses them, along with the information in the
 * the control files to build new raster files and also font and device
 * files for troff. The ASCII control files must list a target device.
 * The new raster files will be put in directory *nrastdir/"rast"*device
 * and the new font files will go in *fontdir/"dev"*device.
 *
 * Individual fonts or sizes can be selected using the -f and -s options.
 * If they're not used all the font 'build' sections in the ASCII control
 * files will be processed for all the sizes listed under 'sizes'. ASCII
 * files will only be built when the current size is equal to the value
 * set in unitwidth.
 *
 * The raster source, new raster, and new font directories can be selected
 * using options -S, -R, and -F respectively. By default they're all set to
 * ".".
 *
 * As an example I used this program and the RASTi300 file to build the
 * tables in this package. The command line was,
 *
 *
 *	buildrast -S /usr/spool/imagen_src/fonts/raster/300 RASTi300
 *
 *
 * The original raster files that we got from Imagen were kept in directory
 * /usr/spool/imagen_src/fonts/raster/300.
 *
 */


#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>

#include "gen.h"			/* general purpose definitions */
#include "ext.h"			/* external variable declarations */
#include "init.h"			/* printer and system definitions */
#include "rast.h"			/* raster file definitions */
#include "buildrast.h"			/* definitions for building files */


char		*nrastdir = ".";	/* new raster files go here - sort of */

FILE		*fp_in;			/* current ASCII control file */
char		buff[100];		/* buffer for reads from fp_in */

int		dodesc = ON;		/* build troff's DESC file? */
int		gotspecial = FALSE;	/* recorded all special characters */
int		dofont = ON;		/* same but for ASCII font files */

char		device[20] = "";	/* everything's built for this printer */

Charset		charset[256];		/* records all special character names */
char		*fontlist[50];		/* only do stuff for these fonts */
int		sizelist[50];		/* and these point sizes */
int		unitwidth = -1;		/* troff tables built from this size */
int		res = -1;		/* target device's resolution */
char		ligatures[50];		/* ligatures strings stored here */
char		comment[100];		/* comments for each font */
int		internalname = 1;	/* for troff's ASCII font files */
int		isspecial = FALSE;	/* is this a special font */
char		ascender[10];		/* used to get ascender data */
char		spacechar[10];		/* spacewidth = width of this character */
int		spacewidth = -1;	/* it's index in charinfo[] */

int		above;			/* has an ascender if yref > above */
int		below;			/* used to set descender value */

int		charcount = 0;		/* number of chars in charset[] */
int		fontcount = 0;		/* number of entries in fonts[] */
int		sizecount = 0;		/* same but for sizelist[] */

int		first = 1;		/* glyph number endpoints */
int		last = 0;

int		fontoutput;		/* process current font if ON */

char		glyphdir[GLYDIR_SIZE];	/* new raster file glyph directory */
Charinfo	charinfo[MAX_INDEX+1];	/* data about characters in new font */


/*****************************************************************************/


main(agc, agv)


    int		agc;
    char	*agv[];


{


/*
 *
 * Builds raster tables and troff's ASCII files from Imagen supplied raster
 * files in directory *rastdir. All this stuff is done under the control of
 * specially formatted ASCII files which are named as command line arguments.
 * The source raster files are found in *rastdir, which can be changed using
 * the -S option.
 *
 */


    argc = agc;				/* other routines may want them */
    argv = agv;

    prog_name = argv[0];		/* really just for error messages */

    rastdir = ".";			/* source raster file directory */
    nrastdir = ".";			/* new raster file directory */
    fontdir = ".";			/* font directory for new files */

    options();				/* first get command line options */
    arguments();			/* then process non-option arguments */

    exit(x_stat);			/* everything probably went OK */

}   /* End of main */


/*****************************************************************************/


options()


{


    int		ch;			/* return value from getopt() */
    char	*names = "DIF:R:S:s:r:u:f:d";

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

	    case 'r':			/* set device resolution */
		    res = atoi(optarg);
		    break;

	    case 'u':			/* font table unitwidth */
		    unitwidth = atoi(optarg);
		    break;

	    case 's':			/* do stuff for these sizes only */
		    get_sizes(optarg);
		    break;

	    case 'f':			/* and these fonts */
		    get_fonts(optarg);
		    break;

	    case 'd':			/* don't build troff's DESC file */
		    dodesc = OFF;
		    break;

	    case 'F':			/* new troff font directory */
		    fontdir = optarg;
		    break;

	    case 'R':			/* directory for new raster files */
		    nrastdir = optarg;
		    break;

	    case 'S':			/* source raster files found here */
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


get_sizes(str)


    char	*str;			/* comma or blank spearated size list */


{


    char	*tok;			/* next token in *str */

    char	*strtok();


/*
 *
 * Converts a comma or blank separated list of point sizes to integers
 * and stores the results in sizelist[]. Used to control how many of the
 * raster files are built. If the -s options isn't used to select point
 * sizes, raster files for all the sizes listed in the ASCII control files
 * will be built.
 *
 */


    while ( (tok = strtok(str, " ,")) != NULL )  {
	sizelist[sizecount++] = atoi(tok);
	str = NULL;
    }	/* End while */

}   /* End of get_sizes */


/*****************************************************************************/


get_fonts(str)


    char	*str;			/* list of fonts to process */


{


    char	*tok;			/* next font name from *str */


/*
 *
 * Called from options() to convert a comma or blank separated list of
 * font names into pointers that can be stored in fontlist[]. Overrides
 * the selection of fonts listed in the ASCII control files.
 *
 */


    while ( (tok = strtok(str, " ,")) != NULL )  {
	fontlist[fontcount++] = tok;
	str = NULL;
    }	/* End while */

}   /* End of get_fonts */


/*****************************************************************************/


arguments()


{


/*
 *
 * All the rest of the comand line arguments are the names of ASCII control
 * files. If there are no arguments left when we get here stdin will be
 * read.
 *
 * Although the program accepts more than one control file name, I haven't
 * really tested things using several control files. Probably needs more
 * some more work.
 *
 */


    if ( argc < 1 )  {
	fp_in = stdin;
	readcontrol();
    } else {
	while ( argc > 0 )  {
	    if ( (fp_in = fopen(*argv, "r")) == NULL )
		error(FATAL, "can't open %s", *argv);
	    readcontrol();
	    fclose(fp_in);
	    argc--;
	    argv++;
	}   /* End while */
    }   /* End else */

}   /* End of arguments */


/*****************************************************************************/


readcontrol()


{


    long	pos;			/* where build section starts in fp_in */
    int		i;			/* loop index for sizelist[] */


/*
 *
 * Starts reading the ASCII description file fp_in. The stuff at the
 * beginning is primarily for the DESC file and is just skipped, although
 * the sizes, res, and unitwidth fields are used if the values haven't been
 * set by options. Lines are read and processed by this routine until
 * EOF is reached or the first build command is found. If we process the
 * first part of the file and don't have res, unitwidth, and sizecount set
 * an error message will be printed and we'll quit.
 *
 * After we break out of the first while loop, usually because a 'build'
 * command was read, we record our position in file *fp_in and then enter
 * a for loop that processes the rest of the file - once for each of the
 * requested sizes. gotspecial controls whether we record special character
 * names for the DESC file. We only need to do it on the first complete read
 * of file *fp_in.
 *
 */


    while ( fscanf(fp_in, "%s", buff) != EOF )
	if ( sizecount <= 0  &&  strcmp(buff, "sizes") == 0 )  {
	    while ( fscanf(fp_in, "%d", &sizelist[sizecount]) == 1 && sizelist[sizecount] != 0 )
		sizecount++;
	} else if ( unitwidth <= 0  &&  strcmp(buff, "unitwidth") == 0 )
	    fscanf(fp_in, "%d", &unitwidth);
	else if ( res <= 0  &&  strcmp(buff, "res") == 0 )
	    fscanf(fp_in, "%d", &res);
	else if ( strcmp(buff, "device") == 0 )
	    fscanf(fp_in, "%s", device);
	else if ( strcmp(buff, "build") == 0 )
	    break;
	else skipline(fp_in);

    if ( res <= 0  ||  unitwidth <= 0  ||  sizecount <= 0 )
	error(FATAL, "Missing res, unitwidth, or size specification");

    if ( device[0] == '\0' )
	error(FATAL, "no device name given");

    pos = (long) ftell(fp_in);		/* probably want to get back here */

    for ( i = 0; i < sizecount; i++ )  {
	build(sizelist[i]);
	fseek(fp_in, pos, 0);
	gotspecial = TRUE;
    }	/* End for */

    builddesc();
    rastdata();

}   /* End of readcontrol */


/*****************************************************************************/


build(size)


    int		size;			/* building files for this size */


{


    char	name[40];		/* name of font we're building */
    char	*ptr;			/* used to pick up font comment field */
    int		i, n;			/* used for font size selection */
    int		lsize;			/* last valid size in font size list */
    int		osize;			/* original size - reset in the loop */


/*
 *
 * Called from readcontrol() when we want to build font and raster files. At the
 * top of the main loop we've read a 'build' command but haven't processed the
 * name of the font yet. Once we get the name we initialize some variables,
 * throw out all raster files we may already have read, skip the rest of the
 * current line in *fp_in, and then enter a loop that processes all the
 * allowed commands in a build section. The most important of these commands
 * is 'using', which allows us to select particular characters from named
 * raster files. It's assumed that we're finished with the current build
 * section when we return from using() or when we read another 'build'
 * command. When we exit the inner loop we're ready to build the new
 * raster file. If size == unitwidth we'll also want to construct an ASCII
 * font file for font *name.
 *
 * Actually that's not quite right. I've added code, that's really not very
 * clear, that handles fonts with less than a complete set of raster files.
 * Although much of the stuff can be done by hand, I think it's important
 * to have buildrast handle everything. Logos will usually come in only
 * one size and we may eventually want to add more logos to the raster file.
 * That in itself would be difficult to do by hand, and adding more font
 * font positions isn't all that clean either. There are a bunch of other
 * ways it could have been handled, but I think adding the code to buildrast
 * was the best choice. Fonts that don't come in all the sizes are described
 * in the 'build' section by a command that looks like,
 *
 *		sizes 12 18 36 0
 *
 * This lists the point sizes that are available for building the font.
 * All output will only be turned OFF unless the size that we're currently
 * working on is in the list. The sizes can come in any order as long as
 * the list is terminated by 0. The ASCII font files are built when the
 * current size is equal to the first size in the list. Widths are properly
 * scaled to the correct unitwidth. The ascender and descender stuff also
 * had to be changed.
 *
 * Turning the output OFF when a given size wasn't available turned out
 * to be tricky, and that's where lsize is used. If the size isn't available
 * we'll still have work to do in addition to reading the file, especially
 * the first time through. So all the stuff works properly I set size to
 * lsize before going on to the 'using' section. It's a kludge but the
 * raster files should be available in lsize even if we're not building
 * any files. Anyway that's sort of what's happening in the new stuff.
 *
 */


    osize = size;			/* in case we change size */

    while ( fscanf(fp_in, "%s", name) != EOF )  {
	fontoutput = wantfont(name);
	size = osize;
	first = 1;
	last = 0;
	comment[0] = '\0';
	ligatures[0] = '\0';
	ascender[0] = '\0';
	spacechar[0] = '\0';
	spacewidth = -1;
	isspecial = FALSE;
	dofont = (size == unitwidth) ? ON : OFF;
	resetrast();
	skipline(fp_in);
	while ( fscanf(fp_in, "%s", buff) != EOF )
	    if ( strcmp(buff, "using") == 0 )  {
		using(name, size);
		break;
	    } else if ( strcmp(buff, "build") == 0 )
		break;
	    else if ( strcmp(buff, "special") == 0 )
		isspecial = TRUE;
	    else if ( strcmp(buff, "ascender") == 0 )
		fscanf(fp_in, "%s", ascender);
	    else if ( strcmp(buff, "spacewidth") == 0 )
		fscanf(fp_in, "%s", spacechar);
	    else if ( strcmp(buff, "start") == 0 )  {
		fscanf(fp_in, "%d", &first);
		last = first - 1;
	    } else if ( strcmp(buff, "sizes") == 0 )  {
		for ( i = 0, lsize = 0; fscanf(fp_in, "%d", &n) == 1 && n != 0 && n != size; i++, lsize = n ) ;
		lsize = (lsize == 0 ) ? n : lsize;
		fontoutput = (fontoutput == ON  &&  n == size) ? ON : OFF;
		dofont = (fontoutput == ON  &&  i == 0 ) ? ON : OFF;
		size = (fontoutput == ON) ? size : lsize;
		skipline(fp_in);
	    } else if ( strcmp(buff, "comment") == 0 )  {
		for ( ptr = comment; (*ptr = getc(fp_in)) != '\n' && *ptr != EOF; ptr++ ) ;
		*ptr = '\0';
	    } else if ( buff[0] == '#' )
		skipline(fp_in);
	    else error(FATAL, "don't understand command %s", buff);
	writerast(name, size);
	buildfont(name, size);
    }	/* End while */

}   /* End of build */


/*****************************************************************************/


using(name, size)


    char	name[];			/* building things for this font */
    int		size;			/* and in this size */


{


    char	chname[10];		/* special character name */


/*
 *
 * Called after we've read a 'using' command. The input file is read until
 * we reach EOF of find another build command. The format of a 'using' command
 * can be either,
 *
 *		using name take ascii characters n to m
 *
 * or,
 *
 *		using name take characters
 *			12	fl
 *			13	ff
 *			103	*a
 *
 *
 * where name refers to one of the raster files in *rastdir. It's assumed
 * that the ascii characters n to m are in positions n to m in raster file
 * name. If that's not the case the more general form given in the second
 * example should be used. That's also the form that will be needed for troff's
 * special characters.
 *
 * Synonyms are also allowed and can be specified by having the character
 * '"' in the first index field as the following example illustrates.
 *
 *		using name take characters
 *			38	&
 *			39	aa
 *			"	'
 *			119	14
 *			"	34
 *			"	12
 *			91	[
 *
 * The list of synonyms for a particular character is recorded in the
 * Charinfo structure for that character and is only used when we build the
 * ASCII font files.
 *
 * There can be as many 'using' commands for a particular font as you want
 * and you can intermix the two formats. We'll continue building the current
 * raster file up until we find another 'build' command or reach EOF.
 *
 */


    if ( fontoutput == ON )
	fprintf(stdout, "Building tables for %s size %d\n", name, size);

    while ( fscanf(fp_in, "%s", buff) != EOF )  {
	getrastdata(buff, size);

	if ( fscanf(fp_in, " take %s", buff) != 1 )
	    error(FATAL, "syntax error in using()");

	if ( strcmp(buff, "ascii") == 0 )
	    getascii();

	skipline(fp_in);
	while ( fscanf(fp_in, "%s", buff) != EOF )  {
	    if ( isdigit(buff[0]) )  {
		fscanf(fp_in, "%s", chname);
		recordchar(chname, atoi(buff));
	    } else if ( buff[0] == '"' )
		getsynonym();
	    else if ( strcmp(buff, "using") == 0 )
		break;
	    else if ( strcmp(buff, "build") == 0 )
		return;
	    else if ( strcmp(buff, "edit") == 0 )  {
		skipline(fp_in);
		edit(fp_in);
		return;
	    } else if ( buff[0] == '#' )
		skipline(fp_in);
	    else error(FATAL, "don't understand command %s", buff);
	}   /* End while */
    }	/* End while */

}   /* End of using */


/*****************************************************************************/


getascii()


{


    int		start;			/* first character's ASCII code */
    int		stop;			/* last character's code */
    char	name[2];		/* name of the character - string */


/*
 *
 * Called when we want to take a bunch of ASCII characters from the current
 * raster file. As I mentioned before the expected format is,
 *
 *	using name take ascii characters start to stop
 *
 * where name is the name of the raster file we want to use, while start
 * and stop and the indices of the ASCII characters we want to steal from
 * font name. Obviously start should be less than or equal to stop otherwise
 * nothing will be done.
 *
 */


    if ( fscanf(fp_in, "%*s %d to %d", &start, &stop) != 2 )
	error(FATAL, "syntax error in getascii()");

    name[1] = '\0';			/* terminate the string */

    for ( ; start <= stop; start++ )  {
	name[0] = start;
	recordchar(name, start);
    }	/* End for */

}   /* End of getascii */


/*****************************************************************************/


recordchar(name, index)


    char	*name;			/* name of the character */
    int		index;			/* its glyph number in *fam */


{


    char	*gptr1, *gptr2;		/* glyph directory pointers */
    int		height, width;		/* of the bitmap */
    int		i;			/* used to copy directory entry */


/*
 *
 * Called when we want to record information about character *name. Its
 * number in the current raster file is index. Most of the information about
 * the character is stored in charinfo[++last]. We'll need to remember its
 * name, pointer to its bitmap, size of the bitmap, and pointer to the new
 * glyph directory entry. Data about the bitmap comes directly from the glyph
 * directory entry for character index. The pointer to the new glyph directory
 * entry is figured out from the current values of first and last. Each entry
 * in the directory is 15 bytes long, and all we do is copy index's entry
 * over to the new character's entry. All that really has to be fixed up
 * is the bitmap pointer, and this guy is figured out in writerast() when
 * we're creating the new raster file.
 *
 */


    if ( ++last > MAX_INDEX )
	error(FATAL, "too many characters in current raster file");

    height = getvalue(G_HEIGHT, index);
    width = getvalue(G_WIDTH, index);
    gptr1 = fam->rst + GLYPH_PTR(index);
    gptr2 = glyphdir + (15 * (last - first));

    strcpy(charinfo[last].name, name);
    strcpy(charinfo[last].rastname, fam->name);
    charinfo[last].index = index;
    charinfo[last].mapsize = ((width + BYTE - 1) / BYTE) * height;
    charinfo[last].map = fam->rst + getvalue(G_BPTR, index);
    charinfo[last].glydir = gptr2;
    charinfo[last].synonyms[0] = '\0';
    charinfo[last].chwidth = -1;

    for ( i = 0; i < 15; i++, gptr1++, gptr2++ )
	*gptr2 = *gptr1;

    if ( gotspecial == FALSE  &&  dodesc == ON  &&  name[1] != '\0' )
	specialchar(name);

    if ( spacechar[0] != '\0'  &&  strcmp(name, spacechar) == 0 )
	spacewidth = last;

}   /* End of recordchar */


/*****************************************************************************/


getsynonym()


{


    char	chname[10];		/* character's name built up here */


/*
 *
 * Called from using() when we've found a '"' in the index field. The
 * character named next in the input file is a synonym for the one we
 * just defined (ie. charinfo[last]). We'll add the name to the list of
 * synonyms for the character and use that list when we build the ASCII
 * font file.
 *
 */


    if ( last < first )			/* haven't defined anything yet */
	error(FATAL, "bad synonym request - no characters defined");

    fscanf(fp_in, "%s", chname);

    strcat(strcat(charinfo[last].synonyms, " "), chname);

    if ( gotspecial == FALSE  &&  dodesc == ON  &&  chname[1] != '\0' )
	specialchar(chname);

}   /* End of getsynonym */


/*****************************************************************************/


specialchar(name)


    char	*name;			/* keep track of special characters */


{


    int		i;			/* loop index */


/*
 *
 * Called to record the special characters we find in the charset[] array.
 * It's used later on when we build the DESC file. We also keep track of
 * the ligatures defined on this font (if any), and we'll use this info
 * when we build the new ASCII font file.
 *
 */


    for ( i = 0; i < charcount; i++ )
	if ( strcmp(name, charset[i].name) == 0 )
	    break;

    if ( i >= charcount )  {
	strcpy(charset[charcount++].name, name);
	if ( strcmp(name, "fi") == 0 )
	    strcat(ligatures, " fi");
	else if ( strcmp(name, "ff") == 0 )
	    strcat(ligatures, " ff");
	else if ( strcmp(name, "fl") == 0 )
	    strcat(ligatures, " fl");
	else if ( strcmp(name, "Fi") == 0 )
	    strcat(ligatures, " ffi");
	else if ( strcmp(name, "Fl") == 0 )
	    strcat(ligatures, " ffl");
    }	/* End if */

}   /* End of specialchar */


/*****************************************************************************/


writerast(name, size)


    char	*name;			/* name of the font */
    int		size;			/* in this point size */


{


    int		fd;			/* new raster file */
    int		presize;		/* really preamble + file mark */
    int		dirsize;		/* size of glyph directory */
    int		totsize;		/* total size of preamble and direc */
    char	*ptr;			/* used to change new directory */
    int		offset;			/* adjust bitmap pointers */
    int		i;			/* loop index */


/*
 *
 * All the raster file data for *name.size has been collected. This routine
 * puts it all together and writes everything out to the new raster file.
 *
 */


    if ( fontoutput == OFF ) return;	/* not doing output for this font */

    fam = &fam_data[0];
    cur_fam = 0;

    sprintf(buff, "%s/rast%s/%s.%d", nrastdir, device, name, size);
    if ( (fd = creat(buff, 0644)) == -1)
	error(FATAL, "can't open %s", buff);

    presize = 8 + rst[P_LENGTH].size + getvalue(P_LENGTH);
    dirsize = (last - first + 1) * 15;
    totsize = presize + dirsize;

    writevalue(first, P_FIRSTGLY, fam->rst + rst[P_FIRSTGLY].offset);
    writevalue(last, P_LASTGLY, fam->rst + rst[P_LASTGLY].offset);
    writevalue(presize, P_GLYDIR, fam->rst + rst[P_GLYDIR].offset);

    write(fd, fam->rst, presize);

    for ( ptr = glyphdir + rst[G_BPTR].offset, offset = 0, i = first; i <= last; i++, ptr += 15 )  {
	writevalue(totsize + offset, G_BPTR, ptr);
	offset += charinfo[i].mapsize;
    }	/* End for */

    write(fd, glyphdir, (last - first + 1) * 15);

    for ( i = first; i <= last; i++ )
	write(fd, charinfo[i].map, charinfo[i].mapsize);

    close(fd);

}   /* End of writerast */


/*****************************************************************************/


buildfont(font, size)


    char	*font;			/* name of troff's font file */
    int		size;			/* have data for this point size */


{


    FILE	*fp;			/* new font file */
    int		i;			/* loop index */
    int		chwidth;		/* next character's font table width */
    char	*ptr;			/* the whole synonym string */
    char	*syn;			/* next synonym for current character */

    char	*strtok();


/*
 *
 * Called to build the ascii font file for the current font. Things are
 * only done if if the current size is equal to unitwidth.
 *
 */


    if ( fontoutput == OFF  ||  dofont == OFF )
	return;

    sprintf(buff, "%s/dev%s/%s", fontdir, device, font);
    if ( (fp = fopen(buff, "w")) == NULL )
	error(FATAL, "can't open font file %s", buff);

    if ( comment[0] != '\0' )
	fprintf(fp, "#%s\n", comment);

    fprintf(fp, "name %s\n", font);
    fprintf(fp, "internalname %d\n", internalname++);

    if ( isspecial == TRUE )
	fprintf(fp, "special\n");

    if ( ligatures[0] != '\0' )
	fprintf(fp, "ligatures %s 0\n", ligatures);

    fprintf(fp, "charset\n");

    if ( isspecial == FALSE )  {
	fprintf(fp, "\\|	%d	0	0\n", (res * unitwidth)/(72 * 6));
	fprintf(fp, "\\^	%d	0	0\n", (res * unitwidth)/(72 * 12));
    }	/* End if */

    setascender(size);

    for ( i = first; i <= last; i++ )  {
	if ( (chwidth = charinfo[i].chwidth) < 0 )  {
	    chwidth = readvalue(charinfo[i].glydir + rst[G_CHWIDTH].offset, rst[G_CHWIDTH].size, UNSIGNED);
	    chwidth = (PIXEL_WIDTH(chwidth, res) * unitwidth) / size;
	}    /* End if */
	fprintf(fp, "%s	%d	%d	%d\n", charinfo[i].name, chwidth, getascender(i), i);
	ptr = charinfo[i].synonyms;
	while ( (syn = strtok(ptr, " ")) != NULL )  {
	    fprintf(fp, "%s	\"\n", syn);
	    ptr = NULL;
	}   /* End while */
    }	/* End for */

    fclose(fp);

}   /* End of buildfont */


/*****************************************************************************/


builddesc()


{


    FILE	*fp;			/* new desc file */
    int		ch;			/* for copying first part of fp_in */
    int		i;			/* for loop index for copying charset */


/*
 *
 * Builds the new DESC file from the data contained in the first part of
 * the ASCII control file *fp_in and the special characters saved in the
 * charset[] array.
 *
 */


    if ( dodesc == OFF )		/* don't build new DESC file */
	return;

    sprintf(buff, "%s/dev%s/DESC", fontdir, device);
    if ( (fp = fopen(buff, "w")) == NULL )
	error(FATAL, "can't open file %s", buff);

    fseek(fp_in, 0L, 0);		/* back to start of control file */

    while ( fscanf(fp_in, "%s", buff) != EOF )
	if ( strcmp(buff, "build") == 0 )
	    break;
	else if ( buff[0] == '#'  ||  strcmp(buff, "device") == 0 )
	    skipline(fp_in);
	else  {
	    fprintf(fp, "%s", buff);
	    while ( (ch = putc(getc(fp_in), fp)) != '\n'  &&  ch != EOF ) ;
	}   /* End else */

    fprintf(fp, "charset\n");
    fprintf(fp, "\\| \\^");

    for ( i = 0; i < charcount; i++ )
	fprintf(fp, "%c%s", (i % 15 == 13) ? '\n' : ' ', charset[i].name);
    putc('\n', fp);

    fclose(fp);

}   /* End of builddesc */


/*****************************************************************************/


rastdata()


{


    FILE	*fp;			/* "RASTDATA" file */


/*
 *
 * The post-processor and possibly others will need to know the resolution
 * and format of the raster files. It's looked for in file "RASTDATA", which
 * is defined in file init.h. Anyway this routine makes sure a reasonable
 * data file goes along with all the raster files we just built.
 *
 */


    sprintf(buff, "%s/rast%s/%s", nrastdir, device, RASTDATA);
    if ( (fp = fopen(buff, "w")) == NULL )
	error(FATAL, "can't write file %s", buff);

    fprintf(fp, "#\n# Format and resolution data\n#\n");
    fprintf(fp, "format new\nresolution %d\n", res);

    fclose(fp);

}    /* End of rastdata */


/*****************************************************************************/


setascender(size)


    int		size;			/* for this size - usually unitwidth */


{


    int		i;			/* just a loop index */


/*
 *
 * Called to set reasonable values for the ascender/descender values above
 * and below. If *ascender != '\0' we'll look the character up in charinfo[]
 * and use it's height and yref to set values for above and below. If we
 * don't find the character or it hasn't been set we'll use size and res
 * to pick reasonable values.
 *
 */


    for ( i = first; i <= last; i++ )
	if ( strcmp(ascender, charinfo[i].name) == 0 )
	    break;

    if ( i > last )
	 above = (size * res) / (72.27 * 2);
    else above = readvalue(charinfo[i].glydir + rst[G_YREF].offset, rst[G_YREF].size, INTEGER);

    above = above + .30 * above + .5;
    below = .20 * above + .5;

}   /* End of setascender */


/*****************************************************************************/


getascender(n)


    int		n;			/* character's index in charinfo[] */


{


    int		height;			/* raster height of n's bitmap */
    int		yref;			/* y reference point */
    int		value;			/* ascender value for character n */


/*
 *
 * Figures out an appropriate value for the ascender/descender field in
 * troff's ASCII font files for character n.
 *
 */


    value = 0;

    yref = readvalue(charinfo[n].glydir + rst[G_YREF].offset, rst[G_YREF].size, INTEGER);
    height = readvalue(charinfo[n].glydir + rst[G_HEIGHT].offset, rst[G_HEIGHT].size, UNSIGNED);

    if ( yref >= above )
	value |= 02;

    if ( height - yref > below )
	value |= 01;

    return(value);

}   /* End of getascender */


/*****************************************************************************/


skipline(fp)


    FILE	*fp;			/* skip rest of line in this file */


{


    int		ch;			/* next character from *fp_in */


/*
 *
 * Skips the rest of the current line in file *fp.
 *
 */


    while ( (ch = getc(fp)) != '\n'  &&  ch != EOF ) ;

}   /* End of skipline */


/*****************************************************************************/


writevalue(val, field, p)


    unsigned	val;			/* write this value out */
    int		field;			/* as next rst[field].size bytes */
    char	*p;			/* starting right here */


{


    int		n;			/* size in bytes of field */
    int		i;			/* just a loop index */


/*
 *
 * Writes val out as the nextrst[field].sizebytes starting at address *p.
 * It's used to build entries in the preamble and glyph directory for the
 * new raster file we're currently working on.
 *
 */


    n = rst[field].size;		/* number of bytes to write out */

    for ( i = 1; i <= n; i++, p++ )
	*p = (val >> (n - i) * BYTE) & BMASK;

}   /* End of writevalue */


/*****************************************************************************/


unsigned readvalue(p, n, kind)


    register char	*p;		/* start here */
    register int	n;		/* and get this many bytes */
    int			kind;		/* type of field - INTEGER or UNSIGNED */


{


    register unsigned	value;		/* result after decoding the field */


/*
 *
 * Does essentially the same thing as getvalue() in rast.c except that we
 * supply different information as parameters.
 *
 */


    value = (kind == INTEGER  &&  (*p & 0200)) ? ~0 : 0;

    for ( ; n > 0; n--, p++ )
	value = (value << BYTE) | (*p & BMASK);

    return(value);

}   /* End of readvalue */


/*****************************************************************************/


wantfont(name)


    char	*name;			/* do work for this font? */


{


    int		i;			/* loop index */


/*
 *
 * If we've just asked to process a few fonts (using -f option) fontcount
 * will be non-zero and pointers to the requested font names will be stored
 * in array fontlist[]. This routine returns ON if *name is one of the
 * requested fonts or if fontcount is 0. Otherwise OFF will be returned to
 * the caller, which should stop all output for *name.
 *
 */


    if ( fontcount == 0 )		/* didn't ask for any fonts */
	return(ON);

    for ( i = 0; i < fontcount; i++ )
	if ( strcmp(name, fontlist[i]) == 0 )
	    return(ON);

    return(OFF);

}   /* End of wantfont */


/*****************************************************************************/


