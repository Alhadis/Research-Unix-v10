/*ident	"@(#)ctrans:src/dcl2.c	1.3.4.22" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

dcl2.c:

*************************************************************************/

#include "cfront.h"
#include "size.h"

Pname conv_dominates(Pname on1, Pname on2)
/*
	compare for duplicates and dominance:

	on1 and on2 are two conversion operator functions
	return the the one that dominates the other (according to the
	class hierarchy) otherwise 0 (0 thus indicats ambiguous conversion)
*/
{ 
	Ptype r1 = Pfct(on1->tp)->returns;
	Ptype r2 = Pfct(on2->tp)->returns;
//error('d',"conv_dominates(%n,%n)",on1,on2);
	if (r1==r2 || r1->check(r2,0)==0) return on1;
	Pptr p1 = r1->is_ptr_or_ref();
	Pptr p2 = r2->is_ptr_or_ref();
	if (p1 && p2) {
		Pname cn1 = p1->typ->is_cl_obj();
		Pname cn2 = p2->typ->is_cl_obj();
		if (cn1 && cn2) {
// is_cl_obj() returns b_name
//			Pclass c1 = Pclass(Pbase(cn1->tp)->b_name->tp);
//			Pclass c2 = Pclass(Pbase(cn2->tp)->b_name->tp);
			Pclass c1 = Pclass(cn1->tp);
			Pclass c2 = Pclass(cn2->tp);
			if (c1 && c2) {
				if (c1==c2 || c1->has_base(c2)) 
					return on1;
				else if (c2->has_base(c1)) 
					return on2;
			}
		}
	}
	return 0;
}


Pstmt curr_loop;
Pstmt curr_switch;
Pblock curr_block;

void stmt::reached()
{
	register Pstmt ss = s_list;

	if (ss == 0) return;

	switch (ss->base) {
	case LABEL:
	case CASE:
	case DEFAULT:
		break;
	default:
		error('w',"S after%k not reached",base);
		for (; ss; ss=ss->s_list) {	// delete unreacheable code
			switch (ss->base) {
			case LABEL:
			case CASE:
			case DEFAULT:		// reachable
				s_list = ss;
				return;
			case DCL:		// the dcl may be used later
						// keep to avoid cascading errors
			case IF:
			case DO:
			case WHILE:
			case SWITCH:
			case FOR:
			case BLOCK:		// may hide a label
				s_list = ss;
				return;
			}
		}
		s_list = 0;
	}
}

bit arg_err_suppress;

