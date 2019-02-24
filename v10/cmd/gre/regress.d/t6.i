#include "alloc.h"
#include <libc.h>

char *
emalloc(unsigned long n)
{
	char *p;
	p=malloc((unsigned)n);
	if(p==0){
		warn("out of memory; exiting");
		exits("out of memory");
	}
	return p;
}
char *
erealloc(char *p, unsigned long n)
{
	p=realloc(p, (unsigned)n);
	if(p==0){
		warn("out of memory; exiting");
		exits("out of memory");
	}
	return p;
}
#include "alloc.h"
#include "word.h"
#include "store.h"
#include "comm.h"
#include <libc.h>

/*
 * Push constants
 */

ipushconst(Proc *proc)
{
	*proc->sp++=(SWord)*++proc->pc;
	return 1;
}

ipush_2(Proc *proc)
{
	*proc->sp++=-2;
	return 1;
}

ipush_1(Proc *proc)
{
	*proc->sp++=-1;
	return 1;
}

ipush0(Proc *proc)
{
	*proc->sp++=0;
	return 1;
}

ipush1(Proc *proc)
{
	*proc->sp++=1;
	return 1;
}

ipush2(Proc *proc)
{
	*proc->sp++=2;
	return 1;
}

ipush3(Proc *proc)
{
	*proc->sp++=3;
	return 1;
}

ipush4(Proc *proc)
{
	*proc->sp++=4;
	return 1;
}

ipush5(Proc *proc)
{
	*proc->sp++=5;
	return 1;
}

ipush6(Proc *proc)
{
	*proc->sp++=6;
	return 1;
}

ipush7(Proc *proc)
{
	*proc->sp++=7;
	return 1;
}

ipush8(Proc *proc)
{
	*proc->sp++=8;
	return 1;
}

ipush9(Proc *proc)
{
	*proc->sp++=9;
	return 1;
}

ipush10(Proc *proc)
{
	*proc->sp++=10;
	return 1;
}

/*
 * Binary operators
 */
ige(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]=proc->sp[-1]>=proc->sp[0];
	return 1;
}

ile(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]=proc->sp[-1]<=proc->sp[0];
	return 1;
}

ine(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]=proc->sp[-1]!=proc->sp[0];
	return 1;
}

ieq(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]=proc->sp[-1]==proc->sp[0];
	return 1;
}

igt(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]=proc->sp[-1]>proc->sp[0];
	return 1;
}

ilt(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]=proc->sp[-1]<proc->sp[0];
	return 1;
}

iadd(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]+=proc->sp[0];
	return 1;
}

isub(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]-=proc->sp[0];
	return 1;
}

imul(Proc *proc)
{
	long l0, l1, l;
	--proc->sp;
	l0=proc->sp[-1];
	l1=proc->sp[0];
	l=l0*l1;
	if(l1 && l/l1 != l0)
		rerror("product overflow");
	proc->sp[-1]=l;
	return 1;
}

idiv(Proc *proc)
{
	--proc->sp;
	if(proc->sp[0]==0)
		rerror("zero divide");
	proc->sp[-1]/=proc->sp[0];
	return 1;
}

imod(Proc *proc)
{
	--proc->sp;
	if(proc->sp[0]==0)
		rerror("zero modulo");
	proc->sp[-1]%=proc->sp[0];
	return 1;
}

iand(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]&=proc->sp[0];
	return 1;
}

ior(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]|=proc->sp[0];
	return 1;
}

ixor(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]^=proc->sp[0];
	return 1;
}

ilsh(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]<<=proc->sp[0];
	return 1;
}

irsh(Proc *proc)
{
	--proc->sp;
	proc->sp[-1]>>=proc->sp[0];
	return 1;
}

imax(Proc *proc)
{
	SWord l;
	l=*--proc->sp;
	if(l>proc->sp[-1])
		proc->sp[-1]=l;
	return 1;
}

/*
 * Unary operators
 */

ineg(Proc *proc)
{
	proc->sp[-1]=-proc->sp[-1];
	return 1;
}

inot(Proc *proc)
{
	proc->sp[-1]=~proc->sp[-1];
	return 1;
}

ilnot(Proc *proc)
{
	proc->sp[-1]=!proc->sp[-1];
	return 1;
}

iref(Proc *proc)
{
	Store *s=(Store *)*--proc->sp;
	*proc->sp++=s->ref-1;
	decref(&s);
	return 1;
}

ilen(Proc *proc)
{
	Store *s=(Store *)*--proc->sp;
	*proc->sp++=s->len;
	decref(&s);
	return 1;
}

/*
 * String comparison: put value of strcmp() on stack
 */

istrcmp(Proc *proc)
{
	int cmp;
	Store *s1, *s2;
	s1=(Store *)proc->sp[-2];
	s2=(Store *)proc->sp[-1];
	cmp=strcmp((char *)s1->data, (char *)s2->data);
	decref(&s1);
	decref(&s2);
	proc->sp--;
	proc->sp[-1]=cmp;
	return 1;
}

/*
 * Print
 */

iprintint(Proc *proc)
{
	pprint(proc, "%ld", *--proc->sp);
	return 1;
}

iprintnewline(Proc *proc)
{
	pprint(proc, "\n");
	return 1;
}

iprintblank(Proc *proc)
{
	pprint(proc, " ");
	return 1;
}

iprintunit(Proc *proc)
{
	pprint(proc, "(unit)");
	return 1;
}

iprintchar(Proc *proc)
{
	pprint(proc, "%c", *--proc->sp);
	return 1;
}

pprint(proc, fmt, a, b, c, d, e)
	Proc *proc;
	char *fmt;
{
	char buf[1024];
	long n;
	n=sprint(buf, fmt, a, b, c, d, e);
	if(proc->prbuf==0){
		proc->prbuf=emalloc(64+n);
		proc->maxprbuf=64+n;
		proc->nprbuf=0;
	}
	if(n+proc->nprbuf+1>proc->maxprbuf){
		proc->prbuf=erealloc(proc->prbuf, proc->maxprbuf+64+n);
		proc->maxprbuf+=64+n;
	}
	strcpy(proc->prbuf+proc->nprbuf, buf);
	proc->nprbuf+=n;
}
/*
 * Stack management
 */

ipop(Proc *proc)
{
	--proc->sp;
	return 1;
}

ipopptr(Proc *proc)
{
	decref((Store **)(proc->sp-1));
	--proc->sp;
	return 1;
}

idup(Proc *proc)
{
	proc->sp++;
	proc->sp[-1]=proc->sp[-2];
	return 1;
}

idupptr(Proc *proc)
{
	proc->sp++;
	proc->sp[-1]=proc->sp[-2];
	((Store *)(proc->sp[-1]))->ref++;
	return 1;
}
#include "node.h"
#include "symbol.h"
#include "alloc.h"
#include "word.h"
#include "store.h"
#include "comm.h"
#include "inst.h"
#include <libc.h>

#define	FNS
#include "lib.h"
#undef	FNS

#define	C	0x40000000
#define	I	0x20000000
#define	F	0x10000000
#define	M(x)	((x)&~(C|I|F))

