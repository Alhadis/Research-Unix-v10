
/*
 *
 * Some definitions and structure declarations used to manage the raster
 * files (Imagen's format) and printer resident fonts.
 *
 * I've changed things a little from the way the raster tables were
 * handled in di10. No RASTERLIST file is needed. If it doesn't exist the
 * troff post-processor assumes every font can be printed in all the
 * sizes listed in the DESC file. That wouldn't have been a realistic
 * assumption for the old raster tables, but it's probably pretty
 * good for the one's Imagen supplies. Anyway even if there are a
 * few missing files we can always link them to ones we really want
 * to use. While that may be a little less efficient, at least in terms
 * of downloading glyphs and memory usage on the printer, I don't think
 * it will be too bad as long as the raster files are fairly complete.
 *
 */


#include "impcodes.h"			/* just for definition of ROT_COUNT */


/*
 *
 * Impress (version 1.9) restricts the families that we can define to
 * the range 0 to 95. MAXFAMILY is the upper limit of the range and
 * should not be changed unless this limit is removed. Family members are
 * also restricted and must lie between 0 and 127. Don't ever change the
 * value of MAXMEMBER as defined below.
 *
 */


#define MAXFAMILY	95		/* most families we can ever use */
#define MAXMEMBER	255		/* largest member in any family */


/*
 *
 * L_FNAME is the maximum length of the font name string stored in
 * in field name[] below. It probably should agree with the size of
 * array namefont[] as defined in troff'd dev.h structure.
 *
 */


#define L_FNAME		10		/* longest allowed font name + 1 */
#define MAX_SIZES	20		/* only used in Sizedata */


/*
 *
 * You can change any of the next three definitions to suit your own
 * setup. They're only limits on array sizes used to keep track of
 * font data.
 *
 */


#define MAXRESIDENT	20		/* most resident fonts we'll handle */
#define MAXFAMTAB	5		/* limit on family tables per font */
#define MAXFONTS	100		/* limit on total number of fonts */


/*
 *
 * We'll have to keep track of some extra stuff for printer resident
 * fonts. Obviously we'll need its name but as it happens we'll also
 * need to know the family table that should be used with each font.
 * It's a shame that things have to be so complicated. If the characters
 * in the resident fonts could all be accessed using codes 0 to 127
 * then map 0 and the font tables would suffice. Anyway it doesn't
 * turn out that way, at least not as far as I can figure, and so we'll
 * need some rather complicated member maps and family tables. All this
 * stuff is handled by the routines in file res.c.
 *
 */


typedef struct  {

	char	name[L_FNAME];		/* name of the resident font */
	int	count;			/* number of table entries below */

	struct  {			/* family tables for each font */
		int	map;		/* use this map */
		char	name[L_FNAME];	/* with this resident font */
	} table[MAXFAMTAB];

} Resdata;


/*
 *
 * As I mentioned earlier no RASTERLIST file is needed, but it can be
 * used. We also may have to deal with printer resident fonts and may
 * have to keep track of font size data for these guys. The following
 * data structure is used to manage raster size data in dimpress.
 *
 */


typedef struct  {

	char	name[L_FNAME];		/* internal font name */
	int	sizes[MAX_SIZES];	/* available sizes */

} Sizedata;


/*
 *
 * The rastdata structure is used to keep track of which raster files
 * have already been read into memory. If rst isn't NULL then we assume
 * it points to a block of memory containing the raster file *name.size.
 * Note that *name probably won't be the name of the font troff is using,
 * but rather it's the name of the raster file that we want to use.
 * It's filled in from the namefont[] string stored in the appropriate
 * binary font file - at least that's the way it will be done in the
 * troff post-processor. That being the case we need to make sure that
 * L_FNAME and the size of namefont[] as defined in dev.h agree.
 *
 * I've added glyph rotation information to Rastdata. We'll really only
 * need to keep track of family numbers and chused bit maps for each
 * allowed orientation. Actually we can probably expect that troff
 * jobs will only use a single orientation for each job, but that's
 * definitely not the case for any kind of graphics preprocessor.
 *
 */


typedef struct  {

    int			size;		/* point size that's been read in */
    int			first;		/* first character number */
    int			last;		/* number of the last character */
    int			mag;		/* font magnification times 1000 */
    int			glyphdir;	/* glyph directory offset */
    int			*advance;	/* downloaded this as the advance */
    char		name[L_FNAME];	/* name of font's raster file */
    char		*rst;		/* pointer to font's RST file */
    int			fam[ROT_COUNT];	/* Impress family number */
    unsigned char	*chused[ROT_COUNT];	/* bit indexed; 1 if downloaded */

} Rastdata;


