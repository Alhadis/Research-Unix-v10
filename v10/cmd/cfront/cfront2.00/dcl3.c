/*ident	"@(#)ctrans:src/dcl3.c	1.6.5.29" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

dcl3.c:
	Routines used by ::dcl fucntions: fct::dcl() etc.

*****************************************************************************/

#include "cfront.h"
#include "size.h"

extern loc no_where;
static void vbase_pointers(Pname fn, Pclass cl)
/*
	insert argument for virtual base pointers (if any)
	after f_this and before f_argtype
*/
{
//error('d',"vbase_pointers(%n,%t) %d %k",fn,cl,fn->tp,fn->n_oper);
	Pfct f = Pfct(fn->tp);
	if (fn->n_oper == CTOR) {
		Pname d = 0;
		for (Pbcl b = cl->baselist; b; b=b->next) {
			if (b->base != VIRTUAL) continue;
			Pname a = new name(b->bclass->string);
			a->tp = b->bclass->this_type;
			a->n_list = d;
			a->n_table = f->body ? f->body->memtbl : 0;
			a->where = no_where;
			d = a;
		}	
				
		if (d) {
			for (Pname dd =d;;) {
				if (d->n_list == 0) {
					d->n_list = f->f_args->n_list;
					break;
				}
				d = d->n_list;
			}
			f->f_args->n_list = dd;
		}
	}
	if (fn->n_oper == DTOR) {	// add __free argument
//error('d',"add __free to %n",fn);
		Pname fa = new name;
		fa->tp = int_type;
		fa->n_scope = ARG;
		fa->where = no_where;

		Pname a = f->f_args;
		if (a == 0)
			f->f_args = fa;
		else {
			for(;;a = a->n_list) {
// error('d',"a %d %t",a,a->tp);
				if (a->n_list == 0) {
					a->n_list = fa;
					break;
				}
			}
		}
	}
}

void make_res(Pfct f)
/*
	returns X where X(X&) has been declared
	add "_result" argument of type X*
*/
{
	Pname cl = f->returns->is_cl_obj();
	if (cl==0 || Pclass(cl->tp)->has_itor()==0) return;

	Pname rv = new name("_result");
	rv->tp  = f->returns->addrof();
	rv->n_scope = FCT;	// not a ``real'' argument
	rv->n_used = 1;
	rv->n_list = f->argtype;
	if (f->f_this)
		f->f_this->n_list = rv;
	else
		f->f_args = rv;
	f->f_result = rv;
	f->s_returns = void_type;
}

void name::check_oper(Pname cn)
/*
	check declarations of operators, ctors, dtors
*/
{
	switch (n_oper) {
	case CALL:
	case DEREF:
	case REF:
		if (cn == 0) error("operator%s must be aM",keys[n_oper]);
		break;
//	case ASPLUS:
//	case ASMINUS:
//	case ASMUL:
//	case ASDIV:
//	case ASMOD:
//	case ASAND:
//	case ASOR:
//	case ASER:
//	case ASLS:
//	case ASRS:
	case ASSIGN:
		if (cn == 0) error('w',"operator%k() must be aM (thisD will soon cease to compile)",n_oper);
		break;
	case 0:
	case TNAME:	/* may be a constructor */
		if (cn && strcmp(cn->string,string)==0) {
			if (tp->base == FCT) {
				Pfct f = Pfct(tp);
				if (f->returns!=defa_type)
					error("%s::%s()W returnT",string,string);
				f->returns = void_type;
				string = "__ct";
				n_oper = CTOR;
			}
			else
				error('s',"struct%nM%n",cn,cn);
		}
		else
			n_oper = 0;
		break;
	case DTOR:	/* must be a destructor */
//error('d',"dtor %s",string);
		if (cn == 0) {
			n_oper = 0;
			error("destructor ~%s() not inC",string);
		}
		else if (strcmp(cn->string,string) == 0) {
		dto:
			Pfct f = (Pfct)tp;
			string = "__dt";
			if (tp->base != FCT) {
				error("%s::~%s notF",cn->string,cn->string);
				tp = new fct(void_type,0,1);
			}
			else if (f->returns!=defa_type/* && f->returns!=void_type*/)
				error("%s::~%s()W returnT",cn->string,cn->string);
			if (f->argtype) {
				error("%s::~%s()WAs",cn->string,cn->string);
				f->nargs = 0;
				f->nargs_known = 1;
				f->argtype = 0;
			}
			f->returns = void_type;
		}
		else {
			if (strcmp(string,"__dt") == 0) goto dto;
			error("~%s in %s",string,cn->string);
			n_oper = 0;
		}
		break;
	case TYPE:
//error('d',"type %t",n_initializer);
		if (cn == 0) {
			error("operator%t() not aM",Ptype(n_initializer));
			n_oper = 0;
			n_initializer = 0;
		}
		else {
			Pfct f = Pfct(tp);
			Ptype tx = Ptype(n_initializer);
			n_initializer = 0;
			if (f->base != FCT) error("badT for%n::operator%t()",cn,tx);
			if (f->returns != defa_type) {
			//	if (f->returns->check(tx,0)) error("bad resultT for%n::operator%t()",cn,tx);
				error("resultT for%n::operator%t()",cn,tx);
				DEL(f->returns);
			}
			if (f->argtype) {
				error("%n::operator%t()WAs",cn,tx);
				f->argtype = 0;
			}
			f->returns = tx;
			Pname nx = tx->is_cl_obj();
			if (nx && can_coerce(tx,cn->tp)) error("both %n::%n(%n) and %n::operator%t()",cn,cn,nx,tx);
			char buf[256];
			char* bb = tx->signature(buf);
			int l2 = bb-buf;
			if (255<l2) error('i',"N::check_oper():N buffer overflow");
			char* p = new char[l2+5];
			p[0] = '_';
			p[1] = '_';
			p[2] = 'o';
			p[3] = 'p';
			strcpy(p+4,buf);
			string = p;
		}
		break;
	}
}

Pexpr vbase_args(Pfct a, Pname bn)
/*
	constructor a calls the constructor bn for a base class
	generate argument list needed for virtual base arguments
*/
{
	Pfct b = Pfct(bn->tp);
//error('d',"vbase_args%n: %t %k",bn,b,b->base);
	Pexpr args = 0;
	Pexpr tail = 0;
	if (b->base == OVERLOAD) b = Pfct(Pgen(b)->fct_list->f->tp);	// doesn't matter which
	for (Pname d = b->f_args->n_list; d!=b->argtype; d=d->n_list) {
		for (Pname dd = a->f_args->n_list; dd; dd=dd->n_list)
			// using strcmp is a trick
			if (strcmp(dd->string,d->string)==0) break;

		Pexpr aa = new expr(ELIST,dd,0);
		if (args == 0)
			args = aa;
		else
			tail->e2 = aa;
		tail = aa;
	}
	return args;
}

void fct::init_bases(Pclass cl, Pexpr)
/*
	in "cl"'s constructor "this" generate code to initialize base classes
	and members using the initializers "f->f_init"
	
	this->f_init == list of names of classes to be initialized
		COLON(b)	=> base class b
				=> constructor call in f_init->n_initializer
		COLON()		=> unnamed base class
				=> constructor call in f_init->n_initializer
		NAME(m)		=> member m
				=> constructor call in m->n_initializer
*/
{
	Ptable ftbl = body->memtbl;
//error('d',"init_bases %t init %d",cl,f_init);

	// explicit initializers
	for (Pname nx, nn=f_init; nn; delete nn,(nn=nx) ) {
		Pexpr i = nn->n_initializer;
		nn->n_initializer = 0;
		nx = nn->n_list;

//error('d',"init_base %s %d",nn->string,i);
		if (nn->string) {
			if (nn->base == TNAME) {	// base class
				for (Pbcl l = cl->baselist; l; l=l->next) {
					Pclass bcl = l->bclass;
					if (strcmp(bcl->string,nn->string) == 0) {
						if (l->init)
							error("twoIrs for%t",bcl);
						else
							l->init = base_init(bcl,i,ftbl,l->obj_offset);
						goto con;
					}
				}
				error("unexpectedAL: noBC%n",nn);
			con:
				continue;
			}
			else {				// member initializer
				Pname m = cl->memtbl->look(nn->string,0);
				if (m && m->n_table==cl->memtbl)
					m->n_initializer = mem_init(m,i,ftbl);
				else
					error("%n not inC%n",m,nn);
			}
		}
		else {			// unnamed base class
			Pbcl l = cl->baselist;
			if (l == 0) {
				error("unexpectedAL: noBC called");
				continue;
			}

			if (l->next) {
				error("unnamedBCIr: multipleBCs");
				continue;
			}
			
			if (l->init)
				error("twoIrs for%t",l->bclass);
			else {
				if (strict_opt) error("anachronism:N ofBC %t missing",l->bclass);
				l->init = base_init(l->bclass,i,ftbl,l->obj_offset);
			}
		}
	} // for

	for (Pbcl l = cl->baselist; l; l=l->next) {
		// default initialization of base classes
		Pname ctor;
		Pclass bcl = l->bclass;
		if (l->init==0 && (ctor=bcl->has_ctor()))
			l->init = base_init(bcl,0,ftbl,l->obj_offset);
	}
}

