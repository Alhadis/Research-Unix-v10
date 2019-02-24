
/*
 *
 * Some definitions used by the troff post-processor dimpress. The stuff at
 * the beginning of the file is rather general and you probably won't have
 * to change any of it. The printer dependent stuff can all be found at the
 * end of the file.
 *
 */


#define NFONT		60		/* max number of font positions */


/*
 *
 * These guys are used set the program exit status. If everything we're
 * supposed to do gets finished the exit status will eventually be set to
 * zero.
 *
 */


#define DO_ACCT		1		/* still want to do job accounting */
#define FILE_STARTED	2		/* started file but didn't finish yet */
#define NO_ACCTFILE	8		/* couldn't open the accounting file */


/*
 *
 * Landscape mode has been added to the post-processor. It's set using the
 * -l option.
 *
 */


#define PORTRAIT	0
#define LANDSCAPE	1


/*
 *
 * SLOP controls how much horizontal positioning error we'll accept. It's
 * used when we're printing glyphs to check if the troff and the printer
 * have gotten too far out of sync.
 *
 */


#define SLOP		1		/* not acccepting much error */


/*
 *
 * I've written the post-processor so it can handle raster files in both
 * the old format and in Imagen's Rst format. Obviously different routines
 * need to be called depending which style we're using. These definitions
 * are used in dimpress.c to figure out which routines to call.
 *
 */


#define OLD_FORMAT	0
#define RST_FORMAT	1


/*
 *
 * The raster files in the old format (not Imagen's) weren't portable and
 * neither was much of the code used to read them. CONVINT() is a macro
 * that we found was needed on 3b's, but only when we were using the old
 * files.
 *
 */


#ifdef u3b
#define CONVINT(C)	convint(C)
#else
#define CONVINT(C)	C
#endif


/*
 *
 * The rest of the stuff in this file deals with the different printers
 * directly supported by the post-processor.
 *
 * There really are only five or six variables in the post-processor that
 * are printer dependent. They include prname, pres, xoff, yoff, penwidth,
 * and possibly bitdir.
 *
 * This structure keeps track of the printer dependent values. An array
 * of Prdata is defined in dimpress.c and initialized using PR_INIT below.
 * The values of the corresponding program variables are set in initialize()
 * after the options are read.
 *
 */


typedef struct  {

    char	*prname;		/* name of the printer - *realdev */
    int		pres;			/* resolution of the printer */
    float	xoff;			/* reasonable values for the offsets */
    float	yoff;			/* in inches */
    int		penwidth;		/* decent looking pen diameter */
    char	*bitdir;		/* raster table directory */

} Prdata;


/*
 *
 * PR_INIT is used in dimpress.c to initialize the Prdata array. Its data
 * is given next. The list of printers must be terminated by an entry with
 * the prname field set to NULL.
 *
 */


#define PR_INIT								\
									\
	{								\
	    { "im300", 300, .20, 0.0, 3, BITDIR },			\
	    { "i300", 300, .20, 0.0, 3, BITDIR },			\
	    { "i240", 240, .41, 0.0, 2, BITDIR },			\
	    { "i480", 480, .41, 0.0, 4, BITDIR },			\
	    { "i10", 240, .41, 0.0, 2, OLDBITDIR },			\
	    { "a490", 300, .10, 0.0, 3, BITDIR },			\
	    { NULL, 0, 0, 0, 0, NULL }					\
	}


/*
 *
 * This guy is used to initialize pr_num in dimpress.c. It will be the 'line'
 * PR_INIT (actually the index in prdata[]) that will be used as the default
 * printer.
 *
 */


#define PR_NUM		0


/*
 *
 * Printer emulation, especially for the APS-5, is important at Murray Hill
 * and our users expect we'll do the best we can in showing font and size
 * changes. The next typedef is used to help us map troff fonts for other
 * devices into what's available on Postscript printers.
 *
 */


typedef struct {

	char	*name;			/* font name we're looking for */
	char	*use;			/* and this is what we should use */

} Fontmap;


/*
 *
 * The next definition is used to initialize the default Fontmap array in dpost.
 * It's not meant to include all the fonts available on every printer. It's
 * just a reasonable default that you can override with special fontmap files
 * for any device. In particular if file *fontdir/devpost/fontmaps/*devname
 * exists, it's used instead of the deafult map defined here. The list must end
 * with an entry that has NULL defined for the name field.
 *
 */


#define	FONTMAP								\
									\
	{								\
	    "G", "H",							\
	    "LO", "S",							\
	    "S2", "S",							\
	    "GI", "HI",							\
	    "HM", "H",							\
	    "HK", "H",							\
	    "HL", "H",							\
	    "PA", "R",							\
	    "PI", "I",							\
	    "PB", "B",							\
	    "PX", "BI",							\
	    NULL, NULL,							\
	}


