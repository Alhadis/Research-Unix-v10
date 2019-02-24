/*ident	"@(#)ctrans:src/dcl.c	1.7" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

dcl.c:

	``declare'' all names, that is insert them in the appropriate symbol tables.

	Calculate the size for all objects (incl. stack frames),
	and find store the offsets for all members (incl. auto variables).
	"size.h" holds the constants needed for calculating sizes.

	Note that (due to errors) functions may nest

*****************************************************************************/


#include "cfront.h"
#include "size.h"

class dcl_context ccvec[MAXCONT], * cc = ccvec;
int byte_offset;
int bit_offset;
int max_align;
int friend_in_class;
static Pstmt itail;

Pname dclass(Pname, Ptable);
Pname denum(Pname, Ptable);
void merge_init(Pname, Pfct, Pfct);

void dosimpl(Pexpr e, Pname n)
{
//error('d',"dosimpl %k %n",e?e->base:0,n);
	if (n==0) {
		if (dummy_fct == 0) make_dummy();
		n = dummy_fct;
	}
	Pname cf = curr_fct;
	curr_fct = n;
	e->simpl();
	curr_fct = cf;
}

static Pexpr co_array_init(Pname n, Ptable tbl)
/*
	handle simple arrays only. To be done well list_check() must
	be rewritten to handle dynamic initialization.
*/
{
// error('d',"Ir forCO%n\[\]",n);
	Pexpr init = n->n_initializer;

	if (init->base != ILIST) {
		error("badIr for array ofCOs %n",n);
		return 0;
	}

	Pexpr el = 0;
	Pvec v = Pvec(n->tp->skiptypedefs());
	Pname cn = v->typ->is_cl_obj();
	Pclass cl = cn ? Pclass(cn->tp) : 0;
//error('d',"v %t cl %d %t",v,cl,cl);
	int i = v->size;

	int count = 0;
	Pexpr il2;
	for (Pexpr il = init->e1; il; il = il2) {
			// generate	n[0].cl(initializer),
			//		...
			//		n[max].cl(initializer),
		Pexpr e = il->e1;
		il2 = il->e2;
		il->e2 = 0;
		if  (e == dummy) break;
//error('d',"il %k e %k",il->base,e?e->base:0);
		if (e->base == VALUE) {
			switch (e->tp2->base) {
			case CLASS:
				if (Pclass(e->tp2)!=cl) e = new texpr(VALUE,cl,il);
				break;
			default:
				Pname n2 = e->tp2->is_cl_obj();
				if (n2==0 || Pclass(n2->tp)!=cl) e = new texpr(VALUE,cl,il);
			}
		}
		else
			e = new texpr(VALUE,cl,il);
		e->e2 = new expr(DEREF,n,new ival(count++));
		e = e->typ(tbl);
		dosimpl(e,cc->nof);
	//	e->simpl();
		if (sti_tbl == tbl) {
			Pstmt ist = new estmt(SM,no_where,e,0);
			if (st_ilist == 0)
				st_ilist = ist;
			else
				itail->s_list = ist;
			itail = ist;
		}
		else {
			el = el ? new expr(G_CM,el,e) : e;
		}
	}

	if (i==0)
		v->size = count;
	else if (i==count)
		;
	else if (i<count) {
                error("too manyIrs for%n (%d)",n, count);
		return 0;
	}	
	else {
//              error('d',"too fewIrs for%n (%d)",n, count);
		if (cl->has_ictor())
                	error('s',"too fewIrs for%n",n);
		else error( "too fewIrs for%n (C %srequires a defaultK)", n, cl->string );
		return 0;
	}
	return el;
}

int need_sti(Pexpr e, Ptable tbl, bit accept_name)
/*
	check if non-static variables or operations are used
	INCOMPLETE 
*/
{
	if (e == 0) return 0;
// error('d',"need_sti: %d - %d %k %t",tbl?1:0,accept_name,e->base,e->tp);

	switch (e->base) {
	case QUEST:
		if (need_sti(e->cond,tbl,0) && tbl==0) return 1;
	case PLUS:
	case MINUS:
	case MUL:
	case DIV:
	case MOD:
	case ER:
	case OR:
	case ANDAND:
	case OROR:
	case LS:
	case RS:
	case EQ:
	case NE:
	case LT:
	case LE:
	case GT:
	case GE:
	case INCR:
	case DECR:
	case ASSIGN:
		if (need_sti(e->e1,tbl,accept_name) && tbl==0) return 1;
		// no break;

	case UMINUS:
	case UPLUS:
	case NOT:
	case COMPL:
		if (need_sti(e->e2,tbl,accept_name) && tbl==0) return 1;
		// no break

	case SIZEOF:
	default:

		return 0;

	case CAST:

		return need_sti(e->e1,tbl,accept_name);

	case ADDROF:
	{
		int i = need_sti(e->e2,tbl,1);
		return i;
	}

	case NAME:
		if (accept_name && Pname(e)->n_stclass==STATIC) return 0;

//		if ((Pname(e)->n_stclass==AUTO || Pname(e)->n_stclass==REGISTER)
//		&&   strncmp(Pname(e)->string,"__Q",3) != 0 ) {
//			error("localN%n inIr for static",e);
//			return 0;
//		}

		if (e->tp->tconst()) {
			if (vec_const || fct_const) return 0;
			Neval = 0;
			e->eval();
			if (Neval == 0) return 0;
		}
		return 1;

	case DEREF:
	case REF:
	case DOT:
		if (accept_name||(e->tp && e->tp->base==VEC)) {
			int x1 = need_sti(e->e1,tbl,e->base == DOT);
			int x2 = need_sti(e->e2,tbl,0);
			return x1 || x2;
		}
		// no break

	case ELIST:
//	case ILIST:
	case G_CM:
	case CM:
		if (e->base==CM) {
			if (need_sti(e->e1,tbl,0) || need_sti(e->e2,tbl,0))
				return 1;
			else
				return 0;
		}
	case CALL:
	case G_CALL:
	case NEW:
	case GNEW:
	case 0:			// hack for `new type (expr)'
		if (tbl) {
			need_sti(e->e1,tbl,accept_name);
			need_sti(e->e2,tbl,accept_name);
			if ( e->tp && e->tp->base == VEC &&
				e->base == NEW || e->base == GNEW )
				// preserve ICON,STRING,CCON,FCON
				need_sti(Pvec(e->tp)->dim,tbl);
		}
                else if (e->base == 0)
                      return 0;
		// no break
	case ICALL:
		return 1;
	case ICON:
	case STRING:
	case CCON:
	case FCON:
//error('d',"save %k tbl %d",e->base,tbl);
		if (tbl) {
			char* p = new char[strlen(e->string)+1];
			strcpy(p,e->string);
			e->string = p;
		}
		return 0;
	}
}