long call0[]={	/* plain function, 0 arguments */
	I+Ipushfp,	C+0,	F,	I+Iret,	C+0*WS,	I+Idone,	0
};
long call1[]={	/* plain function, 1 argument */
	I+Ipushfp,	C+0,	F,	I+Iret,	C+1*WS,	I+Idone,	0
};
long call2[]={	/* plain function, 2 arguments */
	I+Ipushfp,	C+0,	F,	I+Iret,	C+2*WS,	I+Idone,	0
};
long call3[]={	/* plain function, 3 arguments */
	I+Ipushfp,	C+0,	F,	I+Iret,	C+3*WS,	I+Idone,	0
};
long call4[]={	/* plain function, 4 arguments */
	I+Ipushfp,	C+0,	F,	I+Iret,	C+4*WS,	I+Idone,	0
};
long call5[]={	/* plain function, 5 arguments */
	I+Ipushfp,	C+0,	F,	I+Iret,	C+5*WS,	I+Idone,	0
};
long call2_0[]={/* two-step function, 0 arguments */
	I+Ipushfp,	C+0,	F+0,	F+1,	I+Iret,	C+0*WS,	I+Idone,	0
};

struct{
	char	*name;
	int	(*fn[3])();
	int	nargs;
	long	*template;
}bltin[]={
#include "lib.h"
	0,	{0,	0,	0},	0,	0,
};

bltinlookup(char *s)
{
	int i;
	for(i=0; bltin[i].name; i++)
		if(strcmp(s, bltin[i].name)==0)
			return i;
	error("%s not a builtin", s);
	return -1;
}

long
bltinval(char *name, Node *t)
{
	int i, nargs, len;
	long *template, *p;
	Store *s;
	SWord *d;
	if(t->o.t!=TProg)
		error("builtin %s not a function", name);
	i=bltinlookup(name);
	nargs=bltin[i].nargs;
	if(nargs!=length(t->l))	/* necessary but not sufficient */
		error("wrong #args to builtin %s: %d (should be %d)", name, length(t->l), nargs);
	template=bltin[i].template;
	p=template;
	for(len=0; *p; p++)
		len++;
	s=(Store *)emalloc(SHSZ+len*LWS);
	s->ref=1;
	s->type=Sprog;
	s->sbits=0;
	s->len=len;
	d=s->data;
	for(p=template; *p; p++)
		if(*p&C)
			*d++=(SWord)M(*p);
		else if(*p&I)
			*d++=(SWord)insttab[M(*p)].fp;
		else if(*p&F)
			*d++=(SWord)bltin[i].fn[M(*p)];
	return (long)s;
}

Store *
mk(type, len)
{
	Store *s;
	if(type==Sstruct)
		len++;
	s=(Store *)emalloc(SHSZ+len*LWS);
	s->ref=1;
	s->type=type;
	if(type==Sstruct){
		s->sbits=1;
		s->data[0]=0;
	}else
		s->sbits=0;
	s->len=len;
	return s;
}
#include "node.h"
#include "symbol.h"
#include "alloc.h"
#include "ydefs.h"
#include "word.h"
#include "store.h"
#include "comm.h"
#include "inst.h"
#include "errjmp.h"
#include <libc.h>

long		resultloc;
long		returnloc;
Node		*formals;
long		autooffset;
extern int	bflag;
extern int	cflag;
extern int	nscope;
extern Node	arychartype;

compile(n)	/* called from parser only */
	Node *n;
{
	extern long autooffset;
	Errjmp x;
	n=constants(n);
	if(cflag){
		fileline();
		fprint(2, "constants:\n");
		dump(n, 0);
	}
	errsave(x);
	if(errmark()){
		autooffset=0;
		freenode(n);
		errrest(x);
		errjmp();
	}
	istart();
	gen(n, 0);
	freenode(n);
	errrest(x);
}

