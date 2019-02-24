%Start A str def sc br thru sh eqn
%e 1900
%k 150
%a 2100
%o 1650
%p 5300
%n 750

%{
#undef	input
#undef	unput
#include <ctype.h>
#include <math.h>
#include "picasso.h"
#include "y.tab.h"

extern	double	atof();
extern	struct	symtab	symtab[];
extern	char	*filename, *copythru();

#define	CADD	cbuf[clen++]=yytext[0];\
		if(clen>=CBUFLEN-1) {yyerror("string too long",cbuf); BEGIN A;}
#define	CBUFLEN	500

	char	cbuf[CBUFLEN];
	int	c, clen, cflag, delim;
	int	ifsw	= 0;	/* 1 if if statement in progress */
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

<A>^".PS".*	{ if (curfile == infile) yyerror(".PS found inside .PS/.PE"); }
<A>^"."P[EFN].*	{ if (curfile == infile) {
			yylval.i = yytext[2];
			PEseen = 1;
			return(EOF);
		  }
		}
<A>^".EQ".*	{ BEGIN eqn; eqn_save(yytext); return(ST); }
<eqn>^".EN".*	{ eqn_save(yytext); eqn_gen(TROFF); BEGIN A; return(ST); }
<eqn>^.*	{ eqn_save(yytext); return(ST); }
<eqn>\n		;
<A>^".".*	{ yylval.p = tostring(yytext); return(TROFF); }

<A>print		return(yylval.i = PRINT);
<A>show			return(yylval.i = SHOW);
<A>box			return(yylval.i = BOX);
<A>circle		return(yylval.i = CIRCLE);
<A>arc			return(yylval.i = ARC);
<A>sector		return(yylval.i = SECTOR);
<A>ellipse		return(yylval.i = ELLIPSE);
<A>arrow		return(yylval.i = ARROW);
<A>spline		return(yylval.i = SPLINE);
<A>line			return(yylval.i = LINE);
<A>move			return(yylval.i = MOVE);
<A>psfile		return(yylval.i = PSFILE);
<A>(block|"[]")		return(yylval.i = BLOCK);
<A>text			return(yylval.i = TEXTOBJ);
<A>object		return(yylval.i = OBJECT);
<A>reset		return(RESET);
<A>sprintf		return(SPRINTF);

<A>rot(ate)?		return(yylval.i = ROTATE);
<A>trans(late)?		return(yylval.i = TRANSLATE);
<A>scale		return(yylval.i = SCALE);
<A>reflect		return(yylval.i = REFLECT);
<A>transform		return(yylval.i = TRANSFORM);

<A>same			return(SAME);
<A>between		return(BETWEEN);
<A>and			return(AND);

<A>of		;
<A>the		;
<A>way		;

<A>"."(e|east)			{ yylval.i = EAST; return(CORNER); }
<A>"."(r|right)			{ yylval.i = EAST; return(CORNER); }
<A>"."(w|west)			{ yylval.i = WEST; return(CORNER); }
<A>"."(l|left)			{ yylval.i = WEST; return(CORNER); }
<A>"."(n|north)			{ yylval.i = NORTH; return(CORNER); }
<A>"."(t|top)			{ yylval.i = NORTH; return(CORNER); }
<A>"."(s|south)			{ yylval.i = SOUTH; return(CORNER); }
<A>"."(b|bot|bottom)		{ yylval.i = SOUTH; return(CORNER); }
<A>"."(c|center)		{ yylval.i = CENTER; return(CORNER); }
<A>"."(ne|northeast)		{ yylval.i = NE; return(CORNER); }
<A>"."(se|southeast)		{ yylval.i = SE; return(CORNER); }
<A>"."(nw|northwest)		{ yylval.i = NW; return(CORNER); }
<A>"."(sw|southwest)		{ yylval.i = SW; return(CORNER); }
<A>".end"			{ yylval.i = END; return(CORNER); }
<A>".start"			{ yylval.i = START; return(CORNER); }

<A>".first"		{ yylval.i = 1; return(DOTNTH); }
<A>"."{D}+(th|nd|rd|st)	{ yylval.i = atoi(yytext+1); return(DOTNTH); }

<A>top" "+of		{ yylval.i = NORTH; return(CORNER); }
<A>north" "+of		{ yylval.i = NORTH; return(CORNER); }
<A>bottom" "+of		{ yylval.i = SOUTH; return(CORNER); }
<A>south" "+of		{ yylval.i = SOUTH; return(CORNER); }
<A>left" "+of		{ yylval.i = WEST; return(CORNER); }
<A>west" "+of		{ yylval.i = WEST; return(CORNER); }
<A>northwest" "+of	{ yylval.i = NW; return(CORNER); }
<A>southwest" "+of	{ yylval.i = SW; return(CORNER); }
<A>right" "+of		{ yylval.i = EAST; return(CORNER); }
<A>east" "+of		{ yylval.i = EAST; return(CORNER); }
<A>northeast" "+of	{ yylval.i = NE; return(CORNER); }
<A>southeast" "+of	{ yylval.i = SE; return(CORNER); }
<A>center" "+of		{ yylval.i = CENTER; return(CORNER); }
<A>start" "+of		{ yylval.i = START; return(CORNER); }
<A>end" "+of		{ yylval.i = END; return(CORNER); }

<A>fill(ed)?		{ yylval.i = PCOLOR; return(COLOR); }
<A>edge(d)?		{ yylval.i = LCOLOR; return(COLOR); }
<A>label(ed)?		{ yylval.i = TCOLOR; return(COLOR); }

<A>height|ht		{ yylval.i = HEIGHT; return(ATTR); }
<A>width|wid		{ yylval.i = WIDTH; return(ATTR); }
<A>radius|rad		{ yylval.i = RADIUS; return(ATTR); }
<A>diameter|diam	{ yylval.i = DIAMETER; return(ATTR); }
<A>left			{ yylval.i = LEFT; return(DIR); }
<A>right		{ yylval.i = RIGHT; return(DIR); }
<A>up			{ yylval.i = UP; return(DIR); }
<A>down			{ yylval.i = DOWN; return(DIR); }
<A>cw			{ yylval.i = CW; return(ATTR); }
<A>clockwise		{ yylval.i = CW; return(ATTR); }
<A>ccw			{ yylval.i = CCW; return(ATTR); }
<A>noedge		{ yylval.i = NOEDGE; return(ATTR); }
<A>invis(ible)?		{ yylval.i = NOEDGE; return(ATTR); }
<A>layer		{ yylval.i = LAYER; return(ATTR); }
<A>weight|wt		{ yylval.i = LWEIGHT; return(ATTR); }
<A>dot(ted)?		return(yylval.i = DOT);
<A>dashpat		return(yylval.i = DASHPAT);
<A>dash(ed)?		return(yylval.i = DASH);
<A>chop			return(yylval.i = CHOP);
<A>size			return(yylval.i = SIZE);
<A>space		return(yylval.i = SPACE);
<A>font			return(yylval.i = FONT);
<A>setrgb		return(yylval.i = RGB);
<A>setfont		return(yylval.i = SETFONT);

<A>ljust		{ yylval.i = LJUST; return TEXTATTR; }
<A>rjust		{ yylval.i = RJUST; return TEXTATTR; }
<A>above		{ yylval.i = ABOVE; return TEXTATTR; }
<A>below		{ yylval.i = BELOW; return TEXTATTR; }
<A>center		{ yylval.i = CENTER; return TEXTATTR; }

<A>"<-"			{ yylval.i = HEAD1; return(HEAD); }
<A>"->"			{ yylval.i = HEAD2; return(HEAD); }
<A>"<->"		{ yylval.i = HEAD12; return(HEAD); }

<A>".x"			return(yylval.i = DOTX);
<A>".y"			return(yylval.i = DOTY);
<A>"."(ht|height)	return(yylval.i = DOTHT);
<A>"."(wid|width)	return(yylval.i = DOTWID);
<A>"."(rad|radius)	return(yylval.i = DOTRAD);

<A>from		return(yylval.i = FROM);
<A>to		return(yylval.i = TO);
<A>touch(ing)?	return(yylval.i = LOCUS);
<A>at		return(yylval.i = AT);
<A>about	return(yylval.i = AT);
<A>by		return(yylval.i = BY);
<A>with		return(yylval.i = WITH);
<A>last		return(yylval.i = LAST);
<A>del(ete)?	return(yylval.i = DELETE);

<A>abs		return(ABS);
<A>ln		return(LOG);
<A>log		return(LOG10);
<A>exp		return(EXP);
<A>pow		return(POW);
<A>sin		return(SIN);
<A>cos		return(COS);
<A>atan2	return(ATAN2);
<A>sqrt		return(SQRT);
<A>rand		return(RAND);
<A>max		return(MAX);
<A>min		return(MIN);
<A>int		return(INT);

<A>"=="		return(EQ);
<A>">="		return(GE);
<A>"<="		return(LE);
<A>"!="		return(NEQ);
<A>">"		return(GT);
<A>"<"		return(LT);
<A>"&&"		return(ANDAND);
<A>"||"		return(OROR);
<A>"!"		return(NOT);	

<A>Here		return(yylval.i = HERE);

<A>for		{ return(FOR); }
<A>^_Endfor\n	{ endfor(); }
<A>do		{ yylval.p = delimstr("loop body"); return(DOSTR); }

<A>dup(licate)?		return(DUP);
<A>copy|include		return(COPY);
<A>(thru|through){WS}+	{ BEGIN thru; return(THRU); }
<thru>{A}{B}*|.		{ yylval.p = copythru(yytext); BEGIN A; return(DEFNAME); }
<A>until		return(UNTIL);

<A>if		{ ifsw = 1; return(IF); }
<A>then		{ if (!ifsw) { yylval.i = THEN; return(ATTR); }
		  yylval.p = delimstr("then part"); ifsw = 0;
		  return(THENSTR); }
<A>else		{ yylval.p = delimstr("else part"); return(ELSESTR); }

<A>sh{WS}+	{ BEGIN sh;
		  if ((delim = input()) == '{') delim = '}';
							/* no nested {} */
		  shell_init(); }
<sh>{A}{B}*	{ struct symtab *p;
		  if (yytext[0] == delim) {
			shell_exec();
			BEGIN A;
		  } else {
			p = lookup(yytext, 0);
			if (p != NULL && p->s_type == DEFNAME) {
				c = input();
				unput(c);
				if (c == '(')
					dodef(p);
				else
					pbstr(p->s_val.p);
			} else
				shell_text(yytext);
		  }
		}
<sh>.|\n	{ if (yytext[0] == delim) {
			shell_exec();
			BEGIN A;
		  } else
			shell_text(yytext);
		}

<A>define{WS}+		{ BEGIN def; }
<def>{A}{B}*		{ definition(yytext); BEGIN A; }
<A>undef(ine)?{WS}+{A}{B}*	{ undefine(yytext); }

<A>first		{ yylval.i = 1; return(NTH); }
<A>{D}+(th|nd|rd|st)	{ yylval.i = atoi(yytext); return(NTH); }
<A>({D}+("."?){D}*|"."{D}+)((e|E)("+"|-)?{D}+)?d {
			yylval.f = atof(yytext)*M_PI/180.; return(NUMBER); }
<A>({D}+("."?){D}*|"."{D}+)((e|E)("+"|-)?{D}+)?i? {
		  	yylval.f = atof(yytext); return(NUMBER); }

<A>{A}{B}* {	struct symtab *p;
		p = lookup(yytext);
		if (p != NULL && p->s_type == DEFNAME) {
			c = input();
			unput(c);
			if (c == '(')	/* it's name(...) */
				dodef(p);
			else 	/* no argument list */
				pbstr(p->s_val);
		} else {
			yylval.p = tostring(yytext);
			if (strcmp(yytext,"textsize")==0) return(TEXTVAR);
			else if (islower(yytext[0]))	  return(VARNAME);
			else				  return(PLACENAME);
		}
	}

<A>\"		{ BEGIN str; clen=0; }
<str>\"		{ cbuf[clen]=0;
		  yylval.p = tostring(cbuf);
		  BEGIN A;
		  return(TEXT);
		}
<str>\n		{ yyerror("newline in string"); BEGIN A; return(ST); }
<str>"\\\""	{ cbuf[clen++]='"'; }
<str>"\\"t	{ cbuf[clen++]='\t'; }
<str>"\\\\"	{ cbuf[clen++]='\\'; }
<str>.		{ CADD; }

<A>#.*		;

<A>.		return(yylval.i = yytext[0]);

%%
