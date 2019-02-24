#include "expr.pri"
#include "gram.h"
#include "symbol.h"
#include "format.pub"
#include "frame.pri"
#include "phrase.pub"
#include "symtab.pub"
#include "core.pub"
#include "process.pub"
#include <CC/signal.h>		/* floating point exceptions */
SRCFILE("expr.c")

char *OpName(Op op)
{
	switch(op){
	case O_DEREF:	return "*";
	case O_REF:	return "&";
	case O_INDEX:	return "[]";
	case O_DOT:	return ".";
	case O_ARROW:	return "->";
	case O_PLUS:	return "+";
	case O_MINUS:	return "-";
	case O_MULT:	return "*";
	case O_DIV:	return "/";
	case O_CALL:	return "()";
	case O_MOD:	return "%";
	case O_ASSIGN:	return "=";
	case O_COMMA:	return ",";
	case O_SIZEOF:	return "sizeof";
	case O_TYPEOF:	return "typeof";
	case O_QINDEX:	return "[?]";
	case O_CAST:	return "(cast)";
	case O_QCAST:	return " ?";
	case O_QSTRUCT:	return "struct ?";
	case O_QENUM:	return "enum ?";
	case O_EQ:	return "==";
	case O_NE:	return "!=";
	case O_LT:	return "<";
	case O_GT:	return ">";
	case O_LE:	return "<=";
	case O_GE:	return ">=";
	case O_LOGAND:	return "&&";
	case O_LOGOR:	return "||";
	case O_LOGNOT:	return "!";
	case O_BITAND:	return "&";
	case O_BITOR:	return "|";
	case O_BITXOR:	return "^";
	case O_1SCOMP:	return "~";
	case O_FABS:	return "fabs";
	case O_ENV:	return "{}";
	case O_RANGE:	return "..";
	case O_SPECIAL:	return "special";
	case O_LSHIFT:	return "<<";
	case O_RSHIFT:	return ">>";
	default:	return Name( "Op=%d", op );
	}
}

int Prec( Op op )		/* for Expr.text() - not gram.y */
{
	switch(op){
	case O_ENV:	return 1;
	case O_CALL:
	case O_INDEX:
	case O_DOT:
	case O_ARROW:	return 2;
	case O_MULT:
	case O_DIV:
	case O_MOD:	return 3;
	case O_PLUS:
	case O_MINUS:	return 4;
	case O_LSHIFT:
	case O_RSHIFT:	return 5;
	case O_LT:
	case O_GT:
	case O_LE:
	case O_GE:	return 6;
	case O_EQ:
	case O_NE:	return 7;
	case O_BITAND:	return 8;
	case O_BITXOR:	return 9;
	case O_BITOR:	return 10;
	case O_LOGAND:	return 11;
	case O_LOGOR:	return 12;
	case O_ASSIGN:	return 13;
	case O_COMMA:	return 14;
	case O_RANGE:	return 15;
	default:	return 16;
	}
}

Expr *E_Id(char *id ) { return new Expr(E_ID,0,0,0,0,id,0,0); }

Expr *E_Sym(Symbol *s ) { return new Expr(E_SYMNODE,0,0,0,0,0,0,s); }
 
Expr *E_Unary(Op unary, Expr *sub) { return new Expr(E_UNARY,unary,sub,0,0,0,0,0); }

Expr *E_Binary(Expr*l,Op binary,Expr*r)
		{ return new Expr(E_BINARY,binary,l,r,0,0,0,0); }

Expr *E_IConst(long i) { return new Expr(E_LCONST,0,0,0,i,Token,LONG,0); }

Expr *E_DConst(double d) { return new Expr(E_DCONST,0,0,0,d,Token,DOUBLE,0); }

char *Expr.floaterror()
{
	return val.floaterror();
}

char *Expr.under(Expr *e)
{
	if( e->edisc != E_BINARY || Prec(e->op) < Prec(op) ) return e->text();
	return sf( "(%s)", e->text() );
}

Expr.Expr() {}

Expr.Expr( EDisc d, Op o, Expr* s1, Expr* s2, Cslfd c, char* s, int t, Symbol *v)
{
	type.pcc = t;
	edisc = d;
	op = o;
	sub1 = s1;
	sub2 = s2;
	val = c;
	id = s ? sf("%s",s) : "";
	sym = v;
	evalerr = 0;
	spy = 0;
	addr = 0;
	bitaddr = 0;
	trace( "Expr(edisc=%d,op=%s,sub1=%d,sub2=%d,val.lng=%d,id=%s,symbol=%d",
		edisc, OpName(op), sub1, sub2, val.lng, id?id:"", v); 
}

