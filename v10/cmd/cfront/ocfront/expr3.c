/*ident	"@(#)ctrans:src/expr3.c	1.3.5.44" */
/***************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

expr3.c:

	type check function calls, casts, and explicit coercions

************************************************************************/

#include "cfront.h"
#include "size.h"

int pr_dominate(Ptype t1, Ptype t2)
/*
*/
{
	Pname cn1 = t1->is_cl_obj();
	Pname cn2 = t2->is_cl_obj();
//error('d',"pr_dominate(%t,%t)",t1,t2);

	if (cn1==0 || cn2==0) {
		Ptype p1 = t1->is_ptr();
		Ptype p2 = t2->is_ptr();
		if (p1 && p2) {			// pointers
			cn1 = Pptr(p1)->typ->is_cl_obj();
			cn2 = Pptr(p2)->typ->is_cl_obj();
			if (cn1==0 || cn2==0) return 0;
		}
		else {
			p1 = t1->is_ref();
			p2 = t2->is_ref();
			if (p1 && p2) {		// references
				cn1 = Pptr(p1)->typ->is_cl_obj();
				cn2 = Pptr(p2)->typ->is_cl_obj();
				if (cn1==0 || cn2==0) return 0;
			}
		//	else if (p1 && cn2) {
		//		cn1 = Pptr(p1)->typ->is_cl_obj();
		//	}
			else
				return 0;	// not the same and not classes
		}
	}
	Pclass c1 = Pclass(cn1->tp);
	Pclass c2 = Pclass(cn2->tp);
//error('d'," c1 : c2 %d; c2 : c1 %d",c1->has_base(c2),c2->has_base(c1));
	if (c1->has_base(c2)) return 1;
	if (c2->has_base(c1)) return 2;
	return 0;
}

int exact1(Pname,Ptype);
static exact2(Pname,Ptype);
static exact3(Pname,Ptype);

static Pname user_dominate(Pname n1, Pname n2, Pexpr arg)
{
	Pfct f1 = Pfct(n1->tp);
	Pfct f2 = Pfct(n2->tp);

	Pname a1 = f1->argtype;
	Pname a2 = f2->argtype;
//error('d',"user_dominate: %n %t %t",n1,f1,f2);

	for(; a1 && a2; a1 = a1->n_list, a2 = a2->n_list) {
		Ptype t1 = a1->tp;
		Ptype t2 = a2->tp;
	}

	if (a1 && !a1->n_evaluated) return n1;
	if (a2 && !a2->n_evaluated) return n2;

	a1 = f1->argtype;
	a2 = f2->argtype;

	Pname prev = 0;
	Pexpr e = arg;

	for(; a1 && a2 && e; a1 = a1->n_list, a2 = a2->n_list, e = e->e2) {
		Ptype t1 = a1->tp;
		Ptype t2 = a2->tp;
		Ptype at = e->e1->tp;

		int j = can_coerce(t1,at);
		Ptype tt1 = Ncoerce ? Pfct(Ncoerce->tp)->returns : 0;
		j = can_coerce(t2,at);
		Ptype tt2 = Ncoerce ? Pfct(Ncoerce->tp)->returns : 0;
		if(!tt1 || !tt2 || tt1->check(tt2,OVERLOAD)) 
			return 0;

		int one = 0, two = 0;
		if(exact1(a1,tt1)) one = 4;
		else if(exact2(a1,tt1)) one = 3;
		else if(exact3(a1,tt1)) one = 2;
		else one = 1;
		if(exact1(a2,tt1)) two = 4;
		else if(exact2(a2,tt1)) two = 3;
		else if(exact3(a2,tt1)) two = 2;
		else two = 1;

		if(one > two && (!prev || prev==n1)) prev = n1;
		else if(two > one && (!prev || prev==n2)) prev = n2;
		else if(one == two && one) ;
		else return 0;
	}
	if(prev) return prev;
	
	return 0;
}

Pname dominate(Pname n1, Pname n2, Pexpr arg, int const_obj, int level)
/*
	the two functions n1 and n2 can each respond to a call using
	standard conversions. Does the one dominate the other in the
	sense that all its arguments are identical to the other or
	classes defived from the class of the corresponding argument
	of the other.

	If so return it, otherwise return 0;
*/
{
	Pname res = 0;

	Pfct f1 = Pfct(n1->tp);
	Pfct f2 = Pfct(n2->tp);

	Pname a1 = f1->argtype;
	Pname a2 = f2->argtype;
	Pexpr e = arg;
//error('d',"dominate: %n %t %t e %d",n1,f1,f2,e);

	if (e == 0) {
		if (const_obj) {
			if (f1->f_const && f2->f_const==0) return n1;
			if (f2->f_const && f1->f_const==0) return n2;
		}
		else {
			if (f1->f_const==0 && f2->f_const) return n1;
			if (f2->f_const==0 && f1->f_const) return n2;
		}
		return 0;
	}

	for(; a1 && a2 && e; a1 = a1->n_list, a2 = a2->n_list, e = e->e2) {
		Ptype t1 = a1->tp;
		Ptype t2 = a2->tp;
		Ptype at = e->e1->tp;
//error('d',"t1 %t t2 %t at %t",t1,t2,at);
		if (t1==t2 || t1->check(t2,0)==0 )
			continue;
		Pptr r1 = t1->is_ref();
		Pptr r2 = t2->is_ref();
//error('d',"const_problem %t %t %t %d",t1,t2,at,const_problem);
		if (const_problem) {	// t1 and t1 differs only in const
			Pname rr;
			if (at->check(t1,0)==0 && const_problem==0) {
				if (t1->is_ptr())
					rr = n1;
				else
					goto nc;
			}
			else if (at->check(t2,0)==0 && const_problem==0) {
				if (t2->is_ptr())
					rr = n2;
				else
					goto nc;
			}
			else if (r1 && r1->typ->check(at,0)==0 && const_problem==0)
				rr = n1;
			else if (r2 && r2->typ->check(at,0)==0 && const_problem==0)
				rr = n2;
			else
				goto nc;
		
			if (res && res!=rr) return 0;	// mutual dominace
			res = rr;
			continue;	// identical
		}
	nc:
		if (r2 && (t1==r2 || t1->check(r2->typ,0)==0)) continue;
		if (r1 && (t2==r1 || t2->check(r1->typ,0)==0)) continue;

		Pname rr = 0;
		if (t1==at || exact1(a1,at))
			rr = n1;
		else if (t2==at || exact1(a2,at))
			rr = n2;
		else if (1<level) {	// try integral promotion
			if (exact2(a1,at))
				rr = n1;
			if (exact2(a2,at)) {
				if (rr) rr = 0;
				else rr = n2;
			}
		}
		if (!rr && 2<level) {	// try standard conversions
			if (exact3(a1,at))
				rr = n1;
			if (exact3(a2,at)) {
				if (rr) rr = 0;
				else rr = n2;
			}
		}
		if (rr) {
			if  (res && res!=rr) return 0;	// mutual dominance
			res = rr;
			continue;
		}

		int r = pr_dominate(t1,t2);
//error('d',"pr1 %d",r);
		if (r) {
			Pname rr = r==1?n1:n2;
			if (res && res!=rr) return 0;	// mutual dominace
			res = rr;
			continue;
		}

		r = pr_dominate(t1,at);
//error('d',"pr2 %d",r);
		if (r==2) {
			if (res && res!=n1) return 0;	// mutual dominace
			res = n1;
			r = pr_dominate(t2,at);
			if (r==2) return 0;	// mutual dominace
			continue;
		}

		r = pr_dominate(t2,at);
//error('d',"pr3 %d",r);
		if (r==2) {
			if (res && res!=n2) return 0;	// mutual dominace
			res = n2;
			continue;
		}

	}
	//if (a1==0 && a2 && a2->n_initializer==0) return 0;	// wrong number of arguments
	//if (a2==0 && a1 && a1->n_initializer==0) return 0;	// wrong number of arguments

	if (e) {
//error('d',"more args %t a1 %t a2 %t",e->e1->tp,a1?a1->tp:0,a2?a2->tp:0);
		int k1 = f1->nargs_known!=ELLIPSIS;
		int k2 = f2->nargs_known!=ELLIPSIS;
		if (a1 && a1->tp->check(e->e1->tp,ASSIGN)==0) return n1;
		if (a2 && a2->tp->check(e->e1->tp,ASSIGN)==0) return n2;
		if (k1 && k2) return 0;
	}

	if (a1==0 && a2 && a2->n_initializer==0) return 0;
	if (a2==0 && a1 && a1->n_initializer==0) return 0;
	if (res==0)
		if (const_obj) {
			if (f1->f_const && f2->f_const==0) return n1;
			if (f2->f_const && f1->f_const==0) return n2;
		}
		else {
			if (f1->f_const==0 && f2->f_const) return n1;
			if (f2->f_const==0 && f1->f_const) return n2;
		}
//error('d'," -> %n %t",res,res?res->tp:0);
	return res;
}

Pname Ntmp;

static refd;	// initialization routine called by ref_init, do not apply itor
static no_sti;
extern loc no_where;