Pexpr check_cond(Pexpr e, TOK b, Ptable tbl)
{
//error('d',"check_cond(%k %k) tbl %d",e->base,b,tbl);
	Pname cn;

	if (e == dummy) error("empty condition for %k",b);

	if (cn = e->tp->is_cl_obj()) {	
		Pclass cl = Pclass(cn->tp);
		int i = 0;
		Pname found = 0;
		for (Pname on = cl->conv; on; on=on->n_list) {
			Pfct f = Pfct(on->tp);
			Ptype t = f->returns;
		xx:
			switch (t->base) {
			case TYPE:
				t = Pbase(t)->b_name->tp;
				goto xx;
			case FLOAT:
			case DOUBLE:
			case LDOUBLE:
			case PTR:
				if (b == DEREF) break;
			case CHAR:
			case SHORT:
			case INT:
			case LONG:
			case EOBJ:
//                              if (found==0 || found->tp->check(on->tp,0)) {
//error('d',"found %n on %n",found,on);
			{	Pname xx = found;
                                if (found == 0) {
                                        i = 1;
                                        found = on;
                                }
                                else if ((found = conv_dominates(found,on)) == 0) {
                                        i = 2;
                                        found = on;
                                        error("two conversions for%nO in%kE: %n and %n",cn,b,xx,on);
					return e;
				}
			}
			}
		}
//error('d',"i %d",i);
		switch (i) {
		case 0:
			error("%nO in%kE",cn,b);
			return e;
/*
		case 1:
		{	Pname xx = new name(found->string);
			Pref r = new ref(DOT,e,xx);
			Pexpr rr = r->typ(tbl);
			Pexpr c = new expr(G_CALL,rr,0);

			if (e->lval(0)) {
			//	Pref r = new ref(DOT,e,found);
			//	r->tp = found->tp;
			//	Pexpr c = new expr(G_CALL,r,0);
			//	c->fct_name = found;
				return c->typ(tbl);
			}
			else {	// (temp=init,temp.coerce())
				Pname tmp = make_tmp('U',e->tp,tbl); 
				Pexpr ass = init_tmp(tmp,e,tbl);
			//	Pref r = new ref(DOT,tmp,found);
			//	Pexpr rr = r->typ(tbl);
			//	Pexpr c = new expr(G_CALL,rr,0);
			//	c->fct_name = found;
				ass = new expr(CM,ass,c);
				return ass->typ(tbl);
			}
		}
*/
		case 1:
		{	Pname xx = new name(found->string);
			Pexpr c;

			if (e->lval(0)) {
				Pref r = new ref(DOT,e,xx);
				Pexpr rr = r->typ(tbl);
				c = new expr(G_CALL,rr,0);
			}
			else {	// (temp=init,temp.coerce())
				Pname tmp = make_tmp('U',e->tp,tbl); 
				Pexpr ass = init_tmp(tmp,e,tbl);
				Pref r = new ref(DOT,tmp,xx);
				Pexpr rr = r->typ(tbl);
				c = new expr(G_CALL,rr,0);
				c = new expr(CM,ass,c);
			}
			return c->typ(tbl);
		}
	//	default:
	//		error("%d possible conversions for%nO in%kE",i,cn,b);
	//		return e;
		}
		
	}
	if (e->tp->memptr()) {
		e = new mdot("i",e);
		e->i1 = 9;
		e = new expr(NE,e,zero);
	}
	else if (e->tp->num_ptr(b) == FCT)
		error("%k(F)",b);
	return e;
}

extern int catch_delete_bugs;