gen(Node *n, int retain)
{
	int i;
	if(n==0)
		return;
	switch(n->t){
	case NArrayref:
		arygen(n->l, n->r, 0, 0L);
		if(!retain)
			popgen(n->l->o.s->val->type->r);
		return;
	case NBecome:
		if(n->l->t==NCall && !bflag){
			callgen(n->l, Ibecome);
			return;
		}
		gen(n->l, 1);
		n=n->r;
		if(n->o.t==TID)
			n=typeoftid(n);
		switch(n->o.t){
		case TInt:
		case TChar:
			emit(Istoreauto);
			emitconst(-LWS*(3+length(formals)));
			break;
		case TArray:
		case TChan:
		case TProg:
		case TStruct:
			emit(Istoreptrauto);
			emitconst(-LWS*(3+length(formals)));
			break;
		case TUnit:
			break;
		default:
			panic("can't compile %t become", n->o.t);
		}
		scopedecrefgen();
		trlrgen();
		return;
	case NBegin:
		callgen(n->l, Ibegin);
		return;
	case NCall:
		callgen(n, Icall);
		if(!retain)
			popgen(etypeoft(n->l)->r);
		return;
	case NDecl:
	case NDeclsc:
		declare(n, 0, 0, 1);
		return;
	case NExpr:
		switch(n->o.i){
		case GE:
			i=Ige;
		Binop:
			gen(n->l, 1);
			gen(n->r, 1);
			if(eqtype(etypeof(n->l), &arychartype)){
				emit(Istrcmp);
				constgen(0L);
			}
			emit(i);
		Popit:
			if(!retain)
				emit(Ipop);
			return;
		case LE:
			i=Ile;
			goto Binop;
		case NE:
			i=Ine;
			goto Binop;
		case EQ:
			i=Ieq;
			goto Binop;
		case '>':
			i=Igt;
			goto Binop;
		case '<':
			i=Ilt;
			goto Binop;
		case '+':
			i=Iadd;
			goto Binop;
		case '-':
			i=Isub;
			goto Binop;
		case '*':
			i=Imul;
			goto Binop;
		case '/':
			i=Idiv;
			goto Binop;
		case '%':
			i=Imod;
			goto Binop;
		case '&':
			i=Iand;
			goto Binop;
		case '|':
			i=Ior;
			goto Binop;
		case '^':
			i=Ixor;
			goto Binop;
		case LSH:
			i=Ilsh;
			goto Binop;
		case RSH:
			i=Irsh;
			goto Binop;
		case ANDAND:
			condgen(n->l, n->r, Ijmptrue, Ijmpfalse, 0L, 1L, retain);
			return;
		case OROR:
			condgen(n->l, n->r, Ijmpfalse, Ijmptrue, 1L, 0L, retain);
			return;
		case PRINT:
			gen(n->l, 1);
			printgen(n->l);
			emit(Isprint);
			if(!retain)
				emit(Iprint);
			return;
		case SND:
			gen(n->l, 1);
			constgen((long)Cissnd);
			emit(Icommset1);
			emit(Icommcln1);
			gen(n->r, 1);
			if(isptrtype(etypeoft(n->l)->r))
				emit(Isndptr);
			else
				emit(Isnd);
			if(!retain)
				popgen(etypeof(n));
			return;
		case RCV:
			gen(n->l, 1);
			constgen(0L);	/* not Cissnd */
			emit(Icommset1);
			emit(Icommcln1);
			return;
		case '=':
			gen(n->r, 1);
			if(retain)
				dupgen(etypeof(n->r), 1);
			lgen(n->l);
			return;
		case LEN:
			gen(n->l, 1);
			emit(Ilen);
			goto Popit;
		case REF:
			if(isptrtype(etypeof(n->l))){
				gen(n->l, 1);
				emit(Iref);
			}else
				constgen(1L);
			goto Popit;
		case DEF:
			if(retain && n->l->t==NID && isinttype(etypeof(n->l))){
				constgen(1L);
				return;
			}
			/*
			 * don't really need to call lgen1, which will uniquify our
			 * array for us, but it does no harm, and it's easy.
			 */
			lgen1(n->l, Idefauto, Idef, Idefary);
			goto Popit;
		case UMINUS:
			gen(n->l, 1);
			emit(Ineg);
			goto Popit;
		case '~':
			gen(n->l, 1);
			emit(Inot);
			goto Popit;
		case '!':
			gen(n->l, 1);
			emit(Ilnot);
			goto Popit;
		case INC:
			lgen1(n->l, Iincauto, Iinc, Iincary);
			goto Popit;
		case DEC:
			lgen1(n->l, Idecauto, Idec, Idecary);
			goto Popit;
		default:
			panic("can't compile %e expression", n->o.i);
		}

	case NExprlist:
		/*
		 * This is an arg or element list; first is pushed last
		 */
		gen(n->r, 1);
		gen(n->l, 1);
		return;
	case NID:
		if(!retain)
			return;
		switch(typeof(n)->o.t){
		case TInt:
		case TChar:
			if(n->o.s->val->isauto){
				emit(Ipushauto);
				emitconst(n->o.s->val->store.off);
			}else{
				emit(Ipush);
				emitconst((long)&n->o.s->val->store.l);
			}
			return;
		case TProg:
		case TArray:
		case TChan:
		case TStruct:
			if(n->o.s->val->isauto){
				emit(Ipushptrauto);
				emitconst(n->o.s->val->store.off);
			}else{
				emit(Ipushptr);
				emitconst((long)&n->o.s->val->store.l);
			}
			return;
		case TUnit:
			if(retain)
				constgen(0L);
			return;
		case TType:
			lerror(n, "attempt to evaluate type variable %m", n);
		default:
			panic("can't compile type %t", n->o.s->val->type->o.t);
		}
	case NIf:
		ifgen(n);
		return;
	case NList:
		gen(n->l, 0);
		gen(n->r, 0);
		return;
	case NLoop:
		loopgen(n);
		return;
	case NMk:
		mkgen(n->l, n->r);
		return;
	case NNum:
		if(retain)
			constgen(n->o.l);
		return;
	case NProg:
		if(retain)
			proggen(n->l, n->r);
		return;
	case NResult:
		gen(n->l, 1);
		emit(Ijmp);
		emitconst((long)(resultloc-here()-1)*WS);
		return;
	case NScope:
		pushscope();
		if(nscope==1){
			int nauto;
			autooffset=0;
			emit(Ipushfp);
			nauto=here();
			emitconst(0L);
			gen(n->l, 0);
			patch((int)nauto, autooffset);
		}else
			gen(n->l, 0);
		scopedecrefgen();
		popscope();
		return;
	case NSelect:
		selgen(n->l);
		return;
	case NSmash:{
		Value *vl, *vr;
		vl=n->l->o.s->val;
		vr=n->r->o.s->val;
		if(vr->type->o.t==TType){
			freenode(vl->type);
			vl->type=dupnode(vr->type);
			return;
		}
		gen(n->r, 1);
		/*
		 * Free old values; tricky: push as int, pop as ptr
		 */
		if(isptrtype(vl->type)){
			if(vl->isauto){
				emit(Ipushauto);
				emitconst(vl->store.off);
			}else{
				emit(Ipush);
				emitconst((long)&vl->store.l);
			}
			emit(Ipopptr);
		}
		if(vl->isauto){
			emit(Istoreauto);
			emitconst(vl->store.l);
			return;
		}
		emit(Istore);
		emitconst((long)&vl->store.l);
		return;
	}
	case NString:
		if(retain){
			Store *s;
			s=(Store *)emalloc(SHSZ+strlen(n->o.c)+1);
			strcpy((char *)(s->data), n->o.c);
			s->ref=1;
			s->len=strlen(n->o.c);
			s->type=Sarychar;
			emit(Ipushdata);
			emitconst((long)s);
		}
		return;
	case NStructref:
		arygen(n->l, n->r, 1, n->o.l);
		return;
	case NSwitch:
		switchgen(n->l, n->r);
		return;
	case NUnit:
		if(retain)
			constgen(0L);
		return;
	case NVal:
		valgen(n->l);
		if(!retain)
			popgen(n->o.n);
		return;
	}
	panic("can't compile node %n", n->t);
	return;
}

arygen(Node *a, Node *i, int isstr, long off)
{
	int ptr, ischar;
	if(isstr){
		ptr=isptrtype(i);
		constgen(off);
		ischar=0;
	}else{
		Node *t=etypeoft(a)->r;
		ptr=isptrtype(t);
		gen(i, 1);
		ischar=t->o.t==TChar;
	}
	if(a->t!=NID){
		gen(a, 1);
		emit(ptr? Ipusharyptrexpr :
			(ischar? Ipusharycharexpr :Ipusharyexpr));
	}else if(a->o.s->val->isauto){
		emit(ptr? Ipusharyptrauto :
			(ischar? Ipusharycharauto :Ipusharyauto));
		emitconst(a->o.s->val->store.off);
	}else{
		emit(ptr? Ipusharyptr :
			(ischar? Ipusharychar :Ipushary));
		emitconst((long)&a->o.s->val->store.l);
	}
}

lgen(Node *n)
{
	switch(n->t){
	case NID:
		switch(typeof(n)->o.t){
		case TChar:
			if(n->o.s->val->isauto){
				emit(Istorecharauto);
				emitconst(n->o.s->val->store.off);
				return;
			}
			emit(Istorechar);
			emitconst((long)&n->o.s->val->store.l);
			return;
		case TInt:
		case TUnit:
			if(n->o.s->val->isauto){
				emit(Istoreauto);
				emitconst(n->o.s->val->store.off);
				return;
			}
			emit(Istore);
			emitconst((long)&n->o.s->val->store.l);
			return;
		case TArray:
		case TChan:
		case TProg:
		case TStruct:
			if(n->o.s->val->isauto){
				emit(Istoreptrauto);
				emitconst(n->o.s->val->store.off);
				return;
			}
			emit(Istoreptr);
			emitconst((long)&n->o.s->val->store.l);
			return;

		default:
			panic("lgen: ID type %t", n->o.s->val->type->o.t);
			return;
		}
	case NArrayref:
		gen(n->r, 1);
		goto Genref;
	case NStructref:
		constgen(n->o.l);
	Genref:
		lgen1(n->l, Ipushuniqauto, Ipushuniq, Ipushuniqary);
		emit(Istoreary);
		return;
	default:
		panic("lgen: lvalue node %n", n->t);
	}
}

/*
 * n is a compound object about to be assigned into
 */
lgen1(Node *n, int Iauto, int Ivar, int Iary)
{
	switch(n->t){
	case NID:
		if(n->o.s->val->isauto){
			emit(Iauto);
			emitconst(n->o.s->val->store.off);
			return;
		}
		emit(Ivar);
		emitconst((long)&n->o.s->val->store.l);
		return;
	case NArrayref:
		gen(n->r, 1);
		goto Genref;
	case NStructref:
		constgen(n->o.l);
	Genref:
		lgen1(n->l, Ipushuniqauto, Ipushuniq, Ipushuniqary);
		emit(Iary);
		return;
	default:
		panic("lgen1: lvalue node %n", n->t);
	}
}

