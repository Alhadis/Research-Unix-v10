#ifdef DEBUG
#include <stdio.h>
#endif
#include <libc.h>
#include "regprog.h"

/*
 * Parser Information
 */
typedef struct Node{
	Inst	*first;
	Inst	*last;
}Node;
#define	NSTACK	20
static Node	andstack[NSTACK];
static Node	*andp;
static int	atorstack[NSTACK];
static int	*atorp;
static int	cursubid;		/* id of current subexpression */
static int	subidstack[NSTACK];	/* parallel to atorstack */
static int	*subidp;
static int	lastwasand;	/* Last token was operand */
static int	nbra;
static char	*exprp;		/* pointer to next character in source expression */
static int	nclass;
static Class	*classp;
static Inst	*freep;
static int	errors;

/* predeclared crap */
static void operator();
static void pushand();
static void pushator();
static void evaluntil();
static void bldcclass();

static void
rcerror(s)
	char *s;
{
	errors++;
	regerror(s);
}

static Inst *
newinst(t)
	int t;
{
	freep->type=t;
	freep->left=0;
	freep->right=0;
	return freep++;
}

static void
operand(t)
	int t;
{
	register Inst *i;
	if(lastwasand)
		operator(CAT);	/* catenate is implicit */
	i=newinst(t);
	if(t==CCLASS)	/* ugh */
		i->right=(Inst *)&(classp[nclass-1]);	/* UGH! */
	pushand(i, i);
	lastwasand=TRUE;
}

static void
operator(t)
	int t;
{
	if(t==RBRA && --nbra<0)
		rcerror("unmatched right paren");
	if(t==LBRA) {
		if (++cursubid >= NSUBEXP)
			rcerror ("too many subexpressions");
		nbra++;
		if (lastwasand)
			operator(CAT);
	} else
		evaluntil(t);
	if(t!=RBRA)
		pushator(t);
	lastwasand=FALSE;
	if(t==STAR || t==QUEST || t==PLUS || t==RBRA)
		lastwasand=TRUE;	/* these look like operands */
}

static void
regerr2(s, c)
	char *s;
{
	char buf[100];
	char *cp = buf;
	while(*s)
		*cp++ = *s++;
	*cp++ = c;
	*cp = '\0'; 
	rcerror(buf);
}

static void
cant(s)
	char *s;
{
	char buf[100];
	strcpy(buf, "can't happen: ");
	strcat(buf, s);
	rcerror(buf);
}

static void
pushand(f, l)
	Inst *f, *l;
{
	if(andp >= &andstack[NSTACK])
		cant("operand stack overflow");
	andp->first=f;
	andp->last=l;
	andp++;
}

static void
pushator(t)
	int t;
{
	if(atorp >= &atorstack[NSTACK])
		cant("operator stack overflow");
	*atorp++=t;
	*subidp++=cursubid;
}

static Node *
popand(op)
{
	register Inst *inst;

	if(andp <= &andstack[0]) {
		regerr2("missing operand for ", op);
		inst=newinst(NOP);
		pushand(inst,inst);
	}
	return --andp;
}

static int
popator()
{
	if(atorp <= &atorstack[0])
		cant("operator stack underflow");
	--subidp;
	return *--atorp;
}

static void
evaluntil(pri)
	register pri;
{
	register Node *op1, *op2;
	register Inst *inst1, *inst2;

	while(pri==RBRA || atorp[-1]>=pri){
		switch(popator()){
		default:
			rcerror("unknown operator in evaluntil");
			break;
		case LBRA:		/* must have been RBRA */
			op1=popand('(');
			inst2=newinst(RBRA);
			inst2->subid = *subidp;
			op1->last->next = inst2;
			inst1=newinst(LBRA);
			inst1->subid = *subidp;
			inst1->next=op1->first;
			pushand(inst1, inst2);
			return;
		case OR:
			op2=popand('|');
			op1=popand('|');
			inst2=newinst(NOP);
			op2->last->next=inst2;
			op1->last->next=inst2;
			inst1=newinst(OR);
			inst1->right=op1->first;
			inst1->left=op2->first;
			pushand(inst1, inst2);
			break;
		case CAT:
			op2=popand(0);
			op1=popand(0);
			op1->last->next=op2->first;
			pushand(op1->first, op2->last);
			break;
		case STAR:
			op2=popand('*');
			inst1=newinst(OR);
			op2->last->next=inst1;
			inst1->right=op2->first;
			pushand(inst1, inst1);
			break;
		case PLUS:
			op2=popand('+');
			inst1=newinst(OR);
			op2->last->next=inst1;
			inst1->right=op2->first;
			pushand(op2->first, inst1);
			break;
		case QUEST:
			op2=popand('?');
			inst1=newinst(OR);
			inst2=newinst(NOP);
			inst1->left=inst2;
			inst1->right=op2->first;
			op2->last->next=inst2;
			pushand(inst1, inst2);
			break;
		}
	}
}

