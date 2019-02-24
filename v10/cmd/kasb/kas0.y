/*	@(#)kas0.y	1.1	*/
%union {
	int	ival;
	struct exp *expr;
	struct symtab *sym;
	char *str;
	}

%token NOCHAR NL
%token CM SEMI COLON LP RP
%token PLUS MINUS MUL DIV NOT POP BAR INC
%token AND XOR LS RS
%token <ival> INT
%token <sym> DNAME UNAME TNAME PAGE
%token <str> STR
%token <sym> REG BRG MEM MAR REGL REGH PC PCR
%token <sym> MVINS SFINS DFINS BRINS JMPINS
%token <sym> SEG ORG DATA DEBUG

%nonassoc COLON
%left BAR XOR
%left AND
%left LS RS
%left PLUS MINUS
%left MUL DIV
%left NOT POP


%type <sym> fref
%type <ival> mach func page dst pdst sdst udst boreg oreg reg2
%type <expr> expr val db debug
%{
#include <stdio.h>
#include "kas.h"
struct	exp	explist[10];
struct	exp	*xp = { explist};
%}

%%
%{
	int	dinc;
	int	nlf;
%}
prog:		/* empty */
	| prog line = {
		xp = explist;
		if (nlf)
			lineno++;
	}
	;

line:	labels close
	| labels mach = {
		putins($2);
	}
	| labels pseudo close
	| debug close = {
		debug($1->xvalue, $1->xtype);
	}
	| error close = {
		yyerrok;
	}
	;

labels:		/* empty */
	| labels UNAME COLON = {
		$2->type = dot->type;
		backup($2,dot->value);
	}
	| labels INT COLON = {
		if ($2<0 || $2>9) {
			yyerror("illegal numeric label");
		} else {
			loclab[$2].name[0] = 1;
			backup(&loclab[$2], dot->value);
			loclab[$2].type = dot->value;
			loclab[$2].value = 0;
		}
	}
	;

close:	NL = {
		nlf = 1;
	}
	| SEMI = {
		nlf = 0;
	}
	;

mach:	MVINS fref CM udst close = {
		$$ = $1->value|$4;
		reloc[dot->value] = $2->value;
		$2->value = dot->value|RLCMV;
	}
	| MVINS POP fref CM udst close = {
		$$ = $1->value|$5;
		reloc[dot->value] = $3->value;
		$3->value = dot->value|RLCPOP;
	}
	| MVINS expr CM dst close = {
		$$ = $1->value|cksrc($2->xvalue&0377, $4);
	}
	| MVINS REG CM dst close = {
		$$ = $1->value|SRCBRG|SELA|ckreg($2->value, $4);
	}
	| MVINS oreg CM dst close = {
		$$ = $1->value|$2|$4;
	}
	| SFINS REG CM dst close = {
		$$ = $1->value|SRCBRG|ckreg($2->value, $4);
	}
	| SFINS REG close = {
		$$ = $1->value|SRCBRG|$2->value|DSTREG;
	}
	| DFINS reg2 CM REG CM dst close = {
		$$ = $1->value|$2|ckreg($4->value, $6);
	}
	| DFINS reg2 CM REG close = {
		$$ = $1->value|$2|$4->value|DSTREG;
	}
	| BRINS expr close = {
		$$ = $1->value|($2->xvalue&0377)|(($2->xvalue&01400)<<3);
		if ((dot->value&~01777) != ($2->xvalue&~01777))
			yyerror("branch out of range");
	}
	| BRINS fref close = {
		$$ = $1->value;
		reloc[dot->value] = $2->value;
		$2->value = dot->value|RLCBR;
	}
	| BRINS LP func RP page close = {
		$$ = $1->value|$3|$5;
	}
	| JMPINS expr close = {
		$$ = $1->value|($2->xvalue&0377);
	}
	| JMPINS fref close = {
		$$ = $1->value;
		reloc[dot->value] = $2->value;
		$2->value = dot->value|RLCMV;
	}
	| JMPINS LP func RP close = {
		$$ = $1->value|$3;
	}
	;

func:	REG = {
		$$ = $1->value|SRCBRG|SELA;
	}
	| reg2 = {
		$$ = $1|SELB;
	}
	| SFINS CM REG = {
		$$ = $1->value|$3->value|SRCBRG;
	}
	| DFINS CM reg2 CM REG = {
		$$ = $1->value|$3|$5->value;
	}
	;

page:	/* empty */ = {
		$$ = 0;
	}
	| CM PAGE = {
		$$ = $2->value;
	}
	| CM POP val = {
		$$ = ($3->xvalue&01400)<<3;
	}
	| CM POP fref = {
		$$ = 0;
		reloc[dot->value] = $3->value;
		$3->value = dot->value|RLCPG;
	}
	;

fref:
	UNAME
	| TNAME
	;

