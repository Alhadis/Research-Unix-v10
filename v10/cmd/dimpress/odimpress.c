
/*
 *
 * dimpress - cleaned up and modified code borrowed from di10.
 *
 *
 * troff post-processor for Imagen printers. Output is written in
 * version 2.0 Impress, although it does still work for earlier versions.
 * Raster files in either the old format or in Imagen's Rst format can
 * be used for character generation. Drawing functions are handled by
 * the routines in file impdraw.c. I rewrote most of those routines so they
 * use the graphics primitives avaliable in version 1.9 (and later) Impress.
 * If you're printer is still running older systems you'll probably need
 * to get the original version of draw.c and use it instead.
 *
 * All the code should work properly for any printer that accepts Impress.
 * By default I've got things set up for the 8/300, but you can use the
 * -T option to select a different printer. If you choose a new printer
 * name not supported under the -T option you'll probably want to add code
 * to do the proper initialization. There really are only six variables
 * that need to be changed for different printers. They include realdev,
 * penwidth, pres, xoff, yoff, and possibly bitdir. The printer variables
 * are defined in dimpress.h (PR_INIT) and that's what's used to initialize
 * the prdata[] array. If you make changes just be sure that the list ends
 * with an entry that has its prname field set to NULL.
 *
 *
 * output language from troff:
 * all numbers are character strings
 * 
 * sn	size in points
 * fn	font as number from 1-n
 * cx	ascii character x
 * Cxyz	funny char xyz. terminated by white space
 * Hn	go to absolute horizontal position n
 * Vn	go to absolute vertical position n (down is positive)
 * hn	go n units horizontally (relative)
 * vn	ditto vertically
 * nnc	move right nn, then print c (exactly 2 digits!)
 * 		(this wart is an optimization that shrinks output file size
 * 		 about 35% and run-time about 15% while preserving ascii-ness)
 * Dt ...\n	draw operation 't':
 * 	Dl x y		line from here by x,y
 * 	Dc d		circle of diameter d with left side here
 * 	De x y		ellipse of axes x,y with left side here
 * 	Da x y r	arc counter-clockwise by x,y of radius r
 * 	D~ x y x y ...	wiggly line by x,y then x,y ...
 * nb a	end of line (information only -- no action needed)
 * 	b = space before line, a = after
 * p	new page begins -- set v to 0
 * #...\n	comment
 * x ...\n	device control functions:
 * 	x i	init
 * 	x T s	name of device is s
 * 	x r n h v	resolution is n/inch
 * 		h = min horizontal motion, v = min vert
 * 	x p	pause (can restart)
 * 	x s	stop -- done for ever
 * 	x t	generate trailer
 * 	x f n s	font position n contains font s
 * 	x H n	set character height to n
 * 	x S n	set slant to N
 * 
 * 	Subcommands like "i" are often spelled out like "init".
 *
 */

#include	<stdio.h>
#include	<signal.h>
#include	<math.h>
#include	<ctype.h>
#include	<time.h>

#include	"gen.h"			/* general purpose definitions */
#include	"ext.h"			/* external variable definitions */
#include	"init.h"		/* just used for BITDIR definition */
#include	"rast.h"		/* raster file definitions */
#include	"dev.h"			/* typesetter and font descriptions */
#include	"impcodes.h"		/* Impress 2.0 opcode definitions */
#include	"dimpress.h"		/* defs just used by this program */
#include	"spectab.h"


/*
 *
 * A few names we'll need when we're printing files. devname[] is what troff
 * thought was going to be the output device while realdev[] is the name
 * of the printer we're really going to be using. There may be occasions when
 * we want to send the output to a given printer (*realdev) but we want to
 * use the raster tables that were built for a different one (*rastdev). If
 * rastdev isn't NULL we'll assume that this is the name of the device's
 * raster files that we want to use. In otherwords if *rastdev hasn't been
 * set by an option *realdev will be used in combination with *bitdir to
 * define *rastdir.
 *
 */


char		devname[20] = "";	/* troff's target printer */
char		*realdev = NULL;	/* name of the printer we're really using */
char		*rastdev = NULL;	/* use raster tables made for this guy */


/*
 *
 * There's a bunch of stuff we'll want to know about the raster files we're
 * planning on using for this job. The most important, obviously is where
 * they're located. The routines that access the raster files all assume
 * they're in directory *rastdir, so it better be set up right before we
 * try to print anything. I've decided to have this program build up the
 * *rastdir string from *bitdir and either *rastdev or *realdev. All this
 * stuff will be done after the options are read. One of the reasons I'm
 * doing things this way is because we could want to do something like
 * print a troff file generated for the APS-5 on an Imprint-10 using the
 * raster files built for an 8/300. While that may sound rediculous the
 * raster tables can use up a lot of disk space and we may not want to
 * waste the space keeping complete sets of raster files for two or three
 * different devices. Anyway we should be able to do something reasonable
 * no matter what devices and raster files are requested.
 *
 * *bitdir is initialized to BITDIR (file init.h) and can be changed using
 * the -B option. The raster files for a particular device, say the 8/300
 * which is being called i300, will be found in directory *bitdir/rasti300.
 * It's defined in file glob.c because resident font routines need the
 * directory.
 *
 * We'll also need to know the format of the raster files we're using before
 * we can have any characters printed. rastformat keeps track of which
 * style is being used. It's initialized in routine rastsetup() using
 * info in file *rastdir/RASTDATA - if it exists. Right now if rastsetup()
 * can't read the RASTDATA file it assumes that the raster files are the
 * old versetec format and have a resolution of 240 dots per inch. I've
 * done things this way just to make sure we can use the post-processor
 * with the old raster files without having to add an appropriate RASTDATA
 * file to the directory. Eventually it may not be a bad idea to just
 * quit if we can't read the file.
 *
 */


int		rastformat;


/*
 *
 * Although I originally had ideas about not using a RASTERLIST file, I now
 * think it's probably a pretty good idea to use one. We could get around the
 * missing size problems by linking raster files for unavailble sizes to ones
 * we really have. That approach would cost more in downloading glyphs, but
 * more we'd probably be able to place the glyphs a little better because
 * we'd be able to set the character advance separately. Anyway it's really
 * not all that clear which approach is better. I've allowed for use of a
 * RASTERLIST file (routine rastlist()) but I've also decided that if it's
 * not there we'll continue on with the program assuming we have every size
 * that jobs ask for. If the job asks for a non-existent file it will die
 * in routine getrastdata().
 *
 * The following data structures are used to keep track of any RASTLIST
 * data we may have read. If maxrast is zero then there isn't any data
 * and we'll assume, when we do the lookups, that all fonts can be printed
 * in any size.
 *
 */


int		maxrast = 0;		/* number of RASTERLIST font entries */
Sizedata	sizedata[MAXFONTS];	/* available raster size data */