void stmt::dcl()
/*
	typecheck statement "this" in scope "curr_block->tbl"
*/
{
	Pstmt ss;
	Pname n;
	Pname nn;
	Pstmt ostmt = Cstmt;

	if(permanent == 3 && catch_delete_bugs) 
	    error ('i', "stmt::dcl of a deleted stmt.");

	for (ss=this; ss; ss=ss->s_list) {
		Pstmt old_loop, old_switch;
		Cstmt = ss;
		Ptable tbl = curr_block->memtbl;
//error('d',"stmt::dcl %k",ss->base);
		switch (ss->base) {
		case BREAK:
			inline_restr |= 16;
			if (curr_loop==0 && curr_switch==0)
				error("break not in loop or switch");
			ss->reached();
			break;

		case CONTINUE:
			inline_restr |= 32;
			if (curr_loop == 0) error("continue not in loop");
			ss->reached();
			break;

		case DEFAULT:
			if (curr_switch == 0) {
				error("default not in switch");
				break;
			}
			if (curr_switch->has_default) error("two defaults in switch");
			curr_switch->has_default = ss;
			ss->s->s_list = ss->s_list;
			ss->s_list = 0;
			ss->s->dcl();
			break;

		case SM:
		{	if (ss->e ==0) break;
			TOK b = ss->e->base;
			switch (b) {
			case DUMMY:
				ss->e = 0;
				break;
					// check for unused results
					// don't check operators that are likely
					// to be overloaded to represent "actions":
					// ! ~ < <= > >= << >>
			case EQ:
			case NE:
			case PLUS:
			case MINUS:
			case REF:
			case DOT:
			case MUL:
			case DIV:
			case ADDROF:
			case AND:
			case OR:
			case ER:
			case DEREF:
			case ANDAND:
			case OROR:
			case NAME:
			case VALUE:
				if (ss->e->tp) break;	// avoid looking at generated code
				ss->e = ss->e->typ(tbl);
				if (ss->e->base == CALL) break;
				if (ss->e->tp->base != VOID) {
					error('w',"result of%kE not used",b);
					if (ss->e->not_simple()==0) ss->e = dummy;
				}
				break;
			default:
				ss->e = ss->e->typ(tbl);
			}
			break;
		}
		case RETURN:
		{	Pname fn = cc->nof;
			Pfct f = Pfct(fn->tp);
			Ptype rt = f->returns;
			Pexpr v = ss->e;
//error('d',"rt %t",rt);
			if (v != dummy) {
				while (rt->base == TYPE) rt = Pbase(rt)->b_name->tp;
				extern ref_initializer;
				if (rt->base == RPTR) {
					ref_initializer++;
					v = v->typ(tbl);
					ref_initializer--;
				} else
					v = v->typ(tbl);

				if (fn->n_oper==CTOR
				|| fn->n_oper==DTOR
				|| (rt->base==VOID && v->tp!=void_type)) {
					error("unexpected return value");
					// refuse to return the value:
					ss->e = dummy;
				}
				else {
				lx:
					switch (rt->base) {
					case TYPE:
						rt = Pbase(rt)->b_name->tp;
						goto lx;
					case RPTR:
						switch (v->base) {
						case NAME:
							if (Pname(v)->n_scope==FCT
							|| Pname(v)->n_scope==ARG)
								error('w',"R to localV returned");
							break;
						case ICON:
						case CCON:
						case FCON:
						case STRING:
							if (Pptr(rt)->typ->tconst()==0)
								error('w',"R to literal returned");
						}
						v = ref_init(Pptr(rt),v,tbl);
						if (v->base==G_CM
						&& v->e2->base==G_ADDROF
						&& v->e2->e2->base==NAME)
							error('w',"R to temporary returned (return value is not lvalue or of wrongT)");
					case ANY:
						break;
					case COBJ:
if (v->base == DEREF) {
	Pexpr v1 = v->e1;
	if (v1->base==CAST) {
		Pexpr v2 = v1->e1;
		if (v2->base == G_CM) {	// *(T)(e1,e2) => (e1,*(T)e2)
			Pexpr v3 = v2->e2;
			v2->e2 = v;
			v2->tp = v->tp;
			v = v2;
			v1->e1 = v3;
		}
	}
}
	if (f->f_result) {
		if (v->base==G_CM && rt->check(v->tp,0/*ASSIGN*/)==0)
			v = replace_temp(v,f->f_result);
		else {
			v = class_init(f->f_result->contents(),rt,v,tbl);
			Pname rcn = rt->is_cl_obj();
			if (Pclass(rcn->tp)->has_itor()==0) {
				// can happen for virtuals and for user defined conversions
				v->tp = rt;
				v = new expr(ASSIGN,f->f_result->contents(),v);
				v->tp = rt;
			}
		}
	}
	else
		v = class_init(0,rt,v,tbl);
						break;
					case PTR:
					{	Pexpr x = v;
						v = ptr_init(Pptr(rt),v,tbl);
						if (v->base == ADDROF
						&& v->e2->base == NAME
						&& Pname(v->e2)->n_stclass == AUTO)
							error('w',"P to local variable%n returned",Pname(v->e2));
					//	if (v==x ||v->e2==x) goto def;
						if (Pchecked == 0) goto def;
						goto ret_save;
					//	break;
					}
					case INT:
					case CHAR:
					case LONG:
					case SHORT:
						if (Pbase(rt)->b_unsigned
						&& v->base==UMINUS
						&& v->e2->base==ICON)
							error('w',"negative retured fromF returning unsigned");
					default:
					def:
					{
						Pexpr x = try_to_coerce(rt,v,"return value",tbl);
						if (x)
							v = x;
						else if (rt->check(v->tp,ASSIGN))
							error("bad return valueT for%n:%t (%tX)",fn,v->tp,rt);
					}

					}
				ret_save:
					ss->ret_tp = rt;
					ss->e = v;
				}
			}
			else {
				if (rt->base != VOID) error("return valueX");
			}
			ss->reached();
			break;
		}

		case DO:	// in DO the stmt is before the test
			inline_restr |= 8;
			old_loop = curr_loop;
			curr_loop = ss;
			if (ss->s->base == DCL) error("D as onlyS in do-loop");
			ss->s->dcl();
			ss->e = ss->e->typ(tbl);
			ss->e = check_cond(ss->e,DO,tbl);
			curr_loop = old_loop;
			break;

		case WHILE:
			inline_restr |= 8;
			old_loop = curr_loop;
			curr_loop = ss;
			ss->e = ss->e->typ(tbl);
			ss->e = check_cond(ss->e,WHILE,tbl);
			if (ss->s->base == DCL) error("D as onlyS in while-loop");
			ss->s->dcl();
			curr_loop = old_loop;
			break;

		case SWITCH:
		{	int ne = 0;
			inline_restr |= 4;
			old_switch = curr_switch;
			curr_switch = ss;
			ss->e = ss->e->typ(tbl);
			ss->e = check_cond(ss->e,SWITCH,tbl);
			if (ss->s->base == DCL) error("D as onlyS in switchS");
			{	Ptype tt = ss->e->tp;
			sii:
				switch (tt->base) {
				case TYPE:
					tt = Pbase(tt)->b_name->tp; goto sii;
				case EOBJ:
					ne = Penum(Pbase(tt)->b_name->tp)->no_of_enumerators;
				case ZTYPE:
				case ANY:
				case CHAR:
				case SHORT:
				case INT:
				case LONG:
				case FIELD:
					break;
				default:
					error("%t switchE",ss->e->tp);
				}
			}
			ss->s->dcl();
			if (ne) {	/* see if the number of cases is "close to"
					   but not equal to the number of enumerators
					*/
				int i = 0;
				Pstmt cs;
				for (cs=ss->case_list; cs; cs=cs->case_list) i++;
				if (i && i!=ne) {
					if (ne < i) {
				ee:		error('w',"switch (%t)W %d cases (%d enumerators)",ss->e->tp,i,ne);
					}
					else {
						switch (ne-i) {
						case 1: if (3<ne) goto ee;
						case 2: if (7<ne) goto ee;
						case 3: if (23<ne) goto ee;
						case 4: if (60<ne) goto ee;
						case 5: if (99<ne) goto ee;
						}
					}
				}
			}
			curr_switch = old_switch;
			break;
		}
		case CASE:
			if (curr_switch == 0) {
				error("case not in switch");
				break;
			}
			ss->e = ss->e->typ(tbl);
			ss->e->tp->num_ptr(CASE);
			{	Ptype tt = ss->e->tp;
			iii:
				switch (tt->base) {
				case TYPE:
					tt = Pbase(tt)->b_name->tp; goto iii;
				case ZTYPE:
				case ANY:
				case CHAR:
				case SHORT:
				case INT:
				case LONG:
				case EOBJ:
					Neval = 0;
					long i = ss->e->eval();
					if (Neval == 0) {
						Pstmt cs;
						if (largest_int<i) error("long case value");
						for (cs=curr_switch->case_list; cs; cs=cs->case_list) {
							if (cs->case_value == i) error("case %d used twice in switch",i);
						}
						ss->case_value = int(i);
						ss->case_list = curr_switch->case_list;
						curr_switch->case_list = ss;
					}
					else
						error("bad case label: %s",Neval);
					break;
				default:
					error("%t caseE",ss->e->tp);
				}
			}
		//	if (1) {
		//		Neval = 0;
		//		long i = ss->e->eval();
		//		if (Neval == 0) {
		//			Pstmt cs;
		//			if (largest_int<i) error("long case value");
		//			for (cs=curr_switch->case_list; cs; cs=cs->case_list) {
		//				if (cs->case_value == i) error("case %d used twice in switch",i);
		//			}
		//			ss->case_value = int(i);
		//			ss->case_list = curr_switch->case_list;
		//			curr_switch->case_list = ss;
		//		}
		//		else
		//			error("bad case label: %s",Neval);
		//	}
			if (ss->s->s_list) error('i',"case%k",ss->s->s_list->base);
			ss->s->s_list = ss->s_list;
			ss->s_list = 0;
			ss->s->dcl();
			break;

		case GOTO:
			inline_restr |= 2;
			ss->reached();
		case LABEL:
			/* Insert label in function mem table;
			   labels have function scope.
			*/
			n = ss->d;
			nn = cc->ftbl->insert(n,LABEL);

			/* Set a ptr to the mem table corresponding to the scope
			   in which the label actually occurred.  This allows the
			   processing of goto's in the presence of ctors and dtors
			*/
			if (ss->base == LABEL) {
				nn->n_realscope = curr_block->memtbl;
				inline_restr |= 1;
			}

			if (Nold) {
				if (ss->base == LABEL) {
					if (nn->n_initializer) error("twoDs of label%n",n);
					nn->n_initializer = (Pexpr)1;
				}
				if (n != nn) ss->d = nn;
			}
			else {
				if (ss->base == LABEL) nn->n_initializer = (Pexpr)1;
				nn->where = ss->where;
			}
			if (ss->base == GOTO)
				nn->use();
			else {
				if (ss->s->s_list) error('i',"label%k",ss->s->s_list->base);
				ss->s->s_list = ss->s_list;
				ss->s_list = 0;
				nn->assign();
			}
			if (ss->s) ss->s->dcl();
			break;

		case IF:
		{	
			Pexpr ee = ss->e->typ(tbl);
			if (ee->base == ASSIGN) {
				Neval = 0;
				(void)ee->e2->eval();
				if (Neval == 0)
					error('w',"constant assignment in condition");
			}
			ss->e = ee = check_cond(ee,IF,tbl);

			if (ss->s->base == DCL) error("D as onlyS after `if'");

			// pointer to member returns with a tp set to 0
			if ( ee->tp ) switch (ee->tp->base) {
			case INT:
			case EOBJ:
			case ZTYPE:
			{	long i;
				Neval = 0;
				i = ee->eval();

				if (Neval == 0) {
					Pstmt sl = ss->s_list;
					if (i) {
						DEL(ss->else_stmt);
						ss->s->dcl();
						*ss = *ss->s;
					}
					else {
						DEL(ss->s);
						if (ss->else_stmt) {
							ss->else_stmt->dcl();
							*ss = *ss->else_stmt;
						}
						else {
							ss->base = SM;
							ss->e = dummy;
							ss->s = 0;
						}
					}
					ss->s_list = sl;
					continue;
				}
			}
			}
			ss->s->dcl();
			if (ss->else_stmt) {
				if (ss->else_stmt->base == DCL) error("D as onlyS after `else'");
				ss->else_stmt->dcl();
			}
			break;
		}
		case FOR:
			inline_restr |= 8;
			old_loop = curr_loop;
			curr_loop = ss;
			if (ss->for_init) {
				Pstmt fi = ss->for_init;
				switch (fi->base) {
				case SM:
					if (fi->e == dummy) {
						ss->for_init = 0;
						break;
					}
				default:
					fi->dcl();
					break;
				case DCL:
	//	if ()
	//	{
	//		// for (dcl; e1; e2) stmt1 stmt2
	//		// => { dcl; for( ; e1; e2) stmt1 } stmt2
	//		ss->base = BLOCK;
	//		ss->d = fi->d;
	//		ss->s = new forstmt(ss->where,new estmt(SM,ss->where,dummy,0),ss->e,ss->e2,ss->s);
	//		Pblock(ss)->dcl(tbl);
	//		continue;
	//	}
	//	else {
			fi->dcl();
			switch (fi->base) {
			case BLOCK:
			{
				// { ... for( { a } b ; c) d ; e }
				// => { ... { a for ( ; b ; c) d ; e }}
				Pstmt tmp = new stmt (SM,curloc,0);
				*tmp = *ss;	// tmp = for
				tmp->for_init = 0;
				*ss = *fi;	// ss = { }
				if (ss->s)
					ss->s->s_list = tmp;
				else
					ss->s = tmp;
				curr_block = Pblock(ss);
				tbl = curr_block->memtbl;
				Cstmt = ss = tmp;	// rest of for and s_list
				break;
					}
					}
	//	}

				}
			}
			if (ss->e == dummy)
				ss->e = 0;
			else {
				ss->e = ss->e->typ(tbl);
				ss->e = check_cond(ss->e,FOR,tbl);
			}
			if (ss->s->base == DCL) error("D as onlyS in for-loop");
			ss->s->dcl();
			ss->e2 = (ss->e2 == dummy) ? 0 : ss->e2->typ(tbl);
			curr_loop = old_loop;
			break;

		case DCL:	/* declaration after statement */
		{
			/*	collect all the contiguous DCL nodes from the
				head of the s_list. find the next statement
			*/
			int non_trivial = 0;
			int count = 0;
			Pname tail = ss->d;
			for (Pname nn=tail; nn; nn=nn->n_list) {
				//	find tail;
				//	detect non-trivial declarations
				count++;

				if (nn->n_list) tail = nn->n_list;
				Pname n = tbl->look(nn->string,0);

				if (n && n->n_table==tbl) non_trivial = 2;
				if (non_trivial == 2) continue;
				if ((nn->n_sto==STATIC && nn->tp->base!=FCT)
				|| nn->tp->is_ref()
				|| (nn->tp->tconst() && fct_const==0)) {
					non_trivial = 2;
					continue;
				}

				Pexpr in = nn->n_initializer;
				if (in)
					switch (in->base) {
					case ILIST:
					case STRING:
						non_trivial = 2;
						continue;
					default:
						non_trivial = 1;
					}
				Pname cln = nn->tp->is_cl_obj();
				if (cln == 0) cln = cl_obj_vec;
				if (cln == 0) continue;
				if (Pclass(cln->tp)->has_ctor()) {
					non_trivial = 2;
					continue;
				}
				if (Pclass(cln->tp)->has_dtor()) non_trivial = 2;
			}

			while( ss->s_list && ss->s_list->base==DCL ) {
				Pstmt sx = ss->s_list;
				tail = tail->n_list = sx->d;	// add to tail
				for (nn=sx->d; nn; nn=nn->n_list) {
					//	find tail;
					//	detect non-trivial declarations
					count++;
					if (nn->n_list) tail = nn->n_list;
					Pname n = tbl->look(nn->string,0);
					if (n && n->n_table==tbl) non_trivial = 2;
					if (non_trivial == 2) continue;
					if ((nn->n_sto==STATIC && nn->tp->base!=FCT)
					|| nn->tp->is_ref()
					|| (nn->tp->tconst() && fct_const==0)) {
						non_trivial = 2;
						continue;
					}
					Pexpr in = nn->n_initializer;
					if (in)
						switch (in->base) {
						case ILIST:
						case STRING:
							non_trivial = 2;
							continue;
						}
					non_trivial = 1;
					Pname cln = nn->tp->is_cl_obj();
					if (cln == 0) cln = cl_obj_vec;
					if (cln == 0) continue;
					if (Pclass(cln->tp)->has_ctor()) {
						non_trivial = 2;
						continue;
					}
					if (Pclass(cln->tp)->has_dtor()) continue;
				}
				ss->s_list = sx->s_list;
			/*	delete sx;	*/
			}

			Pstmt next_st = ss->s_list;
			if (non_trivial==2	// must
			|| (non_trivial==1	// might
				&& ( curr_block->own_tbl==0	// why not?
				|| inline_restr&3		/* label seen */)
			  	)
			) {
				if (curr_switch && non_trivial==2) {
					Pstmt cs = curr_switch->case_list;
					Pstmt ds = curr_switch->has_default;
					Pstmt bl;
					if (cs == 0)
						bl = ds;
					else if (ds == 0)
						bl = cs;
					else if (cs->where.line<ds->where.line)
						bl = ds;
					else
						bl = cs;

					if ((bl==0 || bl->s->base!=BLOCK) && curr_switch->s->memtbl==tbl)
						error('s',"non trivialD in switchS (try enclosing it in a block)");
				}

				/*	Create a new block,
					put all the declarations at the head,
					and the remainder of the slist as the
					statement list of the block.
				*/
				ss->base = BLOCK;

				/*	check that there are no redefinitions since the last
					"real" (user-written, non-generated) block
				*/
				for( nn=ss->d; nn; nn=nn->n_list ) {
					Pname n;
		if( curr_block->own_tbl
		&& (n=curr_block->memtbl->look(nn->string,0))
		&& n->n_table->real_block==curr_block->memtbl->real_block
		&& n->tp->base!=FCT && n->tp->base!=OVERLOAD
		&& nn->lex_level == n->lex_level )
 			error("twoDs of%n",n);
				}

				/*	attach the remainder of the s_list
					as the statement part of the block.
				*/
				ss->s = next_st;
				ss->s_list = 0;

				/*	create the table in advance, in order to set the
					real_block ptr to that of the enclosing table
				*/
				ss->memtbl = new table(count+4,tbl,0);
				ss->memtbl->real_block = curr_block->memtbl->real_block;
				Pblock(ss)->dcl(ss->memtbl);
			}
			else {	/*	to reduce the number of symbol tables,
					do not make a new block,
					instead insert names in enclosing block,
					and make the initializers into expression
					statements.
				*/
				Pstmt sss = ss;
				for( nn=ss->d; nn; nn=nn->n_list ) {
					Pname n;
//error('d',"nn %n",nn);
		if( curr_block->own_tbl
		&& (n=curr_block->memtbl->look(nn->string,0))
		&& n->n_table->real_block==curr_block->memtbl->real_block
		&& n->tp->base!=FCT && n->tp->base!=OVERLOAD
&& nn->lex_level == n->lex_level )
{
printf( "\ndump tables: bl_level: %d", bl_level );
/*
//extern void display_table( Ptable,int=0 );display_table(curr_block->memtbl);
//extern void display_name( Pname ); display_name( nn ); display_name(n);
*/
			error("twoDs of%n",n);
}
			//error("twoDs of%n",n);
					/*Pname */n = nn->dcl(tbl,FCT);

					if (n == 0) {
						if (ss) {
							ss->base = SM;
							ss->e = 0;
						}
						continue;
					}

					Pexpr in = n->n_initializer;
					n->n_initializer = 0;

					if (ss) {
						sss->base = SM;
						ss = 0;
					}
					else
						sss = sss->s_list = new estmt(SM,sss->where,0,0);
					if (in) {
						switch (in->base) {
						case G_CALL:	/* constructor? */
						{
							Pname fn = in->fct_name;
							if (fn && fn->n_oper==CTOR) break;
						}
						default:
							in = new expr(ASSIGN,n,in);
							in->tp = n->tp;
						}
					//	sss->e = in->typ(tbl);
						sss->e = in;
					}
					else
						sss->e = dummy;
				}

				ss = sss;
				ss->s_list = next_st;
			}
			break;
		}

		case BLOCK:
			Pblock(ss)->dcl(tbl);
			break;

		case ASM:
			/* save string */
		{
			char* s = (char*)ss->e;
			int ll = strlen(s);
			char* s2 = new char[ll+1];
			strcpy(s2,s);
			ss->e = Pexpr(s2);
			break;
		}
		default:
			error('i',"badS(%p %d)",ss,ss->base);
		}
	}
	Cstmt = ostmt;
}