int inline_restr;	/* report use of constructs that the inline expanded
			   cannot handle here
			*/

void fct::dcl(Pname n)
{
	int nmem = TBLSIZE;
	Pname a;
	Pname ll;
	Ptable ftbl;

	Pptr cct = 0;
	int const_old = const_save;

	int bit_old = bit_offset;
	int byte_old = byte_offset;
	int max_old = max_align;

	if (base != FCT) error('i',"F::dcl(%d)",base);
	if (body == 0) error('i',"F::dcl(body=%d)",body);
	if (n==0 || n->base!=NAME) error('i',"F::dcl(N=%d %d)",n,(n)?n->base:0);
//error('d',"fct::dcl(%n) %k %d %t",n,n->n_scope,body->own_tbl,this);
	if (body->own_tbl) return;	// done already

//	if (f_inline==0 /*|| debug_opt*/) n->n_dcl_printed = 1;	// beware of recursive calls, no decl needed
//	if (f_inline && debug_opt) n->n_dcl_printed = 2;
	if (body->memtbl == 0) body->memtbl = new table(nmem+3,gtbl,0);
	body->own_tbl = 1;
	ftbl = body->memtbl;
	ftbl->real_block = body;

	max_align = 0;//AL_FRAME;
	bit_offset = 0;

	cc->stack();
	cc->nof = n;
	cc->ftbl = ftbl;

	switch (n->n_scope) {
	case 0:
	case PUBLIC:
	{	cc->not = n->n_table->t_name;
		cc->cot = Pclass(cc->not->tp);
		cc->tot = cc->cot->this_type;
	//	if (f_this==0 || cc->tot==0) error('i',"F::dcl(%n): f_this=%d cc->tot=%d",n,f_this,cc->tot);
		if (f_this) f_this->n_table = ftbl;	// fake for inline printout
		cc->c_this = f_this;


		Pclass cl = Pclass(cc->not->tp);

		if (cl->c_body!=3
		|| n->n_initializer
		|| n->n_sto==STATIC
		|| f_inline
		|| f_imeasure
		|| f_virtual==0)
			;
		else {		// could be the function where we need to
				// output the vtbl
			int i;
			for (Pname nn=cl->memtbl->get_mem(i=1); nn; nn=cl->memtbl->get_mem(++i) ) {
			Ptype t = nn->tp;
			if (t)
			switch (t->base) {
			case FCT:
				if (nn == n) goto prnt;
				if (nn->n_initializer
				|| nn->n_sto==STATIC
				|| Pfct(nn->tp)->f_inline
				|| Pfct(nn->tp)->f_imeasure
				|| Pfct(nn->tp)->f_virtual==0) break;
				goto zaq;

			case OVERLOAD:
			{	for (Plist gl=Pgen(t)->fct_list; gl; gl=gl->l) {
					Pname nn = gl->f;
					if (nn == n) goto prnt;
					if (nn->n_initializer
					|| nn->n_sto==STATIC
					|| Pfct(nn->tp)->f_inline
					|| Pfct(nn->tp)->f_imeasure
					|| Pfct(nn->tp)->f_virtual==0) continue;
					goto zaq;
				}
			}
			}
		}
			goto zaq;
		prnt:
			cl->print_all_vtbls(cl);
			goto zaq;
		}
	}
	}
zaq:
		// protect against: class x; x f(); class x { x(x&); ....
	if (f_result == 0) make_res(this);
	if (f_result) f_result->n_table = ftbl;		// fake for inline printout

	returns->tsizeof();	// make sure size is known

	Pname ax;
	for (a=argtype, ll=0; a; a=ax) {
		ax = a->n_list;
		Pname nn = a->dcl(ftbl,ARG);
		Pname cn = nn->tp->is_cl_obj();
		if (cn == 0) cn = cl_obj_vec;
		if (cn) (void)cn->tp->tsizeof();	// make sure it is printed
		nn->n_assigned_to = nn->n_used = nn->n_addr_taken = 0;
		nn->n_list = 0;

		switch (nn->tp->base) {
		case CLASS:
		case ENUM:	/* unlink types declared in arg list */
			nn->dcl_print(0);
			break;
		default:
			if (ll)
				ll->n_list = nn;
			else
				f_args = argtype = nn;
			ll = nn;
		}
		delete a;
	}

	if (f_result) {		// link in f_result
		f_args = f_result;
		f_result->n_list = argtype;
	}

	if (f_this) {		// link in f_this
		f_args = f_this;
		f_this->n_list = f_result ? f_result : argtype;
	}

	if (n->n_oper==CTOR || n->n_oper==DTOR) vbase_pointers(n,cc->cot);

	if (n->n_oper == CTOR) {
		const_save = 1;
		init_bases(cc->cot,f_init);
	}
	else if (f_init)
		error(0,"unexpectedAL: not aK");

	PERM(returns);
	const_save = f_inline&&debug_opt==0;
	inline_restr = 0;
	body->dcl(ftbl);
	
	defined |= DEFINED;
	if (f_inline && inline_restr && returns->base!=VOID) {
		f_inline = 0;
		char* s = (inline_restr & 32) ? "continue"
			: (inline_restr & 16) ? "break"
			: (inline_restr & 8) ? "loop"
			: (inline_restr & 4) ? "switch"
			: (inline_restr & 2) ? "goto"
			: (inline_restr & 1) ? "label"
			: "" ;
		error('w', "\"inline\" ignored, %n contains %s",n,s);
	//	if (cc->cot)
		n->simpl();	//BS6
		n->dcl_print(0);
	}
	const_save = const_old;

	if (f_inline && debug_opt==0) isf_list = new name_list(n,isf_list);

	bit_offset = bit_old;
	byte_offset = byte_old;
	max_align = max_old;
	cc->unstack();
//error('d',"fct-> returns %t",returns);
}

Pexpr fct::base_init(Pclass bcl, Pexpr i, Ptable ftbl, int offset)
/*
	have base class bcl and expr list i
	return "( *(base*)this ) . ctor( i )"
	ctor call generated in expr.typ()
*/
{
	Ptype ty = bcl->this_type;
	Pexpr th = rptr(ty,f_this,offset);		// base*
	Pname ctor = bcl->has_ctor();

//error('d',"fct::B_init(C %t, i %d, %d) ctor %n",bcl,i,i?i->tp:0,ctor);

	Pexpr ii = (i && i->base==ELIST)?i->e1:i;

	if (ii
	&& ii->base==DEREF
	&& ii->e1->base==CAST
	&& th->base==CAST) th->i2 = ii->e1->i2;

	if (ctor == 0) {
		if (i && i->base!=ELIST) i = new expr(ELIST,i,0);

		Pexpr v = new texpr(VALUE,bcl,i);	// ?.base(i)
		v->e2 = new expr(DEREF,th,0);		// (*(base*)this).base(i)
		v = v->typ(ftbl);			// *base(&*(base*)this,i)
//error('d',"v %k",v->base);
		switch (v->base) {
		case DEREF:
			return v->e1;			// base(&*(base*)this,i)
		case ASSIGN:		// degenerate base(base&): *(base*)this=i
			th = new texpr(CAST,ty,f_this);
			v = new expr(CM,v,th);		// (*(base*)this=i,(base*)this);
			return v->typ(ftbl);
		default:
			return 0;
		}
	}


	Pname icn;
	if (i) {
		ii = ii->typ(ftbl);
		if (bcl->has_itor()==0
		&& (icn=ii->tp->is_cl_obj())
		&& (Pclass(icn->tp)==bcl || Pclass(icn->tp)->has_base(bcl))) {
			// degenerate base(base&): *(base*)this=i
			// memberwise copy
//error('d',"copy %t",ty);
		//	th = new cast(ty,f_this);
		//	th = th->contents();
			th = new texpr(CAST,ty,f_this);
			th = th->contents();
			th = th->typ(ftbl);
			if (Pclass(icn->tp)!=bcl) {	// cast needed
				Pptr r = new ptr(RPTR,Pptr(ty)->typ);
				ii = new texpr(CAST,r,ii);
				ii = ii->typ(ftbl);
			}
			ii = new expr(ASSIGN,th,ii);
			ii->tp = th->tp;
					// simulate `return this':
					// *(base*)this=i,(base*)this
			ii = new expr(CM,ii,new cast(ty,f_this));
			ii->tp = th->tp;
			return ii;
		//	return ii->typ(ftbl);	// don't find cl::operator=()
		}
		if (i->base == ELIST) i->e1 = ii;
	}
//Pexpr x = call_ctor(ftbl,th,ctor,i,REF,vbase_args(this,ctor));
//error('d',"call %n %t -> %d %k",ctor,ctor->tp,x,x->base);
//	return x;
	return call_ctor(ftbl,th,ctor,i,REF,vbase_args(this,ctor));
}