char *Expr.left(Expr *e)
{
	switch( e->edisc ){
	case E_BINARY:	switch( e->op ){
			case O_INDEX:
			case O_DOT:
			case O_ARROW:	return e->text();
			}
	case E_UNARY:	return sf( "(%s)", e->text() );
	default:	return e->text();
	}
}

char *Expr.textunary()
{
	trace( "%d.textunary()", this );	OK("textunary");
	switch( op ){
	case O_QCAST:
	case O_QSTRUCT:
	case O_QENUM:
		return sf("(%0.*s%s )%s",val.lng,"*******",OpName(op),sub1->text());
	case O_CAST:
		return sf("(%s)%s",type.text(),sub1->text());
	case O_QINDEX:
		return sf( "%s[ ? ]", left(sub1) );
	case O_SIZEOF:
	case O_TYPEOF:
	case O_FABS:
	case O_SPECIAL:
		return sf( "%s(%s)", OpName(op), sub1->text() );
	}
	if( sub1->edisc==E_BINARY && Prec(sub1->op)>=Prec(O_MULT) )
		return sf( "%s(%s)", OpName(op), sub1->text() );
	return sf( "%s%s", OpName(op), sub1->text() );
}

char *Expr.textbinary()
{
	trace( "%d.textbinary()", this );	OK("textbinary");
	switch( op ){
	case O_ENV:
		return sf( "{%s}%s", sub1->text(), sub2->text() );
	case O_COMMA:							/* ??? */
		return sf( "%s,%s", sub1->text(), sub2->text() );
	case O_CALL:
		return sf( "%s(%s)", sub1->text(), sub2?sub2->text():"" );
	case O_ASSIGN:
		return sf("(%s:=%s)", left(sub1), sub2->text() );
	case O_INDEX:
		return sf("%s[%s]", left(sub1), sub2->text() );
	case O_DOT:
	case O_ARROW:
		return sf("%s%s%s", left(sub1), OpName(op), sub2->text());
	}
	if( sub1->edisc==E_BINARY && Prec(sub1->op) == Prec(op) )
		return sf("%s%s%s", sub1->text(), OpName(op), under(sub2));
	return sf("%s%s%s", under(sub1), OpName(op), under(sub2));
}

char *Expr.text()
{
	trace( "%d.text()", this );	OK("text");
	switch( edisc ){
	case E_DCONST:	if( !id || !*id ) return sf( "%g", val.dbl );
	case E_LCONST:	if( !id || !*id ) return sf( "%d", val.lng );
	case E_ID:	return id;
	case E_UNARY:	return textunary();
	case E_BINARY:	return textbinary();
	case E_SYMNODE:
		if( !sym || !sym->_text ) return "<symbol>";
		return sym->_text;
	}
	return "<expr>";
}

int Expr.format()
{
	trace( "%d.format() %s %s", this, text(), type.text() ); OK(F_HEX);
	return type.format();
}

void Expr.reformat(int over, int stars)
{
	trace("%d.reformat(0x%X,%d) %s %s",this,over,stars,text(),type.text()); VOK;
	switch( edisc ){
	case E_ID: break;		// shut up cfront
	case E_LCONST:
	case E_DCONST:
		type.reformat(over);
		return;
	case E_SYMNODE:
		IF_LIVE(!sym) return;
		((Var*)sym)->type.reformat(over,stars);
		return;
	case E_UNARY:
		switch( op ){
		case O_REF:
		case O_SIZEOF:
		case O_CAST:
			type.reformat(over,stars);
			return;
		case O_DEREF:
			sub1->reformat(over,stars+1);
			return;
		case O_MINUS:
		case O_1SCOMP:
		case O_LOGNOT:
			sub1->reformat(over,stars);
			return;
		}
	case E_BINARY:
		switch( op ){
		case O_DIV:
		case O_MULT:
		case O_MOD:
		case O_PLUS:
		case O_MINUS:
		case O_BITAND:
		case O_BITOR:
		case O_BITXOR:
			sub1->reformat(over,stars);		/* fall thru */
		case O_ARROW:
		case O_DOT:
		case O_COMMA:
			sub2->reformat(over,stars);
			return;
		case O_ASSIGN:
		case O_ENV:
		case O_LSHIFT:
		case O_RSHIFT:
			sub1->reformat(over,stars);
			return;
		case O_INDEX:
			sub1->reformat(over,stars+1);
			return;
		case O_CALL:
			Func *func = (Func*) sub1->sym;
			if( func->ok() )
				func->type.decref()->reformat(over,stars);
			return;
		}
	}
}