Pname make_tmp(char c, Ptype t, Ptable tbl)
{
	Pname tn = tbl->t_name;

	if (tn && tn->tp) error('s',"defaultA too complicated");

	if (Ntmp == 0) {
		Pname cn = t->is_cl_obj();
		if (cn && Pclass(cn->tp)->has_dtor()) Ntmp = cn;
	}

//error('d',"tbl %d cstmt %d %d sti %d",tbl,Cstmt,Cstmt?Cstmt->memtbl:0,sti_tbl);
	if (Cstmt) {	//	make Cstmt into a block
		if (Cstmt->memtbl == 0) Cstmt->memtbl = new table(4,tbl,0);
		tbl = Cstmt->memtbl;
	}
	else if (tbl == gtbl && no_sti == 0) {
		if (sti_tbl == 0) sti_tbl = new table(8,gtbl,0);
		tbl = sti_tbl;
	}

	Pname tmpx = new name(make_name(c));
	tmpx->where = no_where;
	tmpx->tp = t;
	(void) t->tsizeof();
	if ( t->base == COBJ ) {
     		Pclass cl = Pclass(Pbase(t)->b_name->tp);
// error( 'd', "cl: %s %d", cl->string, cl->lex_level );
     		if ( cl->lex_level ) tmpx->lex_level = cl->lex_level;
	}
	Pname tmp = tmpx->dcl(tbl,ARG); // ARG => no init
//error('d',"make_tmp %n %t",tmp,t);
	delete tmpx;
	tmp->n_scope = FCT;
	return tmp;
}

Pexpr init_tmp(Pname tmp, Pexpr init, Ptable tbl)
{
	Pname cn = tmp->tp->is_cl_obj();
	Pname ct = cn ? Pclass(cn->tp)->has_itor() : 0;

	tmp->n_assigned_to = 1;
//error('d',"init_tmp %n ct %n refd %d",tmp,ct,refd);
	if (ct) {	// must initialize
		if (refd) {
//error('d',"'orrible %k",init->e1->base);
			switch (init->e1->base) {	// 'orrible 'ack
			case NAME:
			case REF:
			case DEREF:
				if (init->e1->tp->is_ptr())
					init = init->e1;
				else
					init = new expr(G_CM,init,init->e1->address());
					
			}
			if (ct->tp->base == OVERLOAD) ct = Pgen(ct->tp)->fct_list->f;	// first fct
			tbl = 0;
		}
		return call_ctor(tbl,tmp,ct,init,DOT);
	}
	Pexpr ass = new expr(ASSIGN,tmp,init); // no ctor: can assign
	ass->tp = tmp->tp;
	return ass;
}
/*
int exact0(Pname nn, Ptype at)
//	return 1 if
//	exact match

{
	if (nn == 0) return 0; //known==ELLIPSIS;
	Ptype nt = nn->tp;
	if (at == nt) return 1;
//error('d',"exact0 %d",nt->check(at,0));
	return nt->check(at,0)==0;
}
*/

int exact3(Pname nn, Ptype at)
/*
	return 1 if
	match with standard conversions
*/
{
	if (nn == 0) return 0; //known==ELLIPSIS;
	Ptype nt = nn->tp;

	while(nt->base == TYPE)
		nt = Pbase(nt)->b_name->tp;

	if (at == nt) return 1;

	switch (nt->base) {
	case RPTR:
		if (at==zero_type && Pptr(nt)->typ->is_ptr()==0) return 0; //break;
		if (nt->check(at,COERCE)) {
			Pptr pt = at->addrof();
			nt->base = PTR;		// handle derived classes
			if (nt->check(pt,COERCE)) {
				nt->base = RPTR;
				delete pt;
				return 0;
			}
			nt->base = RPTR;
			delete pt;
		}
		break;
	default:
		switch (at->base) {
		default: 
			if (nt->check(at,COERCE)) return 0;
			break;
		case OVERLOAD:
			// the actual argument is an overloaded function
			// we'll try each instance until one matches
			register Plist gl;
			int no_match = 1;

			for (gl = Pgen(at)->fct_list; gl; gl=gl->l) {
				if (nt->check(gl->f->tp,COERCE)==0) {
					no_match = 0;
					break;
				}
			}

			if ( no_match ) return 0;
		} 
	}
	return 1;
}

int exact1(Pname nn, Ptype at)
/*
	return 1 if
	exact match with
		T <-> const T
		X -> X&
		T* -> const T*
		T* -> T*const
	taken into account
*/
{
	if (nn == 0) return 0;//known==ELLIPSIS;
	Ptype nt = nn->tp;
	if (at == nt) return 1;

//error('d',"exact1 nt %t at %t",nt,at);
	if (nt->check(at,0)) {
//error('d',"nt %t at %t cp %d",nt,at,const_problem);
		if (const_problem)  return 1;	// handle T <-> const T

		Pptr rt = nt->is_ref();	//handle X -> X&
		if (rt && (at->check(Pptr(rt)->typ,0)==0 || const_problem))  return 1;

		Pptr art = at->is_ptr();
		if (rt && art) return 0; // ptrs do not match refs

		// handle T* -> const T* and
		// T* -> T*const
		if (rt || (rt = nt->is_ptr())) {
			if (art == 0) art = at->is_ref();
			if (art) {
//error('d',"art %t %t",art->typ,rt->typ);
				if (art->typ->check(rt->typ,0)) {
					if (const_problem)  return 1;
				}
				else	// T* -> T*const
					 return 1;
			}
		}
		return 0;
	}
	return 1;
}

Pexpr Ninit;	// default argument used;
int Nstd;	// standard coercion used (derived* =>base* or int=>long or ...)

bit exact_match(Pname n, Pexpr arg)
/*
	look for an exact match between "n" and the argument list "arg"
	This function goes through three stages:
	(1) exact match (no coercions at all)
	(2) do integral promotions and float->double and re-try exact match
	(3) try for unique standard conversions

*/
{
	Pfct f = Pfct(n->tp);
	register Pexpr e;
	register Pname nn;

// error('d',"exact_match(%n) %t",n,n->tp);

	for (e=arg, nn=f->argtype; e; e=e->e2, nn=nn->n_list) {
		Pexpr a = e->e1;
		Ptype at = a->tp;
		if (at->base == ANY) return 0;
		if (at->base == ZTYPE) at = int_type;
		if (exact1(nn,at)==0) return 0;
	}

//error('d',"exact %d -> %d",nn,nn?nn->n_initializer!=0:1);
	if (nn) {
		Ninit = nn->n_initializer;
		return Ninit!=0;
	}

//error('d',"return 1");
	return 1;	// exact match without any promotions
}

int exact2(Pname nn, Ptype at)
/*
	return 1 if
	do integral promotion and float->double on at, then match
*/
{
//error('d',"exact2 nt %t at %t",nn?nn->tp:0,at);
	while(at->base == TYPE)
		at = Pbase(at)->b_name->tp;

	switch (at->base) {
	case EOBJ:
		at = Penum(Pbase(at)->b_name->tp)->e_type;
		break;
	case ZTYPE:
		at = int_type;
		break;
	case CHAR:
	case SHORT:
		at = (Pbase(at)->b_unsigned && at->tsizeof()==SZ_INT) ? uint_type : int_type;
		break;
	case FLOAT:
		at = double_type;
	}

	if (nn == 0) return 0;//known==ELLIPSIS;
	Ptype nt = nn->tp;
//error('d',"	exact2 nt %t at %t",nt,at);
	if (at == nt) return 1;
	if (nt->check(at,0)) {
		if (const_problem) return 1;

		Pptr rt = nt->is_ref();	// handle X -> X&
		if (rt && (at->check(Pptr(rt)->typ,0)==0 || const_problem)) return 1;

		Pptr art = at->is_ptr();
		if (rt && art) return 0; // ptrs do not match refs

		// handle T* -> const T* and
		// T* -> T*const
		if (rt || (rt = nt->is_ptr())) {
			if (art == 0) art = at->is_ref();
			if (art) {
				if (art->typ->check(rt->typ,0)) {
					if (const_problem) return 1;
				}
				else	// T* -> T*const
					return 1;
			}
		}
		return 0;
	}
	return 1;
}

bit prom_match(Pname n, Pexpr arg)
/*
	look for an exact match between "n" and the argument list "arg"
	using integral promotions and float->double

*/
{
	Pfct f = Pfct(n->tp);
	register Pexpr e;
	register Pname nn;

// error('d',"prom_match(%n) %t",n,n->tp);

	for (e=arg, nn=f->argtype; e; e=e->e2, nn=nn->n_list) {
		Pexpr a = e->e1;
		Ptype at = a->tp;
		if (at->base == ANY) return 0;

		if (exact2(nn,at)==0) return 0;
	}
	
	if (nn) {
		Ninit = nn->n_initializer;
		return Ninit!=0;
	}

	return 1;	// exact match with promotions
}

bit std_match(Pname n, Pexpr arg)
/*
	look for an exact match between "n" and the argument list "arg"
	using standard conversions

*/
{
	Pfct f = Pfct(n->tp);
	register Pexpr e;
	register Pname nn;

// error('d',"std_match(%n) %t",n,n->tp);

	for (e=arg, nn=f->argtype; e; e=e->e2, nn=nn->n_list) {
		Pexpr a = e->e1;
		Ptype at = a->tp;

		if (at->base == ANY) return 0;
		if ( exact3(nn,at) == 0) return 0;
	}

	if (nn) {
		Ninit = nn->n_initializer;
		return Ninit!=0;
	}

	return 1;
}

Pname Ncoerce;
int ref_cast;