Pexpr fct::mem_init(Pname mn, Pexpr i, Ptable ftbl)
/*
	return "member_ctor( m, i )"
*/
{
// a new entry for B::B_pub, in general, has no tp and no
// real info: all the tp-> only work on our systems because
// 0 pointer dereference isn't system memory.  it core dumps
// in set_const since no test is made on this == 0.
//error('d',"mem_init %n",mn);

//	if (mn->n_stclass == STATIC) error('s',"MIr for static%n",mn);
        switch (mn->n_stclass) {
        case STATIC:
                error("MIr for static%n",mn);
                break;
        case ENUM:
                error("MIr for enumeration constant%n", mn);     
                break;
        }

	Pname member = (mn->base==PUBLIC && mn->n_qualifier) ? mn->n_qualifier : mn;

	if (i) i = i->typ(ftbl);
	Pname cn = member->tp->is_cl_obj();	// first find the class name

//	if (member->n_stclass == STATIC) error('s',"MIr for static%n",member);
//	if (i) i = i->typ(ftbl);
//	Pname cn = member->tp->is_cl_obj();	// first find the class name
	Pref tn = new ref(REF,f_this,member);
	tn->tp = member->tp;
//error('d',"MI for %n %t = %t",member,member->tp,i?i->tp:0);
//error('d',"fthis %d %t member %n tp %t",f_this,f_this->tp,member,tn->tp);
	if (cn) {
		Pclass mcl = Pclass(cn->tp);	// then find the classdef
		Pname ctor = mcl->has_ctor();
		Pname icn;

		if (i
		&& mcl->has_itor()==0
		&& (icn=i->tp->is_cl_obj())
		&& Pclass(icn->tp)==mcl) {	// bitwise copy
			Pexpr init = new expr(ASSIGN,tn,i);
			init->tp = tn->tp;
		//	return init->typ(ftbl);	// don't look for mcl.operator=()
			member->assign();
			return init;
		}
		
		if (ctor) return call_ctor(ftbl,tn,ctor,i,DOT);

		error("Ir forM%nW noK",member);
		return 0;
	}

	if (cl_obj_vec) {
		error('s',"Ir forCM arrayWK");
		return 0;
	}

	if (i && i->base == ELIST) {
	// if (i->base == ELIST) {
		if (i->e2) error("Ir for%n not a simpleE",member);
		i = i->e1;
	}

// error( 'd', "fct_mem_init: %n %k", member, member->tp->base );
	switch (member->tp->base) {
	case RPTR:
		if ( i == 0 ) {
			error( "empty Ir for reference %n", member );
			return 0;
		}
		break;
	case VEC:
	case FCT:
	case OVERLOAD:
		error("Ir for%n ofT %t",member,member->tp);
		return 0;
	}

//error('d',"tp %t",member->tp);
	if (member->tp->tconst()) {
                extern ignore_const;
                int save_ignore_const = ignore_const;
                ignore_const = 1;
                i = new expr(ASSIGN,tn,i);
                i = i->typ(ftbl);
                ignore_const = save_ignore_const;
                return i;
	}

	Pptr pt;
	if (pt = member->tp->is_ref()) {
		switch (pt->typ->base) {
		case FCT:
		case OVERLOAD:
			i = ptr_init(pt,i,ftbl);
			break;
		default:
			i = ref_init(pt,i,ftbl);
		}
		i = new expr(ASSIGN,tn,i);
		i->tp = tn->tp;
		member->assign();	// cannot call typ: would cause dereference
		return i;
	}

	i = new expr(ASSIGN,tn,i);
	return i->typ(ftbl);	// typ performs the type check on the assignment
}

Pexpr replace_temp(Pexpr e, Pexpr n)
/*
	e is on the form
				f(&temp,arg) , temp
			or
				&temp->ctor(arg) , temp
			or
				x->f(&temp,arg) , temp
	change it to
				f(n,arg)
			or
				n->ctor(arg)
*/
{
	Pexpr c = e->e1;	// f(&temp,arg) or &temp->ctor(args)
	Pexpr ff = c->e1;
	Pexpr a = c->e2;	// maybe ELIST(&temp,arg)
	Pexpr tmp = e->e2;

//error('d',"suppress(%d %k) %n",tmp->base,tmp->base,tmp->base==NAME?tmp:0);
	if (tmp->base==DEREF) tmp = tmp->e1;
	if (tmp->base==CAST) tmp = tmp->e1;
	if (tmp->base==ADDROF || tmp->base==G_ADDROF) tmp = tmp->e2;
        if (tmp->base != NAME) return e; //error('i',"replace %k",tmp->base);
	tmp->tp = any_type;	// temporary not used: suppress it

//error('d',"replace_temp(%k %k) c %k ff %k",e->base,n->base,c->base,ff->base);
	switch (ff->base) {
	case REF:
		if (ff->e1->base==G_ADDROF && ff->e1->e2==tmp)
			a = ff;				// &tmp -> f()
		break;
	case DOT:
		if (ff->e1->base==NAME && ff->e1==tmp) {
			a = ff;				// tmp . f()
			a->base = REF;
		}
		break;
	}
	a->e1 = n;
	return c;
}

Pname classdef::has_ictor()
/*
	does this class have a constructor taking no arguments?
*/
{
	Pname c = has_ctor();
	if (c == 0) return 0;
	
	Pfct f = Pfct(c->tp);

	switch (f->base) {
	default:
		error('i',"%s: badK (%k)",string,c->tp->base);
	
	case FCT:
		switch (f->nargs) {
		case 0:		return c;
		default:	if (f->argtype->n_initializer) return c;
		}
		return 0;

	case OVERLOAD:
	{	for (Plist l=Pgen(f)->fct_list; l; l=l->l) {
			Pname n = l->f;
			f = (Pfct)n->tp;
			switch (f->nargs) {
			case 0:		return n;
			default:	if (f->argtype->n_initializer) return n;
			}
		}
		return 0;
	}
	}
}

int add_first; // fudge, use ctor arg instead
Pname gen::add(Pname n)
/*
	add "n" to the tail of "fct_list"
	(overloaded names are searched in declaration order)

	detect:	 	multiple identical declarations
			declaration after use
			multiple definitions
*/
{
	extern linkage;
	Pfct f = Pfct(n->tp);
	Pname nx;
//error('d',"add(%n) %d",n,add_first);
	if (f->base != FCT) error("%n: overloaded nonF",n);

	if ( fct_list && (nx=find(f,1)) ) {
//error('d',"found %n %t",nx,nx->tp);
		if (linkage && Pfct(nx->tp)->f_signature && Pfct(nx->tp)->f_signature[0])
			error("inconsistent linkage specifications for%n",n);
		Nold = 1;
	}
	else {
//extern linkage;
		if (add_first==0)
		if (linkage==0 && f->f_signature==0) f->sign();
//error('d',"signature: %d \"%s\" fct_list %d",f->f_signature,f->f_signature,fct_list);
		nx = new name;
		*nx = *n;
		nx->n_tbl_list = Pname(n->string);
		PERM(nx);
		Nold = 0;
		if (fct_list) {
			int clink = f->f_signature==0 || *f->f_signature==0;	// function with C linkage
			Plist gl=fct_list;
			for(;;) {
				if (clink
				&& (Pfct(gl->f->tp)->f_signature==0
					|| *Pfct(gl->f->tp)->f_signature==0)) {
					error("two%ns with c linkage",n);
					f->sign();
				}
				if (gl->l)
					gl = gl->l;
				else
					break;
			}
			gl->l = new name_list(nx,0); 
		}
		else
			fct_list = new name_list(nx,0);
		nx->n_list = 0;
	}
	return nx;
}