char *Expr.ascii(Frame *frame, int limit )
{
	static char buf[256];
	char *raw, *fail = "fail";		// fail is unique
	Bls esc( "\"" );
	int i;

	trace("%d.ascii(%d)", this, frame);	OK("ascii");
	if( !val.lng ) return "0";
	raw = frame->peekstring(val.lng, fail);
	if( raw == fail ) return "<string error>";
	for( i = 0; raw[i] && i<limit; ++i )
		esc.af( "%s",  FmtByte(raw[i]) );
	if( raw[i] ) esc.af("...");
	sprintf(buf, "%s\"", esc.text);
	return buf;
}

char *Expr.enumformat()
{
	Var *m;

	trace( "%d.enumformat()", this ); OK("enumformat");
	TypMems tm(type.utype());
	while( m = tm.gen() )
		if( m->range.lo == val.lng )
			return m->text();
	return sf( "(enum %s)%d", type.utype()?type.utype()->text():"?", val.lng );
}

char *Expr.utypeformat(Frame *frame, Bls &build)
{
	Var *m, g(0,0,0,0,"$pi.Expr.utypeformat");
	int i = 0;
	Expr e;		/* use constructor to guarantee zeros? */

	trace("%d.utypeformat(%d,%d)", this, frame, &build); OK("utypeformat");
	build.af("{");
	TypMems tm(type.utype());
	while( m = tm.gen() ) if( m->showorhide==SHOW ){
		Bls local;
		g = *m;
		g._disc = U_GLB;
		DType t = g.type;
		e.bitaddr = 0;
		if( t.pcc==BITS || t.pcc==UBITS ){
			e.bitaddr = g.range.lo;
			g.range.lo = 0;
		}
		g.range.lo += addr;
		e.edisc = E_SYMNODE;
		e.sym = &g;
		e.spy = 0;
		e.op = 0;
		build.af( "%s%s", i++?",":"", e.evaltext(frame,local) );
	}
	build.af( "}" );
	return build.text;
}

char *Expr.evaltextcomma(Frame *frame, Bls &build)
{
	trace("%d.evaltextcomma(%d,%d)",this,frame,&build);OK("evaltextcomma");
	if( spy ) build.af(">>> ");			// SWMR 493	
	sub1->evaltext(frame,build);
	if( evalerr = sub1->evalerr ) return build.text;
	build.af(", ");
	sub2->evaltext(frame,build);
	evalerr = sub2->evalerr;
	type = sub2->type;
	val = sub2->val;
	return build.text;
}

char *Expr.evaltext(Frame *frame, Bls &build)
{
	trace( "%d.evaltext(%d,%d)", this, frame, &build ); OK("evaltext");
	evalerr = 0;
	doevaltext(frame, build);
	if( evalerr ) setspy(0);
	if( spy ) spy->b = build.text;
	return build.text;
}

char *Expr.doevaltext(Frame *frame, Bls &build)
{
	char *error, *t;
	Format form(0, frame->core->symtab());

	trace( "%d.doevaltext(%d,%d)", this, frame, &build ); OK("doevaltext");
	switch( op ){
		case O_QINDEX:
		case O_QCAST:
		case O_QENUM:
		case O_QSTRUCT:
			build.af( "%s", text() );
			return build.text;
	}
	if( op == O_COMMA ) return evaltextcomma(frame,build);
	if( error = eval(frame) ){
		build.af( "%s: %s", text(), error );
		evalerr = 1;
		return build.text;
	}
	if( spy ) build.af(">>> ");	
	build.af( "%s", text() );
	if( type.pcc == VOID ){
		build.af( "=void" );
		return build.text;
	}
	form.format = format();
	if( form.format&(F_DOUBLE|F_FLOAT) && val.floaterror() ){
		build.af( "=<%s>", val.floaterror() );
		form.format &= ~(F_DOUBLE|F_FLOAT);
		form.format |= F_NONE;
	}
	if( form.format&F_ARY ) val.lng = addr;
	if( *(t = form.f(val.lng,val.dbl)) )
		build.af( "=%s", t );
	if( form.format&F_UTYPE ){
		build.af( "=");
		utypeformat(frame,build);
	}	
	if( form.format&F_ENUM )
		build.af( "=%s", enumformat() );	
	if( form.format&F_STRING )
		build.af( "=%s", ascii(frame, form.format&F_LONGSTRING?200:20) );
	return build.text;
}

