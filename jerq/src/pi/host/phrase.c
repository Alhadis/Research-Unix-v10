#include "expr.pri"
#include "phrase.pri"
#include "frame.pub"
#include "parse.h"
#include "format.pub"
SRCFILE("phrase.c")
			/* next three are for Phrase.memory() ! */
#include "symtab.pub"	
#include "symbol.h"	
#include "core.pub"
#include "process.pub"

Phrase.Phrase(Frame *f, Phrase *s, Expr *e, long k)
{
	trace( "%d.Phrase(%d,%d,%d,%d)", this, f, s, e, k );
	frame = f;
	sib = s;
	expr = e;
	key = k;
}

Pad *Phrase.pad()		{ OK(0); return frame->pad; }

void Phrase.setspy(long s)
{
	trace( "%d.setspy(%d)", this, s ); VOK;
	expr->setspy(s);
	evaluate();
}

void Phrase.reformat(int over)
{
	trace( "%d.reformat(%d)", this, over ); VOK;
	expr->reformat(over);
	evaluate();
}

void Phrase.derive(Expr *e)
{
	char *error;
	frame->freekey(key+2);
	if( error = frame->makephrase(e,key+2) )
		pad()->insert( key+1, SELECTLINE, error );
}
	
void Phrase.applyunary(Op op)
{
	trace( "%d.applyunary(%s)", this, OpName(op) ); VOK;
	derive(E_Unary(op,expr));
}

void Phrase.applybinary(Op op, Expr *e)
{
	trace( "%d.applybinary(%s,%s)", this, OpName(op), e->text() ); VOK;
	derive(E_Binary(expr,op,e));
}

void Phrase.applydot(Var *v)
{
	trace("%d.applydot(%s)", this, v->text()); VOK;
	applybinary(O_DOT, E_Sym(v));
}

void Phrase.applyarrow(Var *v)
{
	trace("%d.applyarrow(%s)", this, v->text()); VOK;
	applybinary(O_ARROW, E_Sym(v));
}

void Phrase.allstar(UType *t)
{
	trace("%d.allstar(%s)", this, t->text()); VOK;
	t->show(SHOW);
	applyunary(O_DEREF);
}

void Phrase.alleval(UType *t)
{
	trace("%d.alleval(%s)", this, t->text()); VOK;
	t->show(SHOW);
	evaluate();
}

void Phrase.evaluate()
{
	char *error;

	trace( "%d.evaluate()", this ); VOK;
	if( error = frame->makephrase(expr,key) )
		pad()->insert( key+1, SELECTLINE, error );
}

int Phrase.changed(Bls &b)
{
	Spy s;

	trace( "%d.changed()", this ); OK(0);
	IF_LIVE( !expr->spy ) return 0;
	s = *expr->spy;
	expr->evaltext(frame,b);
	if( !strcmp(s.b.text, expr->spy->b.text) )
		return 0;
	evaluate();
	return 1;
}

char *Phrase.help()
{
	trace( "%d.help()", this ); OK("Phrase.help");
	switch( expr->op ){
	case O_QSTRUCT:
	case O_QENUM:
		return "<id> { struct/enum name }";
	case O_QINDEX:
		Expr *e = E_Binary( expr->sub1, O_INDEX, E_Id("lo..hi") );
		return sf( "<lo> [..<hi>] {eval %s}", e->text() );
	default:
		return sf( "<expr> {eval in this scope; $ is %s}", expr->text());
	}
}

char *Phrase.kbd(char *s)
{
	trace( "%d.kbd(%s)", this, s );	OK("kbd");
	pad()->removeline(key+1);
	int q = expr->op == O_QINDEX;
	Parse y( q ? G_DOTDOT : G_EXPR, q ? 0  : expr );
	Expr *e = (Expr*)y.parse(s);
	if( !e ){
		pad()->insert(key+1, SELECTLINE, "%s: %s", y.error, s);
		return 0;
	}
	switch( expr->op ){
	case O_QINDEX:
		plugqindex(e);
		break;
	case O_QSTRUCT:
		plugqtype(e);
		break;
	case O_QENUM:
		plugqtype(e);
		break;
	default:
		derive(e);
	}
	return 0;
}

void Phrase.plugqtype(Expr *e)
{
	trace( "%d.plugqtype(%d)", this, e );	VOK;
	if( e->edisc != E_ID ){
		pad()->insert(key+1, SELECTLINE, "not an id: %s", e->text());
		return;
	}
	UType *u = (UType*) frame->core->symtab()->idtosym(U_UTYPE, e->id);
	if( !u ){
		pad()->insert(key+1, SELECTLINE, "type not found: %s", e->id);
		return;
	}
	applycast(&u->type);
}

void Phrase.plugqindex(Expr *e)
{
	trace( "%d.pluqindex(%d)", this, e );	VOK;
	IF_LIVE( !expr || expr->op != O_QINDEX ) return;
	switch( e->op ){
	case O_RANGE:
		{			// cfront bug
		Bls error;
		e->evaltext(frame,error);
		if( !e->evalerr ){
			long i;
			for( i = e->sub2->val.lng; i >= e->sub1->val.lng; --i )
				derive( E_Binary(expr->sub1,O_INDEX,E_IConst(i)) );
		break;
		}
		}			// cfront bug
	default:
		derive( E_Binary(expr->sub1, O_INDEX, e) );
	}
	evaluate();
}

void Phrase.numeric(long n)
{
	trace( "%d.numeric(%d)", n ); VOK;
	sprintf( Token, "%d", n);
	plugqindex( E_IConst(n) );
}

void Phrase.memory()
{
	trace( "%d.memory()", this ); VOK;
	IF_LIVE( !expr ) return;
	frame->symtab()->core()->process()->openmemory(expr->val.lng);
}

int Phrase.iscast()
{
	trace( "%d.iscast()", this ); OK(0);
	switch( expr ? expr->op : 0 ){
		case O_QCAST:
		case O_QSTRUCT:
		case O_QENUM:	return 1;
	}
	return 0;
}

void Phrase.applycast(DType *d)
{
	int i = 0;
	trace( "%d.applycast(%d)", this, d ); VOK;
	Expr *e = expr;
	if( iscast() ){
		i = e->val.lng;
		e = e->sub1;
	}
	while( i-->0 ){
		DType *ptr = new DType;
		*ptr = d->incref();
		d = ptr;
	}
	e = E_Unary(O_CAST, e);
	e->type = *d;
	derive(e);
}

void Phrase.increfcast(long i)
{
	trace("%d.increfcast(%d)", this, i); VOK;
	if( !iscast() )
		expr = E_Unary(O_QCAST, expr);
	Expr *e = E_Unary(expr->op, expr->sub1);
	e->val.lng += i;				/* += ? */
	derive(e);
}

void Phrase.soretycast(long i, short qop)
{
	trace( "%d.soretycast(%d,%d)", this, i, qop ); VOK;
	if( !iscast() )
		expr = E_Unary(O_QCAST, expr);
	Expr *e = E_Unary(qop, expr->sub1);
	e->val.lng = i + expr->val.lng;
	derive(e);
}

void Phrase.strcast(long i) { soretycast(i, O_QSTRUCT); }

void Phrase.enumcast(long i) { soretycast(i, O_QENUM); }
