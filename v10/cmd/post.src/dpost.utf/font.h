/*
 *
 * Characteristics of a font - for postprocessors. Most changes are for
 * Unicode and Plan 9 UTF encoding. Also tried to anticipate larger and
 * more general Unicode fonts.
 * 
 */

#define FIRSTCODE	0x0020
#define LASTCODE	0x8BFF
#define FIRSTSPECIAL	0xE800
#define LASTSPECIAL	0xFDFF

#define INVALIDCODE	0x0000
#define LARGESTFONT	0x1000
#define MAXFONTS	99
#define	SPECIALCHARS	(LASTSPECIAL-FIRSTSPECIAL+1)

#define ValidCode(c)	((c) >= FIRSTCODE && (c) <= LASTCODE)
#define ValidSpecial(c)	((c) >= FIRSTSPECIAL && (c) <= LASTSPECIAL)
#define ValidChar(c)	(ValidCode(c) || ValidSpecial(c))

/*
 *
 * State of a table entry in fonts[].
 *
 */

#define NEWFONT		0
#define RELEASED	1
#define INMEMORY	2

/*
 *
 * Flags - for setting bits in a font's flag field.
 *
 */

#define USED		1
#define NAMED		2

#define skipline(f)	while ( getc(f) != '\n' )

/*
 *
 * Data about each character on a font. Omitted the ascender/descender field.
 * Need 16 bits for num field to support Unicode character set. Use the same
 * for code field which may be translated to Plan 9's UTF represenation on
 * output. A string for code field might be worth a try? Needs considerable
 * thought!!
 *
 */

typedef struct {
	unsigned short	num;		/* INVALIDCODE means not on this font */
	unsigned char	wid;		/* width */
	unsigned short	code;		/* code for actual device. */
} Chwid;

/*
 *
 * Font header - one for each available position. first and last fields
 * are an attempt to anticipate support for Unicode fonts. Characters
 * from first through last are described by the initial last-first+1
 * entries in wp. Entries in wp having num == INVALIDCODE are unassigned
 * in the font. The remaining nchars-(last-first+1) wp entries support
 * troff's traditional two character escapes. Special characters are
 * hashed and assigned codes from Unicode's Private Use Area.
 *
 */

typedef struct {
	char		*path;		/* where it came from */
	char		*name;		/* as known to troff */
	char		*fontname;	/* real name (e.g. Times-Roman) */
	char		state;		/* NEWFONT, RELEASED, or INMEMORY */
	char		flags;		/* for now just USED and NAMED */
	char		mounted;	/* mounted on this many positions */
	char		specfont;	/* 1 == special font */
	unsigned short	first;		/* first wp entry is for this character */
	unsigned short	last;		/* last code for this font */
	unsigned short	nchars;		/* size of width table for this font */
	Chwid		*wp;		/* widths, etc., of the real characters */
} Font;

/*
 *
 * Non-integer functions.
 *
 */

extern char	*chname();
extern char	*strsave();
extern char	*allocate();

