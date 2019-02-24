/*ident	"@(#)ctrans:src/simpl2.c	1.7" */
/******************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

simpl2.c:

	simplify the typechecked function
	remove:		classes:
				class fct-calls
				operators
				value constructors and destructors
			new and delete operators (replace with function calls)
			initializers		(turn them into statements)
			constant expressions		(evaluate them)
			inline functions		(expand the calls)
			enums				(make const ints)
			unreachable code		(delete it)
	make implicit coersions explicit

	in general you cannot simplify something twice

*******************************************************************/

#include "cfront.h"
#include "size.h"
#include <ctype.h>

extern Pname Ntmp;
Pname find_vptr(Pclass);

extern int no_of_returns;

extern Pname new_fct;
//extern Pname del_fct;

extern Pstmt del_list;
extern Pstmt break_del_list;
extern Pstmt continue_del_list;

extern Pname curr_fct;	// current function
extern Pexpr init_list;
extern int imeasure;

Pexpr cdvec(Pname f, Pexpr vec, Pclass cl, Pname cd, int tail, Pexpr i, Pexpr vec2)
/*
	generate a call to construct or destroy the elements of a vector
*/
{
	Pexpr sz = new texpr(SIZEOF,cl,0);	// sizeof elem
	sz->tp = uint_type;
	(void) cl->tsizeof();
					
	Pexpr esz = new texpr(SIZEOF,cl,0);	// noe = sizeof(vec)/sizeof(elem)
	esz->tp = int_type;

Pexpr noe;
if (vec2) noe = new texpr(SIZEOF,vec2->tp,0);
else noe = new texpr(SIZEOF,vec->tp,0);

	// Pexpr noe = new texpr(SIZEOF,vec->tp,0);
	noe->tp = int_type;
	noe = new expr(DIV,noe,esz);
	noe->tp = uint_type;
// error('d',"cdvec tail %d i %d",tail,i);
//	Pexpr arg = (0<=tail) ? new expr(ELIST,zero,0) : 0;	// 0 or 1 for dtors
	Pexpr arg = (i) ? new expr(ELIST,i,0) : 0;
	arg = (0<=tail) ? new expr(ELIST,zero,arg) : arg;	// 0 or 1 for dtors
	arg = new expr(ELIST,cd,arg);		// constructor or destructor
	cd->lval(ADDROF);			// cd->take_addr();

	arg = new expr(ELIST,sz,arg);
	arg = new expr(ELIST,noe,arg);
	arg = new expr(ELIST,vec,arg);

	arg = new call(f,arg);
	arg->base = G_CALL;
	arg->fct_name = f;

	return arg;
}

/*
int new_used;	// pre-define new and delete only if the user didn't

void new_init()
{
	char* ns = oper_name(NEW);
	char* ds = oper_name(DELETE);

	new_used = 1;

	new_fct = gtbl->look(ns,0);
	del_fct = gtbl->look(ds,0);

	if (new_fct && !del_fct)
		error('w',"%n defined but not operator delete()",new_fct);
	if (del_fct && !new_fct)
		error('w',"%n defined but not operator new()",del_fct);

        if (Pfct(new_fct->tp)->body==0) new_fct->dcl_print(0);
        if (Pfct(del_fct->tp)->body==0) del_fct->dcl_print(0);
}
*/
Pstmt trim_tail(Pstmt tt)
/*
	strip off statements after RETURN etc.
	NOT general: used for stripping off spurious destructor calls
*/
{
	if (tt == 0) return 0;

	while (tt->s_list) {
		Pstmt tpx;
		switch (tt->base) {
		case PAIR:
			tpx = trim_tail(tt->s2);
			goto tpxl;
		case BLOCK:
			tpx = trim_tail(tt->s);
		tpxl:
			if (tpx == 0) return 0;

			switch (tpx->base) {
			case SM:
				break;
			case CONTINUE:
			case BREAK:
			case GOTO:
			case RETURN:
				if (tt->s_list->base != LABEL) tt->s_list = 0;
			default:
				return tpx;
			}
		default:
			if (tt = tt->s_list) break;
			return 0;
		case RETURN:
			if (tt->s_list->base != LABEL) tt->s_list = 0;
			return tt;
		}
	}

	switch (tt->base) {
	case PAIR:	return trim_tail(tt->s2);
//	case LABEL:	return trim_tail(tt->s);
	case BLOCK:	if (tt->s) return trim_tail(tt->s);
	default:	return tt;
	}
}

extern Ptype Pfct_type;

Pexpr mptr_assign(Pexpr n, Pexpr in)
{
	Pexpr i1;
	Pexpr i2;
	Pexpr i3;

	if ( n->base == NAME ) 
		Pname(n)->use();

	if (in->base == NAME) {
		i1 = new mdot("d",in);
		i1->i1 = 9;
		i2 = new mdot("i",in);
		i2->i1 = 9;
		i3 = new mdot("f",in);
		i3->i1 = 9;
	}
	else {
		i1 = in->e1->e1;
		i2 = in->e1->e2;
		i3 = in->e2;
	}
	Pexpr nd = new mdot("d",n);
	nd->i1 = 9;
	Pexpr e1 = new expr(ASSIGN,nd,i1);

	Pexpr ni = new mdot("i",n);
	ni->i1 = 9;
	Pexpr e2 = new expr(ASSIGN,ni,i2);

	Pexpr nf = new mdot("f",n);
	nf->i1 = 9;
//	Pexpr ii = in->e2; //new cast(Pfct_type,in->e2);
	Pexpr e3 = new expr(ASSIGN,nf,i3);

	Pexpr ee = new expr(CM,e2,e3);
	return new expr(CM,e1,ee);
}

