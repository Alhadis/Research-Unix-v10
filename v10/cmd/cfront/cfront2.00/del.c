/*ident	"@(#)ctrans:src/del.c	1.1.3.6" */
/************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T Technologies, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

del.c:

	walk the trees to reclaim storage

**************************************************************/

#include "cfront.h"

void name::del()
{
	Pexpr i = n_initializer;

// printf( "\nname::del: %d %s", this, string?string:"no name" );
//	NFn++;
	DEL(tp);
	if(i && i!=(Pexpr)1) DEL(i);
	n_tbl_list = name_free;
	name_free = this;
}

void type::del()
{
//fprintf(stderr,"DEL(type=%d %d)\n",this,base);
	permanent = 3;	/* do not delete twice */
	switch (base) {
	case TNAME:
	case NAME:
		error('i',"%d->T::del():N %s %d",this,Pname(this)->string,base);
	case FCT:
	{	Pfct f = (Pfct) this;
		DEL(f->returns);
		break;
	}
	case VEC:
	{	Pvec v = (Pvec) this;
		DEL(v->dim);
		DEL(v->typ);
		break;
	}
	case PTR:
	case RPTR:
	{	Pptr p = (Pptr) this;
		DEL(p->typ);
		break;
	}
	}

	delete this;
}

void expr::del()
{
//fprintf(stderr,"DEL(expr=%d: %d %d %d)\n",this,base,e1,e2); fflush(stderr);
	permanent = 3;
	switch (base) {
	case IVAL:
		if (this == one) return;
	case FVAL:
	case THIS:
	case ICON:
	case FCON:
	case CCON:
	case STRING:
	case TEXT:
		goto dd;
	case DUMMY:
	case ZERO:
	case NAME:
		return;
	case CAST:
	case SIZEOF:
	case NEW:
	case GNEW:
	case VALUE:
		DEL(tp2);
		break;
	case REF:
	case DOT:
		DEL(e1);
		if (e2) DEL(e2);
	case MDOT:
		if (mem && mem->base!=NAME) DEL(mem);
		goto dd;
	case QUEST:
		DEL(cond);
		break;
	case ICALL:
		delete il->i_args;
		delete il;
		goto dd;
	}

	DEL(e1);
	DEL(e2);
/*	DEL(tp);*/
dd:
	e1 = expr_free;
	expr_free = this;
//	NFe++;
}

void stmt::del()
{
//fprintf(stderr,"DEL(stmt %d %s)\n",this,keys[base]); fflush(stderr);
	permanent = 3;
	switch (base) {
	case SM:
	case WHILE:
	case DO:
	case RETURN:
	case CASE:
	case SWITCH:
		DEL(e);
		break;
	case PAIR:
		DEL(s2);
		break;
	case BLOCK:
		DEL(d);
		DEL(s);
		if (own_tbl) DEL(memtbl);
		DEL(s_list);
		goto dd;
	case FOR:
		DEL(e);
		DEL(e2);
		DEL(for_init);
		break;
	case IF:
		DEL(e);
		DEL(else_stmt);
		break;
	}

	DEL(s);
	DEL(s_list);
dd:
	s_list = stmt_free;
	stmt_free = this;
//	NFs++;
}

void table::del()
{
	for (register i=1; i<free_slot; i++) {
		Pname n = entries[i];
		if (n==0) error('i',"table.del(0)");

//printf( "\ntable::del: %s n_scope: %d, n_stclass: %d", n->string, n->n_scope, n->n_stclass );

		if (n->n_stclass == STATIC) continue;
		switch (n->n_scope) {
		case ARG:
		case ARGT:
			break;
		default:
		{	char* s = n->string;
			if (s && (s[0]!='_' || s[1]!='_' || s[2]!='X')) delete s;
			/* delete n; */
			n->del();
		}
		}
	}
	delete entries;
	delete hashtbl;
	delete this;
}

// local class
void delete_local() 
{
// error( 'd', "delete_local: vlist: %d", vlist );
    do {
	for (vl* v = vlist; v; v = v->next) v->cl->really_print(v->vt);
	vlist = 0;

	for (Plist l=isf_list; l; l=l->l) {
		Pname n = l->f;
		Pfct f = Pfct(n->tp);
// error('d',"isf %n f %d",n,f);
		if ( f == 0 ) { error('d', "delete_local: f == 0" );break;} 
		if (f->base == OVERLOAD) {
			n = Pgen(f)->fct_list->f;	// first fct
			f = Pfct(n->tp);
		}

		if (debug_opt==0 && n->n_addr_taken) {
			f->f_inline = 0;
			if (n->n_dcl_printed<2) n->dcl_print(0);
		}
	}
	isf_list = 0;
    } while (vlist);

    for ( Plist l = local_class; l; l = l->l )
    {
          Pname n = l->f;
// error( 'd' , "delete_local() %d %n %t", n, n, n->tp );
// printf( "\ndelete_local: %d %s", n, n->string );
    Pname nn = Pbase(n->tp)->b_name;
    Pclass cl = Pclass(nn->tp);
	for (Pname px, p=cl->mem_list; p; p=px) {
		px = p->n_list;
		if (p->tp)
		switch (p->tp->base) {
			case FCT:
			{	Pfct f = (Pfct)p->tp;
				if (f->body) {
 				    if (f->f_inline==0
				    && f->f_imeasure==0) {
					DEL(f->body);
					f->body = 0;
					}
				}
			}
			case COBJ:
			case EOBJ:
				DEL(p);
				break;
			case CLASS:
			case ENUM:
				break;
			default:
				delete p;
			} // end switch
		else delete p;
	} // end for mem
        DEL(cl->memtbl);
	cl->mem_list = 0;
	cl->permanent = 3;
        nn->permanent = 0;
	DEL(nn);
	n->permanent = 0;
	extern void table_delete( char*, TOK, int );
// error( 'd', "local delete: n_key: %k lex_level: %d", n->n_key, n->lex_level );
	table_delete( n->string, LOCAL, n->lex_level );
	DEL(n);
    }
}