char *Expr.getval( Frame *frame )		/* addr already fixed */
{
	trace( "%d.getval(%d)", this, frame );	OK("getval");
	if( sym ){
		switch( sym->disc() ){
		case U_FUNC:
			type = ((Func*)sym)->type;
			break;
		default:
			type = ((Var*)sym)->type;
			((Var*)sym)->show(SHOW);
		}
	}
	if( !addr ) return "addressing error: 0";
	addr += (bitaddr>>5)*4;
	if( type.isscalar() ){
		Cslfd *m = frame->peek(addr,0);
		if( !m ) return sf( "addressing error: 0x%X", addr );
		val = *m;
	}
	if( type.isscalar() ) switch( type.pcc ){
		case BITS:
		case UBITS:	val.lng = (val.lng>>(bitaddr&31));
				int power = 1<<type.dim;
				val.lng &= power-1;
				if( type.pcc==BITS && val.lng&(power>>1) )
					val.lng |= ~(power-1);
				addr = 0; break;
		case CHAR:	val.lng = (char)  val.chr;		break;
		case UCHAR:	val.lng = (unsigned char) val.chr;	break;
		case SHORT:	val.lng = (short) val.sht;		break;
		case USHORT:	val.lng = (unsigned short) val.sht;	break;
		case FLOAT:	val.dbl = val.flt;			break;
	}
	trace( "%s %d %d %s", sym?sym->_text:"", addr, val.lng, type.text() );
	return 0;
}

char *Expr.invalidoperands(char *more)
{
	trace( "%d.invalidoperands()", this );	OK("invalidoperands");
	char *colon = more ? ": " : "";
	if( !more ) more = "";
	return sf( "invalid operand(s) of %s%s%s", OpName(op), colon, more );
}

char *Expr.eval(Frame *frame)
{
	trace( "%d.eval(%d)", this, frame );	OK("eval");
	switch(op){
		case O_QINDEX:
		case O_QCAST:
		case O_QSTRUCT:
		case O_QENUM:
			return "? stops evaluation";
	}
	switch( edisc ){
	case E_DCONST:
	case E_LCONST:
		return 0;				/* should all be there! */
	case E_UNARY:
		return evalunary(frame);
	case E_BINARY:
		return evalbinary(frame);
	case E_ID:
		if( !(sym = frame->idtosym(id)) )
			return enumid(frame);
		edisc = E_SYMNODE;				/* fall thru  */
	case E_SYMNODE:
		if( !sym ) return "symbol table error";
		addr = frame->locate((Var*)sym);
		return getval(frame);
	default:
		return "not an expression";
	}
}

char *Expr.enumid(Frame *frame)
{
	trace("%d.enumid(%s)", this, id); OK("enumid");
	UType *u; Var *v;
	for( u = frame->symtab()->utypelist(); u; u = (UType*) u->rsib ){
		if( u->type.pcc != ENUMTY ) continue;
		TypMems tm(u);
		while( v = tm.gen() ) if( !strcmp(id, v->_text) ){
			edisc = E_LCONST;
			val.lng = v->range.lo;
			addr = 0;
			type.pcc = LONG;
			return 0;
		}
	}
	return sf("not found: %s", id);
}

char *Expr.evalindex(Frame *frame, Expr *ap, long i)
{
	long size = 0;

	trace( "%d.index(%d,%d,%d)", this, frame, ap, i); OK("evalindex");
	if( ap && ap->type.decref() ){ 
		type = *ap->type.decref();
		size = type.size_of();
	}
	if( !size ) return "cannot determine size for []";
	addr = ap->type.isptr() ? ap->val.lng : ap->addr;
	if( !addr ) return "zero pointer for []";
	addr += size * i;
	return getval(frame);
}

char *Expr.evaldotarrow(Frame *frame)
{
	DType t;

	trace( "%d.evaldotarrow(%d)", this, frame ); OK("evaldotarrow");
	addr = 0;
	t = sub1->type;
	if( op == O_DOT )
		addr = sub1->addr;
	else {
		if( !t.isptr() ) return invalidoperands("not a pointer");
		t = *t.decref();
		addr = sub1->val.lng;
	}
	if( !t.isstrun() ) return invalidoperands("not a struct/union");
	if( !addr ) return invalidoperands("zero address");
	if( sub2->edisc == E_ID && t.utype() ){
		TypMems tm(t.utype());
		while( sub2->sym = tm.gen() )
			if( !strcmp(sub2->sym->text(), sub2->id) ) break;
		if( !sub2->sym ) return sf("not found: %s", sub2->id);
		sub2->edisc = E_SYMNODE;
	}
	if( sub2->edisc != E_SYMNODE ) return invalidoperands();
	sym = sub2->sym;
	t = ((Var*)sym)->type;
	if( t.pcc==BITS || t.pcc==UBITS )
		bitaddr = sym->range.lo;
	else
		addr += sym->range.lo;
	return getval(frame);
}

Expr *Expr.actual(int a)
{
	int m;
	trace( "%d.actual(%d)", this, a );
	if( !this ) return 0;
	if( op != O_COMMA ) return a == 1 ? this : 0;
	for( m = 0; sub1->actual(m+1); ++m ) {}
	return a<=m ? sub1->actual(a) : sub2->actual(a-m);
}