void fct::sign()
{
	char buf[256];
	char* bb = signature(buf);
	int ll = bb-buf;
	if (255 < ll) error('i',"gen::add():N buffer overflow");
	char* p = new char[ll+1];
	strcpy(p,buf);
	f_signature = p;
//error('d',"fct::sign %s",p);
}

Pname gen::find(Pfct f, bit warn)
{	
	for (Plist gl=fct_list; gl; gl=gl->l) {
		Pname n = match(gl->f,f,warn);
		if (n) return n;
	}
	return 0;
}

Pname gen::match(Pname nx, Pfct f, bit warn)
{
	Pfct fx = Pfct(nx->tp);
	Pname a, ax;
	int op = 0;	// overloading problem: const, ref, vec/ptr, or basetype
//error('d',"fx %d %d f %d %d",fx->nargs_known,fx->nargs,f->nargs_known,f->nargs);

	if (f->nargs_known != fx->nargs_known) return 0; // the bets are off
							 // must rely on checks at
							 // call points
	if (f->f_const != fx->f_const) return 0;

	if (fx->nargs != f->nargs
	&& fx->nargs_known==1
	&& f->nargs_known==1) return 0;	// no warning for potential
					// problems due to default args
	
	for (ax=fx->argtype, a=f->argtype; a&&ax; ax=ax->n_list, a=a->n_list) {
		Ptype at = ax->tp;
		Ptype atp = a->tp;
//error('d',"at %t atp %t",at,atp);
		if (at->check(atp,OVERLOAD) == 0) {
//error('d',"at %t atp %t cp %d vrp %d",at,atp,const_problem,vrp_equiv);
//			if (vrp_equiv) op++; // or differ only by [] vs *
			continue;
		}
//error('d',"warn %d",warn);
		if (warn == 0) goto xx;

		/*
			warn against:
				overload f(X&), f(X);		error
				overload f(int), f(const);	error
				overload f(int*), f(int[10]);	warn
			etc.
		*/

//error('d',"vrp_equiv %d const_problem %d",vrp_equiv,const_problem);
		if (const_problem) {	// differ only in X vs const X
			if (at->is_ptr_or_ref()) return 0;
			op++;
			continue;
		}

	aaa:
		switch (atp->base) {
		case TYPE:
			atp = Pbase(atp)->b_name->tp;
			goto aaa;
	//	case EOBJ:
	//		atp = Penum(Pbase(atp)->b_name->tp)->e_type;
	//		goto aaa;
		case RPTR:	// differ only by X vs X& ?
			if (Pptr(atp)->typ->check(at,0)==0) {
				op++;
				continue;
			}
		}

	atl:
		switch (at->base) {
		case TYPE:
			at = Pbase(at)->b_name->tp;
			goto atl;
	//	case EOBJ:
	//		at = Penum(Pbase(at)->b_name->tp)->e_type;
	//		goto atl;
		case RPTR:	// differ only by X& vs X ?
			if (Pptr(at)->typ->check(atp,0)==0) {
				op++;
				continue;
			}
			break;
	//	case CHAR:	// differ only by int vs char ?
	//	case SHORT:
	//	case INT:
		//	if (atp->base!=at->base && atp->base==EOBJ) {
		//		op++;
		//		continue;
		//	}
	//		break;
		}
//error('d',"return 0");
		//goto xx;
		// some argument is really different
		// e.g. f(int), f(char*);
		return 0;
	}

	// arguments checked. Now look at leftover args, return type,etc.

//	if (warn && a && fx->nargs_known==ELLIPSIS) error('w',"... in%n'sAT hidesATs from the overloading mechanism",nx);

	if (a || ax) return 0;

	if (op == 0) {
		if (warn && fx->returns->check(f->returns,0))
			error("two different return valueTs for%n: %t and %t",nx,fx->returns,f->returns);

		return nx;
	}
xx:
	if (warn && op)
		error("the overloading mechanism cannot tell a%t from a%t",fx,f);

	return 0;
}

int name::no_of_names()
{
	register int i = 0;
	register Pname n;
	for (n=this; n; n=n->n_list) i++;
	return i;
}

static Pexpr lvec[20], *lll, *curr_e;
static Pexpr list_back = 0;

void new_list(Pexpr lx)
{
	if (lx->base != ILIST) error('i',"IrLX");

	lll = lvec;
	lll++;
	*lll = lx->e1;
}

Pexpr next_elem()
{
	Pexpr e;
	Pexpr lx;

	if (lll == lvec) return 0;

 	lx = *lll;

	if (list_back) {
		e = list_back;
		list_back = 0;
		return e;
	}

	if (lx == 0) {				/* end of list */
		lll--;
		return 0;
	}

	switch (lx->base) {
	case ELIST:
		e = lx->e1;
		curr_e = &lx->e1;
		*lll = lx->e2;
		switch (e->base) {
		case ILIST:
			lll++;
			*lll = e->e1;
			return Pexpr(1);	// start of new ILIST
		case ELIST:
			error("nestedEL");
			return 0;
		default:
		{
int need_sti(Pexpr, Ptable tbl = 0);
			if (need_sti(e)) error('s',"generalIr inIrL");
			return e;
		}
		}
	case IVAL:
	case ZERO:
		lll--;
		return 0;
	default:
		error('i',"IrL %k",lx->base);
	}
}

void skip_ilist()
// skip ilist use to represent pointer to member function literal
{
	Pexpr e = next_elem();
	e = next_elem();
}