Pstmt block::simpl()
{
	int i;
	Pname n;
	Pstmt ss=0, sst=0;
	Pstmt dd=0, ddt=0;
	Pstmt stail;
	Ptable old_scope = scope;
	DB( if(Sdebug>=1)
		error('d',"%d->block::simple() own_tbl %d  memtbl %d  curr_fct%n",this,own_tbl,memtbl,curr_fct);
	);
	if (own_tbl == 0) {
		ss = (s) ? s->simpl() : 0;
		return ss;
	}

	scope = memtbl;
	if (scope->init_stat == 0) scope->init_stat = 1; /* table is simplified. */

	for (n=scope->get_mem(i=1); n; n=scope->get_mem(++i)) {
		Pstmt st = 0;
		Pname cln;
		Pexpr in = n->n_initializer;
// error('d',"local %k %n in %k %t",n->n_sto,n,in?in->base:0,in?in->tp:0);
		if (in || n->n_evaluated) {
			scope->init_stat = 2; /* initializer in this scope */
			if (n->n_sto == EXTERN) {
				error(&n->where,"Id local extern%n",n);
				continue;
			}
		}

		switch (n->n_scope) {
		case ARG:
		case 0:
		case PUBLIC:
			 continue;
		}

		if (n->n_stclass == STATIC) { // local static class object
			/* initialization of local static objects; 
			 * set up first pass switch
			 *     temp_switch ? 0 
			 *       : (stat_obj=init_expr, temp_switch=1);
			 *
			 * ARGS: temporary class object in init_expr with dtor
			 *       must call dtor once in std function
			 */

			if ((in==0 && n->n_scope==ARGS) ||
				(in && in->base==STAT_INIT)) 
			{
				Pname cn;
				Pname x;
				Ptype ct;
				int vec_seen = 0;

				cn = n->tp->is_cl_obj();
				if ( cn == 0 ) { ++vec_seen; cn = cl_obj_vec; }

				if ( cn ) {
    					ct = new ptr(PTR,vec_seen?Pvec(n->tp)->typ:n->tp);
					x = make_tmp('F', ct, gtbl );
					x->n_initializer = zero;
				}
				else
					x = make_tmp('F',int_type,scope);

				x->n_sto = n->n_stclass = STATIC;
				
				if (in) {
					if (in->e2)
						in->base = ASSIGN;
					else in = in->e1;
				}

				Pexpr set;
				if ( cn ) {
					x->dcl_print(0);
					Pclass cl = Pclass(cn->tp);
					Pname dtor = cl->has_dtor();

					Pexpr cc;
					if ( dtor ) {
						if ( vec_seen == 0 ) {
							Pexpr eee = new expr(DEREF, x, 0 );
    							Pexpr c = call_dtor(eee,dtor,0,DOT,one);
    							c->tp = any_type;
    							cc = new expr( QUEST, c, zero );
    							cc->cond = x;
						}
						else cc = cdvec(vec_del_fct,x,cl,dtor,0,zero,n);
						cc->tp = any_type; // arghh!

    						Pstmt dls = new estmt( SM, n->where, cc, 0 );
    						if ( st_dlist ) dls->s_list = st_dlist;
    						st_dlist = dls;
					}

					Pexpr xe;
					if (cn) {
						if ( vec_seen == 0 )
							xe = new expr( G_ADDROF, 0, n );
						else {
							Pexpr ee = new expr( DEREF, n, zero );
							xe = new expr( G_ADDROF, 0, ee );
						}
					}

					set = new expr(ASSIGN,x,xe);
					set->tp = ct;
				}
				else {
					set = new expr(ASSIGN,x,one);
					set->tp = int_type;
				}

				// set pointer to static object and continue
				// sorry for goto, but beats rewriting code
				if ( n->n_scope == ARGS && in == 0 ) {
					set = new expr( QUEST, zero, set );
					set->cond = x;
					st = new estmt(SM,n->where,set,0);
					goto init_stat3;
				}

				in = new expr(G_CM,in,set);
				in = new expr(STAT_INIT,zero,in);
				in->cond = x;
			}
			else
				continue;
		}

    		if ( in ) {
	 		if ((in->base == ILIST && in->e2 == 0) ||
             		    (in->base == STRING && n->tp->base == VEC))
                  		if (ansi_opt==0) {
                      		    	error('s',&n->where,"initialization of%n (automatic aggregate)",n);
                      		    	continue;
                   		}
    		}    

		if (n->tp == 0) continue; /* label */
		if (n->n_evaluated) continue;

		/* construction and destruction of temporaries is handled locally */
		{	char* s = n->string;
			register char c3 = s[4];
			if (s[0]=='_' && s[1]=='_' && s[2]=='D' && isdigit(c3)) continue;
		}

		if ( cln=n->tp->is_cl_obj() ) {
			Pclass cl = Pclass(cln->tp);
			Pname d = cl->has_dtor();

			if ( n->n_stclass == STATIC   // local static class object
				&& in && in->base==STAT_INIT )  
					goto stat_init;

			if (d) {			// n->cl.dtor(0);
				Pexpr dl = call_dtor(n,d,0,DOT,one);
			//	Pstmt dls = new estmt(SM,n->where,dl,0);
				Pstmt dls = new estmt(SM,no_where,dl,0);
				if (dd) {
					dls->s_list = dd;
					dd = dls;
				}
				else
					ddt = dd = dls;
			}

//  error('d',"%n: in %d",n,in?in->base:0);
			if (in) {
				switch (in->base) {
				case DEREF:		// *constructor?
					if (in->e1->base == G_CALL) {
						Pname fn = in->e1->fct_name;
						if (fn==0 || fn->n_oper!=CTOR) goto ddd;
						st = new estmt(SM,n->where,in->e1,0);
						n->n_initializer = 0;
						break;
					}
					goto ddd;
				case STAT_INIT:
stat_init:
// error('d', "block::simpl: case #1 stat_init : n: %n", n );
					in->base = QUEST;
					st = new estmt(SM,n->where,in,0);
					n->n_initializer = 0;
					break;
				case G_CM:
					st = new estmt(SM,n->where,in->e1,0);
					n->n_initializer = 0;
					break;
				case ASSIGN:		// assignment to "n"?
					if (in->e1 == n) {
						st = new estmt(SM,n->where,in,0);
						n->n_initializer = 0;
						break;
					}
				default:
					goto ddd;
				}
			}
		}
		else if (cl_obj_vec) {
			Pclass cl = Pclass(cl_obj_vec->tp);
			Pname d = cl->has_dtor();
			Pname c = cl->has_ictor();
			n->n_initializer = 0;

			if ( n->n_stclass == STATIC   // local static class object
				&& in && in->base==STAT_INIT )  
					goto stat_init2;

			if (c) {	//  _vec_new(vec,noe,sz,ctor);
				if (in==0 || in->base==ILIST) {
					Pexpr a = cdvec(vec_new_fct,n,cl,c,-1,0);
					st = new estmt(SM,n->where,a,0);
				}
				else 
					st = new estmt(SM,n->where,in,0);
			}
			// no default ctor but provided all elements with argument
			else if ( in ) st = new estmt(SM,n->where,in,0);

			
			if (d) {	//  __vec_delete(vec,noe,sz,dtor,0);
				Pfct f = Pfct(d->tp);
				int i = 0;
				for (Pname nn = f->f_args->n_list; 
					nn && nn->n_list; nn=nn->n_list) i++;
				Pexpr a = cdvec(vec_del_fct,n,cl,d,0,new ival(i));
			//	Pstmt dls = new estmt(SM,n->where,a,0);
				Pstmt dls = new estmt(SM,no_where,a,0);
				if (dd) {
					dls->s_list = dd;
					dd = dls;
				}
				else
					ddt = dd = dls;
			}
		}
		else if (in) {
			switch (in->base) {
			case ILIST:
				switch (n->n_scope) {
				case FCT:
					if (in->e2) { // pointer to member
						Pexpr ee = mptr_assign(n,in);
						st = new estmt(SM,n->where,ee,0);
						n->n_initializer = 0;
						break;
					}
				case ARG:
					if (ansi_opt == 0) error('s',"Ir list for localV%n",n);	
				}
				break;
			case STAT_INIT:
stat_init2:
// error('d', "block::simpl: case #2 stat_init : n: %n", n );
				in->base = QUEST;
				st = new estmt(SM,n->where,in,0);
				n->n_initializer = 0;
				break;
			case STRING:
				if (n->tp->base==VEC) break; /* BUG char vec only */
			default:
			ddd:
			{	Pexpr ee = new expr(ASSIGN,n,in);
				st = new estmt(SM,n->where,ee,0);
				n->n_initializer = 0;
			}
			}
		}

init_stat3:	if (st) {
			if (ss)
				sst->s_list = st;
			else
				ss = st;
			sst = st;
		}
	}

	if (dd) {
		Pstmt od = del_list;
		Pstmt obd = break_del_list;
		Pstmt ocd = continue_del_list;

		dd->simpl();
		del_list = (od) ? Pstmt(new pair(curloc,dd,od)) : dd;
		break_del_list = (break_del_list&&obd) ? Pstmt(new pair(curloc,dd,obd)) : dd;
		continue_del_list = (continue_del_list&&ocd) ? Pstmt(new pair(curloc,dd,ocd)) : dd;

		stail  = s ? s->simpl() : 0;

		Pfct f = Pfct(curr_fct->tp);
		if (this!=f->body
		|| f->returns->base==VOID
		|| (f->returns->base!=VOID && no_of_returns==0 ) // you have been warned!
		|| strcmp(curr_fct->string,"main")==0 ) {
		// not dropping through the bottom of a value returning function
			if (stail) {
				Pstmt tt = (stail->base==RETURN || stail->base==LABEL) ? stail : trim_tail(stail);
				if (tt && tt->base != RETURN) stail->s_list = dd;
			}
			else
				s = dd;
			stail = ddt;
		}

		del_list = od;
		continue_del_list = ocd;
		break_del_list = obd;
	}
	else
		stail  = s ? s->simpl() : 0;

	if (ss) {	/* place constructor calls */
		ss->simpl();
		sst->s_list = s;
		s = ss;
		if (stail == 0) stail = sst;
	}
	
	scope = old_scope;

	return stail;
}

int no_sizeof;