bit can_coerce(Ptype t1, Ptype t2)
/*	return number of possible coercions of t2 into t1,
	Ncoerce holds a coercion function (not constructor), if found
*/
{
	int zz = 0;
	Ncoerce = 0;
	if (t2->base == ANY) return 0;
// error('d',"can_coerce t1 %t t2 %t",t1, t2);

	while(t1->base == TYPE)
		t1 = Pbase(t1)->b_name->tp;

	switch (t1->base) {
	case RPTR:
		while(t2->base == TYPE)
			t2 = Pbase(t2)->b_name->tp;

		switch (t2->base) {
	//	case VEC:
	// 	case PTR:
	//	case RPTR:
  	//		if (t1->check(t2,COERCE) == 0) return 1;
		default:	
		{	Ptype tt2 = t2->addrof();
			if (t1->check(tt2,COERCE) == 0) return 1;
			if (ref_cast) break;//return 0;	// (T&): no coercions
							// except operator T&()
			Ptype tt1 = Pptr(t1)->typ;
			while (tt1->base==TYPE) tt1 = Pbase(tt1)->b_name->tp;
			int bc;
			if ( tt1->base != PTR && tt1->base != RPTR ) {
     				bc = Pbase(tt1)->b_const;
     				Pbase(tt1)->b_const = 0; 
			}
			int i = can_coerce(tt1,t2);
			if ( tt1->base != PTR && tt1->base != RPTR ) 
     				Pbase(tt1)->b_const = bc;
			if (i) return i;
			zz = 1;
		}
		}
	}

	Pname c1 = t1->is_cl_obj();
	Pname c2 = t2->is_cl_obj();
	int val = 0;
	if (ref_cast || zz) goto oper_coerce;
	if (c1) {
		Pclass cl = Pclass(c1->tp);
		if (c2 && c2->tp==cl) return 1;

		/*	look for constructor
				with one argument
				or with default for second argument
			of acceptable type
		*/
		Pname ctor = cl->has_ctor();
		if (ctor == 0) goto oper_coerce;
		register Pfct f = Pfct(ctor->tp);
//error('d',"ctor %n f %t",ctor,f);
		switch (f->base) {
		case FCT:
			switch (f->nargs) {
			case 1:
			one:
			{	Ptype tt = f->argtype->tp;
					if (tt->check(t2,COERCE)==0)
						val = 1;
					else if (const_problem) {
						Pptr p1 = tt->is_ptr_or_ref();
						if (p1==0 || p1->typ->tconst()) val = 1;
					}
				if (tt = tt->is_ref()) {
					Pptr pt = t2->addrof();	// handle derived classed
					tt->base = PTR;
					if (tt->check(pt,COERCE) == 0) val = 1;
					tt->base = RPTR;
					delete pt;
				}
				goto oper_coerce;
			}
			default:
				if (f->argtype->n_list->n_initializer) goto one;
			case 0:
				goto oper_coerce;
			}
		case OVERLOAD:
		{	register Plist gl;

			for (gl=Pgen(f)->fct_list; gl; gl=gl->l) { // look for match
				Pname nn = gl->f;
				Pfct ff = Pfct(nn->tp);

				switch (ff->nargs) {
				case 0:
					break;
				case 1:
				over_one:
				{	Ptype tt = ff->argtype->tp;
//error('d',"over one %t %t -> %d %d",tt,t2,tt->check(t2,COERCE),const_problem);
					if (tt->check(t2,COERCE)==0)
						val = 1;
					else if (const_problem) {
						Pptr p1 = tt->is_ptr_or_ref();
						if (p1==0 || p1->typ->tconst()) val = 1;
					}
					if (tt=tt->is_ref()) {
						Pptr pt = t2->addrof();	// handle derived classed
						tt->base = PTR;
						if (tt->check(pt,COERCE) == 0) {
							tt->base = RPTR;
							delete pt;
							val = 1;
							goto oper_coerce;
						}
						tt->base = RPTR;
						delete pt;
					}
					break;
				}
				default:
					if (ff->argtype->n_list->n_initializer) goto over_one;
				}
			}
			goto oper_coerce;
		}
		default:
			error('i',"cannot_coerce(%k)\n",f->base);
		}
	}

oper_coerce:
//error('d',"oper_coerce %d",val);
	if (c2) {	
		Pclass cl = Pclass(c2->tp);
		int std = 0;
		int oval = val;
		extern Pname conv_dominates(Pname,Pname);
		for (Pname ox, on=cl->conv; on; on=ox) {
			ox = on->n_list;
// error( 'd', "can_coerce: ox: %s on: %s tp: %k", ox?ox->string:"", on->string, on->tp->base );
			Plist gl = 0;
			if ( on->tp->base == OVERLOAD ) {
				gl = Pgen(on->tp)->fct_list;
				on = gl->f;
				gl = gl->l;
			}

overlist: 
// error( 'd', "can coerce: on: %n tp: %t gl: %d", on, on->tp, gl );

			Pfct f = Pfct(on->tp);
			Nstd = 0;
			if (t1->check(f->returns,COERCE) == 0) {
				if (Nstd==0) {	// forget solutions involving standard conversions
					Pname old = Ncoerce;
					if (std) {	// forget
						val = oval+1;
						std = 0;
						Ncoerce = on;
					}
					else if (Ncoerce == 0) {
					//	val = 1;
						val++;
						Ncoerce = on;
					}
					else if ((Ncoerce = conv_dominates(Ncoerce,on))==0) {
						if (val == 1) {
//error('d',"val==1 on %n old %n",on,old);
							Ptype ton = Pfct(on->tp)->returns;
							Ptype tco = Pfct(old->tp)->returns;
							if (t1->check(ton,0)==0)
								;
							else if (t1->check(tco,0)==0)
								on = old;
							else
								val++;
						}
						else
							val++;
						Ncoerce = on;
					}
				}
				else {	// take note only if no exact match seen
					if (Ncoerce==0 || on->tp->check(Ncoerce->tp,0)) {
						if (val==0 || std) {

							if (Ncoerce) Ncoerce = conv_dominates(Ncoerce,on);
							if (Ncoerce == 0) {
								Ncoerce = on;
								val++;
								std = 1;
							}	
						}
					}
				}
			}
// error( 'd', "can_coerce: gl: %d", gl );
			if ( gl ) {
				on = gl->f;
				gl = gl->l;
				goto overlist; // must walk list of overloaded instances
			}
		}
	}
//error('d',"val %d",val);
	if (val) return val;
	if (c1 && Pclass(c1->tp)->has_itor()) return 0;
//error('d',"%t->check(%t) -> %d",t1,t2,t1->check(t2,COERCE));
	if (t1->check(t2,COERCE)) return 0;
	return 1;
}

int gen_coerce(Pname n, Pexpr arg)
/*
	look to see if the argument list "arg" can be coerced into a call of "n"
	1: it can
	0: it cannot or it can be done in more than one way
*/
{
	Pfct f = (Pfct) n->tp;
	register Pexpr e;
	register Pname nn;

//error('d',"gen_coerce(%n,%d) %t",n,arg,n->tp);
	for (e=arg, nn=f->argtype; e; e=e->e2, nn=nn->n_list) {
		if (nn == 0) return f->nargs_known==ELLIPSIS;
		Pexpr a = e->e1;
		Ptype at = a->tp;
		int i = can_coerce(nn->tp,at);
		if (i != 1) return 0;
	}
	if (nn && nn->n_initializer==0) return 0;
	return 1;
}


Pname Nover;
// int Nover_coerce;

int over_call(Pname n, Pexpr arg)
/*	
	return 4 if n(arg) can be performed without coercion of arg
	return 3 if n(arg) can be performed only with promotion coercion of arg
	return 2 if n(arg) can be performed only with standard coercion of arg
	return 1 if n(arg) can be performed only with user defined coercion of arg
	return 0 if n(arg) is an error
	Nover is the function found, if any
	Nstd is the number of standard coercions used
*/
{	
	register Plist gl;
	Pgen g = Pgen(n->tp);
	if (arg && arg->base!= ELIST) error('i',"ALX");

//error('d',"over_call(%n) %k",n,n->tp->base);
	extern suppress_error;
	suppress_error = 1;
//	Nover_coerce = 0;
	Nstd = 0;
	switch (g->base) {
	default:	error('i',"over_call(%t)\n",g);
	case OVERLOAD:	break;
	case FCT:
		Nover = n;
		Ninit = 0;
		if (exact_match(n,arg)) {suppress_error = 0; return 4;}
		if (prom_match(n,arg)) {suppress_error = 0; return 3;}
		if (std_match(n,arg) && Ninit==0) {suppress_error = 0; return 2;}
		Nstd = 0;
		suppress_error = 0;
		return gen_coerce(n,arg);
	}

	Pname exact = 0;
	int no_exact = 0;
	int ret = 0;
	Pname nret;
	for (gl=g->fct_list; gl; gl=gl->l) {		/* look for match */
		Nover = gl->f;
		Ninit = 0;
		Nstd = 0;
		if (exact_match(Nover,arg)) {suppress_error = 0; return 4;}	// no coercion
	//	if (prom_match(Nover,arg)) return 3;	// only promotion
	//	if (std_match(Nover,arg) && Ninit==0) return 2;	// only built-in conversion
		if (ret<3 && prom_match(Nover,arg)) {
			nret = Nover;
			ret = 3;
		}
		if (ret<2 && std_match(Nover,arg) && Ninit==0) {
			nret = Nover;
			ret = 2;
		}
	}

suppress_error = 0;
	if (ret) {
		Nover = nret;
		return ret;
	}

	Nover = 0;
	for (gl=g->fct_list; gl; gl=gl->l) {		/* look for coercion */
		Pname nn = gl->f;
		if (gen_coerce(nn,arg)) {
			Nover = nn;
			return 1;
		}
	}
	return 0;

}