/*
 *
 * Standard things needed after we've read troff's font and device tables.
 * The binary files are all built by makedev and will be looked for in the
 * target printer's dev directory located in *fontdir. Actually that's not
 * quite right. There are mapping problems, both for fonts and characters,
 * that will exist if we try and use font files for a device that don't
 * exactly map into the raster files that are being used. For example if
 * we used the character code field in the normal APS-5 font tables when
 * we print jobs generated for the APS-5 we'd be guaranteed to get garbage.
 * To get around that problem I have the post-processor first look in
 * *rastdir for the dev directory. If it's not there then we look in *fontdir.
 *
 */


struct dev	dev;			/* DESC.out starts this way */
struct Font	*fontbase[NFONT+1];	/* starts each FONT.out file */
short		*pstab;			/* typesetter knows these sizes */
int		nsizes = 1;		/* sizes in *pstab list */
int		nfonts;			/* number of font positions */
int		smnt;			/* index of first special font */
int		nchtab;			/* number of special character names */
char		*chname;		/* special character strings */
short		*chtab;			/* used to locate character names */
char		*fitab[NFONT+1];	/* locates char info on each font */
char		*widthtab[NFONT+1];	/* widtab would be a better name */
char		*codetab[NFONT+1];	/* codes to get characters printed */


/*
 *
 * Variables that we use to keep track of troff's requests. All these
 * guys are set from values in the input files.
 *
 */


int		size = 1;		/* current size - internal value */
int		font = 1;		/* font position we're using now */
int		hpos = 0;		/* troff's current horizontal position */
int		vpos = 0;		/* same but vertically */
int		lastw = 0;		/* width of the last input character */
int		lastc = 0;		/* and its name (or index) */


/*
 *
 * We'll also want to keep track of some of the same things, but for the
 * target printer.
 *
 */


int		lastsize = -1;		/* last internal size we used */
int		lastfont = -1;		/* last font we told printer about */
int		lastx = -1;		/* printer's current position */
int		lasty = -1;


/*
 *
 * The following structure is used to keep track of the fonts that are
 * loaded in various positions. Both fields are filled in by routine t_fp()
 * using values in the binary font files.
 *
 */


struct  {
	char	*name;			/* name of the font loaded here */
	int	number;			/* its internal number */
} fontname[NFONT+1];


/*
 *
 * A few variables that are really just used for drawing things. Although
 * they're declared here most are only really used in draw.c. The pen
 * width and the step sizes can be changed by options. The step sizes DX
 * and DY control how far we move horizontally or vertically between
 * adjacent dots when drawing curves. They'll have no effect if we're
 * using Impress graphics commands for all graphics. Decreasing DX and DY
 * improves pictures but increases the size and complexity of the output
 * files. Because Impress complies and prints pages on the fly you may
 * not be able to draw fairly simple figures if the step sizes are too
 * small.
 *
 */


int		drawdot = '.';		/* draw with this character */
int		drawsize = 1;		/* shrink size by this factor */
int		penwidth = 1;		/* use this as the pen diameter */
int		DX = 6;			/* horiz step when using drawdot */
int		DY = 6;			/* same but for vertical step */


/*
 *
 * We may want to shift things around on the output page a little. In
 * particular moving everything right on the output pages is normal because
 * the left 3/8 inch or so of each page on an Imprint-10 isn't visible.
 * All this stuff could be done more efficiently by defining a new
 * coordinate system to match the requested offsets rather than doing
 * the calculations each time. I've left things this way for now because
 * that's the way we originally did stuff in di10.
 *
 * The xoff and yoff values are in inches while the xoffset and yoffset
 * numbers are pixels. We'll set the pixel values after we're sure about
 * the target printer's resolution. xoff and yoff can be changed by options.
 * xfac and yfac are scaling factors used to convert coordinates in the
 * input files to appropiate values in the target printers coordinate
 * system. Their values are set in t_init() because we can only do it
 * properly after we've gotten the printer's resolution (from options)
 * and read the "x res" command in each input file.
 *
 */


float		xoff;			/* shift right by this many inches */
float		yoff;			/* and down by this much */

int		xoffset;		/* pixel values = xoff * pres */
int		yoffset;		/* = yoff * pres */
float		xfac = 1.0;		/* scaling factor for x */
float		yfac = 1.0;		/* scaling factor for y */


/*
 *
 * A few miscellaneous variable declarations. I've added landscape mode
 * to the post-processor. It's requested using the -l option. Might also
 * be reasonable to add a new device control command to request landscape
 * mode, although I haven't done it yet. None of this stuff will work with
 * printer resident fonts.
 *
 */


int		mode = PORTRAIT;	/* landscape or portrait mode */
int		angle = ROT_0;		/* ROT_270 for LNADSCAPE mode */
int		output = 0;		/* do we do output at all? */
int		pageno = -1;		/* output page number */
int		copies = 1;		/* ask IPR for this many */
float		aspect	= 1;		/* default aspect ratio */
int		cancenter = TRUE;	/* try to improve char placement? */
int		center = 0;		/* for alphanumeric characters only */


/*
 *
 * We'll really need to deal with three possibly different resolutions
 * when we're printing files. The first is the resolution used by troff in
 * preparing the input file. Next is the resolution of the target printer.
 * Imprint-10s are 240 and 8/300s are 300 dots per inch. Finially we'll need
 * or at least want to know the resolution of the raster files we're using
 * to print the file. Obviously we'll get the best results when all three
 * numbers agree, but no matter what we should be able to do something
 * reasonable with any set of numbers.
 *
 */


int		res;			/* resolution assumed in input file */
int		pres;			/* resolution of the target printer */
int		rres;			/* raster file resolution */


/*
 *
 * The data about the different printers supported by the program is
 * stored in prdata[]. It's initialized using PR_INIT which is defined
 * in dimpress.h. The fields in structure Prdata are used to define the
 * printer name, resolution, x and y offsets (in inches), and the string
 * that should be used for *bitdir.
 *
 */


Prdata		prdata[] = PR_INIT;	/* printer data */

int		pr_num = PR_NUM;	/* printer we're using - index in prdata[] */


/*
 *
 * Many of the printer dependent variables can be set by several different
 * options. Most times we'll want to use the -T option, but there may be
 * occasions when we want to override one or more of the default values.
 * These variables keep track of whether we've tried to set values by other
 * options.
 *
 */


int		setpenwidth = FALSE;	/* changed by the -P option */
int		setxoff = FALSE;	/* -x option */
int		setyoff = FALSE;	/* -y option */
int		setpres = FALSE;	/* -r option */


/*
 *
 * A few variables that are really only used if we're doing accounting.
 * Much of the accounting stuff has been designed for our own use at
 * MHCC and may not suit your needs.
 *
 */


char		*acctfile = NULL;	/* append accounting record to this file */
char		*jacctfile = NULL;	/* just a page count for this job */
int		acctpages = 0;		/* charge for this many pages */
char		*username = NULL;	/* guy whose running this program */


