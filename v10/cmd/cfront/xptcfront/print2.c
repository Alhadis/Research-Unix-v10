/*ident	"@(#)ctrans:src/print2.c	1.4" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

print2.c:

	print names and declarations

****************************************************************************/

#include "cfront.h"
#include "template.h"

bit Cast;
int last_ll = 1;
Pin curr_icall;
char emode;
int ntok;

static int MAIN;	// fudge to get _main() called by main()

#define eprint(e) if (e) Eprint(e)

#ifdef DENSE
void chop(char*);
#endif

void puttok(TOK t)
/*
	print the output representation of "t"
*/
{
//	if (t<=0 || MAXTOK<t) error("illegal token %d",t);
//	char* s = keys[t];
//	if (s == 0) error("V representation token %d",t);

	putstring(keys[t]);

	if (12<ntok++) {
		ntok = 0;
		last_line.putline();
	}
	else if (t == SM) {
		ntok = 0;
		putch('\n');
		if (last_ll) last_line.line++;
	}
	else
		putch(' ');
}

#define MX	20
#define NTBUF	10
class dcl_buf {
	/*
		buffer for assembling declaration (or cast)
		left contains CONST_PTR	=> *CONST
			     CONST_RPTR => &CONST
				PTR	=> *
				RPTR	=> &
				LP	=> (
		right contains	RP	=> )
				VEC	=> [ rnode ]
				FCT	=> ( rnode )
				FIELD	=> : rnode
	*/
	Pbase b;
	Pname n;
	TOK left[MX], right[MX];
	Pnode	rnode[MX];
	Pclass	lnode[MX];
	int li, ri;
public:
	void	init(Pname nn)		{ b=0; n=nn; li=ri=0; }
	void	base(Pbase bb)		{ b = bb; }
	void	front(TOK t)		{ left[++li] = t; }
	void	front(Pclass c)		{ left[++li] = MEMPTR; lnode[li] = c; }
	void	back(TOK t, Pnode nod)	{ right[++ri] = t; rnode[ri] = nod; }
	void	paran() 		{ front(LP); back(RP,0); }
	void	put();
} *tbufvec[NTBUF] = {0}, *tbuf = 0;

int freetbuf = 0;

void dcl_buf::put()
{
	int i;
	Pfct ff = 0;

	if (MX<=li || MX<=ri) error('i',"T buffer overflow");
	if (b == 0) error('i',"noBT%s",Cast?" in cast":"");

	if (n && n->n_sto && n->n_sto!=REGISTER) puttok(n->n_sto);

	b->dcl_print();
	
	for( ; li; li--) {
		switch (left[li]) {
		case LP:
			putch('(');
			break;
		case PTR:
			putch('*');
			break;
		case RPTR:
			if (emode)
				putch('&');
			else
				putch('*');
			break;
		case CONST_PTR:
			if (emode)
				putstring("*const ");
			else
				putch('*');
			break;
		case CONST_RPTR:
			if (emode)
				putstring("&const ");
			else
				putch('*');
			break;
		case MEMPTR:
			if (lnode[li]) fprintf(out_file,"%s::",lnode[li]->string);
		}
	}

	if (n) n->print();

	for(i=1; i<=ri; i++) {
		switch (right[i]) {
		case RP:
			putch(')');
			break;
		case VEC:
			putch('[');
			{	Pvec v = (Pvec) rnode[i];
				Pexpr d = v->dim;
				int s = v->size;
				if (d) d->print();
				if (s) fprintf(out_file,"%d",s);
			}
			putch(']');
			break;
		case FCT:	// beware of function returning pointer to
				// function expressed witout typedef
		{	Pfct f = Pfct(rnode[i]);
			if (f->body) ff = f;
			f->dcl_print();
			break;
		}
		case FIELD:
                {       Pbase f = (Pbase) rnode[i];
                        Pexpr d = (Pexpr)f->b_name;
                        int s = f->b_bits;
                        putch(':');
                        if (d)
                                d->print();
                        else if (s)
                                fprintf(out_file,"%d",s);
                        else
                                puttok(ZERO);
                        break;
                }
		}
	}
void print_body(Pfct);
	if (ff && emode==0) print_body(ff);
}

static Pexpr mk_zero_init( Ptype tt, Pname obname, Pname currname )
/*
	creates 0 initializer for defined objects
	side effect: generates right nesting level for {}
*/
{
zaq:
	switch (tt->base) {
	case TYPE:
		tt = Pbase(tt)->b_name->tp; goto zaq;
	case VEC: // type a[size]; => type a[size] = {0};
	{
		if ( obname == currname ) {
		//xxx initializing vectors blows up the size of some programs
			return 0;
		} else {
			Pexpr i = mk_zero_init(Pvec(tt)->typ,obname,currname);
			if ( i == 0 ) return 0;
			return new expr( ILIST, i, 0 );
		}
	}
	case COBJ: // "X a;" == "X a = {0};"
	{
		Pclass cl = Pclass(Pbase(tt)->b_name->tp);
		if ( !ansi_opt && (cl->csu == ANON || cl->csu == UNION) ) {
		    if ( warning_opt ) {
			// ridiculous handstands to avoid printing
			// compiler generated names in warning
			Ptype tn = obname->tp;
			for(;;) if(tn->base==VEC) tn=Pvec(tn)->typ; else break;
			if ( obname == currname && tt == tn ) {
			    if ( cl->string[0]=='_' && cl->string[1]=='_' )
				error('w',&obname->where,"cannot generate zeroIr for%n ofT union; toI, compile with +a1 or insert non-union object as first member",obname);
			    else
				error('w',&obname->where,"cannot generate zeroIr for%n ofT%t; toI, compile with +a1 or insert non-union object as first member",obname,obname->tp);
			} else { Pclass cx;
			    if ( tn->base == COBJ
			    &&   (cx=(Pclass)Pbase(tn)->b_name->tp,
				  cx->string[0]=='_' && cx->string[1]=='_') )
				error('w',&obname->where,"cannot generate zeroIr for%n (union or aggregate with union as first element); toI, compile with +a1 or insert non-union object as first member",obname);
			    else
				error('w',&obname->where,"cannot generate zeroIr for%n ofT%t (union or aggregate with union as first element); toI, compile with +a1 or insert non-union object as first member",obname,obname->tp);
			}
		    } // if warning_opt
		    return 0;
		}
		Pbcl l = cl->baselist;
		while ( l ) {
			if ( l->base == NAME ) cl = l->bclass;
			else return new expr(ILIST,zero,0);
			l = cl->baselist;
		}
		int i = 1;
		Pname nn = cl->memtbl->get_mem(i);
		for ( ;  nn;  nn = cl->memtbl->get_mem(++i) ) {
			Ptype tx = nn->tp;
			while ( tx->base == TYPE ) tx = Pbase(tx)->b_name->tp;
			if (nn->base==NAME
			&& nn->n_union==0
			&& tx->base!=FCT
			&& tx->base!=OVERLOAD
			&& tx->base!=CLASS
			&& tx->base!=ENUM
			&& nn->n_stclass != STATIC) {
				Pexpr i = mk_zero_init(tx,obname,nn);
				if ( i == 0 ) return 0;
				return new expr(ILIST,i,0);
			}
		}
		return 0;
	}
	case PTR:
		if (tt->memptr()) {
			Pexpr i = new expr(ELIST,zero,zero);
			return new expr(ILIST,i,zero);
		}
		// no break
	case RPTR:
	case CHAR:
	case SHORT:
	case INT:
	case EOBJ:
	case LONG:
	case FLOAT:
	case DOUBLE:
	case LDOUBLE:	// "int a;" == "int a = 0;"
	case FIELD:
	case ANY:
		return zero;
	}
	return 0;
}

