/*
 *
 * Characteristics of a font - for postprocessors. Differs some from the troff
 * version.
 *
 */

#define MAXCH		512
#define MAXFONTS	99

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
 * An unsigned char is good enough for the code field on PostScript printers.
 *
 */

typedef struct {
	short		num;	/* 0 means not on this font */
	unsigned char	wid;	/* width */
	unsigned char	code;	/* code for actual device. */
} Chwid;

/*
 *
 * Font header - one for each available position.
 *
 */

typedef struct {
	char	*path;		/* where it came from */
	char	*name;		/* as known to troff */
	char	*fontname;	/* real name (e.g. Times-Roman) */
	char	state;		/* NEWFONT, RELEASED, or INMEMORY */
	char	flags;		/* for now just USED and NAMED */
	char	mounted;	/* mounted on this many positions */
	char	specfont;	/* 1 == special font */
	short	nchars;		/* number of width entries for this font */
	Chwid	*wp;		/* widths, etc., of the real characters */
} TrFont;

extern	TrFont	*mount[];
/*
 *
 * Non-integer functions.
 *
 */

extern char	*chname();
extern char	*strsave();
extern char	*allocate();