void expr::simpl()
{
	DB(if(Sdebug>=2){
		error('d',"%d->expr::simpl() %k",this,this?base:0);
		if(Sdebug>=3) display_expr(this);
	});
	if (this==0 || permanent==2) return;	// already expanded
	static TOK obase = 0;

	switch (base) {
	case MDOT:
		obase = base;
		mem->simpl();
		obase = 0;
		// no break

	case ICALL:	// already expanded
		return;

	case G_ADDROF:
	case ADDROF:
// error('d',"simpl & %k",e2->base);

		e2->simpl();
		switch (e2->base) {
		case DOT:
		case REF:
		{	Pref r = Pref(e2);
			Pname m = Pname(r->mem);
			while (m->base == MDOT) m = Pname(m->mem);
			if (m->n_stclass == STATIC) {	// & static member
				Pexpr x;
			delp:
				x = e2;
				e2 = m;
				r->mem = 0;
				DEL(x);
			}
			else if (m->tp->base == FCT) {	// & member fct
				Pfct f = Pfct(m->tp);
				if (f->f_virtual) {	// &p->f ==> p->vtbl[fi].f
					int index = f->f_virtual;
					Pexpr ie = index ? new ival(index):0;
					if (ie) ie->tp = int_type;
					Pname cn = m->n_table->t_name;
					Pname vp = find_vptr(Pclass(cn->tp));
					r->mem = vp;

					if ( obase == MDOT ) {
						base = DEREF;
						e1 = e2;
						e2 = ie;
					}
					else { // support old style &b.vf
						base = MDOT;
						mem = new expr(DEREF,e2,ie);
						string2 = "f";
						i1 = 9;
					}
				}
				else {
					goto delp;
				}
			}
			break;
		}
		}
		break;
	
	case ANDAND:
	case OROR:
		Ntmp = 0;
		// no break

	default:
		if (e1) e1->simpl();
		if (e2) e2->simpl();
		break;

	case CM:
	case G_CM:
	{	
		Pname n = 0;
		e1->simpl();
		e2->simpl();
		if (e1->base==ICALL && e1->e1==0) n = e1->il->fct_name;
 		if (e2->base==ICALL && e2->e1==0) n = e2->il->fct_name;
		if (n) error('s',"cannot expand inline void%n called in commaE",n);
		// no break
	}
	case NAME:
	case DUMMY:
	case ICON:
	case FCON:
	case CCON:
	case IVAL:
	// case FVAL:
	// case LVAL:
	case STRING:
	case ZERO:
	case ILIST:
//	case MDOT:
		return;
/*
	case SIZEOF:
		base = IVAL;
		i1 = tp2->tsizeof();
		tp2 = 0;	// can't DEL(tp2)
		break;
*/
	case SIZEOF:
		if (e1) e1->simpl();
		return;

	case G_CALL:
	case CALL:
		Pcall(this)->simpl();
		break;

	case NEW:
	case GNEW:
		simpl_new();
		return;

	case DELETE:
	case GDELETE:
		simpl_delete();
		break;

	case QUEST:
		cond->simpl();
		Ntmp = 0;
		e2->simpl();
		// no break

	case CAST:
	case REF:
		e1->simpl();
		break;

	case DOT:
		e1->simpl();
		switch (e1->base) {
		case CM:
		case G_CM:
		 {	// &( , name). => ( ... , &name)->
			Pexpr ex = e1;
			cfr:
			switch (ex->e2->base) {
			case NAME:
				base = REF;
				ex->e2 = ex->e2->address();
				break;
			case CM:
			case G_CM:
				ex = ex->e2;
				goto cfr;
			}
		}
		}
		break;

	case ASSIGN:
	{	
		Pfct f = 0; 
		Pexpr th = 0;
		if ( curr_fct ) {
			f = Pfct(curr_fct->tp);
			th = f->f_this;
		}

		imeasure++;
		if (e1) e1->simpl();
		if (e2) {
			Pexpr c = e2;
			c->simpl();
			while (c->base == CAST) c = c->e1;
			if (c->base == ILIST) e2 = c;

			if (e2->base == ILIST) {	// pointer to member assignment
				Pexpr ee = mptr_assign(e1,e2);
				Pexpr eee = new expr(CM,ee->e2,e1);
				e1 = ee->e1;
				e2 = eee;
				base = CM;
				delete ee;
			}
		}

		if (th && th==e1 && curr_fct->n_oper==CTOR && init_list) {
			// this=e2 => (this=e2,init_list)
			Pclass cl = Pclass(Pbase(Pptr(th->tp)->typ)->b_name->tp);
			if (cl->c_body == 1) cl->dcl_print(0);
			imeasure++;
			base = CM;
			e1 = new expr(ASSIGN,e1,e2);
			e2 = init_list;
			if (warning_opt)	// timid
				error('w',"assignment to ``this'' inK: try defining%t::operator new() instead",f->memof);
		}
		break;
	}
	}

	switch (base) {
	case QUEST:
	case ANDAND:
	case OROR:
		if (Ntmp) error('s',"temporary ofC%n with destructor needed in%kE",Ntmp,base);
		// no break;
	default:
		Ntmp = 0;
	}
	
	if (tp==int_type || tp==defa_type) {
		Neval = 0;
		no_sizeof = 1;	// do not convert sizeof's to ints
		long i = eval();
		no_sizeof = 0;
		if (Neval == 0) {
			base = IVAL;
			i1 = i;
		}
	}

}

Pexpr vptr_entry(Pexpr pp, Pexpr ie, Pclass cl)
{
	Pptr ttemp = pp->tp->is_ptr_or_ref();

        if (ttemp) {
		Ptype pt = ttemp->typ;  // check if cast pp = (base*)pp
        	Pclass pc = Pclass(pt->is_cl_obj()->tp); // is needed
        	if (pc!=cl) pp = new cast(cl,pp);
        }
	Pname vp = find_vptr(cl);
	if (vp == 0) error('i',"can't find vptr");
	Pexpr vptr = new ref(REF,pp,vp);	// pp->vptr
	return new expr(DEREF,vptr,ie);	// pp->vptr[i]
}

Pexpr new_this(Pexpr pp, Pexpr ee)
{
//error('d',"new this");
	Pexpr dee =  new mdot("d",ee);		// pp->vptr[i].d
	dee->i1 = 9;
	Pexpr nthis = new cast(Pchar_type,pp);
	nthis = new expr(PLUS,nthis,dee);	// ((char*)pp)+delta
	Ptype ct = pp->tp;
	if (pp->base==NAME && Pname(pp)->n_xref) ct = pp->tp->addrof();
	return new cast(ct,nthis);
}

Pcall vcall(Pexpr pp, Pexpr ie, Pfct f, Pclass cl, Pexpr args)
/*
	generate a call of the virtual function with the index ``ie''
	and type "f" in class ``cl'' for the object pointed to by ``pp''

	multiple inheritance virtual call:

	p->f(x) is resolved like this
	pp = p;			// avoid side effects
	pp = p.base_object;	// often: pp = p;
				// done when the name was resolved
	i = index(f);
	entry = pp->_vtbl[i-1];
	pp = (T*)(((char*)pp)+entry.d)
	(*(ftype)entry.f)(pp,x)
*/
{
//error('d',"vcall %t",cl);
 	if (cl->c_body==1) cl->dcl_print(0);	// look for first use of cl
		
	imeasure+=6;
	Pexpr ee = vptr_entry(pp,ie,cl);

	Pexpr fee = new mdot("f",ee);
	fee->i1 = 9;
	Ptype pft = f->addrof();
	fee = new cast(pft,fee);		// (T)pp->vptr[i].f
	Pexpr r = new expr(DEREF,fee,0);	// *(T)pp->vptr[i].f
						// e1->tp must be 0, means "argtype encoded"
	r->tp2 = Ptype(f->f_this);		// encode argtype

	Pexpr nthis = new_this(pp,ee);

	args = new expr(ELIST,nthis,args);
	args->simpl();

	Pcall c = new call(r,args);
	c->tp = f->returns;
	return c;
}

