#include "jim.h"
#include "file.h"

#define	PATSIZ	128

char pattern[PATSIZ], *patp;
short this;
short searching;	/* as opposed to subbing; we need only the leftmost match */
short mustcompile;

/* These are in order of precedence */
#define	LPAR		0200
#define	RPAR		0201
#define	ALTERNATION	0202
#define	CATENATION	0203
#define	CLOSURE		0204
#define	ZEROORONE	0205
#define	ONECLOSURE	0206
#define	ANYC		0207
#define	BOL		0210
#define	EOL		0211
#define	CCLASS		0212
#define	ANYCORNL	0213
#define	NOP		0300
#define	EOFC		0377

#define	NNODE		200
#define	NSTACK		32
#define	NCCL		16
typedef struct Node{
	short op;
	struct Node *left;
	struct Node *right;
}Node;
typedef struct Stack{
	Node *b;	/* beginning */
	Node *e;	/* end */
}Stack;
Stack andstack[NSTACK], *andp;		/* operands */
Node *atorstack[NSTACK], **atorp;	/* operators */
Node node[NNODE];
Node *new();
Node *fstart, *bstart, *thisnode;
int forward;
int needand, needator;
int nccl;
char cclarea[NCCL*16];
char	bittab[] = {
	1,
	2,
	4,
	8,
	16,
	32,
	64,
	128
};
/*
 * Execution stuff
 */
