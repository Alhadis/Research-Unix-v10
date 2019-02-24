#include "sam.h"

Rangeset	sel;
String		lastregexp;
/*
 * Machine Information
 */
typedef struct Inst{
	int	type;	/* < 0x200 ==> literal, otherwise action */
	union {
		int sid;
		struct Inst *other;
	}u;
	struct Inst *right;
	struct Inst *left;
}Inst;
#define	next	left	/* Left branch is usually just next ptr */
#define subid	u.sid
#define right	u.other
#define	NPROG	1024
Inst	program[NPROG];
Inst	*progp;
Inst	*startinst;	/* First inst. of program; might not be program[0] */
Inst	*bstartinst;	/* same for backwards machine */

typedef struct Ilist{
	Inst	*inst;		/* Instruction of the thread */
	Rangeset se;
	Posn	startp;		/* first char of match */
}Ilist;
#define	NLIST	128
Ilist	*tl, *nl;	/* This list, next list */
Ilist	list[2][NLIST];
static	Rangeset sempty;

/*
 * Actions and Tokens
 *
 *	0x2xx are operators, value == precedence
 *	0x3xx are tokens, i.e. operands for operators
 */
#define	OPERATOR	0x200	/* Bitmask of all operators */
#define	START		0x200	/* Start, used for marker on stack */
#define	RBRA		0x201	/* Right bracket, ) */
#define	LBRA		0x202	/* Left bracket, ( */
#define	OR		0x203	/* Alternation, | */
#define	CAT		0x204	/* Concatentation, implicit operator */
#define	STAR		0x205	/* Closure, * */
#define	PLUS		0x206	/* a+ == aa* */
#define	QUEST		0x207	/* a? == a|nothing, i.e. 0 or 1 a's */
#define	ANY		0x300	/* Any character but newline, . */
#define	ANYNL		0x301	/* Any character, including newline, @ */
#define	NOP		0x302	/* No operation, internal use only */
#define	BOL		0x303	/* Beginning of line, ^ */
#define	EOL		0x304	/* End of line, $ */
#define	CCLASS		0x305	/* Character class, [] */
#define	END		0x377	/* Terminate: match found */

/*
 * Parser Information
 */
typedef struct Node{
	Inst	*first;
	Inst	*last;
}Node;
#define	NSTACK	20
Node	andstack[NSTACK];
Node	*andp;
int	atorstack[NSTACK];
int	*atorp;
int	lastwasand;	/* Last token was operand */
int	cursubid;
int	subidstack[NSTACK];
int	*subidp;
int	backwards;
int	nbra;
uchar	*exprp;		/* pointer to next character in source expression */
#define	NCLASS	20
int	nclass;
char	class[NCLASS][32];	/* 32 bytes == 256 bits, one bit per char */