/*
 *
 * If we're emulating another device we'll want to use the following array to
 * help map font names available on that device into PostScript fonts. It's
 * initialized using FONTMAP (file dpost.h) and may be changed in routine
 * getfontmap(). In particular if there's a file that matches the device name
 * in directory *fontdir/devpost/fontmaps, getfontmap() will use it instead of
 * the default - haven't implemented it and probably never will.
 *
 */


Fontmap	fontmap[100] = FONTMAP;		/* just for device emulation */


/*
 *
 * Output and accounting FILE definitions. If you invoke this program with
 * the -t option everything goes to stdout, otherwise it will be written
 * to *tempfile and then passed along to Imagen's spooler using IPR.
 *
 */

char		*tempfile = NULL;	/* output file name if no -t option used */

FILE		*tf = NULL;		/* and Impress output goes here */
FILE		*fp_acct = stderr;	/* accounting stuff  written here */


extern int	maxdots;		/* defined and used in draw.c */


/*****************************************************************************/


main(agc, agv)


	int	agc;
	char	*agv[];


{


/*
 *
 * Post-processor that's used to translate troff's device independent
 * output language into Impress (version 2.0 right now). A call to Imagen's
 * spooling package, through IPR, is made in print_file() provided
 * the output hasn't gone to stdout. Using the -t option or initializing
 * tf to be stdout forces output to stdout. If there are any processing
 * errors x_stat will be non-zero and done() will remove *tempfile before
 * quitting.
 *
 */


	argc = agc;			/* global so everyone can use them */
	argv = agv;

	prog_name = argv[0];		/* just used for error messages */

	init_signals();			/* sets up interrupt handling */
	options();			/* command line options */
	initialize();			/* must be done after options */
	rastsetup();			/* set all the raster file stuff up */
	rastlist();			/* get availble font and size list */
	resfonts(tf);			/* setup for any resident fonts */
	acct_file();			/* open accounting file - maybe */

	arguments();			/* translate all the input files */
	t_wrapup();			/* mark the end of job for Impress */
	print_file();			/* print the file we just built */

	done();				/* everything probably went OK */

}   /* End of main */


/*****************************************************************************/


init_signals()


{


	int	done();			/* handles them if we're catching sigs */


/*
 *
 * Makes sure we handle any interrupts properly. It wasn't done right
 * in di10.
 *
 */


	if ( signal(SIGINT, done) == SIG_IGN )  {
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGHUP, SIG_IGN);
	} else {
		signal(SIGHUP, done);
		signal(SIGQUIT, done);
	}   /* End else */

}   /* End of init_signals */


/*****************************************************************************/


options()


{


	int		ch;		/* option name returned from getopt */
	char		*names = "u:c:tx:y:r:a:o:p:LP:n:d:F:f:B:f:R:T:AJ:";

	extern char	*optarg;	/* option argument set by getopt() */
	extern int	optind;


/*
 *
 * Processes the command line options. The original stuff stuff done in
 * di10 has been changed a little. I've also used getopt() to scan the
 * options.
 *
 * The most confusing options are -B and -R. The *bitdir string is the
 * directory where we expect to find the raster file directories. They all
 * start with "rast" and end with a device name. I've got things set up so
 * that they're in /usr/lib/raster, but di10 looked for directory rasti10
 * in *fontdir (/usr/lib/font/devi10). The -B option changes the value
 * of *bitdir. If you're running DWB and want this program to replace di10
 * call it using the option "-B /usr/lib/font/devi10". The -R option
 * sets the value of string *rastdev. If that string isn't NULL then
 * "/rast*rastdev" will be appended to the end of *bitdir to locate the
 * raster file directory that we really want to use. By default rastdev is
 * NULL so *realdev will be used in its place.
 *
 * The -c option doesn't work right now. If you want to implement it you'll
 * probably just need to change the IPR system() call that's made in
 * print_file().
 *
 */


	while ( (ch = getopt(argc, argv, names)) != EOF )  {
		switch ( ch )  {
		case 'u':
			username = optarg;
			break;

		case 'c':
			copies = atoi(optarg);
			break;

		case 't':
			tf = stdout;
			break;

		case 'x':
			xoff = atof(optarg);
			setxoff = TRUE;
			break;

		case 'y':
			yoff = atof(optarg);
			setyoff = TRUE;
			break;


		case 'r':
			pres = atoi(optarg);
			setpres = TRUE;
			break;

		case 'a':
			aspect = atof(optarg);
			break;

		case 'o':
			out_list(optarg);
			break;

		case 'p':			/* pixels of resolution */
			if ( (DX = DY = atoi(optarg)) <= 0 )
				DX = DY = 1;
			break;

		case 'L':			/* landscape mode */
			mode = LANDSCAPE;
			break;

		case 'P':			/* pen diameter for drawing */
			if ( (penwidth = atoi(optarg)) > 20 )
				penwidth = 20;
			else if ( penwidth < 1 ) penwidth = 1;
			setpenwidth = TRUE;
			break;

		case 'n':			/* a limit for drawing routines */
			if ( (maxdots = atoi(optarg)) <= 0 )
				maxdots = 32000;
			break;

		case 'd':
			if ( (debug = atoi(optarg)) == 0 )
				debug = 1;
			tf = stdout;
			break;

		case 'F':			/* font table directory */
			fontdir = optarg;
			break;

		case 'B':			/* rast* directories found here */
			bitdir = optarg;
			break;

		case 'f':			/* use this resident font file */
			resfile = optarg;
			break;

		case 'R':			/* use this guys raster files */
			rastdev = optarg;
			break;

		case 'T':			/* target printer */
			for ( pr_num = 0; prdata[pr_num].prname != NULL; pr_num++ )
				if ( strcmp(optarg, prdata[pr_num].prname) == 0 )
					break;
			if ( prdata[pr_num].prname == NULL )
				error(FATAL, "don't know printer %s", optarg);
			break;

		/* These options were added for the MHCC */

		case 'A':
			x_stat |= DO_ACCT;
			break;

		case 'J':
			jacctfile = optarg;
			x_stat |= DO_ACCT;
			break;

		case '?':
			error(FATAL, "");
			break;

		default:
			error(FATAL, "don't know option %s", argv[1]);
			break;
		}
	}

	argc -= optind;			/* get ready for non-options args */
	argv += optind;

}   /* End of options */


/*****************************************************************************/


initialize()


