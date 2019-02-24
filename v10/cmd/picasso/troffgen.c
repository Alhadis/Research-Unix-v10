/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:troffgen.c	1.0	*/

#include <string.h>
#include <ctype.h>
#include "font.h"
#include "picasso.h"
#include "y.tab.h"

char	esc	    = '\\';
char	eqn_delim[] = "\0";

/* the following table of troff names for special characters is in ASCII   */
/* order of their names, so that a binary search could be done if desired. */
/* the font field MUST conform to the font initialization in fonts.c, with */
/* the "standard" Times Roman in position 0 and the symbol font as font 1. */

struct	trcode	{char name[2];  char font;  unsigned char value;}
	trcode[]  =
		{'\0','\0','\0','\0',		/* null entry, never matched. */
		'!','=', 1, 0271,		/* not equal */
		'!','b', 1, 0313,		/* not subset */
		'!','m', 1, 0317,		/* not member */
		'"','a', 0, 0315,		/* hungarumlaut accent */
		'\'','\'',0,0272,		/* right double quote */
		'*','*', 1, '*',		/* math star */

		'*','A', 1, 'A', '*','B', 1, 'B', '*','C', 1, 'X',   /* greek */
		'*','D', 1, 'D', '*','E', 1, 'E', '*','F', 1, 'F',   /* alpha-*/
		'*','G', 1, 'G', '*','H', 1, 'Q', '*','I', 1, 'I',   /* bet.  */
		'*','K', 1, 'K', '*','L', 1, 'L', '*','M', 1, 'M',
		'*','N', 1, 'N', '*','O', 1, 'O', '*','P', 1, 'P',
		'*','Q', 1, 'Y', '*','R', 1, 'R', '*','S', 1, 'S',
		'*','T', 1, 'T', '*','U', 1, 'U', '*','W', 1, 'W',
		'*','X', 1, 'C', '*','Y', 1, 'H', '*','Z', 1, 'Z',
		'*','a', 1, 'a', '*','b', 1, 'b', '*','c', 1, 'x',
		'*','d', 1, 'd', '*','e', 1, 'e', '*','f', 1, 'f',
		'*','g', 1, 'g', '*','h', 1, 'q', '*','i', 1, 'i',
		'*','k', 1, 'k', '*','l', 1, 'l', '*','m', 1, 'm',
		'*','n', 1, 'n', '*','o', 1, 'o', '*','p', 1, 'p',
		'*','q', 1, 'y', '*','r', 1, 'r', '*','s', 1, 's',
		'*','t', 1, 't', '*','u', 1, 'u', '*','w', 1, 'w',
		'*','x', 1, 'c', '*','y', 1, 'h', '*','z', 1, 'z',

		'+','-', 1, 0261,		/* plus-minus */
		',','a', 0, 0313,		/* cedilla accent */
		'-','>', 1, 0256,		/* right arrow */
		'-','a', 0, 0305,		/* macron accent */
		'.','a', 0, 0307,		/* dot accent */
		':','a', 0, 0310,		/* dieresis accent */
		'<','-', 1, 0254,		/* left arrow */
		'<','=', 1, 0243,		/* less than or equal */
		'=','=', 1, 0272,		/* identically equal */
		'>','=', 1, 0263,		/* greater than or equal */
		'C','O', 1, 0343,		/* copyright (sanserif) */
		'C','R', 1, 0277,		/* carriage return */
		'C','a', 0, 0316,		/* ogonek accent */
		'I','f', 1, 0301,		/* I (fraktur) */
		'L','B', 1, 0350,		/* paren, left bottom */
		'L','T', 1, 0346,		/* paren, left top */
		'O','+', 1, 0305,		/* circled plus */
		'O','x', 1, 0304,		/* circled times */
		'R','B', 1, 0370,		/* paren, right bottom */
		'R','G', 1, 0342,		/* registered (sanserif) */
		'R','T', 1, 0366,		/* paren, right top */
		'R','f', 1, 0302,		/* R (fraktur) */
		'T','M', 1, 0344,		/* trademark (sanserif) */
		'U','a', 0, 0306,		/* breve accent */
		'^','a', 0, 0303,		/* circumflex accent */
		'`','`', 0, 0252,		/* left double quote */
		'a','a', 0, 0302,		/* circumflex accent */
		'a','b', 1, 0253,		/* arrow both (left & right) */
		'a','f', 1, 0300,		/* aleph */
		'a','h', 1, 0276,		/* arrow horizontal extender */
		'a','n', 1, 0320,		/* angle */
		'a','p', 1, '~',		/* approximates */
		'a','v', 1, 0275,		/* arrow vertical extender */
		'b','<', 1, 0341,		/* left angle bracket */
		'b','>', 1, 0361,		/* right angle bracket */
		'b','u', 0, 0267,		/* bullet */
		'b','v', 1, 0352,		/* bold vertical (extender) */
		'c','.', 1, 0327,		/* centered dot */
		'c','a', 1, 0307,		/* cap */
		'c','g', 1, '@',		/* congruent to */
		'c','o', 1, 0323,		/* copyright (serif) */
		'c','r', 0, 0250,		/* currency */
		'c','t', 0, 0242,		/* cent */
		'c','u', 1, 0310,		/* cup */
		'd','a', 1, 0257,		/* down arrow */
		'd','d', 0, 0263,		/* double dagger */
		'd','e', 1, 0260,		/* degree */
		'd','g', 0, 0262,		/* dagger */
		'd','i', 1, 0270,		/* divide */
		'e','l', 0, 0274,		/* ellipsis */
		'e','m', 0, 0320,		/* emdash */
		'e','n', 0, 0261,		/* endash */
		'e','q', 1, '=',		/* equals */
		'e','s', 1, 0306,		/* empty set */
		'f','a', 1, '"',		/* for all */
		'f','i', 0, 0256,		/* fi ligature */
		'f','l', 0, 0257,		/* fl ligature */
		'f','m', 1, 0242,		/* minute (footmark) */
		'f','n', 0, 0246,		/* florin */
		'f','r', 0, 0244,		/* fraction bar */
		'g','a', 0, 0301,		/* circumflex accent */
		'g','r', 1, 0321,		/* gradient */
		'h','y', 0, '-',		/* hyphen */
		'i','b', 1, 0315,		/* improper subset  */
		'i','f', 1, 0245,		/* infinity */
		'i','p', 1, 0312,		/* improper superset */
		'i','s', 1, 0362,		/* integral sign */
		'l','&', 1, 0331,		/* logical and */
		'l','b', 1, 0356,		/* brace, left bottom */
		'l','c', 1, 0351,		/* square bracket l. ceiling */
		'l','f', 1, 0353,		/* square bracket l. floor */
		'l','k', 1, 0355,		/* brace, left middle */
		'l','t', 1, 0354,		/* brace, left top */
		'l','z', 1, 0340,		/* lozenge */
		'l','|', 1, 0332,		/* logical or */
		'm','i', 1, '-',		/* minus */
		'm','o', 1, 0316,		/* member of */
		'm','u', 1, 0264,		/* multiply */
		'n','o', 1, 0330,		/* logical not */
		'o','a', 0, 0312,		/* ring accent */
		'o','r', 1, '|',		/* or (vertical bar) */
		'p','d', 1, 0266,		/* partial derivative */
		'p','g', 0, 0266,		/* paragraph */
		'p','l', 1, '+',		/* plus */
		'p','p', 1, '^',		/* perpendicular */
		'p','s', 0, 0243,		/* pound sterling */
		'p','t', 1, 0265,		/* proportional to */
		'r','b', 1, 0376,		/* brace, right bottom */
		'r','c', 1, 0371,		/* square bracket r. ceiling */
		'r','f', 1, 0373,		/* square bracket r. floor */
		'r','g', 1, 0322,		/* registered (serif) */
		'r','k', 1, 0375,		/* brace, right middle */
		'r','n', 1, '\'',		/* root extender (en) */
		'r','t', 1, 0374,		/* brace, right top */
		's','b', 1, 0314,		/* subset */
		's','c', 0, 0247,		/* section */
		's','l', 1, '/',		/* slash */
		's','p', 1, 0311,		/* superset */
		's','r', 1, 0326,		/* square root */
		's','t', 1, '`',		/* such that */
		't','e', 1, '$',		/* there exists */
		't','f', 1, '\\',		/* therefore */
		't','m', 1, 0324,		/* trademark (serif) */
		't','s', 1, 'V',		/* terminal sigma */
		'u','a', 1, 0255,		/* up arrow */
		'u','l', 1, '_',		/* underscore */
		'v','a', 0, 0317,		/* caron accent */
		'w','s', 1, 0303,		/* Weierstrass P */
		'y','n', 0, 0245,		/* yen */
		'~','a', 0, 0304,		/* tilde accent */
		'~','~', 1, 0273,		/* approximately */

		'\0','\0','\0','\0',		/* sentinel; loop terminus */
		};

