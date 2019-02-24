%Start A str comment def sc br
%e 1300
%k 100
%a 1400
%o 1200
%p 3500
%n 600

%{
#undef	input
#undef	unput
#include <stdio.h>
#include <ctype.h>
#include "pic.h"
#include "y.tab.h"

extern	double	atof();
extern	int	dbg;
#define	dprintf	if(dbg)printf
extern	int	yylval;
extern	struct	symtab	symtab[];
extern	char	*filename;
extern	int	synerr;
extern	float	lastfloat;

#define	CADD	cbuf[clen++]=yytext[0]; if(clen>=CBUFLEN-1) {yyerror("string too long", cbuf); BEGIN A;}
#define	CBUFLEN	150
char	cbuf[CBUFLEN];
int	clen, cflag;
%}

A	[a-zA-Z_]
B	[a-zA-Z0-9_]
D	[0-9]
WS	[ \t]

%%
	switch (yybgin-yysvec-1) {	/* witchcraft */
	case 0:
		BEGIN A;
		break;
	case sc:
		BEGIN A;
		return('}');
	case br:
		BEGIN A;
		return(']');
	}

<A>{WS}		;
<A>"\\"\n	;
<A>\n		{ return(ST); }
<A>";"		{ return(ST); }
<A>"}"		{ BEGIN sc; return(ST); }
<A>"]"		{ BEGIN br; return(ST); }

<A>^".".*	{ if (yytext[1] == 'P' && (yytext[2] == 'E' || yytext[2] == 'F')) {
			yylval = yytext[2];
			return(EOF);
		  } else {
			yylval = tostring(yytext);
			return(TROFF);
		  }
		}

<A>print	return(yylval = PRINT);
<A>box		return(yylval = BOX);
<A>circle	return(yylval = CIRCLE);
<A>arc		return(yylval = ARC);
<A>ellipse	return(yylval = ELLIPSE);
<A>arrow	return(yylval = ARROW);
<A>spline	return(yylval = SPLINE);
<A>line		return(yylval = LINE);
<A>move		return(yylval = MOVE);
<A>"[]"		return(yylval = BLOCK);

<A>same		return(SAME);
<A>between	return(BETWEEN);
<A>and		return(AND);

<A>of		;
<A>the		;
<A>way		;

<A>".e"		{ yylval = EAST; return(CORNER); }
<A>".east"	{ yylval = EAST; return(CORNER); }
<A>".r"		{ yylval = EAST; return(CORNER); }
<A>".right"	{ yylval = EAST; return(CORNER); }
<A>".w"		{ yylval = WEST; return(CORNER); }
<A>".west"	{ yylval = WEST; return(CORNER); }
<A>".l"		{ yylval = WEST; return(CORNER); }
<A>".left"	{ yylval = WEST; return(CORNER); }
<A>".n"		{ yylval = NORTH; return(CORNER); }
<A>".north"	{ yylval = NORTH; return(CORNER); }
<A>".t"		{ yylval = NORTH; return(CORNER); }
<A>".top"	{ yylval = NORTH; return(CORNER); }
<A>".s"		{ yylval = SOUTH; return(CORNER); }
<A>".south"	{ yylval = SOUTH; return(CORNER); }
<A>".b"		{ yylval = SOUTH; return(CORNER); }
<A>".bot"	{ yylval = SOUTH; return(CORNER); }
<A>".bottom"	{ yylval = SOUTH; return(CORNER); }
<A>".c"		{ yylval = CENTER; return(CORNER); }
<A>".center"	{ yylval = CENTER; return(CORNER); }
<A>".start"	{ yylval = START; return(CORNER); }
<A>".end"	{ yylval = END; return(CORNER); }
<A>".ne"		{ yylval = NE; return(CORNER); }
<A>"."upper" "*right	{ yylval = NE; return(CORNER); }
<A>".se"		{ yylval = SE; return(CORNER); }
<A>"."lower" "*right	{ yylval = SE; return(CORNER); }
<A>".nw"		{ yylval = NW; return(CORNER); }
<A>"."upper" "*left	{ yylval = NW; return(CORNER); }
<A>".sw"		{ yylval = SW; return(CORNER); }
<A>"."lower" "*left	{ yylval = SW; return(CORNER); }

<A>top" "+of		{ yylval = NORTH; return(CORNER); }
<A>north" "+of		{ yylval = NORTH; return(CORNER); }
<A>bottom" "+of		{ yylval = SOUTH; return(CORNER); }
<A>south" "+of		{ yylval = SOUTH; return(CORNER); }
<A>left" "+of		{ yylval = WEST; return(CORNER); }
<A>west" "+of		{ yylval = WEST; return(CORNER); }
<A>right" "+of		{ yylval = EAST; return(CORNER); }
<A>east" "+of		{ yylval = EAST; return(CORNER); }
<A>center" "+of		{ yylval = CENTER; return(CORNER); }
<A>start" "+of		{ yylval = START; return(CORNER); }
<A>end" "+of		{ yylval = END; return(CORNER); }
<A>upper" "+right" "+of	{ yylval = NE; return(CORNER); }
<A>upper" "+left" "+of	{ yylval = NW; return(CORNER); }
<A>lower" "+right" "+of	{ yylval = SE; return(CORNER); }
<A>lower" "+left" "+of	{ yylval = SW; return(CORNER); }