void name::dcl_print(TOK list)
/*
	Print the declaration for a name (list==0) or a name list (list!=0):
		For each name
		(1) print storage class
		(2) print base type
		(3) print the name with its declarators
	Avoid (illegal) repetition of basetypes which are class or enum declarations
	(A name list may contain names with different base types)
	list == SM :	terminator SM
	list == 0:	single declaration with terminator SM
	list == CM :	separator CM
*/
{
	if (error_count) return;

	for (Pname n=this; n; n=n->n_list) {
		Ptype t = n->tp;
		int sm = 0;

// error('d',"%s->dcl_print() tp %t sto %k",n->string,t,n->n_sto);
		if (t == 0) error('i',"N::dcl_print(%n)T missing",n);

		if (n->n_stclass==ENUM) if (list) continue; else return;

		if (n->where.line!=last_line.line || n->where.file!=last_line.file)
//fprintf(stderr,"%s ",n->string?n->string:"<0>"); n->where.put(stderr); fprintf(stderr,"  last "); last_line.put(stderr); putc('\n',stderr);
			if (last_ll = n->where.line)
				n->where.putline();
			else
				last_line.putline();
	
		int tc = Pbase(t)->b_const;
		for (Ptype tt = t; tt->base==TYPE; tt = Pbase(tt)->b_name->tp)
			tc |= Pbase(tt)->b_const;
 
		switch (t->base) {
		case CLASS:
//fprintf(stderr,"class %s->dcl_print()\n",n->string);
			if (n->base != TNAME) {
				Pclass(t)->dcl_print(n);
				sm = 1;
			}
			break;

		case ENUM:
                        Penum(t)->dcl_print(0);
			sm = 1;
			break;

		case FCT:
		{	Pfct f = Pfct(t);

			if (n->base == TNAME) puttok(TYPEDEF);

// error('d',"fct %n->dcl_print() printed %d body %d defined %d",n,n->n_dcl_printed,f->body,f->defined);
// error('d',"n %d tbl %d tp %t inline %d",n,n->n_table,n->tp,f->f_inline);
			if (n->n_dcl_printed==2	// definition already printed
			|| (n->n_dcl_printed==1 && f->body==0)
						// declaration already printed
			) {
				// don't print again
				sm = 1;	// no SM
				break;
			}

			if (f->f_result == 0) make_res(f);

			if (f->body && n->n_sto==EXTERN) n->n_sto = 0;

			if (f->f_inline && ((n->n_table!=gtbl) || strcmp(n->string,"main"))) {
				if (debug_opt)  {
//error('d',"f %t defined %d inline %d",f,f->defined,f->f_inline);
					if (f->defined&DEFINED
					&& f->defined&SIMPLIFIED
					&& f->f_inline!=ITOR)
						goto prnt_def;
					else if (n->n_dcl_printed==0)
						goto prnt_dcl;
					else {
						sm = 1;
						break;
					}
				}
				if (f->f_virtual || n->n_addr_taken) {
				prnt_dcl:
//error('d',"prnt_dcl %d %n %k",n,n,n->n_sto);
					TOK st = n->n_sto;
					Pblock b = f->body;
					f->body = 0;
					t->dcl_print(n);
					n->n_dcl_printed = 1;
					n->n_sto = st;
					f->body = b;
					break;
				}
				else
					sm = 1;	// no SM
			}
			else if ((f->defined&DEFINED)==0
				|| (f->defined&SIMPLIFIED)==0)
				goto prnt_dcl;
			else if (n->n_table==gtbl && strcmp(n->string,"main")==0) {
				if (f->f_inline) {
					if (warning_opt) {
						error('w',"main() cannot be inlined");
						error('w',"out-of-line copy of main() created");
					}
					n->n_sto=0;
				}
				MAIN = 1;
				gtbl->look("main",0)->use();
				f->f_signature = 0;
				t->dcl_print(n);
				n->n_dcl_printed = f->body?2:1;
				MAIN = 0;
			}
			else {
			prnt_def:
// error('d',"prnt_def %n %k %d %k",n,n->n_oper,n,n->n_sto);
				if (n->n_oper==CTOR || n->n_oper==DTOR) {
					Pclass cl = Pclass(n->n_table->t_name->tp);
					if (cl->c_body == 3) cl->print_all_vtbls(cl);
				}

				if ( n->n_sto == STATIC && 
					pdef_name == 0 && def_name == 0 ) {
// error('d', "printing ptbl_vec without first definition" );
						pdef_name = n;
						ptbl_init(0);
				}
				t->dcl_print(n);
				n->n_dcl_printed = f->body?2:1;
			}
			if (f->body) sm = 1;
			break;
		}

		case OVERLOAD:
		{
			for (Plist gl=Pgen(t)->fct_list; gl; gl=gl->l) {
				Pname nn = gl->f;
				nn->dcl_print(0);
				sm = 1;
			}
			break;
		}

		case ASM:
			fprintf(out_file,"asm(\"%s\")\n",(char*)Pbase(t)->b_name);
			break;

		case INT:
		case EOBJ:
		case CHAR:
		case LONG:
		case SHORT:
		tcx:
			// do not allocate space for constants unless necessary
			if (tc
			&& n->n_sto!=EXTERN	// extern const one;
						// const one = 1;
						// allocates storage
			&& (n->n_scope==EXTERN	// FUDGE const one = 1;
						// is treated as static
						// need loader support
				|| n->n_scope==STATIC
				|| n->n_scope==FCT)
			) {
				if (n->n_evaluated && n->n_addr_taken==0) {
					sm = 1;	/* no ; */
					break;
				}
			}
			tc = 0;
			// no break;

		default:
		{
/*	
//  don't print local instance of const
			if ( n->n_dcl_printed == 3 ) {
				sm = 1;
				break;
			}
*/
			Pexpr i = n->n_initializer;
 			if (n->base == TNAME && n->n_key == NESTED) i = 0;
			if (tc) {
				switch (tt->base) {
				case CHAR:
				case SHORT:
				case INT:
				case LONG:
				case EOBJ:
					goto tcx;
				}
			}

			if (n->base == TNAME) {
       			/* Always print template formals, even when they have the same 
			 * formal name, since the instantiation name is different. This 
			 * fix should not be required when the copy mechanism is in place. */
          			if (! n_template_arg)
     				    	for (Pname tn=ktbl->look(n->string,HIDDEN); tn; tn=tn->n_tbl_list) 
     						if (tn && tn->lex_level && t==tn->tp) return;
				puttok(TYPEDEF);
			}

			if (n->n_stclass == REGISTER) {
				// (imperfect) check against member functions
				// register s a; a.f() illegal
				Pname cln = n->tp->is_cl_obj();
				if (cln) {
					Pclass cl = Pclass(cln->tp);
					if (cl->csu!=CLASS
					&& cl->baselist==0
					&& cl->has_itor()==0
					&& cl->virt_count==0) puttok(REGISTER);
				}
				else
					puttok(REGISTER);
			}

			if (i) {
				if (n->n_sto==EXTERN && n->n_stclass==STATIC) {
					n->n_initializer = 0;
					t->dcl_print(n);
					puttok(SM);
					n->n_initializer = i;
					n->n_sto = 0;
					t->dcl_print(n);
					n->n_sto = EXTERN;
				}
				else
					t->dcl_print(n);
				if(n->n_table) {
					Pname nn = n->n_table->look(n->string,0);
					if(nn)
						nn->n_dcl_printed = 1;
					else
						n->n_dcl_printed = 1;
				}
				else
					n->n_dcl_printed = 1;
			}
			else if (n->n_evaluated && Pbase(t)->b_const) {
				if (n->n_sto==EXTERN && n->n_stclass==STATIC) {
					int v = n->n_evaluated;
					n->n_evaluated = 0;
					t->dcl_print(n);
					puttok(SM);
					if (n->where.line!=last_line.line || n->where.file!=last_line.file)
						if (last_ll = n->where.line)
							n->where.putline();
						else
							last_line.putline();
					n->n_evaluated = v;
					n->n_sto = 0;
					t->dcl_print(n);
					n->n_sto = EXTERN;
				}
				else
					t->dcl_print(n);
				if(n->n_table) {
					Pname nn = n->n_table->look(n->string,0);
					if(nn)
						nn->n_dcl_printed = 1;
					else
						n->n_dcl_printed = 1;
				}
				else
					n->n_dcl_printed = 1;
			}
			else {
//error('d',"%n sto %k val %d stc %k",n,n->n_sto,n->n_val,n_stclass);
                               if ((n->n_sto==0 || (n->n_val && n->n_evaluated==0))
                                && n_stclass==STATIC
				&& n->n_sto!=STATIC
                                && n->n_table==gtbl) {
                                        if (n->n_val && n->n_evaluated==0) { 
                                                // extern x = f();
                                                // generate int x = 0;       
                                                // plus dynamic initialization
                                                n->n_sto = 0;
                                        }

					n->n_initializer = i = mk_zero_init(t,n,n);
				}
				t->dcl_print(n);
				if(n->n_table) {
					Pname nn = n->n_table->look(n->string,0);
					if(nn)
						nn->n_dcl_printed = 1;
					else
						n->n_dcl_printed = 1;
				}
				else
					n->n_dcl_printed = 1;
			}

			if (n->n_scope!=ARG) {
				if (i) {
					puttok(ASSIGN);

					Pexpr i2 = i;
					while (i2->base == CAST) i2 = i2->e1;
					if (i2->base == ILIST) i = i2;
					if (t!=i->tp
					&& i->base!=ZERO
					&& i->base!=ILIST /*&& i->tp!=Pchar_type*/) {
						Ptype t1 = n->tp;
					cmp:
						switch (t1->base) {
						case TYPE:	
							t1 = Pbase(t1)->b_name->tp;
							goto cmp;
						default:
							i->print();
							break;
					//	case EOBJ:
					//		goto cst;
						case VEC:
							if (Pvec(t1)->typ->base==CHAR) {
								i->print();
								break;
							}
							// no break
						case PTR:
						case RPTR:
							if (i->tp==0 || n->tp->check(i->tp,0)) {
                                          //      cst:
								putch('(');
								bit oc = Cast;
								Cast = 1;
								t->print();
								Cast = oc;
								putch(')');
							}
							eprint(i);
						}
					}
					else {
						if (i==zero) {
							while (t->base == TYPE) t = Pbase(t)->b_name->tp;
					//		if (t->base == EOBJ) {
					//			putch('(');
					//			bit oc = Cast;
					//			Cast = 1;
					//			t->print();
					//			Cast = oc;
					//			putch(')');
					//		}
						}
						eprint(i);
						
					//	i->print();
					}
				}
				else if (n->n_evaluated) {
					puttok(ASSIGN);
					if (n->tp->base!=INT || n->tp->is_unsigned()) {
						putstring("((");
						bit oc = Cast;
						Cast = 1;
						n->tp->print();
						Cast = oc;
						fprintf(out_file,")%d)",n->n_val);
					}
					else
						fprintf(out_file,"%d",n->n_val);
				}
			}
		}
		}

		switch (list) {
		case SM:
			if (sm==0) puttok(SM);
			break;
		case 0:
			if (sm==0) puttok(SM);
			return;
		case CM:
			if (n->n_list) puttok(CM);
			break;
		}
	}
} 