int	troffcode(str)
	char	*str;
{
	int	n;

	for (n = 1; trcode[n].name[0] != '\0'; n++)
		if (str[0] == trcode[n].name[0] && str[1] == trcode[n].name[1])
			return n;
	return 0;
}

static
troffparm(name, parm, prev)
	char	*name, *parm;
	int	*prev;			/* the previous value */
{
extern	double	atof();
	double	r, s=0, t;

	while (isspace(*parm))
		++parm;
	t = getfval(name);
	if (*parm == '\0')
		r = *prev;
	else {
		r = atof(parm);
		if (*parm == '+' || *parm == '-')
			s = t;
		while (*parm && !isspace(*parm))
			parm++;
		switch (parm[-1]) {
			case 'i':	r *= 72;		break;
			case 'c':	r *= (72.*50./127.);	break;
			case 'P':	r *= 12;		break;
			case 'u':	r /= 6;			break;
		}
	}
	*prev = t;
	setfval(name, r+s);
}

/* The following should be regarded as a kludge, since these values don't
get set by textsize = , etc.  There is confusion between picasso's ideas
of these variables and troff's. */
static	int	ft;		/* save previous font for \fP and .ft */
static	int	psize = 10,	/* ditto previous point size */
		vsize = 12;	/*    and vertical spacing */