void call::simpl()
/*
	fix member function calls:
		p->f(x) becomes f(p,x)
		o.f(x)  becomes f(&o,x)
	or if f is virtual:
		p->f(x) is resolved like this
			pp = p;
			i = index(f);
			entry = pp->_vtbl[i-1];
			pp = (T*)(((char*)pp)+entry.i)
			(*entry.f)(pp,x)
	replace calls to inline functions by the expanded code
*/
{
	Pname fn = fct_name;
//error('d',"%d call::simpl() fn %n %d e1 %d",this,fn,fn,e1);
	Pfct f = fn ? Pfct(fn->tp) : 0;

	if (fn == 0) e1->simpl();

	if (f) {
		switch(f->base) {
		case ANY:
			return;
		case OVERLOAD:
			fct_name = fn = Pgen(f)->fct_list->f;
			f = Pfct(fn->tp);
		}
	}

	switch (e1->base) {
	case MEMPTR:	// (p ->* q)(args)
	{	
		Pexpr p = e1->e1;
		Pexpr q = e1->e2;
		Pclass cl = Pclass(e1->tp2);
		Pfct f = Pfct(q->tp->deref());

		if (e2) e2->simpl();

		if (f->f_this == 0) {	// might not know about ``this'' yet
			if (f->memof == 0) error('i',"memof missing");
			Pname tt = new name("this");
			tt->n_scope = ARG;
			tt->tp = f->memof->this_type;
			PERM(tt);
		//	f->f_this = f->f_args = tt;
			tt->n_list = f->argtype;
		//	f->f_this = tt;
			tt->n_list = f->f_result ? f->f_result : f->argtype;
			f->f_this = f->f_args = tt;
		}
//error('d',"f_this %d",f->f_this);	
extern has_virt(Pclass);

		// beware of sideeffects:
		nin = 1;
		if (q->not_simple()) error('s',"2nd operand of .* too complicated");
		nin = 0;

		Pexpr qq = new mdot("f",q);	// the function: (*(right type)q.f)
		qq->i1 = 9;
		qq = new cast(f->addrof(),qq);
		Pexpr nc = new expr(DEREF,qq,0);
		nc->tp2 = Ptype(f->f_this);	// encode argtype

		Pexpr nthis = new_this(p,q);	// arguments: (p+q.d,args)
		Pexpr args = new expr(ELIST,nthis,e2);
		imeasure+=3;

		if (has_virt(cl) == 0) {	// no virtuals: simple
			if (cl->defined == 0) 
				error("call throughP toMF before definition ofC %t",cl);
//error('d',"no virt");
			// (p ->* q)(args) => (*q.f)(p+q.d,args)
			e1 = nc;
			e2 = args;
			return;
		}
		
		if (find_vptr(cl) == 0) {	// must be a call to a second base
						// that we cannot handle yet
						// it is OK not to to generate
						// a virtual call since a
						// `sorry' will have been generated
						// at the point of initialization
			e1 = nc;
			e2 = args;
			return;

		}
		// beware of sideeffects:
		nin = 1;
		if (p->not_simple()) error('s',"1st operand of .* too complicated");
		nin = 0;

		Pexpr c = new mdot("i",q);	// condition (q.i<0)
		c->i1 = 9;
		c = new expr(LT,c,zero);
		
		Pexpr ie = new mdot("i",q);
		ie->i1 = 9;

		base = QUEST;
		e1 = new call(nc,args);
		e2 = vcall(p,ie,f,cl,e2);
		cond = c;
		return;
	}
	case DOT:
		// if e1 is an object and not just a reference
		// the vtbl need not be used
	case REF:
	{	Pref r = Pref(e1);
		Pexpr a1 = r->e1;
		int obj = r->n_initializer!=0;	// if B::f don't use vcall

		if (obj == 0) {	// don't use vcall if we have an object
				// (not a pointer or a reference)
			if (e1->base==DOT && a1->base!=DEREF) obj = 1;
		}

//error('d',"fct_name %n f %d %d obj %d",fct_name,f,f->f_virtual,obj);
		if (f && obj==0 && f->f_virtual) {
			Pexpr a11 = 0;

			switch(a1->base) {	// see if temporary might be needed
			case NAME:
			case MDOT:
				a11 = a1;
				break;
			case REF:
			case DOT:
				if (a1->e1->base==NAME
				|| ((a1->e1->base==DOT || a1->e1->base==REF) && a1->e1->e1->base==NAME)) a11 = a1;
				break;
			case ADDROF:
			case G_ADDROF:
				if (a1->e2->base == NAME
				|| ((a1->e2->base==DOT || a1->e2->base==REF) && a1->e2->e1->base==NAME)) a11 = a1;
				break;
			case CAST:
				switch (a1->e1->base) {
				case NAME:
				case MDOT:
					a11 = a1;
				}
			}

//		   	if( a1->base==REF && fn->n_oper==DTOR ){
//				a11 = a1;
//				goto zsw;
//		   	}
//
			if (e1->base == DOT) {
//			zsw:
				if (a11) a11 = a11->address();
				a1 = a1->address();
			}
			
			if (a11 == 0) {	// temporary (maybe) needed
				   	// e->f() => (t=e,t->f(t))
				if (a1->base==NAME)
					a11 = a1;	// &*name has become name
				else {
					Pname nx = new name(make_name('K'));
					nx->tp = a1->tp;
					Pname n = nx->dcl(scope,ARG); // no init!
					delete nx;
					Pname cln = a1->tp->is_cl_obj();
					if (cln) {
						Pclass cl = Pclass(cln->tp);
						if (Ntmp==0 && cl->has_dtor()) Ntmp = cln;
						if (cl->has_itor()) n->n_xref = 1;
					}
					n->n_scope = FCT;
					n->assign();
					a11 = n;
					a1 = new expr(ASSIGN,n,a1);
					a1->tp = n->tp;
					a1->simpl();
					Pcall cc = new call(0,0);
					*cc = *this;
					base = CM;
					e1 = a1;
					e2 = cc;
					this = cc;
				}
			}

			int i = f->f_virtual;
			Pexpr ie = i?new ival(i):0;	// index
			Pname cn = fn->n_table->t_name;
			if (fn
			&& fn->n_initializer
			&& cc->nof
			&& cc->nof->n_oper==CTOR
			&& Pfct(cc->nof->tp)->memof->c_abstract
			&& strcmp(Pfct(cc->nof->tp)->memof->string, cn->string) == 0 ) {
				// permit x::x( x& xx ) { xx.pvf(); }
				Pexpr ee = e1->e1;
				while ( ee && ee->base != NAME ) ee = ee->e1;
				if ( ee && strcmp( ee->string, "this" )==0)
					error("call of pure virtualF%n inK%n",fn,cc->nof);
			}
			Pcall vc = vcall(a11,ie,f,Pclass(cn->tp),e2);
			*this = *vc;
			return;	
		}
	
		Ptype tt = r->mem->tp;
	llp:
//error('d',"llp %t",tt);
		switch (tt->base) {
		// default:	// pointer to function: (n->ptr_mem)(args); do nothing
		case TYPE:
			tt = Pbase(tt)->b_name->tp;
			goto llp;
		case OVERLOAD:	// n->fctmem(args);
		case FCT:
			if (fct_name==0) {
				// reconstitute fn destroyed to suppress "virtual" 
				fct_name = fn = Pname(e1->n_initializer);
				f = Pfct(fn->tp);
			}

			if (e1->base == DOT) a1 = a1->address();
			e2 = new expr(ELIST,a1,e2);
			e1 = r->mem;
		}
	}
	}

	if (e2) e2->simpl();
//error('d',"fn %n inl %d imes %d",fn,f->f_inline,f->f_imeasure);
	if (fn && f->f_inline && debug_opt==0) {
		imeasure += f->f_imeasure;
		Pclass cl = f->memof;
		if (cl && cl->c_body) cl->dcl_print(0);
		Ptable oscope = scope;
		Pexpr ee = f->expand(fn,scope,e2);
		scope = oscope;
		if (ee)	*Pexpr(this) = *ee;
	}
	else if (fn && f->f_inline==0 && f->f_imeasure) {
		extern void uninline(Pname fn);
		uninline(fn);
		imeasure += 3;
	}
	else if (fn && debug_opt && f->f_inline==ITOR) {
		extern void expand_itor(Pclass);
		expand_itor(f->memof);
	}
	else
		imeasure += 3;
}

void uninline(Pname fn)
	// inline turned static
{
	Pfct f = Pfct(fn->tp);
//error('d',"uninline %n %d %d",fn,f->body,f->f_expr);

	if (warning_opt) {
		error('w',"%n too complex for inlining",fn);
		error('w',"out-of-line copy of %n created",fn);
	}
	f->f_imeasure = 0;	// now it really is just static

	Pstmt s = f->body->s;
//	for (s = f->body->s; s; s=s->s_list)
//error('d',"start %d %k",s->e,s->e->base);
//	 s = f->body->s;
	while (s) {
//error('d',"s   %k %d %k",s->base,s->e,s->e->base);
		if (s->base == SM) {
			// turn comma expression into statement list
			Pexpr e = s->e;
			if (e)
				switch (e->base) {
				case CM:
				case G_CM:
				{	Pstmt ss = new estmt(SM,no_where,e->e2,0);
					s->e = e->e1;
					ss->s_list = s->s_list;
					s->s_list = ss;
					delete e;
					continue;
				}
				}		
		}
		s = s->s_list;
	}
//	for (s = f->body->s; s; s=s->s_list)
//error('d',"echo %k %d %k",s->base,s->e,s->e->base);

	fn->dcl_print(0);
}

