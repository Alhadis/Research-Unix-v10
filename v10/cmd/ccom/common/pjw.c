#include "mfile2.h"
extern struct symtab {	/* ripped untimely from mfile1.h */
	char *sname;
	TWORD stype;  /* type word */

	char sclass;  /* storage class */
	char slevel;  /* scope level */
	char sflags;  /* flags for set, use, hidden, mos, etc. */
	int offset;  /* offset or value */
	short dimoff; /* offset into the dimension table */
	short sizoff; /* offset into the size table */
	short suse;  /* line number of last use of the variable */
	struct symtab *scopelink;	/* chain of symbols at same level */
} stab[];

#define BINOP 1000
#define ARG 1001
#define UCALL 1002
#define BINARY 1003
#define COPY 1004
#define ENTRY 1005
#define LOCCTR 1006
#define LXINFO 1008
#define URETURN 1010
#define USTCALL 1011
#define STRANGE 1012
struct init {
	int op;
	int type;
	char *name;
} ar[] = {
	{1, 0, "???"},
	{AND, BINOP, "and"},
	{ANDAND, BINOP, "andand"},
	{ASG AND, BINOP, "asgand"},
	{UNARY AND, UNOP, "unaryand"},
	{ARG, ARG, "arg"},
	{ARS, BINOP, "ars"},
	{ASSIGN, BINOP, "assign"},
	{CALL, CALL, "call"},
	{FORTCALL, CALL, "fortcall"},
	{UNARY CALL, UCALL, "unarycall"},
	{UNARY FORTCALL, UNOP, "unaryfortcall"},
	{CBRANCH, BINARY, "cbranch "},
	{CM, BINARY, "cm "},
	{CMP, BINARY, "cmp "},
	{COLON, BINOP, "colon"},
	{COMOP, BINOP, "comop"},
	{COMPL, UNOP, "compl"},
	{CONV, UNOP, "conv"},
	{COPY, COPY, "copy"},
	{DECR, BINOP, "decr"},
	{DIV, BINOP, "div"},
	{ASG DIV, BINOP, "asgdiv"},
	{ENTRY, ENTRY, "entry"},
	{EPILOG, EPILOG, "epilog"},
	{ER, BINOP, "er"},
	{ASG ER, BINOP, "asger"},
	{FCON, FCON, "fcon"},
	{FLD, FLD, "fld"},
	{FUNARG, UNOP, "funarg"},
	{FREE, FREE, "free"},
	{GENBR, GENBR, "genbr"},
	{GENLAB, GENLAB, "genlab"},
	{GENUBR, GENLAB, "genubr"},
	{GOTO, GOTO, "goto"},
	{ICON, ICON, "icon"},
	{INCR, BINOP, "incr"},
	{INIT, UNOP, "init"},
	{LABELPT, GOTO, "labelpt"},
	{LOCCTR, LOCCTR, "locctr"},
	{LS, BINOP, "ls"},
	{ASG LS, BINOP, "asgls"},
	{LXINFO, LXINFO, "lxinfo"},
	{MINUS, BINOP, "minus"},
	{ASG MINUS, BINOP, "asgminus"},
	{UNARY MINUS, UNOP, "unaryminus"},
	{MOD, BINOP, "mod"},
	{ASG MOD, BINOP, "asgmod"},
	{MUL, BINOP, "mul"},
	{ASG MUL, BINOP, "asgmul"},
	{NAME, NAME, "name"},
	{NOT, UNOP, "not"},
	{OR, BINOP, "or"},
	{ASG OR, BINOP, "asgor"},
	{OROR, BINARY, "oror "},
	{PLUS, BINOP, "plus"},
	{ASG PLUS, BINOP, "asgplus"},
	{PROLOG, PROLOG, "prolog"},
	{QUEST, BINARY, "quest"},
	{REG, REG, "reg"},
	{EQ, BINOP, "eql"},
	{NE, BINOP, "neq"},
	{GT, BINOP, "gtr"},
	{GE, BINOP, "geq"},
	{LT, BINOP, "lss"},
	{LE, BINOP, "leq"},
	{UGT, BINOP, "gtru"},
	{UGE, BINOP, "gequ"},
	{ULT, BINOP, "lssu"},
	{ULE, BINOP, "lequ"},
	{RETURN, RETURN, "return"},
	{UNARY RETURN, URETURN, "unaryreturn"},
	{RNODE, STRANGE, "rnode"},
	{QNODE, STRANGE, "qnode"},
	{SNODE, STRANGE, "snode"},
	{RS, BINOP, "rs"},
	{ASG RS, BINOP, "asgrs"},
	{STAR, UNOP, "star"},
	{UNARY MUL, UNOP, "star"},
	{STARG, STARG, "starg"},
	{STASG, STASG, "stasg"},
	{STREF, STASG, "stref"},
	{STCALL, STCALL, "stcall"},
	{UNARY STCALL, USTCALL, "unarystcall"},
	{SWBEG, SWBEG, "swbeg"},
	{SWCASE, SWCASE, "swcase"},
	{SWDEF, SWDEF, "swdef"},
	{SWEND, SWEND, "swend"},
	{TEMP, TEMP, "temp"},
	{VAUTO, VAUTO, "vauto"},
	{VPARAM, VAUTO, "vparam"},
	{0, 0, 0}
};
int opindex[256];
int bothdebug;
FILE *fd;
pjwreader(p)
NODE *p;
{	int n;
	if(bothdebug) {
		printf("#tree ", p);
		prtree(p);
		putchar('\n');
		gencode(p);
		tfree(p);
	}
	else {
		gencode(p);
		tfree(p);
	}
}
prtree(p)
NODE *p;
{	int i, n;
	static init = 0;
	if(!init++)
		myinit();
	if(p == 0) {	
		printf("???");
		return;
	}
	i = opindex[p->in.op];
	if(i == 0) {
		printf("(unk op %d ???) ", p->in.op);
		return;
	}
	printf("(%s", ar[i].name);
	switch(ar[i].type) {
	default:
		printf("%d???) ", ar[i].type);
		return;
	case STRANGE:
		printf(") ");
		return;
	case BINOP:
		prtype(p->in.type);
	case BINARY:
		prtree(p->in.left);
		prtree(p->in.right);
		printf(") ");
		return;
	case UNOP:
		prtype(p->in.type);
		prtree(p->in.left);
		printf(") ");
		return;
	case ARG:
		prtree(p->in.left);
		printf("%d) ", p->tn.rval);
		return;
	case EPILOG:
		printf(" %d %d) ", p->tn.lval, p->tn.rval);
		return;
	case FLD:
		printf(" %d %d ", p->tn.rval % 64, p->tn.rval / 64);
		prtree(p->in.left);
		printf(") ");
		return;
	case CALL:
		prtype(p->in.type);
		printf("%d ", p->stn.argsize/32);
		prtree(p->in.left);
		prtree(p->in.right);
		printf(") ");
		return;
	case UCALL:
		prtype(p->in.type);
		prtree(p->in.left);
		printf(") ");
		return;
	case FCON:
		prtype(p->in.type);
		printf("%g) ", p->fpn.dval);
		return;
	case GENBR:
		printf(" %s L%d ", ar[opindex[p->bn.lop]].name, p->bn.label);
		prtree(p->in.left);
		printf(") ");
		return;
	case GENLAB:
		prtype(p->in.type);
		printf(" L%d ", p->bn.label);
		prtree(p->in.left);
		printf(") ");
		return;
	case GOTO:
		printf(" L%d ", p->bn.label);
		printf(") ");
		return;
	case ICON:
		prtype(p->in.type);
		if(p->tn.name)
			if(p->tn.lval)
				printf("%s+%d) ", p->tn.name, p->tn.lval);
			else
				printf("%s) ", p->tn.name);
		else
			printf("%d) ", p->tn.lval);
		return;
	case VAUTO:
		prtype(p->in.type);
		printf("%d) ", p->tn.lval);
		return;
	case NAME:
		prtype(p->in.type);
		if(p->tn.lval)
			printf("%s+%d) ", p->tn.name, p->tn.lval);
		else if(p->tn.name)
			printf("%s) ", p->tn.name);
		else	if(p->tn.rval >= 0)
			printf("%s) ", stab[p->tn.rval].sname);
		else
			printf("L%d) ", -p->tn.rval);
		return;
	case PROLOG:
		prtype(p->in.type);
		printf("%s %d) ", p->tn.name, p->tn.lval);
		return;
	case RETURN:
		printf("%s) ", p->tn.name);
		return;
	case URETURN:
		prtype(p->in.type);
		printf("%s ", p->tn.name);
		prtree(p->in.left);
		return;
	case SWCASE:
		printf(" %d L%d) ", p->tn.lval, p->tn.rval);
		return;
	case SWDEF:
		printf(" L%d) ", p->tn.rval);
		return;
	case SWEND:
		printf(" %d %d) ", p->tn.lval, p->tn.rval);
		return;
	case STARG:
		printf(" %d %d ", p->tn.rval, p->stn.stsize);
		prtree(p->in.left);
		printf(") ");
		return;
	case STASG:
		printf(" %d ", p->stn.stsize);
		prtree(p->in.left);
		prtree(p->in.right);
		printf(") ");
		return;
	case STCALL:
		printf(" %d %d ", p->stn.stsize, p->stn.argsize);
		prtree(p->in.left);
		prtree(p->in.right);
		printf(") ");
		return;
	case USTCALL:
		printf("%d ", p->stn.stsize);
		prtree(p->in.left);
		printf(") ");
		return;
	case REG:
		prtype(p->in.type);
		printf("%d) ", p->tn.rval, p->tn.lval);
		return;
	}
}
prtype(n)
{	char *s;
	switch(n) {
	default:
		cerror("prtype(#%x)\n", n);
	case TCHAR:	s = "C"; break;
	case TUCHAR:	s = "UC"; break;
	case TSHORT:	s = "S"; break;
	case TUSHORT:	s = "US"; break;
	case TINT:	s = "I"; break;
	case TUNSIGNED:	s = "U"; break;
	case TLONG:	s = "L"; break;
	case TULONG:	s = "UL"; break;
	case TFLOAT:	s = "F"; break;
	case TDOUBLE:	s = "D"; break;
	case TSTRUCT:	s = "St"; break;
	case TPOINT:	s = "P"; break;
	}
	printf("%s ", s);
}
myinit()
{	int i;
	for(i = 0; ar[i].name; i++)
		opindex[ar[i].op] = i;
}
pjwend(p)
NODE *p;
{
	if(bothdebug)
		printf("#treeend\n");
}