char *local_sign( Ptype pt )
{ // get function signature for local class
	char buf[1024];
	char* bb = pt->signature(buf);
	int ll = bb-buf;
	if (1023 < ll) error('i',"local class N buffer overflow");
	char *p = new char[ll+1];
	strcpy(p,buf);
	return p;
}

void enumdef::dcl_print(Pname cln)
/*
*/
{
	// note: ***** modify to handle local enums 
// error('d', "%t::dcl_print(%n) in_class: %t nested_sig: %s", this, cln, in_class, nested_sig );
	char* s = nested_sig?nested_sig:(cln ? cln->string:0);
	if ( nested_sig )
		fprintf(out_file,"enum __%s { ",nested_sig);
	else fprintf(out_file,"enum %s { ",string);

	for (Pname px, p=mem; p; p=px) {
		px = p->n_list;
		if (s) {
			if (p->n_evaluated)
				fprintf(out_file,"%s__%s = %d",p->string,s,p->n_val);
			else
				fprintf(out_file,"%s__%s",p->string,s);
		}
		else {
			if (p->n_evaluated)
				fprintf(out_file,"%s = %d",p->string,p->n_val);
			else
				fprintf(out_file,"%s",p->string);
		}
		if (px) puttok(CM);
		p->n_initializer = 0;
		delete p;
	}
	mem = 0;
	puttok(RC);
	puttok(SM);
}