ifgen(Node *n)
{
	int loc1, loc2;
	gen(n->o.n, 1);
	emit(Ijmpfalse);
	loc1=here();
	emit(0);
	gen(n->l, 0);
	if(n->r==0){
		patch(loc1, (long)(here()-loc1-1)*WS);
		return;
	}
	emit(Ijmp);
	loc2=here();
	emit(0);
	patch(loc1, (long)(here()-loc1-1)*WS);
	gen(n->r, 0);
	patch(loc2, (long)(here()-loc2-1)*WS);
	return;
}

valgen(Node *n)
{
	int loc1, loc2;
	int orl;
	emit(Ijmp);
	loc1=here();
	emitconst(0L);
	orl=resultloc;
	resultloc=here();
	emit(Ijmp);
	loc2=here();
	emitconst(0L);
	patch(loc1, (long)(here()-loc1-1)*WS);
	gen(n, 1);
	emit(Ivalnoresult);
	patch(loc2, (long)(here()-loc2-1)*WS);
	resultloc=orl;
}

loopgen(Node *n)
{
	int loc0, loc1, loc2;
	if(n->o.i){	/* enter loop at top, so jump to body */
		emit(Ijmp);
		loc0=here();
		emit(0);
	}
	gen(n->r->l, 0);	/* left expr */
	if(n->r->r){		/* jump to condition */
		emit(Ijmp);
		loc1=here();
		emit(0);
	}
	if(n->o.i)
		patch(loc0, (here()-loc0-1)*LWS);
	loc2=here();
	gen(n->l, 0);		/* body */
	gen(n->r->o.n, 0);	/* right expr */
	if(n->r->r){
		patch(loc1, (here()-loc1-1)*LWS);
		gen(n->r->r, 1);
		emit(Ijmptrue);
	}else
		emit(Ijmp);
	emitconst((loc2-here()-1)*LWS);
}

condgen(Node *l, Node *r, Inst i1, Inst i2, long t1, long t2, int retain)
{
	int loc1, loc2, loc3;
	gen(l, 1);
	emit(i1);
	loc1=here();
	emit(0);
	loc2=here();
	if(retain)
		constgen(t1);
	emit(Ijmp);
	loc3=here();
	emit(0);
	patch(loc1, (long)(here()-loc1-1)*WS);
	gen(r, 1);
	emit(i2);
	emitconst((long)(loc2-here()-1)*WS);
	if(retain)
		constgen(t2);
	patch(loc3, (long)(here()-loc3-1)*WS);
}

callgen(Node *n, int callinst)
{
	Node *pt;
	pt=etypeof(n->l);
	/*
	 * Space for result
	 */
	constgen(0L);
	/*
	 * Args
	 */
	gen(n->r, 1);
	/*
	 * Call
	 */
	emit(Ipushconst);
	if(n->l->t==NID)
		emitconst((long)n->l->o.s->name);
	else{
		char buf[128];
		char *p;
		sprint(buf, "prog(){call on line %d}", n->line);
		p=emalloc((unsigned long)strlen(buf)+1);
		strcpy(p, buf);
		emitconst((long)p);
	}
	gen(n->l, 1);
	switch(callinst){
	case Icall:
		emit(Icall);
		return;
	case Ibegin:
		constgen(LWS*(1+1+length(pt->l)));	/* result+procname+args */
		emit(Ibegin);
		return;
	case Ibecome:
		constgen(LWS*(1+1+length(pt->l)));	/* result+procname+args */
		scopedecrefgen();
		fdecrefgen(formals, -3L*WS);
		emit(Ibecome);
		if(formals)
			emitconst(length(formals)*LWS);
		else
			emitconst(0L);
		return;
	}
	panic("callgen");
}

selgen(Node *n)
{
	int tbl, i;
	long l;
	int ends[200];
	selchangen(n);
	l=length(n);
	constgen(l);
	emit(Icommset);
	emit(Icommcln);
	if(l>(sizeof ends/sizeof ends[0]))
		panic("selgen table too small");
	tbl=here();
	emitspace(l);
	i=0;
	seltblgen(n, tbl, ends, &i);
	for(i=0; i<l; i++)
		patch(ends[i], (long)(here()-ends[i]-1)*WS);
}

selchangen(Node *n)
{
	long flags;
	if(n->t==NList){
		selchangen(n->l);
		selchangen(n->r);
		return;
	}
	if(n->t!=NCase)
		panic("selchangen");
	n=n->l->l;
	if(n->o.t=='=')
		n=n->r;		/* n is now RCV or SND */
	flags=0;
	if(n->o.t==SND)
		flags|=Cissnd;
	n=n->l;			/* n is now channel */
	if(n->t==NArraycom){
		flags|=Cisary;
		n=n->l;
	}else if(etypeoft(n)->o.t==TArray)
		flags|=Cisary;
	gen(n, 1);
	constgen(flags);
}

seltblgen(Node *n, int tbl, int *ends, int *ip)
{
	Node *c, *s, *l, *t;
	if(n->t==NList){
		/* chans are eval'ed from the top, so table is backwards */
		seltblgen(n->r, tbl, ends, ip);
		seltblgen(n->l, tbl, ends, ip);
		return;
	}
	if(n->t!=NCase)
		panic("seltblgen");
	if(n->l->t==NList)
		error("sorry, empty cases not implemented");
	patch(tbl+*ip, (long)(here()-tbl)*WS);
	c=n->l->l;	/* communication */
	s=n->r;		/* statement */
	l=0;
	if(c->o.t=='='){
		l=c->l;	/* lvalue */
		c=c->r;
	}
	if(c->o.t==SND){
		gen(c->r, 1);
		if(isptrtype(etypeoft(c->l)->r))
			emit(Isndptr);
		else
			emit(Isnd);
	}
	c=c->l;	/* channel expression */
	/*
	 * The value is still on the stack; save it or toss it
	 */
	if(l)
		lgen(l);
	else if(c->t==NArraycom){
		t=etypeoft(c->l)->r;
		if(t->o.t==TID)
			t=typeoftid(t);
		popgen(t->r);
	}else
		popgen(etypeoft(c)->r);
	if(c->t==NArraycom){	/* save array index */
		if(c->r)
			lgen(c->r);
		else
			emit(Ipop);
	}
	gen(s, 0);
	emit(Ijmp);
	ends[*ip]=here();
	(*ip)++;
	emitconst(0L);
}

switchgen(Node *s, Node *e)
{
	int isptr, out;
	isptr=isptrtype(etypeof(e));
	gen(e, 1);
	emit(Ijmp);
	emitconst(2*LWS);
	emit(Ijmp);	/* each case jumps to here to get out */
	out=here();
	emitconst(0L);
	switchgen1(s, isptr, out-1);
	/* pop leftover value if no case matched */
	if(isptr)
		emit(Ipopptr);
	else
		emit(Ipop);
	patch(out, (here()-out-1)*LWS);
}

