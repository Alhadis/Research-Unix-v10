#include "master.pri"
#include "process.pri"
#include "frame.pub"
#include "hostcore.h"
#include "format.pub"
#include <CC/stdio.h>
#include "expr.pub"
#include "symbol.h"
SRCFILE("batch.c")

void BatchProcess.open()
{
	core = (Core*) new HostCore(this, master);
	char *error = core->open();
	if( error ){
		fprintf(stderr, "%s\n", error);
		exit(1);
	}
	CallStk *c = core->callstack();
	for( long l = c->size-1; l>=0; --l )
		if( l<32 || l>c->size-32 ){
		Frame f = c->frame(l);
		Bls t;
		f.addsymbols();
		printf("%s\n", f.text(t));
		if( f.func ){
			BlkVars bv(f.func->blk(f.pc));
			Var *v;
			while( v = bv.gen() ){
				Bls tt;
				if( v->disc() == U_ARG ) continue;
				Expr *e = E_Sym(v);
				e->evaltext(&f, tt);
				printf("\t%s\n", tt.text);
			}
		}
	}
}

BatchMaster.BatchMaster(char *c, char *a)
{
	new BatchProcess(c, a)->open();
}