{


	char	*mktemp();
#ifdef V9
	char	*getlogin();
#endif


/*
 *
 * Much of this stuff can only be done properly after the command line
 * options have been processed, so make sure it's called (from main())
 * after options().
 *
 */

#ifdef V9
	if (username == NULL && (username = getlogin()) == NULL )
#endif
#ifdef SYSV
	if ( (username = cuserid((char *) 0)) == NULL )
#endif
		username = "???";

	if (tf != stdout)  {
		tempfile = mktemp("/tmp/dimpXXXXX");
		if ((tf = fopen(tempfile, "w")) == NULL)
			error(FATAL, "can't open temporary file %s", tempfile);
	}   /* End if */

	if ( realdev == NULL )
		realdev = prdata[pr_num].prname;

	if ( setpres == FALSE )
		pres = prdata[pr_num].pres;

	if ( setxoff == FALSE )
		xoff = prdata[pr_num].xoff;

	if ( setyoff == FALSE )
		yoff = prdata[pr_num].yoff;

	if ( setpenwidth == FALSE )
		penwidth = prdata[pr_num].penwidth;

	if ( bitdir == NULL )
		bitdir = prdata[pr_num].bitdir;

	if ( resfile == NULL )
		resfile = prdata[pr_num].prname;

	res = rres = pres;		/* just to be safe */

	xoffset = pres * xoff;		/* set the pixel offsets */
	yoffset = pres * yoff;

}   /* End of initialize */


/*****************************************************************************/


rastsetup()


{


	static char	rastname[100];	/* rastdir string saved here */
	char		temp[100];	/* for pathnames and reads from *fp */
	FILE		*fp;		/* RASTDATA and RASTLIST files */
	int		ch;		/* just used to skip lines in *fp */


/*
 *
 * We'll need to know a bunch of stuff about the raster files before we can
 * actually use them. First of all we need to figure out where they are.
 * Strings *bitdir and either *rastdev or *realdev will be used to initialize
 * *rastdir. Once we've got rastname set up we'll need to determine the
 * format of the raster files and their resolution. All that stuff should
 * be in file *rastdir/RASTDATA. If the file's not there or we can't read
 * it we'll assume the files are written in the old format at a resolution
 * of 240 dots per inch. I've done things this way because RASTDATA is new
 * and won't be part of the of raster file directory unless you add it.
 * This way nothing has to be done to the old raster file directory.
 *
 */


	sprintf(rastname, "%s/rast%s", bitdir, (rastdev == NULL) ? realdev : rastdev);
	rastdir = rastname;

	sprintf(temp, "%s/%s", rastdir, RASTDATA);
	if ( (fp = fopen(temp, "r")) == NULL )  {
		rastformat = OLD_FORMAT;
		rres = 240;
		return;
	}   /* End if */

	while ( fscanf(fp, "%s", temp) != EOF )
		if ( strcmp(temp, "format") == 0 )  {
			fscanf(fp, "%s", temp);
			if ( strcmp(temp, "old") == 0 )
				rastformat = OLD_FORMAT;
			else rastformat = RST_FORMAT;
		} else if ( strcmp(temp, "resolution") == 0 )
			fscanf(fp, "%d", &rres);
		else while ( (ch = getc(fp)) != '\n'  &&  ch != EOF ) ;

	fclose(fp);

}   /* End of rastsetup */


/*****************************************************************************/


rastlist()


{


	int		n;		/* next size for current font */
	int		i;		/* next size goes here */
	char		name[100];	/* pathname for RASTERLIST file */
	FILE		*fp;


/*
 *
 * If we're not using the old format raster files we'll want to read the
 * list of available fonts and sizes in *rastdir. The file that's got all
 * the info is called RASTLIST (defined in init.h). The format is exactly
 * the same as Brian's original file. Under the old format all this stuff
 * is done in routine initfontdata().
 *
 * For now if we can't open RASTERLIST and were not using the old format
 * raster files we'll just return to the caller. That really just means
 * we'll assume we can handle any font + size request that may come our
 * way. If that's not the case we better make up a complete RASTERLIST file.
 *
 */


	if ( rastformat == OLD_FORMAT )	/* don't bother doing it here */
		return;

	sprintf(name, "%s/%s", rastdir, RASTLIST);
	if ( (fp = fopen(name, "r")) == NULL )
		return;

	while ( fscanf(fp, "%s", sizedata[maxrast].name) != EOF )  {
		i = 0;
		while ( fscanf(fp, "%d", &n) != EOF  &&  n < 100 )
			sizedata[maxrast].sizes[i++] = n;
		sizedata[maxrast].sizes[i] = 999;
		if ( ++maxrast >= MAXFONTS )
			error(FATAL, "too many fonts in %s", name);
	}	/* End while */

	fclose(fp);

}   /* End of rastlist */


/*****************************************************************************/


arguments()


{


	FILE	*fp;			/* next input file */


/*
 *
 * All the rest of the command line options are input files we want to
 * translate. If we get here and there are no more arguments, or if '-' is
 * in the list of file names, we'll process stdin.
 *
 */


	if (argc < 1)
		conv(stdin);
	else
		while (argc > 0) {
			if (strcmp(*argv, "-") == 0)
				fp = stdin;
			else if ((fp = fopen(*argv, "r")) == NULL)
				error(FATAL, "can't open %s", *argv);
			conv(fp);
			fclose(fp);
			argc--;
			argv++;
		}

}   /* End of arguments */


/*****************************************************************************/


print_file()


{


	char	buf[BUFSIZ];		/* IPR command line built up here */


/*
 *
 * Finished with all the input files and everything the printer needs to
 * know is in FILE *tf. We'll close the file because we're all done with
 * it, and then call IPR if we haven't been writing to stdout.
 *
 */


	fclose(tf);			/* everything's in the file */

	if ( tf != stdout  ||  debug )  {
		sprintf(buf, "%s -Limpress -r -o '-Downer \"'\"%s\"'\"'\", jobheader on, pagecollation on, pagereversal on\" %s 0</dev/null 1>/dev/null 2>&1 &",
		IPR, username, tempfile);
		if ( debug )
			fprintf(stderr, "executing %s\n", buf);
		else system(buf);
	}   /* End if */

}   /* End of print_file */


/*****************************************************************************/


acct_file()


{


/*
 *
 * If we want to do accounting *acctfile will be the name of the file where
 * we put the accounting data. If for some reason we can't open the file
 * we'll just quit. I've called the routine from main() before anything is
 * written to the output file.
 *
 */


	if ( acctfile != NULL  &&  *acctfile != '\0' )  {
		if ( (fp_acct = fopen(acctfile, "a")) == NULL )  {
			fp_acct = stderr;
			x_stat |= NO_ACCTFILE;
			error(FATAL, "can't open accounting file");
			exit(x_stat);
		}
		if ( tf != stdout )
			x_stat |= DO_ACCT;
	}

}   /* End of acct_file */


/*****************************************************************************/


account()