Ptype expr::call_fct(Ptable tbl)
/*
	check "this" call:
		 e1(e2)
	e1->typ() and e2->typ() has been done
*/
{
	Pfct f;
	Pname fn;
	int x;
	int k;
	Pname nn;
	Pexpr e;
	Ptype t;
	Pexpr arg = e2;
	Ptype t1 = e1?e1->tp:0;
	int argno;
	Pexpr etail = 0;
	Pname no_virt = 0;	// set if explicit qualifier was used: c::f()
	Pname chk = 0;		// set if visibility check is needed
				// that is if function name might have been
				// found without use of find_name()
	int const_obj = 0;

	if (t1 == any_type) return any_type;

	switch (base) {
	case CALL:
	case G_CALL:	break;
	default:	error('i',"call_fct(%k)",base);
	}

// error('d',"call %d %k %n arg %d",this,e1->base,e1->base==NAME?e1:0,arg);
	if (t1 == 0) error('i',"call_fct(e1=%d,e1->tp=%t)",e1,t1);
	if (arg && arg->base!=ELIST) error('i',"badAL%d%k",arg,arg->base);

	switch (e1->base) {
	case NAME:
		fn = Pname(e1);
//error('d',"name %n %k",fn,fn->n_oper);
		switch (fn->n_oper) {
		case 0:
		case CTOR:
		case DTOR:
		case TYPE:
		case NEW:
		case DELETE:
			break;
		default:	// real operator: check for operator+(1,2);
			if (arg == 0) break;
			Pexpr a = arg->e1;	// first operand

			if (Pfct(fn->tp)->memof	// obj.operator(1) is OK
			|| a->tp->is_cl_obj()
			|| a->tp->is_ref()) break;
			a = arg->e2;
			if (a == 0)		// unary
				error("%k of basicT",fn->n_oper);
			else {			// binary
				a = a->e1;	// second operand
				if (a->tp->is_cl_obj() || a->tp->is_ref()) break;
				error("%k of basicTs",fn->n_oper);
			}
			break;
		}
		break;
	case REF:
	case DOT:
		no_virt = Pname(e1->n_initializer);
		e1->n_initializer = 0;
		if (e1 && e1->e1) {
			Ptype t = e1->e1->tp;
			Pptr tt = t->is_ptr_or_ref();
			Ptype ft = tt ? tt->typ : t;
			Pexpr ee = e1->e1;
			const_obj = ft->tconst();
                	while (ee && (ee->base==DOT || ee->base==REF)) {
                        	Pexpr m = ee->mem;
                        	if ( ee->base==REF && m->tp &&  m->tp->is_ptr())
                                	break;
                        	ee = ee->e1;
                	}
                 	if (ee) {
                        	Ptype ttt = ee->tp;
				int tc;
                        	switch (e1->base) {
                        	case REF:
                        	        Pptr p = ttt?ttt->is_ptr():0;
                                	if (p && p->typ->tconst())
                                        	const_obj = 1;
                                	break;
                        	case DOT:
					tc = ttt ? ttt->tconst() : 0;
                                	if(ttt && tc && (!strict_opt || tc!=2))
                                        	const_obj = 1;
                        	}
                	}
		}
	case MDOT:
	{	Pexpr n = e1->mem;
	lxlx:
		switch (n->base) {
		case MDOT:
			// reverse mdot (see expr::print())
			//	p->a.b()  => (&p->a)->b() => b(&p->a)
			// or	p->a->b() => (p->a)->b()  => b(p->a)
			// or	oo.a.b()  => (&oo.a)->b() => b(&oo.a)
			// or	oo.a->b() => (oo.a)->b()  => b(oo.a)
		{	
			Pexpr r = e1;
			Pexpr p = r->e1;
			for (Pexpr m = r->mem; m->base==MDOT; m = r->mem) {
				p = new mdot(m->string2,p);
				p->i1 = m->i1+2;
				p->tp = p->mem->tp;
				r->mem = m->mem;
				r->e1 = p;
			}
		}
		case REF:
		case DOT:
			n = n->mem;
			goto lxlx;
		case NAME:
			break;
		default:
			error('i',"ref %k",n->base);
		}
		fn = Pname(n);
//error('d',"mem %n",fn);
		break;
	}
	case MEMPTR:
	default:
		fn = 0;
	};

lll:
//error('d',"lll: %t %k",t1,t1->base);
	switch (t1->base) {
	case TYPE:
		t1 = Pbase(t1)->b_name->tp;
		goto lll;

	case PTR:	// pf() allowed as shorthand for (*pf)()
		switch (Pptr(t1)->typ->base) {
		case FCT:
		case OVERLOAD:
			if (Pptr(t1)->memof) error("O missing in call throughP toMF");
			t1 = Pptr(t1)->typ;
			fn = 0;
			goto lll;
		}

	default:
		if (fn)
			error("call of%n;%n is a%t",fn,fn,e1->tp);
		else
			error("call of%kE ofT%t",e1->base,e1->tp);

	case ANY:
		return any_type;
	
	case OVERLOAD:
	{	register Plist gl;
		Pgen g = Pgen(t1);
		Pname found = 0;
		Pname exact = 0;
		int no_exact = 0;
		int no_gen = 0;

		for (gl=g->fct_list; gl; gl=gl->l) {	// look for exact match
			register Pname nn = gl->f;
			if (exact_match(nn,arg)) {
//error('d',"found exact %n %t",nn,nn->tp);
				if (found) {
					// check if one fct dominates the other
					Pname d = dominate(found,nn,arg,const_obj,0);
					if (d)
						nn = d;
					else
						error("two exact matches for%n:%t and%t",nn,nn->tp,found->tp);
				}
				found = nn;
			}
		}
//error('d',"found exact2 %n",found);
		if (found) goto fnd;

Pname mvec[20];

/****************************************************************************
   the next 2 loops have been commented out to eliminate the rule that calls
   requiring only promotions and standard conversions are preferred over
   calls requiring user-defined conversions.

		for (gl=g->fct_list; gl; gl=gl->l) {	// look for exact match
			register Pname nn = gl->f;

			if (prom_match(nn,arg)) {
				if (found) {
					// check if one fct dominates the other
					Pname d = dominate(found,nn,arg,const_obj,1);
					if (d)
						nn = d;
					else
						error("two exact matches (after integral promotions) for%n:%t and%t",nn,nn->tp,found->tp);
				}
				found = nn;
			}
		}
//error('d',"found exact2 %n",found);
		if (found) goto fnd;

	//	for (gl=g->fct_list; gl; gl=gl->l) {	// look for exact match
	//		register Pname nn = gl->f;
	//
	//		if (exact_match(nn,arg) || prom_match(nn,arg)) {
	//			found = nn;
	//			goto fnd;
	//		}
	//	}

		for (gl=g->fct_list; gl; gl=gl->l) {	// look for match
							// with standard conversion
			register Pname nn = gl->f;
			Ninit = 0;
			Nstd = 0;
			if (std_match(nn,arg)) {
			//	if (Nstd == 0)  {
			//		found = nn;
			//		goto fnd;
			//	}
				if (exact) {
					// check if one fct dominates the other
					Pname d = dominate(exact,nn,arg,const_obj,2);
					if (d == 0) {
						mvec[no_exact++] = nn;
					//	no_exact++;
					//	error("two standard conversions possible for%n: %t and %t",fn,exact->tp,nn->tp);
					}
					else
						exact = d;
				}
				else
					exact = nn;
			}
			
		}

//error('d',"excact %n",exact);
		if (exact) {
			if (no_exact) {
				while (no_exact) {
					Pname d = dominate(exact,mvec[--no_exact],arg,const_obj,2);
					if (d)
						exact = d;
					else
						error("two standard conversions possible for%n: %t and %t",fn,exact->tp,mvec[no_exact]->tp);
				}
				
			}
//error('d',"found exact3 %n",found);
			found = exact;
			goto fnd;
		}

    this is the end of the commented out section.
************************************************************************/

		for (gl=g->fct_list; gl; gl=gl->l) {	/* look for coercion */
			register Pname nn = gl->f;
			if (prom_match(nn,arg) ||
			    std_match(nn,arg)  ||
			    gen_coerce(nn,arg)) {
//error('d',"user2 %n %t",nn,nn->tp);
				if (found) {
					// check if one fct dominates the other
					Pname d = dominate(found,nn,arg,const_obj,3);
//error('d',"dom d %d",d);
					if (d == 0) d = user_dominate(found,nn,arg);
					if (d == 0) {
						mvec[no_gen++] = nn;
					}
					else
						found = d;
				}
				else
					found = nn;
			}
		}
		if(found) {
			while(no_gen) {
				Pname d = dominate(found,mvec[--no_gen],arg,const_obj,3);
				if(d==0) d = user_dominate(found,mvec[no_gen],arg);
				if(d)
					found = d;
				else {
					//error("ambiguousA for%n: %t and %t",fn,found->tp,mvec[no_gen]->tp);
					error("ambiguous call of%n: %t and %t",fn,found->tp,mvec[no_gen]->tp);
				}
			}
		}
	fnd:
//error('d',"fnd %t",found?found->tp:0);
		if (found) {
			overFound = chk = fn = found;
			f = Pfct(fn->tp);
		}
		else {
			error("badAL for%n (no match against any %n)",fn,fn);
			return any_type;
		}
		break;
	}
	case FCT:
		f = Pfct(t1);
		if (fn) {
			switch (fn->n_oper) {
			case CTOR:
			case TYPE:
				chk = fn;
			}
		}
	}

//error('d',"chk %n",chk);
	if (chk) {
		Ptype t = 0;
		Pexpr ee = e1->e1;

		switch (e1->base) {
		case REF:	// ptr->chk()
			if (ee == 0) {	// 0->x() fudge handling new x()
				check_visibility(chk,no_virt,Pclass(chk->n_table->t_name->tp),tbl,cc->nof);
				break;
			};
			t = ee->tp;
			while(t->base==TYPE) t = Pbase(t)->b_name->tp;
			t = Pptr(t)->typ;
			break;
		case DOT:	// obj.chk()
			t = ee->tp;
		}

		Pname cn = t?t->is_cl_obj():0;
		Pclass cl = cn?Pclass(cn->tp):0; // class of ``this'' for chk

		if (cl) {
			if (chk->n_oper==CTOR
				&& chk->n_protect
				&& cc->nof
				&& cc->nof->n_oper==CTOR)
					// BUG: cannot handle protected base
					// class constructor
				;
			else {
				check_visibility(chk,no_virt,cl,tbl,cc->nof);
			}
		}
	}

	if (fn && f->returns->is_cl_obj() && f->f_result==0) {
		// protect against class cn; cn f(); ... class cn { cn(cn&); ... };
		make_res(f);
//error('d',"returns %t",f->returns);
		f->returns->tsizeof();	// make sure it is declared
	}

//error('d',"fn %n %t printed %d",fn,fn?fn->tp:0,fn?fn->n_dcl_printed:0);
	if (fn && fn->n_dcl_printed==0) {
		if (f->f_inline==0 && f->f_imeasure) {
			extern void uninline(Pname fn);
			uninline(fn);
		}

		// ensure printout of class declaration:
		for (Pname nn=f->argtype; nn; nn=nn->n_list)
			if (nn->tp->is_cl_obj()) (void) nn->tp->tsizeof();

		fn->dcl_print(0);
	}

	if (no_virt && f->f_static==0) {
		if (e1->base==REF || e1->base==DOT) e1->n_initializer = fn;
	}
	else
		fct_name = fn;
//error('d',"fn %n %t %d %d",fn,f,f->f_this,f->f_static);
	if (f->f_this) {	//SSS call of non-static memberfunction
		switch (e1->base) {
		case MEMPTR:
		case REF:
		case DOT:
			break;
		default:
			error("O orP missing for%n ofT %t",fct_name,f);
		}
	}
	else if (fn) {	//SSS call of static function
	sss:
		switch (e1->base) {
		case REF:
		case DOT:
			e1 = e1->mem;
			goto sss;
		}
	}

	if (fn) fn->use();	// a patch: ctors are sometimes not use()d

	if (f->f_const==0
	&& (fn==0 || (fn->n_oper!=CTOR && fn->n_oper!=DTOR))) {	//CCC
		Pexpr ee = e1->e1;
                // while (ee && (ee->base==DOT || ee->base==REF)) ee = ee->e1;
                while (ee && (ee->base==DOT || ee->base==REF)) {
			Pexpr m = ee->mem;
// error('d', "m: %k tp %t", m?m->base:0, m?m->tp:0 ); 
			if ( ee->base==REF && m->tp &&  m->tp->is_ptr())
				break;
			ee = ee->e1;
		}
// error('d', "ee: %k tp %k", ee?ee->base:0, ee?ee->tp->base:0);

		if (ee) {
			Ptype tt = ee->tp;
			switch (e1->base) {
			case REF:
			{	Pptr p = tt?tt->is_ptr():0;
				if (p && p->typ->tconst())
					error(strict_opt?0:'w',"non-constMF%n called for constO (anachronism)",fn);
				// is really an error, but only warn to help transition
				break;
			}
			case DOT:
				int tc = tt ? tt->tconst() : 0;
				if (tt && tc && (!strict_opt || tc!=2))
					error(strict_opt?0:'w',"non-constMF%n called for constO (anachronism)",fn);
				// is really an error, but only warn to help transition
			}
		}
	}

	t = f->returns;
	x = f->nargs;
	k = f->nargs_known;

	e = arg;
	if (k == 0) goto rlab;

	for (nn=f->argtype, argno=1; e||nn; nn=nn->n_list, e=etail->e2, argno++) {
		Pexpr a;
		int save_base = 0;
		char* save_name = 0;

		if (e) {
			a = e->e1;
			etail = e;

			if (nn) {	/* type check */
				Ptype t1 = nn->tp;
//error('d',"argtp %t etp %t a %k",t1,a->tp,a->base);

				while(t1->base == TYPE)
					t1 = Pbase(t1)->b_name->tp;

				switch (t1->base) {
				case RPTR:
				{	Ptype pt = Pptr(t1)->typ;
					if (pt->base != FCT ||
						( pt->base == FCT && 
						  pt->check(a->tp,0)))
							a = ref_init(Pptr(nn->tp),a,tbl);
					goto cbcb;
				}
				case COBJ:
					if (a->base!=G_CM
					|| nn->tp->check(a->tp,ASSIGN))
						a = class_init(0,t1,a,tbl);
					if (nn->n_xref) {
						// (temp.ctor(arg),&arg)
						a = a->address();
					}
					else {
						// defend against:
						//	int f(X); ... X(X&);
						Pname cln = Pbase(t1)->b_name;	
						if (cln && Pclass(cln->tp)->has_itor()) {
							// mark X(X&) arguments
							nn->n_xref = 1;
							a = a->address();
						}
					}
	cbcb:
//error('d',"cbcb: a %d %k %t",a->base,a->base,a->tp);
		if (a->base==G_CM) {
			if (a->e1->base==DEREF) a->e1 = a->e1->e2; // (*e1,e2) => (e1,e2)
//error('d',"      a %d %k",a->e1->base,a->e1->base);
			if (a->e1->base==G_CALL
			&& Pname(a->e1->fct_name)
			&& Pname(a->e1->fct_name)->n_oper==CTOR
			&& (a->e2->base==G_ADDROF || a->e2->base==ADDROF)) {
				a = a->e1;	// (ctor(&tmp),&tmp) => ctor(&tmp)
//error('d',"tmp %k %n",a->e2->base,a->e2->e2);
				goto cccc;
			}
			else if (a->e2->base==G_ADDROF
			&& a->e2->e2->base==NAME)  {
			cccc:
//error('d',"cccc: a %d %k %t",a->base,a->base,a->tp);
	if (t1->base==RPTR
	&& Pptr(t1)->typ->tconst()==0) {	// temporary used
		if (warning_opt)
			error('w',"temporary used for non-const%tA",nn->tp);
		else {
			Ptype atp = a->tp;
			if (atp==void_type
			&& a->base==G_CALL
			&& a->e1->tp->base==FCT)
				atp = Pfct(a->e1->tp)->s_returns;

			Ptype tt = t1->is_ref();
//error('d',"tt %t atp %t",tt,atp);
			if (tt) {
				if (Pptr(tt)->typ->tsizeof()!=atp->tsizeof()) {	// sliced
					Ptype aat = atp->is_ptr_or_ref();
					if (aat==0
					|| Pptr(tt)->typ->tsizeof()!=Pptr(aat)->typ->tsizeof())
						error('w',"temporary used for non-const%tA",nn->tp);
				}
			}
			else if (t1->tsizeof()!=atp->tsizeof())	// sliced
				error('w',"temporary used for non-const%tA",nn->tp);
		}
		
	//	if (warning_opt				// blabber
	//	||  t1->tsizeof()!=a->tp->tsizeof()))	// sliced
	//		error('w',"temporary used for non-const%tA",nn->tp);

	}
			}
		}
					e->e1 = a;
					break;
				case ANY:
					goto rlab;
				case PTR:
				{
					save_base = e->e1->base;
					if(a->tp->base==OVERLOAD)
						save_name = Pgen(a->tp)->fct_list->f->string;
					e->e1 = a = ptr_init(Pptr(t1),a,tbl);
				//	if (x==a || x==a->e2) goto def;	// needs checking
					if (Pchecked == 0) goto def;
					break;
				}
				//	e->e1 = a = ptr_init(Pptr(t1),a,tbl);
				//	goto def;
				case CHAR:
				case SHORT:
				case INT:
				//	if (a->base==ICON && a->tp==long_type)
				//		error('w',"long constantA for%n,%kX",fn,t1->base);
				{	Ptype t = a->tp;
					while(t->base == TYPE)
						t = Pbase(t)->b_name->tp;

					switch (t->base) {
					case LONG:
					case FLOAT:
					case DOUBLE:
					case LDOUBLE:
						error('w',"A%d: %t passed as %t",argno,a->tp,t1);
					}
				}
					// no break
				case LONG:
					if (Pbase(t1)->b_unsigned
					&& a->base==UMINUS
					&& a->e2->base==ICON)
						error('w',"negativeA for%n, unsignedX",fn);
				default:
				def:
				{	Pexpr x = try_to_coerce(t1,a,"argument",tbl);
//error('d',"x %d t1 %t nn %t a1 %t",x,t1,nn->tp,a->tp);
					if (x)
						e->e1 = x;
					else if (nn->tp->check(a->tp,ARG)) {
						error("badA %dT for%n:%t (%tX)",argno,fn,a->tp,nn->tp);
						return any_type;
					}
				}
				}

                                Pexpr tt = e->e1;
                                while ( tt->base == CAST )
                                        tt = tt->e1;
                                if ( tt->base == ILIST )
                                        e->e1 = tt;

                                if (e->e1->base == ILIST) {
                                        // memptr constant
                                        // f({1,2,f}) ==> memptr t; f((t={1,2,f},t))
					if(save_base == REF) {
						Pptr m = Pptr(a->tp);
						error(strict_opt?0:'w',
						"address of boundF (try using ``%s::*'' forPT and ``&%s::%s'' for address) (anachronism)",
						m->memof->string,
						m->memof->string,
						save_name
						);
					}
                                        Pname temp = make_tmp('A',mptr_type,tbl);
                                        e->e1 = mptr_assign(temp,e->e1);
                                        e->e1 = a = new expr(G_CM,e->e1,temp);
                                        a->tp = temp->tp;
                                }

			}
			else {
				if (k != ELLIPSIS) {
					error("unexpected %dA for%n",argno,fn);
					return any_type;
				}
				Pexpr te=e;
				while(e) {
                                	if (e->e1->base == ILIST) {
                                        	// memptr constant
                                        	// f({1,2,f}) ==> memptr t; f((t={1,2,f},t))
                                        	Pname temp = make_tmp('A',mptr_type,tbl);
                                        	e->e1 = mptr_assign(temp,e->e1);
                                        	e->e1 = a = new expr(G_CM,e->e1,temp);
                                        	a->tp = temp->tp;
                                	}
					e = e->e2;
				}
				e = te;
				goto rlab;
			}
		}
		else {	/* default argument? */
			a = nn->n_initializer;
			if (a == 0) {
				error("A %d ofT%tX for%n",argno,nn->tp,fn);
				return any_type;
			}
                                if (a->base == ILIST) {
                                        // memptr constant
                                        // f({1,2,f}) ==> memptr t; f((t={1,2,f},t))
                                        Pname temp = make_tmp('A',mptr_type,tbl);
                                        a = mptr_assign(temp,a);
                                        a = new expr(G_CM,a,temp);
                                        a->tp = temp->tp;
                                }
			a->permanent = 2;	// ought not be necessary, but it is
			e = new expr(ELIST,a,0);
			if (etail)
				etail->e2 = e;
			else
				e2 = e;
			etail = e;
		}
	}

rlab:
//error('d',"rlab fct_name %n %t",fct_name,fct_name?fct_name->tp:0);
	for (; e; e = e->e2) {	// unchecked arguments
		Pexpr a = e->e1;
		Pname cn;

		if (a->base==NAME && a->tp->base==FCT) {
			// function name that escaped the type system:
			// update use count
			a->lval(ADDROF);
		}
		else if (warning_opt && (cn = a->tp->is_cl_obj())) {
			Pclass cl = Pclass(cn->tp);
			if (cl->has_ctor() || cl->memtbl->look("__as",0))//cl->has_oper(ASSIGN)
			{
				if (fct_name)
				error('w',"O ofC%t withK or = copied asA to%n (%t)",cl,fct_name,fct_name->tp);
				else
				error('w',"O ofC%t withK or = copied asA to `...'",cl);
			}
		}
		else if (a->tp->is_ref())
			e->e1 = a->contents();
	}

	if (f->f_result) {		// f(args) => (f(&temp,args),temp)
		Pname tn = make_tmp('R',f->returns,tbl);
		e2 = new expr(ELIST,tn->address(),e2);
// error('d',"result %n refd: %d",fn, refd);
		Pexpr ee = new expr(0,0,0);
		*ee = *this;
		base = G_CM;		// (f(&temp,args),temp)
		e1 = ee;
		if (refd == 2)
			e2 = tn->address();
		else e2 = tn;
		tp = tn->tp;
	}

	return t;
}