switchgen1(Node *s, int isptr, int out)
{
	Node *e;
	int loc;
	if(s->t==NList){
		switchgen1(s->l, isptr, out);
		switchgen1(s->r, isptr, out);
		return;
	}
	if(s->t!=NCase)
		panic("switchgen1");
	if(s->r==0)
		error("sorry; can't fold cases together yet");
	if(s->l->t==NDefault)
		loc=-1;
	else{
		e=s->l->l;
		if(isptr){	/* string */
			emit(Idupptr);
			gen(e, 1);
			emit(Istrcmp);
			constgen(0L);
		}else{
			emit(Idup);
			gen(e, 1);
		}
		emit(Ieq);
		emit(Ijmpfalse);
		loc=here();
		emitconst(0L);
	}
	if(isptr)
		emit(Ipopptr);
	else
		emit(Ipop);
	gen(s->r, 0);
	emit(Ijmp);
	emitconst((out-here()-1)*LWS);
	if(loc!=-1)
		patch(loc, (here()-loc-1)*LWS);
}

popgen(Node *t)
{
	if(isptrtype(t))
		emit(Ipopptr);
	else if(isinttype(t) || t->o.t==TUnit)
		emit(Ipop);
	else
		panic("popgen %t\n", t->o.t);
}

genfreeauto(Symbol *s)
{
	if(!s->val->isauto)
		panic("genfreeauto");
	if(isptrtype(s->val->type)){
		emit(Idecrefauto);
		emitconst(s->val->store.off);
	}
}

printgen(Node *n)
{
	Node *t;
	if(n==0)
		return;
	if(n->t==NExprlist){
		printgen(n->l);
		printgen(n->r);
		return;
	}
	t=etypeoft(n);
	switch(t->o.t){
	case TArray:
	case TChan:
	case TProg:
	case TStruct:
		emit(Iprintary);
		break;
	case TChar:
		emit(Iprintchar);
		break;
	case TInt:
		emit(Iprintint);
		break;
	case TUnit:
		emit(Iprintunit);
		break;
	default:
		panic("printgen: bad type %t", t->o.t);
	}
}

proggen(Node *t, Node *n)
{
	int or;
	Node *of;
	Errjmp s;
	Store *p;
	long len, loc;
	long nauto, oao;
	extern int (*prog[])();
	oao=autooffset;
	or=returnloc;
	of=formals;
	autooffset=0;
	returnloc=0;
	formals=t->l;
	errsave(s);
	if(errmark()){
		returnloc=or;
		formals=of;
		autooffset=oao;
		errrest(s);
		errjmp();
	}
	loc=here();
	pushscope();
	dclformals(t->l);
	autooffset=0;
	emit(Ipushfp);
	nauto=here();
	emitconst(0L);
	gen(n, 0);
	trlrgen();
	patch((int)nauto, autooffset);
	popscope();
	errrest(s);
	autooffset=oao;
	returnloc=or;
	formals=of;
	len=here()-loc+1;
	p=(Store *)emalloc(SHSZ+len*LWS);
	memcpy((char *)(p->data), (char *)(prog+loc), len*LWS);
	p->ref=1;
	p->len=len;
	p->type=Sprog;
	setprog(loc);
	emit(Ipushdata);
	emitconst((long)p);
}

trlrgen()
{
	if(returnloc){
		emit(Ijmp);
		emitconst((long)(returnloc-here()-1)*WS);
		return;
	}
	returnloc=here();
	fdecrefgen(formals, -3L*WS);
	emit(Iret);
	if(formals)
		emitconst(length(formals)*LWS);
	else
		emitconst(0L);
}

fdecrefgen(Node *types, long offset)
{
	if(types==0)
		return 0;
	if(types->t==NList){
		offset=fdecrefgen(types->l, offset);
		return fdecrefgen(types->r, offset);
	}
	if(types->t!=NFormal)
		panic("fdecrefgen");
	types=types->r;
	if(isptrtype(types)){
		emit(Idecrefauto);
		emitconst(offset);
	}
	return offset-WS;
}

dupgen(Node *t, int n)
{
	while(n--)
		emit(isptrtype(t)? Idupptr : Idup);
}

mkgen(Node *t, Node *v)
{
	switch(t->o.t){
	case TChar:
	case TInt:
	case TUnit:
		if(v)
			gen(v, 1);
		else
			constgen(0L);
		return;
	case TID:
		mkgen(typeoftid(t), v);
		return;
	case TChan:
		if(v)
			gen(v, 1);
		else{
			constgen((long)(sizeof(Chan)-sizeof(Store)));
			mallocgen(t);
		}
		return;
	case TArray:
		if(v==0){
			gen(t->l, 1);
			mallocgen(t);
			return;
		}
		gen(v, 1);
		if(v->t!=NExprlist && eqtype(t, etypeof(v)))
			return;
		if(v->t==NString)
			constgen((long)strlen(v->o.c));
		else
			constgen((long)length(v));
		emit(Idup);
		if(t->l)
			gen(t->l, 1);
		else
			constgen(0L);
		emit(Imax);
		mallocgen(t);
		if(t->r->o.t==TChar){
			if(v->t==NString)
				emit(Imemcpychar);
			else
				emit(Imemcpycharint);
		}else
			emit(Imemcpy);
		return;
	case TProg:
		if(v==0){
			v=new(NProg, dupnode(t), (Node *)0, (Node *)0);
			gen(v, 1);
			freenode(v);
			return;
		}
		gen(v, 1);
		return;
	case TStruct:
		if(v==0){
			mallocgen(t);
			return;
		}
		gen(v, 1);
		if(v->t!=NExprlist && eqtype(t, etypeof(v)))
			return;
		constgen((long)length(v));
		mallocgen(t);
		emit(Imemcpystruct);
		return;		
	default:
		panic("mkgen: bad type %t", t->o.t);
	}
}

mallocgen(Node *t)
{
	switch(t->o.t){
	case TArray:
		t=t->r;
		if(t->o.t==TID)
			t=typeoftid(t);
		if(isptrtype(t)){
			constgen((long)Saryptr);
			emit(Imalloc);
		}else if(t->o.t==TInt || t->o.t==TUnit){
			constgen((long)Saryint);
			emit(Imalloc);
		}else if(t->o.t==TChar)
			emit(Imallocarychar);
		else
			panic("mallocgen array of %t", t->o.t);
		return;
	case TStruct:{
		int pos=0;
		long bits=0;
		t=t->l;
		elembitsgen(t, &pos, &bits);
		if(pos)
			constgen(bits);
		constgen((long)length(t));
		emit(Imallocstruct);
		return;
	}
	case TChan:
		constgen((long)Schan);
		emit(Imalloc);
		return;
	}
	panic("mallocgen of %t", t->o.t);
}

elembitsgen(Node *t, int *pos, long *bits)
{
	int i;
	if(t->t==NList){
		elembitsgen(t->l, pos, bits);
		elembitsgen(t->r, pos, bits);
		return;
	}
	if(t->t!=NElem)
		panic("elembitsgen %n", t->t);
	for(i=length(t); --i>=0; ){
		if(*pos==BPW){
			constgen(*bits);
			*pos=0;
			*bits=0;
		}
		if(isptrtype(t->r))
			*bits|=1L<<*pos;
		(*pos)++;
	}
}