{


	FILE	*f;			/* just the job's page count */


/*
 *
 * Writes an accounting record, usually to *fp_acct, for the current job.
 * All this stuff is set up for our MHCC printers. In some cases we'll
 * know most everything about the job (called with -J option) and all
 * we'll do is write a page count to *jacctfile. In other cases we'll want
 * a more complete record and all the stuff will go to *fp_acct. You'll
 * undoubtedly want to change this stuff to suit your own needs.
 *
 */

	if ( x_stat & DO_ACCT )  {
		if ( jacctfile == NULL || *jacctfile == '\0' )  {
			fprintf(fp_acct, " user = %-10s", username);
			fprintf(fp_acct, " paper = %-10d", acctpages * copies);
			x_stat &= ~DO_ACCT;
			fprintf(fp_acct, " exit_status = 0%-6o", x_stat);
			fprintf(fp_acct, " type = t ");
			if ( tf == stdout )
				fprintf(fp_acct, "  ??");
			fprintf(fp_acct, "\n");
		} else {
			if ( (f = fopen(jacctfile, "a")) != NULL )  {
				fprintf(f, "%d\n", acctpages);
				fclose(f);
			}
			x_stat &= ~DO_ACCT;
		}	/* End else */
	}

}   /* End of account */


/*****************************************************************************/


done()


{


/*
 *
 * Finished with everything so we want to make sure accounting is handled
 * properly and the right exit status is returned to the caller. We'll also
 * delete the temporary file if x_stat != 0.
 *
 */


	account();

	if ( tf != stdout  &&  x_stat != 0 )
		unlink(tempfile);

	exit(x_stat);

}   /* End of done */


/*****************************************************************************/


conv(fp)


	register FILE	*fp;


{


	register int	c, k;
	int		m, n, i, n1, m1;
	char		str[100], buf[300];


/*
 *
 * Controls the translation of troff's device independent output language
 * to Impress. Some of the commands, like slant and height, are no-ops
 * on Imagen's bitmap printers even though routines are called to do the
 * processing.
 *
 */


	lineno = 1;			/* line in current file */
	x_stat |= FILE_STARTED;		/* we'll clear it when done with *fp */

	while ((c = getc(fp)) != EOF) {

		switch (c) {

		case '\n':		/* just count this line */
			lineno++;
			break;

		case ' ':		/* when input is text */
		case 0:			/* occasional noise creeps in */
			break;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			/* two motion digits plus a character */
			hmot((c-'0')*10 + getc(fp)-'0');
			put1(getc(fp));
			break;

		case 'c':		/* single ascii character */
			put1(getc(fp));
			break;

		case 'C':		/* special character */
			fscanf(fp, "%s", str);
			put1s(str);
			break;

		case 'N':		/* character at position n */
			fscanf(fp, "%d", &m);
			oput(m);
			break;

		case 'D':		/* drawing function */
			fgets(buf, sizeof(buf), fp);
			lineno++;
			switch (buf[0]) {
			case 'l':	/* draw a line */
				sscanf(buf+1, "%d %d", &n, &m);
				drawline(n, m);
				break;

			case 'c':	/* circle */
				sscanf(buf+1, "%d", &n);
				drawcirc(n);
				break;

			case 'e':	/* ellipse */
				sscanf(buf+1, "%d %d", &m, &n);
				drawellip(m, n);
				break;

			case 'a':	/* arc */
				sscanf(buf+1, "%d %d %d %d", &n, &m, &n1, &m1);
				drawarc(n, m, n1, m1);
				break;

			case '~':	/* wiggly line */
				drawwig(buf+1);
				break;

			default:
				error(FATAL, "unknown drawing function %s", buf);
				break;
			}
			break;

		case 's':			/* use this point size */
			fscanf(fp, "%d", &n);	/* ignore fractional sizes */
			setsize(t_size(n));
			break;

		case 'f':			/* use font mounted here */
			fscanf(fp, "%s", str);
			setfont(t_font(str));
			break;

		case 'H':			/* absolute horizontal motion */
/*
 *
 * The simple scan I've commented out didn't handle negative numbers right
 * and believe it or not we did get jobs that asked for negative absolute
 * positions. Even though negative absolute coordinates probably are a
 * mistake it makes more sense to handle the numbers properly.
 *
			while ((c = getc(fp)) == ' ')
				;
			k = 0;
			do {
				k = 10 * k + c - '0';
			} while (isdigit(c = getc(fp)));
			ungetc(c, fp);
			hgoto(k);
 *
 */
			fscanf(fp, "%d", &n);
			hgoto(n);
			break;

		case 'h':		/* relative horizontal motion */
/*
 *
 * Again the same potential problem with negative numbers exists here. In
 * fact it makes a lot more sense to expect negative relative motions even
 * though I never did see them. Anyway just to be safe I've made the same
 * change - take it out if you want.
 *
			while ((c = getc(fp)) == ' ')
				;
			k = 0;
			do {
				k = 10 * k + c - '0';
			} while (isdigit(c = getc(fp)));
			ungetc(c, fp);
			hmot(k);
 *
 */
			fscanf(fp, "%d", &n);
			hmot(n);
			break;

		case 'w':			/* word space */
			break;

		case 'V':			/* absolute vertical position */
			fscanf(fp, "%d", &n);
			vgoto(n);
			break;

		case 'v':			/* relative vertical motion */
			fscanf(fp, "%d", &n);
			vmot(n);
			break;

		case 'p':			/* new page */
			fscanf(fp, "%d", &n);
			t_page(n);
			break;

		case 'n':			/* end of line */
			while ( (c = getc(fp)) != '\n'  &&  c != EOF ) ;
			t_newline();
			lineno++;
			break;

		case '#':			/* comment */
			while ( (c = getc(fp)) != '\n'  &&  c != EOF ) ;
			lineno++;
			break;

		case 'x':			/* device control function */
			devcntrl(fp);
			break;

		default:
			error(!FATAL, "unknown input character %o %c", c, c);
			done();
		}
	}

	x_stat &= ~FILE_STARTED;

}   /* End of conv */


/*****************************************************************************/


devcntrl(fp)


	FILE	*fp;			/* current input file */


{


	char	str[20], str1[50], buf[50];
	int	c, n, x, y;


/*
 *
 * Called form conv() to process the rest of a device control function.
 * There's a whole family of them and they all begin with the string
 * "x ". The rest of the command consists of the function name followed
 * by zero or more arguments that depend on the particular command. We've
 * already read the "x" from the input file, that's why the routine was
 * called. The whole rest of the input line is assumed to be part of the
 * device control command.
 *
 */


	fscanf(fp, "%s", str);		/* get the control function name */

	switch ( str[0] )  {		/* only the first character counts now */
	case 'i':			/* initialize */
		fileinit();
		t_init(0);
		break;

	case 'T':			/* device name */
		fscanf(fp, "%s", devname);
		break;

	case 't':			/* trailer */
		t_trailer();
		break;

	case 'p':			/* pause -- can restart */
		t_reset('p');
		break;

	case 's':			/* stop */
		t_reset('s');
		break;

	case 'r':			/* resolution assumed when prepared */
		fscanf(fp, "%d", &res);
		break;

	case 'f':			/* load font in a position */
		fscanf(fp, "%d %s", &n, str);
		fgets(buf, sizeof buf, fp);	/* in case there's a filename */
		ungetc('\n', fp);	/* fgets goes too far */
		str1[0] = 0;	/* in case there's nothing to come in */
		sscanf(buf, "%s", str1);
		loadfont(n, str, str1);
		break;

	/* these don't belong here... */
	case 'H':			/* char height */
		fscanf(fp, "%d", &n);
		t_charht(n);
		break;

	case 'S':			/* slant */
		fscanf(fp, "%d", &n);
		t_slant(n);
		break;

	case 'I':
	case 'B':
		x = hpos * xfac + xoffset + .5;
		y = vpos * yfac + yoffset + .5;

		/* force 32x32 alignment, imagen 3.2 software does this
		** anyway, but this should fix others
		*/
		putc(ASETAV, tf); putint(y&~0x1f, tf);
		putc(ASETAH, tf); putint(x&~0x1f, tf);

		if (str[0] == 'I') {
			fscanf(fp, "%d %s", &n, buf);
			iglyphpage(tf, buf, n, x&0x1f, y&0x1f);
		} else if (str[0] == 'B') {
			fscanf(fp, "%d %s", &n, buf);
			glyphpage(tf, buf, n, x&0x1f, y&0x1f);
		}

		/* position back where it belongs */
		putc(ASETAV, tf); putint(y, tf); lasty = y;
		putc(ASETAH, tf); putint(x, tf); lastx = x;
		break;
	}

	while ( (c = getc(fp)) != '\n'  &&  c != EOF ) ;

	lineno++;

}   /* End of devcntrl */