void name::print()
{ // print just the name itself
	if (this == 0) error('i',"0->N::print()");
	if (string == 0) {
		if (emode) putch('?');
		return;
	}

// error( 'd', "%s->name::print(), base: %k", string, base );
	switch (base) {
	case TNAME:
		if (n_key == NESTED && tpdef &&
			tpdef->nested_sig) {
			if ( emode == 0 ) {
				putstring( " __" );
				putst(tpdef->nested_sig);
			}
			else {
				putst(tpdef->in_class->string);
				putstring( "::" );
				putst(string);
			}
			return;
		}

		if (emode && tp) {
			if (n_template_arg_string) {
				tp->dcl_print(0); return;
			} else
			if(tp->base == COBJ) {
				Pclass cl = Pclass(Pbase(tp)->b_name->tp);
				if(cl && cl->base == CLASS && 
				  (cl->class_base == instantiated_template_class)) {
					Ptclass(cl)->inst->print_pretty_name(); return;
				}
			}
		}

		putst(n_template_arg_string ? n_template_arg_string : string);
		return;

	case MDOT:
		Pexpr(this)->print();
		return;
	}

	if (emode) {
		Ptable tbl;
		char* cs = 0;
		bit f = 0;
		if (tp) {
			switch (tp->base) {
			case OVERLOAD:
			case FCT:
				f = 1;
			default:
				if (tbl=n_table) {
					if (tbl == gtbl) {
						if (f == 0) putstring("::");
					}
					else {
						if (tbl->t_name) {
							Ptclass pc = Ptclass(tbl->t_name->tp);
							cs = tbl->t_name->string;
							if ((pc->base == CLASS)  && 
			    					(pc->class_base == instantiated_template_class))
			  						pc->inst->print_pretty_name();
							else fprintf(out_file,"%s",cs);
							fprintf (out_file, "::") ;
						}
					}
				}

				if (n_scope==ARG && strcmp(string,"this")==0) {
					// tell which "this" it is
					Ptype tt = Pptr(tp)->typ;
					Pname cn = Pbase(tt)->b_name;
					fprintf(out_file,"%s::",cn->string);
				}

			case CLASS:
			case ENUM:
		//	case TYPE:
				break;
			}
		nop:
			switch (n_oper) {
			case TYPE:
				putstring("operator ");
				if (tp)	Pfct(tp)->returns->dcl_print(0);
				break;
			case 0:
				putstring(string);
				break;
			case DTOR:
				putch('~');
			case CTOR:
				if (cs) {
		    			if(tbl->t_name) {
						Ptclass pc = Ptclass(tbl->t_name->tp);
						if ((pc->base == CLASS) && 
			    				(pc->class_base == instantiated_template_class))
			    					pc->inst->print_pretty_name();
						else putstring(cs);
					}
				}
				else {
					if (string)
						fprintf(out_file,"%s()", n_template_arg_string 
							? n_template_arg_string : string);
					else putstring("constructor");
					f = 0;
				}
				break;
			case TNAME:
				putstring(string);
				break;
			default:
				putstring("operator ");
				putstring(keys[n_oper]);
				break;
			}
			if (f) putstring("()");
		}
		else {
			if (n_oper) goto nop;
			if (string) 
 				putstring(n_template_arg_string 
					? n_template_arg_string : string);
		}
		return;
	}
	
	char* sig = 0;
	Pclass cl = 0;
	Penum en = 0;
	int i = n_union;

	if (tp) {
		Ptable tbl;

		switch (tp->base) {
		default:
			if (tbl=n_table) {	// global or member
				Pname tn;
				if (tbl == gtbl) {
					// if (i) fprintf(out_file,"__O%d.",i);
					if ( i ) {
						if (n_anon) 
    						fprintf(out_file,"__O%d.%s.", i, n_anon );
					else fprintf(out_file,"__O%d.",i);
					}
					break;
				}

				if (tn=tbl->t_name) { 
					cl = Pclass(tn->tp);
					if (i) { 
						if (cl->string[0]=='_'	
						    && cl->string[1]=='_'
						    && cl->string[2]=='C' ) {
							if (n_anon)
    						    		fprintf(out_file,"__O%d.%s.", i, n_anon );
							else fprintf(out_file,"__O%d.",i);
						}
						else
						if ( cl->nested_sig ) 
							fprintf(out_file,"__O%d__%s.",i,cl->nested_sig);
						else
						if ( cl->lex_level ) {
							char *str = make_local_name(cl,1);
							fprintf(out_file,"__O%d%s.",i,str);
							delete str;
						}
						else fprintf(out_file,"__O%d__%d%s.",i,cl->strlen,cl->string);
						cl = 0;
					}
					else 
					if (cl->string[0]=='_'
						&& cl->string[1]=='_'
						&& cl->string[2]=='C'
						&& n_stclass != STATIC )
							cl = 0;
					break;
				}
			}

			switch (n_stclass) {	// local variable
			case STATIC:
			case EXTERN:
				if (i)
					fprintf(out_file,"__O%d.",i);
				else if (n_sto==STATIC && tp->base!=FCT) {
					if (lex_level == 0)
						putstring("__S");
					else
						fprintf(out_file,"__%d",lex_level);
				}
				break;
			default:
				// encode with lexical level UNLESS ``special''
				// e.g. __builtin
				if (string[0]!='_' || string[1]!='_' || string[2] != 'C' ) {
					if (i)
					{
						if (n_anon)
						    fprintf(out_file,"__%d__O%d.%s.",lex_level-1,i,n_anon);
						else fprintf(out_file,"__%d__O%d.",lex_level-1,i);
					}
					else
						fprintf(out_file,"__%d",lex_level);
				}
			}
			break;
		case CLASS:
		case ENUM:
			if (tp->in_class && tp->nested_sig) en = Penum(tp);
			break;
		}

		if (tp->base==FCT) {
			sig = Pfct(tp)->f_signature;
			if (sig && sig[0]==0) sig = 0;
		}
	}

	if (string) {
#ifdef DENSE
		int i = strlen(string);
		if (cl) i += cl->strlen+4;	// __dd<class name>
		if (sig) {
			if (cl == 0) i += 2;
			i += strlen(sig);
		}

		if (31<i) {
			char buf[256];
			if (cl && sig)
				sprintf(buf,"%s__%d%s%s",string,cl->strlen,cl->string,sig);
			else if (cl)
				sprintf(buf,"%s__%d%s",string,cl->strlen,cl->string);
			else if (sig)
				sprintf(buf,"%s__%s",string,sig);
			else 
				sprintf(buf,"%s",string);
			chop(buf);
			fprintf(out_file,"%s ",buf);
			return;
		}
#endif

		if ( en )
    			fprintf(out_file,"__%s", en->nested_sig);
		else 
        		putstring(n_template_arg_string 
				? n_template_arg_string : string);

		if ( cl ) {
			if ( cl->nested_sig )
		        	fprintf(out_file,"__%s",cl->nested_sig);
			else
			if ( cl->lex_level ) {
				char *str = make_local_name( cl, 1 );
				putstring( str );
				delete str;
			}
		        else fprintf(out_file,"__%d%s",cl->strlen,cl->string);
		}

		if (sig) {
			if (cl == 0) putstring("__");
			putstring(sig);
		}
		putch(' ');
	}
}

#ifdef DENSE
void chop(char* buf)
{
	static char alpha[] = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const asz = sizeof(alpha)-1;
	int hash = 0;
	char* p = &buf[29];
			
	while (*p) {
		hash <<= 1;
		if (hash & (1<<12)) {
			hash &= ~(1<<12);
			hash++;
		}
		hash ^= *p++;
	}

	buf[29] = alpha[(int)(hash%asz)];
	buf[30] = alpha[(int)((hash/asz)%asz)];
	buf[31] = 0;
}
#endif

void type::print()
{
	switch (base) {
	case PTR:
	case RPTR:
	case VEC:
		Pptr(this)->dcl_print(0);
		break;
	case FCT:
		Pfct(this)->dcl_print();
		break;
//	case VEC:
//		Pvec(this)->dcl_print(0);
//		break;
	case CLASS:
	case ENUM:
		if (emode)
			fprintf(out_file,"%k",base);
		else
		//	error('i',"%p->T::print(%k %s)",this,base,Pclass(this)->string);	
			fprintf(out_file,"struct %s *",Pclass(this)->string);
		break;
	case TYPE:
		if (Cast||Pbase(this)->b_name->lex_level) {
			Pbase(this)->b_name->tp->print();
			break;
		}
		// no break
	default:
		Pbase(this)->dcl_print();
	}
}