troff(s)
	char	*s;
{
	int	i, fp;

	if (s[1] == 'f' && s[2] == 't') {
		for (s += 3; isspace(*s); ++s)
			;
		i = getfval("textfont");	/* save current font */
		if (s == '\0')			/* reset previous */
			fp = ft;
		else if (isdigit(*s))		/* allow multi-digit nums */
			fp = checkfont((double)atoi(s));
		else
			fp = setfont(tostring(s));
		ft = i;
		setfval("textfont", fp);
	}
	else if (s[1] == 'p' && s[2] == 's')
		troffparm("textsize", s+3, &psize);
	else if (s[1] == 'v' && s[2] == 's')
		troffparm("textspace", s+3, &vsize);
	else if (s[1] == 'e')
		if (s[2] == 'o')
			esc = '\0';
		else if (s[2] == 'c') {
			for (s += 3; isspace(*s); ++s)
				;
			esc = *s ? *s : '\\';
		}
}

obj	*troffgen(s)
	YYSTYPE	s;
{
	troff(s.p);
	save_one(CENTER, 0, 0, 0, s.p);
	free(s.p);
	return makenode(TROFF, 0, (int)getfval("curlayer"));
}

char	*parse_text (txt, fp, sp)	/* split txt into substrings with */
	char	*txt;			/* uniform font and pointsize;    */
	int	*fp, *sp;		/* some '\0' chars are put in txt.*/
{
	extern int  parsing;
static	char	buf[256];
static	int	n   = 0,
		lim = 0;
	int	i, j;
	char	*bp;

	if (n == 0) {	/* new string; initialize */
		lim = strlen(txt);
		ft = *fp;
	}
	else if (n == lim) {
		n = 0;
		return NULL;
	}
	for (bp = buf; n < lim; ) {
		if (txt[n] != esc && txt[n] != *eqn_delim)
			*bp++ = txt[n++];
		else if (txt[n] == *eqn_delim) {
			if (!parsing) {
				do n++;
				while (txt[n] != eqn_delim[1] && n < lim);
				n++;
				continue;
			}
			if (bp > buf) { *bp = '\0'; return buf; }
			eqn_save(".EQ");
			while (txt[++n] != eqn_delim[1] && n < lim)
				*bp++ = txt[n];
			if (n < lim)
				++n;
			strcpy(bp, "\n.EN");
			eqn_save(buf);
			eqn_gen(TEXT);
			bp = buf;
			*bp++ = '\0';	/* fudge, to prevent a 2nd save_one() */
		}
		else {				/* fonts, sizes, troff codes  */
			switch (txt[n+1]) {
			default:
				*bp++ = txt[n++];
				break;
			case '&':		/* zero-width char */
				n += 2;		/* discard it */
				break;
			case '(':
				if ((i = troffcode(txt+n+2)) == 0)
					*bp++ = txt[n++];	/* unknown */
				else {
					if (bp > buf && *fp != trcode[i].font)
						{ *bp = '\0'; return buf; }
					*bp++ = trcode[i].value;
					ft = *fp;
					*fp = trcode[i].font;
					txt[++n] = '\\';
					txt[n+1] = 'f';
					txt[n+2] = 'P';	/* restore old font */
				}
				break;
			case 'N':
				n += 3;
				*bp++ = atoi(txt+n);
				while (isdigit(txt[n])) n++;
				break;
			case 'f':
				if (bp > buf) { *bp = '\0'; return buf; }
				n += 2;
				i = *fp;		/* save current font */
				if (txt[n] == 'P') {	/* reset previous */
					*fp = ft;
					++n;
				}
				else if (isdigit(txt[n]))
					*fp = checkfont((double)(txt[n++]-'0'));
				else {
					if (txt[n] != '(')
						*bp++ = txt[n++];
					else {
						*bp++ = txt[++n];
						*bp++ = txt[++n];
						++n;
					}
					*bp = '\0';
					*fp = setfont(tostring(bp=buf));
				}
				ft = i;
				break;
			case 's':
				if (bp > buf) { *bp = '\0'; return buf; }
				n += 2;
				if (txt[n] == '+' || txt[n] == '-') {
					if ((i = *sp) == 0)
						i = getfval("textsize");
					if (txt[n] == '-')
						i = -i;
					++n;
				}
				else
					i = 0; 
				j = 0;
				if (i == 0 && txt[n] == '0'){/* use previous */
					i = psize;
					++n;
				}
				else {
					if (isdigit(txt[n]))
						j = txt[n++] - '0';
					if (j && isdigit(txt[n]))
						j = j * 10 + txt[n++] - '0';
				}
				psize = *sp = (i < 0 ? -(i+j) : i+j);
				break;
			}
		}
	}
	*bp = '\0';
	if (bp != buf)
		return buf;
	else {
		n = 0;
		return NULL;
	}
}