extern int in_class_dcl;
void block::dcl(Ptable tbl)
/*
	Note: for a block without declarations memtbl denotes the table
	for the enclosing scope.
	A function body has its memtbl created by fct::dcl().
*/
{
	int bit_old = bit_offset;
	int byte_old = byte_offset;
	int max_old = max_align;
	Pblock block_old = curr_block;

	if (base != BLOCK) error('i',"block::dcl(%d)",base);

	curr_block = this;
//error('d',"block %k %k",d?d->base:0,s?s->base:0);
	if (d) {
		own_tbl = 1;
		if (memtbl == 0) {
			int nmem = d->no_of_names()+4;
			memtbl = new table(nmem,tbl,0);
			memtbl->real_block = this;
			/*	this is a "real" block from the
				source text, and not one created by DCL's
				inside a block. */
		}
		else
			if (memtbl != tbl) error('i',"block::dcl(?)");

		Pname nx;
		for (Pname n=d; n; n=nx) {
			nx = n->n_list;
			n->dcl(memtbl,FCT);
			switch (n->tp->base) {
			case CLASS:
			case ANON:
			case ENUM:
				break;
			default:
				delete n;
			}
		}
	}
	else
		memtbl = tbl;

	if (s) {
		Pname odcl = Cdcl;
		Pname m;
		int i;

		s->dcl();

		if (own_tbl)
		for (m=memtbl->get_mem(i=1); m; m=memtbl->get_mem(++i)) {
			Ptype t = m->tp;

if (in_class_dcl) m->lex_level -= 1;

			if (t == 0) {
				if (m->n_assigned_to == 0)
				   error("label %sU",m->string);
				if (m->n_used == 0)
				   error('w',"label %s not used", m->string);
				continue;
			}
		ll:
			switch (t->base) {
			case TYPE:	t = Pbase(t)->b_name->tp; goto ll;
			case CLASS:
			case ANON:
			case ENUM:
			case FCT:
			case VEC:	continue;
			}

			if (m->n_addr_taken == 0) {
				if (m->n_used) {
					if (m->n_assigned_to) {
					}
					else {
						switch (m->n_scope) {
						case FCT:
							Cdcl = m;
							if (m->string[0] != '_' && m->string[1] != '_' )
								error('w',&m->where,"%n used but not set",m);
						}
					}
				}
				else {
					if (m->n_assigned_to) {
					}
					else if (m->string[0]!='_' || m->string[1]!='_') {
						switch (m->n_scope) {
						case ARG:
						case FCT:
							Cdcl = m;
							error('w',&m->where,"%n not used",m);
						}
					}
				}
			}
		}
		Cdcl = odcl;
	}

	d = 0;

	if (bit_offset) byte_offset += SZ_WORD;
	bit_offset = bit_old;
	byte_offset = byte_old;
	curr_block = block_old;
}

