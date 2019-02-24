/*	Copyright (c) 1988 AT&T
        All Rights Reserved
        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
        The copyright notice above does not evidence any
        actual or intended publication of such source code.

        Author:  Chi Chung Choy
	Revised: Michael Siemon
	Revised: Donald Knudsen
*/

/*	@(#)picasso:fonts.c	1.0	*/

#include "font.h"
#include "picasso.h"

extern	char	*fontdir;

extern	FILE	*fopen();
extern	int	fread(),	fclose();

int	devres = 720;
int	unitwidth = 10;
int	nfonts = 0;
char	**lfonts = NULL;

TrFont	*curfont;		/* not used in batch picasso */

int	fontcount =  0;

static	char	devfontpath[1024];

#define	NAMEMIN	6
char	*troffname[] = {
			"R",	"Times-Roman",
			"I",	"Times-Italic",
			"B",	"Times-Bold",
			"BI",	"Times-BoldItalic",
			"H",	"Helvetica",
			"HI",	"Helvetica-Oblique",
			"HB",	"Helvetica-Bold",
			"HX",	"Helvetica-BoldOblique",
			"CW",	"Courier",
			"CI",	"Courier-Oblique",
			"CB",	"Courier-Bold",
			"CX",	"Courier-BoldOblique",
			"PA",	"Palatino-Roman",
			"PI",	"Palatino-Italic",
			"PB",	"Palatino-Bold",
			"PX",	"Palatino-BoldItalic",
			"Hr",	"Helvetica-Narrow",
			"Hi",	"Helvetica-Narrow-Oblique",
			"Hb",	"Helvetica-Narrow-Bold",
			"Hx",	"Helvetica-Narrow-BoldOblique",
			"KR",	"Bookman-Light",
			"KI",	"Bookman-LightItalic",
			"KB",	"Bookman-Demi",
			"KX",	"Bookman-DemiItalic",
			"AR",	"AvantGarde-Book",
			"AI",	"AvantGarde-BookOblique",
			"AB",	"AvantGarde-Demi",
			"AX",	"AvantGarde-DemiOblique",
			"NR",	"NewCenturySchlbk-Roman",
			"NI",	"NewCenturySchlbk-Italic",
			"NB",	"NewCenturySchlbk-Bold",
			"NX",	"NewCenturySchlbk-BoldItalic",
			"ZD",	"ZapfDingbats",
			"ZI",	"ZapfChancery-MediumItalic",
			"GR",	"Symbol",
			(char *)0, (char *)0
	       };

#define	NDEFWID	96
static	int	defaultwidthtab[NDEFWID] = {
	25,	33,	41,	50,	50,	83,
	78,	33,	33,	33,	50,	56,
	25,	33,	25,	28,	50,	50,
	50,	50,	50,	50,	50,	50,
	50,	50,	28,	28,	56,	56,
	56,	44,	92,	72,	67,	67,
	72,	61,	56,	72,	72,	33,
	39,	72,	61,	89,	72,	72,
	56,	72,	67,	56,	61,	72,
	72,	94,	72,	72,	61,	33,
	28,	33,	47,	50,	33,	44,
	50,	44,	50,	44,	33,	50,
	50,	28,	28,	50,	28,	78,
	50,	50,	50,	50,	33,	39,
	28,	50,	50,	72,	50,	50,
	44,	48,	20,	48,	54,	0
};

static void		/* initialize with Times-Roman */
initdefaultfont()
{
	int i;
	extern	int	fcount, mcount;		/* fake with these */

	fontinit(tostring(troffname[0]));
	if (fontcount == 0)	{	/* normal initialization failed */
		mount[0]->name = tostring(troffname[0]);
		mount[0]->fontname = tostring(troffname[1]);
		mount[0]->nchars = NDEFWID;
		mount[0]->state = INMEMORY;
		mount[0]->mounted++;
		mount[0]->wp = (Chwid *)malloc(NDEFWID*sizeof(Chwid));
		for (i = 0; i < NDEFWID; i++) {
			mount[0]->wp[i].wid = defaultwidthtab[i];
			mount[0]->wp[i].num = i + 32;
		}
		fontcount = 1;
		fcount = mcount = 1;
	}
}

int
devinit(devname)		/* -1  means error; 1 means success */
	char	*devname;
{
	char	filename[1024];
	int	n;

	strcpy(filename, fontdir);
	strcat(filename, "/dev");
	strcat(filename, devname);
	strcpy(devfontpath, filename);
	strcat(filename, "/DESC");	/* "{FONTDIR}/dev{devname}/DESC" */
	if (getdesc(filename) == -1) {
		unitwidth = 10;
		devres = 720;
		initdefaultfont();
		return -1;
	}
	initdefaultfont();
	for (n = 1; n < nfonts; n++)
		fontinit(lfonts[n]);
	return(1);
}

double
checkfont(f)
	double	f;
{
	if (f < 0 || f > fontcount-1) {
		if (f != (int)f) {
			yyerror("%g is not a valid font index", f);
		}
		return 0.;
	}
	return f;
}

double
setfont(fontname)		/* accepts either troff or (some) PostScript */
	char	*fontname;	/* fontname, and initializes (mounts) it.    */
{
	int	i;

	if (fontname == NULL || *fontname == '\0')
		return 0.;
	if (strlen(fontname) >= NAMEMIN)
		for (i = 1; troffname[i] != (char *)0; i += 2)
			if (strcmp(fontname, troffname[i]) == 0) {
				free(fontname);
				fontname = tostring(troffname[i-1]);
				break;
			}
	for (i = 0; i < fontcount; i++)    /* linear search */
		if (strcmp(mount[i]->name, fontname) == 0)
			break;
	if (i == fontcount)
		if ((i = fontinit(fontname)) == -1) {
			yyerror("font %s not found", fontname);
			i = (int)getfval("textfont");
		}
	free(fontname);
	return (double)i;
}

static	int
fontinit(fontname)	/* 0 if there is error and default font will be used;
				1+ is the fontinfo index used */
	char	*fontname;
{
	char	filename[1024];

	if (fontcount == MAXFONTS)	/* reuse the last fontinfo slot */
		fontcount--;		/* if all the slots are used.   */
					/* (may want a yyerror msg here)*/
	strcpy(filename, devfontpath);
	strcat(filename, "/");
	strcat(filename, fontname);	/* "{devfontpath}/{fontname}" */
	if (mountfont(filename, fontcount) == -1)
		return 0;
	fontcount++;
	return(fontcount-1);
}

double
getstringwidth(s, fontnum, ptsize)	/* assumes no font or size changes */
	char	*s;
	int	fontnum, ptsize;
{
	double	width = 0.0;
	int	pos;

	fontnum = abs(fontnum);		/* negatives imply font or size attrs */
	ptsize  = abs(ptsize);		/* positives imply troff-style escapes*/
	curfont = mount[fontnum];
	for(; *s; ++s)
		if (*s > 31 && (pos = onfont(*s, fontnum)) != -1)
			width += chwidth(pos, fontnum);
	return width * ptsize / (unitwidth * devres);
}
