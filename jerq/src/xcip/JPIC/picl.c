# include "stdio.h"
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX 200
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
# define A 2
# define str 4
# define comment 6
# define def 8
# define sc 10
# define br 12
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
# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
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
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
	;
break;
case 2:
;
break;
case 3:
	{ return(ST); }
break;
case 4:
	{ return(ST); }
break;
case 5:
	{ BEGIN sc; return(ST); }
break;
case 6:
	{ BEGIN br; return(ST); }
break;
case 7:
{ if (yytext[1] == 'P' && (yytext[2] == 'E' || yytext[2] == 'F')) {
			yylval = yytext[2];
			return(EOF);
		  } else {
			yylval = tostring(yytext);
			return(TROFF);
		  }
		}
break;
case 8:
return(yylval = PRINT);
break;
case 9:
	return(yylval = BOX);
break;
case 10:
return(yylval = CIRCLE);
break;
case 11:
	return(yylval = ARC);
break;
case 12:
return(yylval = ELLIPSE);
break;
case 13:
return(yylval = ARROW);
break;
case 14:
return(yylval = SPLINE);
break;
case 15:
	return(yylval = LINE);
break;
case 16:
	return(yylval = MOVE);
break;
case 17:
	return(yylval = BLOCK);
break;
case 18:
	return(SAME);
break;
case 19:
return(BETWEEN);
break;
case 20:
	return(AND);
break;
case 21:
	;
break;
case 22:
	;
break;
case 23:
	;
break;
case 24:
	{ yylval = EAST; return(CORNER); }
break;
case 25:
{ yylval = EAST; return(CORNER); }
break;
case 26:
	{ yylval = EAST; return(CORNER); }
break;
case 27:
{ yylval = EAST; return(CORNER); }
break;
case 28:
	{ yylval = WEST; return(CORNER); }
break;
case 29:
{ yylval = WEST; return(CORNER); }
break;
case 30:
	{ yylval = WEST; return(CORNER); }
break;
case 31:
{ yylval = WEST; return(CORNER); }
break;
case 32:
	{ yylval = NORTH; return(CORNER); }
break;
case 33:
{ yylval = NORTH; return(CORNER); }
break;
case 34:
	{ yylval = NORTH; return(CORNER); }
break;
case 35:
{ yylval = NORTH; return(CORNER); }
break;
case 36:
	{ yylval = SOUTH; return(CORNER); }
break;
case 37:
{ yylval = SOUTH; return(CORNER); }
break;
case 38:
	{ yylval = SOUTH; return(CORNER); }
break;
case 39:
{ yylval = SOUTH; return(CORNER); }
break;
case 40:
{ yylval = SOUTH; return(CORNER); }
break;
case 41:
	{ yylval = CENTER; return(CORNER); }
break;
case 42:
{ yylval = CENTER; return(CORNER); }
break;
case 43:
{ yylval = START; return(CORNER); }
break;
case 44:
{ yylval = END; return(CORNER); }
break;
case 45:
	{ yylval = NE; return(CORNER); }
break;
case 46:
{ yylval = NE; return(CORNER); }
break;
case 47:
	{ yylval = SE; return(CORNER); }
break;
case 48:
{ yylval = SE; return(CORNER); }
break;
case 49:
	{ yylval = NW; return(CORNER); }
break;
case 50:
{ yylval = NW; return(CORNER); }
break;
case 51:
	{ yylval = SW; return(CORNER); }
break;
case 52:
{ yylval = SW; return(CORNER); }
break;
case 53:
	{ yylval = NORTH; return(CORNER); }
break;
case 54:
	{ yylval = NORTH; return(CORNER); }
break;
case 55:
	{ yylval = SOUTH; return(CORNER); }
break;
case 56:
	{ yylval = SOUTH; return(CORNER); }
break;
case 57:
	{ yylval = WEST; return(CORNER); }
break;
case 58:
	{ yylval = WEST; return(CORNER); }
break;
case 59:
	{ yylval = EAST; return(CORNER); }
break;
case 60:
	{ yylval = EAST; return(CORNER); }
break;
case 61:
	{ yylval = CENTER; return(CORNER); }
break;
case 62:
	{ yylval = START; return(CORNER); }
break;
case 63:
	{ yylval = END; return(CORNER); }
break;
case 64:
{ yylval = NE; return(CORNER); }
break;
case 65:
{ yylval = NW; return(CORNER); }
break;
case 66:
{ yylval = SE; return(CORNER); }
break;
case 67:
{ yylval = SW; return(CORNER); }
break;
case 68:
{ yylval = HEIGHT; return(ATTR); }
break;
case 69:
	{ yylval = HEIGHT; return(ATTR); }
break;
case 70:
	{ yylval = WIDTH; return(ATTR); }
break;
case 71:
{ yylval = WIDTH; return(ATTR); }
break;
case 72:
	{ yylval = RADIUS; return(ATTR); }
break;
case 73:
{ yylval = RADIUS; return(ATTR); }
break;
case 74:
	{ yylval = DIAMETER; return(ATTR); }
break;
case 75:
{ yylval = DIAMETER; return(ATTR); }
break;
case 76:
	{ yylval = SIZE; return(ATTR); }
break;
case 77:
	{ yylval = LEFT; return(DIR); }
break;
case 78:
{ yylval = RIGHT; return(DIR); }
break;
case 79:
	{ yylval = UP; return(DIR); }
break;
case 80:
	{ yylval = DOWN; return(DIR); }
break;
case 81:
	{ yylval = CW; return(ATTR); }
break;
case 82:
	{ yylval = CCW; return(ATTR); }
break;
case 83:
	{ yylval = THEN; return(ATTR); }
break;
case 84:
{ yylval = INVIS; return(ATTR); }
break;
case 85:
{ yylval = INVIS; return(ATTR); }
break;
case 86:
	return(yylval = DOT);
break;
case 87:
return(yylval = DOT);
break;
case 88:
	return(yylval = DASH);
break;
case 89:
return(yylval = DASH);
break;
case 90:
	return(yylval = CHOP);
break;
case 91:
return(yylval = SPREAD);
break;
case 92:
	return(yylval = FILL);
break;
case 93:
return(yylval = LJUST);
break;
case 94:
return(yylval = RJUST);
break;
case 95:
return(yylval = ABOVE);
break;
case 96:
return(yylval = BELOW);
break;
case 97:
	{ yylval = HEAD1; return(HEAD); }
break;
case 98:
	{ yylval = HEAD2; return(HEAD); }
break;
case 99:
{ yylval = HEAD12; return(HEAD); }
break;
case 100:
	return(yylval = DOTX);