constgen(long l)
{
	if(l<-2 || l>10){
		emit(Ipushconst);
		emitconst(l);
		return;
	};
	switch((int)l){
	case -2:
		emit(Ipush_2);
		break;
	case -1:
		emit(Ipush_1);
		break;
	case 0:
		emit(Ipush0);
		break;
	case 1:
		emit(Ipush1);
		break;
	case 2:
		emit(Ipush2);
		break;
	case 3:
		emit(Ipush3);
		break;
	case 4:
		emit(Ipush4);
		break;
	case 5:
		emit(Ipush5);
		break;
	case 6:
		emit(Ipush6);
		break;
	case 7:
		emit(Ipush7);
		break;
	case 8:
		emit(Ipush8);
		break;
	case 9:
		emit(Ipush9);
		break;
	case 10:
		emit(Ipush10);
		break;
	default:
		panic("constgen");
	}
}

printable(Node *n)
{
	if(n==0)
		return 0;
	switch(n->t){
	case NExpr:
		return n->o.t!='=';
	case NArrayref:
	case NCall:
	case NID:
	case NMk:
	case NNum:
	case NProg:
	case NString:
	case NStructref:
	case NUnit:
	case NVal:
		return 1;
	}
	return 0;
}
#include "alloc.h"
#include "node.h"
#include "symbol.h"
#include "ydefs.h"
#include "word.h"
#include "store.h"
#include <libc.h>

Node		*doconst();
extern int	Cflag;

Node *
constants(Node *n)
{
	if(n==0)
		return 0;
	if(Cflag)
		return n;
	switch(n->t){
	case NArrayref:
		if(isconst(n))
			return doconst(n);
		break;
	case NArraycom:
		break;
	case NBecome:
		break;
	case NBegin:
		break;
	case NCall:
		break;
	case NCase:
		break;
	case NDecl:
		n->r=constants(n->r);
		n->o.n=constants(n->o.n);
		declare(n, 0, 0, 0);
		return n;
	case NDeclsc:
		break;
	case NDefault:
		return n;
	case NElem:
		n->r=constants(n->r);
		return n;
	case NExpr:
		switch(n->o.i){
		case GE:
		case LE:
		case NE:
		case EQ:
		case '>':
		case '<':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '&':
		case '|':
		case '^':
		case ANDAND:
		case OROR:
		case LSH:
		case RSH:
			if(isconst(n->l) && isconst(n->r))
				return doconst(n);
			break;
		case DEF:
		case REF:
		case LEN:
		case UMINUS:
		case '~':
		case '!':
			if(isconst(n->l))
				return doconst(n);
			break;
		case PRINT:
		case RCV:
		case SND:
		case INC:
		case DEC:
			break;
		case '=':
			break;
		default:
			fprint(2, "can't const expression %e\n", n->o.i);
			return n;
		}
		break;
	case NExprlist:
		break;
	case NFormal:
		n->r=constants(n->r);
		return n;
	case NLabel:
		break;
	case NID:
		if(isconst(n))
			return doconst(n);
		break;
	case NIf:
		n->l=constants(n->l);
		n->r=constants(n->r);
		n->o.n=constants(n->o.n);
		if(isconst(n->o.n)){
			Node *m;
			gen(n->o.n, 1);
			execute();
			if(topofstack()){
				m=n->l;
				n->l=0;
			}else{
				m=n->r;
				n->r=0;
			}
			freenode(n);
			return m;
		}
		return n;
	case NList:
		break;
	case NLoop:
		break;
	case NLoopexpr:
		n->o.n=constants(n->o.n);
		break;
	case NMk:
		break;
	case NNum:
		return n;
	case NProg:
		pushscope();
		dclformals(n->l->l);
		n->r=constants(n->r);
		popscope();
		return n;
	case NResult:
		break;
	case NScope:
		pushscope();
		n->l=constants(n->l);
		popscope();
		return n;
	case NSelect:
		break;
	case NSmash:
		return n;
	case NString:
		return n;
	case NSwitch:
		break;
	case NStructref:
		if(isconst(n))
			return (n);
		break;
	case NType:
		break;
	case NUnit:
		break;
	case NVal:
		if(isconst(n->l))
			return doconst(n);
		break;
	default:
		fprint(2, "can't const node %n\n", n->t);
		return n;
	}
	n->l=constants(n->l);
	n->r=constants(n->r);
	return n;
}

isconst(Node *n)
{
	if(n==0)
		return 1;
	switch(n->t){
	case NArrayref:
		return isconst(n->l) && isconst(n->r);
	case NCall:
		return 0;
	case NExpr:
		switch(n->o.i){
		case GE:
		case LE:
		case NE:
		case EQ:
		case '>':
		case '<':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '&':
		case '|':
		case '^':
		case ANDAND:
		case OROR:
		case LSH:
		case RSH:
			return isconst(n->l) && isconst(n->r);
		case DEF:
		case LEN:
		case UMINUS:
		case '~':
		case '!':
			return isconst(n->l);
		case REF:
		case '=':
		case RCV:
		case SND:
		case INC:
		case DEC:
			return 0;
		}
		fprint(2, "can't isconst expression %e", n->o.i);
		return 0;
	case NID:
		return n->o.s->val->scope==0 && (n->o.s->val->stclass&SCconst);
	case NIf:
		return isconst(n->o.n) && isconst(n->l) && isconst(n->r);
	case NList:
		return 0;
	case NLoop:
		return 0;
	case NNum:
		return 1;
	case NResult:
		return isconst(n->l);
	case NScope:
		return isconst(n->l);
	case NString:
		return 1;
	case NStructref:
		return isconst(n->l);
	case NVal:
		return isconst(n->l);
	case NUnit:
		return 1;
	}
	fprint(2, "can't isconst node %n\n", n->t);
	return 0;
}

Node *
doconst(Node *n)
{
	Node *t;
	if(n->t==NNum || n->t==NString || n->t==NUnit)
		return n;	/* already const */
	t=etypeoft(n);
	switch(t->o.t){
	case TChar:
	case TInt:
		gen(n, 1);
		freenode(n);
		execute();
		return new(NNum, (Node *)0, (Node *)0, (Node *)topofstack());
	case TUnit:
		return new(NUnit, (Node *)0, (Node *)0, (Node *)0);
	case TArray:
		if(t->r->o.t==TChar){
			Store *s;
			char *c;
			gen(n, 1);
			freenode(n);
			execute();
			s=(Store *)topofstack();
			c=emalloc(s->len+1);
			strncpy(c, (char *)s->data, (int)s->len);
			return newc(NString, (Node *)0, (Node *)0, c);
		}
		return n;
	}
	return n;
}
#include "alloc.h"
#include "word.h"
#include "store.h"
#include "comm.h"
#include <libc.h>

extern	int	pflag;

/*
 * Jumps
 */

ijmp(Proc *proc)
{
	SWord l;
	l=(SWord)*++proc->pc;
	proc->pc+=l/WS;
	return 1;
}

ijmpfalse(Proc *proc)
{
	SWord l;
	l=(SWord)*++proc->pc;
	if(*--proc->sp==0)
		proc->pc+=l/WS;
	return 1;
}

ijmptrue(Proc *proc)
{
	SWord l;
	l=(SWord)*++proc->pc;
	if(*--proc->sp!=0)
		proc->pc+=l/WS;
	return 1;
}

ivalnoresult(Proc *proc)
{
	rerror("val produces no result");
	return 0;
}

/*
 * Progs
 *
 *   Layout of a stack frame
 *
 *	sp:
 *		automatics
 *	fp:	old fp
 *		old pc
 *		symbol
 *		arg1
 *		arg2
 *		...
 *		result
 */

