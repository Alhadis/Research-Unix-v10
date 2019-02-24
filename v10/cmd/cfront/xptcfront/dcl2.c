/*ident	"@(#)ctrans:src/dcl2.c	1.2" */
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
	Ptype r1 = on1->tp->base==FCT ? Pfct(on1->tp)->returns 
			: Pfct(Pgen(on1->tp)->fct_list->f->tp)->returns;
	Ptype r2 = on2->tp->base==FCT ? Pfct(on2->tp)->returns 
			: Pfct(Pgen(on2->tp)->fct_list->f->tp)->returns;
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
				//if (c1==c2 || c1->has_base(c2)) 
				if (c1->has_base(c2)) 
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
		error('w',&ss->where,"S after%k not reached",base);
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

Pexpr check_cond(Pexpr e, TOK b, Ptable tbl)
{
//error('d',"check_cond(%k %k) tbl %d",e->base,b,tbl);
	Pname cn;
	if (e == dummy) error("empty condition for %k",b);

	int const_obj = 0;
	const_obj = e->tp->tconst();
	Pexpr ee = e;
        while (ee && (ee->base==DOT || ee->base==REF)) {
               	Pexpr m = ee->mem;
               	if ( ee->base==REF && m->tp &&  m->tp->is_ptr())
                       	break;
               	ee = ee->e1;
        }
        if (ee) {
               	Ptype ttt = ee->tp;
               	switch (e->base) {
               	case REF:
               	        Pptr p = ttt?ttt->is_ptr():0;
                       	if (p && p->typ->tconst())
                               	const_obj = 1;
                       	break;
               	case DOT:
                       	if (ttt && ttt->tconst())
                               	const_obj = 1;
               	}
        }

	if (cn = e->tp->is_cl_obj()) {	
		Pclass cl = Pclass(cn->tp);
		int i = 0;
		Pname found = 0;
		for (Pname on = cl->conv; on; on=on->n_list) {
			Pfct f = on->tp->base == FCT ? Pfct(on->tp)
				 : Pfct(Pgen(on->tp)->fct_list->f->tp);
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
                                else if ((found = really_dominate(found,on,const_obj)) == 0) {
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

void stmt::dcl()
/*
	typecheck statement "this" in scope "curr_block->tbl"
*/
{
	Pstmt ss;
	Pname n;
	Pname nn;
	Pstmt ostmt = Cstmt;
	DB( if(Ddebug>=1) error('d',&where,"'%k'->stmt::dcl()",base); );
	for (ss=this; ss; ss=ss->s_list) {
		Pstmt old_loop, old_switch;
		Cstmt = ss;
		enum_promote = 0;
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
					if ( ss->e->base == DEREF )
					   error('w',"result of %sE not used",ss->e->e2?"[]":"*");
					else 
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
			while (rt->base == TYPE) rt = Pbase(rt)->b_name->tp;
			if (v != dummy) {
				if (rt->base == RPTR || rt->base == PTR) {
					const_ptr = Pbase(Pptr(rt)->typ)->tconst();
				}
				if (rt->base == RPTR) {
					ref_initializer++;
					v = v->typ(tbl);
					ref_initializer--;
				} else
					v = v->typ(tbl);
				const_ptr = 0;

				if (fn->n_oper==CTOR
				|| fn->n_oper==DTOR
				|| (rt->base==VOID && v->tp!=void_type)) {
					error("unexpected return value");
					// refuse to return the value:
					ss->e = dummy;
				}
				else {
					switch (rt->base) {
					// case TYPE: laready done above
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
							error('w',"negative returned fromF returning unsigned");
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
			{ Pstmt st = ss->s;
			  while(st && st->base == FOR) st = st->for_init;
			  if (st && st->base == DCL)
			  	if (st==ss->s) error("D as onlyS in do-loop");
			}
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
			{ Pstmt st = ss->s;
			  while(st && st->base == FOR) st = st->for_init;
			  if (st && st->base == DCL)
				if(st==ss->s) error("D as onlyS in while-loop");
			}
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
			{ Pstmt st = ss->s;
			  while(st && st->base == FOR) st = st->for_init;
			  if (st && st->base == DCL)
				if(st==ss->s) error("D as onlyS in switchS");
			}
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
					fi->dcl();
					break;
				default:
					// for (stmt; e1; e2) stmt1 stmt2
					// => {stmt; for(; e1; e2) stmt1 stmt2}
					//    if stmt != declaration
					// if stmt == declaration, temporarily
					//    rewrite to avoid symbol table
					//    problems in some contexts.
					//    Then put decl back to avoid
					//    extraneous {}.
					// Note: to maintain pointers, ss
					//       must not change
				{	Pstmt tmp = new stmt (SM,curloc,0);
					*tmp = *ss;	// tmp = original for
					tmp->for_init = 0;
					fi->s_list = tmp;
					*ss = *fi;
					curr_loop = old_loop;
					ss->dcl();
					tmp = ss->s_list;
					if ( ss->base == DCL
					&&   tmp->base == FOR   // sanity check
					&&   tmp->for_init == 0 // sanity check
					) {
						// put DCL back in for init
						*fi = *ss;
						*ss = *tmp;
						ss->for_init = fi;
						fi->s_list = 0;
					} else {
						// non-decl stmt in for init
						// put stmts in block in case
						//    they follow a condition...
						// allocate tmp to be sure
						//    fields are initialized
						*fi = *ss;
						tmp = new block(ss->where,0,fi);
						tmp->own_tbl = 0;
						tmp->memtbl = curr_block->memtbl;
						tmp->permanent = ss->permanent;
						*ss = *tmp;
						tmp->permanent = 0; delete tmp;
					}
					// don't repeat stmt::dcl() for
					//    remaining stmts
					goto done;
				}
				}
			}
			if (ss->e == dummy)
				ss->e = 0;
			else {
				ss->e = ss->e->typ(tbl);
				ss->e = check_cond(ss->e,FOR,tbl);
			}
			{ Pstmt st = ss->s;
			  while(st && st->base == FOR) st = st->for_init;
			  if (st && st->base == DCL)
				if(st==ss->s) error("D as onlyS in for-loop");
			}
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
					}
				non_trivial = 1;
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
					if (Pclass(cln->tp)->has_dtor()) non_trivial = 2;
				}
				ss->s_list = sx->s_list;
			/*	delete sx;	*/
			}

			Pstmt next_st = ss->s_list;
//error('d',"dcl stmt : d %n  non_trivial %d  curr own_tbl %d  inline_restr 0%o",ss->d,non_trivial,curr_block->own_tbl,inline_restr);
			if (non_trivial==2	// must
			|| (non_trivial==1	// might
				&& ( curr_block->own_tbl==0	// why not?
				|| inline_restr&3	/* label seen */)
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
				//ss->base = BLOCK; //DCL 

				/*	check that there are no redefinitions
					since the last "real" (user-written,
					non-generated) block
				*/
				{ Pname lastnn = 0;
				  for( nn=ss->d; nn; nn=nn->n_list ) {
					Pname n;
//n=curr_block->memtbl->look(nn->string,0);
//error('d',"checking %n  lex_level: %d  n: %n  n->lex_level: %d",nn,nn->lex_level,n,n?n->lex_level:0);
//error('d',"   own_tbl: %d  curr_block: %d  n->n_table: %d",curr_block->own_tbl,curr_block,n->n_table);
//error('d',"   real_block: %d  n's real_block: %d",curr_block->memtbl->real_block,n->n_table->real_block);
					//if( curr_block->own_tbl
					if ( (n=curr_block->memtbl->look(nn->string,0))
					&& n->n_table->real_block==curr_block->memtbl->real_block
					&& n->tp->base!=FCT
					&& n->tp->base!=OVERLOAD
					&& nn->lex_level == n->lex_level ) {
						error("twoDs of%n",n);
						if (lastnn==0) ss->d=nn->n_list;
						else lastnn->n_list=nn->n_list;
					} else lastnn = nn;
				  } // for nn
				}

				/*	attach the remainder of the s_list
					as the statement part of the block.
				*/
				ss->s = next_st;
				ss->s_list = 0;

				/*	create the table in advance,
					in order to set the real_block
					ptr to that of the enclosing table
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
				{ Pname lastnn = 0;
				  for( nn=ss->d; nn; nn=nn->n_list ) {
					Pname n;
//error('d',"nn %n",nn);
					//if( curr_block->own_tbl
					if ( (n=curr_block->memtbl->look(nn->string,0))
					&& n->n_table->real_block==curr_block->memtbl->real_block
					&& n->tp->base!=FCT && n->tp->base!=OVERLOAD
					&& nn->lex_level == n->lex_level ) {
						error("twoDs of%n",n);
						n = 0;
						if (lastnn==0) ss->d=nn->n_list;
						else lastnn->n_list=nn->n_list;
					} else {
						n = nn->dcl(tbl,FCT);
						lastnn=nn;
					}

					if (n == 0) {
						if (ss) {
							ss->base = SM;
							ss->e = 0;
						}
						continue;
					}
//error('d',"hoisted %n to outer blk",n);

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
				  } // for nn
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
done:
	Cstmt = ostmt;
}

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

	if (base != BLOCK && base != DCL) error('i',"block::dcl(%d)",base);

	curr_block = this;
//error('d',"%d->block::dcl(%d) base %k",this,tbl,base);
//error('d',"    memtbl %d  own_tbl %d  d %n  s %k",memtbl,own_tbl,d,s?s->base:0);
	if (d) {
		own_tbl = 1;
		base = BLOCK;
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
	else if ( base == BLOCK ) {
		own_tbl = 1;
		if (memtbl == 0) {
			int nmem = 4;
			memtbl = new table(nmem,tbl,0);
			memtbl->real_block = this;
			/*	this is a "real" block from the
				source text, and not one created by DCL's
				inside a block. */
		} else
			if (memtbl != tbl) error('i',"block::dcl(?)");
	} else {
		base = BLOCK;
		memtbl = tbl;
	}

	Pname odcl = Cdcl;
	if (s) s->dcl();
	if (own_tbl) {
		Pname m;
		int i;
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
			//case VEC:
				continue;
			}

			if (m->n_addr_taken == 0) {
				if (m->n_used) {
					if (m->n_assigned_to) {
					}
					else if ( t->base != VEC ) {
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
	}
	Cdcl = odcl;

	d = 0;

	if (bit_offset) byte_offset += SZ_WORD;
	bit_offset = bit_old;
	byte_offset = byte_old;
	curr_block = block_old;
}

void name::field_align()
/*
	 adjust alignment
*/
{
	Pbase fld = (Pbase)tp;
	int nbits = fld->b_bits;

	int a = (F_SENSITIVE) ? fld->b_fieldtype->align() : SZ_WORD;
	if (max_align < a) max_align = a;

	if (nbits == 0) {		// force word alignment
		int b;
		if (bit_offset)
			nbits = BI_IN_WORD - bit_offset;
		else if (b = byte_offset%SZ_WORD)
			nbits = b * BI_IN_BYTE;
	//	else
	//		nbits = BI_IN_WORD;
		if (max_align < SZ_WORD) max_align = SZ_WORD;
	}
	else if (bit_offset == 0) {	// take care of part of word
		int b = byte_offset%SZ_WORD;
		if (b) {
			byte_offset -= b;
			bit_offset = b*BI_IN_BYTE;
		}
	}
//error('d',"byteoff %d bitoff %d bits %d",byte_offset,bit_offset,nbits);
	int x = (bit_offset += nbits);
	if (BI_IN_WORD < x) {
		fld->b_offset = 0;
		byte_offset += SZ_WORD;
		bit_offset = nbits;
	}
	else {
		fld->b_offset = bit_offset;
		if (BI_IN_WORD == x) {
			bit_offset = 0;
			byte_offset += SZ_WORD;
		}
		else
			bit_offset = x;
	}
	n_offset = byte_offset;
}