char* type::signature(register char* p)
/*
	take a signature suitable for argument types for overloaded
	function names
*/
{
	Ptype t = this;
	int pp = 0;	// pointer to

xx:
//error('d',"xx(%d) %d %k",this,t,t->base);

	// first unroll typedefs and handle derived types:

	switch (t->base) {
	case TYPE:
		if (Pbase(t)->b_const) *p++ = 'C';
		t = Pbase(t)->b_name->tp;
		goto xx;

	case VEC:
               if (pp && Pvec(t)->size) {           // A<size>_
			*p++ = 'A';
			sprintf(p,"%d\0",Pvec(t)->size); // don't trust
							 // sprintf return value
			while (*++p);
			*p++ = '_';
		}
		else
			*p++ = 'P';
		t = Pvec(t)->typ;
		pp = 1;
		goto xx;

	case PTR:
		if (Pptr(t)->rdo) *p++ = 'C';		// *const
		if (Pptr(t)->memof) {			// M<size><classname>
			Pclass cl = Pptr(t)->memof;
			register char* s = cl->string;
			int d = cl->strlen;
			if (d==0) cl->strlen = d = strlen(s);
	 		*p++ = 'M';
			if (d/10) *p++ = '0'+d/10;
			*p++ = '0'+ d%10;	// assume <100 char
			while (*p++ = *s++);
			--p;				// not the '\0'
		}
	 	else
			*p++ = 'P';
		t = Pptr(t)->typ;
		pp = 1;
		goto xx;

	case RPTR:
		*p++ = 'R';
		t = Pptr(t)->typ;
		pp = 1;
		goto xx;

	case FCT:
	{	Pfct f = Pfct(t);
		Pname n = f->argtype;

		if (f->f_const) *p++ = 'C';	// constant member function
		if (f->f_static) *p++ = 'S';	// static member function
	//	if (f->memof && f->f_this==0) *p++ = 'S';	//SSS static member function
		*p++ = 'F';
		if (n == 0)
			*p++ = 'v';	// VOID, that is f() == f(void)
		else
	for ( ; n; n=n->n_list) {	// print argument encoding
					// check if argtype is the same
					// as previously seen argtype
		int i = 0;
		for (Pname nn=f->argtype; n!=nn; nn=nn->n_list) {
			i++;
			if (nn->tp==n->tp || nn->tp->check(n->tp,0)==0) {
				// typeof (n) == typeof(arg i)
				int x = 1;	// try for a run after n
				Pname nnn = n;
				while ((nnn=nnn->n_list) && x<9) {
					if (nnn->tp==n->tp
					|| nnn->tp->check(n->tp,0)==0) {
						x++;
						n = nnn;
					}
					else
						break;
				}

				if (x == 1)		// Ti
					*p++ = 'T';
				else {			// Nxi
					*p++ = 'N';
					*p++ = '0'+x;
				}
	
				// assume <100 arguments
				if (9<i) *p++ = '0'+i/10;
				*p++ = '0'+i%10;
				goto zk;
			}	
		}

		// ``normal'' case print argument type signature
	//	if (n->n_xref) *p++ = 'R';
		p = n->tp->signature(p);
		zk:;
	}

		if (f->nargs_known == ELLIPSIS) *p++ = 'e';

		if (pp) {		// '_' result type
			*p++ = '_';
			p = f->returns->signature(p);
		}

		*p = 0;
		return p;
	}
	}

	// base type modifiers:

	if ( Pbase(t)->b_const )	*p++ = 'C';
//	if ( Pbase(t)->b_signed )	*p++ = 'S';
	if ( Pbase(t)->b_unsigned )	*p++ = 'U';
//	if ( Pbase(t)->b_volatile )	*p++ = 'V';


	// now base types:

	register char *s, *ns = 0;;
	int d;
	Pclass cl;
//lll:
	switch (t->base) {
//	case TNAME:	t = Pbase(t)->b_name->tp; goto lll;
	case ANY:			break;
	case ZTYPE:			break;
	case VOID:	*p++ = 'v';	break;
	case CHAR:	*p++ = 'c';	break;
	case SHORT:	*p++ = 's';	break;
//	case EOBJ:
	case INT:	*p++ = 'i';	break;
	case LONG:	*p++ = 'l';	break;
	case FLOAT:	*p++ = 'f';	break;
	case DOUBLE:	*p++ = 'd';	break;
	case LDOUBLE:	*p++ = 'r';	break;
	case EOBJ:
//		*p++ = 'i';
//		break;
	{	Penum en = Penum(Pbase(t)->b_name->tp);
	//	t = en->e_type;
	//	goto lll;
		ns = en->nested_sig; 
		s = en->string;
		d = en->strlen;
		if (d==0) en->strlen = d = strlen(s);
		goto pppp;
	}

	case COBJ:
	{	cl = Pclass(Pbase(t)->b_name->tp);
		ns = cl->nested_sig;
		s = cl->string;
		d = cl->strlen;
		if (d==0) cl->strlen = d = strlen(s);
	pppp:
		if ( ns == 0 ) {
			if (d/10) *p++ = '0'+d/10;
			*p++ = '0'+ d%10;	// assume less that 99 characters
		}
		else s = ns;

		while (*p++ = *s++);
		--p;
		break;
	}
	case FIELD:
	default:
		error('i',"signature of %k",t->base);
	}

	*p = 0;
	return p;
}

void basetype::dcl_print()
{
	Pname nn;
	Pclass cl;

	if (emode) {
		if (b_virtual) puttok(VIRTUAL);
		if (b_inline) puttok(INLINE);
	}
	if(ansi_opt || emode)
		if (b_const) puttok(CONST);
	if (b_unsigned) puttok(UNSIGNED);

	switch (base) {
	case ANY:
		if (emode)
			putstring("any ");
		else
			putstring("int ");
		break;

	case ZTYPE:
		if (emode)
			putstring("zero ");
		else
			putstring("int ");
		break;

	case VOID:
                if (emode==0 && ansi_opt==0) {
                        // silly trick to bypass BSD C compiler bug
                        // void* (*)() dosn't work there
                        // note simpl.c knows that VOID -> CHAR grep for VCVC
                        puttok(CHAR);
			break;
                } 
	case CHAR:
	case SHORT:
	case INT:
	case LONG:
	case FLOAT:
	case DOUBLE:
        case LDOUBLE:
		puttok(base);
		break;

	case EOBJ:
		nn = b_name;
	eob:
		if (emode == 0)
		//	puttok(INT);
			Penum(nn->tp)->e_type->dcl_print();
        	else {
                        puttok(ENUM);
                        nn->print();
		}
                break;

	case COBJ:
		nn = b_name;
	cob:
		cl = Pclass(nn->tp);
		if (emode && (cl->base == CLASS)  &&
			(cl->class_base == instantiated_template_class)) {
				Ptclass(cl)->inst->print_pretty_name();
				break;
		} else
		if (cl && (cl->csu==UNION || cl->csu==ANON))
			puttok(UNION);
		else puttok(STRUCT);
		{ 
			char* s = 0;
			if ( cl && cl->nested_sig )
			       	fprintf(out_file," __%s ",cl->nested_sig);
			else {
				if ( cl && cl->lex_level )
					s = make_local_name( cl );
				putst(s?s:nn->string);
				delete s;
			}
		}
		break;

	case TYPE:
		if (emode == 0) {
			switch (b_name->tp->base) {
			case COBJ:
				nn = Pbase(b_name->tp)->b_name;
				goto cob;
			case EOBJ:
				nn = Pbase(b_name->tp)->b_name;
				goto eob;
			}
		}
		b_name->print();
		break;

	default:
		if (emode) {
			if (0<base && base<=MAXTOK && keys[base])
				fprintf(out_file," %s",keys[base]);
			else
				putch('?');
		}
		else
			error('i',"%p->BT::dcl_print(%d)",this,base);
	}
}
Pbase memptr_type;

void type::dcl_print(Pname n)
/*
	"this" type is the type of "n". Print the declaration
*/
{
//error('d',"%p::dcl_print(%n)",this,n);
	Ptype t = this;
	Pptr p;
	TOK pre = 0;

	if (t == 0) error('i',"0->dcl_print()");
	if (n && n->tp!=t) error('i',"not %n'sT (%p)",n,t);

	if (base == OVERLOAD) {
		for (Plist gl=Pgen(this)->fct_list; gl; gl=gl->l) {
			Pname nn = gl->f;
			nn->tp->dcl_print(nn);
			if (gl->l) puttok(SM);
		}
		return;
	}

	tbuf = tbufvec[freetbuf];
	if (tbuf == 0) {
		if (freetbuf == NTBUF-1) error('i',"AT nesting overflow");
		tbufvec[freetbuf] = tbuf = new class dcl_buf;
	}
	freetbuf++;
	if ( base==FIELD && Pbase(this)->b_bits == 0 )
		tbuf->init(0);// some compilers can't handle mangled names here
	else
		tbuf->init(n);
	if (n && n->n_xref) tbuf->front(PTR);

	while (t) {
		TOK k;

		switch (t->base) {
		case PTR:
			p = Pptr(t);
			k = (p->rdo) ? CONST_PTR : PTR;
			goto ppp;
		case RPTR:
			p = Pptr(t);
			k = (p->rdo) ? CONST_RPTR : RPTR;
		ppp:
			if (p->memof) {
				if (emode) {
					tbuf->front(k);
					tbuf->front(p->memof);
				}
				else {
					t = p->typ;
					while (t->base==TYPE) t = Pbase(t)->b_name->tp;
					if (t->base == FCT) {
						tbuf->base(mptr_type);
						goto zaq;
					}
					else
						tbuf->front(k);
				}
			}
			else
				tbuf->front(k);
			pre = PTR;
			t = p->typ;
			break;
		case VEC:
		{	Pvec v = Pvec(t);
			if (Cast && pre != PTR && pre != VEC) {	// for Macintosh: ptr to array uses [] notation
				tbuf->front(PTR);
				pre = PTR;
			}
			else {
				if (pre == PTR) tbuf->paran();
				tbuf->back(VEC,v);
				pre = VEC;
			}
			t = v->typ;
			break;
		}

		case FCT:
		{	Pfct f = Pfct(t);
			if (pre == PTR)
				tbuf->paran();
			else if (emode && f->memof && n==0)
				tbuf->front(f->memof);
			tbuf->back(FCT,f);
			pre = FCT;
			t = (f->s_returns) ? f->s_returns : f->returns;
			break;
		}
		case FIELD:
			tbuf->back(FIELD,t);
			tbuf->base( Pbase(Pbase(t)->b_fieldtype) );
			t = 0;
			break;
		case 0:
			if(!emode)error('i',"noBT(B=0)");
			goto dobase;
		case TYPE:
			if (Cast||Pbase(t)->b_name->lex_level) { // unravel type in case it contains vectors
				t = Pbase(t)->b_name->tp;
				break;
			}
		default: // the base has been reached
		dobase:
			if (emode) {
				char* s;
				for (Ptype tt = t; tt->base==TYPE; tt=Pbase(tt)->b_name->tp);
				switch (tt->base) {
				case CLASS:
					s = Pclass(tt)->string;
                                        if (Ptclass(this)->class_base == instantiated_template_class ||
                                            Ptclass(this)->class_base == uninstantiated_template_class) {
                                     		Ptclass(this)->inst->print_pretty_name();
                                        	--freetbuf;
                                        	return;
                                        }
					if (s[0]=='_' &&s[1]=='_' && s[2]=='C') s="class";
					goto fret;
				case ENUM:
					s = "enum";
					goto fret;
				case OVERLOAD:
					s = "overloaded";
				fret:
					putstring(s);
					freetbuf--;
					return;
				}
			}

			tbuf->base( Pbase(t) );
			goto zaq;
		} // switch
	} // while
zaq:
	tbuf->put();
	freetbuf--;
}

