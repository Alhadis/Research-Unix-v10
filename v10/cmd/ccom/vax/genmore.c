#include "gencode.h"
char *jeq = "jeql";
char *jne = "jneq";
char *jgt = "jgtr";
char *jge = "jgeq";
char *jlt = "jlss";
char *jle = "jleq";
char *jugt = "jgtru";
char *juge = "jgequ";
char *jult = "jlssu";
char *jule = "jlequ";
mnod *gimmemnod();

char prbuf[10*1024];
extern int bothdebug, nosharp;
#define NBUF 256	/* must fit in an unsigned char */
char bufs[NBUF][BUF], *buf;
char *bufend = (char *)bufs + sizeof(bufs);
pr(fmt, list)
char *fmt; long list;
{	char *n;
	char *sprintxl();
	nosharp = !bothdebug;
	prptr = sprintxl(n=prptr, fmt, &list);
	if(prptr > prbuf + sizeof(prbuf))
		cerror("prbuf overflow");
	nosharp = 0;
}

outpr()
{
	*prptr = 0;
	printbuf(prbuf, prptr-prbuf);
}

ret
checksize(p, s, regmask)
mnod *p;
ret s;
{	ret t;
	if(p->type != Tdouble)
		return(s);
	regmask |= s.regmask;
	t = allocreg(p, regmask);
	return(t);
}

gimmetemp(n)
{
	return(freetemp(n)/8 - maxboff/SZCHAR);
}

commutes(op)
{
	switch(op) {
	default:
		cerror("unexpected op in commutes");
	case Plus: case Mul: case Or: case Xor: case And:
		return(1);
	case Minus: case Div: case Mod:
		return(0);
	}
}

strshift(s, n)
char *s;
{	int i, j;
		i = strlen(s);
	if(n > 0)
		for(j = i; j >= 0; j--)
			s[j + n] = s[j];
	else
		for(j = -n; j <= i; j++)
			s[j + n] = s[j];
}

ret
tostack()
{	ret s;
	sprintx(buf, "-(sp)");
	done(s, 0, 0);
}

char *
genjbc(n)
{
	if(n == EQ)
		return("jbc");
	if(n == NE)
		return("jbs");
	cerror("impossible jmp mask type");
	return("jweird");
}

char *
genjmp(n)
{
	switch(n) {
	default: cerror("impossible jmp type"); return("jweird");
	case EQ: return(jeq);
	case NE: return(jne);
	case GT: return(jgt);
	case GE: return(jge);
	case LT: return(jlt);
	case LE: return(jle);
	case UGT: return(jugt);
	case UGE: return(juge);
	case ULT: return(jult);
	case ULE: return(jule);
	}
}

pow2(n)
{	int i;
	if(n & (n-1))
		return(-1);
	for(i = 0; i < 32; i++)	/* good old vax */
		if(n & (1 << i))
			return(i);
	return(-1);	/* can't happen */
}

ret
indirit(s)
ret s;
{
	if(s.flag & ISREG) {
		strcat(str(s), ")");
		strshift(str(s), 1);
		str(s)[0] = '(';
		return(s);
	}
	if(s.flag & CANINDIR) {
		strshift(str(s), 1);
		str(s)[0] = '*';
		return(s);
	}
	if(str(s)[0] == '(') {	/* (r3)[r11] */
		strshift(str(s), 1);
		str(s)[0] = '*';
		return(s);
	}
	if(str(s)[0] == '$') {	/* an icon for structure returns */
		strshift(str(s), -1);
		return(s);
	}
	debugpr("# FAIL ");
	s.flag = FAIL;
	return(s);
}

ret	/* unseal the seals for Stasg */
addrsimp(a, b)
ret a, b;
{	extern char *index();
	char *p;
	if(str(b)[0] == '(') {	/* posit register */
reg:
		strshift(str(b), -1);
		*index(str(b), ')') = 0;
		return(b);
	}
	if(str(a)[0] == '(') {	/* posit register */
		b = a;
		goto reg;
	}
	if(str(b)[0] == '_') {	/* posit name of struct */
name:
		pr("#\tmoval\t%s,r0\n", str(b));
		b.regmask = 1;
		b.flag = ISREG|SCRATCH;
		strcpy(str(b), "r0");		/* arrrrrgh */
		return(b);
	}
	if(str(a)[0] == '_') {
		b = a;
		goto name;
	}
	if(str(b)[0] == '*') {
unstar:
		p = index(str(b), '+');
		if(!p)
			p = index(str(b), '-');
		if(p && (*(p+1) == 0 || *(p+1) == '('))
			goto bad;
		strshift(str(b), -1);
		return(b);
	}
	if(str(a)[0] == '*') {
		b = a;
		goto unstar;
	}
	if(str(b)[0] == 'L') {
label:
		strshift(str(b), 1);
		str(b)[0] = '&';
		return(b);
	}
	if(str(a)[0] == 'L') {
		b = a;
		goto label;
	}
bad:
	cerror("(addrsimp) rewrite struct asg");
}
		