extern int ZB_BOUNDARY;

void name::field_align()
/*
	 adjust alignment
*/
{
    Pbase           fld = (Pbase) tp;

    int             a;
    a = (F_SENSITIVE) ? fld->b_fieldtype->align() : ZB_BOUNDARY;
    if (max_align < a)
	max_align = a;

    if (fld->b_bits == 0) {	/* # force word alignment # */
	int             b;
	if (bit_offset && bit_offset != ZB_BOUNDARY * BI_IN_BYTE)
	    fld->b_bits = (BI_IN_BYTE * ZB_BOUNDARY) - bit_offset;
	else if (b = byte_offset % ZB_BOUNDARY)
	    fld->b_bits = b * BI_IN_BYTE;
	/* #	else # */
	/* #		fld->b_bits = BI_IN_WORD; # */
	if (max_align < ZB_BOUNDARY)
	    max_align = ZB_BOUNDARY;
    } else if (bit_offset == 0) {	/* # take care of part of word # */
	int             b = byte_offset % ZB_BOUNDARY;
	if (b) {
	    byte_offset -= b;
	    bit_offset = b * BI_IN_BYTE;
	}
    }
/*#error('d',"byteoff %d bitoff %d bits %d",byte_offset,bit_offset,fld->b_bits); #*/
    int             x = (bit_offset += fld->b_bits);

    if (BI_IN_WORD < x) {
	fld->b_offset = 0;
	byte_offset += SZ_WORD;
	bit_offset = fld->b_bits;
    } else {
	fld->b_offset = bit_offset;
	if (BI_IN_WORD == x) {
	    bit_offset = 0;
	    byte_offset += SZ_WORD;
	} else
	    bit_offset = x;
    }
    n_offset = byte_offset;
}

/* ODI notes --
   delete bug catching code.
   fix to bitfield alignment on 68K
*/