void list_check(Pname nn, Ptype t, Pexpr il)
/*
	see if the list "lll" can be assigned to something of type "t"
	"nn" is the name of the variable for which the assignment is taking place.
	"il" is the last list element returned by next_elem()
*/
{
	Pexpr e;
	bit lst = 0;
	int i;
	Pclass cl;
	int tdef = 0;

//error('d',"list_check%n: %t (%d)",nn,t,il);
	if (il == Pexpr(1))
		lst = 1;
	else if (il)
		list_back = il;

zzz:
	switch (t->base) {
	case TYPE:
		t = Pbase(t)->b_name->tp;
		tdef = 1;
		goto zzz;

	case VEC:
	{	Pvec v = Pvec(t);
		Ptype vt = v->typ;

		if (v->size) {	/* get at most v->size initializers */
			if (v->typ->base == CHAR) {
				e = next_elem();
				if (e->base == STRING) {	// v[size] = "..."
					int isz = Pvec(e->tp)->size;
					if (v->size < isz) error("Ir too long (%d characters) for%n[%d]",isz,nn,v->size);
					break;
				}
				else
					list_back = e;
			}
			for (i=0; i<v->size; i++) { // check next list element type
			ee:
				e = next_elem();
				if (e == 0) goto xsw; // too few initializers are ok
			vtz:
//error('d',"vtz: %d",vt->base);
				switch (vt->base) {
				case TYPE:
					vt = Pbase(vt)->b_name->tp;
					goto vtz;
				case VEC:
				case COBJ:
					list_check(nn,vt,e);
					break;
				case PTR:
					if (Pptr(vt)->memof
					&& Pptr(vt)->typ->base==FCT
					&& e==Pexpr(1)) {
						skip_ilist();
						break;
					}
					// no break
				default:
				{
					if (e == (Pexpr)1) {
						error("unexpectedIrL");
						goto ee;
					}

					if (vt->check(e->tp,ASSIGN))
						error("badIrT for%n:%t (%tX)",nn,e->tp,vt);

					Pptr p;
					if (vt->check(e->tp,0)
					&& (p=vt->is_ptr())
					&& Ptype(p)!=zero_type
					&& p->typ!=char_type)
						*curr_e = new cast(vt,e);
				}
				}
			}
			if ( lst && (e=next_elem()) ) error("end ofIrLX after array");
		xsw:;
		}
		else {		/* determine v->size */
			i = 0;
			(void) v->typ->tsizeof();
		xx:
			while ( e=next_elem() ) {	// get another initializer
				i++;
			vtzz:
//error('d',"vtzz");
				switch (vt->base) {
				case TYPE:
					vt = Pbase(vt)->b_name->tp;
					goto vtzz;
				case VEC:
				case COBJ:
					list_check(nn,vt,e);
					break;
				case PTR:
					if (Pptr(vt)->memof
					&& Pptr(vt)->typ->base==FCT
					&& e==Pexpr(1)) {
						skip_ilist();
						break;
					}
					// no break
				default:
				{	if (e == Pexpr(1)) {
						error("unexpectedIrL");
						goto xx;
					}

					if (vt->check(e->tp,ASSIGN))
						error("badIrT for%n:%t (%tX)",nn,e->tp,vt);

					Pptr p;
					if (vt->check(e->tp,0)
					&& (p=vt->is_ptr())
					&& Ptype(p)!=zero_type
					&& p->typ!=char_type)
						*curr_e = new cast(vt,e);
				}
				}
			}
			if (tdef==0) v->size = i;
		}
		break;
	}

	case CLASS:
		cl = Pclass(t);
		goto ccc;

	case COBJ:	/* initialize members */
		cl = Pclass(Pbase(t)->b_name->tp);
	ccc:
		if (cl->defined == 0) {
			lll = lvec;	// we are lost: ignore rest of list
			return;
		}

		if (cl->c_body == 1) cl->dcl_print(0);

	{	Ptable tbl = cl->memtbl;
		Pname m;

		if (cl->baselist) {
			if (cl->baselist->next) error("IrL forO ofC with multipleBCs");
			list_check(nn,cl->baselist->bclass,0);
		}

		for (m=tbl->get_mem(i=1); m; m=tbl->get_mem(++i)) {
			Ptype mt = m->tp;
			switch (mt->base) {
			case FCT:
			case OVERLOAD:
			case CLASS:
			case ENUM:
				continue;
			}
			if (m->n_stclass == STATIC) continue;
			/* check assignment to next member */
		dd:
			e = next_elem();
			if (e == 0) return; //break;
		mtz:
//error('d',"mtz%n: %d",m,mt->base);
			switch (mt->base) {
			case TYPE:
				mt = Pbase(mt)->b_name->tp;
				goto mtz;
			case CLASS:
			case ENUM:
				break;
			case VEC:
			case COBJ:
				list_check(nn,m->tp,e);
				break;
			case PTR:
				if (Pptr(mt)->memof
				&& Pptr(mt)->typ->base==FCT
				&& e==Pexpr(1)) {
					skip_ilist();
					break;
				}
				// no break
			default:
			{	if (e == Pexpr(1)) {
					error("unexpectedIrL");
					goto dd;
				}

				if (mt->check(e->tp,ASSIGN))
					error("badIrT for%n:%t (%tX)",m,e->tp,m->tp);

				Pptr p;
				if (mt->check(e->tp,0)
				&& (p=mt->is_ptr())
				&& Ptype(p)!=zero_type
				&& p->typ!=char_type)
					*curr_e = new cast(mt,e);
			}
			}
		}
		if (lst && (e=next_elem()) ) error("end ofIrLX afterCO");
		break;
	}
	default:
		e = next_elem();

		if (e == 0) {
			error("noIr forO");
			break;
		}
		
		if (e == Pexpr(1)) {
			error("unexpectedIrL");
			break;
		}
//error('d',"t %t e->tp %t",t,e->tp);
		if (t->check(e->tp,ASSIGN)) error("badIrT for%n:%t (%tX)",nn,e->tp,t);
		Pptr p;
		if (t->check(e->tp,0)
		&& (p=t->is_ptr())
		&& Ptype(p)!=zero_type
		&& p->typ!=char_type)
			*curr_e = new cast(t,e);
		if (lst && (e=next_elem()) ) error("end ofIrLX afterO");
		break;
	}
}

Pname dclass(Pname n, Ptable tbl)
{	
	Pclass cl;
	Pbase bt;
	Pname bn;
	TOK t;

//local class	
	Pname nx = ktbl->look(n->string,0);		// TNAME
// error( 'd', "dclass n %n %d nx %d", n,n->lex_level, nx);
	if (nx == 0 || n->lex_level ) {
	/*	search for hidden name for
		(1) nested class declaration
		(2) local class declaration
	*/
		int tn = 0;
		t = n->lex_level ? LOCAL : HIDDEN;
// error( 'd', "dclass t: %k", t );
		for (nx=ktbl->look(n->string,t); nx; nx=nx->n_tbl_list) {
// error('d',"dclass: %n %t t: %s",n,n->tp, t==LOCAL?"Local":"Hidden");
// error('d',"dclass: nx: %s %k t: %s",nx->string,nx->tp->base, t==LOCAL?"Local":"Hidden");
			if (nx->n_key != t) continue;
			if ( nx->lex_level != n->lex_level ) continue;

			if (nx->tp->base != COBJ) {
				tn = 1;
				continue;
			}


			bt = (Pbase)nx->tp;
			bn = bt->b_name;
			cl = (Pclass)bn->tp;

			if (cl == 0) continue;

			if ( n->lex_level && 
   			(cl->lcl==0 || strcmp(cl->lcl,Pclass(n->tp)->lcl))) continue;
//error('d',"goto bbb");
			goto bbb;
		}

		if (tn)
			error("%n redefined using typedef",n);
		else
			error('i',"%n is not aCN",n);
	}
	else {
		bt = Pbase(nx->tp);			// COBJ
// error ('d', "dclass: bt->base: %k", bt->base );
                if ( bt->base != COBJ ) {
                        error("%n redefined using typedef",n);
                        Pname tn = ktbl->look(n->string,HIDDEN); 
// error( 'd', "dclass: tn: %t", tn );
                        if ( tn->tp->base == COBJ )  
                                bt = Pbase(tn->tp);
                        else error('i',"%n is not a CN", n );
                }
		bn = bt->b_name;
	}
bbb:
	Pname ln = tbl->look(bn->string,0);
//      if (ln && ln->n_table==tbl) error('w',"%n redefined",ln);
//error('d',"ln %n",ln);
	bn->where = nx->where;
	Pname bnn = tbl->insert(bn,CLASS);		// copy for member lookup
	cl = Pclass(bn->tp);
							// CLASS
	if (cl->defined&(DEFINED|SIMPLIFIED))
		error("C%n defined twice",n);
	else {
		if (bn->n_scope == ARG) bn->n_scope = ARGT;
		cl->dcl(bn,tbl);
	}
	n->tp = cl;
//error('d'," -> ");
	return bnn;
}

Pname denum(Pname n, Ptable tbl)
{
	Pname nx = ktbl->look(n->string,0);		// TNAME
	if (nx == 0) nx = ktbl->look(n->string,HIDDEN);	// hidden TNAME
	Pbase bt = (Pbase)nx->tp;			// EOBJ
	Pname bn = bt->b_name;
	Pname bnn = tbl->insert(bn,CLASS);
	Penum en = (Penum)bn->tp;			// ENUM
	if (en->defined&(DEFINED|SIMPLIFIED))
		error("enum%n defined twice",n);
	else {
		if (bn->n_scope == ARG) bn->n_scope = ARGT;
		en->dcl(bn,tbl);
	}
	n->tp = en;
	return bnn;
}

