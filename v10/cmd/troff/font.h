/*
	font.h: characteristics of fonts
*/

typedef struct {	/* data on one character */
	short	num;		/* character number:
					0 -> not on this font
					>= 128 -> its number among all Cxy's */
	uchar	wid;		/* width */
	uchar	kern;		/* ascender/descender */
	short	code;		/* char code for actual device.  used for \N */
} Chwid;

typedef struct {	/* characteristics of a font */
	short	name;		/* short name, e.g., BI (2 chars) */
	char	longname[10];	/* long name of this font (e.g., "Bembo" */
	short	nchars;		/* number of width entries for this font */
	char	specfont;	/* 1 == special font */
	short	spacewidth;	/* width of space on this font */
	Chwid	*wp;		/* widths, etc., of the real characters */
	char	ligfont;	/* 1 == ligatures exist on this font */
} Font;

/* ligatures, ORed into ligfont */

#define	LFF	01
#define	LFI	02
#define	LFL	04
#define	LFFI	010
#define	LFFL	020

extern	int	nchnames;	/* number of Cxy names currently seen */
extern	char	*chnames[];	/* chnames[n-128] -> name of char n */
extern	char	*chname();	/* maps n >= 128 to name of char */