char *Expr.evalcall(Frame *frame)
{
	char *error = 0;
	long i, nargs = 1, argwords = 0, regloc;
	Var *formal;
	Func *func;
	Frame *called = 0;

	trace( "%d.evalcall(%d)", this, frame ); OK("evalcall");
	if( !sub1 )return "<fcn call>";
	switch( sub1->edisc ){
	default: return "<fcn call>";
	case E_ID:
		sub1->sym = frame->symtab()->idtosym( U_FUNC, sub1->id );
	case E_SYMNODE:
		func = (Func*) sub1->sym;
	}
	if( !func ) return sf( "not a function: %s", sub1->id );
	while( sub2->actual(nargs) )
		if( !(formal = func->argument(nargs)) )
			return "too many args in function call";
		else {
			argwords += (formal->type.size_of()+3)/4;   /* vax & 32 */
			++nargs;
		}
	if( func->argument(nargs) )
		return "too few args in function call";
	--nargs;
	Core *core = frame->core;
	if( !core->online() )
		return "cannot call function in dump";
	Context *cc = core->newContext();
	if( cc->error ){
		delete cc;
		return cc->error;
	}
	called = new Frame(core);
	called->ap = core->apforcall(argwords*4);
	if( !called->ap )
		{ error = "function calling is broken"; goto Restore; }
	for( i = 1; i <= nargs; ++i ){
		Expr *e;
		e = E_Binary(E_Sym(func->argument(i)), O_ASSIGN, sub2->actual(i));
		if( error = e->sub1->eval(called) )
			{ error = sf("formal arg: %s", error); goto Restore; }
		if( error = e->sub2->eval(frame) )
			{ error = sf("actual arg: %s", error); goto Restore; }
		if( error = e->evalassign(called) )
			{ error = sf("arg assign: %s", error); goto Restore; }
	}
	if( error = core->docall(func->range.lo, argwords) )
		goto Restore;
	type = *func->type.decref();
	regloc = core->returnregloc();
	if( !regloc)
		{ error = "function return error"; goto Restore; }
	if( type.isstrun() )
		switch( type.size_of() ){
		case 1:
		case 2:
		case 4:	addr = regloc;
			break;
		default:
			addr = core->peek(regloc)->lng;
		}
	else
		val = *core->peek(regloc);		/* doubles? */
Restore:
	if( called ) delete called;
	cc->restore();
	if( cc->error && !error )
		error = cc->error;
	delete cc;
	return error;
}

char *Expr.evalenv(Frame *frame)
{
	Frame *env = frame ? frame->core->process()->frame(0) : 0;
	char *error;

	trace( "%d.evalenv(%d)", this, frame ); OK("evalenv");
	for( ; env; env = env->caller() )
		if( !strcmp(env->func->_text,sub2->text()) )
			break;
	if( !env ){
		Frame sta(frame->core);
		sta.level = -1;		// regs fail: Frame.regloc Frame.locate
		sta.func = (Func*) sta.core->symtab()->idtosym(U_FUNC,sub2->text());
		if( sta.func ) env = &sta;
	}
	if( !env ) return sf( "not found: %s()", sub2->text() );
	if( error = sub1->eval(env) ) return error;
	type = sub1->type;
	val = sub1->val;
	addr = sub1->addr;
	return 0;
}

char *Expr.evalrange()
{
	const int range = 32;
	trace( "%d.evalrange()", this ); OK("evalrange");
	if( !sub1->type.isintegral()
	 || !sub2->type.isintegral()
	 ||  sub2->val.lng < sub1->val.lng )
		return invalidoperands();
	if( sub2->val.lng > sub1->val.lng+range )
		return sf( "range may not exceed %d", range );
	return 0;
}

void Expr.catchfpe()		/* VAX: put down operands that must succeed */
{				/*	and restart the instruction	    */
	*fp1 = *fp2 = 1.0;
	fpe = "floating point exception";
}

char *Expr.evalflop()		/* VAX host */
{
	char *error;
	trace( "%d.evalflop()", this ); OK("evalflop");
	if( !sub1->type.isreal() || !sub2->type.isreal() )
		return invalidoperands();
	if( (error = sub1->floaterror())
	 || (error = sub2->floaterror()) )
			 return invalidoperands(error);
	type.pcc = DOUBLE;
	double l = sub1->val.dbl; double r = sub2->val.dbl;
	fp1 = &l;
	fp2 = &r;
	fpe = 0;
	signal(SIGFPE, (SIG_ARG_TYP)&Expr::catchfpe);
	switch( op ){
		case O_MULT:	val.dbl = l*r;	break;
		case O_DIV:	val.dbl = l/r;	break;
		case O_PLUS:	val.dbl = l+r;	break;
		case O_MINUS:	val.dbl = l-r;	break;
	}
	signal(SIGFPE, (SIG_ARG_TYP)SIG_DFL);
	return fpe;
}