/*
void ccheck(Pexpr e)

	 Is there a conditional in this expression? (not perfect)

{
//error('d',"ccheck(e %k)",e,e?e->base,0);
	if (e)
	switch (e->base) {
	case QUEST:
	case ANDAND:
	case OROR:
		error('s',"E too complicated: uses%k and needs temporary ofCW destructor",e->base);
		break;
	case LT:
	case LE:
	case GT:
	case GE:
	case EQ:
	case NE:
	case ASSIGN:
	case ASPLUS:
	case ASMINUS:
	case G_CM:
	case CM:
	case PLUS:
	case MINUS:
	case MUL:
	case DIV:
	case OR:
	case ER:
	case AND:
	case G_CALL:
	case CALL:
	case ELIST:
	case DEREF:
		ccheck(e->e1);
	case NOT:
	case COMPL:
	case CAST:
	case ADDROF:
	case G_ADDROF:
		ccheck(e->e2);
		break;
	case ICALL:	// check inlined arguments
	{	Pin il = e->il;
		for (int i = 0; il->args[i].arg && i<il->i_slots; i++) ccheck(il->args[i].arg);
	}
	}
}
*/

void temp_in_cond(Pexpr ee, Pstmt ss, Ptable tbl)
/*
	insert destructor calls 'ss' into condition 'ee'
	ee => (Qnn = ee, dtors, Qnn)
*/
{
//error('d',"temp_in_cond");
//	ccheck(ee);
	while (ee->base==CM || ee->base==G_CM) ee = ee->e2;
	Ptype ct = ee->tp;
	Pname n = new name(make_name('Q'));	// int Qnn;
	n->tp = ct;
	Pname tmp = n->dcl(tbl,ARG);
	delete n;
	tmp->n_scope = FCT;

	Pexpr v = new expr(0,0,0);
	*v = *ee;
	PERM(ct);
	v = new cast(ct,v);

	tmp->n_assigned_to = 1;
	Pexpr c = new expr(ASSIGN,tmp,v);	// Qnn = ee
	c->tp = ct;
	ee->base = CM;
	ee->e1 = c;

	Pexpr ex = 0;				// add dtors at end

	for (Pstmt sx = ss; sx; sx = sx->s_list) {
		if (ex) {
			ex = new expr(CM,ex,sx->e);
			ex->tp = sx->e->tp;
		}
		else
			ex = sx->e;
	}
	ee->e2 = new expr(CM,ex,tmp);	// add Qnn at end
	ee->e2->tp = ct;
}

bit not_safe(Pexpr e)
{

	switch (e->base) {
	default:
		return 1;
/*
	case CALL:
	case G_CALL:
	case DOT:
	case REF:
	case ANAME:
		return 1;
*/
	case NAME:
		// if the name is automatic and has a destructor it is not safe
		// to destroy it before returning an expression depending on it
	{	Pname n = Pname(e);
		if (n->n_table!=gtbl && n->n_table->t_name==0) {
			Pname cn = n->tp->is_cl_obj();
			if (cn && Pclass(cn->tp)->has_dtor()) return 1;
		}
	}
	case IVAL:
	case ICON:
	case CCON:
	case FCON:
	case STRING:
		return 0;
	case NOT:
	case COMPL:
	case ADDROF:
	case G_ADDROF:
		return not_safe(e->e2);
	case DEREF:
	//	return not_safe(e->e1) || e->e2?not_safe(e->e2):0;
	{	int i = not_safe(e->e1);
		if (i) return i;
		if (e->e2) return not_safe(e->e2);
		return 0;
	}
	case CM:
	case PLUS:
	case MINUS:
	case MUL:
	case DIV:
	case MOD:
	case ASSIGN:
	case ASPLUS:
	case ASMINUS:
	case ASMUL:
	case ASDIV:
	case OR:
	case AND:
	case OROR:
	case ANDAND:
	case LT:
	case LE:
	case GT:
	case GE:
	case EQ:
	case NE:
		return not_safe(e->e1) || not_safe(e->e2);
	case QUEST:
		return not_safe(e->cond) || not_safe(e->e1) || not_safe(e->e2);
	}
}
		
	
Pexpr curr_expr;	/* to protect against an inline being expanded twice
			   in a simple expression keep track of expressions
			   being simplified
			*/