int cm_const_save; 

Pexpr ref_init(Pptr p, Pexpr init, Ptable tbl)
/*
	initialize the "p" with the "init"
	remember to call ptr_init to ensure that pointers to second bases
	are handled correctly.
*/
{
	register Ptype it = init->tp;
	Pptr px = p;
	while (px->base == TYPE) px = Pptr(Pbase(px)->b_name->tp);
	Ptype p1 = px->typ;
	Pname c1 = p1->is_cl_obj();
// error('d',"ref_init: p %t, p1 %t, px %t, init->tp %t",p,p1,px,it);
// error('d', "ref_init: nof: %n f_const: %d", cc?cc->nof:0, cc?(cc->nof?Pfct(cc->nof->tp)->f_const:0):0);

	if (init->base == ILIST) error("IrL as RIr");

	if (init->base==NAME
	&& Pname(init)->n_scope==ARG
	&& init->tp->base==FLOAT)
		error('w',"initializing a float& with floatA is non-portable");

	while(it->base == TYPE)
		it = Pbase(it)->b_name->tp; 

	switch (it->base) {
	default:
		{	Ptype tt = it->addrof();
			px->base = PTR;	// allow &x for y& when y : public x
					// but not &char for int&
			int x = px->check(tt,COERCE);

			if (x == 0) {	//CCC type is fine check for constness:
				if (init->tp->tconst()
				&& vec_const==0
				&& fct_const==0) {
					// not ``it''
					if (init->base == ELIST) init = init->e1;
					if (px->typ->tconst() == 0) error("R to constO");
					px->base = RPTR;
					// if we have a const lvalue we can still pass its address
					ignore_const++;
					if (init->lval(0)) {
						init->lval(ADDROF); // force output
						ignore_const--;
//error('d',"in1 %t",init->tp);
						return ptr_init(px,init->address(),tbl);//return init->address();
					}
					ignore_const--;
					goto xxx;
				}
				px->base = RPTR;
                                if (init->lval(0)) {	// can pass the address							// no temporary needed 
					init->lval(ADDROF); // force output
//error('d',"px %t init %t init %t",px,init->tp,init->tp);
					return ptr_init(px,init->address(),tbl);
				}
				goto xxx;
			}

			px->base = RPTR;
		}
	}

//error('d',"c1 %n",c1);
	if (c1) {	// assigning to a const X & is fine
		ref_cast++;
		Pexpr x = try_to_coerce(p,init,"reference initialization",tbl);
		ref_cast--;
		if (x) {
			init = x;
			goto xxx;
		}
		while (p1->base==TYPE) p1 = Pbase(p1)->b_name->tp;
		int bc = Pbase(p1)->b_const;
		Pbase(p1)->b_const = 0;
//		refd = 1;	
		switch ( init->base ) {
			case STRING: case ZERO: case CCON:
			case ICON: case FCON: case IVAL:
				refd = 1; 
				break;
			default:
				refd = (init->e1 && init->e1->base == NAME && 
					init->e1->tp->base != RPTR &&
					Pname(init->e1)->n_xref == 0) ? 2: 1;
				break;
		}
// error('d', "***** refd: %d", refd );
		Pexpr a = class_init(0,p1,init,tbl);
		Pbase(p1)->b_const = bc;
		refd = 0;
		if (a==init && init->tp!=any_type) goto xxx;
// error('d',"ri a %d %k",a->base,a->base);
		switch (a->base) {
		case G_CALL:
	//	case CM:
	//	case G_CM:
			init = a;
			goto xxx;
		}
		a = a->address();
		a =  ptr_init(px,a,tbl);
		return a;
//		return ptr_init(px,a->address(),tbl);//a->address();
	}

//error('d',"p1 %t it %t",p1,it);
	if (p1->check(it,0)) {

		if (p1->check(it,ASSIGN) == 0) {
		//	if (p1->is_ptr())  // check for base* = derived*
		//		goto xxx;

			// things like ``double& rr = 1;'' temporary needed
			// warn in case of ``slightly wrong lvalue'', e.g.
			//	int i; double& r = i;
			if (init->lval(0) && p1->tconst()==0)
				error('w',"temporary used toIR; no changes will be propagated to actualA");
			goto def;
		}

		Pexpr x = try_to_coerce(p1,init,"reference",tbl);	// x==init
		if (x==0) x = try_to_coerce(px,init,"reference",tbl);	// x&=init
		if (x) {
			init = x;
			goto def;
		}

		error("badIrT:%t (%tX)",it,p);
		if (init->base != NAME) init->tp = any_type;
		return init;
	}
	
xxx:	/*
		here comes the test of a ``fundamental theorem'':
		a structure valued expression is
			(1) an lvalue of type T (possibly const)
		or	(2) the result of a function (a _result if X(X&) is defined)
		or	(3) a * or [] or ? or , expression
	*/
//error('d',"xxx %k %d %t",init->base,init->base,init->tp);

	switch (init->base) {
	case NAME:
	case DEREF:
	case REF:
	case DOT:			// init => &init
		if (it->tconst() && vec_const==0 && fct_const==0) goto def;
		if ( cc && cc->nof && 
			Pfct(cc->nof->tp)->f_const ) 
				cm_const_save = Pbase(p->typ)->b_const;
		init->lval(ADDROF);
		cm_const_save = 0;

		if (vec_const) return init;
		if (fct_const && p1->is_ptr()) goto def;	// fptr& = fct
		// no break
	case CM:
	case G_CM:			// & (f(&temp), temp)
		return ptr_init(px,init->address(),tbl);//init->address();
	default:
	def:
	{
// error('d',"def: init->tp %t p1 %t ",init->tp,p1);	
// error('d',"p1: %t const_ptr: %d", p1, const_ptr);
		if (const_ptr == 0) {
			if (tbl == gtbl || strict_opt) 
				error("Ir for%snon-constR not an lvalue", strict_opt?"":" global ");
			else
			if (warning_opt) 
				error('w', "Ir for non-constR not an lvalue (anachronism)");
		}

		Pname n = make_tmp('I',p1,tbl);
		Pexpr a;
		Pname ic = init->tp->is_cl_obj();

		if (p1->tconst()==0
		&& (init->tp->tconst() && vec_const==0 && fct_const==0)
		&& p1->check(it,ASSIGN)==0)
			error('w',"constIr: temporary used toI reference");

		switch (p1->base) {
		case INT:
		case CHAR:
		case SHORT:
			switch (it->base) {
			case LONG:
			case FLOAT:
			case DOUBLE:
			case LDOUBLE:
				error('w',"%t assigned to %t inRIr",it,p1);
			}
		}

		if (ic!=c1 && Pclass(ic->tp) != Pclass(c1->tp)) {
			// derived class1 => must cast: ``it Ix; (Ix=init,(p)&Ix);''
			n->tp = init->tp;
			a = ptr_init(px,n->address(),tbl);//n->address();
			PERM(p);
			a = new texpr(CAST,p,a);
			a->tp = p;
		}
		else
			a = n->address();

		refd = 1;
		Pexpr as = init_tmp(n,init,tbl);
		refd = 0;
		a = new expr(G_CM,as,a);
		a->tp = a->e2->tp;
		return a;
	}
	}
}