void fct::dcl_print()
{
	Pname nn;
//error('d',"fct::dcl_print()");
	if (emode) {
		putch('(');
		for (nn=argtype; nn;) {
			nn->tp->dcl_print(0);
			if (nn=nn->n_list) puttok(CM); else break;
		}
		switch (nargs_known) {
		case 0:		//	putst("?"); break;
		case ELLIPSIS:	puttok(ELLIPSIS); break;
		}
		putch(')');
		if (f_const) puttok(CONST);	
		if (f_static) puttok(STATIC);	// wrong place for ``static''
		return;
	}

	Pname at = f_args;
	putch('(');

	if (ansi_opt) {
		// print typed arguments:
		at = (f_this) ? f_this : (f_result) ? f_result : argtype; 
		// WNG -- note:  at = f_args had 0 value with ansi_opt set
		//               mystery fix added here
		if (at == 0) {
			if (nargs_known == ELLIPSIS) {
				putch(')');
				return;
			}
			puttok(VOID);
		}
		else if (body && Cast==0)
			at->dcl_print(CM);	// print argument type and name
		else {
			for (nn=at; nn;) {
			//	nn->tp->dcl_print(0);	// print argument type
				nn->tp->dcl_print(nn);	// print argument type
							// (there may not be a name)
				if (nn=nn->n_list) puttok(CM); else break;
			}
		}
		if (nargs_known == ELLIPSIS) putstring(",...");
		putch(')');
	}
	else {
		// print argument names followed by argument type declarations:
		if (body && Cast==0) {
			for (nn=at; nn;) {
				nn->print();
				if (nn=nn->n_list) puttok(CM); else break;
			}
#ifdef mips
			if (nargs_known == ELLIPSIS)
				putstring(", va_alist");
#endif
#ifdef sparc
                        if (nargs_known == ELLIPSIS)
                                putstring(", __builtin_va_alist");
#endif
			putch(')');
		}
		else
			putch(')');
	}
}

void print_body(Pfct f)
{
	if (Cast==0) {
		
		if (ansi_opt==0 && f->f_args) {
			f->f_args->dcl_print(SM);
			if ( last_ll==0 && (last_ll = f->body->where.line) )
				f->body->where.putline();
		}

		if (MAIN) {
			putstring("{ _main(); ");	// call constructors
			f->body->print();
			puttok(RC);
		}
		else 
			f->body->print();
	}
}

Pbcl shared_seen;

void classdef::print_members()
{
	int i;
	
	Pbcl l = baselist;
// error('d',"%t->print_members()",this);
	if (l) {
		if (l->base == NAME) {
			l->bclass->print_members();	// first base only
			// pad to ensure alignment:
			int boff = l->bclass->real_size;
			int ba = l->bclass->align();
			int xtra = boff%ba;
			int waste = (xtra) ? ba-xtra : 0;	// padding
//error('d',"%s: size % align %d waste %d",string,boff,ba,waste);
			if (waste) {
				// waste it to protect against structure
				// assignments to the base class
				char* s = make_name('W');
				fprintf(out_file,"char %s[%d];\n",s,waste);
				delete s;
			}
			l = l->next;
		}

		for (; l; l=l->next)
		/*	for second base etc. one must allocate as an object
			(rather than a list of members) to ensure proper alignment
			for shared base allocate a pointer
			size, alignment, & offset handled in cassdef::dcl()
		*/
			if (l->base == NAME) {
				Pclass bcl = l->bclass;
        			char *str =  0;
				char *cs = bcl->nested_sig?bcl->nested_sig:bcl->string;
				if (bcl->lex_level && 
					bcl->nested_sig==0) 
						str=make_local_name(bcl);
				puttok(STRUCT);
				putst(str?str:cs);
				putcat('O',bcl->string); // leave as unencoded
				puttok(SM);
				delete str;
			}
	}

    	// Sam: A class or an enum type declared within a class can hide a
    	// member with the same name, so make sure that it gets printed by
    	// traversing the n_tbl_list to get at these names.
    	for (Pname nn=memtbl->get_mem(i=1); nn; nn=memtbl->get_mem(++i)) {
    	  do { if (nn->base==NAME
		&& nn->n_union==0
		&& nn->tp->base!=FCT
		&& nn->tp->base!=OVERLOAD
		&& nn->tp->base!=CLASS
		&& nn->tp->base!=ENUM
		&& nn->n_stclass != STATIC) {
		//	if (nn->tp->base==FIELD && Pbase(nn->tp)->b_bits==0) continue;
			Pexpr i = nn->n_initializer;
			nn->n_initializer = 0;
			nn->dcl_print(0);
			nn->n_initializer = i;
    		}
    	  } while ((nn->base == NAME) &&
    		 ((nn->tp->base!=CLASS) || (nn->tp->base!=ENUM)) &&
    		 (nn = nn->n_tbl_list)) ;
	}

	for (l=baselist; l; l=l->next)
		if (l->base==VIRTUAL && l->ptr_offset) {
			Pclass bcl = l->bclass;
       			char* str =  0;
			char *cs = bcl->nested_sig?bcl->nested_sig:bcl->string;
			if (bcl->lex_level && 
				bcl->nested_sig==0) 
					str=make_local_name(bcl);
			puttok(STRUCT);
			putst(str?str:cs);
			putch('*');
			putcat('P',bcl->string); // leave unencoded
			puttok(SM);
			delete str;
		}
}

// void classdef::print_vtbl(Pvirt vtab)
// {
// error('d',"%s->print_vtbl(%s) vtbl_opt %d",string,vtab->string,vtbl_opt);
// error('d',"print_vtbl: lex_level: %d", lex_level );
// 
//	switch (vtbl_opt) {
//	case -1:
//	case 1:
// 		vlist = new vl(this,vtab,vlist);
//	}
//
//	int oo = vtbl_opt;	// make `simulated static' name
//	vtbl_opt = -1;
//      char* str =  lex_level ? make_local_name(this) : 0;
//      char* s = vtbl_name(vtab->string,str?str:string);
//	vtbl_opt = oo;
//	fprintf(out_file,"extern struct __mptr %s[];\n",s);
//	s[2] = 'p';	// pointer, not tbl itself
//	fprintf(out_file,"extern struct __mptr* %s;\n",s);
//
//       delete s;
//	delete str;
// }