ret
simpler(a, b)	/* returns b (as dest) preferentially */
ret a, b;
{
	if(b.flag & ISREG)
		return(b);
	if(a.flag & ISREG)
		return(a);
	if(b.flag & SCRATCH)
		return(b);
	if(a.flag & SCRATCH)
		return(a);
	if(!(b.flag & INDEX))
		return(b);
	if(!(a.flag & INDEX))
		return(a);
	/* disallow *p++ = *q++ but not p[i]=q[i]*/
	if(!index(str(b), '-') && !index(str(b), '+'))
		return(b);
	if(!index(str(a), '-') && !index(str(a), '+'))
		return(a);
	b.flag |= USED;
	return(b);
}

hasqnode(p)
mnod *p;
{
	if(p->op == Asg && p->left->op == Qnode) {
		return(1);
	}
	if(1) {
		return(0);
	}
	switch(p->op) {
	default:
		cerror("hasqnode\n");
	case Andeq: case And: case Cmp: case Comop: case Decr: 
	case Diveq: case Div: case Xoreq: case Xor:
	case Incr: case Lseq: case Ls: case Minuseq: case Minus:
	case Modeq: case Mod: case Muleq: case Mul: case Oreq:
	case Or: case Pluseq: case Plus: case Rseq: case Rs:
	case Asg: case Cm:
	case Call: case Stcall:
	case Stasg:
		return(hasqnode(p->left) || hasqnode(p->right));
	case Compl: case Conv: case Genbr: case Genlab:
	case Genubr: case Star: case Addr: case Ucall:
	case Uminus: case Ustcall: case Init: case Funarg:
	case Fld:case Starg:
		return(hasqnode(p->left));
	case Auto: case Reg: case Name: case Param: case Icon:
	case Snode: case Rnode:
		return(0);
	case Qnode:
		return(1);
	}
}