Pexpr class_init(Pexpr nn, Ptype tt, Pexpr init, Ptable tbl)
/*
	initialize "nn" of type "tt" with "init"
	if nn==0 make a temporary,
	nn may not be a name
*/
{	
	if (init == dummy) return 0;
//error('d',"class_init %t with %t init %k refd %d",tt,init->tp,init->base,refd);
	Pname c1 = tt->is_cl_obj();

        if (init == 0) {
                error("emptyIr");
                return dummy;
        }

	if (c1) {
		Pclass cl = Pclass(c1->tp);
		Pname c2 = init->tp->is_cl_obj();

		if (c1!=c2 || (refd==0 && cl->has_itor())) {
			/*	really ought to make a temp if refd,
				but ref_init can do that
			*/
			int i = can_coerce(tt,init->tp);
//error('d',"i %d nn %n",i,nn);
			switch (i) {
			default:
				error("%d ways of making a%n from a%t",i,c1,init->tp);
				init->tp = any_type;
				return init;
			case 0:
				if (c2 && Pclass(c2->tp)->has_base(cl)) {
					init = init->address();
					Pexpr x = cast_cptr(cl,init,tbl,0);

					if (x == init) {
						Ptype pt = tt->addrof();
						PERM(pt);
						x = new cast(pt,init);
					}

					return x->contents();
				}
				error("cannot make a%n from a%t",c1,init->tp);
				init->tp = any_type;
				return init;
			case 1:
//error('d',"ncoerce %n %k %d",Ncoerce,init->base,init->base);
				if (Ncoerce == 0) {
					Pexpr a = new expr(ELIST,init,0);
					a = new texpr(VALUE,tt,a);
					a->e2 = nn;
				//	return a->typ(tbl);
					a = a->typ(tbl);
//error('d',"ci a %k %d %t",a->base,a->base,a->tp);
					return a;
				}

				switch (init->base) {
				case CM:
				case G_CM:	//ddd
				case NAME:	/* init.coerce() */
	/* *ref */		case DEREF:
				{	Pref r = new ref(DOT,init,Ncoerce);
					Pexpr rr = r->typ(tbl);
					init = new expr(G_CALL,rr,0);
					init->fct_name = Ncoerce;
					break;
				}
				default:	// (temp=init,temp.coerce())
				{	Pname tmp = make_tmp('U',init->tp,tbl); 
					int x = refd;	
					refd = 0;	// ??
					Pexpr ass = init_tmp(tmp,init,tbl);
					refd = x;
					Pref r = new ref(DOT,tmp,Ncoerce);
					Pexpr rr = r->typ(tbl);
					Pexpr c = new expr(G_CALL,rr,0);
					c->fct_name = Ncoerce;
					c = c->typ(tbl);
					init = new expr(CM,ass,c);
					init->tp = c->tp;
					if (refd) {	// &f() => (t=f(), &t)
						Pname tmp2 = make_tmp('L',c->tp,tbl); 
						ass = init_tmp(tmp2,init,tbl);
						init = new expr(G_CM,ass,tmp2);
					}
				}
				}
//error('d',"nn %n",nn);
				if (nn) {
					Pexpr a = new expr(ELIST,init,0);
					a = new texpr(VALUE,tt,a);
					a->e2 = nn;
					return a->typ(tbl);
				}
			}
//error('d',"c1 %n c2 %n",c1,c2);
			return init->typ(tbl);
		}
		return init;
	}
//error('d',"ci check tt %t init->tp %t",tt,init->tp);
	if (tt->check(init->tp,ASSIGN) && refd==0) {
		error("badIrT:%t (%tX)",init->tp,tt);
		init->tp = any_type;
	}

	return init;
}