dst:	 MINUS = {
		$$ = 0;
	}
	| pdst
	| pdst BAR pdst = {
		$$ = ckdst($1, $3);
	}
	| pdst BAR pdst BAR pdst = {
		$$ = ckdst(ckdst($1, $3), $5);
	}
	;

pdst:	REG = {
		$$ = $1->value|DSTREG;
	}
	| BRG = {
		$$ = DSTBRG;
	}
	| MEM = {
		$$ = DSTMEM;
	}
	| REGL = {
		$$ = $1->value|DSTREGL;
	}
	| REGH = {
		$$ = $1->value|DSTREGH;
	}
	| BRG RS = {
		$$ = DSTBGRS;
	}
	| MAR = {
		$$ = DSTMAR;
	}
	| MAR INC = {
		$$ = DSTMARI;
	}
	| POP MAR = {
		$$ = DSTMARP;
	}
	| PCR = {
		$$ = DSTPCH|DSTREGH;
	}
	;

udst:	sdst
	| sdst BAR sdst = {
		$$ = ckdst($1, $3);
	}
	;

sdst:	BRG = {
		$$ = DSTBRG;
	}
	| MEM = {
		$$ = DSTMEM;
	}
	| MAR = {
		$$ = DSTMAR;
	}
	| MAR INC = {
		$$ = DSTMARI;
	}
	| POP MAR = {
		$$ = DSTMARP;
	}
	;

oreg:	reg2 = {
		$$ = $1|SELB;
	}
	| REGL = {
		$$ = ($1->value<<4)|SRCREGL;
	}
	| REGH = {
		$$ = ($1->value<<4)|SRCREGH;
	}
	| boreg = {
		$$ = ($1<<4)|SRCREGH;
	}
	;

reg2:	BRG = {
		$$ = SRCBRG;
	}
	| MEM = {
		$$ = SRCMEM;
	}
	;

boreg:	MAR = {
		$$ = SRCMARL;
	}
	| POP MAR = {
		$$ = SRCMARH;
	}
	| PC = {
		$$ = SRCPCL;
	}
	| POP PC = {
		$$ = SRCPCH;
	}
	;

expr:	val
	| LP expr RP = {
		$$ = $2;
	}
	| expr BAR expr = {
		$1->xvalue |= $3->xvalue;
	}
	| expr XOR expr = {
		$1->xvalue ^= $3->xvalue;
	}
	| expr AND expr = {
		$1->xvalue &= $3->xvalue;
	}
	| expr LS expr = {
		$1->xvalue <<= $3->xvalue;
	}
	| expr RS expr = {
		$1->xvalue >>= $3->xvalue;
	}
	| expr PLUS expr = {
		$1->xvalue += $3->xvalue;
	}
	| expr MINUS expr = {
		$1->xvalue -= $3->xvalue;
	}
	| expr MUL expr = {
		$1->xvalue *= $3->xvalue;
	}
	| expr DIV expr = {
		$1->xvalue /= $3->xvalue;
	}
	| NOT expr = {
		$2->xvalue = ~$2->xvalue;
		$$ = $2;
	}
	| POP expr = {
		$2->xvalue = ($2->xvalue>>8)&0377;
		$$ = $2;
	}
	| MINUS expr %prec POP = {
		$2->xvalue = -$2->xvalue;
		$$ = $2;
	}
	;

val:	DNAME = {
		$$ = xp++;
		$$->xtype = XABS;
		$$->xvalue = $1->value;
	}
	| INT = {
		$$ = xp++;
		$$->xtype = XABS;
		$$->xvalue = $1;
	}
	;

pseudo: SEG = {
		if (dot->type==XTEXT)
			textsv = dot->value;
		else	datasv = dot->value;
		dot->type = $1->value;
		if (dot->type==XTEXT)
			dot->value = textsv;
		else	dot->value = datasv;
	}
	| ORG expr = {
		if ($2->xvalue<0 || $2->xvalue>=NKMCI)
			yyerror("illegal org value");
		else	dot->value = $2->xvalue;
	}
	| data explist
	;

data:	DATA = {
		if (dot->type != XDATA) {
			yyerror("no data in text");
		}
		dinc = $1->value;
	}
	;

explist: outexpr
	| explist CM outexpr
	;

outexpr: expr = {
		putdat($1->xvalue&0377);
		if (dinc==2)
			putdat(($1->xvalue>>8)&0377);
		xp = explist;
	}
	;

debug:	db
	| db INT = {
		$1->xtype = $2;
	}
	| db STR = {
		$1->xtype = (int)$2;
	}
	;

db:	DEBUG = {
		$$ = xp++;
		$$->xvalue = $1->value;
		$$->xtype = 0;
	}
	;
%%

yyerror(s, a)
char *s;
{
	anyerrs++;
	if (infile)
		fprintf(stderr, "%s:", infile);
	fprintf(stderr, "%d:", lineno);
	fprintf(stderr, s, a);
	fprintf(stderr, "\n");
}

#include "kas0.yl"