iret(Proc *proc)
{
	SWord nargs;
	nargs=(SWord)(proc->pc[1]);
	proc->sp=(SWord *)proc->fp+1;
	proc->fp=(SWord *)*--proc->sp;
	proc->pc=(int (**)())*--proc->sp;
	proc->sp-=(sizeof(char *)+nargs)/WS;
	if(proc->pc==0){
		if(pflag)
			fprint(2, "%d halts\n", proc->procnum);
		halt(proc);
		return 0;
	}
	return 1;
}

ibecome(Proc *proc)
{
	int nargs;
	int (**newpc)();
	SWord oldfp, oldpc, *oldresultaddr, *newresultaddr;
	Store *s;
	nargs=*--proc->sp/LWS;
	nargs+=2;	/* includes result and sym; add pc, fp */
	s=(Store *)*--proc->sp;
	if(--(s->ref)==0)
		rpanic("ibecome ref==0");
	newpc=((int (**)())s->data);
	oldfp=proc->fp[0];
	oldpc=proc->fp[-1];
	*proc->sp++=oldpc;
	*proc->sp++=oldfp;
	oldresultaddr=proc->fp-3-(long)(*++proc->pc)/LWS;
	newresultaddr=proc->sp-nargs;
	memcpy((char *)oldresultaddr, (char *)newresultaddr, LWS*nargs);
	/* args in place.  do the call by hand, jmp to pushfp */
	proc->sp=oldresultaddr+(nargs-2);
	*proc->sp++=oldpc;
	proc->fp=(SWord *)oldfp;
	proc->pc=newpc-1;
	return 1;
}

ipushfp(Proc *proc)
{
	int nauto;
	*proc->sp=(SWord)proc->fp;
	proc->fp=proc->sp++;
	nauto=((SWord)*++proc->pc)/WS;
	while(nauto--)
		*proc->sp++=0;
	if(proc->sp>=&proc->stack[NSTACK])
		rerror("stack overflow");
	return 1;
}

icall(Proc *proc)
{
	int (**newpc)();
	Store *s;
	s=(Store *)*--proc->sp;
	if(--(s->ref)==0)
		rpanic("icall ref==0");
	newpc=((int (**)())s->data);
	*proc->sp++=(SWord)proc->pc;
	proc->pc=newpc-1;
	return 1;
}
#include "node.h"
#include "symbol.h"
#include "alloc.h"
#include "ydefs.h"
#include "word.h"
#include "store.h"
#include <libc.h>

extern int	nscope;

declare(Node *n, int stclass, int dotypchk, int docomp)
{
	extern int iflag;
	if(n==0)
		return;
	if(n->t==NList){
		declare(n->l, stclass, dotypchk, docomp);
		declare(n->r, stclass, dotypchk, docomp);
		return;
	}
	if(n->t==NDeclsc){
		declare(n->l, n->o.i, dotypchk, docomp);
		return;
	}
	if(dotypchk)
		type(n->o.n, 0);
	if(n->r==0){
		if(n->o.n==0)
			panic("declare: no type");
		if(n->o.n->t==NMk && n->o.n->l==0)
			lerror(n, "can't derive type in declaration");
		n->r=dupnode(etypeof(n->o.n));
	}
	if(dotypchk){
		type(n->r, 0);
		if(n->o.n){
			/*
			 * Make it a mk
			 */
			if(n->o.n->t!=NMk)
				n->o.n=new(NMk, (Node *)0, n->o.n, (Node *)0);
			/*
			 * Default type for mk
			 */
			if(n->o.n->l==0)
				n->o.n->l=dupnode(n->r);
			else if(!compattype(n->r, n->o.n->l))
				lerror(n, "type clash in declaration (%t %t)\n",
					n->r->o.t, etypeof(n->o.n)->o.t);
			mkcheck(n->o.n->l, n->o.n->r);
		}
	}
	if(docomp && n->o.n){
		if(dotypchk)	/* top level declaration */
			n->o.n=constants(n->o.n);
		gen(n->o.n, 1);
		dupgen(n->r, length(n->l)-1);
	}else
		docomp=0;
	dcl(n->l, n->r, stclass, n->o.n, docomp);
	if(n->o.n && docomp && nscope==0){
		if(iflag)
			idump();
		execute();
	}
}

dcl(id, typ, stclass, val, docomp)
	Node *id, *typ, *val;
{
	if(id->t==NList){
		dcl(id->l, typ, stclass, val, docomp);
		dcl(id->r, typ, stclass, val, docomp);
		return;
	}
	if(typ->o.t==TID && typ->l->o.s->val->type->o.t!=TType)
		error("%m not a type", typ->l);
	if(id->t!=NID)
		panic("dcl not ID");
	pushval(id->o.s, dupnode(typ));
	if(stclass&SCbltin)
		id->o.s->val->store.l=bltinval(id->o.s->name, typ);
	if(docomp)
		lgen(id);
	id->o.s->val->stclass=stclass;
}

/*
 * To compile this
 * 	rec {
 * 		x : chan of T = f(x,y);
 * 		y : chan of T = g(x,y);
 * 	};
 * convert it to this
 * 	x : chan of T = mk();
 * 	y : chan of T = mk();
 * 	x1 : chan of T = f(x,y);
 * 	y1 : chan of T = g(x,y);
 * 	x <- x1;
 * 	y <- y1;
 *	toss x1, y1;
 * where the operator x <- x1 means copy the representation of x1 into x.
 *
 *	rec type T: struct of { t:T; };
 *
 * is handled similarly.
 */

Node *
op1(Node *n)
{
	Node *m;
	if(n->t==NDeclsc){
		m=op1(n->l);
		return newi(NDeclsc, m, (Node *)0, n->o.i);
	}
	if(n->r==0){
		if(n->o.n && (n->o.n->t==NProg || (n->o.n->t==NMk && n->o.n->l)))
			n->r=dupnode(n->o.n->l);
		else			
			lerror(n, "can't deduce type for rec decl");
	}else if(n->r->o.t==TType){
		m=newi(NType, (Node *)0, (Node *)0, n->r->l->o.t);
		m=new(NDecl, dupnode(n->l), m, (Node *)0);
		return m;
	}
	m=new(NMk, dupnode(n->r), (Node *)0, (Node *)0);
	m=new(NDecl, dupnode(n->l), dupnode(n->r), m);
	return m;
}

Node *
op2(Node *n)
{
	Node *m;
	char s[Namesize+2];
	if(n->t==NDeclsc){
		m=op2(n->l);
		return newi(NDeclsc, m, (Node *)0, n->o.i);
	}
	if(n->l->t==NList)
		error("no identifier lists in rec's, please");
	strcpy(s+1, n->l->o.s->name);
	s[0]='*';
	m=new(NDecl, idnode(lookup(s, ID)), dupnode(n->r), dupnode(n->o.n));
	return m;
}

Node *
op3(Node *n)
{
	Node *m;
	char s[Namesize+2];
	if(n->t==NDeclsc)
		return op3(n->l);
	if(n->l->t==NList)
		error("no lists in rec's, please");
	strcpy(s+1, n->l->o.s->name);
	s[0]='*';
	m=new(NSmash, idnode(lookup(s+1, ID)), idnode(lookup(s, ID)), (Node *)0);
	return m;
}