/*
 *
 * Imagen's raster file format is described in the Imprint-10 Programmer's
 * Manual that I got with our original printer (back in 1982). Although
 * that described a Version 0 system it's the best info I've got right
 * now so that's what I'll use.
 *
 * The raster files all contain 4 sections called the file mark, the
 * preamble, the glyph directory, and the raster data section. We're
 * really only concerned with the last 3. The preamble describes some
 * general information about the font. One of the most important
 * fields, at least the way I've written things, is the pointer to
 * the glyph directory. It begins at offset 11 and is 3 bytes long.
 * That's what we'll use to get to the glyph directory, which contains
 * specific data about each of the characters represented in the
 * raster file. Each entry in the glyph directory is 15 bytes long.
 * Knowing the number of the character that we want to print and the
 * pointer to the start of the glyph directory lets us locate that
 * character's entry in the glyph directory. The raster data section
 * starts at the end of the glyph directory and contains the bitmaps
 * used to print the characters represented in the file. Each character's
 * entry in the glyph directory has a pointer to the start of it's
 * bitmap in the raster data section, along with enough other data
 * so we can figure out where the bitmap ends.
 *
 * To use Imagen's raster files we need to be able locate a particular
 * field and then decode it. That means we need to be able to get the
 * field's offset from the beginning of the file and then determine how
 * many bytes are used by the field. The main tool I'll be using is a
 * structure called Rst (defined below). There will be one associated
 * data structure for the preamble and the glyph directory. The declaration
 * and initialization of these Rst arrays is handled in file rast.c
 * using the definitions given here.
 *
 * I'll begin by giving the Rst structure and a few other associated
 * definitions. The definitions are used to classify field types, while the
 * the Rst structure is designed to enable us to pick fields out of a given
 * raster file once it's been read into memory. size is the size in bytes
 * of each field, offset is where it starts, usually relative to the
 * beginning of the raster file. If glyphdir is TRUE then the field is
 * in the glyph directory and we'll need to add in the directory's offset
 * before we get to the right place. type describes the field and allows
 * us to decode it properly. Most fields will be unsigned, although glyph
 * reference points are signed integers. strings are a little tougher but
 * we really don't need them, at least not right now, so I'll ignore them.
 * They're all variable length and are only found in the preamble. They
 * could be handled by letting offset field point to the first string and
 * the size field be the number of strings that preceed the one we're
 * looking for. Imagen encodes strings a little differently than we're
 * used to. They're not terminated by '\0', but rather the first byte
 * in each string gives the number of bytes in the rest of the string.
 *
 */


#define INTEGER		0
#define UNSIGNED	1
#define STRING		2


typedef struct  {

    int		size;			/* bytes used by the field */
    int		offset;			/* field's offset */
    int		type;			/* STRING, INTEGER, or UNSIGNED */
    int		glyphdir;		/* add in directory offset? */

} Rst;


/*
 *
 * Next we'll need a few definitions that go with the preamble. The
 * first set, ie. the ones beginning with the characters P_ are
 * indices into the Rst array rst[] (defined in rast.c), where we can
 * locate size and offset information about a particular field. There's
 * really no need to ever change any of this stuff - all it has to do is
 * agree with the way RST_INIT is defined. RST_INIT is used in rast.c
 * to do the initialization of the Rst array that's used to define raster
 * structures.
 *
 */


#define P_LENGTH	0		/* length of the preamble - bytes */
#define P_VERSION	1		/* format version number */
#define P_GLYDIR	2		/* pointer to glyph directory */
#define P_FIRSTGLY	3		/* number of first glyph in font */
#define P_LASTGLY	4		/* same but for last glyph */
#define P_MAG		5		/* font magnification */
#define P_SIZE		6		/* design size of the font */
#define P_LINESP	7		/* interline spacing */
#define P_WORDSP	8		/* interword spacing */
#define P_ROT		9		/* rotation of the font */
#define P_CHADV		10		/* character advance direction */
#define P_LINEADV	11		/* line advance direction */
#define P_CHECK		12		/* check identifier */
#define P_RES		13		/* font resolution */
#define P_IDENT		14		/* font identifier string */
#define P_FACE		15		/* font type face encoding - string */
#define P_DEVICE	16		/* intended output device - string */
#define P_CREATOR	17		/* creator of the file - string */


/*
 *
 * We'll need to do the same kind of thing for the glyph directory entries.
 * The next set of definitions all begin with G_ and define the positions
 * in the Rst array where we can find size, offset, type, and glyphdir
 * information for each of the fields. Again remember these are just array
 * indices that need to agree with the way RST_INIT is defined.
 *
 */


#define G_HEIGHT	18		/* pixel height of raster image */
#define G_WIDTH		19		/* pixel width of raster image */
#define G_YREF		20		/* y reference point */
#define G_XREF		21		/* x reference point */
#define G_CHWIDTH	22		/* character width in fix units */
#define G_BPTR		23		/* pointer to raster data */


/*
 *
 * I've already mentioned RST_INIT, which is used in rast.c to initialize
 * the Rst array. It's definition is given next and it needs to agree with
 * the P_ and G_ definitions just given. In otherwords the data describing
 * field P_XXX really better be in position rst[P_XXX] or things will
 * really get messed up.
 *
 */