vl* vlist;

void really_really_print(Pclass cl, Pvirt vtab, char* s, char* ss);

int p2(Pname nn, Ptype t, Pclass cl, Pvirt vtab, char* s)
{
	int init;

	if (t->base == FCT) {
		Pfct f = Pfct(t);

//error('d',"p2 %n init %d inline %d virtual %d",nn,nn->n_initializer,f->f_inline,f->f_virtual);
//error('d',"p2 %s expr %d imeasure %d body %d",s,f->f_expr,f->f_imeasure,f->body);
//error('d',"sto %k",nn->n_sto);
		if (nn->n_initializer
		|| nn->n_sto==STATIC
		|| f->f_inline
		|| f->f_imeasure
		|| f->f_virtual==0) return 0;
		init = f->body!=0;
	}
	else
		init = nn->n_initializer!=0;

	int oo = vtbl_opt;
	vtbl_opt = 1;	// make sure the name is universal
	char *cs = cl->nested_sig?cl->nested_sig:cl->string;
        char* sstr = (cl->lex_level&&cl->nested_sig==0)? make_local_name(cl) : 0;
        char* ss = vtbl_name(vtab->string,sstr?sstr:cs);

	if (init) {	// unique definition here
		really_really_print(cl,vtab,ss,s);
	}
	else {	// unique definition elsewhere
		char *vstr = 0;
		if ( vtab && vtab->string ) {
			vstr = new char[strlen(vtab->string)+cl->strlen+1];
			strcpy( vstr, vtab->string );
			strcat( vstr, cl->string );
		}
		Pname nn;
		if (nn=ptbl->look(vstr?vstr:cl->string,0)) { // use of ptbl in file
        		fprintf(out_file,"extern struct __mptr %s[];\n",ss);
			s[2] = 'p';
        		// fprintf(out_file,"struct __mptr* %s = ",s);
        		// fprintf(out_file,"%s;\n",ss);
			ptbl_add_pair(s, ss);
			nn->n_key=HIDDEN; 
		}
		delete vstr;
	}
	vtbl_opt = oo;

	delete ss;
	delete sstr;

	return 1;
}

void classdef::really_print(Pvirt vtab)
{
// error('d',"really_print %t %d",this,vtbl_opt);
	int oo = vtbl_opt;	// make `simulated static' name
	vtbl_opt = -1;
	char *cs = nested_sig?nested_sig:string;
        char* str = (lex_level&&nested_sig==0) ? make_local_name(this) : 0;
        char* s = vtbl_name(vtab->string,str?str:cs);
// error('d', "really_print: %s, vtab: %s, s: %s", string, vtab->string, s );
	vtbl_opt = oo;

	// see if needed
	int i;
	for (Pname nn=memtbl->get_mem(i=1); nn; nn=memtbl->get_mem(++i) ) {
		Ptype t = nn->tp;
	zse:
		if (t)
		switch (t->base) {
		case TYPE:
			t = Pbase(t)->b_name->tp;
			goto zse;
/*
		case COBJ:
			if (nn->n_sto == EXTERN)
			{	Pclass cl = Pclass(Pbase(t)->b_name->tp);
				if (cl->has_ctor()) {
					p2(nn,t,this,vtab,s);
					return;
				}
			}
			break;
*/
		case FCT:
			if (p2(nn,t,this,vtab,s))
				goto xyzzy;
			break;

		case OVERLOAD:
		{	for (Plist gl=Pgen(t)->fct_list; gl; gl=gl->l)
				if (p2(gl->f,gl->f->tp,this,vtab,s))
					goto xyzzy;
		}
		}
	}

        { // must be initialized var in block to permit goto
	char* ss = vtbl_name(vtab->string,str?str:cs);
// error('d', "really_print: %s, vtab: %s, ss: %s", cs, vtab->string, s );

	if (vtbl_opt)
		really_really_print(this,vtab,ss,s);
	else {
		char *vstr = 0;
		if ( vtab && vtab->string ) {
			vstr = new char[::strlen(vtab->string)+strlen+1];
			strcpy( vstr, vtab->string );
			strcat( vstr, string );
		}
		Pname nn;
		if (nn=ptbl->look(vstr?vstr:string,0)) { // use of ptbl in file
        		fprintf(out_file,"extern struct __mptr %s[];\n",ss);
			s[2] = 'p';
        		// fprintf(out_file,"struct __mptr* %s = ",s);
        		// fprintf(out_file,"%s;\n",ss);
			ptbl_add_pair(s, ss);
			nn->n_key=HIDDEN; 
		}
		delete vstr;
	}
	delete ss;
	}
xyzzy:
	delete s;
	delete str;
}

void really_really_print(Pclass cl, Pvirt vtab, char* s, char* ss)
{
// error('d',"really %s %s",s, ss);
	// make sure function is declared before using
	// it in vtbl initializer
	Pname nn;
	int i;
	for (i=0; nn = vtab->virt_init[i].n; i++) {
		Pfct f = Pfct(nn->tp);
                if (nn->n_initializer) {       // pure virtual
			static pv;
			if (pv == 0) {	// VCVC void->char assumed
				fprintf(out_file,"char __pure_virtual_called();\n");
				pv = 1;
			}
			continue;
		}
		if (f->base != FCT) error('i',"vtbl %n",nn);
//extern void expand_dtor(Pclass cl);
//		if (f->f_inline == IDTOR) expand_dtor(f->memof);

		if (nn->n_dcl_printed==0 /*|| f->f_inline*/) {
			if (f->f_inline && vtbl_opt) puttok(STATIC);
			if (f->f_result == 0) make_res(f);
			Ptype r = f->s_returns ? f->s_returns : f->returns;
			r->print();
			nn->print();
			putstring("()");
			puttok(SM);
			nn->n_dcl_printed = 1;
		}
	}

//	if (vtbl_opt == -1) puttok(STATIC);


        fprintf(out_file,"struct __mptr %s[] = {0,0,0,\n",s);

	Pname n;
	for (i=0; n=vtab->virt_init[i].n; i++) {
		if (n->n_initializer)
			putstring("0,0,(__vptp)__pure_virtual_called,\n");
		else {
			fprintf(out_file,"%d,0,(__vptp)",-vtab->virt_init[i].offset);
			n->print();
			n->n_addr_taken = 1;
			putstring(",\n");
		}
	}
	putstring("0,0,0};\n");

	ss[2] = 'p';
        // fprintf(out_file,"struct __mptr* %s = ",ss);
	s[2] = 'v';
        // fprintf(out_file,"%s;\n",s);
	ptbl_add_pair(ss,s);

        Pname nm;
	char *cstr = 0;
	char *vstr = 0;
	char *cs = cl->nested_sig?cl->nested_sig:cl->string;
	if (cl->lex_level && cl->nested_sig==0) cstr = make_local_name( cl );
	if ( vtab && vtab->string ) {
		vstr = new char[strlen(vtab->string)+(cstr?strlen(cstr):strlen(cs))+1];
		strcpy( vstr, vtab->string );
		strcat( vstr, cstr?cstr:cs );
	}

	if ( vstr == 0 ) vstr = cstr;
	if ( nm = ptbl->look(vstr?vstr:cl->string,0) ) 
		nm->n_key = HIDDEN;
	else 
	if ( ptbl->look(vstr?vstr:cl->string,HIDDEN) == 0 )
		ptbl->insert(new name(vstr?vstr:cl->string),HIDDEN);
        if (cstr == vstr) delete cstr;
        else {
                delete cstr;
                delete vstr;
        }

//error('d',"really-> %s",s);
}