#define	NLIST	256
#define	NMATCH	200
typedef struct List{
	Node *node;
	long startp;
}List;
List list[2][NLIST];
long startp;
List *tl, *nl;	/* This list, next list */
List *tlp;
typedef struct Match{
	long b, e;
}Match;
Match match[NMATCH+1];
int nmatch;
char *compilepat;
compile(s, save)
	char *s;
{
	if(strlen(s)>=PATSIZ)
		error("pattern too long\n", (char *)0);
	forward=1;
	startpat(compilepat=s);
	expr();
	fstart=andstack[0].b;
	optimize();
	forward=0;
	startpat(s);
	expr();
	bstart=andstack[0].b;
	optimize();
	if(save)
		strcpy(pattern, s);
	mustcompile=FALSE;
}
optimize(){
	register Node *inst, *target;
	for(inst=node; inst->op!=0; inst++){
		target=inst->right;
		while(target->op==NOP)
			target=target->right;
		inst->right=target;
	}
}
reerror(s)
	char *s;
{
	mustcompile=TRUE;
	dprintf("%s - ", compilepat);
	error("RE error:", s);
}
pushand(b, e)
	register Node *b, *e;
{
	if(andp >= &andstack[NSTACK])
		reerror("(compiler) operand overflow");
	andp->b=b;
	andp++->e=e;
}
Stack *
popand()
{
	if(andp <= andstack)
		reerror("operand stack underflow");
	return --andp;
}
pushator(o)
	register Node *o;
{
	if(atorp >= &atorstack[NSTACK])
		reerror("too many operators");
	popto(o->op);
	*atorp++=o;
}
popto(op)
	register op;
{
	register Node *p, *q;
	register Stack *a1, *a2;
	if(op==LPAR)
		return;
	while(atorp>atorstack && (atorp[-1])->op>=op){
		p= *--atorp;
		if(andp <= andstack)
			switch(p->op){
			case LPAR:
				reerror("unmatched left paren");
			case RPAR:
				reerror("unmatched right paren");
			}
		a2=popand();
		switch(p->op){
		case CATENATION:
			old(p);
			a1=popand();
			if(forward || a2->b->op==EOFC){
				a1->e->right=a2->b;
				pushand(a1->b, a2->e);
			}else{
				a2->e->right=a1->b;
				pushand(a2->b, a1->e);
			}
			break;
		case ALTERNATION:
			if(andp<=andstack)
				reerror("expected right operand for |");
			a1=popand();
			p->left=a1->b;
			p->right=a2->b;
			q=new(NOP);
			a1->e->right=q;
			a2->e->right=q;
			pushand(p, q);
			break;
		case CLOSURE:
			p->op=ALTERNATION;
			p->right=a2->e->right;
			p->left=a2->b;
			a2->e->right=p;
			pushand(p, p);
			break;
		case ONECLOSURE:
			p->op=ALTERNATION;
			p->right=a2->e->right;
			p->left=a2->b;
			a2->e->right=p;
			pushand(a2->b, p);
			break;
		case ZEROORONE:
			p->op=ALTERNATION;
			q=new(NOP);
			p->right=q;
			p->left=a2->b;
			a2->e->right=q;
			pushand(p, q);
			break;
		case LPAR:
			reerror("unmatched left paren");
		default:
			reerror("unknown operator in popto()");	
		}
	}
}
expr()
{
	register Node *n;
	for(;; advpat()){
		n=thisnode;
		switch(this){
		default:	/* token */
			if(needator)	/* concatenation first */
				pushator(new(CATENATION));
			pushand(n, n);
			needand=0;
			needator=1;
			break;
		case EOFC:
			popto(0);
			pushand(n, n);
			pushator(new(CATENATION));
			popto(0);
			return;
		case LPAR:
			if(needator)	/* catenation first */
				pushator(new(CATENATION));
			pushator(n);
			needator=0;
			needand=1;
			break;
		case RPAR:
			if(needand){
				if(andp <= andstack)
					reerror("unmatched right paren");
				reerror("empty parens");
			}
			old(n);
			popto(RPAR);
			if(atorp<=atorstack || atorp[-1]->op!=LPAR)
				reerror("unmatched right paren");
			old(*--atorp);
			needator=1;
			needand=0;
			break;
		case ONECLOSURE:
		case ZEROORONE:
		case CLOSURE:
			if(needand)
				reerror("operand expected for *, + or ?");
			pushator(n);
			popto(this);
			needand=0;
			needator=1;
			break;
		case ALTERNATION:
			if(needand)
				reerror("operand expected for |");
			pushator(n);
			needand=1;
			needator=0;
			break;
		}
			
	}
}
startpat(s)
	char *s;
{
	register Node *n;
	if(forward){
		for(n=node; n<&node[NNODE]; n++)
			old(n);
		nccl=0;
	}
	andp=andstack;
	atorp=atorstack;
	patp=s;
	needand=1;
	needator=0;
	advpat();
}
char *
cclblock(){
	register char *p;
	register i;
	if(nccl >= NCCL)
		reerror("too many character classes");
	for(p=cclarea+nccl*16, i=0; i<16; i++)
		*p++=0;
	return cclarea+(nccl++*16);
}
advpat()
{
	thisnode=new(0);
	switch(this= *patp++){
	case 0:
		this=EOFC;
		--patp;	/* in case we get called again */
		break;
	case '\\':
		if(*patp)
			switch(this= *patp++){
			case 'n':
				this='\n';
			}
		else	/* end of string */
			this='\\';
		break;
	case '.':
		this=ANYC;
		break;
	case '@':
		this=ANYCORNL;
		break;
	case '^':
		this=BOL;
		break;
	case '$':
		this=EOL;
		break;
	case '*':
		this=CLOSURE;
		break;
	case '+':
		this=ONECLOSURE;
		break;
	case '?':
		this=ZEROORONE;
		break;
	case '|':
		this=ALTERNATION;
		break;
	case '(':
		this=LPAR;
		break;
	case ')':
		this=RPAR;
		break;
	case '[':{
		register neg=0, cclcnt, c;
		register char *ccl, *cstart;
		this=CCLASS;
		if((c = *patp++) == '^') {
			neg = 1;
			c = *patp++;
		}
		ccl = cclblock();
		thisnode->left = (Node *)ccl;
		cstart = patp;
		do {
			if (c=='\0')
				reerror("character class syntax");
			if (c=='-' && patp>cstart && *patp!=']') {
				for (c = patp[-2]; c<*patp; c++)
					ccl[c>>3] |= bittab[c&07];
				patp++;
			}
			if(c=='\\' && *patp=='n'){
				patp++;
				c='\n';
			}		
			ccl[c>>3] |= bittab[c&07];
		} while((c = *patp++) != ']');
		if(neg) {
			for(cclcnt = 0; cclcnt < 16; cclcnt++)
				ccl[cclcnt] ^= -1;
			ccl[0] &= ~1; /* reject nulls and newlines */
			ccl[1] &= ~(1<<('\n'-8));
		}}
		break;
	}
	thisnode->op=this;
}
Node *
new(op)
{
	register Node *n;
	for(n=node; n->op; n++)
		if(n>=&node[NNODE-1])
			reerror("expression too complicated");
	n->op=op;
	n->right=0;
	n->left=0;
	return n;
}
old(n)
	Node *n;
{
	n->op=0;
}
addtolist(l, n, sp)
	register List *l;
	register Node *n;
	long sp;
{
	register List *p;
	for(p=l; p->node; p++){
		if(p>=&l[NLIST-5])
			reerror("list overflow");
		if( forward && p->node==n && p->startp<=sp ||
		   !forward && p->node==n && p->startp>=sp)
			return;
	}
	p->node=n;
	p->startp=sp;
	(++p)->node=0;
}
execute(f, c)
	File *f;
	int c;
{
	if(mustcompile)
		compile(pattern, TRUE);
	if(c=='/')
		return fexecute(f);
	else
		return bexecute(f);
}
fexecute(f)
	register File *f;
{
	register Node *n;
	register long startposn=f->selloc+f->nsel;
	long l=length(f);
	register flag;
	register char *s;
	int i;
	char *ends;
	int lastwasnl=FALSE;
	int startchar;
	int wrapped=FALSE;
	if(l==0)
		return 0;
	forward=1;
	startp=startposn;
	searching=TRUE;		/*BUG*/
	nmatch=0;
	match[0].b=0x7FFFFFFF;
	match[0].e=0x7FFFFFFF;
	startchar=0;
	if(fstart->op<0200)
		startchar=fstart->op;
	i=Fseek(f, startp)-1;
	if(i<0 || f->str.s[i]=='\n')
		lastwasnl=TRUE;
Restart:
	flag=1;
	list[0][0].node=0;
	list[1][0].node=0;
	for(;;){	/* exits in middle */
		if(startp>l || (i=Fseek(f, startp+1)-1)<0){
			lastwasnl=TRUE;
			startp=0;
			goto Restart;
		}
		ends=f->str.s+f->str.n;
		for(s=f->str.s+i; s<ends || startp==l; ){
			if(nl[0].node==0){	/* nl will become this list */
				if(wrapped || (searching && nmatch>0))
					goto Found;
				/* fast check for first char */
				if(startchar && *s!=startchar)
					goto Continue;
			}
			if(flag){
				nl=list[1];
				tl=list[0];
				flag=0;
			}else{
				nl=list[0];
				tl=list[1];
				flag=1;
			}
			nl[0].node=0;
			if(!wrapped && nmatch==0)
				addtolist(tl, fstart, startp);
			for(tlp=tl; tlp->node; tlp++){
				n=tlp->node;
		Again:		switch(n->op){
				default:	/* Literal char */
					if(*s==n->op)
		Addtonlist:
						addtolist(nl, n->right, tlp->startp);
					break;
				case CCLASS:
					if(((char *)(n->left))[*s>>3] & bittab[*s & 07])
						goto Addtonlist;
					break;
				case ANYC:
					if(*s && *s!='\n')
						goto Addtonlist;
					break;
				case ANYCORNL:
					if(*s)
						goto Addtonlist;
					break;
				case BOL:
					if(lastwasnl){
		Addtotlist:
						addtolist(tl, n->right, tlp->startp);
					}
					break;
				case EOL:
					if(*s=='\n' || *s=='\0')
						goto Addtotlist;
					break;
				case EOFC:
					newmatch(tlp->startp, startp);
					break;
				case ALTERNATION:
					addtolist(tl, n->right, tlp->startp);
					n=n->left;
					goto Again;
				}
			}
		Continue:
			if(++startp==startposn || (startposn==0 && startp>l))
				wrapped=TRUE;
			lastwasnl=FALSE;
			if(*s++=='\n')
				lastwasnl=TRUE;
		}
	}
 Found:
	if(nmatch==0)
		return FALSE;
	postcompress();
	loc1=match[0].b;
	loc2=match[0].e;
	return TRUE;
}
bexecute(f)
	register File *f;
{
	register Node *n;
	register long startposn=f->selloc;
	long l=length(f);
	register flag;
	register char *s;
	int i;
	char *ends;
	int lastwasnl=FALSE;
	int startchar;
	int wrapped=FALSE;
	if(l==0)
		return 0;
	forward=0;
	startp=startposn-1;
	searching=TRUE;		/*BUG*/
	nmatch=0;
	match[0].b=0x7FFFFFFF;
	match[0].e=0x7FFFFFFF;
	startchar=0;
	if(bstart->op<0200)
		startchar=bstart->op;
	/* This little dance is just to get lastwasnl right */
	i=Fseek(f, startp+1);
	if(i>0 && f->str.s[i]=='\n')
		lastwasnl=TRUE;
	i=Fseek(f, startp);
	if(i==0)
		lastwasnl=TRUE;
Restart:
	flag=1;
	list[0][0].node=0;
	list[1][0].node=0;
	for(;;){	/* exits in middle */
		if(startp<0 || (i=Fseek(f, startp))<0/* should never happen */){
			lastwasnl=TRUE;
			startp=l;
			goto Restart;
		}
		ends=f->str.s;
		for(s=f->str.s+i; s>=ends || startp==-1; ){
			if(startp==-1)
				s="";	/* ugly, oh so ugly */
			if(nl[0].node==0){	/* nl will become this list */
				if(wrapped || (searching && nmatch>0))
					goto Found;
				/* fast check for first char */
				if(startchar && *s!=startchar)
					goto Continue;
			}
			if(flag){
				nl=list[1];
				tl=list[0];
				flag=0;
			}else{
				nl=list[0];
				tl=list[1];
				flag=1;
			}
			nl[0].node=0;
			if(!wrapped && nmatch==0)
				addtolist(tl, bstart, startp);
			for(tlp=tl; tlp->node; tlp++){
				n=tlp->node;
		Again:		switch(n->op){
				default:	/* Literal char */
					if(*s==n->op)
		Addtonlist:
						addtolist(nl, n->right, tlp->startp);
					break;
				case CCLASS:
					if(((char *)(n->left))[*s>>3] & bittab[*s & 07])
						goto Addtonlist;
					break;
				case ANYC:
					if(*s && *s!='\n')
						goto Addtonlist;
					break;
				case ANYCORNL:
					if(*s)
						goto Addtonlist;
					break;
				case EOL:
					if(lastwasnl || startp==l){
		Addtotlist:
						addtolist(tl, n->right, tlp->startp);
					}
					break;
				case BOL:
					if(*s=='\n' || startp==-1)
						goto Addtotlist;
					break;
				case EOFC:
					newmatch(-(tlp->startp), -(startp));
					break;
				case ALTERNATION:
					addtolist(tl, n->right, tlp->startp);
					n=n->left;
					goto Again;
				}
			}
		Continue:
			if(--startp==startposn || (startposn==l && startp<-1))
				wrapped=TRUE;
			lastwasnl=FALSE;
			if(*s--=='\n')
				lastwasnl=TRUE;
		}
	}
 Found:
	if(nmatch==0)
		return FALSE;
	postcompress();
	loc2= -match[0].b;
	if(loc2<l)
		loc2++;
	loc1= -match[0].e;
	if(loc1<l)
		loc1++;
	return TRUE;
}
newmatch(b, e)
	register long b, e;
{
	register Match *m, *n;
	if(searching){	/* kill all threads in the machine with starts later than here */
		killlater(tlp, b);
		killlater(nl, b);
	}
	for(m=match; m->b<b; m++)
		;
	if(m->b==b){
		if(m->e<e)
			m->e=e;	/* easy */
		return;
	}
	if(nmatch>=NMATCH-1)
		reerror("output list overflow");
	for(n= &match[nmatch+1]; n>m; --n)
		n[0]=n[-1];
	m->b=b;
	m->e=e;
	nmatch++;
}
postcompress()
{
	register Match *m, *n;
	for(m=match; m<&match[nmatch]; m++){
		while(m->b==(m+1)->b){
			if(m->e < (m+1)->e)
				m->e=(m+1)->e;
			for(n=m+1; n<&match[nmatch]; n++)
				n[0]=n[1];
			--nmatch;
		}
		/* now there is just one match at m->b; delete overlaps */
		while(m->e>(m+1)->b || m->e>=(m+1)->e){
			for(n=m+1; n<&match[nmatch]; n++)
				n[0]=n[1];
			--nmatch;
		}
	}
}
killlater(lp, b)
	List *lp;
	register long b;
{
	register List *p, *q;
	if(forward){
		for(p=lp; p->node; p++)
			if(p->startp > b)	/* kill this thread */
				for(q=p; q->node; q++)
					q[0]=q[1];
	}else{
		for(p=lp; p->node; p++)
			if(p->startp < b)	/* kill this thread */
				for(q=p; q->node; q++)
					q[0]=q[1];
	}
}