Pstmt stmt::simpl()
/*
	return a pointer to the last statement in the list, or 0
*/
{
	if (this == 0) error('i',"0->S::simpl()");
	DB( if(Sdebug>=1){
		error('d',"%d->stmt::simpl():  %k",this,base);
		if(Sdebug>=2) display_stmt(this);
	});
	//Pstmt ostmt = Cstmt;
	//if ( where.line ) Cstmt = this;

	stmtno++;
	curr_expr = e;
//error('d',"stmt::simpl %k s_list %d",base,s_list);

	switch (base) {
	default:
		error('i',"S::simpl(%k)",base);

	case ASM:
		break;

	case BREAK:
		if (break_del_list) {	// break => { _dtor()s; break; }
			Pstmt bs = new stmt(base,where,0);
			Pstmt dl = break_del_list->copy();
			base = BLOCK;
			s = new pair(where,dl,bs);
		}
		break;

	case CONTINUE:
		if (continue_del_list) { // continue => { _dtor()s; continue; }
			Pstmt bs = new stmt(base,where,0);
			Pstmt dl = continue_del_list->copy();
			base = BLOCK;
			s = new pair(where,dl,bs);
		}
		break;

	case DEFAULT:
		s->simpl();
		break;

	case SM:
		if (e) {
			if (e->base == DEREF) e = e->e1;
			e->simpl();
			if (e->base == DEREF) e = e->e1;
		}
		break;

	case RETURN:
	{	/*	return x;
			=>
				{ dtor()s; return x; }
			OR (returning an X where X(X&) is defined) =>
				{ ctor(_result,x); _dtor()s; return; }
			OR (where x needs temporaries)
			OR (where x might involve an object to be destroyed) =>
				{ _result = x; _dtor()s;  return _result; }
			return;		=>
				{ _dtor()s; return; }
			OR (in constructors) =>
				{ _dtor()s; return _this; }
		*/
		Pstmt sx = this;
		Pexpr ex = e;

		no_of_returns++;

		Pstmt dl = (del_list) ? del_list->copy() : 0;
		Pfct f = Pfct(curr_fct->tp);

		if (e == 0) e = dummy;
		if (e==dummy && curr_fct->n_oper==CTOR) e = f->f_this;


		//      need to generate a temporary for mptr return
		Pexpr tt = e;
		while ( tt->base == CAST )
			tt = tt->e1;
		if ( tt->base == ILIST )
			e = tt;

		if (e->base == ILIST) {
			extern Pbase mptr_type;
			extern Ptype Pvptr_type;
			Pexpr mptr_assign(Pexpr, Pexpr);
			// memptr constant
			// return({1,2,f}) ==> memptr t; return((t={1,2,f},&t))

			Ptable ftbl = Pfct(curr_fct->tp)->body->memtbl;
			Pname temp = make_tmp('A',mptr_type,ftbl);

			// placed in mptr_assign()
			// temp->use(); // necessary for inlines to force declaration

			e = mptr_assign(temp,e);
			e = new expr(G_CM,e,temp);
			e->tp = mptr_type;
		}

		if (f->f_result) {	// ctor(_result,x); dtors; return;
			if (e->base == G_CM) e = replace_temp(e,f->f_result);
			e->simpl();
			Pstmt cs = new estmt(SM,where,e,0);
			if (dl)	cs = new pair(where,cs,dl);
			base = PAIR;
			s = cs;
			s2 = new estmt(RETURN,where,0,0);
//#ifdef RETBUG
//			s2->empty = 1; // fudge to bypass C bug (see print.c)
//			s2->ret_tp = ret_tp;
//#endif
		}
		else {			// dtors; return e;
			e->simpl();
			if (dl) {
				if (e!=dummy && not_safe(e)) {
				// { _result = x; _dtor()s;  return _result; }
					Ptable ftbl = Pfct(curr_fct->tp)->body->memtbl;
					
					Pname r = ftbl->look("_result",0);
					if (r == 0) {
						r = new name("_result");
						r->tp = ret_tp;
						Pname rn = r->dcl(ftbl,ARG);
						rn->n_scope = FCT;
						rn->where = no_where;
						rn->assign();
						delete r;
						r = rn;
					}
					Pexpr as = new expr(ASSIGN,r,e);
					as->tp = ret_tp;	// wrong if = overloaded, but then X(X&) ought to have been used 
					Pstmt cs = new estmt(SM,where,as,0);
					cs = new pair(where,cs,dl);
					base = PAIR;
					s = cs;
					s2 = new estmt(RETURN,where,r,0);
				//	s2->ret_tp = ret_tp;
				}
				else { // { _dtor()s;  return x; }
					base = PAIR;
					s = dl;
					s2 = new estmt(RETURN,where,e,0);
				}
                                 s2->ret_tp = ret_tp;
			}
		}

//		if (sx->memtbl) {
//			int i;
//			for (Pname n=sx->memtbl->get_mem(i=1); n; n=sx->memtbl->get_mem(++i)) {
//				Pname cn = n->tp->is_cl_obj();
//				if (cn && Pclass(cn->tp)->has_dtor()) {
//					ccheck(ex);
//					break;
//				}
//			}
//		}
		break;
	}

	case WHILE:
	case DO:
		e->simpl();
		{	Pstmt obl = break_del_list;
			Pstmt ocl = continue_del_list;
			break_del_list = 0;
			continue_del_list = 0;
			s->simpl();
			break_del_list = obl;
			continue_del_list = ocl;
		}
		break;

	case SWITCH:
		e->simpl();
		{	Pstmt obl = break_del_list;
			break_del_list = 0;
			s->simpl();
			break_del_list = obl;
		}
		switch (s->base) {
		case SM: 
			// permit switch (expr) ;
			if (s->e) goto df;
		case DEFAULT:
		case LABEL:
		case CASE:
			break;
		case BLOCK:
			if (s->s)
			switch (s->s->base) {
			case BREAK:	// to cope with #define Case break; case
			case CASE:
			case LABEL:
			case DEFAULT:
				break;
			default:
				goto df;
			}
			break;
		default:
		df:
			error(&s->where,"S orIdE not reached: (case label missing?)");
		}
		break;

	case CASE:
		e->simpl();
		s->simpl();
		break;

	case LABEL:
		if (del_list) error('s',"label in blockW destructors");
		s->simpl();
		break;

	case GOTO:
		/*	If the goto is going to a different (effective) scope,
			then it is necessary to activate all relevant destructors
			on the way out of nested scopes, and issue errors if there
			are any constructors on the way into the target.

			Only bother if the goto and label have different effective
			scopes. (If mem table of goto == mem table of label, then
			they're in the same scope for all practical purposes.
		*/
		{
		Pname n = scope->look( d->string, LABEL );
		if (n == 0) error('i',&where,"label%n missing",d);
		if(n->n_realscope!=scope && n->n_assigned_to) {

			/*	Find the root of the smallest subtree containing
				the path of the goto.  This algorithm is quadratic
				only if the goto is to an inner or unrelated scope.
			*/

			Ptable r = 0;

			for(Ptable q=n->n_realscope; q!=gtbl; q=q->next) {
				for( Ptable p = scope; p != gtbl; p = p->next ) {
					if( p==q ) {
						r = p;	// found root of subtree!
						goto xyzzy;
					}
				}
			}

xyzzy:			if( r==0 ) error( 'i',&where,"finding root of subtree" );

			/* At this point, r = root of subtree, n->n_realscope
			 * = mem table of label, and scope = mem table of goto. */

			/* Climb the tree from the label mem table to the table
			 * preceding the root of the subtree, looking for
			 * initializers and ctors.  If the mem table "belongs"
			 * to an unsimplified block(s), the n_initializer field
			 * indicates presence of initializer, otherwise initializer
			 * information is recorded in the init_stat field of
			 * mem table. */

			for( Ptable p=n->n_realscope; p!=r; p=p->next )
				if( p->init_stat == 2 ) {
					error(&where,"goto%n pastDWIr",d);
					goto plugh; /* avoid multiple error msgs */
				}
				else if( p->init_stat == 0 ) {
					int i;
					for(Pname nn=p->get_mem(i=1);nn;nn=p->get_mem(++i))
						if(nn->n_initializer||nn->n_evaluated){
							error(&nn->where,"goto%n pastId%n",d,nn);
							goto plugh;
						}
				}
plugh:

			/* Proceed in a similar manner from the point of the goto,
			 * generating the code to activate dtors before the goto. */
			/* There is a bug in this code.  If there are class objects
			 * of the same name and type in (of course) different mem
			 * tables on the path to the root of the subtree from the
			 * goto, then the innermost object's dtor will be activated
			 * more than once. */

			{
			Pstmt dd = 0, ddt = 0;

			for( Ptable p=scope; p!=r; p=p->next ) {
				int i;
				for(Pname n=p->get_mem(i=1);n;n=p->get_mem(++i)) {
		Pname cln;
		if (n->tp == 0) continue; /* label */

		if ( cln=n->tp->is_cl_obj() ) {
			Pclass cl = (Pclass)cln->tp;
			Pname d = cl->has_dtor();

			if (d) {	/* n->cl::~cl(0); */
				Pexpr dl = call_dtor(n,d,0,DOT,one);
				Pstmt dls = new estmt(SM,n->where,dl,0);
				if (dd)
					ddt->s_list = dls;
				else
					dd = dls;
				ddt = dls;
			}

		}
		else if (cl_obj_vec) {
			Pclass cl = (Pclass)cl_obj_vec->tp;
		//	Pname c = cl->has_ictor();
			Pname d = cl->has_dtor();

			if (d) {	//  __vec_delete(vec,noe,sz,dtor,0);
				Pfct f = Pfct(d->tp);
				int i = 0;
				for (Pname nn = f->f_args->n_list; 
					nn && nn->n_list; nn=nn->n_list) i++;
				Pexpr a = cdvec(vec_del_fct,n,cl,d,0,new ival(i));
				Pstmt dls = new estmt(SM,n->where,a,0);
				if (dd)
					ddt->s_list = dls;
				else
					dd = dls;
				ddt = dls;
			}
		}
				} /* end mem table scan */
			} /* end dtor loop */

			/* "activate" the list of dtors obtained. */

			if( dd ) {
				dd->simpl();
				Pstmt bs = new stmt( base, where, 0 );
				*bs = *this;
				base = PAIR;
				s = dd;
				s2 = bs;
			}
			}
		} /* end special case for non-local goto */
		}
		break;

	case IF:
		e->simpl();
		s->simpl();
		if (else_stmt) else_stmt->simpl();
		break;

	case FOR:	// "for (s;e;e2) s2; => "s; for(;e,e2) s2"
		if (for_init) for_init->simpl();
		if (e) {
			curr_expr = e;
			e->simpl();
		}
		if (e2) {
			curr_expr = e2;
			e2->simpl();
 			if (e2->base==ICALL)
				if (e2->e1 == 0) error('s',"cannot expand inline void%n called in forE", e2->il->fct_name);
		}
		{	Pstmt obl = break_del_list;
			Pstmt ocl = continue_del_list;
			break_del_list = 0;
			continue_del_list = 0;
			s->simpl();
			break_del_list = obl;
			continue_del_list = ocl;
		}
		break;

	case BLOCK:
		Pblock(this)->simpl();
		break;

	case PAIR:
		break;
	}

	/*if (s) s->simpl();*/
//error('d',"base %k memtbl %d",base,memtbl);
	if (base!=BLOCK && memtbl) {
		Pstmt t1 = (s_list) ? s_list->simpl() : 0;
		Pstmt tpx = t1 ? t1 : this;

		Pstmt ss = 0;
		Pname cln;	// used for warnings
		int i;
		Pname tn = memtbl->get_mem(i=1);
		for (; tn; tn=memtbl->get_mem(++i)) {
			if (cln = tn->tp->is_cl_obj()) {
				Pname d = Pclass(cln->tp)->has_dtor();
				if (d) {	/* n->cl::~cl(0); */
					Pexpr dl = call_dtor(tn,d,0,DOT,one);
					Pstmt dls = new estmt(SM,tn->where,dl,0);
					dls->s_list = ss;
					ss = dls;
				}
			}
		}

		if (ss) {
			Pstmt t2 = ss->simpl();

			switch (base) {
			case IF:
			case WHILE:
			case DO:
			case SWITCH:
				temp_in_cond(e,ss,memtbl);
				break;

			case PAIR:	// can hide a return
			{	Pstmt ts = s2;
				while (ts->base==PAIR) ts = ts->s2;
				if (ts->base == RETURN) {	// sordid
					this = ts;
					goto retu;
				}
				goto def;
			}
			case RETURN:
			retu:
			{	
				if (e == 0) {
					// return; dtors; => dtors; return;
					Pstmt rs = new estmt(RETURN,where,0,0);
					//rs->empty = empty;	// BSD fudge
					rs->ret_tp = ret_tp;
					base = PAIR;
					s = ss;
					s2 = rs;
					//Cstmt = ostmt;
					return t1 ? t1 : rs;
				}

				Pname cln = e->tp->is_cl_obj();
				if (cln==0
				|| Pclass(cln->tp)->has_oper(ASSIGN)==0) {
					//  ... return e; dtors; =>
					//  ... X r; ... r = e; dtors; return r;
					Pname rv = new name("_rresult"); // NOT "_result"
					rv->tp = ret_tp /* e->tp */;
					if (memtbl == 0) memtbl = new table(4,0,0);
					Pname n = rv->dcl(memtbl,ARG);
					n->where = no_where;
					n->n_scope = FCT;
					n->n_assigned_to = 1;
					delete rv;
					Pstmt rs = new estmt(RETURN,where,n,0);
					rs->ret_tp = ret_tp;
					base = SM;
					e = new expr(ASSIGN,n,e);
					e->tp = n->tp;
					Pstmt ps = new pair(where,ss,rs);
					ps->s_list = s_list;
					s_list = ps;
					//Cstmt = ostmt;
					return t1 ? t1 : rs;
				}
			}
				
			case FOR:	// don't know which expression the temp comes from
				error('s',&where,"E in %kS needs temporary ofC%nW destructor",base,cln);
				break;

			case SM:	// place dtors after all "converted" DCLs
				if (t1) {
//					ccheck(e);
					for (Pstmt ttt, tt=this;
						(ttt=tt->s_list) && ttt->base==SM;
						tt = ttt) ;
					t2->s_list = ttt;
					tt->s_list = ss;
					//Cstmt = ostmt;
					return t1!=tt ? t1 : t2;
				}
			default:
			def:
//				if (e) ccheck(e);
				if (t1) {	// t1 == tail of statment list
					t2->s_list = s_list;
					s_list = ss;
					//Cstmt = ostmt;
					return t1;
				}
				s_list = ss;
				//Cstmt = ostmt;
				return t2;
			}
		}
		//Cstmt = ostmt;
		return (t1) ? t1 : this;
	}

	//Cstmt = ostmt;
	return (s_list) ? s_list->simpl() : this;
}