#include <ctype.h>
char* vtbl_name(char* s1, char* s2)
{
	char* s3 = (vtbl_opt == -1 && *src_file_name) ? src_file_name : 0;
		// if vtbl_opt == -1 fake a static (there are no portable
		// way of doing a forward declaration of a static in C)
	int ll = s1 ? strlen(s1) : 0;
	int ll2 = strlen(s2);
	int ll3 = s3 ? strlen(s3) : 0;
        int sz = (ll+ll2+ll3+20)/32+1;  // avoid fragmentation

        sz *= 32;
// error('d',"vtbl_name(%s,%s,%s) %d",s1?s1:"",s2,s3?s3:"",sz);
        char* buf = new char[sz];
	if (s3) {
		if (s1)
			sprintf(buf,"__vtbl__%d%s__%d%s__%s",ll,s1,ll2,s2,s3);
		else
			sprintf(buf,"__vtbl__%d%s__%s",ll2,s2,s3);
	}
	else if (s1)
		sprintf(buf,"__vtbl__%d%s__%d%s",ll,s1,ll2,s2);
	else
		sprintf(buf,"__vtbl__%d%s",ll2,s2);

	if (vtbl_opt == -1) {
		for (char* p = buf+ll2+11; *p; p++)
			if (!isalpha(*p) && !isdigit(*p)) *p = '_';
	}
#ifdef DENSE
	chop(buf);
#endif
	return buf;
}

void classdef::print_all_vtbls(Pclass bcl)
{
//error('d',"%t->print_all_vtbls(%t) vlt %d bl %d",this,bcl,virt_list,baselist);

	for (Pvirt blist = bcl->virt_list; blist; blist = blist->next) {
		if (this != blist->vclass) continue;
		if (blist->printed) continue;
        //      if (blist->string==0 && find_vptr(this)==0) {        //BSopt
        //              continue;
        //      }
	//	print_vtbl(blist);
		vlist = new vl(this,blist,vlist);
		blist->printed = 1;
	}

	for (Pbcl b = bcl->baselist; b; b = b->next)
		print_all_vtbls(b->bclass);

	if (this==bcl) c_body = 0;
}

extern Pclass current_instantiation;
void classdef::dcl_print(Pname)
{ 
//error('d',"%t->classdef::dcl_print() -- c_body==%d defined==0%o",this,c_body,defined);
	defined |= REF_SEEN;
// ensure template instantiations are printed exactly once.
        if (class_base != vanilla_class && current_instantiation != this) return;

	if (c_body==0 || c_body==3 || (defined&DEFINED)==0) return;
	c_body = 3;

	int i;
	for (Pname nn=memtbl->get_mem(i=1); nn; nn=memtbl->get_mem(++i) ) {
		if (nn->base==NAME
		&& nn->n_union==0
		&& nn->tp->base==CLASS
		&& Pclass(nn->tp)->c_body==1)
			Pclass(nn->tp)->dcl_print(nn);
		else if (nn->base == TNAME && Pbase(nn->tp)->base != COBJ)
			nn->dcl_print(0);
                else if (nn->tp && nn->tp->base == ENUM) {
			if(nn->n_union==0) Penum(nn->tp)->dcl_print(nn);
		}
	}

	TOK lvl = in_class ? NESTED : lex_level ? LOCAL : 0;
	Pname n = ktbl->look(string,lvl);
	if (n==0) n = ktbl->look(string,HIDDEN);

	if (n) {
		if (n->where.line!=last_line.line
		|| n->where.file!=last_line.file)
			if (last_ll = n->where.line)
				n->where.putline();
			else
				last_line.putline();
	}

	TOK c = csu==CLASS ? STRUCT : csu;
	puttok(c);
//	if (string[0]!='_' || string[1]!='_' || string[2]!='C')

        char *str = 0;
	if ( lex_level && nested_sig == 0) str = make_local_name( this );
	if ( nested_sig ) putstring( " __" );
	putst(str?str:(nested_sig?nested_sig:string));

	int sm = 0;
	int sz = tsizeof();
	int dvirt = 0;

	if ( nested_sig )
		fprintf(out_file,"{\t/* sizeof __%s == %d */\n",nested_sig,obj_size);
	else
		fprintf(out_file,"{\t/* sizeof %s == %d */\n",str?str:string,obj_size);
	if ( last_ll ) last_line.line++;
	delete str;

	print_members();
	for (Pbcl b = baselist; b; b = b->next) {	// declare virtual classes
		if (b->base != VIRTUAL) continue;
		Pclass bcl = b->bclass;
		dvirt += bcl->virt_count;
//error('d',"%t in %t %d",b->bclass,this,b->allocated);
		if (b->allocated==0) continue;
        	char* str =  0;
		if (bcl->lex_level) str = make_local_name(bcl);
		puttok(STRUCT);			// struct bcl Obcl;
		putst(str?str:bcl->string);
		putcat('O',bcl->string); // leave unencoded
		puttok(SM);
		delete str;
	}
	putstring("};\n");
	if ( last_ll ) last_line.line++;

	for (nn=memtbl->get_mem(i=1); nn; nn=memtbl->get_mem(++i) ) {
		if (nn->base==NAME && nn->n_union==0) {
			Ptype t = nn->tp;
			switch (t->base) {
			case FCT:
			case OVERLOAD:
				break;
			default:
				if (nn->n_stclass == STATIC) {
					TOK b = nn->n_sto;
//error('d',"print nn %n tp %t b %k eval %d",nn,nn->tp,b,nn->n_evaluated);
				/*
					Pname cn;
					TOK bb = ((cn=nn->tp->is_cl_obj())
					&& Pclass(cn->tp)->has_ctor())==0
						?0:b; // force explicit initialization
					nn->n_sto = (nn->n_evaluated) ? STATIC : bb;
				*/
					nn->n_sto = (nn->n_evaluated) ? STATIC : b;
					nn->dcl_print(0);
					nn->n_sto = b;
				}
			}
		}
	}
	if (vtbl_opt != -1) print_all_vtbls(this);	// force declaration
//error('d',"dcl_print -> ");
}


char *
make_local_name( Pclass cl, int ln ) 
{
    char *buf; 
    if ( cl->in_fct == 0 ) error( 'i', "localC %s missingFN", cl->string ); 
    char *fsig = Pfct(cl->in_fct->tp)->f_signature;
    if ( fsig == 0 ) fsig = local_sign( cl->in_fct->tp );
    char *fs = cl->in_fct->string;
    int class_len=cl->strlen+strlen(fsig)+strlen(fs)+strlen(cl->lcl)+4;
    int sz = (class_len+20)/32+1; // from vtbl_name()

    if ( Pfct(cl->in_fct->tp)->memof == 0 ) {
	 sz *= 32;
	 buf = new char[ sz ];
// error('d', "make_local_name: sz: %d", sz );

         if (ln) 
             sprintf(buf, "__%d%s__%s__%s%s", class_len, cl->string, fs, fsig, cl->lcl);
	 else
             sprintf(buf, "%s__%s__%s%s", cl->string, fs, fsig, cl->lcl);
    }
    else 
    {
        char *cs = Pclass(Pfct(cl->in_fct->tp)->memof)->string;
        int len = Pclass(Pfct(cl->in_fct->tp)->memof)->strlen;
        if ( len < 10 )
	     ++class_len;
	else 
        if ( len > 99 )
	     class_len += 3;
	else class_len += 2;
	class_len += len;
        sz = (class_len+20)/32+1; 
	sz *= 32;
        buf = new char[ sz ];
// error('d', "make_local_name: sz: %d", sz );

	if ( ln )
             sprintf(buf, "__%d%s__%s__%d%s%s%s",class_len,cl->string,fs,len,cs,fsig,cl->lcl);
        else sprintf(buf, "%s__%s__%d%s%s%s",cl->string,fs,len,cs,fsig,cl->lcl);
    }

#ifdef DENSE
    chop( buf );
#endif

    return buf;
}