char *Expr.evalbinary(Frame *frame)
{
	char *error;
	long size;

	trace( "%d.evalbinary(%d)", this, frame ); OK("evalbinary");
	IF_LIVE( edisc!=E_BINARY ) return "<binary expr>";
	switch( op ){
		case O_CALL: return evalcall(frame);
		case O_ENV:  return evalenv(frame);
	}
	if( error = sub1->eval(frame) ) return error;
	type.pcc = LONG;
	type.over = sub1->type.over;
	if( op==O_DOT || op==O_ARROW ) return evaldotarrow(frame);
	if( (sub1->type.isintegral() && op==O_LOGAND && !sub1->val.lng)
	 || (sub1->type.isintegral() && op==O_LOGOR  &&  sub1->val.lng) ){
		val = sub1->val.lng != 0;
		return 0;
	}
	if( error = sub2->eval(frame) ) return error;
	type.over |= sub2->type.over;
	switch( op ){
	case O_RANGE:
		return evalrange();
	case O_COMMA:
		type = sub2->type;
		val = sub2->val;
		return 0;
	case O_ASSIGN:
		return evalassign(frame);
	case O_MULT:
	case O_DIV:
		if( sub1->type.isreal() || sub2->type.isreal() )
			return evalflop();
	case O_MOD:
		if( sub1->type.isintegral() && sub2->type.isintegral() ){
			long l = sub1->val.lng, r = sub2->val.lng;
			if( op==O_MULT ){
				val.lng = l * r;
				return 0;
			}
			if( r == 0 ) return "zero divide";
			if( op == O_MOD ) val.lng = l % r;
			if( op == O_DIV ) val.lng = l / r;
			return 0;
		}
		return invalidoperands();
	case O_LOGAND:
	case O_LOGOR:
	case O_BITAND:
	case O_BITOR:
	case O_BITXOR:
	case O_LSHIFT:
	case O_RSHIFT:
		if( sub1->type.isscalar() && sub2->type.isscalar() ){
			long l = sub1->val.lng, r = sub2->val.lng;
			switch( op ){
				case O_LOGAND:	val.lng = l && r; return 0;
				case O_LOGOR:	val.lng = l || r; return 0;
				case O_BITAND:	val.lng = l &  r; return 0;
				case O_BITOR:	val.lng = l |  r; return 0;
				case O_BITXOR:	val.lng = l ^  r; return 0;
				case O_LSHIFT:	val.lng = l << r; return 0;
				case O_RSHIFT:	val.lng = l >> r; return 0;
			}
			return "internal error: && || & | ^ >> <<";
		}
		return invalidoperands();
	case O_EQ:
	case O_NE:
	case O_LT:
	case O_GT:
	case O_LE:
	case O_GE:
		type.pcc = LONG;
		if( sub1->type.isreal() || sub2->type.isreal() ){
			if( !sub1->type.isreal() || sub1->floaterror()
			 || !sub2->type.isreal() || sub2->floaterror() )
				return invalidoperands();
			double l = sub1->val.dbl; double r = sub2->val.dbl; /*C++*/
			switch( op ){
				case O_EQ: val.lng = l == r; return 0;
				case O_NE: val.lng = l != r; return 0;
				case O_LT: val.lng = l <  r; return 0;
				case O_GT: val.lng = l >  r; return 0;
				case O_LE: val.lng = l <= r; return 0;
				case O_GE: val.lng = l >= r; return 0;
			}
			return "internal floating relation error";
		}
		if( sub1->type.isscalar() && sub2->type.isscalar() ){
			long l = sub1->val.lng, r = sub2->val.lng;
			switch( op ){
				case O_EQ: val.lng = l == r; return 0;
				case O_NE: val.lng = l != r; return 0;
				case O_LT: val.lng = l <  r; return 0;
				case O_GT: val.lng = l >  r; return 0;
				case O_LE: val.lng = l <= r; return 0;
				case O_GE: val.lng = l >= r; return 0;
			}
			return "internal fixed point relation error";
		}
		return invalidoperands();
	case O_PLUS:
	case O_MINUS:
		if( sub1->type.isreal() || sub2->type.isreal() )
			return evalflop();
		if( sub1->type.isary() ){
			int o = sub1->type.over;		// SWMR buf+4
			sub1->val.lng = sub1->addr;
			sub1->type = sub1->type.decref()->incref();
			sub1->type.over = o&~F_ARY;		// SWMR buf+4
		}
		if( sub2->type.isary() ){
			int o = sub2->type.over;		// SWMR buf+4
			sub2->val.lng = sub2->addr;
			sub2->type = sub2->type.decref()->incref();
			sub2->type.over = o&~F_ARY;		// SWMR buf+4
		}
		if( sub1->type.isptr() && sub2->type.isintegral() ){
			size = sub1->type.decref()->size_of();
			if( !size ) return "pointer arithmetic error";
			if( op == O_MINUS ) size = -size;
			val.lng = sub1->val.lng + size*sub2->val.lng;
			type = sub1->type;
			return 0;
		}
		if( sub1->type.isintegral() && sub2->type.isintegral() ){
			if( op==O_PLUS ) val.lng = sub1->val.lng+sub2->val.lng;
			if( op==O_MINUS) val.lng = sub1->val.lng-sub2->val.lng;
			return 0;
		}
		if( op == O_PLUS && sub2->type.isptr() && sub1->type.isintegral() ){
			size = sub2->type.decref()->size_of();
			if( !size ) return "pointer arithmetic error";
			val.lng = sub2->val.lng + size*sub1->val.lng;
			type = sub2->type;
			return 0;
		}
		if( op == O_MINUS && sub1->type.isptr() && sub2->type.isptr() ){
			size = sub1->type.decref()->size_of();
			if( !size || size!=sub2->type.decref()->size_of() )
				return "pointer-pointer size error";
			val.lng = (sub1->val.lng - sub2->val.lng)/size;
			type.over |= sub2->type.over;
			return 0;
		}
		return invalidoperands();
	case O_INDEX:
		trace( "%s %s", sub1->type.text(), sub2->type.text() );
		if( sub1->type.isaryorptr() && sub2->type.isintegral() )
			return evalindex(frame,sub1,sub2->val.lng);
		if( sub2->type.isaryorptr() && sub1->type.isintegral() )
			return evalindex(frame,sub2,sub1->val.lng);
		return invalidoperands();
	default:
		return "binary operator not implemented";
	}
}

