
/*
 *
 * Some important definitions used by the program that builds raster tables
 * and troff's ASCII files from Imagen supplied raster files.
 *
 */


#define	L_CHNAME	2		/* longest special character name */
#define MAX_INDEX	128		/* largest allowed glyph number */
#define GLYDIR_SIZE	15 * MAX_INDEX	/* enough room for a whole directory */


typedef struct  {

	char	name[L_CHNAME+1];	/* special char names saved here */

} Charset;

typedef struct  {

	char	name[L_CHNAME+1];	/* name of the special character */
	char	*map;			/* address of the bitmap */
	int	mapsize;		/* size in bytes of the bitmap */
	int	chwidth;		/* new char width if >= 0 */
	char	*glydir;		/* new glyph directory entry */
	char	rastname[15];		/* glyph comes from this file */
	int	index;			/* glyph number in *rastname */
	char	synonyms[100];		/* synonyms that go in font file */

} Charinfo;


extern unsigned	readvalue();