mnod *
copytree(p)
NODE *p;
{	mnod *a, *b, *c;
	c = gimmemnod();
	switch(p->in.op) {
	case ASG AND: case AND: case CMP: case COMOP: case DECR: 
	case ASG DIV: case DIV: case ASG ER: case ER:
	case INCR: case ASG LS: case LS: case ASG MINUS: case MINUS:
	case ASG MOD: case MOD: case ASG MUL: case MUL: case ASG OR:
	case OR: case ASG PLUS: case PLUS: case ASG RS: case RS:
	case ASSIGN: case CM:
		a = copytree(p->in.left);
		b = copytree(p->in.right);
		c->left = a;
		c->right = b;
		break;
	case CALL: case STCALL:
		a = copytree(p->in.left);
		b = copytree(p->in.right);
		c->argsize = p->stn.argsize;
		c->left = a;
		c->right = b;
		break;
	case STASG:
		a = copytree(p->in.left);
		b = copytree(p->in.right);
		c->stsize = p->stn.stsize;
		c->left = a;
		c->right = b;
		break;
	case COMPL: case CONV: case GENBR: case GENLAB:
	case GENUBR: case STAR: case UNARY AND: case UNARY CALL:
	case UNARY MINUS: case UNARY STCALL: case INIT: case FUNARG:
		a = copytree(p->in.left);
		c->label = p->bn.label;	/* should be separated out */
		c->lop = p->bn.lop;
		c->left = a;
		break;
	case FLD:case STARG:
		a = copytree(p->in.left);
		c->left = a;
		c->rval = p->tn.rval;
		c->stsize = p->stn.stsize;
		break;
	case VAUTO: case REG: case NAME: case VPARAM: case ICON:
	case SNODE: case RNODE: case QNODE:
		c->name = p->tn.name;
		c->lval = p->tn.lval;
		c->rval = p->tn.rval;
		break;
	}
	switch(p->in.op) {
	default:
		cerror("unk op in copytree");
	case ASG AND:	c->op = Andeq; break;
	case AND:	c->op = And; break;
	case CMP:	c->op = Cmp; break;
	case COMOP:	c->op = Comop; break;
	case DECR:	c->op = Decr; break;
	case ASG DIV:	c->op = Diveq; break;
	case DIV:	c->op = Div; break;
	case ASG ER:	c->op = Xoreq; break;
	case ER:	c->op = Xor; break;
	case INCR:	c->op = Incr; break;
	case ASG LS:	c->op = Lseq; break;
	case LS:	c->op = Ls; break;
	case ASG MINUS:	c->op = Minuseq; break;
	case MINUS:	c->op = Minus; break;
	case ASG MOD:	c->op = Modeq; break;
	case MOD:	c->op = Mod; break;
	case ASG MUL:	c->op = Muleq; break;
	case MUL:	c->op = Mul; break;
	case ASG OR:	c->op = Oreq; break;
	case OR:	c->op = Or; break;
	case ASG PLUS:	c->op = Pluseq; break;
	case PLUS:	c->op = Plus; break;
	case ASG RS:	c->op = Rseq; break;
	case RS:	c->op = Rs; break;
	case ASSIGN:	c->op = Asg; break;
	case CM:	c->op = Cm; break;
	case CALL:	c->op = Call; break;
	case STCALL:	c->op = Stcall; break;
	case STASG:	c->op = Stasg; break;
	case COMPL:	c->op = Compl; break;
	case CONV:	c->op = Conv; break;
	case GENBR:	c->op = Genbr; break;
	case GENLAB:	c->op = Genlab; break;
	case GENUBR:	c->op = Genubr; break;
	case STAR:	c->op = Star; break;
	case UNARY AND:	c->op = Addr; break;
	case UNARY CALL:	c->op = Ucall; break;
	case UNARY MINUS:	c->op = Uminus; break;
	case UNARY STCALL:	c->op = Ustcall; break;
	case INIT:	c->op = Init; break;
	case FUNARG:	c->op = Funarg; break;
	case FLD:	c->op = Fld; break;
	case STARG:	c->op = Starg; break;
	case VAUTO:	c->op = Auto; break;
	case REG:	c->op = Reg; break;
	case NAME:	c->op = Name; break;
	case VPARAM:	c->op = Param; break;
	case ICON:	c->op = Icon; break;
	case SNODE:	c->op = Snode; break;
	case QNODE:	c->op = Qnode; break;
	case RNODE:	c->op = Rnode; break;
	}
	switch(p->in.type) {
	default:
		cerror("incomprehensible type");
	case TCHAR:	c->type = Tchar; break;
	case TUCHAR:	c->type = Tuchar; break;
	case TSHORT:	c->type = Tshort; break;
	case TUSHORT:	c->type = Tushort; break;
	case TINT:	c->type = Tint; break;
	case TUNSIGNED:	c->type = Tuint; break;
	case TLONG:	c->type = Tlong; break;
	case TULONG:	c->type = Tulong; break;
	case TPOINT:	c->type = Tpoint; break;
	case TFLOAT:	c->type = Tfloat; break;
	case TDOUBLE:	c->type = Tdouble; break;
	case TSTRUCT:	c->type = Tstruct; break;
	case TVOID:	c->type = Tvoid; break;
	}
}

/* does p contain an asgop or Incr (or could it be evaluated twice) */
sideffects(p)
mnod *p;
{
	switch(p->op) {
	case Andeq: case Decr: case Diveq: case Xoreq: case Incr:
	case Lseq:  case Minuseq:case Modeq: case Muleq: case Oreq:
	case Pluseq: case Rseq: case Call: case Stcall: case Ucall:
		return(1);
	case And: case Cmp: case Comop: case Div: case Xor:
	case Ls: case Minus: case Mod: case Mul:
	case Or: case Plus: case Rs: case Asg: case Cm: case Stasg:
		return(sideffects(p->left) || sideffects(p->right));
	case Compl: case Conv: case Genbr: case Genlab:
	case Genubr: case Star: case Addr:
	case Uminus: case Ustcall: case Init: case Funarg:
	case Fld: case Starg:
		return(sideffects(p->left));
	case Auto: case Reg: case Name: case Param: case Icon:
	case Snode: case Rnode: case Qnode:
		return(0);
	default:
		cerror("unk mnod in sideffects");
	}
}
/* to make up for pointless 9th edition name change */
char *
index(s, c)
char *s;
{
	return(strchr(s, c));
}

char *
rindex(s, c)
{
	return(strrchr(s, c));
}

overr()
{
	cerror("expression too complicated");
}