extern int bound;	// fudge for bound pointers to functions

Pexpr expr::docast(Ptable tbl)
{	
	// check cast against value, INCOMPLETE

//error('d',"docast %d %t %k",this,tp2,e1->base);
	if (e1 == dummy) {
		error("E missing for cast");
		tp = any_type;
		return this;
	}

	int pmf = 0;
	int ptom_cast = 0;
	Pexpr ee = e1;

//error('d',"ee %k %d",ee->base,ee->base);
	switch (ee->base) {
	case ADDROF:
		ee = ee->e2;
		switch (ee->base) {
		case NAME:	goto nm;
		case REF:	goto rf;
		}
		break;

	case NAME:
	nm:
		if (Pname(ee)->n_qualifier) pmf = 1;
		break;
		
	case REF:
	rf:
		if (ee->e1->base == THIS) bound = 1;
		break;
	}

	e1 = e1->typ(tbl);

	int b = bound;	// distinguish between explicit and implicit THIS
	bound = 0;
	pmf = pmf && e1->base==CAST;

	Ptype etp = e1->tp;
	Ptype tt = tp2;
	Ptype t = tt;
	tt->dcl(tbl);

	while (etp->base == TYPE) etp = Pbase(etp)->b_name->tp;
	while (tt->base == TYPE) tt = Pbase(tt)->b_name->tp;
	
//error('d',"e1 %k etp %t tt %t",e1->base,etp,tt);

	switch (etp->base) {
	case PTR:
	case RPTR:
		if (Pptr(etp)->typ->base == OVERLOAD) goto over;

		if (warning_opt && i2==0 && Pptr(etp)->typ->tconst()) {
			switch (tt->base) {
			case FCT:
				break;
			case PTR:
			case RPTR:
				if (Pptr(tt)->typ->tconst()) break;
			default:
				// casting away const
				// should be an error
				// but ANSI says OK so I chicken out
				// to be able to compile strtok(), etc.
				error('w',"const cast away:%t->%t",e1->tp,tp2);
			}
		}
		else
			i2 = 0; // to allow cfront to escape its own checking
		break;
	case COBJ:
	{	ref_cast = 1;
		Pexpr x = try_to_coerce(tt,e1,"cast",tbl);
		ref_cast = 0;
//error('d',"x %k %t tt %d %t",x?x->base:0,x?x->tp:0,tt,tt);
		if (x) {
			if (x!=e1 && x->base==DEREF && tt->is_ref()) x = x->e1;
			if (tt==x->tp || tt->check(x->tp,0)==0 || const_problem)
				return x;
			else
				return new cast(tt,x);
		}
	//	else if (e1->base==DEREF && tt->is_ref()) return e1;
		break;
	}
	case VOID:
		if (tt->base == VOID) {
			tp = t;
			return this;
		}
		error("cast of void value");
		// no break;
	case ANY:
	any:
		tp = any_type;
		return this;
	case FCT:
		if (tt->base == PTR && Pptr(tt)->typ->base != FCT)
			error('w',"P toF cast toP to nonF");
		break;
	case OVERLOAD:
	over:
		error("cast of overloaded");
		goto any;
	}

//error('d',"tt %t",tt);
	switch (tt->base) {
	case PTR:
		if (Pptr(tt)->typ->base==FCT && Pptr(tt)->memof) {
			if (etp->base!=PTR
			|| Pptr(etp)->typ->base!=FCT
			|| Pptr(etp)->memof==0)
				error("cast toP toM %t",tt);
			else {	// adjust delta in MI case
				// for the moment just suppress the cast
				// all pmfs are the same to cc
/*
				Pclass c1 = Pptr(tt)->memof;
				Pclass c2 = Pptr(etp)->memof;
*/
				ptom_cast = 1;
				tp2 = void_type;
			}
		}

		switch (etp->base) {
		case COBJ:
			error("cannot castCO toP");
			break;
		case FCT:
			e1 = new expr(G_ADDROF,0,e1);
			bound = b;
			e1 = e1->typ(tbl);
			bound = 0;
			if (e1->base == CAST)
				pmf = 1;
			else
				break;
			// no break;

		case PTR:
		{	Pname cn = Pptr(tt)->typ->is_cl_obj();
			if (cn) {
				Pexpr x = cast_cptr(Pclass(cn->tp),e1,tbl,1);

				if (x == e1) {
					PERM(tt);
					e1 = new cast(tt,e1);
					e1->i2 = i2;
				}
				else
					e1 = x;
			}
			if (pmf) {
				while(tt->base == TYPE)
					tt = Pbase(tt)->b_name->tp;

				switch (tt->base) {
				case PTR:
					if (Pptr(tt)->memof) break;
				default:
					error("%t cast to%t (%t is not aP toM)",e1->tp,tp2,tp2);
				}
			}
		}
		}
		break;

	case RPTR:		// (x&)e: pretend e is an x
	{	Ptype er = etp;
		Ptype cr = tt;
		do {
			if (er = er->is_ptr_or_ref()) er = Pptr(er)->typ;
			if (cr = cr->is_ptr_or_ref()) cr = Pptr(cr)->typ;
		} while (er && cr);
		int pp = er!=0;	//	if `e' is a suitable pointer cast it:
				// 	(x&)e => (x*)e, otherwise
				//	(x&)e => *(x*)&e
// error('d',"rptr tt %t e1->base %k e1->tp %t",tt,e1->base,e1->tp);
	//	if (Pptr(tt)->typ->tsizeof()>etp->tsizeof()) goto zse;
	// we need to be able to ``raise the type'' from base to derived
	//	if (etp->is_cl_obj() && Pptr(tt)->typ->is_cl_obj()==0) goto zse;

		if (e1->base==G_CM
		|| e1->base==CALL
		|| e1->base==G_CALL
		|| e1->lval(0))
			;
		else if (e1->tp->tconst()) {
				// casting away const
				// should be an error
				// but ANSI says OK so I chicken out
				// to be able to compile strtok(), etc.
			if (warning_opt && Pptr(tt)->typ->tconst()==0)
				error('w',"const cast away:%t->%t",e1->tp,tp2);

		}
		else
			error("cannot cast%t to%t",etp,t);
//error('d',"e1 %k %t %d",e1->base,e1->tp,pp);
		if (pp == 0) e1 = e1->address();	// *(x*)&e
		tp = t;

		// do proper pointer manipulation for multiple inheritance
		Pname cn = Pptr(tt)->typ->is_cl_obj();
		if (cn) {
			Pexpr x = cast_cptr(Pclass(cn->tp),e1,tbl,1);

			if (x == e1) {
				PERM(tt);
				e1 = new cast(tt,e1);
				e1->i2 = i2;
			}
			else
				e1 = x;
		}

		return pp ? this : contents();
//	zse:
//		error("cannot cast%t to%t",etp,t);
//		tp2 = tt = any_type;
//		break;
	}
	case COBJ:
		base = VALUE;	// (x)e => x(e): construct an x from e
		e1 = new expr(ELIST,e1,0);
		return typ(tbl);

	case CHAR:
	case INT:
	case SHORT:
	case LONG:
		switch (etp->base) {
		case FCT:
			e1 = new expr(ADDROF,0,e1);
			e1 = e1->typ(tbl);
			//etp = e1->tp;
		case PTR:
			if(!e1->tp->memptr() && e1->tp->tsizeof()>tt->tsizeof())
				error("type ``%t'' not large enough for values of ``%t ''",tt,etp);
			break;
		case COBJ:
			error("cannot castCO to%k",tt->base);
			break;
		}	
		break;

	case FLOAT:
	case DOUBLE:
	case LDOUBLE:
		switch (etp->base) {
		case FLOAT:
		case DOUBLE:
		case LDOUBLE:
		case CHAR:
		case INT:
		case SHORT:
		case LONG:
		case EOBJ:
		case ZTYPE:
			break;
		default:
			error("cannot cast ``%t '' to ``%t''",etp,tt);
			break;
		}	
		break;

	case FCT:
		error("cannot cast toFT");
		break;
	}

	tp = t;

	if (e1->base==ILIST && ptom_cast==0) { // pointer to member constant
		Pexpr ee = e1->e1;	// ELIST
		int i;
		switch (ee->e2->base) {
		case IVAL:
			i = int(ee->e2->i1);
			break;
		case ZERO:
			i = 0;
		}

		if (i<0)
			e1 = e1->e2;	// just the function
		else
			e1 = ee->e2;	// just the index
		return this;
	}

	if (etp->base==PTR && Pptr(etp)->memof && Pptr(etp)->typ->base==FCT) {
		Pclass cl = Pptr(etp)->memof;

		if (Pptr(tt)->memof==0 && b == 0 ) {
			Pexpr y = new mdot("f",e1);
			y->i1 = 9;
			y = new cast(tt,y);
			if (cl->virt_count && b==0) {
				// ERROR: no check for side effects
				Pexpr z = new mdot("i",e1);
				Pexpr x = new mdot("i",e1);
				x->i1 = 9;
				x = new cast(tt,x);
				z->i1 = 9;
				Pexpr q = new expr (QUEST,x,y);
				q->cond = new expr(LE,zero,z);
				q->tp = tt;
				delete this;
				return q;
			}
			delete this;
			return y;
		}
	}

	return this;
}