Node *
rewr(Node *n, Node *(*f)())
{
	if(n->t==NList)
		return new(NList, rewr(n->l, f), rewr(n->r, f), (Node *)0);
	return (*f)(n);
}

recrewrite(Node *n)
{
	Node *n1, *n2, *n3;
	n1=rewr(n->l, op1);
	n2=rewr(n->l, op2);
	n3=rewr(n->l, op3);
	freenode(n->l);
	n->t=NList;
	n->r=n3;
	n->l=new(NList, n1, n2, (Node *)0);
	ndump(n);
}

/*
 *
 * To compile this
 *
 *	prog(a:int){
 *		begin prog(b:int){ f(a, b); }(b);
 *	}
 *
 * convert it to this
 *
 *	prog(a:int){
 *		begin prog(b:int, a:int){ f(a, b); }(b, a);
 *	}
 *
 */

Node 	*begf;
Node	*bega;
int	fscope;
int	progerr;

proglocals(Node *n)
{
	progerr=1;
	pushscope();
	fscope=nscope;
	begf=n->l->l;
	bega=0;
	dclformals(begf);
	progid(n->r);
	popscope();
}

begrewrite(Node *n)
{
	progerr=0;
	pushscope();
	fscope=nscope;
	begf=n->l->l->l;
	bega=n->r;
	dclformals(begf);
	progid(n->l->r);
	popscope();
	n->l->l->l=begf;
	n->r=bega;
}

addformal(Node *n)
{
	Node *nf;
	if(!alreadyformal(n, begf)){
		nf=new(NFormal, dupnode(n), dupnode(n->o.s->val->type), (Node *)0);
		if(begf)
			begf=new(NList, begf, nf, (Node *)0);
		else
			begf=nf;
		nf=dupnode(n);
		if(bega)
			bega=new(NExprlist, bega, nf, (Node *)0);
		else
			bega=nf;
	}		
}

alreadyformal(Node *n, Node *f)
{
	if(f==0)
		return 0;
	if(f->t==NList)
		return alreadyformal(n, f->l) || alreadyformal(n, f->r);
	return strcmp(n->o.s->name, f->l->o.s->name)==0;
}

progid(Node *n)
{
	if(n==0)
		return;
	switch(n->t){
	case NArrayref:
	case NArraycom:
	case NBecome:
	case NBegin:
	case NCall:
	case NCase:
		break;
	case NDecl:
		progid(n->r);
		progid(n->o.n);
		declare(n, 0, 0, 0);
		return;
	case NDeclsc:
	case NDefault:
		break;
	case NElem:
		return;
	case NExpr:
	case NExprlist:
	case NFormal:
		break;
	case NID:
		if(n->o.s->val)
		if(0<n->o.s->val->scope && n->o.s->val->scope<fscope){
			if(progerr)
				lerror(n, "%m not in an accessible scope", n);
			addformal(n);
		}
		return;
	case NLabel:
	case NList:
	case NLoop:
		break;
	case NLoopexpr:
		progid(n->o.n);
		break;
	case NIf:
		progid(n->o.n);
		break;
	case NMk:
		break;
	case NNum:
		return;
	case NProg:
		pushscope();
		dclformals(n->l->l);
		progid(n->r);
		popscope();
		return;
	case NResult:
		break;
	case NScope:
		pushscope();
		progid(n->l);
		popscope();
		return;
	case NSelect:
		break;
	case NSmash:
		return;	/* ?? */
	case NString:
		return;
	case NSwitch:
	case NStructref:
		break;
	case NType:
		break;
	case NUnit:
		return;
	case NVal:
		break;
	default:
		fprint(2, "can't progid node %n\n", n->t);
		return;
	}
	progid(n->l);
	progid(n->r);
}

#include "nodenames.h"
#include "typenames.h"
#include "errjmp.h"
#include "node.h"
#include "symbol.h"
#include "ydefs.h"
#include <libc.h>

lerror(Node *n, char *s, a, b, c, d, e, f)
{
	lfileline(n->line);
	fprint(2, s, a, b, c, d, e, f);
	if(s[strlen(s)-1]!='\n')
		fprint(2, "\n");
	errflush();
	errjmp();
}

error(char *s, a, b, c, d, e, f)
{
	fileline();
	fprint(2, s, a, b, c, d, e, f);
	if(s[strlen(s)-1]!='\n')
		fprint(2, "\n");
	errflush();
	errjmp();
}

rerror(char *s, a, b, c, d, e, f)
{
	fileline();
	fprint(2, s, a, b, c, d, e, f);
	fprint(2, "\n");
	processes(0);
	errflush();
	errjmp();
}

warn(char *s, a, b, c, d, e, f)
{
	fileline();
	fprint(2, "warning: ");
	fprint(2, s, a, b, c, d, e, f);
	fprint(2, "\n");
}

panic(char *s, a, b, c, d, e, f)
{
	fileline();
	fprint(2, "internal error: ");
	fprint(2, s, a, b, c, d, e, f);
	fprint(2, "\n");
	abort();
}

rpanic(char *s, a, b, c, d, e, f)
{
	fileline();
	processes(0);
	fprint(2, "internal error: ");
	fprint(2, s, a, b, c, d, e, f);
	fprint(2, "\n");
	abort();
}

bconv(int *o, int f1, int f2)
{
	extern int printcol;
	while(printcol<*o-8)
		strconv("\t", f1, f2);
	strconv("        "+(8-(*o-printcol)), f1, f2);
	return sizeof(int);
}

nconv(int *o, int f1, int f2)
{
	if(*o<0 || sizeof(Ntypename)/sizeof(Ntypename[0])<=*o)
		strconv("mystery node", f1, f2);
	else
		strconv(Ntypename[*o], f1, f2);
	return sizeof(int);
}

tconv(int *o, int f1, int f2)
{
	if(*o<0 || sizeof(Ttypename)/sizeof(Ttypename[0])<=*o)
		strconv("mystery type", f1, f2);
	else
		strconv(Ttypename[*o], f1, f2);
	return sizeof(int);
}

char	bufx[128][10];
int	bufno=9;

char *
prbuf(){
	if(++bufno==10)
		bufno=0;
	return bufx[bufno];
}

econv(int *o, int f1, int f2)
{
	char *buf=prbuf();
	char *x;
	int t=*o;
	if(t<128 && strchr("+-*/%|&^~?!><=", t))
		sprint(buf, "%c", t);
	else{
		switch(t){
		case GE:
			x=">=";
			break;
		case LE:
			x="<=";
			break;
		case NE:
			x="!=";
			break;
		case EQ:
			x="==";
			break;
		case ANDAND:
			x="&&";
			break;
		case OROR:
			x="||";
			break;
		case REF:
			x="ref";
			break;
		case LEN:
			x="len";
			break;
		case UMINUS:
			x="unary -";
			break;
		case RCV:
			x="rcv";
			break;
		case SND:
			x="send";
			break;
		case LSH:
			x="<<";
			break;
		case RSH:
			x=">>";
			break;
		case DEC:
			x="--";
			break;
		case INC:
			x="++";
			break;
		default:
			x="mystery expression";
			break;
		}
		strcpy(buf, x);
	}
	strconv(buf, f1, f2);
	return sizeof(int);
}

mconv(int *o, int f1, int f2)
{
	char *buf=prbuf();
	Node *n=(Node *)*o;
	switch(n->t){