Pstmt stmt::copy()
// now handles dtors in the expression of an IF stmt
// not general!
{
	Pstmt ns = new stmt(0,curloc,0);

	*ns = *this;
	if (s) ns->s = s->copy();
	if (s_list) ns->s_list = s_list->copy();

	switch (base) {
	case PAIR:
		ns->s2 = s2->copy();
		break;
	}

	return ns;
}

Pname overFound = 0;

static Pexpr 
mk_new_with_args( Pexpr pe, Ptype tt, Pclass cl, Pexpr vec = 0 ) 
{ // allocate using operator new(sizeof(cl),args1)
	Pexpr p;
	Pexpr args = pe->e2;

	// Pexpr ce = new texpr(SIZEOF,tt,0);
	Pexpr ce;
	if (vec) 
  		ce = vec;
	else ce = new texpr(SIZEOF,tt,0);

	(void) tt->tsizeof();
	ce->tp = size_t_type;
	args = new expr(ELIST,ce,args);
	char* s = oper_name(NEW);
	Pname n = new name(s);
	if (pe->base == GNEW)	// ::new
		p = gtbl->look(s,0);
	else 
		p = find_name(n,cl,scope,CALL,curr_fct);
	p = new call(p,args);
	overFound=0; // set in call_fct
	(void) p->call_fct(cl->memtbl);
	if (overFound && overFound->n_scope != EXTERN) 
		check_visibility(overFound,0,cl,cc->ftbl,cc->nof);
	overFound=0;
	return p;
}

void expr::simpl_new()
/*
	change NEW or GNEW node to CALL node
*/
{
	Pname cln;
	Pname ctor;
	int sz = 1;
//	int esz;
	Pexpr var_expr = 0;
	Pexpr const_expr = 0;
	Ptype tt = tp2;
	Pexpr arg;
	Pexpr szof;
	Pname nf;
	Pexpr init = e1;

	if (init && init->base) init = 0;	// only non-ctor init
// error('d',"simpl_new %k e1 %k e2 %k init %k",base, e1?e1->base:0,e2?e2->base:0,init?init->base:0);

	if ((cln=tt->is_cl_obj()) && init == 0) {
		Pclass cl = Pclass(cln->tp);
		Pexpr p;
		ctor=cl->has_ctor();
// error('d',"cl %t ctor %n",cl, ctor);
		if (e2		// placement
		    || ctor==0	// no constructor
		    || ctor->n_table!=cl->memtbl // inherited constructor??? 
		    || (base==GNEW && cl->has_oper(NEW)) )
			p = mk_new_with_args( this, tt, cl ); // new(sizeof(cl),args1)
		else {
			p = zero;	// 0->ctor(args)
			// check visibility anyway...
			(void)mk_new_with_args(this,tt,cl);
		}

		if (ctor) {
			Pexpr c = e1;	// ctor call generated in expr::typ
			Ptype ttt = tp;
			c->e1->e1 = p;	// p->ctor
			c->simpl();
			*this = *c;
			tp = ttt;
			delete c;
		}
		else {		// (tp)new(args)
			base = CAST;
			tp2 = tp;
			e1 = p;
			e2 = 0;
			simpl();
		}
		return;
	} else if ( cln ) {
		Pclass cl = Pclass(cln->tp);
		// check visibility anyway...
		(void)mk_new_with_args(this,tt,cl);
	}
	
	Pclass covn = 0;
	if (cl_obj_vec) {
		covn = Pclass(cl_obj_vec->tp);
		ctor = covn->has_ictor();
		if (ctor == 0) {
			if (covn->has_ctor()) error("new %s[], no defaultK",covn->string);
			cl_obj_vec = 0;
		}
	}

xxx:
//error('d',"xxx %t",tt);
	switch (tt->base) {
	case TYPE:
		tt = Pbase(tt)->b_name->tp;
		goto xxx;

	default:
		(void) tt->tsizeof();
		szof = new texpr(SIZEOF,tt,0);
		szof->tp = uint_type;
		break;

	case VEC:
	{	Pvec v = Pvec(tt);
//error('d',"v %d %d",v->size,v->dim);
		if (v->size)
			sz *= v->size;
		else if (v->dim) 
			var_expr = v->dim;
		else
			sz = 0;
		tt = v->typ;
		goto xxx;
	}
	}

	if (cl_obj_vec) { // _vec_new(0,no_of_elements,element_size,ctor)
		const_expr = new ival(sz);
		Pexpr noe = (var_expr) ? (sz!=1) ? new expr(MUL,const_expr,var_expr) : var_expr : const_expr;
		const_expr = szof;
		const_expr->tp = uint_type;
		base = CALL;
		arg = new expr(ELIST,ctor,0);
		/*ctor->take_addr();*/
		ctor->lval(ADDROF);
		Pexpr sub=0;
		if (e2 && e2->e1 &&
			e2->e1->tp) 
		{ // new(size_t, args)
			Pexpr vec_sz = new expr(MUL, noe, const_expr);
			sub = mk_new_with_args( this, tt, covn, vec_sz );
		}
		arg = new expr(ELIST,const_expr,arg);
		arg = new expr(ELIST,noe,arg);
		// arg = new expr(ELIST,e2?e2:zero,arg);	// may be preallocated
		arg = new expr(ELIST,e2?(sub?sub:e2):zero,arg);	// may be preallocated
		base = CAST;
		tp2 = tp;
		e1 = new expr(G_CALL,vec_new_fct,arg);
		e1->fct_name = vec_new_fct;
		e1->tp = Pfct(vec_new_fct->tp)->returns;
		simpl();
		return;
	}

	/* call _new(element_size*no_of_elements) */
//error('d',"sz %d var %d",sz,var_expr);
	if (sz == 1)
		arg = (var_expr) ? new expr(MUL,szof,var_expr) : szof;
	else {
		const_expr = new ival(sz);
		const_expr->tp = uint_type;
		const_expr = new expr(MUL,const_expr,szof);
		const_expr->tp = uint_type;
		arg = (var_expr) ? new expr(MUL,const_expr,var_expr) : const_expr;
	}

	arg->tp = uint_type;
	base = CAST;
	tp2 = tp;
	arg = new expr(ELIST,arg,e2);
	nf = gtbl->look(oper_name(NEW),0);	// always global,
						// all class object handled above
	e1 = new expr(G_CALL,nf,arg);
	(void) e1->call_fct(gtbl);
	simpl();

	if (init) {	// alloc(sz) => (p=alloc(sz),*p=init,p);
		Pexpr p = init->e1;
		Pexpr ee = new expr(0,0,0);
		*ee = *this;	
		ee = new expr(ASSIGN,p,ee);	// ee: p = alloc(sz);
		init->base = ASSIGN;
		init->e1 = p->contents();	// init: *p = init_val
		ee = new expr(CM,ee,init);
		ee->simpl(); 
		base = CM;
		e1 = ee;
		e2 = p;
	}
}