#define RST_INIT						\
								\
	{							\
	    { 2, 8, UNSIGNED, FALSE },		/* P_LENGTH */			\
	    { 1, 10, UNSIGNED, FALSE },		/* P_VERSION */			\
	    { 3, 11, UNSIGNED, FALSE },		/* P_GLYDIR */			\
	    { 2, 14, UNSIGNED, FALSE },		/* P_FIRSTGLY */		\
	    { 2, 16, UNSIGNED, FALSE },		/* P_LASTGLY */			\
	    { 4, 18, UNSIGNED, FALSE },		/* P_MAG */			\
	    { 4, 22, UNSIGNED, FALSE },		/* P_SIZE */			\
	    { 4, 26, UNSIGNED, FALSE },		/* P_LINESP */			\
	    { 4, 30, UNSIGNED, FALSE },		/* P_WORDSP */			\
	    { 2, 34, UNSIGNED, FALSE },		/* P_ROT */			\
	    { 1, 36, UNSIGNED, FALSE },		/* P_CHADV */			\
	    { 1, 37, UNSIGNED, FALSE },		/* P_LINEADV */			\
	    { 4, 38, UNSIGNED, FALSE },		/* P_CHECK */			\
	    { 2, 42, UNSIGNED, FALSE },		/* P_RES */			\
	    { 0, 44, STRING, FALSE },		/* P_IDENT */			\
	    { 1, 44, STRING, FALSE },		/* P_FACE */			\
	    { 2, 44, STRING, FALSE },		/* P_DEVICE */			\
	    { 3, 44, STRING, FALSE },		/* P_CREATOR */			\
	    { 2, 0, UNSIGNED, TRUE },		/* G_HEIGHT */			\
	    { 2, 2, UNSIGNED, TRUE },		/* G_WIDTH */			\
	    { 2, 4, INTEGER, TRUE },		/* G_YREF */			\
	    { 2, 6, INTEGER, TRUE },		/* G_XREF */			\
	    { 4, 8, UNSIGNED, TRUE },		/* G_CHWIDTH */			\
	    { 3, 12, UNSIGNED, TRUE }		/* G_BPTR */			\
	}


/*
 *
 * It turns out to be convenient to be able to gather all the important
 * glyph information together in a single place. The Glyph structure
 * includes all the important data a glyph.
 *
 */


typedef struct  {

    int		xref;			/* reference point */
    int		yref;
    int		width;			/* bitmap dimensions */
    int		height;
    int		advance;		/* move this far after printing */
    char	*bptr;			/* character's bitmap */

} Glyph;


/*
 *
 * In order to locate a particular glyph's entry in the glyph directory
 * we'll need to know the pointer to the start of the directory and the
 * size of each glyph entry in the directory. It's clear from the definition
 * of RST_INIT that this size, at least right now, is 15 bytes. The size
 * is simply size+offset of the last field, which in this case is G_BPTR.
 * The pointer to glyph n's entry is given by,
 *
 *                 DirPtr + ((n - fg) * 15)
 *
 * where DirPtr is the pointer to the start of the directory and fg is the
 * number of the first glyph in the directory (usually 0). The following
 * macro is used in rast.c to get the entry pointer for each glyph.
 *
 */


#define GLYPH_PTR(A)							\
									\
	(fam->glyphdir + ((A - fam->first) * 15))


/*
 *
 * A lot of the numbers in Imagen's raster tables are given in units called
 * a fix, where 2**20 fixes are equal to a point. The next macro can be used
 * to convert fix units to inches.
 *
 */


#define FIX(A)								\
									\
	((float) A / ((01 << 20) * 72.27))


/*
 *
 * The character width for glyphs in Imagen's raster tables are stored in
 * fix units without font magnification figured in. The following macro
 * takes a fix width and converts it to a pixel width.
 *
 */


#define PIXEL_WIDTH(A, B)						\
									\
	(int) ((FIX(A) * fam->mag * B) / 1000.0 + 1.0)


/*
 *
 * The raster tables we're using are assumed to have been prepared for
 * printing at resolution RAST_RES. The data is supposedly already in each
 * of the raster files as field P_RES, but I'm not positive the value is
 * right. Everything except that field seems to indicate that the raster
 * tables are meant to be printed at 300 dots per inch.
 *
 */


#define RAST_RES	300


/*
 *
 * Finially a few external definitions that will be needed by programs
 * that deal with Imagen's raster files. They're all declared in rast.c.
 *
 */


extern Rst		rst[];		/* raster file structure info */

extern Rastdata		fam_data[];	/* data on raster files we've read */
extern Rastdata		*fam;		/* &fam_data[cur_fam] */
extern int		next_fam;	/* next available family number */
extern int		cur_fam;	/* family we're using right now */
extern int		last_fam;	/* last family we told printer about */
extern int		rast_res;	/* raster table resolution */

extern unsigned		getvalue();


