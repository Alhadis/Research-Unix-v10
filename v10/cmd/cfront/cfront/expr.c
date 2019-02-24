/*ident	"@(#)ctrans:src/expr.c	1.1.5.38" */
/***************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

expr.c:

	type check expressions

************************************************************************/

#include "cfront.h"
#include "size.h"

int const_save;
int const_ptr;
extern Ptype Pfct_type;

Pexpr expr::address()
{
// error('d',"address %k %d %s",base,base,base==NAME||base==ANAME?string:"?");
// error('d',"address e1 %d %k e2 %d %k", e1, e1?e1->base:0, e2, e2?e2->base:0); 
	switch (base) {
	case DEREF:
		if (e2 == 0) return e1;			// &*e	=> e
		break;
	case QUEST:					// &(a?b:c) => a?&b:&c
		e1 = e1->address();
		// no break;
	case G_CM:
		if ( e1 && e1->base == G_CALL &&
			e2 && e2->base == G_ADDROF)
			return this;
	case CM:
		e2 = e2->address();			// &(e1,e2) => (e1,&e2)
		tp = e2->tp;	//LLL
		return this;
	case INCR:
	case DECR:					// &(++a) => (++a,&a)
		if (e1) break;
		nin++;
		if (e2->not_simple()) error('s',"& of%k",base);
		nin--;
		e1 = new expr(base,0,e2);
		e2 = e2->address();
		base = G_CM;
		tp = e2->tp;
		return this;
	case ASSIGN:					// &(a=b) => ((a=b),&a)
	case ASPLUS:
	case ASMINUS:
	case ASMUL:
	case ASDIV:
	case ASMOD:
	case ASAND:
	case ASOR:
	case ASER:
	case ASLS:
	case ASRS:
		nin++;
		if (e1->not_simple()) error('s',"& of%k",base);
		nin--;
		Pexpr a = new expr(base,e1,e2);
		a->tp = a->e1->tp;
		base = G_CM;
		e1 = a;
		e2 = a->e1->address();
		tp = e2->tp;
		return this;

	case NAME:
		if (Pname(this)->n_stclass == REGISTER) error("& register%n",Pname(this));
		Pname(this)->take_addr();
		break;
	case CALL:
	case CAST:
	case NEW:
	case GNEW:
		if (tp && tp->is_ptr_or_ref()) {	// hack?
			return this;
		}
		break;
	}

	register Pexpr ee = new expr(G_ADDROF,0,this);
	if (tp) {					// tp==0 ???
		ee->tp = tp->addrof();

		switch (tp->base) {
		case PTR:
			Pptr(ee->tp)->memof = Pptr(tp)->memof;
			break;
		case FCT:
			if (Pfct(tp)->f_static==0)
			Pptr(ee->tp)->memof = Pfct(tp)->memof;
			break;
		case OVERLOAD:
			if (Pfct(Pgen(tp)->fct_list->f->tp)->f_static==0)
			Pptr(ee->tp)->memof = Pfct(Pgen(tp)->fct_list->f->tp)->memof;
		}
	}

	return ee;
}

Pexpr expr::contents()
{
//error('d',"deref %k %d %t",base,base,tp);
//	if (base==ADDROF || base==G_ADDROF) return e2;		// *&
	switch (base) {
	case ADDROF:
	case G_ADDROF:
		return e2;		// *&
	case ELIST:
//error('d',"contents of  elist");
		e1 = e1->contents();
		tp = e1->tp;
		return this;
	};

	register Pexpr ee = new expr(DEREF,this,0);
	if (tp) {			// tp==0 ???
		Ptype tt = tp;
		while (tt->base == TYPE) tt = Pbase(tt)->b_name->tp;
		ee->tp = Pptr(tt)->typ;
		Pname cn = ee->tp->is_cl_obj();
		if (cn) {
			Pclass cl = Pclass(cn->tp);
			if (cl->c_body==1) cl->dcl_print(0);	// look for first use of cl
		}
	}
	return ee;
}

int bound;
int chars_in_largest;	// no of characters in largest int
static Pclass mpglob;