void expr::simpl_delete()
/*
	delete p => _delete(p);
		    or  cl::~cl(p,1);
	delete[s]p => _delete(p);
			or vec_del_fct(p,vec_sz,elem_sz,~cl,1);
*/		 
{
	for (Ptype tt = e1->tp; tt->base==TYPE; tt=Pbase(tt)->b_name->tp);
	tt = Pptr(tt)->typ;
//error('d',"simpl_delete() %t",e1->tp);
	Pname cln = tt->is_cl_obj();
	Pname n;
	Pclass cl;

	if (cln) {
		cl = Pclass(cln->tp);
		if ((cl->defined&DEFINED) == 0) error('w',"delete%t (%t not defined)",cl,cl);
	}
	else
		cl = 0;

	if (cl && (n=cl->has_dtor())) {	// ~cl() might be virtual
		//xxx check for private/protected op delete
		{
			Pexpr ee = new expr(ELIST,e1,0);
			char* s = oper_name(DELETE);
			Pname n;
//error('d',"%s( %k )",s,e1->base);
			if (base!=GDELETE) {
				n = new name(s);
				n = (Pname)find_name(n,cl,scope,CALL,curr_fct);
//error('d',"found%n  %t",n,n->tp);
				if (n->tp->base==OVERLOAD
				||  Pfct(n->tp)->nargs==2) {
					Pexpr ss = new texpr(SIZEOF,cl,0);
					ss->tp = size_t_type;
					ee->e2 = new expr(ELIST,ss,0);
				}
			}
			else
				n = gtbl->look(s,0);
//error('d',"found%n  %t",n,n->tp);
			ee = new call(n,ee);
			ee->base = G_CALL;
//error('d',"delete..."); display_expr(ee);
// following commented out to avoid typ::checking problems
			//overFound=0; // set in call_fct
			//(void) ee->call_fct(cl->memtbl);
			//if (overFound && overFound->n_scope != EXTERN) 
				//check_visibility(overFound,0,cl,cc->ftbl,cc->nof);
			//overFound=0;
		}

//if ( base!=GDELETE ) (void) cl->has_oper(DELETE);

		if(base==GDELETE && e2==0 || Pfct(n->tp)->f_virtual) {	// may need temp
			nin=1;
			int needtemp = e1->not_simple();
			nin=0;
			if(needtemp) {
				// convert:	delete [e2] e1
				// to:	(T=e1), delete [e2] T
				// where T is a new temporary.
				Pname tnx = new name(make_name('K'));
				tnx->tp = e1->tp;
				Pname tn = tnx->dcl(scope,FCT);
				delete tnx;
				tn->assign();
				e1 = new expr(ASSIGN,tn,e1);
				e2 = new expr(base,tn,e2);
				base=CM;

				simpl();
				return;
			}
		}
		Pexpr r = e1;
	
		// handle delete p, where p has a private destructor
		if (n->n_scope != PUBLIC) check_visibility(n,0,cl,cc->ftbl,cc->nof);
//error('d',"e2 %d %k",e2,base);
		if (e2 == 0) {		// e1->cl::~cl(1)
			Pexpr ee = call_dtor(r,n,base==GDELETE?0:one,REF,one);
			if (Pfct(n->tp)->f_virtual) {
				if (ansi_opt) { // q?void:int would be an error
					ee = new expr(G_CM,ee,zero);
					ee->tp = zero_type;
				}
				ee = new expr(QUEST,ee,zero);
				ee->tp = ee->e1->tp;
				ee->cond = r;
			}
			if (base == GDELETE) {
				char* s = oper_name(DELETE);
				Pexpr p = gtbl->look(s,0);
				e2 = new call(p,new expr(ELIST,e1,0));
				base = CM;
				e1 = ee;
			}
			else {
				*this = *ee;
				delete ee;
			}
			simpl();
			return;
		}
		else {		// del_cl_vec(e1,e2,elem_size,~cl,1);
			Pexpr sz = new texpr(SIZEOF,tt,0);
			(void)tt->tsizeof();
			Pfct f = Pfct(n->tp);
			int i = 0;
			Pname nn = f->f_args->n_list;
			for (; nn && nn->n_list; nn=nn->n_list) i++;
//error('d',"n %n i %d",n,i);
			Pexpr arg = new ival(i);
			sz->tp = uint_type;
		//	Pexpr arg = one;
			arg = new expr(ELIST,one,arg);
			if (Pfct(n->tp)->f_virtual) {
				Pexpr a = new ref(REF,e1,n);
				a = a->address();
				a = new mdot( "f", a );
				a->i1 = 9;
				arg = new expr(ELIST,a,arg);
			}
			else {
				arg = new expr(ELIST,n,arg);
				n->lval(ADDROF);	// n->take_addr();
			}

			arg = new expr(ELIST,sz,arg);
			if (e2->base==DUMMY) {
			        e2 = new ival(-1);      // handle `delete[]p'
			}

			arg = new expr(ELIST,e2,arg);
			arg = new expr(ELIST,e1,arg);
			Pexpr ee = new expr(G_CALL,vec_del_fct,arg);
			ee->fct_name = vec_del_fct;
			ee->tp = tp;
	//		if (ansi_opt) { // q?void:int would be an error
	//			ee = new expr(G_CM,ee,zero);
	//			ee->tp = zero_type;
	//		}
	//		ee = new expr(QUEST,ee,zero);
	//		ee->tp = tp;
	//		ee->cond = r;
			*this = *ee;
			simpl();
			return;
		}
	}
	else if (cl_obj_vec) {
		error("delete array of arrays");
	}
	else {					// _delete(e1)
		Pexpr ee = new expr(ELIST,e1,0);
		char* s = oper_name(DELETE);
		if (cl && base!=GDELETE) {
			Pname n = new name(s);
			e1 = find_name(n,cl,scope,CALL,curr_fct);
			if (e1->tp->base==OVERLOAD || Pfct(e1->tp)->nargs==2) {
				Pexpr ss = new texpr(SIZEOF,cl,0);
				ss->tp = size_t_type;
				ee->e2 = new expr(ELIST,ss,0);
			}
		}
		else
			e1 = gtbl->look(s,0);
		base = G_CALL;
		e2 = ee;
		overFound=0; // set in call_fct
		(void) call_fct(scope);
		if (overFound && overFound->n_scope != EXTERN) 
			check_visibility(overFound,0,cl,cc->ftbl,cc->nof);
		overFound=0;
	}

	Pcall(this)->simpl();
}