static Prog *
optimize(pp)
	Prog *pp;
{
	register Inst *inst, *target;
	int size;
	Prog *npp;

	/*
	 *  get rid of NOOP chains
	 */
	for(inst=pp->firstinst; inst->type!=END; inst++){
		target=inst->next;
		while(target->type == NOP)
			target=target->next;
		inst->next=target;
	}

	/*
	 *  The original allocation is for an area larger than
	 *  necessary.  Reallocate to the actual space used
	 *  and then relocate the code.
	 */
	size = sizeof(Prog) + (freep - pp->firstinst)*sizeof(Inst);
	npp = (Prog *)realloc((char *)pp, size);
	if(npp==NULL || npp==pp)
		return(pp);
	freep = &npp->firstinst[freep - pp->firstinst];
	npp->startinst = &npp->firstinst[pp->startinst - pp->firstinst];
	for(inst=npp->firstinst; inst<freep; inst++){
		switch(inst->type){
		case OR:
		case STAR:
		case PLUS:
		case QUEST:
			inst->right = &npp->firstinst[inst->right - pp->firstinst];
			break;
		case CCLASS:
			inst->right = (Inst *) &npp->class[(Class*)inst->right - pp->class];
			break;
		}
		inst->left = &npp->firstinst[inst->left - pp->firstinst];
	}
	return(npp);
}

#ifdef	DEBUG
static char *
dumptype(t){
	static char ordinary[4] = "'.'";

	switch(t){
	case START:	return "START";
	case RBRA:	return "RBRA";
	case LBRA:	return "LBRA";
	case OR:	return "OR";
	case CAT:	return "CAT";
	case STAR:	return "STAR";
	case PLUS:	return "PLUS";
	case QUEST:	return "QUEST";
	case ANY:	return "ANY";
	case NOP:	return "NOP";
	case BOL:	return "BOL";
	case EOL:	return "EOL";
	case CCLASS:	return "CCLASS";
	case END:	return "END";
	default:
		ordinary[1] = t;
		return ordinary;
	}
}

static void
dumpstack(){
	Node *stk;
	int *ip;

	printf("operators\n");
	for(ip=atorstack; ip<atorp; ip++)
		printf("0%o\n", *ip);
	printf("operands\n");
	for(stk=andstack; stk<andp; stk++){
		printf("%s\t", dumptype(stk->first->type));
		printf("%s\n", dumptype(stk->last->type));
	}
}

static void
putC(c)
{
	if(c < ' ' || '~' < c){
		switch(c){
		default:
			putchar('^');
			putchar((c + '@') & 0x7f);
			return;
		case '\b':
			c = 'b';
			break;
		case '\t':
			c = 't';
			break;
		case '\n':
			c = 'n';
			break;
		case '\v':
			c = 'v';
			break;
		case '\f':
			c = 'f';
			break;
		case '\r':
			c = 'r';
			break;
		}
		putchar('\\');
	}
	putchar(c);
}

static void
putCHAR(from, to)
{
	if(from != 0)
		if(from == to)
			putC(from);
		else{
			putC(from);
			putchar('-');
			putC(to);
		}
}

static void
dump(pp)
	Prog *pp;
{
	int c, from;
	Inst *l;
	Class *classp;

	l=pp->firstinst;
	do{
		printf("%d:\t%s\t%d\t", l-pp->firstinst, dumptype(l->type),
			l->left-pp->firstinst);
		if(l->type == CCLASS){
			classp = (Class*) l->right;
			putchar('[');
			if(classp->map[0] & 02){	/* ^A? */
				putchar('^');		/* assume negation */
				for(from=0, c=1; c < 128; ++c){
					if(classp->map[c/8] & (1<<(c&07))){
						putCHAR(from, c-1);
						from = 0;
					} else {
						if(from == 0)
							from = c;
					}
				}
				putCHAR(from, c-1);
			} else {
				for(from=0, c=1; c < 128; ++c){
					if(classp->map[c/8] & (1<<(c&07))){
						if(from == 0)
							from = c;
					} else {
						putCHAR(from, c-1);
						from = 0;
					}
				}
				putCHAR(from, c-1);
			}
			putchar(']');
		} else
			printf("%d", l->right-pp->firstinst);
		putchar('\n');
	}while(l++->type);
}
#endif