/*****************************************************************************/


fileinit()


{


    int		i, fin;
    char	*filebase;
    char	temp[100];


/*
 *
 * Called from t_init(), which in turn is called from devcntrl(), whenever we get
 * an "x init" command. There are a few lines of code here that set things up for
 * emulating another device.
 *
 */


    sprintf(temp, "%s/dev%s/DESC.out", fontdir, devname);
    if ( (fin = open(temp, 0)) < 0 )
	error(FATAL, "can't open tables for %s", temp);

    read(fin, &dev, sizeof(struct dev));

    nfonts = dev.nfonts;

    if ( strcmp(devname, realdev) != 0 )  {	/* device emulation */
	close(fin);
	strcpy(devname, realdev);
	sprintf(temp, "%s/dev%s/DESC.out", fontdir, devname);
	if ( (fin = open(temp, 0)) < 0 )
	    error(FATAL, "can't open tables for %s", temp);
	read(fin, &dev, sizeof(struct dev));
    }	/* End if */

    nsizes = dev.nsizes;
    nchtab = dev.nchtab;

    if ( (filebase = malloc(dev.filesize)) == NULL )
	error(FATAL, "no memory for description file");

    read(fin, filebase, dev.filesize);	/* all at once */

    pstab = (short *) filebase;
    chtab = pstab + nsizes + 1;
    chname = (char *) (chtab + dev.nchtab);

    for ( i = 1; i <= nfonts; i++ )  {
	fontbase[i] = NULL;
	widthtab[i] = codetab[i] = fitab[i] = NULL;
    }	/* End for */

    close(fin);

}   /* End of fileinit */


/*****************************************************************************/


fontprint(i)


    int		i;			/* font's index in fontbase[] */


{


    int		j, n;
    char	*p;


/*
 *
 * Just a debugging routine that dumps most of the important information about
 * the font that's mounted in position i.
 *
 */


    fprintf(tf, "font %d:\n", i);

    p = (char *) fontbase[i];
    n = fontbase[i]->nwfont & BMASK;

    fprintf(tf, "base=0%o, nchars=%d, spec=%d, name=%s, widtab=0%o, fitab=0%o\n",
	    p, n, fontbase[i]->specfont, fontbase[i]->namefont, widthtab[i], fitab[i]);

    fprintf(tf, "widths:\n");
    for ( j = 0; j <= n; j++ )  {
	fprintf(tf, " %2d", widthtab[i][j] & BMASK);
	if ( j % 20 == 19 ) putc('\n', tf);
    }	/* End for */

    fprintf(tf, "\ncodetab:\n");
    for ( j = 0; j <= n; j++ )  {
	fprintf(tf, " %2d", codetab[i][j] & BMASK);
	if ( j % 20 == 19 ) putc('\n', tf);
    }	/* End for */

    fprintf(tf, "\nfitab:\n");
    for ( j = 0; j <= dev.nchtab + 128-32; j++ )  {
	fprintf(tf, " %2d", fitab[i][j] & BMASK);
	if ( j % 20 == 19 ) putc('\n', tf);
    }	/* End for */

    putc('\n', tf);

}   /* End of fontprint */


/*****************************************************************************/


loadfont(n, s, s1)


    int		n;
    char	*s, *s1;


{


    char	temp[80];
    int		fin, nw;


/*
 *
 * Called to to load font info for font *s on position n using the binary tables
 * located in directory *s1 (if it's not NULL or the empty string).
 *
 */


    if ( n < 0  ||  n > NFONT )		/* make sure it's a legal position */
	error(FATAL, "illegal fp command %d %s", n, s);

/*
 *
 * If the font's already loaded on position n there's nothing to do.
 *
 */

    if ( fontbase[n] != NULL && strcmp(s, fontbase[n]->namefont) == 0 )
	return;

/*
 *
 * If *s1 isn't NULL or the NULL string that's what we'll use for the font
 * directory, otherwise use *fontdir.
 *
 */

    if ( s1 == NULL || s1[0] == '\0' )
	sprintf(temp, "%s/dev%s/%s.out", fontdir, devname, s);
    else sprintf(temp, "%s/%s.out", s1, s);

    if ( (fin = open(temp, 0)) < 0 )  {
	sprintf(temp, "%s/dev%s/%s.out", fontdir, devname, mapfont(s));
	if ( (fin = open(temp, 0)) < 0 )
	    error(FATAL, "can't open font table %s", temp);
    }	/* End if */

    if ( fontbase[n] != NULL )		/* something's already there */
	free(fontbase[n]);		/* so release the memory first */

    fontbase[n] = (struct Font *) malloc(3*255 + dev.nchtab + (128-32) + sizeof(struct Font));
    if ( fontbase[n] == NULL )
	error(FATAL, "Out of space in loadfont %s", s);

    read(fin, fontbase[n], 3*255 + nchtab+128-32 + sizeof(struct Font));
    close(fin);

    if ( smnt == 0 && fontbase[n]->specfont == 1 )
	smnt = n;

    nw = fontbase[n]->nwfont & BMASK;
    widthtab[n] = (char *) fontbase[n] + sizeof(struct Font);
    codetab[n] = (char *) widthtab[n] + 2 * nw;
    fitab[n] = (char *) widthtab[n] + 3 * nw;

    t_fp(n, fontbase[n]->namefont, fontbase[n]->intname);

    if ( debug == ON )
	fontprint(n);

}   /* End of loadfont */


/*****************************************************************************/


char *mapfont(name)


    char	*name;			/* name of the font troff wants */