void dargs(Pname, Pfct f, Ptable tbl)
{
	int oo = const_save;
	const_save = 1;

	for (Pname a=f->argtype; a; a=a->n_list) {
		Pexpr init;

		if (a->tp == 0) {
			error( "A has noT" );
			a->tp = any_type;
			continue;
		}

		Pname cln = a->tp->is_cl_obj();
//error('d',"dargs %t",a->tp);
		if (cln && Pclass(cln->tp)->has_itor())	// mark X(X&) arguments
			a->n_xref = 1;
		else {
			Ptype t = a->tp;
			while (t->base == TYPE) t = Pbase(t)->b_name->tp;
			if (t->base == FCT) a->tp = new ptr(PTR,a->tp);
		}

		if (init = a->n_initializer) {	// default argument
			Pptr pt;
			if (init == dummy) {
				error("emptyIr");
				a->n_initializer = 0;
				continue;
			}
			if (cln) {
				if (init->base==VALUE) {
					switch (init->tp2->base) {
					case CLASS:
						if (Pclass(init->tp2)!=Pclass(cln->tp)) goto inin2;
						break;
					default:
						Pname n2 = init->tp2->is_cl_obj();
						if (n2==0 || Pclass(n2->tp)!=Pclass(cln->tp)) goto inin2;
					}

					a->n_initializer = init = 0;
					error('s',"K as defaultA");
				}
				else {
				inin2:
					if (init->base == ILIST) error("list as AIr");
					Pexpr i = init->typ(tbl);
					init = class_init(a,a->tp,i,tbl);
					if (i!=init && init->base==DEREF) {
						error('s',"K needed forAIr");
						init = 0;
					}
					else {
void dosimpl(Pexpr,Pname);
						dosimpl(init,cc->nof);
					//	init->simpl();
						init->permanent = 2;
					}
					a->n_initializer = init;
				}
			}
			else if (pt = a->tp->is_ref()) {
				extern ref_initializer;
				ref_initializer++;
				init = init->typ(tbl);
				ref_initializer--;
				int tcount = stcount;
				init = ref_init(pt,init,tbl);
				if (tcount != stcount) {
					error('s',"needs temporaryV to evaluateAIr");
					init = 0;
				}
				else {
void dosimpl(Pexpr,Pname);
						dosimpl(init,cc->nof);
				//	init->simpl();
					init->permanent = 2;
				}
				a->n_initializer = init;
			}
			else {
				Pptr p = a->tp->is_ptr();
				init = init->typ(tbl);
				if (p) init = ptr_init(p,init,tbl);

				if (a->tp->check(init->tp,ARG)) {
					int i = can_coerce(a->tp,init->tp);

					switch (i) {
					case 1:
						if (Ncoerce) {
							Pname cn = init->tp->is_cl_obj();							Pname xx = new name(Ncoerce->string);
							Pref r = new ref(DOT,init,xx);
							init = new expr(G_CALL,r,0);
							init = init->typ(tbl);
						}
						break;
					default:
						error("%d possible conversions for defaultA",i);
					case 0:
						error("badIrT%t forA%n",init->tp,a);
						DEL(init);
						a->n_initializer = init = 0;
					}
				}
				
				if (init) {
void dosimpl(Pexpr,Pname);
						dosimpl(init,cc->nof);
				//	init->simpl();
					init->permanent = 2;
					a->n_initializer = init;
					Neval = 0;
					long i = init->eval();
					if (Neval == 0) {
						a->n_evaluated = 1;
						a->n_val = i;
					}
				}
			}
		}
	}
	const_save = oo;
}

void merge_init(Pname nn, Pfct f, Pfct nf)
{
//	Pname a1 = f->f_args; if (a1==0) a1 = f->argtype;
//	Pname a2 = nf->f_args;//nf->argtype; 
	Pname a1 = f->argtype;
	Pname a2 = nf->argtype; 

	for (; a1; a1=a1->n_list, a2=a2->n_list) {
		int i1 = a1->n_initializer || a1->n_evaluated;
		int i2 = a2->n_initializer || a2->n_evaluated;

		if (i1 && i2) error(&a1->where,"twoIrs for%nA%n",nn,a1);

		if (i1) {
			a2->n_initializer = a1->n_initializer;
			a2->n_evaluated = a1->n_evaluated;
			a2->n_val = a1->n_val;
		}
		if (i2) {
			a1->n_initializer = a2->n_initializer;
			a1->n_evaluated = a2->n_evaluated;
			a1->n_val = a2->n_val;
		}

	}
}

Pexpr try_to_coerce(Ptype rt, Pexpr e, char* s, Ptable tbl)
/*
	``e'' is of class ``cn'' coerce it to type ``rt''
*/
{
	int i;
	Pname cn;
//error('d',"try_to_coerce(%t, %t, %s, %d)",rt,e->tp,s,tbl);

	if ((cn=e->tp->is_cl_obj()) && (i=can_coerce(rt,e->tp)) && Ncoerce) {
		if (1 < i) error("%d possible conversions for %s",i,s);
//error('d',"coerce %n",Ncoerce);
		Pclass cl = Pclass(cn->tp);
	//	Pref r = new ref(DOT,e,Ncoerce);
	//	Pexpr rr = r->typ(tbl);
	//	Pexpr c = new expr(G_CALL,rr,0);
	//	c->fct_name = Ncoerce;
		Pname xx = new name(Ncoerce->string);
		Pref r = new ref(DOT,e,xx);
		Pexpr c = new expr(G_CALL,r,0);
	//	return c->typ(tbl);
		c = c->typ(tbl);
//error('d',"coerce -> %k %t",c->base,c->tp);
		return c;
	}
//error('d',"coerce ->0");
	return 0;
}

int in_class_dcl;