Pexpr expr::dovalue(Ptable tbl)
{
	Ptype tt = tp2;
	Pclass cl;
	Pname cn;

//error('d',"value %d %t e1 %d e2 %d",tt,tt,e1,e2);
	
	tt->dcl(tbl);

	while(tt->base == TYPE)
		tt = Pbase(tt)->b_name->tp;

	switch (tt->base) {
	case EOBJ:
	default:
		if (e1 == 0) {
			error("value missing in conversion to%t",tt);
			return dummy;
		}
		base = CAST;
		e1 = e1->e1;	// strip ELIST
		return typ(tbl);

	case CLASS:
		cl = Pclass(tt);
		tp2 = Pptr(cl->this_type)->typ;
		break;

	case COBJ:
		cn = Pbase(tt)->b_name;
		cl = Pclass(cn->tp);
	}

//error('d',"e1 %k e1->e2 %k",e1->base,e1?e1->e2->base:0);
	if (e1 && e1->e2==0) {		// single argument
		if (e1->e1->base==ELIST) e1->e1 = e1->e1->e1;	// spurious elist
		e1->e1 = e1->e1->typ(tbl);
		if (tt->base==COBJ) {
			Pexpr x = try_to_coerce(tt,e1->e1,"type conversion",tbl);
			if (x) return x;
		}

		Pname acn = e1->e1->tp->is_cl_obj();
//error('d',"acn %n %d",acn,cl->has_itor());
		if (acn && cl->has_itor()==0) {
			Pclass acl = Pclass(acn->tp);
			int hb = acl->has_base(cl);

			if (acl==cl || hb) {
				vcllist->clear();
				vcllist=0;
				if (1<is_unique_base(acl,cl->string,0)) error("ambiguous assignment to base %t",cl);
				Pexpr ee = e1->e1;
				if (ee->base == ELIST) ee = ee->e1;	// ???
				if (hb) {	// ee => *(tp2*)&ee
						// remember = may be overloaded
//error('d',"hb %k %t %d",ee->base,ee->tp,ee->lval(0));
					ignore_const++;
					if (ee->lval(0)==0) {
						Pname tmp = make_tmp('T',ee->tp,tbl);
						ee = init_tmp(tmp,ee,tbl);
						ee = new expr(G_CM,ee,tmp->address());
					}
					else
						ee = ee->address();
					ignore_const--;
					ee = new texpr(CAST,new ptr(PTR,tp2),ee); //new cast(new ptr(PTR,tp2),ee);
					ee = ee->contents();
					ee->typ(tbl);
				}

				if (e2) {	// x(x_obj) => e2=x_obj
					base = ASSIGN;
					e1 = e2;
					e2 = ee;
					tp = tp2;
					return this;
				}
				return ee;	// strip ELIST: x(x_obj) => x_obj
			}
		}
	}


	/* x(a) => obj.ctor(a); where e1==obj */
	Pname ctor = cl->has_ctor();
	if (ctor == 0) {
		error("cannot make a%t",cl);
		return dummy;
	}

// error('d',"e2 %k",e2?e2->base:0);
// error('d',"refd: %d const_ptr: %d", refd, const_ptr);
	if (e2 == 0) {		// x(a) => x temp; (temp.x(a),temp)
/* incomplete condition
		if ( refd && const_ptr == 0) {
    			if ( tbl == gtbl ) {
				error("Ir for global non-constCR not an lvalue");
    			}
    			else	
    			if (strict_opt) 
				error("Ir for non-constCR not an lvalue");
    			else
    			if (warning_opt) 
				error('w', "Ir for non-constR not an lvalue (anachronism)");
			}
*/
	
		no_sti = 1;
		Pname n = make_tmp('V',tp2,tbl);
		no_sti = 0;
		n->assign();
		if (tbl == gtbl) n->dcl_print(0);	// a hack
		Pexpr c = call_ctor(tbl,n,ctor,e1,DOT);
		c = new expr(G_CM,c,n);
		c->tp = n->tp;
//error('d',"tp1 %t",c->tp);
		return c;
	}
	else {
		Pexpr c = call_ctor(tbl,e2,ctor,e1,DOT);
		c = new expr(DEREF,c,0); // deref value returned by constructor
		c->tp = c->e1->tp;
//error('d',"tp2 %t",c->tp);
		return c;
	}
}