Inst *
newinst(t)
	int t;
{
	if(progp>=&program[NPROG])
		error(Etoolong);
	progp->type=t;
	progp->left=0;
	progp->right=0;
	return progp++;
}
Inst *
realcompile(s)
	uchar *s;
{
	register token;

	startlex(s);
	atorp=atorstack;
	andp=andstack;
	subidp=subidstack;
	cursubid=0;
	lastwasand=FALSE;
	/* Start with a low priority operator to prime parser */
	pushator(START-1);
	while((token=lex()) != END){
		if((token&0x300) == OPERATOR)
			operator(token);
		else
			operand(token);
	}
	/* Close with a low priority operator */
	evaluntil(START);
	/* Force END */
	operand(END);
	evaluntil(START);
	if(nbra)
		error(Eleftpar);
	--andp;	/* points to first and only operand */
	return andp->first;
}
compile(r)
	Regexp *r;
{
	register String *s=&r->text;
	Inst *oprogp;
	if(s->n==lastregexp.n &&
	    strncmp((char *)s->s, (char *)lastregexp.s, lastregexp.n)==0)
		return;
	progp=program;
	backwards=FALSE;
	startinst=realcompile(s->s);
	optimize(program);
	oprogp=progp;
	backwards=TRUE;
	bstartinst=realcompile(s->s);
	optimize(oprogp);
	strdupstr(&lastregexp, s);
}
operand(t)
	int t;
{
	register Inst *i;
	if(lastwasand)
		operator(CAT);	/* catenate is implicit */
	i=newinst(t);
	if(t==CCLASS)	/* ugh */
		i->right=(Inst *)class[nclass-1];	/* UGH! */
	pushand(i, i);
	lastwasand=TRUE;
}
operator(t)
	int t;
{
	if(t==RBRA && --nbra<0)
		error(Erightpar);
	if(t==LBRA){
/*
 *		if(++cursubid >= NSUBEXP)
 *			error(Esubexp);
 */
		cursubid++;	/* silently ignored */
		nbra++;
		if(lastwasand)
			operator(CAT);
	}else
		evaluntil(t);
	if(t!=RBRA)
		pushator(t);
	lastwasand=FALSE;
	if(t==STAR || t==QUEST || t==PLUS || t==RBRA)
		lastwasand=TRUE;	/* these look like operands */
}
cant(s)
	char *s;
{
	char buf[100];
	sprint(buf, "can't happen: %s", s);
	panic(buf);
}
pushand(f, l)
	Inst *f, *l;
{
	if(andp >= &andstack[NSTACK])
		cant("operand stack overflow");
	andp->first=f;
	andp->last=l;
	andp++;
}
pushator(t)
	int t;
{
	if(atorp >= &atorstack[NSTACK])
		cant("operator stack overflow");
	*atorp++=t;
	if(cursubid >= NSUBEXP)
		*subidp++= -1;
	else
		*subidp++=cursubid;
}
Node *
popand(op)
{
	if(andp <= &andstack[0])
		error_c(Emissop, op);
	return --andp;
}
popator()
{
	if(atorp <= &atorstack[0])
		cant("operator stack underflow");
	--subidp;
	return *--atorp;
}
evaluntil(pri)
	register pri;
{
	register Node *op1, *op2, *t;
	register Inst *inst1, *inst2;
	while(pri==RBRA || atorp[-1]>=pri){
		switch(popator()){
		case LBRA:
			op1=popand('(');
			inst2=newinst(RBRA);
			inst2->subid = *subidp;
			op1->last->next = inst2;
			inst1=newinst(LBRA);
			inst1->subid = *subidp;
			inst1->next=op1->first;
			pushand(inst1, inst2);
			return;		/* must have been RBRA */
		default:
			panic("unknown regexp operator");
			break;
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
			if(backwards && op2->first->type!=END)
				t=op1, op1=op2, op2=t;
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
optimize(start)
	register Inst *start;
{
	register Inst *inst, *target;

	for(inst=start; inst->type!=END; inst++){
		target=inst->next;
		while(target->type == NOP)
			target=target->next;
		inst->next=target;
	}
}
#ifdef	DEBUG
dumpstack(){
	Node *stk;
	int *ip;

	dprint("operators\n");
	for(ip=atorstack; ip<atorp; ip++)
		dprint("0%o\n", *ip);
	dprint("operands\n");
	for(stk=andstack; stk<andp; stk++)
		dprint("0%o\t0%o\n", stk->first->type, stk->last->type);
}
dump(){
	Inst *l;

	l=program;
	do{
		dprint("%d:\t0%o\t%d\t%d\n", l-program, l->type,
			l->left-program, l->right-program);
	}while(l++->type);
}
#endif
startlex(s)
	uchar *s;
{
	exprp=s;
	nclass=0;
	nbra=0;
}
char *
newclass(){
	register char *p;
	register n;

	if(nclass >= NCLASS)
		error(Eclass);
	p=class[nclass++];
	for(n=0; n<32; n++)
		p[n]=0;
	return p;
}
lex(){
	register c= *exprp++;

	switch(c){
	case '\\':
		if(*exprp)
			if((c= *exprp++)=='n')
				c='\n';
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
	case '@':
		c=ANYNL;
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
nextrec(){
	if(exprp[0]==0 || (exprp[0]=='\\' && exprp[1]==0))
		error(Ebadclass);
	if(exprp[0]=='\\'){
		exprp++;
		if(*exprp=='n'){
			exprp++;
			return '\n';
		}
		return *exprp++|0x100;
	}
	return *exprp++;
}
bldcclass(){
	register c1, c2;
	register char *classp;
	register negate=FALSE;

	classp=newclass();
	/* we have already seen the '[' */
	if(*exprp=='^'){
		negate=TRUE;
		exprp++;
	}
	while((c1=c2=nextrec()) != ']'){
		if(*exprp=='-'){
			exprp++;	/* eat '-' */
			if((c2=nextrec()) == ']')
				error(Ebadclass);
		}
		for((c1&=0xFF), (c2&=0xFF); c1<=c2; c1++)
			classp[c1/8] |= 1<<(c1&07);
	}
	if(negate){
		for(c1=0; c1<32; c1++)
			classp[c1]^=0xFF;
		classp[1]&=~(1<<('\n'-8));	/* exclude '\n' */
	}
	classp[0]&=0xFE;		/* exclude NUL */
}
/*
 * Note optimization in addinst:
 * 	*l must be pending when addinst called; if *l has been looked
 *		at already, the optimization is a bug.
 */
addinst(l, inst, sep)
	register Ilist *l;
	register Inst *inst;
	Rangeset *sep;
{
	register Ilist *p;

	for(p=l; p->inst; p++){
		if(p->inst==inst){
			if((sep)->p[0].p1 < p->se.p[0].p1)
				p->se= *sep;	/* this would be bug */
			return;	/* It's already there */
		}
	}
	p->inst=inst;
	p->se= *sep;
	(++p)->inst=0;
}
execute(f, startp, eof)
	File *f;
	Posn startp, eof;
{
	register flag=0;
	register Inst *inst;
	register Ilist *tlp;
	register Posn p=startp;
	int nnl=0, ntl;
	register c;
	int wrapped=0;
	register startchar=startinst->type<OPERATOR? startinst->type : 0;

	list[0][0].inst=list[1][0].inst=0;
	sel.p[0].p1 = -1;
	Fgetcset(f, startp);
	/* Execute machine once for each character */
	for(;;p++){
	doloop:
		c=Fgetc(f);
		if(p>=eof || c<0){
			switch(wrapped++){
			case 0:		/* let loop run one more click */
			case 2:
				break;
			case 1:		/* expired; wrap to beginning */
				if(sel.p[0].p1>=0 || eof!=INFINITY)
					goto Return;
				list[0][0].inst=list[1][0].inst=0;
				Fgetcset(f, (Posn)0);
				p=0;
				goto doloop;
			default:
				goto Return;
			}
		}else if(((wrapped && p>=startp) || sel.p[0].p1>0) && nnl==0)
			break;
		/* fast check for first char */
		if(startchar && nnl==0 && c!=startchar)
			continue;
		tl=list[flag];
		nl=list[flag^=1];
		nl->inst=0;
		ntl=nnl;
		nnl=0;
		if(sel.p[0].p1<0 && (!wrapped || p<startp || startp==eof)){
			/* Add first instruction to this list */
			if(++ntl >= NLIST)
	Overflow:
				error(Eoverflow);
			sempty.p[0].p1 = p;
			addinst(tl, startinst, &sempty);
		}
		/* Execute machine until this list is empty */
		for(tlp=tl; inst=tlp->inst; tlp++){	/* assignment = */
	Switchstmt:
			switch(inst->type){
			default:	/* regular character */
				if(inst->type==c){
	Addinst:
					if(++nnl >= NLIST)
						goto Overflow;
					addinst(nl, inst->next, &tlp->se);
				}
				break;
			case LBRA:
				if(inst->subid>=0)
					tlp->se.p[inst->subid].p1 = p;
				inst=inst->next;
				goto Switchstmt;
			case RBRA:
				if(inst->subid>=0)
					tlp->se.p[inst->subid].p2 = p;
				inst=inst->next;
				goto Switchstmt;
			case ANYNL:
				goto Addinst;
			case ANY:
				if(c!='\n')
					goto Addinst;
				break;
			case BOL:
				if(p==0){
	Step:
					inst=inst->next;
					goto Switchstmt;
				}
				if(f->getci>1){
					if(f->getcbuf[f->getci-2]=='\n')
						goto Step;
				}else{
					uchar c;
					if(Fchars(f, &c, p-1, p)==1 && c=='\n')
						goto Step;
				}
				break;
			case EOL:
				if(c=='\n')
					goto Step;
				break;
			case CCLASS:
				if(c>=0 && ((char *)inst->right)[c/8]&(1<<(c&07)))
					goto Addinst;
				break;
			case OR:
				/* evaluate right choice later */
				if(++ntl >= NLIST)
					goto Overflow;
				addinst(tlp, inst->right, &tlp->se);
				/* efficiency: advance and re-evaluate */
				inst=inst->left;
				goto Switchstmt;
			case END:	/* Match! */
				tlp->se.p[0].p2 = p;
				newmatch(&tlp->se);
				break;
			}
		}
	}
    Return:
	return sel.p[0].p1>=0;
}
newmatch(sp)
	register Rangeset *sp;
{
	register int i;

	if(sel.p[0].p1<0 || sp->p[0].p1<sel.p[0].p1 ||
	   (sp->p[0].p1==sel.p[0].p1 && sp->p[0].p2>sel.p[0].p2))
		for(i=0; i<NSUBEXP; i++)
			sel.p[i] = sp->p[i];
}
bexecute(f, startp)
	register File *f;
	Posn startp;
{
	register flag=0;
	register Inst *inst;
	register Ilist *tlp;
	register Posn p=startp;
	int nnl=0, ntl;
	register c;
	int wrapped=0;
	register startchar=bstartinst->type<OPERATOR? bstartinst->type : 0;

	list[0][0].inst=list[1][0].inst=0;
	sel.p[0].p1= -1;
	Fgetcset(f, startp);
	/* Execute machine once for each character, including terminal NUL */
	for(;;--p){
	doloop:
		if((c=Fbgetc(f))==-1){
			switch(wrapped++){
			case 0:		/* let loop run one more click */
			case 2:
				break;
			case 1:		/* expired; wrap to end */
				if(sel.p[0].p1>=0)
			case 3:
					goto Return;
				list[0][0].inst=list[1][0].inst=0;
				Fgetcset(f, f->nbytes);
				p=f->nbytes;
				goto doloop;
			default:
				goto Return;
			}
		}else if(((wrapped && p<=startp) || sel.p[0].p1>0) && nnl==0)
			break;
		/* fast check for first char */
		if(startchar && nnl==0 && c!=startchar)
			continue;
		tl=list[flag];
		nl=list[flag^=1];
		nl->inst=0;
		ntl=nnl;
		nnl=0;
		if(sel.p[0].p1<0 && (!wrapped || p>startp)){
			/* Add first instruction to this list */
			if(++ntl >= NLIST)
	Overflow:
				error(Eoverflow);
			/* the minus is so the optimizations in addinst work */
			sempty.p[0].p1 = -p;
			addinst(tl, bstartinst, &sempty);
		}
		/* Execute machine until this list is empty */
		for(tlp=tl; inst=tlp->inst; tlp++){	/* assignment = */
	Switchstmt:
			switch(inst->type){
			default:	/* regular character */
				if(inst->type==c){
	Addinst:
					if(++nnl >= NLIST)
						goto Overflow;
					addinst(nl, inst->next, &tlp->se);
				}
				break;
			case LBRA:
				if (inst->subid>=0){
					tlp->se.p[inst->subid].p1 = p;
				}
				inst=inst->next;
				goto Switchstmt;
			case RBRA:
				if (inst->subid>=0){
					tlp->se.p[inst->subid].p2 = p;
				}
				inst=inst->next;
				goto Switchstmt;
			case ANYNL:
				goto Addinst;
			case ANY:
				if(c!='\n')
					goto Addinst;
				break;
			case BOL:
				if(c=='\n' || p==0){
	Step:
					inst=inst->next;
					goto Switchstmt;
				}
				break;
			case EOL:
				if(f->getci<f->ngetc-1){
					if(f->getcbuf[f->getci+1]=='\n')
						goto Step;
				}else if(p<f->nbytes-1){
					uchar c;
					if(Fchars(f, &c, p+1, p+2)==1 && c=='\n')
						goto Step;
				}
				break;
			case CCLASS:
				if(((char *)inst->right)[c/8]&(1<<(c&07)))
					goto Addinst;
				break;
			case OR:
				/* evaluate right choice later */
				if(++ntl >= NLIST)
					goto Overflow;
				addinst(tlp, inst->right, &tlp->se);
				/* efficiency: advance and re-evaluate */
				inst=inst->left;
				goto Switchstmt;
			case END:	/* Match! */
				tlp->se.p[0].p1 = -tlp->se.p[0].p1; /* minus sign */
				tlp->se.p[0].p2 = p;
				bnewmatch(&tlp->se);
				break;
			}
		}
	}
    Return:
	return sel.p[0].p1>=0;
}
bnewmatch(sp)
        register Rangeset *sp;
{
        register i;
        if(sel.p[0].p1<0 || sp->p[0].p1>sel.p[0].p2 || (sp->p[0].p1==sel.p[0].p2 && sp->p[0].p2<sel.p[0].p1))
                for(i=0; i<NSUBEXP; i++){       /* note the reversal; p1<=p2 */
                        sel.p[i].p1=sp->p[i].p2;
                        sel.p[i].p2=sp->p[i].p1;
                }
}