static void check_def_name( Pname nn, int scope ) 
{
// error('d', "check_def_name: nn: %n n_sto: %k scope: %k", nn,nn->n_sto,scope );

     	Pfct f = Pfct(nn->tp);
	if ( def_name==0 && pdef_name==0 && friend_in_class == 0 &&
		scope == EXTERN && nn->n_scope != STATIC &&  
		nn->n_oper != NEW && nn->n_oper != DELETE ) { 
     			if (f->body && 
				f->f_inline == 0 && 
				f->f_imeasure == 0) {
// error('d', "check_def_name: do it: %n", nn );
					pdef_name = def_name = nn;
					ptbl_init(0);
					def_name = 0;
			}
    	}
	if (strcmp(nn->string,"main")==0 && (nn->n_table==gtbl) && f->f_inline) {

					pdef_name = def_name = nn;
					ptbl_init(0);
					def_name = 0;
	}
}

int stat_init = 0; // in an expression initializing a local static
Pname name::dcl(Ptable tbl, TOK scope)
/*
	enter a copy of this name into symbol table "tbl";
		- create local symbol tables as needed
	
	"scope" gives the scope in which the declaration was found
		- EXTERN, FCT, ARG, PUBLIC, or 0
	Compare "scope" with the specified storage class "n_sto"
		- AUTO, STATIC, REGISTER, EXTERN, OVERLOAD, FRIEND, or 0

	After name::dcl()
	n_stclass ==	0		class or enum member
			REGISTER	auto variables declared register
			AUTO		auto variables not registers
			STATIC		statically allocated object
	n_scope ==	0		private class member
			PUBLIC		public class member
			EXTERN		name valid in this and other files
			STATIC		name valid for this file only
			FCT		name local to a function
			ARG		name of a function argument
			ARGT		name of a type defined in an
					argument list
                        ARGS            temporary class object with dtor in
                                        initialization of a local static


	typecheck function bodies;
	typecheck initializers;

	note that functions (error recovery) and classes (legal) nest

	The return value is used to chain symbol table entries, but cannot
	be used for printout because it denotes the sum of all type information
	for the name

	names of typenames are marked with n_oper==TNAME

	WARNING: The handling of scope and storage class is cursed!
*/
{
	Pname nn;
	Ptype nnt = 0;
	Pname odcl = Cdcl;
	int sti_vb = 0;	// set if initialize with virtual base class

//	if (this == 0) error('i',"0->N::dcl()");
//	if (tbl == 0) error('i',"%n->N::dcl(tbl=0,%k)",this,scope);
//	if (tbl->base != TABLE) error('i',"%n->N::dcl(tbl=%d,%k)",this,tbl->base,scope);
//	if (tp == 0) error('i',"N::dcl(%n,%k)T missing",this,scope);

	Cdcl = this;
	Ptype tx = tp->skiptypedefs();

	DB( if(Ddebug>=1) error('d',"dcl %n %t",this,tx); );
// error('d',"dcl %n %t cc->cot: %d",this,tx,cc?cc->cot:0); 

	switch (base) {
	case TNAME:
		nn = tbl->look(string,0);
		tp->dcl(tbl);
                if (nn && nn->lex_level == lex_level) {
//error('d', "dcl: cot: %s ccl: %s", cc->cot?cc->cot->string:"notset", ccl?ccl->string:"notset");

                        if ( tp->check(nn->tp,0) )
                                error("%n declared as %t and %t",nn,nn->tp,tp);
                        else if ( nn->base == NAME && cc->cot == 0 &&
			(tp->base != COBJ || tp->base != EOBJ))
                                error("%n declared as identifier and typedef", nn );
                        Cdcl = odcl;
                        return 0;

                }
		PERM(tp);
		nn = new name(string);
		nn->base = TNAME;
		nn->tp = tp;
		Pname tn = tbl->insert(nn,0);
		if (n_key == NESTED) {
			tn->n_key = NESTED;
			tn->tpdef = tpdef;
		}
		delete nn;
		Cdcl = odcl;
		return this;
	
	case NAME:
		switch (n_oper) {
		case COMPL:
			if (tp->base != FCT) {
				error("~%s notF",string);
				n_oper = 0;
			}
			break;
		case TNAME:
			if (tp->base != FCT) n_oper = 0;
			break;
		}
		break;
	default:
		error('i',"NX in N::dcl()");
	}

	if (n_qualifier) {	
				// c::f()
				// class function,
				// friend declaration, or 
				// static member initializer
		Pname cn = n_qualifier;

		switch (cn->base) {
		case TNAME:
			break;
		case NAME:
			cn = gtbl->look(cn->string,0);
			if (cn && cn->base==TNAME) break;
		default:
			error("badQr%n for%n",n_qualifier,this);
			Cdcl = odcl;
			return 0;
		}

		cn->tp = cn->tp->skiptypedefs();
// error('d',"actualQr%n %t",cn,cn->tp);
		if (cn->tp->base != COBJ) {
			error(&where,"Qr%nnot aCN",n_qualifier);
			Cdcl = odcl;
			return 0;
		}

		cn = Pbase(cn->tp)->b_name;
		if (n_oper) check_oper(cn);

		Pclass cl = Pclass(cn->tp);
		if (cl == cc->cot) {
			n_qualifier = 0;
			goto xdr;
		}
		else if ((cl->defined&(DEFINED|SIMPLIFIED)) == 0) {
			error("C%nU",cn);
			Cdcl = odcl;
			return 0;
		}
		else if (cl->c_body==1)	//III
			cl->dcl_print(0);

		Ptable etbl = cl->memtbl;
		Pname x = etbl->look(string,0);
		if (x==0 || x->n_table!=etbl) {
			error("%n is not aM of%n",this,cn);
			Cdcl = odcl;
			return 0;
		}

		if (tp->base == FCT) {	//III
		//	if (friend_in_class==0 && Pfct(tp)->body==0) {	// c::f(); needed for friend
			if (friend_in_class==0
			&& n_sto!=FRIEND
			&& Pfct(tp)->body==0) {	// c::f(); needed for friend
				error("QdN%n inFD",x);
				Cdcl = odcl;
				return 0;
			}

			if (Pfct(tp)->body==0) {
				Pfct(tp)->memof = cl;
				int xx;
				if (x->tp->base==OVERLOAD)
					xx = Pgen(x->tp)->find(Pfct(tp),0)==0;
				else
					xx = x->tp->check(tp,0);

				if (xx) {
					error("%n is not aM of%n",this,cn);
					Cdcl = odcl;
					return 0;
				}
			}
		}
		else {
			if (x->n_stclass != STATIC) {	// e.g. int c::i = 7
				error("D of non staticCM %n",this);
				Cdcl = odcl;
				return 0;
			}
			if (n_sto) error("staticCM declared%k",n_sto);
			tbl = etbl;
		}
	}
xdr:
	if (n_oper
	&& tp->base!=FCT
	&& n_sto!=OVERLOAD) error("operator%k not aF",n_oper);


	/*	if a storage class was specified
			check that it is legal in the scope 
		else
			provide default storage class
		some details must be left until the type of the object is known
	*/

	n_stclass = n_sto;
	n_scope = scope;	/* default scope & storage class */

	switch (n_sto) {
	default:
		error('i',"unX %k",n_sto);
	case FRIEND:
	{
		Pclass cl = cc->cot;

		switch (scope) {
		case 0:
		case PUBLIC:
			break;
		default:
			error("friend%n not inCD(%k)",this,scope);
			base = 0;
			Cdcl = odcl;
			return 0;
		}

		switch (n_oper) {
		case 0:
		case NEW:
		case DELETE:
		case CTOR:
		case DTOR:
		case TYPE:
			n_sto = 0;
			break;
		default:
			n_sto = OVERLOAD;
		}

		switch (tx->base) {
		case COBJ:
			nn = Pbase(tx)->b_name;
			break;
		case CLASS:
			nn = this;
			break;
		case FCT:
			cc->stack();
			cc->not = 0;
			cc->tot = 0;
			cc->cot = 0;
			friend_in_class++;
		//	n_sto = EXTERN;
			n_sto = 0;
			nn = dcl(gtbl,EXTERN);
			if (nn == 0) {
				Cdcl = odcl;
				return 0;
			}
			friend_in_class--;
			cc->unstack();
			if (nn->tp->base == OVERLOAD) nn = Pgen(nn->tp)->find(Pfct(tx),1);
			break;
		default:
			error("badT%tof friend%n",tp,this);
			Cdcl = odcl;
			return 0;
		}
		PERM(nn);
		cl->friend_list = new name_list(nn,cl->friend_list);
		Cdcl = odcl;
		return nn;
	}
	case OVERLOAD:
		if (strict_opt||warning_opt)
			error(strict_opt?0:'w',"`overload' used (anachronism)");
		n_sto = 0;
		// ignore overload!
		switch (tp->base) {
		case FCT:
			break;
		default:
			base = 0;
			Cdcl = odcl;
			return this;
		}
		break;
	case REGISTER:
		if (tp->base == FCT) {
			error('w',"%n: register (ignored)",this);
			goto ddd;
		}
	case AUTO:
		switch (scope) {
		case 0:
		case PUBLIC:
		case EXTERN:
			error("%k not inF",n_sto);
			goto ddd;
		}
		if (n_sto==AUTO) n_sto = 0;	// always redundant
		break;
	case EXTERN:
		switch (scope) {
		case ARG:
			//error("externA");
			goto ddd;
		case 0:
		case PUBLIC:
			/* extern is provided as a default for functions without body */
			if (tp->base != FCT) error("externM%n",this);
			goto ddd;
		case FCT:
			{
			Pname nn = gtbl->look( string, 0 );
			tp->dcl(tbl);
			if ( nn && tp->base != FCT &&
				tp->check(nn->tp,0)) 
				{
				error("twoDs of%n;Ts:%t and%t",this,nn->tp,tp);
				Cdcl = odcl;
				return 0;
				}
			}
		}
		n_stclass = STATIC;
		n_scope = EXTERN;	/* avoid FCT scoped externs to allow better checking */
		break;
	case STATIC:
		switch (scope) {
		case ARG:
			//error("static used forA%n",this);
			goto ddd;
		case 0:
		case PUBLIC:
			n_stclass = STATIC;
			n_scope = scope;
			break;
		default:
			n_scope = STATIC;
		}
		break;
	case 0:
	ddd:
		switch (scope) {	/* default storage classes */
		case EXTERN:
			n_scope = EXTERN;
			n_stclass = STATIC;
			break;
		case FCT:
			if (tp->base == FCT) {
				n_stclass = STATIC;
				n_scope = EXTERN;
			}
			else
				n_stclass = AUTO;
			break;
		case ARG:
			n_stclass = AUTO;
			break;
		case 0:
		case PUBLIC:
			n_stclass = 0;
			break;
		}
	}

	
	/*
		now insert the name into the appropriate symbol table,
		and compare types with previous declarations of that name

		do type dependent adjustments of the scope
	*/
//error('d',"sw %d",tx->base);
	static int warn_ldouble=0;

	switch (tx->base) {
	case ASM:
	{	Pbase b = Pbase(tp);
		Pname n = tbl->insert(this,0);
		n->assign();
		n->use();
		char* s = (char*) b->b_name;	// save asm string. Shoddy
		int ll = strlen(s);
		char* s2 = new char[ll+1];
		strcpy(s2,s);
		b->b_name = Pname(s2);
		Cdcl = odcl;
		return this;
	}

	case CLASS:
		tp = tx;
		nn = dclass(this,tbl);
		Cdcl = odcl;
		return nn;

	case ENUM:
		tp = tx;
		nn = denum(this,tbl);
		Cdcl = odcl;
		return nn;
	
	case FCT:
		tp = tx;
		nn = dofct(tbl,scope);
		if (nn == 0) {
			Cdcl = odcl;
			return 0;
		}

// error('d', "%s n_oper: %k, scope: %k", string, n_oper, scope );
		if (pdef_name == 0) check_def_name(nn, scope);
		break;

	case FIELD:
		switch (n_stclass) {
		case 0:
		case PUBLIC:
			break;
		default:
			error("%k field",n_stclass);
			n_stclass = 0;
		}

		if (cc->not==0 || cc->cot->csu==UNION || cc->cot->csu==ANON) {
			error(cc->not?"field in union":"field not inC");
			PERM(tp);
			Cdcl = odcl;
			return this;
		}

		if (string) {
			nn = tbl->insert(this,0);
			n_table = nn->n_table;
			if (Nold) error("twoDs of field%n",this);
		}

		tp->dcl(tbl);
		field_align();
		break;
	
	case COBJ:
	{	Pclass cl = Pclass(Pbase(tx)->b_name->tp);
//error('d',&where,"dcl %n; tx->base == cobj",this);
		if (cl->lex_level > lex_level) 
     			error("C%t is not visible in this scope",cl);

		if (cl->csu == ANON) {	// export member names to enclosing scope
                        if (tbl==gtbl && n_sto!=STATIC) error("extern anonymous union (declare as static)");
			char* p = cl->string;
			while (*p++ != 'C');	/* sneaky */
			int uindex = (int)str_to_long(p);

// error('d', "%n->dcl() n_scope: %k n_protect: %k", this, n_scope, n_protect );
		// cannot cope with use counts for ANONs:
			Pbase(tp)->b_name->n_used = 1;
			Pbase(tp)->b_name->n_assigned_to = 1;

			Ptable mtbl = cl->memtbl;
                        int i, err_msg = 0;
			for (Pname nn=mtbl->get_mem(i=1); nn; nn=mtbl->get_mem(++i)) {
				if (nn->tp->base == FCT) {
					error("FM%n for anonymous union",nn);
					break;
				}
				Ptable tb = nn->n_table;
				nn->n_table = 0;
				nn->n_scope = n_protect?n_protect:n_scope; 
				Pname n = tbl->insert(nn,0);
				if (Nold) {
					error("twoDs of%n (one in anonymous union)",nn);
					break;
				}

				Pclass tc;
				if (tc = cl->in_class) 
					--n->lex_level;
				if ( tc && tc->csu == ANON) {
//error('d', "tc->csu anon %k %s", tc->csu, string );
					if ( n->n_anon ) {
						if ( !err_msg ) 
							error('s', "anonymous unions nested deeper than 2 levels" );
						err_msg = 1;
					}
					n->n_anon = string;
				}

				n->n_union = uindex;
				nn->n_table = tb;
			}
		}
		if (cl->c_abstract) error("D ofO of abstractC%t",cl);
		goto cde;
	}

	case VOID:
		if (n_scope != ARG) {
			error("badBT:%k%n",tx->base,this);
			Cdcl = odcl;
			return 0;
		}
		break;

	case LDOUBLE:
                if (warn_ldouble==0 && ansi_opt==0) {
                        ++warn_ldouble;
                        error('w',"long double supported under ``+a1'' option only, generating ``double%n''", this);
                }
		goto cde;

	case PTR:
	case VEC:
	case RPTR:
		tp->dcl(tbl);

	default:
	cde:
//error('d',"cde: %n %t",this,tp);
		nn = tbl->insert(this,0);
		n_table = nn->n_table;

		if (Nold) {

			if ( nn->base == PUBLIC ) { // X::i
     				error("twoDs ofCM%n", nn);
     				Cdcl = odcl;
     				return 0;
			}

			if (nn->tp->base == ANY) goto zzz;

			if (tp->check(nn->tp,0)) {
				error("twoDs of%n;Ts:%t and%t",this,nn->tp,tp);
				Cdcl = odcl;
				return 0;
			}
//error('d',"%n: %k %k scope %k",this,n_sto,nn->n_sto,nn->n_scope);
			if (n_sto && n_sto!=nn->n_scope) {
				if (n_sto==EXTERN && nn->n_scope==STATIC) {
					error('w',"%n declared extern after being declared static",this);
					goto ext_fudge;
				}
				else
					error("%n declared as both%k and%k",this,n_sto,(nn->n_sto)?nn->n_sto:(scope==FCT?AUTO:EXTERN));
			}
			else if (nn->n_scope==STATIC && n_scope==EXTERN) {
				error('w',"static%n followed by definition",this);
			ext_fudge:
				if (n_initializer) {
//					error('d',"static%n redefined (WIr)",this);
					n_initializer = 0;
				}
				n_sto = EXTERN;
			}
			else if (nn->n_sto==STATIC && n_sto==STATIC ) 
				error("static%n declared twice",this);
			else {
				if (n_sto==0
				&& nn->n_sto==EXTERN
				&& n_initializer
				&& tp->tconst())
					n_sto = EXTERN;
				n_scope = nn->n_scope;

				switch (scope) {
				case FCT:
					if (n_sto != EXTERN) {
						error("twoDs of%n",this);
						Cdcl = odcl;
						return 0;
					}
					break;
				case ARG:
					error("twoAs%n",this);
					Cdcl = odcl;
					return 0;
				case 0:
				case PUBLIC:
					error("twoDs ofM%n",this);
					Cdcl = odcl;
					return 0;
				case EXTERN:
					if (n_sto==0) {
						switch(nn->n_sto) {
						case 0:
							error("two definitions of%n",this);
							Cdcl = odcl;
							return 0;
						case EXTERN:
							if(nn->n_initializer) {
								error("two definitions of%n",this);
								Cdcl = odcl;
							}
							else
								nn->n_sto=0;
							break;
						}
					}
					else if(n_sto==EXTERN && n_initializer) {
						switch(nn->n_sto) {
						case 0:
							error("two definitions of%n",this);
							Cdcl = odcl;
							return 0;
						case EXTERN:
							nn->n_sto=0;
							break;
						}
					}
					break;
				}
			}
			n_scope = nn->n_scope;
/* n_val */
			if (n_initializer) {
				if (nn->n_initializer || nn->n_val) error("twoIrs for%n",this);
				nn->n_initializer = n_initializer;
			}
			if (tp->base == VEC) {
			//	handle:	 extern v[]; v[200];
			//	and	extern u[10]; u[11];

				Ptype ntp = nn->tp->skiptypedefs();
				if (Pvec(ntp)->dim == 0) Pvec(ntp)->dim = Pvec(tp)->dim;
				if (Pvec(ntp)->size) {
					if (Pvec(tp)->size
					&& Pvec(ntp)->size!=Pvec(tp)->size)
						error("bad array size for%n: %d %dX",this,Pvec(tp)->size,Pvec(ntp)->size);
				}
				else
					Pvec(ntp)->size = Pvec(tp)->size;
			}
		}
		else {
//error('d',"%n %t scope %d sto %k",this,tp,scope,n_sto);		
			if (scope!=ARG
			&& n_sto!=EXTERN
			&& (!((scope==0 || scope==PUBLIC) && n_sto==STATIC)) // static member
			&& n_initializer==0
			&& tp->base==VEC
			&& Pvec(tp)->size==0)
				error(&where,"dimension missing for array%n",this);

			if (scope==EXTERN
			&& n_sto==0
			&& tp->tconst()
			&& vec_const==0
			&& fct_const==0)
				nn->n_sto = n_sto = STATIC;
		}
	
	zzz:
		if (base != TNAME) {
			Ptype t = nn->tp;

			if (t->base == TYPE) {
				Ptype tt = Pbase(t)->b_name->tp;
				if (tt->base == FCT) nn->tp = t = tt;
			}

			switch (t->base) {
			case FCT:
			case OVERLOAD:
				break;
			default:
				fake_sizeof = 1;
				switch (nn->n_stclass) {
				default:
					if (nn->n_scope != ARG) {
						int x = t->align();
						int y = t->tsizeof();

						if (max_align < x) max_align = x;

						while (0 < bit_offset) {
							byte_offset++;
							bit_offset -= BI_IN_BYTE;
						}
						bit_offset = 0;

						if (byte_offset && 1<x) byte_offset = ((byte_offset-1)/x)*x+x;
						nn->n_offset = byte_offset;
						byte_offset += y;
					}
					break;
				case STATIC:
					if (n_sto != EXTERN
					&& nn->n_scope
					&& nn->n_scope!=PUBLIC)
					t->tsizeof();	// check that size is known
				}
				fake_sizeof = 0;
			}
		}

	{	Ptype t = nn->tp;
		int const_old = const_save;
		bit vec_seen = 0;
		Pexpr init = n_initializer;

	lll:
// error('d',"lll %n %t init %d %k",this,t,init,init?init->base:0);
		switch (t->base) {
		case COBJ:
		{	Pname cn = Pbase(t)->b_name;
			Pclass cl = (Pclass)cn->tp;
			Pname ctor = cl->has_ctor();
			Pname dtor = cl->has_dtor();
			int stct = 0;
//error('d',"obj %n; class '%s'; ctor '%s'; dtor '%s'",nn,cn->string,ctor?ctor->string:"???",dtor?dtor->string:"???");
			if (dtor) {
				Pstmt dls;

				// if dtor is not public check scope of class object

//error('d',"dcl %n has dtor",nn);
				if (dtor->n_scope != PUBLIC)  {
//error('d',"dcl %n->n_scope: %d fct %n",nn,nn->n_scope,cc->nof);
					switch (nn->n_scope) {
					case ARG:
					case 0:
					case PUBLIC:
						break;
					default: 
						check_visibility( dtor, 0, cl, tbl, cc->nof );
					}
			//		if (nn->n_scope == FCT) 
			//			check_visibility( dtor, 0, cl, tbl, cc->nof );
			//		else if ( nn->n_sto != EXTERN )
			//			error("%k%n cannot access%n: %sM",nn->n_scope,nn,dtor,dtor->n_protect?"protected":"private");
				}

				switch ( nn->n_scope ) {
				case 0:
				case PUBLIC:
					if (n_stclass==STATIC) { //III
						Pclass cl = Pclass(nn->n_table->t_name->tp);
						if (cl->defined&DEFINED) goto dtdt;
					}
					break;
				case EXTERN:
					if (n_sto==EXTERN) break;

				case STATIC:
				{	Pexpr c;
				dtdt:
// local static class objects have destructors set up in simpl2.c
// special case: temporary class object generated in init expression
					if (stat_init && scope == ARGS ) {
     						nn->n_scope = ARGS;
     						goto ggg;
 					}

					if ( nn->lex_level
					&&   nn->n_sto == STATIC ) {
						if (ctor==0) 
						    error('s',"local static%n has%n but noK(add%n::%n())", nn, dtor, cn, cn );
						goto static_init;
					}

					Ptable otbl = tbl;
					    // to collect temporaries generated
					    // in static destructors where we
					    // can find them again (in std_tbl)
					if (std_tbl == 0) std_tbl = new table(8,gtbl,0);
					tbl = std_tbl;
					if (vec_seen) {
						c = cdvec(vec_del_fct,nn,cl,dtor,0,zero);
					} else {	// nn->cl::~cl(0);
						c = call_dtor(nn,dtor,0,DOT,one);
					}
					c->tp = any_type; // avoid another check
					dls = new estmt(SM,nn->where,c,0);
					
					// destructors for statics are executed in reverse order
					if (st_dlist) dls->s_list = st_dlist;
					st_dlist = dls;
					tbl = otbl;
				} // case STATIC
				} // switch nn->n_scope
			} // if dtor

// local static class objects must defer setting up static dtor
static_init:
			if (ctor) {
// error('d',"ctor %n scope %k",ctor,nn->n_scope);
				Pexpr oo = nn;
				for (int vi=vec_seen; vi; vi--) oo = oo->contents();
				int sti = 0;
				switch (nn->n_scope) {
				case EXTERN:
					if (init==0 && n_sto==EXTERN) goto ggg;
				case STATIC:
					if (tbl == gtbl)
						sti = 1;
					else
						stct = 1;
				default:
					if (vec_seen && init) {
						if (1<vec_seen) {
/* ?????
							if (init->base != ILIST)
								error("badIr forO ofC %t withK%n",cl,this);
							else
*/
								error('s',"Ir for multi-dimensional array%n ofOsofC %t withK",this,cl);
							}
						else {
							if (sti) {
								if (sti_tbl==0) sti_tbl = new table(8,gtbl,0);
								const_save = 1;
								(void) co_array_init(nn,sti_tbl);
								const_save = 0;
								n_initializer = init = 0;
							}
							else {
								n_initializer = init = co_array_init(nn,tbl);
								if (stct)  
									nn->n_initializer = n_initializer = init = new expr( STAT_INIT, init, 0 );
							}
						}
						goto ggg;
					}
					break;
				case PUBLIC:
				case 0:
					if (n_stclass==STATIC) { //III
						Pclass cl = Pclass(nn->n_table->t_name->tp);
						if (cl->defined&DEFINED) {
							sti = 1;
							break;
						}
					}
					{
						Pname c;
						if (vec_seen) {
							c = cl->has_ictor();
							if (c == 0)
								error("array ofC%n that does not have aK taking noAs",cn);
							else if (Pfct(c->tp)->nargs)
								error("defaultAs forK for array ofC%n",cn);
						}
					}
					// no break
				case ARG:
					goto ggg;
				}

				const_save = 1;
				nn->assign();
				Ptable otbl = tbl;
				if (sti) {	// to collect temporaries generated
						// in static initializers where we
						// can find them again (in sti_tbl)
					if (sti_tbl == 0) sti_tbl = new table(8,gtbl,0);
					tbl = sti_tbl;
					if (n_sto == EXTERN) nn->n_sto = n_sto = 0;
				}

				if (init) {
// error('d',"init %k",init->base);
					if (init->base==VALUE) {
						switch (init->tp2->base) {
						case CLASS:
// error('d',"class %t %t",Pclass(init->tp2),cl);
							if (Pclass(init->tp2)!=cl) goto inin;
							break;
						default:
							Pname  n2 = init->tp2->is_cl_obj();
// error('d',"default %t %t",n2->tp,cl);
							if (n2==0 || Pclass(n2->tp)!=cl) goto inin;
						}

						Pexpr ee = init->e1;
// error('d',"init->e1 %k",ee->base);
						if (ee && vec_seen==0)
						switch (ee->base) {
						case CALL:	// T a = f();
							init = ee;
							goto inin;
						case ELIST:	// T a(f());
							if (ee->e1->base==CALL
							&& ee->e2 == 0) {
								init = ee->e1;
								goto inin;
							}
						} // end switch

						init->e2 = oo;
						init = init->typ(tbl);

						if (init->base == G_CM)	// beware of type conversion operators
						switch (init->tp2->base) {
						case CLASS:
							if (Pclass(init->tp2)!=cl) goto inin;
							break;
						default:
							Pname  n2 = init->tp2->is_cl_obj();
							if (n2==0 || Pclass(n2->tp)!=cl) goto inin;
						}
					}
					else {
					inin:
// error('d',"inin %k",init->base);
						init = init->typ(tbl);
//error('d', "inin: init->typ: %d %k n->tp %t init->tp %t",init->base,init->base,nn->tp,init->tp);
						if (init->base==G_CM
						&& nn->tp->check(init->tp,0)==0)
							(void) replace_temp(init,nn->address());
						else
							init = class_init(nn,nn->tp,init,tbl);
					}
				}
				else if (vec_seen == 0) {
//error('d',"make value");
					init = new texpr(VALUE,cl,0);
					init->e2 = oo;
					init = init->typ(tbl);
				}

				Pname c;
				if (vec_seen) {
					c = cl->has_ictor();
					if (c == 0)
						error("array ofC%n that does not have aK taking noAs",cn);
					else if (Pfct(c->tp)->nargs)
						error("defaultAs forK for array ofC%n",cn);
				}

// error( 'd', "stct: %d init: %d", stct, init );
				if (stct) {
					if (tbl!=gtbl && nn->n_sto==EXTERN) {
						error(&where,"Id local extern%n",this);
						init = 0;
					}
					else if (init) 
						init->base = STAT_INIT;
					else {
  						if (tp->base == VEC && Pvec(tp)->size ) {
  							Pexpr ilist = 0;
  							for (int i=Pvec(tp)->size; i>0; i--) {
  								Pexpr e = new texpr(VALUE,cl,0);
  								ilist = new expr(ELIST, e, ilist);
  							}
  							nn->n_initializer=new expr(ILIST,ilist,0);
  							init = co_array_init(nn,tbl);
  							nn->n_initializer = n_initializer = init = new expr( STAT_INIT, init, 0 );
  						}
  						else
						error('s',"local staticC%n (%t)",this, tp);
				 	}
				}

// error('d', "%n->sti: %d vec_seen: %d n_stclass: %k", this, sti, vec_seen, n_stclass );
	if (sti) {
		if (vec_seen) {	// _vec_new(vec,noe,sz,ctor);
// error('d', "%n->dcl: n_stclass: %k ", this, n_stclass );
			if ( n_stclass == STATIC && n_initializer ) {
				const_save = 1;
				(void) co_array_init(nn,sti_tbl);
				const_save = 0;
				n_initializer = init = 0;
				goto ggg;
			}

			init = cdvec(vec_new_fct,nn,cl,c,-1,0);
			init->tp = any_type;
		}
		else {
			switch (init->base) {
			case DEREF:		// *constructor?
				if (init->e1->base == G_CALL) {	
					Pname fn = init->e1->fct_name;
					if (fn==0 || fn->n_oper!=CTOR) goto as;
					init = init->e1;
					break;
				}
				goto as;
			case G_CM:
				init = init->e1;
					// suppress further type checking
				if (init->tp == 0) init->tp= any_type; 
				break;
			case ASSIGN:
				if (init->e1 == nn) break;	// simple assignment
			as:	
			default:	
				init = new expr(ASSIGN,nn,init);
			}
		}
		Pstmt ist = new estmt(SM,nn->where,init,0);
		// constructors for statics are executed in order
		if (st_ilist == 0)
			st_ilist = ist;
		else
			itail->s_list = ist;
		itail = ist;
		init = 0;	// suppress further processing
	} // if (sti)
				nn->n_initializer = n_initializer = init;
				const_save = const_old;
				tbl = otbl;
			}
			else if (init == 0)		// no initializer
				goto str;
			else if (cl->is_simple()
			//	&& cl->csu!=UNION	// accept ANSIism
				&& cl->csu!=ANON
				) {	// struct
				init = init->typ(tbl);
				if (nn->tp->check(init->tp,0)==0
				&& init->base==G_CM) 
					(void) replace_temp(init,nn->address());
				else {
					if (ansi_opt==0
					&& init->base==ILIST
					&& cl->csu==UNION)
						error('s',"initialization of union withIL");
					goto str;
				}
			}
			else if (init->base == ILIST) {	// class or union
				error("cannotI%nWIrL",nn);
			}
			else {			// bitwise copy ok?
						// possible to get here?
				init = init->typ(tbl);
//error('d',"init22 %t %t",nn->tp,init->tp);
				if (nn->tp->check(init->tp,0)==0) {
					if (init->base==G_CM) 
						(void) replace_temp(init,nn->address());
					else
						goto str;
				}
				goto str;
			//	else
			//		error("cannotI%n:%k %s has noK",nn,cl->csu,cl->string);
			}
			break;
		}
		case VEC:	
			t = Pvec(t)->typ;
			vec_seen++;
			goto lll;
		case TYPE:
			if (init==0 && Pbase(t)->b_const) {
				switch (n_scope) {
				case ARG:
					break;
				case 0:
				case PUBLIC:
					if ( cc->cot ) break;
				default: {
					Pname n = t->is_cl_obj();
					Pclass cl;
					if ( n ) cl = Pclass( n->tp );

					if (n_sto!=EXTERN && 
					   (n==0 || (cl->has_ctor()==0 && is_empty(cl,1)==0))) 
						error("uninitialized const%n",this);
					}
				}
			}
			t = Pbase(t)->b_name->tp;
			goto lll;
		case RPTR:
			if (init) {
				if (nn->n_scope == ARG) break;
				if (Pptr(nn->tp)->memof) error("R toCM%n ofT%t illegal",nn,nn->tp);
				ref_initializer++;
				init = init->typ(tbl);
				ref_initializer--;
				Nvirt = 0; // set within ref_init() call
				const_ptr = Pbase(Pptr(t)->typ)->tconst(); 

				if (n_sto==STATIC
				&& init->lval(0)==0
				&& fct_const==0)
					error("Ir for staticR%n not an lvalue",this);
				else
					nn->n_initializer = n_initializer = init = ref_init(Pptr(t),init,tbl);

				const_ptr = 0;
				if (Nvirt == VIRTUAL) sti_vb = 1;
				nn->assign();

				if (init->base==ILIST && init->e2==0) {
					new_list(init);
					list_check(nn,nn->tp,0,tbl);
					if (next_elem()) error(&where,"IrL too long");
				}

			}
			else {
				switch (nn->n_scope) {
				default:
					if (n_sto == EXTERN) break;
					error("uninitializedR%n",this);
				case ARG:
					break;
				case PUBLIC:
				case 0:
			//III		if (n_sto == STATIC) error("a staticM%n cannot be aR",this);
// error('d', "%n->dcl cot: %s", this, cc->cot?cc->cot->string:"notset");
					if ( cc->cot == 0 )
						error("uninitializedR%n",this);
					break;
				}
			}
			goto stgg;
		default:
		str:
// error('d',"str %n %t %k %t",this,tp,init?init->base:0,init?init->tp:0);
			if (init == 0) {
				switch (n_scope) {
				case ARG:
					break;
				case 0:
				case PUBLIC:
					if ( cc->cot ) break;
				default:
					if (n_sto!=EXTERN && t->tconst()) error("uninitialized const%n",this);
				}

				break;
			}

			const_save = 	   const_save
					|| n_scope==ARG
					|| (t->tconst() && vec_const==0);

			if ( tp->base == PTR ) {
				if ( cc && cc->nof 
					&& Pfct(cc->nof->tp)->f_const ) 
						cm_const_save = Pbase(Pptr(tp)->typ)->b_const; 
				const_ptr = Pbase(Pptr(tp)->typ)->b_const; 
			}

  			if (n_sto==STATIC) stat_init++;
                        nn->n_initializer = n_initializer = init = init->typ(tbl);
  			if (n_sto==STATIC) stat_init--;

			cm_const_save = 0;
			const_ptr = 0;

			if (const_save) PERM(init);
			nn->assign();
			const_save = const_old;
//error('d',"init2 %k %t",init->base,init->tp);
			switch (init->base) {
			case ILIST:
				if (init->e2) goto dfdf;//break; // pointer to member
				new_list(init);
				list_check(nn,nn->tp,0,tbl);
				if (next_elem()) error(&where,"IrL too long");
				break;
			case STRING:
			{	Ptype v = nn->tp->skiptypedefs();
				if (v->base==VEC) {
					Ptype vv = Pvec(v)->typ->skiptypedefs();
					if(vv->base==CHAR) {
						int sz = Pvec(v)->size;
						int isz = Pvec(init->tp)->size;
						if (sz == 0)
							Pvec(v)->size = isz;
						else if (sz < isz)
							error(&where,"Ir too long (%d characters) for%n[%d]",isz,nn,sz);
						break;
					}
				}
				// no break
			}
			default:
			dfdf:
			{	bit ntc = Pbase(nn->tp)->b_const;
				Ptype nt = nn->tp->skiptypedefs(ntc);

				if (vec_seen) {
					error("badIr for array%n",nn);
					break;
				}

				switch (nt->base) {
				case INT:
				case CHAR:
				case SHORT:
				case EOBJ:
				//	if (init->base==ICON && init->tp==long_type)
				//		error('w',"longIr constant for%k%n",nn->tp->base,nn);
				{	Ptype t = init->tp->skiptypedefs();
					switch (t->base) {
					case LONG:
					case FLOAT:
					case DOUBLE:
					case LDOUBLE:
						error('w',"%tIdW %t",nt,init->tp);
					}
				}
					// no break
				case LONG:
					if (Pbase(nt)->b_unsigned
					&& init->base==UMINUS
					&& init->e2->base==ICON)
						error('w',"negativeIr for unsigned%n",nn);
					if (ntc && scope!=ARG) {
						long i;
						Neval = 0;
						i = init->eval();
						if (Neval == 0) {
							DEL(init);
							nn->n_evaluated = n_evaluated = 1;
							nn->n_val = n_val = i;
							DEL(init);
							nn->n_initializer = n_initializer = 0;
						}
					}
					break;
				case PTR:
					Nvirt = 0;
					nn->n_initializer = n_initializer = init = ptr_init(Pptr(nt),init,tbl);
//display_expr(init,string);
					if (Nvirt == VIRTUAL) sti_vb = 1;
					if (Pchecked) goto stgg;
				} 

			{	Pexpr x = try_to_coerce(nt,init,"initializer",tbl);
				if (x) {
					nn->n_initializer = n_initializer = init = x;
					goto stgg;
				}
			}
//error('d',"check %t %t %k",nt,init->tp,init->base);
		//	if (nt->check(init->tp,ASSIGN)) {
				Pname c1 = nt->is_cl_obj();
				Pname c2 = init->tp->is_cl_obj();
				if (c1
				&& c2
				&& Pclass(c2->tp)->has_base(Pclass(c1->tp))) {
					init = new texpr(CAST,new ptr(PTR,nt),init->address());
					init = init->typ(tbl);
					nn->n_initializer = n_initializer = init = init->contents();
					goto stgg;
				}

				if (nt->check(init->tp,ASSIGN)) {
					error("badIrT%t for%n (%tX)",init->tp,this,nn->tp);
					break;
				}
		}

		stgg:
// error('d',"stgg %n init %k %t Nvirt: %k",this,init?init->base:0,init?init->tp:0, Nvirt);
			if (init && n_stclass==STATIC && (sti_vb || need_sti(init))) {
				/* check if non-static variables are used */
				int local = (0<lex_level);
// error('d',"init %n %t local %d",nn,init->tp,local);
				if (local==0) need_sti(init,tbl);	// save consts

				Pptr r = nn->tp->is_ref();	//III

				if (r) init = init->address();
				init = new expr(ASSIGN,nn,init);
// error('d',"init r %t: nn %n %t init %t",r,nn,nn->tp,init->tp);
				if (r)
					init->tp = nn->tp;
				else if (nn->tp!=init->tp) { // static member refs
					TOK t = nn->tp->set_const(0);	//JJJ
					init = init->typ(tbl);
					nn->tp->set_const(t);		//JJJ
				}

				if (local) {
					if (init->base != ASSIGN) error('s',"Ir for local static too complicated");
					if (nn->n_sto == EXTERN) {
						error(&where,"Id local extern%n",this);
							init = 0;
					}
					else init->base = STAT_INIT;
					nn->n_initializer = n_initializer = init;
					
				}
				else {
					Pstmt ist = new estmt(SM,nn->where,init,0);
					// constructors for statics are executed in order
		
					if (st_ilist == 0)
						st_ilist = ist;
					else
						itail->s_list = ist;
					itail = ist;
					nn->n_initializer = n_initializer = init = 0;	// suppress further processing
					nn->n_val = n_val = 1;
				}
			}
			
		
		} /* switch */
	} /* block */
	} /* default */

	} /* switch */
ggg:
//error('d',"ggg");
	PERM(nn);
	switch (n_scope) {
	case FCT:
		nn->n_initializer = n_initializer;
		break;
	default:
	{	Ptype t = nn->tp;
	px:
		PERM(t);
		switch (t->base) {
		case PTR:
		case RPTR:
		case VEC:	t = Pptr(t)->typ; goto px;
		case TYPE:	t = Pbase(t)->b_name->tp; goto px;
		case FCT:	t = Pfct(t)->returns; goto px; /* args? */
		} 
	}
	}
	
	Cdcl = odcl;
	return nn;
}