Pexpr expr::typ(Ptable tbl)
/*
	find the type of "this" and place it in tp;
	return the typechecked version of the expression:
	"tbl" provides the scope for the names in "this"
*/
{
	//if (this == 0) error('i',"0->expr::typ");
	Pname n;
	Ptype t = 0;
	Ptype t1, t2;
	TOK b = base;
	TOK r1, r2;
#define nppromote(b)	t=np_promote(b,r1,r2,t1,t2,1)
#define npcheck(b)	(void)np_promote(b,r1,r2,t1,t2,0)
	if (tbl->base != TABLE) error('i',"expr::typ(%d)",tbl->base);

  	DB( if(Tdebug>=1) {
  		error('d',"%d->expr::typ(%d) %k %t",this,tbl,b,tp);
  		display_expr(this);
  	});
// error('d',"%k->typ %n tp: %t", b,b==NAME?this:0,tp);
// error('d',"     e1 %d %k e1 %d %k",e1,e1?e1->base:0,e2,e2?e2->base:0);
	if (tp) {
		switch (b) {
		case NAME:
//??			Pname(this)->use();
			break;
		case MDOT:
			mem = mem->typ(tbl);
		}	;
	//	if (b == NAME) Pname(this)->use();
		return this;
	}

extern Pname Ntmp;

	switch (b) {		// is it a basic type
	case MDOT:
		error('i',"mdot %s",string2);
	case DUMMY:
		error("emptyE");
		tp = any_type;
		return this;

	case ZERO:
		tp = zero_type;
		return this;

	case IVAL:
		tp = int_type;
		return this;

	//case FVAL:
	//	tp = float_type;
	//	return this;

	case ICON:
		/*	is it long?
			explicit long?
			decimal larger than largest signed int
			octal or hexadecimal larger than largest unsigned int
		 */
	{	int ll = strlen(string);
		switch (string[ll-1]) {
		case 'l':
		case 'L':
			switch (string[ll-2]) {
			case 'u':
			case 'U':
				string[ll-2] = 0;
				tp = ulong_type;
				goto cast_n_save;
			}
		lng:
			tp = long_type;
			goto save;
		case 'u':
		case 'U':			// 1u => unsigned(1)
			switch (string[ll-2]) {
			case 'l':
			case 'L':
				string[ll-2] = 0;
		ulng:
				tp = ulong_type;
				goto cast_n_save;
			default:
				string[ll-1] = 0;
		uint:
				tp = uint_type;
				goto cast_n_save;
			}
		}

		// no suffix - see if we can figure it out
		if  (string[0] == '0') {	// assume 8 bits in byte
			register index = 1;
			switch (string[1]) {
			case 'x':
			case 'X':
				while(string[++index]=='0') ;
				ll -= index;
				int HSZ = SZ_INT+SZ_INT;
				if(ll < HSZ) goto nrm;
				if(ll == HSZ)
					if(string[2]>='8') goto uint;
					else goto nrm;
				if(SZ_INT==SZ_LONG) break;
				HSZ = SZ_LONG+SZ_LONG;
				if(ll < HSZ) goto lng;
				if(ll == HSZ)
					if(string[2]>='8') goto ulng;
					else goto lng;
				break;
			default:   // OCTAL
				register IBITS = BI_IN_BYTE*SZ_INT;
				while(string[index]=='0') index++;
				register char x = string[index];
				int lbt = x=='1' ? 1 :
					( x=='2' || x=='3' ? 2 : 3 );
				int nbits = (ll-index-1)*3 + lbt;
				if(nbits < IBITS) goto nrm;
				if(nbits == IBITS) goto uint;
				if(nbits < BI_IN_BYTE*SZ_LONG) goto lng;
			}
			goto ulng;
		}
		else {  // DECIMAL
			if (ll<chars_in_largest) {
		nrm:
				tp = int_type;
				goto save;
			}
			if (ll>chars_in_largest) {
				if(SZ_INT==SZ_LONG || ll>2*chars_in_largest)
					goto ulng;
				goto lng;
			}
			// ll == chars_in_largest
			char* p = string;
			char* q = LARGEST_INT;
			do if (*p>*q) {
				if(SZ_INT==SZ_LONG) goto ulng;
				goto lng; 
			} while (*p++==*q++ && *p);
		}

		goto nrm;
	}
	case CCON:
		tp = c_strlen(string)<5 ? char_type : int_type;	// stored as 'a'
		goto save;

	case FCON:
	{	int ll = strlen(string);
		int last = string[ll-1];
		tp = double_type;
		if (last=='F' || last=='f') {
			tp = float_type;
			if (!ansi_opt) {
				string[ll-1] = 0;
				goto cast_n_save;
			}
		}
		else if (last=='L' || last=='l') {
			if (ansi_opt == 0) string[ll-1] = 0;
			tp = ldouble_type;
		}
		goto save;
	}
	case STRING:			// type of "as\tdf" is char[6]
					// c_strlen counts the terminating '\0'
	{	Pvec v = new vec(char_type,0);
		v->size = c_strlen(string);
		tp = v;
	}
	save:
		if (const_save) {	// "as\tdf" needs 7 chars for storage
			char* p = new char[strlen(string)+1];
			strcpy(p,string);
			string = p;
		}

		return this;

	cast_n_save:
		if (const_save) {	// "as\tdf" needs 7 chars for storage
			char* p = new char[strlen(string)+1];
			strcpy(p,string);
			string = p;
		}
		
		return new cast(tp,this);

	case THIS:
		delete this;
		if (cc->c_this) {
			cc->c_this->use();
			return cc->c_this;
		}
		error("``this'' used in nonC context");
		n = new name("this");
		n->tp = any_type;
		return tbl->insert(n,0); 

	case NAME:
	{	
		Pname q = Pname(this)->n_qualifier; // suppress virtual iff x::
		Pexpr ee = find_name(Pname(this),cc->cot,tbl,0,cc->nof);
		if (q && (ee->base==REF || ee->base==DOT))
			ee->n_initializer = Pexpr(q);
//error('d',"ee %k %t %n",ee->base,ee->tp,ee->base==NAME?ee:ee->base==REF?ee->mem:0);
	//	if (ee->tp->base == RPTR) return ee->contents();
		if (ee->tp->is_ref()) return ee->contents();

		if (ee->base==NAME && Pname(ee)->n_xref) {
		// fudge to handle X(X&) args
			ee = new expr(DEREF,ee,0);
			ee->tp = ee->e1->tp;	// !!
		}

		return ee;
	}

	case ADDROF:
	case G_ADDROF:	// handle lookup for &s::i
		mpglob = 0;
		if (e2->base == NAME) e2 = find_name(Pname(e2),cc->cot,tbl,ADDROF,cc->nof);
		if (e2->base==NAME && Pname(e2)->n_xref) {
			// fudge to handle X(X&) args
			e2 = new expr(DEREF,e2,0);
			e2->tp = e2->e1->tp;	// !!
		}
		if (e2->base==DOT) {	// &f().x  = > &(tmp=f(),&tmp)->x
			switch (e2->e1->base) {
			case CALL:
			case G_CALL:
			case VALUE:
				error("& non-lvalue");
			}
		}
		break;

	case SIZEOF:
		if (tp2) {	
			tp2->dcl(tbl);
			switch (tp2->base) {
			case VOID:
				error("sizeof(void)");
				break;
			case CLASS:
			{
				Pclass cl = Pclass(tp2);
				if (cl->c_body==1
				&& (cl->defined&(DEFINED|SIMPLIFIED)) == 0) 
					error('s',"class defined within sizeof");
			}
			}
	
			if (e1 && e1!=dummy) {
				e1 = e1->typ(tbl);
				DEL(e1);
				e1 = dummy;
			}
			Pptr r = tp2->is_ref();
			if (r) tp2 = r->typ;	// sizeof(T&)==sizeof(T)
		}
		else if (e1 == dummy) {
			error("sizeof emptyE");
			tp = any_type;
			return this;
		}
		else {
			e1 = e1->typ(tbl);
			tp2 = e1->tp;
			if(tp2->base == VEC) tp2->permanent = 1;
			if (e1->base==ILIST)	// PtoM
				e1 = dummy;
			else if (tp2 == char_type)	// sizeof ('a')
				e1 = dummy;
		}
		(void) tp2->tsizeof();
		tp = size_t_type;
		return this;

	case CAST:
//		return docast(tbl);

	{	Pexpr ee = docast(tbl);
		return ee->tp->is_ref() ? ee->contents() : ee;
	}


	case VALUE:
                if ( tp2->base == COBJ ) {
                        char *s = Pbase(tp2)->b_name->string;
                        //Pname tn = gtbl->look( s, 0 );
			//??? non type name apparently takes precedence
			//    Don't use global ftn if it hides local
			//    class ctor.  (Note: More info is needed
			//    to detect all cases.)
                        Pname tn = tbl->look( s, 0 );
//error( 'd', "tn: %n %t lex_levels: %d %d", tn, tn?tn->tp:0, tn?tn->lex_level:0, Pbase(tp2)->b_name->lex_level );
			if ( tn && tn->n_qualifier == 0
			&&   tn->lex_level == Pbase(tp2)->b_name->lex_level
			&&   (tn->tp->base==FCT || tn->tp->base==OVERLOAD)) 
			{
                                Pexpr et = new expr( CALL, tn, e1 );
                                *this = *et;
                                b = CALL;
                                break;
			}
		}
		return dovalue(tbl);

	case NEW:
	case GNEW:
		return donew(tbl);

	case DELETE:	// delete e1 OR delete[e2] e1
	case GDELETE:
	{	int i;
		if (e1->base == ADDROF) error('w',"delete &E");
		e1 = e1->typ(tbl);
		i = e1->tp->num_ptr(DELETE);
		if (i != 'P') error("nonP deleted");
		if (e2) {
			e2 = e2->typ(tbl);
			e2->tp->integral(DELETE);
		}
		tp = void_type;
		return this;
	}

	case ILIST:	/* an ILIST is pointer to an ELIST */
		e1 = e1->typ(tbl);
		tp = any_type;
		return this;

	case ELIST:
	{	Pexpr e;
		Pexpr ex;

		if (e1 == dummy && e2==0) {
			error("emptyIrL");
			tp = any_type;
			return this;
		}
				
		for (e=this; e; e=ex) {
			Pexpr ee = e->e1;
//error('d',"e %d %d ee %d %d",e,e?e->base:0,ee,ee?ee->base:0);
			if (e->base != ELIST) error('i',"elist%k",e->base);
			if (ex = e->e2) {	/* look ahead for end of list */
				if (ee == dummy) error("EX in EL");
				if (ex->e1 == dummy && ex->e2 == 0) {
					/* { ... , } */
					DEL(ex);
					e->e2 = ex = 0;
				}
			}
			e->e1 = ee->typ(tbl);
			t = e->e1->tp;
			if (t->base == FCT) {	// yuk!
				ee = new expr(G_ADDROF,0,e->e1);
				e->e1 = ee->typ(tbl);
				t = e->e1->tp;
			}
		}
		tp = t;
		return this;
	}

	case DOT:
	case REF:
	{	if (e2) { // *. or ->*
			if (b == REF) b = base = REFMUL;
			break;
		}

		Pbase b;

		e1 = e1->typ(tbl);
		t = e1->tp;

		// check that . (->) is applied to class object (pointer)
		if (base == REF) {
		xxx:
			switch (t->base) {
			case TYPE:
				t = Pbase(t)->b_name->tp;
				goto xxx;
			case COBJ:
			{	Pname n = Pclass(Pbase(t)->b_name->tp)->has_oper(REF);
				if (n) {
					n->n_used += 2;
					e1 = new call(new ref(DOT,e1,n),0);
					return typ(tbl);
				}
				// no break;
			}
			default:
				error("nonP ->%n",mem);
				t = any_type;
				// no break;
			case ANY:
				goto qqq;
			case PTR:
			case VEC:
				b = Pbase(Pptr(t)->typ);
			}
		}
                else {  // base == DOT
		qqq:
			switch (t->base) {
			case TYPE:	t = Pbase(t)->b_name->tp;	goto qqq;
			default:	error("nonO .%n",mem); t = any_type;
			case ANY:
			case COBJ:	break;
			}
//error('d',"dot %k",e1->base);
			switch (e1->base) { 
			case QUEST:
			case ASSIGN:
			case INCR:
			case DECR:
			case ASPLUS:
			case ASMINUS:
			case ASMUL:
			case ASDIV:
			case ASMOD:
			case ASAND:
			case ASOR:
			case ASER:
			case ASLS:
			case ASRS:
			case CM:
			case G_CM:
				base = REF;
				e1 = e1->address();
				break;
                        case CALL:
                        case G_CALL:
//error('d'," f().  %n mem->tp %t",e1->fct_name,mem->tp);
#ifdef FDOTRIGHT
// this rewrite is only necessary on machines with broken C compilers
// there seems to be a lot of those
// and for inlines (a,b).c
                        if (e1->fct_name && Pfct(e1->fct_name->tp)->f_inline)
#endif
                        {       // f(). => (tmp=f(),&tmp)->
                                Pname tmp = make_tmp('Q',e1->tp,tbl);
//error('d',"fdot2 %k",e1->base);
                                e1 = init_tmp(tmp,e1,tbl);
                                Pexpr aa = tmp->address();
                                e1 = new expr(G_CM,e1,aa);
                                e1->tp = aa->tp;
                                base = REF;
                                break;
                        }
                        }
			b = Pbase(t);
		}

	xxxx:
		switch (b->base) {
		case TYPE:
			b = Pbase(b->b_name->tp);
			goto xxxx;
		default:
			error("(%t) before %k%n (%n not aM)",e1->tp,base,mem,mem);
		case ANY:
			tp = any_type;
			return this;
		case COBJ:
		{	Pclass cl = Pclass(Pbase(b)->b_name->tp);
			if (cl->c_body == 1) cl->dcl_print(0);
			break;
		}
		}	

/*
	x.m is not a const even if x is a const object, this case is handled
	by lval() rejecting it
*/

		if (mem->tp) {
			tp = mem->tp;
			for (Pexpr ee = mem; ee->base==REF; ee = ee->e1) {
				if (ee->e1 == cc->c_this) { // this-> => p->
					ee->e1 = e1;
					ee->base = base;
					return mem->tp->is_ref() ? mem->contents() : mem;
				}
			}

		//	return tp->base==RPTR ? contents() : this;
			return tp->is_ref() ? contents() : this;
		}

		
		Pname q = Pname(mem)->n_qualifier;
		Pexpr e = find_name(Pname(mem),Pclass(b->b_name->tp),0,base,cc->nof);		
		if (q && (e->base==REF || e->base==DOT))
			Pname(e)->n_initializer = Pexpr(q);
//error('d',"e %k %t %n",e->base,e->tp,e->base==NAME?e:e->base==REF?e->mem:0);	

		for (Pexpr ee = e; ee->base==REF; ee = ee->e1) {
			if (ee->e1 == cc->c_this) { // this-> => p->
				ee->e1 = e1;
				ee->base = base;
				break;
			}
		}

		if (e->base == NAME) {
			switch (e->tp->base) {
			case FCT:
			case OVERLOAD:
				mem = e;
				tp = e->tp;
				e = this;
			}
		}
			
			// function or static member
		return e->tp->is_ref() ? e->contents() : e;
	}

//??	case G_CALL:
	case CALL:	/* handle undefined function names */
//error('d',"call %k %t %k",e1->base,e1->tp,e2?e2->base:0);
		if (e1->base==NAME && e1->tp==0) {
			Pname q = Pname(e1)->n_qualifier;
			e1 = find_name(Pname(e1),cc->cot,tbl,CALL,cc->nof);
//error('d',"e1 %k %t %n",e1->base,e1->tp,e1->base==NAME?e1:e1->base==REF?e1->mem:0);
			if (q && (e1->base==REF ||e1->base==DOT)) // suppress virtual call
				e1->n_initializer = Pexpr(q);
		}

		if (e1->base==NAME && Pname(e1)->n_xref) {
			// fudge to handle X(X&) args
			e1 = new expr(DEREF,e1,0);
			e1->tp = e1->e1->tp;	// !!
		}

		switch (e1->base) {
		case DOT:
/*
#ifdef FDOTRIGHT
		{
//error('d',"fdot1 %k",e1->e1->base);
			switch (e1->e1->base) {
			case CALL:
			case G_CALL:	// f().g() => (tmp=f(),tmp.g())
			{	Pexpr ee = e1->e1;
				Pexpr ex = ee->typ(tbl);
				Pname tmp = make_tmp('Q',ex->tp,tbl);
				ex = init_tmp(tmp,ex,tbl);

				Pexpr ee2 = new expr(base,e1,e2);
				e1->base = DOT;
				e1->e1 = tmp;

				base = G_CM;
				e1 = ex;
				e2 = ee2;

				return typ(tbl);
			}
			}
			break;
		}
#endif
*/
		case REF:			// becomes ob.x::~x(0) or
			if (e2==0		// becomes p->x::~x(0)
			&& Pref(e1)->mem 
			&& Pref(e1)->mem->base==NAME
			&& Pname(Pref(e1)->mem)->n_oper==DTOR) {
				e1 = e1->typ(tbl);
				Pexpr ee = call_dtor(e1->e1,Pref(e1)->mem,0,e1->base,one);
				ee->tp = void_type;
				return ee;
			}
		}
		break;

	case QUEST:
		cond = cond->typ(tbl);
	case ANDAND:
	case OROR:
		Ntmp = 0;
	}

	if (e1) {
		e1 = e1->typ(tbl);
		if (e1->tp->is_ref()) e1 = e1->contents();
		t1 = e1->tp;
	//	if (t1->is_cl_obj()) t1->tsizeof();
	}
	else
		t1 = 0;

	if (e2) {
		e2 = e2->typ(tbl);
		if (e2->tp->is_ref()) e2 = e2->contents();
		t2 = e2->tp;
	//	if (t2->is_cl_obj()) t2->tsizeof();
	}
	else 
		t2 = 0;

//error('d',"b %k t1 %t t2 %t",b,t1,t2);
	switch (b) {	// filter out non-overloadable operators
	default:
	{	Pexpr x = try_to_overload(tbl);
		if (x) return x;
		if (t2 && t1==0 && t2!=e2->tp) t2 = e2->tp;
	}
	case DOT:
	case G_CM:
	case G_ADDROF:
	case G_CALL:
	case QUEST:
		break;
	}

	switch (b) {
	case QUEST:
	case ANDAND:
	case OROR:
		if (Ntmp) error('s',"temporary ofC%n with destructor needed in%kE",Ntmp,b);
	}

	t = (t1==0) ? t2 : (t2==0) ? t1 : 0;
// error('d', "expr::typ t %t", t );
// error('d',"b2 %d %k",b,b);
	switch (b) {		/* are the operands of legal types */
	case REFMUL:
		base = REF;
		// no break;
	case DOT:
	{	// a .* p => &a MEMPTR p => appropriate indirection
			// to be considered: what happens if a .* expression
			// is used except in a call/=?
		Pexpr a = e1->typ(tbl);
		Ptype at = a->tp;
		if (base == DOT) {
			a = a->address();
			at = at->addrof();	// beware of n_xref
		}

		while (at->base == TYPE) at = Pbase(at)->b_name->tp;
		Pname cn = Pptr(at)->typ->is_cl_obj();
		Pclass mm = cn ? Pclass(cn->tp) : 0;

		Pexpr p = e2->typ(tbl);
		Ptype pt = p->tp;
		Pname pcn = pt->is_cl_obj();

//error('d',"mm %t pt %t",mm,pt);
		if (pcn) {
			Pclass cl = Pclass(pcn->tp);
			Pname found = 0;
			for (Pname on=cl->conv; on; on=on->n_list) {
				Pfct f = Pfct(on->tp);
				Pptr frt = Pptr(f->returns);
				while (frt->base == TYPE) frt = Pptr(Pbase(frt)->b_name->tp);
				if (frt->base!=PTR || Pptr(frt)->memof==0) break;
				if (Pptr(frt)->memof==mm || mm->has_base(Pptr(frt)->memof)) {
					if (found)
						error("two possible conversions forP toM: %n and %n",found,on);
					else
						found = on;
				}
			}
//error('d',"found %n",found);
			if (found) {
				p = new ref(DOT,p,found);
				p = new call(p,0);
				p = p->typ(tbl);
				pt = p->tp;
			}
	
		}
		while (pt->base == TYPE) pt = Pbase(pt)->b_name->tp;
		if (pt->base!=PTR || Pptr(pt)->memof==0) {
			error("P toMX in .*E: %t",pt);
			tp = any_type;
			base = DUMMY;
			return this;
		}
		Pclass pm = Pptr(pt)->memof;

	//	while (at->base == TYPE) at = Pbase(at)->b_name->tp;
	//	Pname cn = Pptr(at)->typ->is_cl_obj();
	//	Pclass mm = cn ? Pclass(cn->tp) : 0;
//error('d',"mm %t %t %d %d",mm,pm,pm->baseof(mm),mm->has_base(pm));

	//	if (mm!=pm && pm->baseof(mm)==0 && mm->has_base(pm)==0) {
		if (mm != pm) {
			if (mm->has_base(pm) == 0) {
				error("badOT in .*E: %t (%s*X)",a->tp,pm->string);
				tp = any_type;
				//return this;
			}
			a = new texpr(CAST,pm->this_type,a);
			e1 = a = a->typ(tbl);
		}

		Ptype tpx = Pptr(pt)->typ;
		while (tpx->base == TYPE) tpx = Pbase(tpx)->b_name->tp;

		if (tpx->base == FCT) {	// a.*p => (&a MEMPTR p)
			base = MEMPTR;
			tp2 = mm;	// keep the class for simpl.c
			e1 = a;
			e2 = p;
		}
		else {	// a .* p => *(typeof(p))((char*)&a + (int)p-1)
			a = new cast(Pchar_type,a);
			p = new cast(int_type,p);
			p = new expr(MINUS,p,one);
			p->tp = int_type;
			Pexpr pl = new expr(PLUS,a,p);
			pl->tp = Pchar_type;
			base = DEREF;
			pt = new ptr(PTR,tpx);	// need a T* not a T C::*
			Pptr(pt)->rdo = Pptr(tpx)->rdo;
			mpglob = pm;
		//	PERM(pt);
			e1 = new cast(pt,pl);
			e2 = 0;
		}
		tp = tpx;
		return tp->is_ref() ? contents() : this;
	}

	case G_CALL:
	case CALL:
		tp = call_fct(tbl);	/* two calls of use() for e1's names */
		if (tp->is_ref()) return contents();
		return this;

	case DEREF:
		if (e1 == dummy) error("O missing before []\n");
		if (e2 == dummy) error("subscriptE missing");

		if (t) {	/*	*t	*/
			while (t->base == TYPE) t = Pbase(t)->b_name->tp;
		//	t->vec_type();
			if (t->base==PTR && Pptr(t)->memof) error("P toM dereferenced");
			tp = t->deref();
		}
		else {					// e1[e2] that is *(e1+e2)
//error('d',"deref %t[%t]",t1,t2);
			if (t1->vec_type()) {		// e1[e2]
				switch (t2->base) {
				case CHAR:
				case SHORT:
				case INT:
				case LONG:
				case EOBJ:
					break;
				default:
				{	Pname cn = t2->is_cl_obj();
					if (cn)	// conversion to integral?
						e2 = check_cond(e2,DEREF,tbl);
					else
						t2->integral(DEREF);
				}
				}
				while (t1->base == TYPE) t1 = Pbase(t1)->b_name->tp;
				if (t1->base==PTR && Pptr(t1)->memof) error("P toM dereferenced");
				tp = t1->deref();
				(void) tp->tsizeof();
			}
			else if (t2->vec_type()) {	// really e2[e1]
				t1->integral(DEREF);
				while (t2->base == TYPE) t2 = Pbase(t2)->b_name->tp;
				if (t2->base==PTR && Pptr(t2)->memof) error("P toM dereferenced");
				tp = t2->deref();
				(void) tp->tsizeof();
			}
			else {
				error("[] applied to nonPT:%t[%t]",t1,t2);
				tp = any_type;
			}
		}

		if (tp->is_ref()) return contents();

		return this;

	case G_ADDROF:
	case ADDROF:
//error('d',"addrof(%d) %k %d",base,e2->base,e2->base);
		switch (e2->base) {	// potential lvalues
		case G_CM:
			if (base==ADDROF && e2->e2->base==NAME) {
				// check for cfront generated result variable
				char* s = e2->e2->string;
				if (s[0]=='_' && s[1] && s[1]=='_') {
					if (s[2] && (s[2]=='R' || s[2]=='V')) {
						error("address of non-lvalue");
						break;
					}
				}
			}
		case CM:	// &(a,b) => (a,&b)
		{
			Pexpr ee = e2;
			ee->tp = 0;
			ee->e2 = new expr(base,0,ee->e2);
			delete this;
			return ee->typ(tbl);
		}
		case QUEST:	// & (a?b:c) => (a?&b:&c)
		{
			Pexpr ee = e2;
			ee->tp = 0;
			ee->e1 = new expr(base,0,ee->e1);
			ee->e2 = new expr(base,0,ee->e2);
			delete this;
			return ee->typ(tbl);
		}
		case INCR:
		case DECR:
			if (e2->e1) break;
		case ASSIGN:
		case ASPLUS:
		case ASMINUS:
		case ASMUL:
		case ASDIV:
		case ASMOD:
		case ASAND:
		case ASOR:
		case ASER:
		case ASLS:
		case ASRS:
			return e2->address();
		}

		if (e2->lval(ADDROF) == 0) {
			if (error_count) {	// error_count == 0 possible
						// in case of & const
				tp = any_type;
				return this;
			}
		}

		tp = t->addrof();

		if( base==ADDROF && mpglob)
			Pptr(tp)->memof = mpglob;

		if (e2->tp->base == VEC) {
			base = CAST;
			tp2 = tp;
			e1 = e2;
			e2 = 0;
			return this;
		}

	//	tp = t->addrof();
		if (t->base==FCT) 
			Pptr(tp)->memof = Pfct(t)->memof;

//		if (t->tconst() && vec_const==0 && fct_const==0) Pptr(tp)->rdo = 1;
// ??? & (const T) is NOT T*const but const T*

		switch (e2->base) {
		case NAME:
		mname:					// check for &s::i
		{	Pname n2 = Pname(e2);
			Pname cn = (n2->n_table && n2->n_table!=gtbl) ? n2->n_table->t_name : 0;
// error('d',"n2 %k cn %n t %t",n2->base,cn,t);
			if (cn == 0) break;
		//	Pptr(tp)->memof = Pclass(cn->tp);

			if (t->base==FCT && Pfct(t)->f_this==0) {
				Pptr(tp)->memof = 0;
				break;
			}

			switch (t->base) {
			case OVERLOAD:
				return e2;
			case FCT:
// error('d', "cn->tp: %d, %k", cn->tp, cn->tp->base);
				Pptr(tp)->memof = Pclass(cn->tp); //SSS
				
				if (Pfct(t)->f_virtual)	{
					// { 0,vtbl index,0 }
					e1 = new ival(Pfct(t)->f_virtual);
					e1 = new expr(ELIST,zero,e1);
					e2 = zero;
					base = ILIST;
				//	tp = int_type;
					return this;
				}		// use the pointer
				
				// { 0,-1,(int(*)())ptr }
				e1 = new ival(-1);
				e1 = new expr(ELIST,zero,e1);
				// e2 is the name
				e2 = new cast(Pfct_type,e2);
				base = ILIST;
			//	tp = int_type;
				return this;

			//	return e2;

			default:
				if (n2->n_stclass != STATIC) {	// offset + 1
//error('d',"n2 %n %d",n2,n2->n_offset);
//error('d',"q %n cn %n",n2->n_qualifier,cn);
					e1 = new ival(n2->n_offset+1);
					Pptr(tp)->memof = Pclass(cn->tp);
//error('d',"cl %d %s i %d",Pclass(cn->tp),Pclass(cn->tp)?Pclass(cn->tp)->string:"0",n2->n_offset);
				}
				else
					return this;
			}
//error('d',"int_type");
			e1->tp = int_type;
			e2 = 0;
			tp2 = tp;
			base = CAST;
			return this;
		}
		case DOT:
                case REF:
                {       
                        Pname m = Pname(e2->mem);
// error( 'd', "e2->mem->base: %k", e2->mem->base );
//                      Pname m = (e2->mem->base == MDOT) ? Pname(e2->mem->mem) : Pname(e2->mem);
			while ( m->base == MDOT ) m = Pname(Pexpr(m)->mem);

			Pfct f = Pfct(m->tp);

			if (f->base == FCT || f->base == OVERLOAD) {  // &p->f
				Pexpr q = Pname(e2)->n_initializer;  // &p->x::f

				if (q && bound==0 && e2->e1==cc->c_this) {	
					// FUDGE: &this->x::f => &x::f
					DEL(e2);
					e2 = m;
					goto mname;
				}

				bound = 1;
				if(!f->f_static) {
					error(strict_opt?0:'w',
					    "address of boundF (try using ``%s::*'' forPT and ``&%s::%s'' for address) (anachronism)",
					    m->n_table->t_name->string,
					    m->n_table->t_name->string,
					    m->string
					);
				}

				if (q || f->f_virtual==0) {
					// & x.f  =>  & f
					DEL(e2);
					e2 = m;
				}
			}
			break;

		}
		case MEMPTR:
			// &(p->*q)
			error("& .* E");
		}

		return this;

	case UMINUS:
		t->numeric(UMINUS);
		tp = t;
		return this;

	case UPLUS:
		t->num_ptr(UPLUS);
		tp = t;
		base = PLUS;
		e1 = zero;
		return this;

	case NOT:
		e2 = check_cond(e2,NOT,tbl);
		tp = int_type;
		return this;

	case COMPL:
		t->integral(COMPL);
		tp = t;
		return this;

	case INCR:
	case DECR:
		{
		Pexpr e = e1?e1:e2;	// e1!=0 ==> e++ or e--
					// e1==0 ==> ++e or --e

		e->lval(b);

		switch(e->base) {
		case QUEST:
			e->tp=0;
			e->e1 = e1 ? new expr(base,e->e1,0) : new expr(base,0,e->e1);
			e->e2 = e1 ? new expr(base,e->e2,0) : new expr(base,0,e->e2);
			delete this;
			return e->typ(tbl);
		case CM:
		case G_CM:
			e->tp=0;
			e->e2 = e1 ? new expr(base,e->e2,0) : new expr(base,0,e->e2);
			delete this;
			return e->typ(tbl);
		case INCR:
		case DECR:
			if(e->e1)
				break;
			nin++;
			if(e->e2->not_simple())
				error('s',"overly complex %k of %k",b,e->base);
			nin--;
			e = new expr(G_CM,e,e->e2->typ(tbl));
			if (e1) e1=e; else e2=e;
			// e1?e1:e2 = e;
			return typ(tbl);
		case ASSIGN:
		case ASPLUS:
		case ASMINUS:
		case ASMUL:
		case ASDIV:
		case ASMOD:
		case ASAND:
		case ASOR:
		case ASER:
		case ASLS:
		case ASRS:
			nin++;
			if(e->e1->not_simple())
				error('s',"overly complex %k of %k",b,e->base);
			nin--;
			e = new expr(G_CM,e,e->e1->typ(tbl));
			if (e1) e1=e; else e2=e;
			// e1?e1:e2 = e;
			return typ(tbl);
		}
		}

		r1 = t->num_ptr(b);
		if (r1='P' && t->check(Pvoid_type,0)==0) error("%k of void*",b);
		tp = t;
		return this;
	
	}

	if (e1==dummy || e2==dummy || e1==0 || e2==0) error("operand missing for%k",b);
	switch (b) {
	case MUL:
	case DIV:
		r1 = t1->numeric(b);
		r2 = t2->numeric(b);
		nppromote(b);
		break;

	case PLUS:
		r2 = t2->num_ptr(PLUS);
		r1 = t1->num_ptr(PLUS);
		nppromote(PLUS);
		goto void_check;

	case MINUS:
		r2 = t2->num_ptr(MINUS);
		r1 = t1->num_ptr(MINUS);
		if (r2=='P' && r1!='P' && r1!='A') error("nonP - P");
		nppromote(MINUS);
	void_check:
		if ((r1='P' && t1->check(Pvoid_type,0)==0) 
		||  (r2='P' && t2->check(Pvoid_type,0)==0))
			error("%k of void*",b);
		tp = t;
		break;

	case LS:
	case RS:
	case AND:
	case OR:
	case ER:
		switch (e1->base) {
		case LT:
		case LE:
		case GT:
		case GE:
		case EQ:
		case NE:
			error('w',"%kE as operand for%k",e1->base,b);
		}
		switch (e2->base) {
		case LT:
		case LE:
		case GT:
		case GE:
		case EQ:
		case NE:
			error('w',"%kE as operand for%k",e2->base,b);
		}
	case MOD:
		r1 = t1->integral(b);
		r2 = t2->integral(b);
		nppromote(b);
		break;

	case LT:
	case LE:
	case GT:
	case GE:
	case EQ:
	case NE:
		r1 = t1->num_ptr(b);
		r2 = t2->num_ptr(b);
		if (r1=='P' && r2=='I') {	// allow things like:
						// if (p==2-2)
						// YUCK!
			Neval = 0;
			long i = e2->eval();
			if (Neval==0 && i==0) {
				DEL(e2);
				e2 = zero;
				r2 = 'Z';
			}
		}
		else if (r2=='P' && r1=='I') {
			Neval = 0;
			long i = e1->eval();
			if (Neval==0 && i==0) {
				DEL(e1);
				e1 = zero;
				r1 = 'Z';
			}
		}

		if (b!=EQ && b!=NE) {
			if (r1=='P' && r2=='Z') error("P%k 0",b);
			if (r2=='P' && r1=='Z') error("P%k 0",b);
		}

		// make sure functions are properly converted to pointers to
		// functions and make sure overloaded functions are rejected
		Pexpr ptof(Pfct ef, Pexpr e, Ptable tbl);
		if (r1 == FCT) e1 = ptof(0,e1,tbl);
		if (r2 == FCT) e2 = ptof(0,e2,tbl);

		npcheck(b);

		if (r1=='P') { // need cast for pointers to virtual and second bases
			Pptr p1 = t1->is_ptr();
			Pptr p2 = t2->is_ptr();
			Pname cn = p1?p1->typ->is_cl_obj():0;
			Pname cn2 = p2?p2->typ->is_cl_obj():0;

			if (cn && cn2) {
				Pclass cl = Pclass(cn->tp);
				Pclass cl2 = Pclass(cn2->tp);

				if (cl->has_base(cl2)) {
					e1 = cast_cptr(cl2,e1,tbl,0);
					e1 = new cast(p2,e1);
				}
				else if (cl2->has_base(cl)) {
					e2 = cast_cptr(cl,e2,tbl,0);
					e2 = new cast(p1,e2);
				}
			}
		}

		if (r1=='P' && t1->memptr()) {
		mpmp:
			if (r2=='Z') {
				e2 = zero;
				e1 = new mdot("i",e1);
				e1->i1 = 9;
			}
			else if (r2=='P' && t2->memptr()) {
				// ERROR: no check for sideeffects
				Pexpr i1 = new mdot("i",e1);
				Pexpr i2;
				i1->i1 = 9;

				i2 = e2;
				while ( i2->base == CAST )
				    i2 = i2->e1;
				if ( i2->base == ILIST )
				    e2 = i2;

				if (e2->base==ILIST)
					i2 = e2->e1->e2;
				else {
					i2 = new mdot("i",e2);
					i2->i1 = 9;
				}

				Pexpr f1 = new mdot("f",e1);
				Pexpr f2;
				f1->i1 = 9;
				if (e2->base == ILIST)
				//	f2 = new cast(Pfct_type,e2->e2);
					f2 = e2->e2;
				else {
					f2 = new mdot("f",e2);
					f2->i1 = 9;
				}

				Pexpr ei = new expr(base,i1,i2);
				Pexpr fi = new expr(base,f1,f2);

				base = ANDAND;
				e1 = ei;
				e2 = fi;
			} else if (r2=='P') {
				error('s',"%t %k %t",t1,base,t2);
			}
		}
		else if (r2=='P' && t2->memptr()) {
			Pexpr ee = e1;	// swap
			e1 = e2;
			e2 = ee;
			int rr = r1;
			r1 = r2;
			r2 = rr;
			Ptype tt = t1;
			t1 = t2;
			t2 = tt;
			goto mpmp;
		}
		t = int_type;
		break;

	case ANDAND:
	case OROR:
		e1 = check_cond(e1,b,tbl);
		e2 = check_cond(e2,b,tbl);
		t = int_type;
		break;

	case QUEST:
		{
		Pname c1, c2;
		cond = check_cond(cond,b,tbl);
//error('d',"cond %k %t",cond->base,cond->tp);
//error('d',"t1 %t t2 %t",t1,t2);

		// still doesn't do complete checking for possible conversions...

		bit MPTR = 0; // local hack

		extern int suppress_error;                      // global hack
		suppress_error++;
		r1 = t1->num_ptr(b);
		r2 = t2->num_ptr(b);
		suppress_error--;
if (r1=='P' && r2=='P' && t1->memptr() && t2->memptr()) {

		++MPTR; // prevent later sorry

		// watch for casts
		Pexpr tt = e1;
		while ( tt->base == CAST ) tt = tt->e1;
		if ( tt->base == ILIST ) e1 = tt; 

		tt = e2;
		while ( tt->base == CAST ) tt = tt->e1;
		if ( tt->base == ILIST ) e2 = tt; 

		// cannot have sides return (expr?{}:{})
		// reuse same temp for both sides ?:
		Pname temp = make_tmp( 'A', mptr_type, tbl );

		e1 = mptr_assign( temp, e1 );
		e1 = new expr( G_CM, e1, temp );
		e1->tp = temp->tp;

		e2 = mptr_assign( temp, e2 );
		e2 = new expr( G_CM, e2, temp );
		e2->tp = temp->tp;
}

		if (t1==t2
		|| (	(c1=t1->is_cl_obj())
			&& (c2=t2->is_cl_obj())
			&& (c1->tp==c2->tp)
		))
			t = t1;
		else {
			r1 = t1->num_ptr(b);
			r2 = t2->num_ptr(b);

			if (r1=='P' && r2=='P') {
				Pptr p1 = t1->is_ptr();
				Pptr p2 = t2->is_ptr();
//error('d',"p1 %t p2 %t",p1,p2);
				if ((c1 = p1->typ->is_cl_obj())
				&& (c2 = p2->typ->is_cl_obj())) {
					Pclass cl1 = Pclass(c1->tp);
					Pclass cl2 = Pclass(c2->tp);
					if (cl1==cl2 || cl2->has_base(cl1)) {
						t = t1;
						goto caca;
					}
					else if (cl1->has_base(cl2)) {
						t = t2;
						goto caca;
					}
				}
			}

			if (r1==FCT && r2==FCT) {	// fudge
				if (t1->check(t2,ASSIGN))
					error("badTs in ?:E: %t and %t",t1,t2);
				else if (Pfct(t1)->memof) error('s',"conditionalE with%t",t1);
				t = t1;
			}
			else 
				nppromote(b);
		caca:
//error('d',"?: t %t t1 %t t2 %t",t,t1,t2);
			if (t!=t1 && t->check(t1,0)) {
				PERM(t);
				e1 = new cast(t,e1);
			}

			if (t!=t2 && t->check(t2,0)) {
				PERM(t);
				e2 = new cast(t,e2);
			}

			Pptr pt = t->is_ptr();
			//if (pt && pt->base==PTR && pt->memof) error('s',"conditionalE with%t",t);
			if (pt && pt->base==PTR && pt->memof && MPTR == 0) error('s',"conditionalE with%t",t);
		}
		}
		break;

	case ASPLUS:
		r1 = t1->num_ptr(ASPLUS);
		r2 = t2->num_ptr(ASPLUS);
		nppromote(ASPLUS);
		goto ass;

	case ASMINUS:
		r1 = t1->num_ptr(ASMINUS);
		r2 = t2->num_ptr(ASMINUS);
		if (r2=='P' && r1!='P' && r1!='A') error("P -= nonP");
		nppromote(ASMINUS);
		goto ass;

	case ASMUL:
	case ASDIV:
		r1 = t1->numeric(b);
		r2 = t1->numeric(b);
		nppromote(b);
		goto ass;

	case ASMOD:
		r1 = t1->integral(ASMOD);
		r2 = t2->integral(ASMOD);
		nppromote(ASMOD);
		goto ass;

	case ASAND:
	case ASOR:
	case ASER:
	case ASLS:
	case ASRS:
		r1 = t1->integral(b);
		r2 = t2->integral(b);
		npcheck(b);
		t = int_type;
		goto ass;
	ass:
		if (r1='P' && t1->check(Pvoid_type,0)==0) error("%k of void*",b);
		tp = t;
		as_type = t;	/* the type of the rhs */
		t2 = t;

	case ASSIGN:
//error('d',"assign %k: %t %t %s",b,t1,t2,ignore_const?"ignore const":"");
//error('d',"    e1 %d %k e1 %d %k",e1,e1->base,e2,e2->base);
		if (ignore_const) {	// handle static initializers
					// represented as assignments
					// ignore consts.
			tp = e1->tp;
			return this;
		}

		switch (e1->base) {
		case G_CM:
		case CM:	// (a,b)=c => *(a,&b)=c
		{
			e1->e2 = new expr(G_ADDROF,0,e1->e2);
			e1->tp = 0;
			e1 = new expr(DEREF,e1,0);
			return typ(tbl);
		}
		case QUEST:	// (a?b:c)=d => *(a?&b:&c)=c
		{
			e1->e1 = new expr(G_ADDROF,0,e1->e1);
			e1->e2 = new expr(G_ADDROF,0,e1->e2);
			e1->tp = 0;
			e1 = new expr(DEREF,e1,0);
			return typ(tbl);
		}
		case ASSIGN:	// (a*=b)=c => a*=b,a=c
		case ASPLUS:
		case ASMINUS:
		case ASMUL:
		case ASDIV:
		case ASMOD:
		case ASAND:
		case ASOR:
		case ASER:
		case ASLS:
		case ASRS:
		{
			base = G_CM;
			nin++;
			if (e1->e1->not_simple()) error('s',"lvalue %k too complicated",b);
			nin--;
			Pexpr aa = new expr(e1->base,e1->e1,e1->e2);
			Pexpr bb = new expr(b,e1->e1,e2);
			e1 = aa;
			e2 = bb;
			return typ(tbl);
		}
		case INCR:
		case DECR:	// ++a=b => ++a,a=b
		{
			if(!e1->e2) break ;
			base = G_CM;
			nin++;
			if (e1->e2->not_simple()) error('s',"lvalue %k too complicated",b);
			nin--;
			Pexpr aa = new expr(e1->base,0,e1->e2);
			Pexpr bb = new expr(b,e1->e2,e2);
			e1 = aa;
			e2 = bb;
			return typ(tbl);
		}
		case REF:
		{
			Pexpr r = e1;
			// hack to prevent
			//	f().i = j
			// transformed into
			//	((t=f()),&t)->i = j
			if (r->e1->base==G_CM
			&& r->e1->e2->base==G_ADDROF
			&& r->e1->e2->e2->base==NAME) {
				char* s = r->e1->e2->e2->string;
				if (s[0]=='_' && s[1]=='_')
					error("left hand side not lvalue");
			}
		}
		}

		if (e1->lval(b) == 0) {
			tp = any_type;
			return this;
		}
	lkj:
		switch (t1->base) {
		case TYPE:
			t1 = Pbase(t1)->b_name->tp;
			goto lkj;
		case INT:
		case CHAR:
		case SHORT:
		//	if (e2->base==ICON && e2->tp==long_type)
		//		error('w',"long constant assigned to%k",t1->base);
		{	Ptype t = e2->tp;
		csi:
			switch (t->base) {
			case TYPE:
				t = Pbase(t)->b_name->tp; goto csi;
			case LONG:
			case FLOAT:
			case DOUBLE:
			case LDOUBLE:
				error('w',"%t assigned to %t",e2->tp,t1);
			}
		}
			// no break
		case LONG:
			if (b==ASSIGN
			&& Pbase(t1)->b_unsigned
			&& e2->base==UMINUS
			&& e2->e2->base==ICON)
				error('w',"negative assigned to unsigned");
			break;
		case PTR:
			if (b == ASSIGN) {
//error('d',"ptr t1 %t %d %t",t1,t1->memptr(),t2);
			//??	if (t1->memptr() && t2->base!=OVERLOAD) break;
				e2 = ptr_init(Pptr(t1),e2,tbl);
				t2 = e2->tp;
//error('d',"pchecked %d",Pchecked);
				if (Pchecked) {
					tp = e1->tp;
					return this;
				}
			}
			break;
   		case COBJ:
		{	Pname c1 = t1->is_cl_obj();
				// test of c1->tp necessary for ``fake classes''
				// _Sdd generated for vector assignemnts
			if (c1
			&& c1->tp
		/*	&& Pclass(c1->tp)->memtbl->look("__as",0)==0*/) {
			//&& Pclass(c1->tp)->has_oper(ASSIGN)==0) {
				Pname c2 = t2->is_cl_obj();
// error('d', "expr::typ: c1: %n c2: %n", c1, c2 );
				if (c1 != c2) {
/*
	consider:

	struct A { A(B&); };
	struct B : A {};

	A aa;
	B bb;

	aa = bb;	// aa.operator=(A(bb));
			// optimize to aa.A(bb) when possible
			// avoid temporary where aa = *(A*)&bb is legal
*/
// error('d',"expr::typ c1 %n %d c2 %n %d",c1,c1?c1->tp:0,c2,c2?c2->tp:0);
					if (c2
					&& c2->tp
					&& can_coerce(t1,t2)==0
					//&& Pclass(c2->tp)->has_base(Pclass(c1->tp))
                                   	//&& (1<is_unique_base(Pclass(c2->tp),c1->string,0))
					&& (vcllist->clear(),vcllist=0,1<is_unique_base(Pclass(c2->tp),c1->string,0))
					&& Pclass(c1->tp)->c_xref&(C_VBASE|C_VPTR|C_ASS)) {
// error('d',"aaa");
						if (make_assignment(c1)) return try_to_overload(tbl);
					}
				// optimize
					else {
						e2 = new expr(ELIST,e2,0);
						e2 = new texpr(VALUE,t1,e2);
						if (Pclass(c1->tp)->has_dtor()==0 &&
							Pclass(c1->tp)->has_oper(ASSIGN)==0) {
				// optimize
// error('d',"bbb");
							e2->e2 = e1;
							e2 = e2->typ(tbl);
                                                	if (e2->base==DEREF && e2->e1->base==G_CALL ||
                                                    	e2->base==ASSIGN && e2->e1==e1) {
// error('d',"ccc");
                                                        	*this = *e2;
							}
							tp = t1;
							return this;
						}
						return typ(tbl);
					}
				}
					// test of c1->tp necessary for ``fake classes''
					// _Sdd generated for vector assignemnts
				else if (c1->tp && Pclass(c1->tp)->c_xref&(C_VBASE|C_VPTR|C_ASS)) {
					if (make_assignment(c1)) return try_to_overload(tbl);
				}
			}
			(void) t1->tsizeof();
			break;
		}
		}

//error('d',"check(%t,%t) -> %d",e1->tp,t2,try_to_coerce(t1,e2,"assignment",tbl));
		{	Pexpr x = try_to_coerce(t1,e2,"assignment",tbl);
			if (x)
				e2 = x;
			else if (e1->tp->check(t2,ASSIGN))
				error("bad assignmentT:%t =%t",e1->tp,t2);
			else if ((t1 = t1->is_ptr()) && t1->memptr()) {
				if (t2 == zero_type) {
					Pexpr ee = new expr(ELIST,zero,zero);
					e2 = new expr(ILIST,ee,zero);
				}
				else if (t2->base==PTR && t2->memptr()) {
					// do nothing: structure assignment
				}
				else {
					Pexpr x = ptr_init(Pptr(t1),e2,tbl);
					if (x != e2) e2 = x;
				}
			}
		}		
		t = e1->tp;	// the type of the lhs
		break;
	case CM:
	case G_CM:
		t = t2;
		break;
	default:
		error('i',"unknown operator%k",b);
	}

	tp = t;
	return this;
}