int	eqn_count	  = 0;
char	psfname[L_tmpnam] = "";
char	dpost[]	= "dpost";
FILE	*eqnfp  = NULL,
	*pipefp = NULL;
int	no_eqn = 0;

eqn_save(s)
	char	*s;
{
extern	char	*gwblib;
	char	*dp, private[64], buf[BUFSIZ];

	if (pipefp == NULL && !no_eqn){/* open up eqn|troff|dpost subprocess */
		if (access("/usr/bin/eqn", 1))	/* kludge! */
			no_eqn = 1;
		else {
			sprintf(private, "%s/%s", gwblib, dpost);
			dp = access(private,1) ? dpost : private;
			tmpnam(psfname);
			sprintf(buf, "eqn|troff -Tpost|%s -B>%s", dp, psfname);
			if ((pipefp = popen(buf, "w")) == NULL)
				fatal("cannot pipe through eqn");
		}
	}
	if (no_eqn) {
		if (strncmp(s, ".EQ", 3) == 0 || strncmp(s, ".EN", 3) == 0 ||
					strncmp(s, "delim", 5) == 0)
			;
		else
			savetext(CENTER, s);
		return;
	}
	if (s[0] == '.' && s[1] == 'E' && s[2] == 'Q') {
		double	f = getfval("textfont");
		sprintf(buf,".ft %s\n.ps %d\n.vs %d\n", mount[(int)f]->name,
			(int)getfval("textsize"), (int)getfval("textspace"));
		fputs(buf, pipefp);
	}
	fputs(s, pipefp);
	fputc('\n', pipefp);
	scan_delim(s);
}

scan_delim(s)
	char    *s;
{
	while (isspace(*s))
		++s;
	/* probably eqn's syntax is less restrictive than the following */
	if (strncmp(s,"delim",5)==0) {
		for (s += 5; isspace(*s); ++s)
			;
		if (strcmp(s,"off")==0)
			eqn_delim[1] = eqn_delim[0] = '\0';
		else {
			eqn_delim[1] = eqn_delim[0] = *s;
			if (*s++)
				if (*s)
					eqn_delim[1] = *s;
		}
	}
}

eqn_gen(type)
	int	type;
{
extern	int	ntextlines;
	obj	*p;
	char	buf[8];

	if (!no_eqn) {
		fputs(".bp\n", pipefp);
		sprintf(buf, "%d", ++eqn_count);
		save_one(EQNTXT, 0, 0, ntextlines, buf);
	}
	if (type == TROFF) {
		p = makenode(TROFF, 0, (int)getfval("curlayer"));
		checktextcolor(p);
		codegen = 1;
	}
}
