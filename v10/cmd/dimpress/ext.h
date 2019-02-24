
/*
 *
 * External definitions for the variables defined in glob.c. I've also
 * included a few commonly used function declarations just for convenience.
 *
 */


extern char	**argv;			/* global so everyone can use them */
extern int	argc;

extern char	*prog_name;		/* really just for error messages */

extern int	x_stat;			/* program exit status */
extern int	debug;			/* debug flag */
extern int	ignore;			/* what we do with FATAL errors */
extern long	lineno;			/* really just for post-processor */

extern char	*fontdir;		/* troff's binary font table directory */
extern char	*bitdir;		/* rast* directories found here */
extern char	*rastdir;		/* Imagen raster table directory */
extern char	*resfile;		/* control file in *bitdir/RESIDENT */


extern char	*mapfont();
extern char	*malloc();
extern char	*rastalloc();
extern char	*cuserid();