{


    int		i;			/* loop index for fontmap[] */


/*
 *
 * Only called from loadfont() when we haven't been able to open the font file
 * that troff asked for. We take the font name and map it into an appropriate
 * substitute. If the initial lookup fails we do a simple mapping that should
 * be good enough most of the time.
 *
 * This stuff should only be needed when we're emulating another printer like
 * the APS-5.
 *
 */


    for ( i = 0; fontmap[i].name != NULL; i++ )
	if ( strcmp(name, fontmap[i].name) == 0 )
	    return(fontmap[i].use);

    switch ( *++name )  {
	case 'I':
	case 'B':
		return(name);

	case 'X':
		return("BI");

	default:
		return("R");
    }	/* End switch */

}   /* End of mapfont */


/*****************************************************************************/


convint(c)


	unsigned char	c;


{


/*
 *
 * Converts a character to a proper 2's complement integer. Really just used
 * on 3b's when we're reading raster tables in the old format. There's
 * really no need to make this a separate routine - at least I don't
 * think there is. Should just redefine CONVINT() macro so it does this
 * stuff.
 *
 */


	return((c & 0200) ? ((256 - c) * -1) : c);

}   /* End of convint */


/*****************************************************************************/


t_init(reinit)


	int	reinit;


{


	int	i;
	FILE	*tmpfile();


/*
 *
 * Called to initialize the printer and associated variables. reinit will
 * be zero when the "x init" command was found in the output file, while
 * it will be non-zero when the routine is called to set things up for a
 * new page.
 *
 */


	if (! reinit) {
		for (i = 0; i < nchtab; i++)
			if (strcmp(&chname[chtab[i]], "l.") == 0)
				break;
		if (i < nchtab) {
			drawdot = i + 128;
			drawsize = 1;
		} else {
			drawdot = '.';
			drawsize = 2;	/* half size */
		}

		xfac = (float) pres / res * aspect;
		yfac = (float) pres / res;

		putc(ASETPEN, tf);
		putc(penwidth, tf);

		if ( mode == LANDSCAPE )  {
		    putc(ASETHV, tf);
		    putc(0107, tf);
		    angle = ROT_270;
		}   /* End if */
	}

	hpos = vpos = 0;
	setsize(t_size(10));	/* start somewhere */

}   /* End of t_init */


/*****************************************************************************/


t_page(pg)


{


	register int		i, j, n;
	register unsigned	char *p;


/*
 *
 * Get the printer and everything else ready for a new page. If the -o
 * option has been used only selected pages will be printed. If output
 * is ON when this routine is called we'll write the endpage Impress
 * command to the output file. If we're supposed to be doing output for
 * page pg we'll write the APAGE command out, and that will cause the
 * horizontal and vertical positions (in Impress) to be set to zero. hpos
 * and vpos are initialized in routine t_init(), although I really don't
 * see any reason why we couldn't just skip the call and do it here.
 *
 */


	if ( debug == ON )
		fprintf(stderr, "t_page %d, output=%d\n", pg, output);

	pageno = pg;

	if ( output == ON )		/* doing output for last page */
		putc(AENDP, tf);

	if ( (output = in_olist(pg)) == ON )  {		/* print this page */
		putc(APAGE, tf);
		acctpages++;
	}

	lastx = lasty = -1;
	t_init(1);			/* setup for this page */

}   /* End of t_page */


/*****************************************************************************/


t_newline()


{


/*
 *
 * Read an "n a b" command in the input file and are ready to start a new
 * line. There really isn't much else this routine can do besides set the
 * horizontal position to zero. Anyway troff takes care of any needed
 * positioning before it starts printing text.
 *
 */


	hpos = 0;

}   /* End of t_newline */


/*****************************************************************************/


t_size(n)


	int	n;			/* convert this point size */


{


	int	i;


/*
 *
 * Converts a point size into an internal size that can be used as an
 * index into the pstab[] array. Actually the internal size is defined as
 * one plus the index of the least upper bound of n in pstab[] or nsizes
 * if n is larger than all the listed sizes.
 *
 */

	if (n <= pstab[0])
		return(1);
	else if (n >= pstab[nsizes-1])
		return(nsizes);
	for (i = 0; n > pstab[i]; i++) ;

	return(i+1);

}   /* End of t_size */


/*****************************************************************************/


t_charht(n)


	int	n;			/* use this as the character height */


{


/*
 *
 * Although it can be done on some typesetters, like the APS-5, it's ignored
 * on Imagen's bitmap printers like the Imprint-10 and the 8/300. It would
 * be absurd to keep any extra raster files around just so we could implement
 * this command, and right now there's no Impress command to do it to
 * downloaded glyphs.
 *
 */


}   /* End of t_charht */


/*****************************************************************************/


t_slant(n)


	int	n;			/* slant characters this many degrees */


{


/*
 *
 * As with troff's height command, there's no simple way to slant characters
 * on Imagen's bitmap printers and it's really not worth any effort to try
 * and make it work.
 *
 */


}   /* End of t_slant */


/*****************************************************************************/


t_font(s)


	char	*s;


{


	int	n;


/*
 *
 * Just converts the string *s into an integer and checks to make sure the
 * number is a legal font position. If it's not we'll print an error message
 * and then quit. di10 handled bad requests a little differently. Instead
 * of treating mistakes as FATAL errors it would return 1 as the font
 * number.
 *
 */


	if ( (n = atoi(s)) < 0  ||  n > nfonts )
		error(FATAL, "illegal font position %d", n);

	return(n);

}   /* End of t_font */


/*****************************************************************************/


t_reset(c)


	int	c;			/* pause or restart */


{


/*
 *
 * Called from devcntrl() when we've found an "x stop" or "x pause" command.
 * There's really nothing we need to do for Imagen's bitmap printers, and
 * leaving the final cleanup (ie. AEOF code etc.) to someone else means
 * we should be able to cat a bunch of troff output files together and
 * have them all printed properly.
 *
 */


}   /* End of t_reset */


/*****************************************************************************/


t_wrapup()


{


/*
 *
 * We're finished with all the input files and all that's left to do is
 * end the last page we were working on and then mark the end of the Impress
 * file.
 *
 */


	putc(AENDP, tf);
	putc(AEOF, tf);

}   /* End of t_wrapup */


/*****************************************************************************/


t_trailer()


{


/*
 *
 * Called from devcntrl() when an "x trailer" command is found. There's
 * nothing we need to do here for Imagen's printers.
 *
 */



}   /* End of t_trailer */


/*****************************************************************************/


hgoto(n)


	int	n;			/* where we want to be */


{


/*
 *
 * Want to be at this absolute horizontal position next - usually will
 * get these motion commands right before printing a character.
 *
 */


	hpos = n;

}   /* End of hgoto */


/*****************************************************************************/


hmot(n)


	int	n;			/* move this far from where we are */


{


/*
 *
 * Handles relative horizontal motion. troff's current positon as recorded
 * in hpos is changed by n units.
 *
 */


	hpos += n;

}   /* End of hmot */