char *Expr.evalcast()
{
	trace( "%d.evalcast()", this,  ); OK("<cast>");
	IF_LIVE( !type.isscalar() || !sub1->type.isscalar() ) return "cast error";
	if( type.isptr() ){
		if( sub1->type.isreal() ) return "can't cast float to ptr";
		val.lng = sub1->val.lng;
		return 0;
	}
	if( type.isreal() ){
		if( sub1->floaterror() ) return sub1->floaterror();
		val.dbl = sub1->val.dbl;
		if( !sub1->type.isreal() ) val.dbl = sub1->val.lng;
		return 0;
	}
	val.lng = sub1->val.lng;
	switch( sub1->type.pcc ){
		case CHAR:	val.lng = sub1->val.chr; break;
		case SHORT:	val.lng = sub1->val.sht; break;
	}
	addr = sub1->addr;
	if( sub1->type.isreal() ){
		if( sub1->floaterror() ) return sub1->floaterror();
		val.lng = (long) sub1->val.dbl;
	}
	return 0;
}

char *Expr.evalunary(Frame *frame)
{
	char *error;

	trace( "%d.evalunary(%d)", this, frame ); OK("<unary expr>");
	IF_LIVE( edisc!=E_UNARY ) return "<unary expr>";
	if( error = sub1->eval(frame) ) return error;
	switch( op ){
	case O_SPECIAL:
		type = sub1->type;
		addr = sub1->addr;
		val = sub1->val;
		if( type.isptr() ){
			type = *type.decref();
			addr = val.lng;
		}
		if( !type.pcc == STRTY || !type.utype() || !addr )
			return invalidoperands();
		error = frame->special( type.utype()->text(), addr );
		{			// cfront bug
		DType nulltype;
		type = nulltype;
		}			// cfront bug
		type.pcc = VOID;
		addr = 0;
		return error;
	case O_1SCOMP:
	case O_LOGNOT:
		if( !sub1->type.isscalar() || sub1->type.isreal() )
			return invalidoperands();
		type.pcc = LONG;
		if( op == O_1SCOMP ) val.lng = ~sub1->val.lng;
		if( op == O_LOGNOT ) val.lng = !sub1->val.lng;
		return 0;
	case O_FABS:
		{				// cfront bug
		double fabs(double);
		if( !sub1->type.isreal() || sub1->floaterror() )
			return invalidoperands();
		type.pcc = DOUBLE;
		val.dbl = fabs(sub1->val.dbl);		/* cannot fail? */
		return 0;
		}				// cfront bug
	case O_MINUS:
		if( sub1->type.isintegral() ){
			type.pcc = LONG;
			val.lng = -sub1->val.lng;
			return 0;
		}
		if( sub1->type.isreal() && !sub1->floaterror() ){
			type.pcc = DOUBLE;
			val.dbl = -sub1->val.dbl;
			return 0;
		}
		return invalidoperands();
	case O_CAST:
		return evalcast();
	case O_SIZEOF:
		addr = 0;
		if( !(val.lng = sub1->type.size_of())) return "sizeof error";
		type.pcc = UNSIGNED;
		return 0;
	case O_TYPEOF:
		return sub1->type.text();
	case O_DEREF:
		if( !sub1->type.isptr() ) return "unary * applied to non-pointer";
		addr = sub1->val.lng;
		type = *sub1->type.decref();
		return getval(frame);
	case O_REF:
		if( !sub1->addr ) return "unary & applied to non-lvalue";
		addr = 0;
		int o = type.over;
		type = sub1->type.incref();
		type.over = o;
		val.lng = sub1->addr;
		return 0;
	default:
		return "unary operator not implemented";
	}
}