<A>height	{ yylval = HEIGHT; return(ATTR); }
<A>ht		{ yylval = HEIGHT; return(ATTR); }
<A>wid		{ yylval = WIDTH; return(ATTR); }
<A>width	{ yylval = WIDTH; return(ATTR); }
<A>rad		{ yylval = RADIUS; return(ATTR); }
<A>radius	{ yylval = RADIUS; return(ATTR); }
<A>diam		{ yylval = DIAMETER; return(ATTR); }
<A>diameter	{ yylval = DIAMETER; return(ATTR); }
<A>size		{ yylval = SIZE; return(ATTR); }
<A>left		{ yylval = LEFT; return(DIR); }
<A>right	{ yylval = RIGHT; return(DIR); }
<A>up		{ yylval = UP; return(DIR); }
<A>down		{ yylval = DOWN; return(DIR); }
<A>cw		{ yylval = CW; return(ATTR); }
<A>ccw		{ yylval = CCW; return(ATTR); }
<A>then		{ yylval = THEN; return(ATTR); }
<A>invis	{ yylval = INVIS; return(ATTR); }
<A>invisible	{ yylval = INVIS; return(ATTR); }
<A>dot		return(yylval = DOT);
<A>dotted	return(yylval = DOT);
<A>dash		return(yylval = DASH);
<A>dashed	return(yylval = DASH);
<A>chop		return(yylval = CHOP);

<A>spread	return(yylval = SPREAD);
<A>fill		return(yylval = FILL);
<A>ljust	return(yylval = LJUST);
<A>rjust	return(yylval = RJUST);
<A>above	return(yylval = ABOVE);
<A>below	return(yylval = BELOW);

<A>"<-"		{ yylval = HEAD1; return(HEAD); }
<A>"->"		{ yylval = HEAD2; return(HEAD); }
<A>"<->"	{ yylval = HEAD12; return(HEAD); }

<A>".x"		return(yylval = DOTX);
<A>".y"		return(yylval = DOTY);
<A>".ht"	return(yylval = DOTHT);
<A>".height"	return(yylval = DOTHT);
<A>".wid"	return(yylval = DOTWID);
<A>".width"	return(yylval = DOTWID);
<A>".rad"	return(yylval = DOTRAD);
<A>".radius"	return(yylval = DOTRAD);

<A>from		return(yylval = FROM);
<A>to		return(yylval = TO);
<A>at		return(yylval = AT);
<A>by		return(yylval = BY);
<A>with		return(yylval = WITH);
<A>last		return(yylval = LAST);

<A>Here		return(yylval = HERE);
<A>define{WS}+	{ BEGIN def; }
<def>{A}{B}*	{ yylval = definition(yytext); BEGIN A; return(TROFF); }

<A>first		{ yylval = 1; return(NTH); }
<A>{D}+(th|nd|rd|st)	{ yylval = atoi(yytext); return(NTH); }
<A>({D}+("."?){D}*|"."{D}+)i?	{
		int i, inch;
		inch = 0;
		for (i = 0; i < yyleng; i++)
			if (yytext[i] == '.' || yytext[i] == 'i') {
				inch++;
				break;
			}
		if (inch)
			yylval = atof(yytext) * getvar("scale") + 0.5;
		else
			yylval = atoi(yytext);
		lastfloat = atof(yytext);
		return(NUMBER); }

<A>{A}{B}* {
		int c;
		char buf[100];
		struct symtab *p;
		p = lookup(yytext);
		if (p != NULL && p->s_type == DEFNAME) {
			yylval = defuse(yytext, p);
			return(TROFF);
		} else if (islower(yytext[0])) {
			yylval = (int) tostring(yytext);
			return(VARNAME);
		} else {
			yylval = (int) tostring(yytext);
			return(PLACENAME);
		}
	}

<A>\"		{ BEGIN str; clen=0; }

<A>#		{ BEGIN comment; }
<comment>\n	{ BEGIN A; return(ST); }
<comment>.	;

<A>.		{ yylval = yytext[0]; return(yytext[0]); }

<str>\"		{ BEGIN A; cbuf[clen]=0; yylval = tostring(cbuf); return(TEXT); }
<str>\n		{ yyerror("newline in string"); BEGIN A; return(ST); }
<str>"\\\""	{ cbuf[clen++]='"'; }
<str>"\\"t	{ cbuf[clen++]='\t'; }
<str>"\\\\"	{ cbuf[clen++]='\\'; }
<str>.		{ CADD; }

%%