/*****************************************************************************/


vgoto(n)


	int	n;			/* new vertical position */


{


/*
 *
 * Moves vertically in troff's coordinate system to absolute position n.
 *
 */


	vpos = n;

}   /* End of vgoto */


/*****************************************************************************/


vmot(n)


	int	n;			/* move this far vertically */


{


/*
 *
 * Handles relative vertical motion of n units in troff's coordinate system.
 *
 */


	vpos += n;

}   /* End of vmot */


/*****************************************************************************/


put1s(s)


	register char	*s;


{


	static int	i = 0;		/* last one we found - usually */


/*
 *
 * Does whatever is necessary to have special character *s printed. If we're
 * doing output on this page we'll look the character up using chname[] and
 * chtab[]. The first array contains all the special character names
 * separated by '\0' that are mentioned in the typesetter's DESC file, while
 * chname[i] points to the start of character i in chname[]. Both arrays
 * come from the DESC.out file for printer *devname which is found in
 * *fontdir (wherever that might be).
 *
 */


	if ( output == OFF )
		return;

	if ( debug )
		printf("%s", s);

	if (strcmp(s, &chname[chtab[i]]) != 0)
		for (i = 0; i < nchtab; i++)
			if (strcmp(&chname[chtab[i]], s) == 0)
				break;
	if (i < nchtab)
		put1(i + 128);
	else
		i = 0;

}   /* End of put1s */


/*****************************************************************************/


put1(c)


    register int	c;		/* print this character */


{


    char		*pw;
    register char	*p;
    register int	i, j, k;
    int			ofont, code;


/*
 *
 * Arranges to have character c printed. If c < 128 it's a simple ASCII character,
 * otherwise it's a special character. We subtract 32 from c because non-graphic
 * ASCII characters aren't included in the tables.
 *
 */


    lastc = c;				/* charlib() may need the real name */
    c -= 32;

    if ( c <= 0 )  {
	if ( debug == ON )
	    fprintf(tf, "non-exist 0%o\n", lastc);
	return;
    }	/* End if */

    k = ofont = font;
    i = fitab[font][c] & BMASK;

    if ( i != 0 )  {			/* it's on this font */
	p = codetab[font];
	pw = widthtab[font];
    } else if ( smnt > 0 )  {		/* on special (we hope) */
	for ( k=smnt, j=0; j <= nfonts; j++, k = (k+1) % (nfonts+1) )  {
	    if ( k == 0 )  continue;
	    if ( (i = fitab[k][c] & BMASK) != 0 )  {
		p = codetab[k];
		pw = widthtab[k];
		setfont(k);
		break;
	    }	/* End if */
	}   /* End for */
    }	/* End else */

    if ( i == 0 || (code = p[i] & BMASK) == 0 || k > nfonts )  {
	if ( debug == ON )
	    fprintf(tf, "not found 0%o\n", lastc);
	if ( font != ofont ) setfont(ofont);
	return;
    }	/* End if */

    lastw = ((pw[i] & BMASK) * pstab[size-1] + dev.unitwidth/2) / dev.unitwidth;

    if ( debug == ON )  {
	if ( isprint(lastc) )
	    fprintf(tf, "%c %d\n", lastc, code);
	else fprintf(tf, "%03o %d\n", lastc, code);
    } else oput(code);

    if ( font != ofont )
	setfont(ofont);

}   /* End of put1 */


/*****************************************************************************/


setsize(n)


	int	n;			/* new internal size */


{


/*
 *
 * We want to use internal size n for now, where n is an index into *pstab
 * where we can find the closest available approximation to the requested
 * point size.
 *
 */


	size = n;

}   /* End of setsize */


/*****************************************************************************/


t_fp(n, s, si)


	int	n;			/* font position to update */
	char	*s;			/* it now contains this font */
	char	*si;			/* and this is its internal number */


{


/*
 *
 * Called when we've loaded font *s in position n. The font position info
 * is recorded in fontname[].
 *
 */


	fontname[n].name = s;
	fontname[n].number = atoi(si);
	if ( n == lastfont ) lastfont = -1;	/* force getfontdata() call */

}   /* End of t_fp */


/*****************************************************************************/


setfont(n)


	int	n;			/* this will be the current font */


{


/*
 *
 * Job now wants to use the font loaded in position n. The variable font
 * keeps track of which one we're currently using. If the requested positon
 * is out of range we'll just quit.
 *
 */


	if ( output == OFF )
		return;

	if (n < 0 || n > NFONT)
		error(FATAL, "illegal font %d", n);

	font = n;

}   /* End of setfont */


/*****************************************************************************/


oput(c)


	int	c;			/* want to print this character */


{


	int		x, y;		/* current scaled position */
	int		member;		/* glyph number for c in current family */


/*
 *
 * Arranges to print the character whose code is c in the current font.
 * The old and new raster file formats are supported.
 *
 */


	if ( output == OFF )
		return;

	if ( rastformat == OLD_FORMAT )  {
		xychar(c, fontname[font].name, pstab[size-1], lastw, tf);
		return;
	}    /* End if */

	if ( font != lastfont  ||  size != lastsize )  {
		if ( isresident(fontname[font].name) == FALSE )
			getrastdata(fontname[font].name, mapsize(fontname[font].name, pstab[size-1]));
		else getresdata(fontname[font].name, mapsize(fontname[font].name, pstab[size-1]), tf);
		lastfont = font;
		lastsize = size;
	}   /* End if */

	member = download(c, lastw, angle, tf);

	x = hpos * xfac + xoffset + .5;
	y = vpos * yfac + yoffset + .5;

	if ( y != lasty )  {
		putc(ASETAV, tf);
		putint(y, tf);
		lasty = y;
	}   /* End if */

	if ( ABS(x - lastx) > SLOP )  {
		putc(ASETAH, tf);
		putint(x, tf);
		lastx = x + fam->advance[member];
	} else lastx += fam->advance[member];

	putc(member, tf);

}   /* End of oput */


/*****************************************************************************/


mapsize(name, ps)


	char		*name;		/* name of the font */
	int		ps;		/* point size troff wants to use */


{


	int		i, j;


/*
 *
 * Uses the data that's been filled in from RASTLIST to map point size
 * ps font font *name into the best size available in our raster tables.
 * If no RASTLIST file was used rastsize will be zero and we'll just
 * assume that any size that's aske for is available.
 *
 */


	if ( maxrast == 0 )		/* don't have any size data */
		return(ps);

	for ( i = 0; i < maxrast; i++ )
		if ( strcmp(name, sizedata[i].name) == 0 )
			break;

	if ( i >= maxrast )		/* didn't find the font */
		error(FATAL, "missing raster list data for font %s", name);

	for ( j = 1, ps = (ps * pres) / rres; ps >= sizedata[i].sizes[j]; j++ ) ;

	return(sizedata[i].sizes[--j]);

}	/* End of mapsize */


/*****************************************************************************/