Pname name::dofct(Ptable tbl, TOK scope)
{	
	Pfct f = Pfct(tp);
	Pname class_name;
	Ptable etbl;
	in_class_dcl = cc->not!=0;
	int just_made = 0;
//	int fvirt = 0;  //BSopt
//error('d',"dofct %n %d %t %s",this,tp,tp,tbl==gtbl?"global":"");

	if (f->f_inline) n_sto = STATIC;

	if (n_stclass)
		switch (n_stclass) {
		case EXTERN:
		case STATIC:
		case OVERLOAD:
			break;
		default:
			error("%n declared%k",this,n_stclass);
			n_stclass = EXTERN;
		}

	tp->dcl(tbl);		// must be done before the type check

	if (n_qualifier) {	// qualified name: c::f() checked above
	// ineffective
	//	if (in_class_dcl) {
	//		error("unexpectedQdN%n",this);
	//		return 0;
	//	}

          	if (f->f_virtual) {
 			 error("virtual outsideCD (ignored)");
			 f->f_virtual = 0;
		}

		if (n_sto
		&& n_sto!=FRIEND	// friend X::f();
		&& f->f_inline==0) {	// inline causes n_sto==STATIC
			error("%k specifed for QdN%n",n_sto,this);
			n_sto = 0;
		}

		class_name = Pbase(n_qualifier->tp)->b_name;
		etbl = Pclass(class_name->tp)->memtbl;
	}
	else {
		class_name = cc->not;
		// beware of local function declarations in member functions
		if (class_name && tbl!=cc->cot->memtbl) {
			class_name = 0;
			in_class_dcl = 0;
		}

		if (f->f_static && f->f_virtual) {
 			 error("virtual staticM");
			 f->f_virtual = 0;
		}

		if (n_oper) check_oper(class_name);
		etbl = tbl;
	}

//	Pfct(tp)->memof = class_name ? Pclass(class_name->tp) : 0;

Pname find_virtual(Pclass,Pname);

	if (class_name) {
		f->memof = Pclass(class_name->tp);
		if (f->f_virtual==0 && find_virtual(f->memof,this))
			f->f_virtual = VTOK;
        }

	if (etbl==0 || etbl->base!=TABLE) error('i',"N::dcl: etbl=%d",etbl);

	switch (n_oper) {
	case CTOR:
		if (f->f_virtual) {
			error("virtualK");
			f->f_virtual = 0;
		}

//	case DTOR:
//		f->f_const = 1;
		break;

	case REF:
		if (f->argtype) 
			error("%n takes no argument",this);
		else if (f->returns->is_ptr() == 0) {
			Pname cn = f->returns->is_cl_obj();
			if (cn==0 && f->returns->base==RPTR) cn = Pptr(f->returns)->typ->is_cl_obj();
			if (cn==0 || Pclass(cn->tp)->has_oper(REF)==0)
				error("%n must return aP toCO, aR toCO, or aCO",this);
		}
		break;

	case NEW:	// void* operator new(long)
		if (f->f_virtual)
			error("virtual%n (operator new() is static)",this);
		if (class_name) f->f_static = 1;	// if member: static by default
		if (f->nargs_known != 1)
                       error("ATs must be fully specified for%n",this);
		else if (f->nargs<1)
			error("%n requires a firstA ofT size_t",this);
		else if (f->argtype->tp->check(size_t_type,0)) {
				if (strict_opt==0
				&& f->argtype->tp->check(long_type,0)==0) {
					error('w',"%n firstA should be size_t",this);
					f->argtype->tp = size_t_type;
					if (f->f_signature) f->sign();
				}
				else
					error("%n requires a firstA ofT size_t",this);
		}
		else {
			Ptype t = f->s_returns  ? f->s_returns : f->returns;
			if (t->check(Pvoid_type,0)) error("bad returnT for %n",this);
		}
		break;

		case DELETE:	// void operator delete(void*) or
				// void operator delete(void*, long)
			if (f->f_virtual)
				error("virtual%n (operator delete() is static)",this);
			if (class_name) f->f_static = 1;	// if member: static by default
			if (f->nargs_known != 1)
                       		error("ATs must be fully specified for%n",this);
			else {
				Ptype t = f->s_returns ? f->s_returns : f->returns;
				if (t->base != VOID)
					error("bad returnT for %n", this);
				else {
					switch (f->nargs) {
					default:
						error("%n takes 1 or 2As",this);
						break;
					case 1:
					case 2:
					{	Pname a = f->argtype;
						if (a->tp->check(Pvoid_type,0))
                               				error("%n's 1stA must be a void*",this);
						else if (a = a->n_list) {
							if (class_name == 0)
								error("%n takes only oneA",this);
							else if (a->tp->check(size_t_type,0)) {
								if (strict_opt==0
								&& a->tp->check(long_type,0)==0) {
									error('w',"%n's 2ndA should be a size_t",this);
									a->tp = size_t_type;
									if (f->f_signature) f->sign();
								}
								else
									error("%n's 2ndA must be a size_t",this);
							}
						}
					}
					}
				}
               		}
               break;

	case ASSIGN:
		if (class_name && f->nargs==1) {
			Ptype t = f->argtype->tp;
			Pname an = t->is_cl_obj();  // X::operator=(X) ?
			if (an==0 && (t=t->is_ref())) { // X::operator=(X&) ?
				t = Pptr(t)->typ;
			rx1:
				switch (t->base) {
				case TYPE:	t = Pbase(t)->b_name->tp; goto rx1;
				case COBJ:	an = Pbase(t)->b_name;
				}
			}
			if (an && an==class_name) Pclass(an->tp)->c_xref |= C_ASS;
		}
		else if (f->nargs == 2) {
			Ptype t = f->argtype->tp;
			Pname an1;
			if (t=t->is_ref()) { // operator=(X&,?) ?
				t = Pptr(t)->typ;
			rx2:
				switch (t->base) {
				case TYPE:	t = Pbase(t)->b_name->tp; goto rx2;
				case COBJ:	an1 = Pbase(t)->b_name;
				}
			}
			t = f->argtype->n_list->tp;
			Pname an2 = t->is_cl_obj(); // operator=(X&,X) ?
			if (an2==0 && (t=t->is_ref())) { // operator=(X&,X&) ?
				t = Pptr(t)->typ;
			rx3:
				switch (t->base) {
				case TYPE:	t = Pbase(t)->b_name->tp; goto rx3;
				case COBJ:	an2 = Pbase(t)->b_name;
				}
			}
			if (an1 && an1==an2) Pclass(an1->tp)->c_xref |= C_ASS;
		}
	}

	switch (scope) {
	case FCT:
	case ARG:
		if (n_sto == STATIC) error("D of staticF in aF");
	}

	Pname nn = etbl->insert(this,0);
	nn->assign();
	n_table = etbl;
//error('d',"nn %n %t %d %d",nn,nn->tp,nn->tp,f);

	if (n_initializer) {
		if (f->f_virtual == 0) error("Ir for non-virtualF%n",this);
		if (n_initializer != zero) error("virtualFIr must be 0");
	}
			
	if (Nold) {
		Pfct nf = Pfct(nn->tp);
//error('d',"old %n: %t and %t",nn,nf,tp);
		if (nf->base==ANY || f->base==ANY)
				;
		else if (nf->base == OVERLOAD) {
			string = nn->string;
			nn = Pgen(nf)->add(this);

			if (Nold == 0) {
				if (f->body && n_qualifier) {
					error("badAL for %n::%s()",n_qualifier,string);
					return 0;
				}
				goto thth;
			}
		//	else {
		//		if (f->body==0 && friend_in_class==0) error('w',"%n redeclared",nn);
		//	}
				
			nf = Pfct(nn->tp);

			if (f->body && nf->body) {
				error("two definitions of%n",nn);
				return 0;
			}

			if (f->body) goto bdbd;
				
			goto stst;
		}
		else if (nf->base != FCT) {
			error("%n declared both as%t and asF",this,nf);
			f->body = 0;
		}
		else  {
//error('d',"%t->check(%t) -> %d %d",nf,f,nf->check(f,OVERLOAD)/*,vrp_equiv*/);
			if (nf->check(f,OVERLOAD)/* || vrp_equiv */|| const_problem) {
				if (f->body && n_qualifier) {
					error("%nT mismatch: %t and %t",nn,nf,f);
					return 0;
				}
				Pgen g = new gen;
add_first = 1;
				Pname n1 = g->add(nn);
add_first = 0;
				string = nn->string;
				Pname n2 = g->add(this);
				nn->tp = g;
				nn = n2;
				goto thth;
			}
				
			if (in_class_dcl) {
			//	error("twoDs of%n",this);
			//	f->body = 0;
			//	return 0;
			}
			else if (nf->f_static && f->f_inline==0 && n_sto==STATIC) {
				//error('d',"MF%n declared static outsideF",this);
				n_sto = 0;
			}
			else if (n_sto && n_sto!=nn->n_scope) {
				if (n_sto==EXTERN && nn->n_scope==STATIC)
					error('w',"%n declared extern after being declared static",this);
				else if (nf->f_inline==0 && f->f_inline==0) {
					extern Pname new_fct;	// beware of pre-defined functions
					extern Pname del_fct;
					if (nn->tp==new_fct->tp || nn->tp==del_fct->tp)
						nn->n_sto = n_sto;
					else
						error("%n declared as both%k and%k",this,n_sto,(nn->n_sto)?nn->n_sto:EXTERN);
				}
			}

//error('d',"fct %n: %k %k scope %k",this,n_sto,nn->n_sto,nn->n_scope);	

			extern linkage;
//error('d',"link %d lcount %d sig %s",linkage,lcount,nf->f_signature);
			if (linkage && nf->f_signature && nf->f_signature[0])
				error("inconsistent linkage specifications for%n",this);
			if (nf->body && f->body) {
				error("two definitions of%n",this);
				f->body = 0;
				return 0;
			}
				
			if (f->body) goto bdbd;

			goto stst;
	
		bdbd: 
			if (f->nargs_known && nf->nargs_known) merge_init(nn,f,nf);
			f->f_virtual = nf->f_virtual;
			f->f_this = nf->f_this;
			f->f_result = nf->f_result;
			f->s_returns = nf->s_returns;
			f->f_args = nf->f_args;
		//	f->argtype = nf->argtype;
			f->f_signature = nf->f_signature;
			f->f_const = nf->f_const;
			f->f_static = nf->f_static;
			nn->tp = f;
			if (f->f_inline) {
				if (nf->f_inline==0) {
                                          if (nn->n_used && nn->n_sto!=STATIC)
						error("%n declared with external linkage and called before defined as inline",nn);
				//	else if (nf->memof)
				//		error('w',"%n declared as non-inline but defined as inline",nn);
					else if (nn->n_used) {
						nn->take_addr(); // force printout
						if (warning_opt) error('w',"%n called before defined as inline",nn);
				}
				}
				nf->f_inline = 1;
				nn->n_sto = STATIC;
			}
			else if (nf->f_inline) {
		//		error('w',"%n defined as inline but not declared as inline",this);
				f->f_inline = 1;
			}
			goto stst2;

		stst:
//error('d',"stst");
			if (f->nargs_known && nf->nargs_known) merge_init(nn,f,nf);
			f->f_args = nf->f_args;
		//	f->argtype = nf->argtype;
		stst2:
//error('d',"stst2 %n printed %d",nn,nn->n_dcl_printed);
			if (f->f_inline) n_sto = STATIC;
			if (n_sto
			&& nn->n_scope!=n_sto
			&& friend_in_class==0
			&& f->f_inline==0){ // allow re-def to "static"
				if (n_sto == STATIC)
					nn->n_sto = STATIC;
				else {
					error("%n both%k and%k",this,n_sto,nn->n_scope);
				}
			}
			n_scope = nn->n_scope; // first specifier wins
			n_sto = nn->n_sto;
		}
	}
	else {	// new function: make f_this for member functions
	thth:
		just_made = 1;
		if (f->f_inline)
			nn->n_sto = STATIC;
		else if (class_name==0 && n_sto==0 && f->body==0)
			nn->n_sto = EXTERN;
//error('d',"thth %n %t static %d sto %k",nn,f,f->f_static,nn->n_sto);
		if (f->f_static)
			switch (n_oper) {	// what about + ??
			case CTOR:
			case DTOR:
			case TYPE:
			case CALL:
			case DEREF:
			case REF:
				error("%n cannot be a staticMF",nn);
				f->f_static = 0;
			}

		if (class_name
		&& f->f_static==0	// no ``this'' in static members
		&& n_oper!=NEW	 	// X::operator new() static by default
		&& n_oper!=DELETE	// X::operator delete() static by default
		&& etbl!=gtbl) {	/* beware of implicit declatation */
			Pname cn = nn->n_table->t_name;
			Pname tt = new name("this");
			tt->n_scope = ARG;
			tt->where = no_where;
		//	tt->n_sto = ARG;
			tt->tp = Pclass(class_name->tp)->this_type;
			PERM(tt);
			Pfct(nn->tp)->f_this = f->f_this = Pfct(nn->tp)->f_args = f->f_args = tt;
			tt->n_list = f->argtype;
//error('d',"nn %n tp %t const %d",nn,nn->tp,f->f_const);
			if (f->f_const /*&& n_oper!=CTOR && n_oper!=DTOR*/) {
				Pbase x = Pbase(Pptr(tt->tp)->typ);
				Pbase y = new basetype(COBJ,0);
				*y = *x;
				y->b_const = 1;
				tt->tp = new ptr(PTR,y);
				PERM(tt->tp);
			}

		}
		else {
			Pfct(nn->tp)->f_args = f->f_args = f->f_result?f->f_result:f->argtype;
			Pfct(nn->tp)->f_signature = f->f_signature;
			Pfct(nn->tp)->f_const = f->f_const;
			Pfct(nn->tp)->f_static = f->f_static;
		}

extern linkage;
		// if C++ linkage encode type in function name
		if (linkage==0 && Pfct(nn->tp)->f_signature==0) Pfct(nn->tp)->sign();

		if (f->f_result == 0) {
//error('d',"re1 %n %t %d",this,f,f);
			make_res(f);
		}
		else if (f->f_this)
			f->f_this->n_list = f->f_result;

		if (nn->n_oper==CTOR || nn->n_oper==DTOR) vbase_pointers(nn,Pclass(class_name->tp));

		if (f->f_virtual) {
			switch (nn->n_scope) {
			default:
				error("nonC virtual%n",this);
				break;
			case 0:
			case PUBLIC:
				//      if (fvirt)      //BSopt
				cc->cot->virt_count = 1;
				Pfct(nn->tp)->f_virtual = f->f_virtual;
				break;
			}
		}
	}

		/*	an operator must take at least one class object or
			reference to class object argument
		*/

	if (just_made)
	switch (n_oper) {
	case CTOR:
		switch (f->nargs) {	// check for X(X) and X(X&)
		case 0:
			break;
		default:		// handle X(X&, int i = 0)
		{	Pname n2 = f->argtype->n_list;
			if (n2->n_initializer==0 && n2->n_evaluated==0) break;
		}
		case 1:
		{
			Ptype t = f->argtype->tp;
		clll:
			switch (t->base) {
			case TYPE:
				t = Pbase(t)->b_name->tp;
				goto clll;
			case RPTR:			/* X(X&) ? */
				t = Pptr(t)->typ;
			cxll:
				switch (t->base) {
				case TYPE:
					t = Pbase(t)->b_name->tp;
					goto cxll;
				case COBJ:
					if (class_name == Pbase(t)->b_name)
						Pclass(class_name->tp)->c_itor = nn;
				}
				break;
			case COBJ:			/* X(X) ? */
				if (class_name == Pbase(t)->b_name) {
					error("badK %s(%s) use %s(%s&)",class_name->string,class_name->string,class_name->string,class_name->string);
					f->argtype->tp = any_type;
				}
			}
		}
		}
		if (Pclass(class_name->tp)->c_ctor == 0) Pclass(class_name->tp)->c_ctor = nn;
		break;

	case TYPE:
		// somewhat simple minded solution to the inheritance of
		// conversion operator problem
		nn->n_list = Pclass(class_name->tp)->conv;
		Pclass(class_name->tp)->conv = nn;
		break;

	case DTOR:
		Pclass(class_name->tp)->c_dtor = nn;
		break;

	case NEW:	// void* operator new(long)
/*
		if (Pfct(nn->tp)->f_virtual)
			error("virtual%n (operator new() is static)",this);
		if (class_name) Pfct(nn->tp)->f_static = f->f_static = 1;	// if member: static by default
		if (f->nargs_known != 1)
                       error("ATs must be fully specified for%n",nn);
		else if (f->nargs<1)
			error("%n requires a firstA ofT size_t", nn);
		else if (f->argtype->tp->check(size_t_type,0)) {
				if (strict_opt==0
				&& f->argtype->tp->check(long_type,0)==0) {
					error('w',"%n firstA should be size_t",nn);
					f->argtype->tp = size_t_type;
					if (f->f_signature) Pfct(nn->tp)->sign();
				}
				else
					error("%n requires a firstA ofT size_t",nn);
		}
		else {
			Ptype t = f->s_returns  ? f->s_returns : f->returns;
			if (t->check(Pvoid_type,0)) error("bad returnT for %n", nn);
		}
		break;
*/
	case DELETE:	// void operator delete(void*) or
			// void operator delete(void*, long)
/*
//error('d',"delete %n %t",nn,nn->tp);
		if (Pfct(nn->tp)->f_virtual)
			error("virtual%n (operator delete() is static)",this);
		if (class_name) Pfct(nn->tp)->f_static = f->f_static = 1;	// if member: static by default
		if (f->nargs_known != 1)
                       error("ATs must be fully specified for%n",nn);
		else {
                       Ptype t = f->s_returns ? f->s_returns : f->returns;
                       if (t->base != VOID)
                               error("bad returnT for %n", nn);
			else {
				switch (f->nargs) {
				default:
					error("%n takes 1 or 2As",nn);
					break;
				case 1:
				case 2:
				{	Pname a = f->argtype;
					if (a->tp->check(Pvoid_type,0))
                               			error("%n's 1stA must be a void*",nn);
					else if (a = a->n_list) {
						if (class_name == 0)
							error("%n takes only oneA",nn);
						else if (a->tp->check(size_t_type,0)) {
							if (strict_opt==0
							&& a->tp->check(long_type,0)==0) {
								error('w',"%n's 2ndA should be a size_t",nn);
								a->tp = size_t_type;
								if (f->f_signature) Pfct(nn->tp)->sign();
							}
							else
								error("%n's 2ndA must be a size_t", nn);
						}
					}
				}
				}
			}
               }
               break;
*/
	case CALL:
	case 0:
		break;

	default:
		if (f->nargs_known != 1) {
			error("ATs must be fully specified for%n",nn);
		}
		else if (class_name == 0) {
			Pname a;
			switch (f->nargs) {
			case 1:
			case 2:
				for (a=f->argtype; a; a=a->n_list) {
					Ptype tx = a->tp;
					while (tx->base == TYPE) tx = Pbase(tx)->b_name->tp;
					if (tx->is_ref()) tx = Pptr(tx)->typ;
					if (tx->is_cl_obj()) goto cok;
				}
				error("%n must take at least oneCTA",nn);
				break;
			default:
				error("%n must take 1 or 2As",nn);
			}
		}
		else {
			switch (f->nargs) {
			case 0:
			case 1:
				break;
			default:
				error("%n must take 0 or 1As",nn);
			}
		}
	cok:;
	}

	int i = 0;	// check that every argument after an argument with
			// initializer have an initializer
	for (Pname a = f->f_args/*f->argtype*/; a; a=a->n_list) {
		if (a->n_initializer)
			i = 1;
		else if (i)
			error("trailingA%n withoutIr",a);
	}

	/*
		the body cannot be checked until the name
		has been checked and entered into its table
	*/
	if (f->body) f->dcl(nn);
	return nn;
}