Index Expr.castcarte()
{
	static Index ix;
	static short bt[] = { DOUBLE, FLOAT, LONG, SHORT, CHAR, 0 };
	Menu m;
	DType *base, *ptr;

	trace( "%d.castcarte()", this );	OK(ZIndex);
	if( !ix.null() ) return ix;
	Action a = (Action)&Phrase::applycast;
	int t;
	for( t = 0; bt[t]; ++t ){
		base = new DType();
		base->pcc = bt[t];
		ptr = new DType;
		*ptr = base->incref();
		m.first( sf( "%s\240",  ptr->text() ), a, (long) ptr  );
		m.first( sf( " %s\240", base->text()), a, (long) base );
	}
	Action i = (Action)&Phrase::increfcast;
	Action e = (Action)&Phrase::enumcast;
	Action s = (Action)&Phrase::strcast;
	m.last( "        * ? ",	i, 1 );
	m.last( "     enum ? ",	e, 0 );
	m.last( "  *struct ? ",	s, 1 );
	return ix = m.index("cast $");
}

Index Expr.carte(Frame *f)
{
	Menu m;

	trace( "%d.carte()", this );	OK(ZIndex);
	m.last( "eval $", (Action)&Phrase::evaluate );
	if( evalerr || op==O_TYPEOF )
		return m.index();
	switch( op ){
	case O_QCAST:
		return castcarte();
	case O_QSTRUCT:
		return f ? f->symtab()->utypecarte(STRTY)  : ZIndex;
	case O_QENUM:
		return f ? f->symtab()->utypecarte(ENUMTY) : ZIndex;
	case O_QINDEX:
		return NumericRange( -2, 20 );
	}
	if( !spy )
		m.first( "spy on $", (Action) &Phrase::setspy, 1 );
	else
		m.first( "unspy $",   (Action) &Phrase::setspy, 0 );
	if( addr ) m.last( "& $", (Action)&Phrase::applyunary, (long)O_REF );
	if( val.lng )
		m.last( "mem .=$ ", (Action)&Phrase::memory );
	if( type.isscalar() ) m.last(castcarte());
	m.last(type.carte());
	return m.index();
}

void Expr.setspy(long s)
{
	trace( "%d.setspy(%d)", this, s );	VOK;
	if( !s && spy ){
		delete spy;
		spy = 0;
	} else if( s )
		spy = new Spy;
}

char *Expr.evalassign(Frame *frame)
{
	char *error;

	trace( "%d.evalassign(%d)", this, frame ); OK("evalassign");
	if( !sub1->addr ) return "lhs of = does not yield an lvalue";
	if( sub2->type.isary() && sub2->addr ){
		sub2->type.pcc = PTR;
		sub2->val.lng = sub2->addr;
	}
	if( sub1->type.isscalar() && sub2->type.isscalar() ){
		DType type1 = sub1->type, type2 = sub2->type;
		long addr1 = sub1->addr, size1 = type1.size_of();
		Cslfd *val2 = &sub2->val;
		if( type1.isreal() || type1.isreal() ){
			if( !type1.isreal() || !type2.isreal() )
				return invalidoperands("mixed mode");
			error = frame->pokedbl(addr1, val2->dbl, type1.size_of());
		} else
			error = frame->poke(addr1, val2->lng, type1.size_of());
		if( error )
			return sf( "%s: 0x%X", error, sub1->addr );
		if( error = sub1->eval(frame) )
			return error;
		type = sub1->type;
		val = sub1->val;
		addr = 0;
		return 0;
	}
	if( sub1->type.isstrun() && sub2->type.isstrun() ){
		UType *u1 = sub1->type.utype(), *u2 = sub2->type.utype();
		if( !u1
		 || !u2
		 || u1->type.pcc != u2->type.pcc
		 || strcmp(u1->type.text(), u2->type.text()) )
			return "incompatible struct/union =";
		if( !sub2->addr )
			return "rhs of struct/union = does not yield an lvalue";
		error = frame->blockmove(sub2->addr,sub1->addr,u1->type.size_of());
		type = sub1->type;
		val = 0;
		addr = sub1->addr;
		return error;
	}
	return "invalid asssignment";
}