break;
case 101:
	return(yylval = DOTY);
break;
case 102:
return(yylval = DOTHT);
break;
case 103:
return(yylval = DOTHT);
break;
case 104:
return(yylval = DOTWID);
break;
case 105:
return(yylval = DOTWID);
break;
case 106:
return(yylval = DOTRAD);
break;
case 107:
return(yylval = DOTRAD);
break;
case 108:
	return(yylval = FROM);
break;
case 109:
	return(yylval = TO);
break;
case 110:
	return(yylval = AT);
break;
case 111:
	return(yylval = BY);
break;
case 112:
	return(yylval = WITH);
break;
case 113:
	return(yylval = LAST);
break;
case 114:
	return(yylval = HERE);
break;
case 115:
{ BEGIN def; }
break;
case 116:
{ yylval = definition(yytext); BEGIN A; return(TROFF); }
break;
case 117:
	{ yylval = 1; return(NTH); }
break;
case 118:
{ yylval = atoi(yytext); return(NTH); }
break;
case 119:
{
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
break;
case 120:
{
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
break;
case 121:
	{ BEGIN str; clen=0; }
break;
case 122:
	{ BEGIN comment; }
break;
case 123:
{ BEGIN A; return(ST); }
break;
case 124:
;
break;
case 125:
	{ yylval = yytext[0]; return(yytext[0]); }
break;
case 126:
	{ BEGIN A; cbuf[clen]=0; yylval = tostring(cbuf); return(TEXT); }
break;
case 127:
	{ yyerror("newline in string"); BEGIN A; return(ST); }
break;
case 128:
{ cbuf[clen++]='"'; }
break;
case 129:
{ cbuf[clen++]='\t'; }
break;
case 130:
{ cbuf[clen++]='\\'; }
break;
case 131:
	{ CADD; }
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */
int yyvstop[] = {
0,

125,
0,

1,
125,
0,

3,
0,

121,
125,
0,

122,
125,
0,

125,
0,

125,
0,

119,
125,
0,

4,
125,
0,

125,
0,

120,
125,
0,

120,
125,
0,

125,
0,

125,
0,

6,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

120,
125,
0,

5,
125,
0,

7,
125,
0,

131,
0,

127,
0,

126,
131,
0,

131,
0,

124,
0,

123,
0,

116,
0,

98,
0,

119,
0,

38,
0,

41,
0,

24,
0,

30,
0,

32,
0,

26,
0,

36,
0,

34,
0,

28,
0,

100,
0,

101,
0,

119,
0,

119,
0,

119,
0,

97,
0,

120,
0,

120,
0,

17,
0,

2,
0,

120,
0,

120,
0,

120,
0,

110,
120,
0,

120,
0,

120,
0,

111,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

81,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

69,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

21,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

109,
120,
0,

79,
120,
0,

120,
0,

120,
0,

120,
0,

7,
0,

7,
119,
0,

7,
38,
0,

7,
41,
0,

7,
24,
0,

7,
0,

7,
30,
0,

7,
32,
0,

7,
26,
0,

7,
36,
0,

7,
34,
0,

7,
0,

7,
28,
0,

7,
100,
0,

7,
101,
0,

128,
0,

130,
0,

129,
0,

102,
0,

45,
0,

49,
0,

47,
0,

51,
0,

118,
0,

99,
0,

120,
0,

120,
0,

20,
120,
0,

11,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

9,
120,
0,

82,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

86,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

72,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

22,
120,
0,

120,
0,

120,
0,

23,
120,
0,

120,
0,

70,
120,
0,

120,
0,

7,
119,
0,

7,
0,

7,
0,

7,
0,

7,
0,

7,
0,

7,
102,
0,

7,
0,

7,
0,

7,
45,
0,

7,
0,

7,
49,
0,

7,
0,

7,
0,

7,
47,
0,

7,
0,

7,
0,

7,
51,
0,

7,
0,

7,
0,

7,
0,

7,
0,

39,
0,

44,
0,

106,
0,

35,
0,

104,
0,

114,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

90,
120,
0,

120,
0,

88,
120,
0,

120,
0,

74,
120,
0,

120,
0,

80,
120,
0,

120,
0,

120,
0,

92,
120,
0,

120,
0,

108,
120,
0,

120,
0,

120,
0,

113,
120,
0,

77,
120,
0,

15,
120,
0,

120,
0,

120,
0,

16,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

18,
120,
0,

76,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

83,
120,
0,

120,
0,

120,
0,

120,
0,

112,
120,
0,

7,
39,
0,

7,
0,

7,
0,

7,
44,
0,

7,
0,

7,
0,

7,
0,

7,
0,

7,
106,
0,

7,
0,

7,
0,

7,
0,

7,
35,
0,

7,
0,

7,
0,

7,
104,
0,

25,
0,

31,
0,

29,
0,

95,
120,
0,

13,
120,
0,

96,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

117,
120,
0,

120,
0,

84,
120,
0,

93,
120,
0,

120,
0,

120,
0,

8,
120,
0,

120,
0,

78,
120,
0,

94,
120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

120,
0,

71,
120,
0,

7,
0,

7,
0,

7,
25,
0,

7,
0,

7,
31,
0,

7,
0,

7,
0,

7,
0,

7,
0,

7,
0,

7,
0,

7,
0,

7,
29,
0,

7,
0,

33,
0,

27,
0,

37,
0,

43,
0,

105,
0,

120,
0,

120,
0,

120,
0,

10,
120,
0,

89,
120,
0,

120,
0,

120,
0,

87,
120,
0,

120,
0,

63,
0,

68,
120,
0,

120,
0,

73,
120,
0,

14,
120,
0,

91,
120,
0,

53,
0,

7,
0,

7,
0,

7,
0,

7,
0,

7,
33,
0,

7,
0,

7,
27,
0,

7,
37,
0,

7,
43,
0,

7,
0,

7,
105,
0,

40,
0,

42,
0,

103,
0,

107,
0,

19,
120,
0,

115,
0,

120,
0,

60,
0,

12,
120,
0,

120,
0,

57,
0,

58,
0,

7,
40,
0,

7,
42,
0,

7,
103,
0,

7,
0,

7,
0,

7,
107,
0,

7,
0,

7,
0,

75,
120,
0,

120,
0,

54,
0,

59,
0,

56,
0,

62,
0,

7,
0,

7,
0,

7,
0,

7,
0,

55,
0,

61,
0,

85,
120,
0,

7,
0,

7,
0,

7,
0,

7,
0,

52,
0,

50,
0,

7,
52,
0,

7,
0,

7,
50,
0,

7,
0,

48,
0,

46,
0,

7,
48,
0,

7,
46,
0,

67,
0,

65,
0,

66,
0,

64,
0,
0};
# define YYTYPE int
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	3,15,	0,0,	
7,54,	0,0,	0,0,	0,0,	
0,0,	0,0,	3,16,	3,17,	
7,54,	7,55,	28,83,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	3,18,	
3,19,	0,0,	0,0,	6,52,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	3,20,	3,21,	
24,79,	3,22,	0,0,	7,54,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
3,23,	3,24,	0,0,	0,0,	
0,0,	20,57,	3,25,	0,0,	
7,54,	0,0,	4,18,	4,19,	
0,0,	3,26,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	4,20,	4,49,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
3,27,	3,28,	3,29,	4,23,	
4,24,	6,53,	3,30,	3,31,	
3,32,	3,33,	3,34,	3,35,	
26,81,	3,36,	3,37,	27,82,	
4,26,	3,38,	3,39,	3,40,	
3,41,	3,42,	0,0,	3,43,	
3,44,	3,45,	3,46,	0,0,	
3,47,	35,103,	0,0,	5,50,	
0,0,	0,0,	3,48,	4,27,	
4,28,	4,29,	35,104,	5,50,	
5,51,	4,30,	4,31,	4,32,	
4,33,	4,34,	4,35,	31,88,	
4,36,	4,37,	30,84,	37,107,	
4,38,	4,39,	4,40,	4,41,	
4,42,	31,89,	4,43,	4,44,	
4,45,	4,46,	30,85,	4,47,	
5,52,	36,105,	30,86,	31,90,	
30,87,	4,48,	32,91,	34,100,	
32,92,	33,96,	39,113,	32,93,	
32,94,	33,97,	5,50,	40,114,	
36,106,	33,98,	34,101,	41,115,	
34,102,	42,116,	45,125,	33,99,	
46,127,	59,149,	32,95,	60,150,	
67,166,	45,126,	68,167,	5,50,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	5,53,	69,168,	
75,170,	77,170,	9,56,	69,169,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	9,56,	9,56,	
9,56,	9,56,	21,58,	21,58,	
21,58,	21,58,	21,58,	21,58,	
21,58,	21,58,	21,58,	21,58,	
78,170,	22,72,	79,171,	22,73,	
22,73,	22,73,	22,73,	22,73,	
22,73,	22,73,	22,73,	22,73,	
22,73,	38,108,	81,172,	43,117,	
61,151,	38,109,	44,120,	84,173,	
62,153,	38,110,	38,111,	43,118,	
43,119,	85,174,	44,121,	38,112,	
65,160,	61,152,	47,128,	91,181,	
44,122,	44,123,	47,129,	62,154,	
65,161,	44,124,	47,130,	63,155,	
21,59,	21,60,	92,182,	21,61,	
89,179,	88,177,	21,62,	93,183,	
89,180,	63,156,	21,63,	86,175,	
21,64,	88,178,	94,184,	96,185,	
21,65,	21,66,	21,67,	21,68,	
22,74,	21,69,	21,70,	21,71,	
97,186,	22,75,	86,176,	98,187,	
100,190,	22,76,	22,77,	22,78,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	99,188,	101,191,	
102,192,	99,189,	104,195,	105,196,	
107,197,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	53,146,	
108,198,	109,199,	110,200,	25,80,	
111,201,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	25,80,	
25,80,	25,80,	25,80,	49,131,	
103,193,	112,202,	113,203,	114,204,	
116,205,	117,206,	103,194,	49,131,	
49,0,	56,56,	56,56,	56,56,	
56,56,	56,56,	56,56,	56,56,	
56,56,	56,56,	56,56,	118,207,	
119,208,	120,209,	121,210,	122,211,	
124,214,	53,147,	58,58,	58,58,	
58,58,	58,58,	58,58,	58,58,	
58,58,	58,58,	58,58,	58,58,	
123,212,	64,157,	66,162,	125,215,	
126,216,	127,217,	123,213,	128,218,	
129,219,	130,220,	49,132,	64,158,	
66,163,	53,148,	132,0,	133,0,	
134,0,	66,164,	135,0,	64,159,	
66,165,	136,0,	137,0,	138,0,	
139,0,	130,221,	142,0,	49,131,	
72,72,	72,72,	72,72,	72,72,	
72,72,	72,72,	72,72,	72,72,	
72,72,	72,72,	141,0,	143,0,	
131,131,	144,0,	140,131,	145,0,	
149,244,	150,245,	151,246,	58,74,	
131,131,	131,0,	140,131,	140,0,	
132,132,	152,247,	153,248,	155,249,	
156,250,	158,251,	160,252,	161,253,	
49,133,	49,134,	163,254,	49,135,	
164,255,	166,256,	49,136,	167,257,	
168,258,	169,259,	49,137,	172,260,	
49,138,	173,261,	176,262,	177,263,	
49,139,	49,140,	49,141,	49,142,	
178,264,	49,143,	49,144,	49,145,	
179,265,	182,266,	183,267,	131,131,	
184,268,	140,131,	185,269,	186,270,	
187,271,	188,272,	189,273,	190,274,	
191,275,	192,276,	193,277,	194,278,	
195,279,	196,280,	197,281,	198,282,	
131,131,	135,225,	140,131,	134,224,	
199,283,	132,222,	200,284,	139,234,	
136,227,	137,229,	138,231,	201,285,	
133,223,	202,286,	135,226,	139,235,	
203,287,	204,288,	205,289,	137,230,	
138,232,	206,290,	207,291,	136,228,	
142,241,	208,292,	143,242,	209,293,	
138,233,	210,294,	143,243,	211,295,	
212,296,	213,297,	214,298,	141,240,	
215,299,	216,300,	140,236,	217,301,	
219,302,	220,303,	221,304,	222,0,	
223,0,	224,0,	225,0,	226,0,	
140,237,	227,0,	228,0,	229,0,	
230,0,	140,238,	231,0,	232,0,	
140,239,	233,0,	234,0,	235,0,	
236,0,	237,0,	238,0,	239,0,	
240,0,	241,0,	242,0,	243,0,	
244,321,	245,322,	246,323,	248,324,	
249,325,	250,326,	251,327,	252,328,	
253,329,	254,330,	255,331,	257,332,	
258,333,	259,334,	261,335,	262,336,	
263,337,	264,338,	265,339,	266,340,	
268,341,	269,342,	270,343,	271,344,	
272,345,	274,346,	275,347,	276,276,	
278,349,	280,350,	281,351,	283,352,	
285,353,	286,354,	288,355,	289,356,	
290,357,	291,358,	292,359,	295,360,	
296,361,	297,362,	298,363,	300,300,	
301,365,	302,366,	303,367,	305,0,	
306,0,	307,0,	308,0,	309,0,	
310,0,	312,0,	311,0,	313,0,	
315,0,	314,0,	317,0,	316,0,	
319,0,	318,0,	320,0,	321,382,	
322,383,	324,384,	326,385,	327,386,	
328,387,	226,308,	329,388,	330,389,	
331,390,	332,391,	334,392,	229,310,	
227,309,	224,306,	338,393,	339,394,	
234,313,	238,316,	223,305,	225,307,	
235,314,	340,395,	341,396,	342,397,	
343,398,	243,320,	344,399,	232,312,	
345,400,	230,311,	346,346,	347,402,	
348,403,	350,404,	240,317,	241,318,	
237,315,	351,405,	352,352,	242,319,	
354,407,	355,408,	276,348,	357,409,	
358,410,	360,411,	361,412,	362,413,	
363,414,	364,415,	365,416,	366,366,	
368,0,	369,0,	370,0,	371,0,	
372,0,	373,0,	300,364,	375,0,	
374,0,	376,0,	377,0,	378,0,	
379,0,	380,0,	381,0,	382,429,	
383,430,	384,431,	385,385,	387,434,	
393,437,	394,438,	395,439,	391,391,	
399,441,	398,440,	401,442,	402,443,	
309,371,	311,373,	405,444,	406,445,	
408,408,	410,410,	313,375,	314,376,	
318,379,	305,368,	306,369,	307,370,	
411,411,	417,454,	310,372,	312,374,	
407,407,	414,414,	315,377,	316,378,	
398,440,	416,416,	319,380,	418,0,	
320,381,	419,0,	420,0,	421,0,	
422,0,	346,401,	423,0,	424,0,	
425,0,	426,0,	428,0,	427,0,	
432,463,	352,406,	433,464,	435,465,	
436,466,	438,438,	439,439,	441,469,	
440,440,	444,470,	446,471,	447,472,	
448,473,	421,421,	366,417,	449,474,	
450,475,	451,476,	452,477,	453,478,	
455,0,	427,427,	456,0,	457,0,	
458,0,	459,0,	460,0,	461,0,	
369,419,	462,0,	385,432,	440,440,	
463,483,	371,420,	464,484,	391,435,	
385,433,	368,418,	374,422,	465,485,	
377,425,	391,436,	466,486,	467,487,	
381,428,	373,421,	468,488,	408,448,	
410,449,	470,489,	375,423,	376,424,	
379,427,	378,426,	471,490,	411,450,	
407,446,	472,491,	477,492,	478,493,	
414,451,	416,452,	407,447,	479,0,	
480,0,	483,498,	481,0,	416,453,	
482,0,	484,499,	485,500,	486,501,	
490,502,	491,503,	492,504,	493,505,	
494,0,	495,0,	496,0,	497,0,	
499,510,	501,511,	502,512,	503,513,	
438,467,	439,468,	418,455,	504,514,	
505,515,	421,458,	506,0,	507,0,	
508,0,	419,456,	509,0,	421,459,	
420,457,	427,461,	512,512,	423,460,	
513,519,	514,514,	515,521,	427,462,	
516,0,	517,0,	518,522,	519,519,	
520,524,	521,521,	523,526,	525,527,	
0,0,	0,0,	0,0,	458,479,	
0,0,	0,0,	461,481,	0,0,	
459,480,	0,0,	0,0,	0,0,	
462,482,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	479,494,	
0,0,	480,495,	481,496,	0,0,	
0,0,	482,497,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	495,507,	
0,0,	497,509,	0,0,	0,0,	
0,0,	512,518,	0,0,	0,0,	
514,520,	0,0,	494,506,	0,0,	
496,508,	0,0,	519,523,	0,0,	
521,525,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	507,516,	0,0,	0,0,	
509,517,	0,0,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+0,	0,		0,	
yycrank+0,	0,		0,	
yycrank+-1,	0,		0,	
yycrank+-36,	yysvec+3,	0,	
yycrank+-122,	0,		0,	
yycrank+-5,	yysvec+5,	0,	
yycrank+-3,	0,		0,	
yycrank+0,	yysvec+7,	0,	
yycrank+123,	0,		0,	
yycrank+0,	yysvec+9,	0,	
yycrank+0,	0,		0,	
yycrank+0,	0,		0,	
yycrank+0,	0,		0,	
yycrank+0,	0,		0,	
yycrank+0,	0,		yyvstop+1,
yycrank+0,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+0,	0,		yyvstop+8,
yycrank+0,	0,		yyvstop+11,
yycrank+3,	0,		yyvstop+14,
yycrank+198,	0,		yyvstop+16,
yycrank+211,	0,		yyvstop+18,
yycrank+0,	0,		yyvstop+21,
yycrank+3,	0,		yyvstop+24,
yycrank+280,	0,		yyvstop+26,
yycrank+3,	yysvec+25,	yyvstop+29,
yycrank+14,	0,		yyvstop+32,
yycrank+4,	0,		yyvstop+34,
yycrank+0,	0,		yyvstop+36,
yycrank+44,	yysvec+25,	yyvstop+39,
yycrank+38,	yysvec+25,	yyvstop+42,
yycrank+63,	yysvec+25,	yyvstop+45,
yycrank+68,	yysvec+25,	yyvstop+48,
yycrank+66,	yysvec+25,	yyvstop+51,
yycrank+16,	yysvec+25,	yyvstop+54,
yycrank+56,	yysvec+25,	yyvstop+57,
yycrank+33,	yysvec+25,	yyvstop+60,
yycrank+172,	yysvec+25,	yyvstop+63,
yycrank+55,	yysvec+25,	yyvstop+66,
yycrank+60,	yysvec+25,	yyvstop+69,
yycrank+73,	yysvec+25,	yyvstop+72,
yycrank+63,	yysvec+25,	yyvstop+75,
yycrank+174,	yysvec+25,	yyvstop+78,
yycrank+177,	yysvec+25,	yyvstop+81,
yycrank+74,	yysvec+25,	yyvstop+84,
yycrank+68,	yysvec+25,	yyvstop+87,
yycrank+189,	yysvec+25,	yyvstop+90,
yycrank+0,	0,		yyvstop+93,
yycrank+-402,	0,		yyvstop+96,
yycrank+0,	0,		yyvstop+99,
yycrank+0,	0,		yyvstop+101,
yycrank+0,	0,		yyvstop+103,
yycrank+337,	0,		yyvstop+106,
yycrank+0,	0,		yyvstop+108,
yycrank+0,	0,		yyvstop+110,
yycrank+365,	yysvec+9,	yyvstop+112,
yycrank+0,	0,		yyvstop+114,
yycrank+382,	0,		yyvstop+116,
yycrank+70,	0,		yyvstop+118,
yycrank+82,	0,		yyvstop+120,
yycrank+175,	0,		yyvstop+122,
yycrank+175,	0,		0,	
yycrank+194,	0,		yyvstop+124,
yycrank+340,	0,		yyvstop+126,
yycrank+187,	0,		yyvstop+128,
yycrank+341,	0,		yyvstop+130,
yycrank+73,	0,		yyvstop+132,
yycrank+74,	0,		0,	
yycrank+114,	0,		yyvstop+134,
yycrank+0,	0,		yyvstop+136,
yycrank+0,	0,		yyvstop+138,
yycrank+420,	yysvec+58,	yyvstop+140,
yycrank+0,	yysvec+22,	yyvstop+142,
yycrank+0,	0,		yyvstop+144,
yycrank+116,	0,		0,	
yycrank+0,	yysvec+75,	0,	
yycrank+101,	0,		0,	
yycrank+152,	0,		0,	
yycrank+196,	0,		yyvstop+146,
yycrank+0,	yysvec+25,	yyvstop+148,
yycrank+156,	yysvec+25,	yyvstop+150,
yycrank+0,	0,		yyvstop+152,
yycrank+0,	0,		yyvstop+154,
yycrank+164,	yysvec+25,	yyvstop+156,
yycrank+181,	yysvec+25,	yyvstop+158,
yycrank+208,	yysvec+25,	yyvstop+160,
yycrank+0,	yysvec+25,	yyvstop+162,
yycrank+193,	yysvec+25,	yyvstop+165,
yycrank+184,	yysvec+25,	yyvstop+167,
yycrank+0,	yysvec+25,	yyvstop+169,
yycrank+168,	yysvec+25,	yyvstop+172,
yycrank+188,	yysvec+25,	yyvstop+174,
yycrank+192,	yysvec+25,	yyvstop+176,
yycrank+196,	yysvec+25,	yyvstop+178,
yycrank+0,	yysvec+25,	yyvstop+180,
yycrank+196,	yysvec+25,	yyvstop+183,
yycrank+218,	yysvec+25,	yyvstop+185,
yycrank+226,	yysvec+25,	yyvstop+187,
yycrank+222,	yysvec+25,	yyvstop+189,
yycrank+209,	yysvec+25,	yyvstop+191,
yycrank+231,	yysvec+25,	yyvstop+193,
yycrank+240,	yysvec+25,	yyvstop+195,
yycrank+296,	yysvec+25,	yyvstop+197,
yycrank+231,	yysvec+25,	yyvstop+199,
yycrank+238,	yysvec+25,	yyvstop+201,
yycrank+0,	yysvec+25,	yyvstop+203,
yycrank+226,	yysvec+25,	yyvstop+206,
yycrank+257,	yysvec+25,	yyvstop+208,
yycrank+271,	yysvec+25,	yyvstop+210,
yycrank+264,	yysvec+25,	yyvstop+212,
yycrank+259,	yysvec+25,	yyvstop+214,
yycrank+286,	yysvec+25,	yyvstop+216,
yycrank+288,	yysvec+25,	yyvstop+218,
yycrank+293,	yysvec+25,	yyvstop+220,
yycrank+0,	yysvec+25,	yyvstop+222,
yycrank+303,	yysvec+25,	yyvstop+225,
yycrank+309,	yysvec+25,	yyvstop+227,
yycrank+320,	yysvec+25,	yyvstop+229,
yycrank+307,	yysvec+25,	yyvstop+231,
yycrank+316,	yysvec+25,	yyvstop+233,
yycrank+304,	yysvec+25,	yyvstop+235,
yycrank+310,	yysvec+25,	yyvstop+237,
yycrank+332,	yysvec+25,	yyvstop+239,
yycrank+331,	yysvec+25,	yyvstop+241,
yycrank+342,	yysvec+25,	yyvstop+243,
yycrank+332,	yysvec+25,	yyvstop+245,
yycrank+333,	yysvec+25,	yyvstop+248,
yycrank+326,	yysvec+25,	yyvstop+251,
yycrank+333,	yysvec+25,	yyvstop+253,
yycrank+349,	yysvec+25,	yyvstop+255,
yycrank+-479,	0,		yyvstop+257,
yycrank+-444,	yysvec+131,	yyvstop+259,
yycrank+-445,	yysvec+131,	yyvstop+262,
yycrank+-446,	yysvec+131,	yyvstop+265,
yycrank+-448,	yysvec+131,	yyvstop+268,
yycrank+-451,	yysvec+131,	yyvstop+271,
yycrank+-452,	yysvec+131,	yyvstop+273,
yycrank+-453,	yysvec+131,	yyvstop+276,
yycrank+-454,	yysvec+131,	yyvstop+279,
yycrank+-481,	0,		yyvstop+282,
yycrank+-468,	yysvec+131,	yyvstop+285,
yycrank+-456,	yysvec+131,	yyvstop+288,
yycrank+-469,	yysvec+131,	yyvstop+290,
yycrank+-471,	yysvec+131,	yyvstop+293,
yycrank+-473,	yysvec+131,	yyvstop+296,
yycrank+0,	0,		yyvstop+299,
yycrank+0,	0,		yyvstop+301,
yycrank+0,	0,		yyvstop+303,
yycrank+368,	0,		0,	
yycrank+375,	0,		0,	
yycrank+371,	0,		0,	
yycrank+393,	0,		0,	
yycrank+389,	0,		0,	
yycrank+0,	0,		yyvstop+305,
yycrank+393,	0,		0,	
yycrank+377,	0,		0,	
yycrank+0,	0,		yyvstop+307,
yycrank+383,	0,		0,	
yycrank+0,	0,		yyvstop+309,
yycrank+398,	0,		0,	
yycrank+396,	0,		0,	
yycrank+0,	0,		yyvstop+311,
yycrank+385,	0,		0,	
yycrank+407,	0,		0,	
yycrank+0,	0,		yyvstop+313,
yycrank+393,	0,		0,	
yycrank+395,	0,		0,	
yycrank+393,	0,		0,	
yycrank+409,	0,		0,	
yycrank+0,	0,		yyvstop+315,
yycrank+0,	0,		yyvstop+317,
yycrank+410,	yysvec+25,	yyvstop+319,
yycrank+395,	yysvec+25,	yyvstop+321,
yycrank+0,	yysvec+25,	yyvstop+323,
yycrank+0,	yysvec+25,	yyvstop+326,
yycrank+403,	yysvec+25,	yyvstop+329,
yycrank+404,	yysvec+25,	yyvstop+331,
yycrank+401,	yysvec+25,	yyvstop+333,
yycrank+408,	yysvec+25,	yyvstop+335,
yycrank+0,	yysvec+25,	yyvstop+337,
yycrank+0,	yysvec+25,	yyvstop+340,
yycrank+409,	yysvec+25,	yyvstop+343,
yycrank+414,	yysvec+25,	yyvstop+345,
yycrank+429,	yysvec+25,	yyvstop+347,
yycrank+426,	yysvec+25,	yyvstop+349,
yycrank+426,	yysvec+25,	yyvstop+351,
yycrank+423,	yysvec+25,	yyvstop+353,
yycrank+417,	yysvec+25,	yyvstop+355,
yycrank+424,	yysvec+25,	yyvstop+358,
yycrank+419,	yysvec+25,	yyvstop+360,
yycrank+431,	yysvec+25,	yyvstop+362,
yycrank+505,	yysvec+25,	yyvstop+364,
yycrank+430,	yysvec+25,	yyvstop+366,
yycrank+424,	yysvec+25,	yyvstop+368,
yycrank+431,	yysvec+25,	yyvstop+370,
yycrank+438,	yysvec+25,	yyvstop+372,
yycrank+437,	yysvec+25,	yyvstop+374,
yycrank+427,	yysvec+25,	yyvstop+376,
yycrank+432,	yysvec+25,	yyvstop+378,
yycrank+449,	yysvec+25,	yyvstop+380,
yycrank+440,	yysvec+25,	yyvstop+382,
yycrank+456,	yysvec+25,	yyvstop+384,
yycrank+459,	yysvec+25,	yyvstop+386,
yycrank+445,	yysvec+25,	yyvstop+388,
yycrank+452,	yysvec+25,	yyvstop+390,
yycrank+460,	yysvec+25,	yyvstop+392,
yycrank+462,	yysvec+25,	yyvstop+395,
yycrank+454,	yysvec+25,	yyvstop+397,
yycrank+470,	yysvec+25,	yyvstop+399,
yycrank+472,	yysvec+25,	yyvstop+401,
yycrank+459,	yysvec+25,	yyvstop+403,
yycrank+471,	yysvec+25,	yyvstop+405,
yycrank+476,	yysvec+25,	yyvstop+407,
yycrank+464,	yysvec+25,	yyvstop+409,
yycrank+470,	yysvec+25,	yyvstop+411,
yycrank+549,	yysvec+25,	yyvstop+414,
yycrank+482,	yysvec+25,	yyvstop+416,
yycrank+0,	yysvec+25,	yyvstop+418,
yycrank+468,	yysvec+25,	yyvstop+421,
yycrank+469,	yysvec+25,	yyvstop+423,
yycrank+482,	yysvec+25,	yyvstop+426,
yycrank+-577,	yysvec+131,	yyvstop+428,
yycrank+-578,	yysvec+131,	yyvstop+431,
yycrank+-579,	yysvec+131,	yyvstop+433,
yycrank+-580,	yysvec+131,	yyvstop+435,
yycrank+-581,	yysvec+131,	yyvstop+437,
yycrank+-583,	yysvec+131,	yyvstop+439,
yycrank+-584,	yysvec+131,	yyvstop+441,
yycrank+-585,	yysvec+131,	yyvstop+444,
yycrank+-586,	yysvec+131,	yyvstop+446,
yycrank+-588,	yysvec+131,	yyvstop+448,
yycrank+-589,	yysvec+131,	yyvstop+451,
yycrank+-591,	yysvec+131,	yyvstop+453,
yycrank+-592,	yysvec+131,	yyvstop+456,
yycrank+-593,	yysvec+131,	yyvstop+458,
yycrank+-594,	yysvec+131,	yyvstop+460,
yycrank+-595,	yysvec+131,	yyvstop+463,
yycrank+-596,	yysvec+131,	yyvstop+465,
yycrank+-597,	yysvec+131,	yyvstop+467,
yycrank+-598,	yysvec+131,	yyvstop+470,
yycrank+-599,	yysvec+131,	yyvstop+472,
yycrank+-600,	yysvec+131,	yyvstop+474,
yycrank+-601,	yysvec+131,	yyvstop+476,
yycrank+496,	0,		yyvstop+478,
yycrank+497,	0,		0,	
yycrank+498,	0,		0,	
yycrank+0,	0,		yyvstop+480,
yycrank+512,	0,		0,	
yycrank+500,	0,		0,	
yycrank+516,	0,		0,	
yycrank+502,	0,		0,	
yycrank+514,	0,		yyvstop+482,
yycrank+516,	0,		0,	
yycrank+505,	0,		0,	
yycrank+508,	0,		0,	
yycrank+0,	0,		yyvstop+484,
yycrank+522,	0,		0,	
yycrank+508,	0,		0,	
yycrank+509,	0,		yyvstop+486,
yycrank+0,	yysvec+25,	yyvstop+488,
yycrank+525,	yysvec+25,	yyvstop+491,
yycrank+508,	yysvec+25,	yyvstop+493,
yycrank+509,	yysvec+25,	yyvstop+495,
yycrank+528,	yysvec+25,	yyvstop+497,
yycrank+519,	yysvec+25,	yyvstop+499,
yycrank+530,	yysvec+25,	yyvstop+501,
yycrank+0,	yysvec+25,	yyvstop+503,
yycrank+524,	yysvec+25,	yyvstop+506,
yycrank+532,	yysvec+25,	yyvstop+508,
yycrank+524,	yysvec+25,	yyvstop+511,
yycrank+534,	yysvec+25,	yyvstop+513,
yycrank+535,	yysvec+25,	yyvstop+516,
yycrank+0,	yysvec+25,	yyvstop+518,
yycrank+605,	yysvec+25,	yyvstop+521,
yycrank+526,	yysvec+25,	yyvstop+523,
yycrank+607,	0,		0,	
yycrank+0,	yysvec+25,	yyvstop+525,
yycrank+524,	yysvec+25,	yyvstop+528,
yycrank+0,	yysvec+25,	yyvstop+530,
yycrank+537,	yysvec+25,	yyvstop+533,
yycrank+527,	yysvec+25,	yyvstop+535,
yycrank+0,	yysvec+25,	yyvstop+537,
yycrank+611,	yysvec+25,	yyvstop+540,
yycrank+0,	yysvec+25,	yyvstop+543,
yycrank+528,	yysvec+25,	yyvstop+546,
yycrank+531,	yysvec+25,	yyvstop+548,
yycrank+0,	yysvec+25,	yyvstop+550,
yycrank+542,	yysvec+25,	yyvstop+553,
yycrank+531,	yysvec+25,	yyvstop+555,
yycrank+531,	yysvec+25,	yyvstop+557,
yycrank+533,	yysvec+25,	yyvstop+559,
yycrank+534,	yysvec+25,	yyvstop+561,
yycrank+0,	yysvec+25,	yyvstop+563,
yycrank+0,	yysvec+25,	yyvstop+566,
yycrank+547,	yysvec+25,	yyvstop+569,
yycrank+542,	yysvec+25,	yyvstop+571,
yycrank+556,	yysvec+25,	yyvstop+573,
yycrank+538,	yysvec+25,	yyvstop+575,
yycrank+0,	yysvec+25,	yyvstop+577,
yycrank+623,	0,		0,	
yycrank+542,	yysvec+25,	yyvstop+580,
yycrank+625,	yysvec+25,	yyvstop+582,
yycrank+554,	yysvec+25,	yyvstop+584,
yycrank+0,	yysvec+25,	yyvstop+586,
yycrank+-649,	yysvec+131,	yyvstop+589,
yycrank+-650,	yysvec+131,	yyvstop+592,
yycrank+-651,	yysvec+131,	yyvstop+594,
yycrank+-652,	yysvec+131,	yyvstop+596,
yycrank+-653,	yysvec+131,	yyvstop+599,
yycrank+-654,	yysvec+131,	yyvstop+601,
yycrank+-656,	yysvec+131,	yyvstop+603,
yycrank+-655,	yysvec+131,	yyvstop+605,
yycrank+-657,	yysvec+131,	yyvstop+607,
yycrank+-659,	yysvec+131,	yyvstop+610,
yycrank+-658,	yysvec+131,	yyvstop+612,
yycrank+-661,	yysvec+131,	yyvstop+614,
yycrank+-660,	yysvec+131,	yyvstop+616,
yycrank+-663,	yysvec+131,	yyvstop+619,
yycrank+-662,	yysvec+131,	yyvstop+621,
yycrank+-664,	yysvec+131,	yyvstop+623,
yycrank+564,	0,		0,	
yycrank+575,	0,		0,	
yycrank+0,	0,		yyvstop+626,
yycrank+573,	0,		0,	
yycrank+0,	0,		yyvstop+628,
yycrank+564,	0,		0,	
yycrank+575,	0,		0,	
yycrank+563,	0,		0,	
yycrank+566,	0,		0,	
yycrank+579,	0,		0,	
yycrank+568,	0,		0,	
yycrank+571,	0,		0,	
yycrank+0,	0,		yyvstop+630,
yycrank+582,	0,		0,	
yycrank+0,	yysvec+25,	yyvstop+632,
yycrank+0,	yysvec+25,	yyvstop+635,
yycrank+0,	yysvec+25,	yyvstop+638,
yycrank+589,	yysvec+25,	yyvstop+641,
yycrank+582,	yysvec+25,	yyvstop+643,
yycrank+583,	yysvec+25,	yyvstop+645,
yycrank+597,	yysvec+25,	yyvstop+647,
yycrank+599,	yysvec+25,	yyvstop+649,
yycrank+599,	yysvec+25,	yyvstop+651,
yycrank+586,	yysvec+25,	yyvstop+653,
yycrank+604,	yysvec+25,	yyvstop+655,
yycrank+674,	0,		0,	
yycrank+592,	yysvec+25,	yyvstop+657,
yycrank+606,	0,		0,	
yycrank+0,	yysvec+25,	yyvstop+659,
yycrank+593,	yysvec+25,	yyvstop+662,
yycrank+608,	yysvec+25,	yyvstop+664,
yycrank+682,	0,		0,	
yycrank+0,	yysvec+25,	yyvstop+667,
yycrank+684,	yysvec+25,	yyvstop+670,
yycrank+685,	yysvec+25,	yyvstop+672,
yycrank+0,	yysvec+25,	yyvstop+674,
yycrank+604,	yysvec+25,	yyvstop+677,
yycrank+688,	yysvec+25,	yyvstop+679,
yycrank+0,	yysvec+25,	yyvstop+682,
yycrank+689,	yysvec+25,	yyvstop+685,
yycrank+621,	yysvec+25,	yyvstop+687,
yycrank+623,	yysvec+25,	yyvstop+689,
yycrank+692,	yysvec+25,	yyvstop+691,
yycrank+623,	0,		0,	
yycrank+694,	yysvec+25,	yyvstop+693,
yycrank+695,	0,		0,	
yycrank+0,	yysvec+25,	yyvstop+695,
yycrank+-718,	yysvec+131,	yyvstop+698,
yycrank+-719,	yysvec+131,	yyvstop+700,
yycrank+-720,	yysvec+131,	yyvstop+702,
yycrank+-721,	yysvec+131,	yyvstop+705,
yycrank+-722,	yysvec+131,	yyvstop+707,
yycrank+-723,	yysvec+131,	yyvstop+710,
yycrank+-726,	yysvec+131,	yyvstop+712,
yycrank+-725,	yysvec+131,	yyvstop+714,
yycrank+-727,	yysvec+131,	yyvstop+716,
yycrank+-728,	yysvec+131,	yyvstop+718,
yycrank+-729,	yysvec+131,	yyvstop+720,
yycrank+-730,	yysvec+131,	yyvstop+722,
yycrank+-731,	yysvec+131,	yyvstop+724,
yycrank+-732,	yysvec+131,	yyvstop+727,
yycrank+634,	0,		0,	
yycrank+630,	0,		0,	
yycrank+629,	0,		0,	
yycrank+714,	0,		0,	
yycrank+0,	0,		yyvstop+729,
yycrank+632,	0,		0,	
yycrank+0,	0,		yyvstop+731,
yycrank+0,	0,		yyvstop+733,
yycrank+0,	0,		yyvstop+735,
yycrank+719,	0,		0,	
yycrank+0,	0,		yyvstop+737,
yycrank+638,	yysvec+25,	yyvstop+739,
yycrank+717,	yysvec+25,	yyvstop+741,
yycrank+718,	yysvec+25,	yyvstop+743,
yycrank+0,	yysvec+25,	yyvstop+745,
yycrank+0,	yysvec+25,	yyvstop+748,
yycrank+744,	yysvec+25,	yyvstop+751,
yycrank+651,	yysvec+25,	yyvstop+753,
yycrank+0,	yysvec+25,	yyvstop+755,
yycrank+652,	0,		0,	
yycrank+654,	yysvec+25,	yyvstop+758,
yycrank+0,	0,		yyvstop+760,
yycrank+0,	yysvec+25,	yyvstop+762,
yycrank+660,	yysvec+25,	yyvstop+765,
yycrank+657,	0,		0,	
yycrank+740,	0,		0,	
yycrank+728,	0,		0,	
yycrank+0,	yysvec+25,	yyvstop+767,
yycrank+729,	0,		0,	
yycrank+736,	0,		0,	
yycrank+0,	yysvec+25,	yyvstop+770,
yycrank+0,	yysvec+25,	yyvstop+773,
yycrank+741,	0,		0,	
yycrank+0,	0,		yyvstop+776,
yycrank+745,	0,		0,	
yycrank+667,	0,		0,	
yycrank+-769,	yysvec+131,	yyvstop+778,
yycrank+-771,	yysvec+131,	yyvstop+780,
yycrank+-772,	yysvec+131,	yyvstop+782,
yycrank+-773,	yysvec+131,	yyvstop+784,
yycrank+-774,	yysvec+131,	yyvstop+786,
yycrank+-776,	yysvec+131,	yyvstop+789,
yycrank+-777,	yysvec+131,	yyvstop+791,
yycrank+-778,	yysvec+131,	yyvstop+794,
yycrank+-779,	yysvec+131,	yyvstop+797,
yycrank+-781,	yysvec+131,	yyvstop+800,
yycrank+-780,	yysvec+131,	yyvstop+802,
yycrank+0,	0,		yyvstop+805,
yycrank+0,	0,		yyvstop+807,
yycrank+0,	0,		yyvstop+809,
yycrank+691,	0,		0,	
yycrank+689,	0,		0,	
yycrank+0,	0,		yyvstop+811,
yycrank+694,	0,		0,	
yycrank+691,	0,		0,	
yycrank+0,	yysvec+25,	yyvstop+813,
yycrank+765,	0,		0,	
yycrank+766,	0,		0,	
yycrank+791,	0,		yyvstop+816,
yycrank+685,	yysvec+25,	yyvstop+818,
yycrank+0,	0,		yyvstop+820,
yycrank+0,	yysvec+25,	yyvstop+822,
yycrank+693,	yysvec+25,	yyvstop+825,
yycrank+0,	0,		yyvstop+827,
yycrank+701,	0,		0,	
yycrank+698,	0,		0,	
yycrank+702,	0,		0,	
yycrank+705,	0,		0,	
yycrank+706,	0,		0,	
yycrank+707,	0,		0,	
yycrank+709,	0,		0,	
yycrank+706,	0,		0,	
yycrank+0,	0,		yyvstop+829,
yycrank+-802,	yysvec+131,	yyvstop+831,
yycrank+-804,	yysvec+131,	yyvstop+834,
yycrank+-805,	yysvec+131,	yyvstop+837,
yycrank+-806,	yysvec+131,	yyvstop+840,
yycrank+-807,	yysvec+131,	yyvstop+842,
yycrank+-808,	yysvec+131,	yyvstop+844,
yycrank+-809,	yysvec+131,	yyvstop+847,
yycrank+-811,	yysvec+131,	yyvstop+849,
yycrank+722,	0,		0,	
yycrank+723,	0,		0,	
yycrank+729,	0,		0,	
yycrank+731,	0,		0,	
yycrank+733,	0,		0,	
yycrank+736,	0,		0,	
yycrank+0,	yysvec+25,	yyvstop+851,
yycrank+740,	yysvec+25,	yyvstop+854,
yycrank+744,	0,		0,	
yycrank+746,	0,		0,	
yycrank+0,	0,		yyvstop+856,
yycrank+0,	0,		yyvstop+858,
yycrank+0,	0,		yyvstop+860,
yycrank+0,	0,		yyvstop+862,
yycrank+748,	0,		0,	
yycrank+748,	0,		0,	
yycrank+-845,	yysvec+131,	yyvstop+864,
yycrank+-846,	yysvec+131,	yyvstop+866,
yycrank+-848,	yysvec+131,	yyvstop+868,
yycrank+-850,	yysvec+131,	yyvstop+870,
yycrank+741,	0,		0,	
yycrank+757,	0,		0,	
yycrank+746,	0,		0,	
yycrank+759,	0,		0,	
yycrank+0,	0,		yyvstop+872,
yycrank+0,	0,		yyvstop+874,
yycrank+0,	yysvec+25,	yyvstop+876,
yycrank+748,	0,		0,	
yycrank+761,	0,		0,	
yycrank+750,	0,		0,	
yycrank+763,	0,		0,	
yycrank+-858,	yysvec+131,	yyvstop+879,
yycrank+-859,	yysvec+131,	yyvstop+881,
yycrank+-860,	yysvec+131,	yyvstop+883,
yycrank+-861,	yysvec+131,	yyvstop+885,
yycrank+0,	0,		yyvstop+887,
yycrank+756,	0,		0,	
yycrank+0,	0,		yyvstop+889,
yycrank+757,	0,		0,	
yycrank+842,	0,		0,	
yycrank+759,	0,		0,	
yycrank+847,	0,		0,	
yycrank+764,	0,		0,	
yycrank+-872,	yysvec+131,	yyvstop+891,
yycrank+-873,	yysvec+131,	yyvstop+894,
yycrank+-874,	yysvec+131,	yyvstop+896,
yycrank+-876,	yysvec+131,	yyvstop+899,
yycrank+0,	0,		yyvstop+901,
yycrank+0,	0,		yyvstop+903,
yycrank+858,	0,		0,	
yycrank+860,	0,		0,	
yycrank+861,	0,		0,	
yycrank+862,	0,		0,	
yycrank+-886,	yysvec+131,	yyvstop+905,
yycrank+-887,	yysvec+131,	yyvstop+908,
yycrank+796,	0,		0,	
yycrank+867,	0,		0,	
yycrank+798,	0,		0,	
yycrank+869,	0,		0,	
yycrank+0,	0,		yyvstop+911,
yycrank+800,	0,		0,	
yycrank+0,	0,		yyvstop+913,
yycrank+801,	0,		0,	
yycrank+0,	0,		yyvstop+915,
yycrank+0,	0,		yyvstop+917,
0,	0,	0};
struct yywork *yytop = yycrank+992;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,'A' ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
#ident	"@(#)/usr/lib/lex/ncform.sl 1.1 1.0 01/15/86 20012 AT&T-SF"
int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
			yyfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
yyback(p, m)
	int *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
	return(input());
	}
yyoutput(c)
  int c; {
	output(c);
	}
yyunput(c)
   int c; {
	unput(c);
	}