static void
startlex(s)
	char *s;
{
	exprp=s;
	nclass=0;
	nbra=0;
}

static Class *
newclass(){
	register Class *p;
	register n;

	if(nclass >= NCLASS)
		regerr2("too many character classes; limit", NCLASS+'0');
	p = &(classp[nclass++]);
	for(n=0; n<16; n++)
		p->map[n]=0;
	return p;
}

static int
lex(){
	register c= *exprp++;

	switch(c){
	case '\\':
		if(*exprp)
			c= *exprp++;
		break;
	case 0:
		c=END;
		--exprp;	/* In case we come here again */
		break;
	case '*':
		c=STAR;
		break;
	case '?':
		c=QUEST;
		break;
	case '+':
		c=PLUS;
		break;
	case '|':
		c=OR;
		break;
	case '.':
		c=ANY;
		break;
	case '(':
		c=LBRA;
		break;
	case ')':
		c=RBRA;
		break;
	case '^':
		c=BOL;
		break;
	case '$':
		c=EOL;
		break;
	case '[':
		c=CCLASS;
		bldcclass();
		break;
	}
	return c;
}

static int
nextc(){
	if(exprp[0]==0 || (exprp[0]=='\\' && exprp[1]==0))
		rcerror("malformed '[]'");
	if(exprp[0]=='\\'){
		exprp++;
		return *exprp++|0200;
	}
	return *exprp++;
}

static void
bldcclass(){
	register c1, c2;
	register Class *classp;
	register negate=FALSE;

	classp=newclass();
	/* we have already seen the '[' */
	if(*exprp=='^'){
		negate=TRUE;
		exprp++;
	}
	while((c1=c2=nextc()) != ']'){
		if(*exprp=='-'){
			exprp++;	/* eat '-' */
			if((c2=nextc()) == ']')
				rcerror("malformed '[]'");
		}
		for((c1&=0177), (c2&=0177); c1<=c2; c1++)
			classp->map[c1/8] |= 1<<(c1&07);
	}
	if(negate)
		for(c1=0; c1<16; c1++)
			classp->map[c1]^=0377;
	classp->map[0] &= 0376;		/* exclude NUL */
}

extern regexp *
regcomp(s)
	char *s;
{
	register token;
	Prog *pp;

	/* get memory for the program */
	pp = (Prog *)malloc(sizeof(Prog) + 3*sizeof(Inst)*strlen(s));
	if (pp == NULL) {
		rcerror("out of memory");
		return NULL;
	}
	freep = pp->firstinst;
	classp = pp->class;
	errors = 0;

	/* go compile the sucker */
	startlex(s);
	atorp=atorstack;
	andp=andstack;
	subidp=subidstack;
	lastwasand=FALSE;
	cursubid=0;

	/* Start with a low priority operator to prime parser */
	pushator(START-1);
	while((token=lex()) != END){
		if((token&0300) == OPERATOR)
			operator(token);
		else
			operand(token);
	}

	/* Close with a low priority operator */
	evaluntil(START);

	/* Force END */
	operand(END);
	evaluntil(START);
#ifdef DEBUG
	dumpstack();
#endif
	if(nbra)
		rcerror("unmatched left paren");
	--andp;	/* points to first and only operand */
	pp->startinst=andp->first;
#ifdef DEBUG
	dump(pp);
#endif
	pp = optimize(pp);
#ifdef DEBUG
	printf("start: %d\n", pp->startinst-pp->firstinst);
	dump(pp);
	fflush(stdout);
#endif
	if (errors) {
		free((char *)pp);
		pp = NULL;
	}
	return (regexp *)pp;
}

#ifdef DEBUG
#include <setjmp.h>

jmp_buf	jmpbuf;

main( argc, argv )
	char **argv;
{
	regexp *prog = NULL;
	regsubexp match[NSUBEXP];
	char line[256];
	int i;

	while( TRUE ) {
		setjmp( jmpbuf );

		if(prog)
			free( prog );

		do {
			fputs( "Enter re: ", stdout );
			if ( gets(line) == NULL )
				exit(0);
		} while( (prog = regcomp(line)) == NULL );

		fputs( "Enter string: ", stdout );
		while( gets(line) != NULL ) {
			if ( !regexec(prog,line,match,NSUBEXP) )
				puts( "*** NO MATCH ***" );
			else
				for( i = 0; i < NSUBEXP; ++i )
					if ( match[i].sp )
						printf( "match[%d] = \"%.*s\"\n", i, match[i].ep - match[i].sp, match[i].sp );
		}
	}
}

regerror( s )
	char *s;
{
	puts( s );
	longjmp( jmpbuf );
}

#endif
