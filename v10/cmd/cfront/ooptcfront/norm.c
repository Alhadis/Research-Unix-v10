/*ident	"@(#)ctrans:src/norm.c	1.6.4.23" */
/************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

norm.c:

	"normalization" handles problems which could have been handled
	by the syntax analyser; but has not been done. The idea is
	to simplify the grammar and the actions accociated with it,
	and to get a more robust error handling

****************************************************************************/

#include "cfront.h"
#include "template.h"
#include "size.h"

void syn_init()
{
        any_type = new basetype(ANY,0); 	
	PERM(any_type); any_type->defined = DEFINED ;
	dummy = new expr(DUMMY,0,0);
	PERM(dummy);
	dummy->tp = any_type;
	zero = new expr(ZERO,0,0);
	PERM(zero);
}

int stcount;

char* make_name(TOK c)
{
	char* s = new char[8];	// as it happens: fits in two words

	if (15000 <= ++stcount) error('i',"too many generatedNs");

	s[0] = '_';		// __ prefix belongs to the implementation
	s[1] = '_';
	s[2] = c;
	int count = stcount;
	int i = 3;
	if (10000 <= count) {
		s[i++] = '0' + count/10000;
		count %= 10000;
	}
	if (1000 <= count) {
		s[i++] = '0' + count/1000;
		count %= 1000;
	}
	else if (3<i) s[i++] = '0';

	if (100 <= count) {
		s[i++] = '0' + count/100;
		count %= 100;
	}
	else if (3<i) s[i++] = '0';

	if (10 <= count) {
		s[i++] = '0' + count/10;
		count %= 10;
	}
	else if (3<i) s[i++] = '0';

	s[i++] = '0' + count;
	s[i] = 0;
//if (s[7]!=0) error('d',"impossible make_name");
	return s;
}

Pbase basetype::type_adj(TOK t)
{
//error('d',"bastype::type_adj(%k)",t);
	switch (base) {
	case COBJ:
	case EOBJ:
	{	Pbase bt = new basetype(0,0);
		*bt = *this;
		DEL(this);
		this = bt;
	}
	}
			
	if (b_xname) {
		if (base)
			error("badBT:%n%k",b_xname,t);
		else {
			base = TYPE;
			b_name = b_xname;
		}
		b_xname = 0;
	}

	switch (t) {
	case TYPEDEF:	b_typedef = 1;	break;
	case INLINE:	b_inline = 1;	break;
	case VIRTUAL:	b_virtual = 1;	break;
	case CONST:	if (b_const) error('w',"two const declarators");
			b_const = 1;	break;
	case UNSIGNED:	b_unsigned = 1;	break;
	case SHORT:	b_short = 1;	break;
	case LONG:	if (b_long) error('w',"two long declarators");
			if (base == DOUBLE)
				base = LDOUBLE;
			else
				b_long = 1;
			break;
	case FRIEND:
	case OVERLOAD:
	case EXTERN:
	case STATIC:
	case AUTO:
	case REGISTER:
		if (b_sto)
			error("badBT:%k%k",b_sto,t);
		else
			b_sto = t;
		break;
	case DOUBLE:
		if (b_long) {
			t = LDOUBLE;
			b_long = 0;
		}
		// no break
	case VOID:
	case CHAR:
	case INT:
	case FLOAT:
		if (base)
			error("badBT:%k%k",base,t);
		else
			base = t; 
		break;
	case SIGNED:
	case VOLATILE:
		error('w',"\"%k\" not implemented (ignored)",t);
		break;
	default:
		error('i',"BT::type_adj(%k)",t);
	}
	return this;
}

extern int in_arg_list;

Pbase basetype::name_adj(Pname n)
{
//error('d',"name_adj(%n)",n);
	if (b_xname) {
		if (base)
			error("badBT:%n%n",b_xname,n);
		else {
			base = TYPE;
			b_name = b_xname;
		}
		b_xname = 0;
	}

	if ( base==0 
 		&& n->base == TNAME 
 		&& ( n->tp->base!=COBJ || in_arg_list )) {
 			base = TYPE;
 			b_name = n;
	}
	else
		b_xname = n;

	return this;
}

TOK type_set( Pbase b ) {
	TOK t = 0;

	if ( b->b_long ) t = LONG;
	else if ( b->b_short ) t = SHORT;
	else if ( b->b_unsigned ) t = UNSIGNED;
	else if ( b->b_inline ) t = INLINE;
	else if ( b->b_virtual ) t = VIRTUAL;
	else if ( b->b_sto == OVERLOAD ) t = OVERLOAD;
	return t;
}

int declTag = 1;

Pbase basetype::base_adj(Pbase b)
{
	Pname bn = b->b_name;

	switch (base) {
	case COBJ:
	case EOBJ:
		error("NX after%k%n",base,b_name);
		return this;
	}

	TOK t;
	if (base) {
		if (b_name)
			error("badBT:%k%n%k%n",base,b_name,b->base,bn);
		else
			error("badBT:%k%k%n",base,b->base,bn);
	}
	else if ( t = type_set(this)) {
		if (b_name)
			error("badBT:%k%n%k%n",t,b_name,b->base,bn);
		else {
			if ( declTag++ ) error("badBT:%k%k%n",t,b->base,bn);
			base=b->base; b_name = bn;
// error('d',"base_adj: t: %k", t );
		}
	}
	else {
		base = b->base;
		b_name = bn;
		b_table = b->b_table;
	}
	return this;
}

Pbase basetype::check(Pname n)
/*
	"n" is the first name to be declared using "this"
	check the consistency of "this"
	and use "b_xname" for "n->string" if possible and needed
*/
{
	b_inline = 0;
	b_virtual = 0;
//error('d',"basetype::check(%n) base %k b_xname %n",n,base,b_xname);

	if (b_xname && (n->tp || n->string)) {
		if (base)
			error("badBT:%k%n",base,b_xname);
		else {
			base = TYPE;
			b_name = b_xname;
		}
		b_xname = 0;
	}

	if (b_xname) {
		if (n->string)
			error("twoNs inD:%n%n",b_xname,n);
		else {
			n->string = b_xname->string;
			b_xname->hide();
		}
		b_xname = 0;
	}

	if (ccl==0
	&& n
	&& n->n_oper==TNAME
	&& n->n_qualifier==0
	&& n->string) {	// hide type name
		Pname nx = ktbl->look(n->string,0);
		if (nx) nx->hide();
	}

	int defa = 0;
	switch (base) {
	case 0:
		defa = 1;
		base = INT;
		break;
	case EOBJ:
	case COBJ:
		if (b_name->base == TNAME) error('i',"TN%n inCO %p",b_name,this);
	}

	if (b_long || b_short) {
		TOK sl = (b_short) ? SHORT : LONG;
		if (b_long && b_short) error("badBT:long short%k%n",base,n);
		if (base != INT)
			error("badBT:%k%k%n",sl,base,n);
		else
			base = sl;
		b_short = b_long = 0;
	}

	if (b_typedef && b_sto) error("badBT:Tdef%k%n",b_sto,n);
	b_typedef = b_sto = 0; 

	if (b_linkage) {
		if (1 <= bl_level) error("local linkage directive");
/*
		Pfct f = Pfct(n->tp);
while (f && f->base==PTR) f = Pfct(Pptr(f)->typ);

		if (f && f->base==FCT) {
extern linkage;
			set_linkage(b_linkage);
			if (linkage)
				f->f_signature = "";
			else
				f->sign();
			set_linkage(0);
		}
*/
	}

	if (Pfctvec_type == 0) return this;

	if (b_const) {
		if (b_unsigned) {
			switch (base) {
			default:
				error("badBT: unsigned const %k%n",base,n);
				b_unsigned = 0;
			case LONG:
			case SHORT:
			case INT:
			case CHAR:
				return this;
			}
		}
		return this;
	}
	else if (b_unsigned) {
		switch (base) {
		case LONG:
			delete this;
			return ulong_type;
		case SHORT:
			delete this;
			return ushort_type;
		case INT:
			delete this;
			return uint_type;
		case CHAR:
			delete this;
			return uchar_type;
		default:
			error("badBT: unsigned%k%n",base,n);
			b_unsigned = 0;
			return this;
		}
	}
	else {
		switch (base) {
		case LONG:
			delete this;
			return long_type;
		case SHORT:
			delete this;
			return short_type;
		case INT:
                        if (this==int_type || this==defa_type) return this;
                //      if (this != int_type)
                                delete this;
                        if (defa) return defa_type;
                        return int_type;
		case CHAR:
			delete this;
			return char_type;
		case VOID:
			delete this;
			return void_type;
		case TYPE:
			/* use a single base saved in the keyword */
			if (b_name->n_qualifier) {
				Pbase rv = Pbase(b_name->n_qualifier);
				delete this;
				return rv;
			}
			else {
				PERM(this);
				b_name->n_qualifier = (Pname)this;
				return this;
			}
		default:
			return this;
		}
	}
}

Pname basetype::aggr()
/*
	"type SM" seen e.g.	struct s {};
				class x;
				enum e;
				int tname;
				friend cname;
				friend class x;
				int;
				
				typedef int i; // where i is tname

	convert
		union { ... };
	into
		union name { ... } name ;
*/
{
	if (b_xname) {
		if (base) {
			Pname n = new name(b_xname->string);
                        b_xname->hide();
                        b_xname = 0;
			return n->normalize(this,0,0);
}
		else {
			base = TYPE;
			b_name = b_xname;
			b_xname = 0;
		}
	}

	switch (base) {
	case COBJ:
	{	Pclass cl = Pclass(b_name->tp);
		char* s = cl->string;
		if (b_name->base == TNAME) error('i',"TN%n inCO",b_name);
		if (b_const) error("const%k%n",cl->csu,b_name);

		if (cl->c_body == 2) {	/* body seen */
			if (s[0]=='_'
			&& s[1]=='_'
			&& s[2]=='C') {
				char* ss = new char[8];	// max size of generated name is 7 chars, see make_name()
				Pname obj = new name(ss);
				strcpy(ss,s);
				if (cl->csu == UNION) {
					ss[2] = 'O';
					cl->csu = ANON;
					return obj->normalize(this,0,0);
				}
				error('w',"unusable%k ignored",cl->csu);
			}
			cl->c_body = 1;
			return b_name;
		}
		else {	/* really a typedef for cfront only: class x; */
			if (b_sto == FRIEND) goto frr;
			if (ansi_opt) printf("struct %s;\n",s);
			return 0;
		}
	}

	case EOBJ:
	{	Penum en = Penum(b_name->tp);
		if (b_name->base == TNAME) error('i',"TN%n in enumO",b_name);
		if (b_const) error("const enum%n",b_name);
		if (en->e_body == 2) {
			en->e_body = 1;
			return b_name;
		}
		else {
			error("forward declaration of enum%n", b_name);
			en->e_type = int_type;
		}
		//if (b_sto == FRIEND) goto frr;
		return 0;
	}

	case 0:
	{	Pname n = new name(make_name('D'));
		n->tp = defa_type;
		error("NX inDL");
		return n;
	}	
	default:
		if (b_typedef) error('w',"illegalTdef ignored");

		if (b_sto == FRIEND && b_name ) {
		frr:
			Pname fr = ktbl->look(b_name->string,0);
			if (fr == 0) error('i',"cannot find friend%n",b_name);
			Pname n = new name(b_name->string);
			n->n_sto = FRIEND;
			//If it is a parametrized type name, use the
			// sepcific instantiation type, and not the general
			// type.
			if ((fr->tp->base == COBJ) &&
			    (Pclass(Pbase(fr->tp)->b_name->tp)->class_base ==
			     template_class))
			  {
			    if (base == COBJ) n->tp = this ;
			    else if ((base == TYPE) &&
				     (Pbase(this)->b_name->base == TNAME) &&
				     (Pbase(this)->b_name->tp->base == COBJ))
			      n->tp = Pbase(this)->b_name->tp ;
			    else error('i', "basetype wasn't a COBJ") ;
			  }else n->tp = fr->tp;
			return n;
		}
		else {
			Pname n = new name(make_name('D'));
			n->tp = this;
			error('w',"NX inDL");
			return n;
		}
	}
}

// local class
extern void local_restore();
extern Plist local_blk;
void local_name() 
{ /* need to provide an additional temporary name
   * to handle case of
   * f() {
   *	 { class x{...}; }
   *	 { class x{...}; }
   * }
   * generate name after closing } of block
   * to distinquish between separate blocks at same lexical level 
   */
	for (Plist l=local_blk; l; l=l->l) {
		Pname n = l->f;
		if ( n->tp == 0 ) error( 'i', "no tp yet: #0 local_name" );
		if ( Pbase(n->tp)->b_name == 0 ) error( 'i', "no tp yet: #1 local_name" );
		Pname bn = Pbase(n->tp)->b_name;
		if ( bn->tp == 0 ) error( 'i', "no tp yet#2: local_name" );
		Pclass cl = Pclass(bn->tp);
                cl->lcl = make_name( 'L' );
// error( 'd', "local_name(): %n bn: %n: cl : %s nof: %d", n, bn, cl->string, cc->nof );
        }
}

// local class
void local_restore() 
{
	for (Plist l=local_tn; l; l=l->l) {
		Pname n = l->f;
		n->n_key = (n->lex_level==0)?0:(n->lex_level && n->lex_level<=bl_level) ? LOCAL : HIDDEN;
// error( 'd', "local_restore(): %n n_key: %d", n, n->n_key );
	}
}

// local class
void local_hide( Pname n ) 
{
   if ( n->base != TNAME ) return; 
   if ( n->n_key == 0 ) 
   {
        local_tn = new name_list( n, local_tn );
        n->n_key = HIDDEN;
// error( 'd', "local_hide(): %n n_key: %d", n, n->n_key );
   }
}

extern int defer_check = 0;
extern Ptype in_typedef;
extern Pname in_tag;
extern Pname statStat = 0;

void name::hide()
/*
	hide "this": that is, "this" should not be a keyword in this scope
*/
{
	if (base != TNAME) return;
	if (n_key == 0) {
                if (lex_level == bl_level && in_arg_list == 0) {
			if (tp->base != COBJ) { 
				if ( !in_typedef ) 
     				     error("%nredefined: typedef and identifier", this);
				else if ( in_typedef->base 
						&& tp->base != type_set(Pbase(in_typedef)) 
						&& in_typedef->check(tp,0) ) {
							if ( defer_check == 0 )
     								error("%nredefined: previous: %t now: %t", this, tp, in_typedef);
				} 
			} 
			else {
				if ( in_typedef && 
     					in_typedef->base && 
                                        in_typedef->check(tp, 0) &&
                                        defer_check == 0 ) {
    						error( "%nredefined: previous: %t now: %t", this, tp, in_typedef);
				}
				else {
					Pname nn = Pbase(tp)->b_name;
					Pclass cl = Pclass( nn->tp );
					if ( cl->has_ctor() )
   						error( "%nredefined: both aCN with constructor and an identifier", this );
				}
			}

                }
// error( 'd', "%n::hide", this );
		modified_tn = new name_list(this,modified_tn);
		n_key = HIDDEN;
	}
}

Pname Ntncheck; // ensure TNAMES hidden within class scopes
void set_scope(Pname tn) 
/* enter the scope of class tn after seeing "tn::f" */
{

// error( 'd', "set_scope: %n", tn );

	Pbase b = Pbase(tn->tp);
	if (b->b_name==0 || b->b_name->tp->base!=CLASS ) return;

	Pclass cl = Pclass(b->b_name->tp);

	if ( !Ntncheck || strcmp( tn->string, Ntncheck->string )) {
		int i = 1;
		Plist ll = 0;
		Pname n = 0;
 		if (b->parametrized_class())  {
 		  for (Pname nn = cl->mem_list ; nn ; nn = nn->n_list)
 		    switch (nn->base) {
 		    case PUBLIC: case PRIVATE: case PROTECTED:
 		      continue ;
 		    default:
 		      if (nn->tp->base == CLASS) continue;
 		      if (nn->tp->base == ENUM) continue;
 		      if ((nn->base == NAME) &&
			  ((nn->n_oper == TNAME) && (nn->tp->base == FCT)) ||
			  (nn->n_oper == CTOR) ||
			  (nn->n_oper == DTOR))
			continue ;
 		      n = ktbl->look( nn->string, 0 );
 		      if (n) ll = new name_list( n, ll );
 		    }
 		}else
		  for (Pname nn=cl->memtbl->get_mem(i); nn; nn=cl->memtbl->get_mem(++i) ) {
// error( 'd', "set scope: nn: %n %k", nn, nn->tp->base );
			if (nn->tp->base == CLASS) continue;
			if (nn->tp->base == ENUM) continue;
                        if (nn->base == TNAME) continue;
			n = ktbl->look( nn->string, 0 );
// if ( n ) error( 'd', "set scope: n: %n key: %d", n, n->n_key );
			if (n) ll = new name_list( n, ll );
		}

 		if ( ll ) cl->tn_list = ll;
		Ntncheck = tn;
	}
	
	for (Plist l=cl->tn_list; l; l=l->l) {
		Pname n = l->f;
		n->n_key = (n->lex_level) ? 0 : HIDDEN;
// error( 'd', "set scope: tn_list, n: %n n_key: %d n_lex_level: %d", n, n->n_key, n->lex_level );
		modified_tn = new name_list(n,modified_tn);
	}
}

void restore()
{
	for (Plist l=modified_tn; l; l=l->l) {
		Pname n = l->f;
// error('d',"restore: n %n %t %d bl_level: %d", n, n->tp, n->lex_level, bl_level );
		n->n_key = (n->lex_level==0 || (n->lex_level && n->lex_level<=bl_level)) ? 0 : HIDDEN;
		if ( n->lex_level == 0 
			&& (n->tp->base == COBJ || n->tp->base == EOBJ)) {
			Pname nn = gtbl->look( n->string, 0 );
			if ( nn ) n->n_key = HIDDEN;
		}
	}
}


// save and restore typenames around a function member declaration. This code
// fixes a bug where the declaration of a member function with a formal
// argument name that is the same as an outer type name blocks visibility to
// the name for the remainder of the class. Note that this fix does not deal
// correctly with nested class declarations, when the function member contains
// a class declaration which in turn contains a function declaration. I'm not
// sure whether such a construct is legal, or even useful, so am avoiding a
// more convoluted fix.	    
static Plist fn_modified_tn = 0 ;

void note_fn_modified_tn() {
  fn_modified_tn = modified_tn ;
}
	
	     
// restore the tnames that were modified during the processing of the function
// argument list within a class member.
void restore_fn_modified_tn(Pname n) {

  if ((n->base == NAME) && n->tp && (n->tp->base == FCT)) {
    for (Plist l=modified_tn; l; l=l->l)
      if (l == fn_modified_tn) {
	modified_tn = fn_modified_tn ;
	return ;
      }else{
	Pname n = l->f ;
	// code below courtesy of name::restore above
	n->n_key = (n->lex_level==0 || 
		    (n->lex_level && n->lex_level<=bl_level)) ? 0 : HIDDEN;
	if ( n->lex_level == 0 
	    && (n->tp->base == COBJ || n->tp->base == EOBJ)) {
	      Pname nn = gtbl->look( n->string, 0 );
	      if ( nn ) n->n_key = HIDDEN;
	    }
      }
  }
  fn_modified_tn = modified_tn ;
}
	


  
Pbase start_cl(TOK t, Pname c, Pbcl b)
{
        int mk_local = 0;
	if (c == 0)
	{
	    extern int in_class_decl;
	    c = new name(make_name('C'));
	    c->lex_level -= in_class_decl + 1;
	    if ( in_typedef && c->lex_level ) 
		 mk_local = 1;
	    else c->lex_level = 0;
	}

        for ( Pclass tc = ccl; tc; tc = tc->in_class )
            if ( tc->lex_level == c->lex_level &&
                 strcmp( tc->string, c->string) == 0) {
                    error( "C %s redefined", c->string );
                    c->tp = any_type;
                    return Pbase(any_type);
            }

	Pname n = c->tname(t);			/* t ignored */

	if (templp->in_progress && (c->lex_level == 0))
	  // bring the template in scope
	  templp->introduce_class_templ(n) ;
	
// typedef struct {} x;
	if ( mk_local ) {
	    n->n_key  = LOCAL;
	    extern Plist local_blk, local_class; // place in cfront.h
	    local_class = new name_list( n, local_class );
	    local_blk = new name_list( n, local_blk );
	    modified_tn = modified_tn->l;
	    }

	n->where = curloc;
	Pbase bt = Pbase(n->tp);		/* COBJ */
	if (bt->base != COBJ) {
		error("twoDs of%n:%t andC",n,bt);
		error('i', "can't recover from previous errors");
	}
	Pclass occl = ccl;
	ccl = Pclass(bt->b_name->tp);		/* CLASS */
	if (ccl->defined) ccl->defined |= IN_ERROR;
	ccl->defined |= DEF_SEEN;
	if (ccl->in_class = occl) {
		occl->tn_list = modified_tn;	// save  mod-list
		modified_tn = 0;
	}
	
	Ntncheck = 0;  // zero it out with each new class declaration
	ccl->string = n->string;
	ccl->csu = t;
	if (b) {	// list of base classes
		for (Pbcl bx, bb=b, l=0; bb; bb = bx) {
			bx = bb->next;
			bb->next = 0;

			if (l == 0) l = bb;
			else {	// append and check for duplicates
				for (Pbcl ll = l;;) {
					 if (bb->bclass && ll->bclass==bb->bclass) {
						error("%s has %s asB twice",ccl->string,bb->bclass->string);
						break;
					}

					if (ll->next)
						ll = ll->next;
					else {
						bb->next = l;
						l = bb;
						break;
					}
				}
			}
		}
		ccl->baselist = l;
	}
	return bt;	
}

void end_cl()
{
	Pclass occl = ccl->in_class;
	Plist ol = occl ? occl->tn_list : 0;	// saved modified name list
	ccl->c_body = 2;

	if (modified_tn) {	// export nested class names to outer scope:
		Plist local = 0;
		for (Plist l=modified_tn, nl=0; l; l=nl) {
			nl = l->l;
			Pname n = l->f;
			if (ktbl->look(n->string,0)) {
				// add it to enclosing class's modified name list
				l->l = ol;
				ol = l;
			}
			else {	// retain it in this class's modified name list
				l->l = local;
				local = l;
			}
		}
		if (ccl->tn_list = modified_tn = local) restore();
	}
	modified_tn = ol;	// restore mod-list (possibly modified)
	ccl = occl;
}

extern int in_class_decl;
Pbase end_enum(Pname n, Pname b)
{
	if (n == 0) n = new name(make_name('E'));
	n = n->tname(ENUM);
	Pbase bt = (Pbase)n->tp;
	if (bt->base != EOBJ) {
		error("twoDs of%n:%t and enum",n,bt);
		error('i', "can't recover from previous errors");
	}
	Penum en = (Penum)bt->b_name->tp;
	en->e_body = 2;
	en->mem = name_unlist((class nlist *)b);
	if (en->defined) {
		if ( in_class_decl )
			error("%nredefined, enum tag not local to class", n);
		en->defined |= IN_ERROR;
        }
	en->defined |= DEF_SEEN;
	return bt;
}

Pname name::tdef()
/*
	typedef "this"
*/
{
// error('d', "%n->tdef() %t", this, tp );
        int anon_cl = 0;
        if (n_qualifier) {
                error("QdN in typedef",this);
                n_qualifier = 0;
        }

	lex_level = bl_level - in_class_decl;
	Pname n = ktbl->insert(this,0);
	if (tp == 0) error('i',"Tdef%n tp==0",this);
	n->base = base = TNAME;
	PERM(n);
	PERM(tp);

	if (tp->base == COBJ || tp->base == EOBJ ) 
	{	// typedef struct/enum { } s; => struct/enum s {};
		Pname b = Pbase(tp)->b_name;
		if (b->string[0] == '_' && b->string[1] == '_' ) 
		    	switch ( tp->base ) {
				case COBJ: {
		             		if (b->string[2] == 'C') {
						Pclass cl = Pclass(b->tp);
						b->string = n->string;
						cl->string = n->string;
						cl->strlen = strlen(cl->string);
						if ( lex_level ) {
			     				anon_cl = 1;
			     				n->n_key = LOCAL;
						}
			      		}
			      		break;
				}
				case EOBJ: {
		             		if (b->string[2] == 'E') {
						Penum en = Penum(b->tp);
						b->string = n->string;
						en->string = n->string;
						en->strlen = strlen(en->string);
					}	
		    		}
	        	}
	}

	if ( anon_cl == 0 )
		modified_tn = new name_list(n,modified_tn);
	return n;
}

Pname name::tname(TOK csu)
/*
	"csu" "this" seen, return typedef'd name for "this"
	return	(TNAME,x)
	x:	(COBJ,y)
	y:	(NAME,z)
	z:	(CLASS,ae);
*/
{
	switch (base) {
	case TNAME:
		return this;
	case NAME:
	{	Pname tn = ktbl->insert(this,0);
// error('d',"tname %n",this);
		Pname on = new name;
		tn->base = TNAME;
		tn->lex_level = lex_level;
		modified_tn = new name_list(tn,modified_tn);
		tn->n_list = n_list = 0;
		string = tn->string;
		*on = *this;
		switch (csu) {
		case ENUM:
			tn->tp = new basetype(EOBJ,on);
			on->tp = new enumdef(0);
			Penum(on->tp)->string = tn->string;
			break;
		default:
			on->tp =  new classdef(csu);
			Pclass(on->tp)->string = tn->string;
			Pclass(on->tp)->lex_level = lex_level;
			tn->tp = new basetype(COBJ,on);
			Pbase(tn->tp)->b_table = Pclass(on->tp)->memtbl;
		}
		PERM(tn);
		PERM(tn->tp);
		PERM(on);
		PERM(on->tp);
		return tn;
	}
	default:
		error('i',"tname(%s %d %k)",string,this,base);
	} 
}

int co_hack;
Pname name::normalize(Pbase b, Pblock bl, bit Cast)
/*
	if (bl) : a function definition (check that it really is a type

	if (Cast) : no name string

	for each name on the name list
	invert the declarator list(s) and attatch basetype
	watch out for class object initializers

	convert
		struct s { int a; } a;
	into
		struct s { int a; }; struct s a;
*/
{
	Pname n;
	Pname nn;
	TOK stc;
	bit tpdf;
	bit inli;
	bit virt;
	char * lnkg;
// error('d',"%n::normalize b %k",this,b->base);
	if (b) {
		stc = b->b_sto;
		tpdf = b->b_typedef;
		inli = b->b_inline;
		virt = b->b_virtual;
		lnkg = b->b_linkage;
	}
	else {
		stc = 0;
		tpdf = 0;
		inli = 0;
		virt = 0;
		lnkg = 0;
	}

	if (inli && stc==EXTERN)  {
		error("both extern and inline");
		inli = 0;
	}

	if (stc==FRIEND && tp==0) {
		/*	friend x;
			must be handled during syntax analysis to cope with
				class x { friend y; y* p; };
			"y" is not local to "x":
				class x { friend y; ... }; y* p;
			is legal

			examples:
				
				typedef void	SIG_TYP(int);
				class x {
					friend class y;
					friend z;
					friend x;		// dumb
					friend int i;		// error
					friend SIG_TYP sigFunc;	// subtle
					friend int f();
					friend g(int);
				};
		*/
		if (b && (b->base || b->b_name || b->b_xname)) goto ccc;

		if (n_list) {
			error("L of friends");
			n_list = 0;
		}

		if (!Cast) {
    			Pname nn = gtbl->look( string, 0 );
    			if ( nn ) {
        			if (nn->tp->base == FCT) 
        				error("friendF must include signature:%n", this ); 
    				else
        				error("illegal friend declaration:%n", this );
    			}
		}

//error( 'd', "%n ll: %d", ccl, ccl->lex_level );
		lex_level = ccl->lex_level;
		Pname nx = tname(CLASS);
		modified_tn = modified_tn->l;	/* global */
		n_sto = FRIEND;
		tp = nx->tp;
		return this;
	}
ccc:
	if (tp  			// FUDGE: fix the bad grammar
	&& tp->base==FCT
	&& (n_oper==TNAME || Pfct(tp)->returns)) {
		Pfct f = Pfct(tp);
		Pfct f2 = Pfct(f->returns);

		if (f2) {
			Ptype pt;
			Ptype t = f2;
		lxlx:
			switch (t->base) {
			case PTR:	// x(* p)(args) ?
			case VEC:	// x(* p[10])(args) ?
				if (pt = Pptr(t)->typ) {
					if (pt->base == TYPE) {
						Pptr(t)->typ = 0;
						b = Pbase(pt);
					//	stc = b->b_sto;
					//	tpdf = b->b_typedef;
					//	inli = b->b_inline;
					//	virt = b->b_virtual;
					}
					else {
						t = pt;
						goto lxlx;
					}
				}
				goto zse1;
			case FCT:
			{//	Pexpr e = f2->argtype;
				Pexpr e = Pfct(f)->argtype;
				if  (e && e->base==ELIST) { // get the real name; fix its type
					if (e->e2 || e->e1->base!=DEREF) goto zse1;
					Pexpr ee = e->e1;
					Ptype t = 0;
					Ptype tpx;
				ldld:
					switch (ee->base) {
					case DEREF:
					{	Ptype tt = (ee->e2) ? Ptype(new vec(0,ee->e2)) : Ptype (new ptr(PTR,0));
						if (t)
							Pptr(t)->typ = tt;
						else
							tpx = tt;
						t = tt;
						ee = ee->e1;
						goto ldld;
					}
					case NAME:
					{	Pname rn = Pname(ee);
						b = new basetype(TYPE,ktbl->look(string,0));
						f->returns = tpx;
						n_oper = 0;
						string = rn->string;
						base = NAME;
					}
					}
				}
			}
			}
		}
	}

zse1:
	if (b == 0) {
		error("BTX for %s",string);
		b = Pbase(defa_type);
	}
	
	if (Cast) string = "";
	b = b->check(this);

	switch (b->base) {	//	separate class definitions
				//	from object and function type declarations
	case COBJ:
		nn = b->b_name;

		if (Pclass(nn->tp)->c_body==2) {	/* first occurrence */
			if (tp && tp->base==FCT && co_hack == 0) {
				error(&this->where,"%k%n defined as returnT for%n (did you forget a ';' after '}' ?)",Pclass(nn->tp)->csu,nn,this);
				nn = this;
				break;
			}
			nn->n_list = this;
			Pclass(nn->tp)->c_body = 1;	/* other occurences */
		}
		else
			nn = this;
		break;
	case EOBJ:
		nn = b->b_name;
		if (Penum(nn->tp)->e_body==2) {
			if (tp && tp->base==FCT) {
				error(&this->where,"enum%n defined as returnT for%n (did you forget a ';'?)",nn,this);
				nn = this;
				break;
			}
			nn->n_list = this;
			Penum(nn->tp)->e_body = 1;
		}
		else {
			Penum en = Penum(nn->tp);
			if ( en->defined == 0 ) 
				error( "forward declaration of enum%n", nn );
			en->e_type = int_type; 
			nn = this;
		}
		break;
	default:
		nn = this;
	}

	Pname nx; 
	for (n=this; n; n=nx) {
		Ptype t = n->tp;
		nx = n->n_list;
		n->n_sto = stc;

		if (n->base == TNAME) error('i',"redefinition ofTN%n",n);

		if (t == 0) {
			if (bl == 0)
				n->tp = t = b;
			else {
				error("body of nonF%n",n);
				t = new fct(defa_type,0,0);
			}
		}

		switch (t->base) {
		case PTR:
		case RPTR:
			n->tp = Pptr(t)->normalize(b);
			break;
		case VEC:
			n->tp = Pvec(t)->normalize(b);
			break;
		case FCT:
			n->tp = Pfct(t)->normalize(b);
			break;
		case FIELD:
			if (n->string == 0) n->string = make_name('F');
			n->tp = t;
			Pbase tb = b;
// error('d', "field t %k tb %k", t->base, tb->base );
		flatten:
			switch (tb->base) {
			case TYPE:   /* chase typedefs */
				tb = (Pbase)tb->b_name->tp;
				goto flatten;
			case CHAR:
			case SHORT:
			case EOBJ:
			case INT:
//  typedef const unsigned cu_int;
//     struct x { x(); cu_int b1: 2; }

				Pbase(t)->b_fieldtype = (b->b_unsigned||tb->b_unsigned) ? uint_type : int_type;
		//		goto iii;
		//	case CHAR:
		//		Pbase(t)->b_fieldtype = (b->b_unsigned) ? uchar_type : char_type;
		//		goto iii;
		//	case SHORT:
		//		Pbase(t)->b_fieldtype = (b->b_unsigned) ? ushort_type : short_type;
		//		goto iii;
		//	iii:
				Pbase(t)->b_unsigned = b->b_unsigned?b->b_unsigned:tb->b_unsigned;
				Pbase(t)->b_const = b->b_const?b->b_const:tb->b_const;
				break;
			default:
				error("non-int field");
				n->tp = defa_type;
			}
			break;
		}

		Pfct f = Pfct(n->tp);

		if (f->base != FCT) {
			if (bl) {
				error("body for nonF%n",n);
				n->tp = f = new fct(defa_type,0,0);
				continue;
			}
			if (inli) error("inline nonF%n",n);
			if (virt) error("virtual nonF%n",n);
				
			if (tpdf) {
				if (n->n_initializer) {
					error("Ir forTdefN%n",n);
					n->n_initializer = 0;
				}
				n->tdef();
			}
			continue;
		}

		if (lnkg) {
			extern linkage;
			set_linkage(lnkg);
			if (linkage)
				f->f_signature = "";
			else
				f->sign();
			set_linkage(0);
		}

		f->f_inline = inli;
		extern int vcounter;
                f->f_virtual = virt?(vcounter++,VTOK):0;

		if (tpdf) {
			if (f->body = bl) error("Tdef%n { ... }",n);
			if (n->n_qualifier) {
				// typedef T x::f(args);
				// a pointer to member fucntion:
				// equivalent to typedef T x::(f)(args);
				f->memof = Pclass(Pbase(n->n_qualifier->tp)->b_name->tp);
				n->n_qualifier = 0;
			}
			n->tdef();
			continue;
		}

		if (f->body = bl) continue;

		/*
			Check function declarations.
			Look for class object instansiations
			The real ambiguity:		; class x fo();
				is interpreted as an extern function
				declaration NOT a class object with an
				empty initializer
		*/
		{	Pname cn = f->returns->is_cl_obj();
			Ptype template_formal_type ;
			
			bit clob = (cn || cl_obj_vec);
			if (f->argtype) {
				/* check argument/initializer list */
				Pname nn;

				for (nn=f->argtype; nn; nn=nn->n_list) {
				  if (nn->base != NAME) {
				    if (!clob) {
				      if ((f->returns->base == TYPE) &&
					  (Pbase(f->returns)->b_name->n_template_arg  == template_type_formal))
					{
					// T x(var) ; wher T is a template formal
					// it could be a class when
					// instantiated, wait until then to
					// issue an error messsage.
					template_formal_type = f->returns ;
					Pbase(f->returns)->b_name->n_template_formal_must_be_class = 1 ;
					goto is_obj ;
				      }
					
				      error("ATX for%n",n);
				      goto zzz;
						}
				    goto is_obj;
					}
/*
					if (nn->string) {
						error("AN%n inD of%n",nn,n);
						nn->string = 0;
					}
*/
					if (nn->tp) goto ok;
				}
				if (!clob) {
					error("FALX");
					goto zzz;
				}
		is_obj:
				/* it was an initializer: expand to constructor */
				n->tp = f->returns;
				if (f->argtype->base != ELIST) f->f_args = f->argtype = (Pname)new expr(ELIST,(Pexpr)f->argtype,0);
				n->n_initializer = new texpr(VALUE,
							     (cn ? cn->tp :
							      template_formal_type)
							      ,(Pexpr)f->argtype);
				goto ok;
			zzz:
				if (f->argtype) {
					DEL(f->argtype);
					f->argtype = 0;
					f->nargs = 0;
					f->nargs_known = 1;
				}
			}
			else {	/* T a(); => function declaration */
/*
				if (clob) {
					DEL(n->tp);
					n->tp = f->returns;
				}
*/
			}
		ok:
			;
		}
	}
	return nn;
}

Ptype vec::normalize(Ptype vecof)
{
	Ptype t = typ;
	typ = vecof;
	if (t == 0) return this;

xx:
	switch (t->base) {
	case TYPE:	t = Pbase(t)->b_name->tp;
			goto xx;
	case PTR:
	case RPTR:	return Pptr(t)->normalize(this);
	case VEC:	return Pvec(t)->normalize(this);
	case FCT:	return Pfct(t)->normalize(this);
	default:	error('i',"bad arrayT(%d)",t->base);
	}

}

Ptype ptr::normalize(Ptype ptrto)
{
//	if (this == 0) error('i',"0->ptr.normalize()");
	Ptype t = typ;
	typ = ptrto;

	int bc = 0;
	while (ptrto->base == TYPE) {
		bc += Pbase(ptrto)->b_const;
		ptrto = Pbase(ptrto)->b_name->tp;
	}

	switch (ptrto->base) {
	case FCT:
		if (memof)
			if (Pfct(ptrto)->memof) {
				if (memof != Pfct(ptrto)->memof) error("P toMF mismatch: %s and %s",memof->string, Pfct(ptrto)->memof->string);
			}
			else
				Pfct(ptrto)->memof = memof;
		else
			memof = Pfct(ptrto)->memof;
		break;
	case RPTR:
		switch (base) {
		case PTR:	error("P toR"); break;
		case RPTR:	error("R toR"); break;
		}
	}

	if (t == 0) {
		Pbase b = Pbase(ptrto);
		if (Pfctvec_type
		&& rdo==0
		&& b->b_unsigned==0
		&& b->b_const==0
		&& bc == 0
		&& memof==0
		&& base==PTR) {
			switch (b->base) {
			case INT:	delete this;	return Pint_type;
			case CHAR:	delete this;	return Pchar_type;
			case VOID:	delete this;	return Pvoid_type;
			}
		}
		if (base==RPTR && b->base==VOID) error("void& is not a validT");
		return this;
	}

xx:
	switch (t->base) {
	case TYPE:	t = Pbase(t)->b_name->tp;
			goto xx;
	case PTR:
	case RPTR:	return Pptr(t)->normalize(this);
	case VEC:	if (base == RPTR) error("array ofRs");
			return Pvec(t)->normalize(this);
	case FCT:	return Pfct(t)->normalize(this);
	default:	error('i',"badPT(%k)",t->base);
	}
}

Ptype fct::normalize(Ptype ret)
/*
	normalize return type
*/
{
	register Ptype t = returns;
	returns = ret;

	if (argtype && argtype->base==NAME && argtype->n_qualifier) {
		error("syntax: ANX");
		argtype = 0;
		nargs = 0;
		nargs_known = 0;
	}

	if (t == 0) return this;
xx:
	switch (t->base) {
	case PTR:
	case RPTR:	return Pptr(t)->normalize(this);
	case VEC:	error("array ofFs");
			return Pvec(t)->normalize(this);
	case FCT:	return Pfct(t)->normalize(this);
	case TYPE:	t = Pbase(t)->b_name->tp;	goto xx;
	default:	error('i',"badFT:%k",t->base);
	}

}

	
void fct::argdcl(Pname dcl, Pname fn)
/*
	sort out the argument types for old syntax:
			f(a,b) int a; char b; { ... }
	beware of
			f(a) struct s { int a; }; struct s a;
*/
{
	Pname n;
/*fprintf(stderr,"%d argtype %d %d dcl %d %d\n",this, argtype, argtype?argtype->base:0, dcl, dcl?dcl->base:0); fflush(stderr);*/
	switch (base) {
	case FCT:	break;
	case ANY:	return;
	default:	error('i',"fct::argdcl(%d)",base);
	}

	if (argtype) {
		switch (argtype->base) {
		case NAME:
			if (dcl) error("badF definition syntax");
			for (n=argtype; n; n=n->n_list) {
				if (n->string == 0) n->string = make_name('A');
			}
			return;
		case ELIST:	// expression list:	f(a,b,c) int a; ... { ... }
				// scan the elist and build a NAME list
		{
			Pname tail = 0;
			n = 0;

			error(strict_opt?0:'w',&fn->where,"old style definition of%n",fn);

			for (Pexpr e=Pexpr(argtype); e; e=e->e2) {
				Pexpr id = e->e1;
				if (id->base != NAME) {
					error("NX inAL");
					argtype = 0;
					dcl = 0;
					break;
				}
				Pname nn = new name(id->string);
				if (n)
					tail = tail->n_list = nn;
				else
					tail = n = nn;
			}
			f_args = argtype = n;
			break;
		}
		default:
			error("ALX(%d)",argtype->base);
			argtype = 0;
			dcl = 0;
		}
	}
	else {
		nargs_known = 1;
		nargs = 0;
		if (dcl) error("ADL forFWoutAs");
		return;
	}

//	nargs_known = 0;

	if (dcl) {
		Pname d;
		Pname dx;
		/*	for each  argument name see if its type is specified
			in the declaration list otherwise give it the default type
		*/

		for (n=argtype; n; n=n->n_list) {
			char* s = n->string;
			if (s == 0) {
				error("AN missing inF definition");
				n->string = s = make_name('A');
			}
			else if (n->tp) error("twoTs forA %s",n->string);

			for (d=dcl; d; d=d->n_list) {
				if (strcmp(s,d->string) == 0) {
					if (d->tp->base == VOID) {
						error("voidA%n",d);
						d->tp = any_type;
					}
					n->tp = d->tp;
					n->n_sto = d->n_sto;
					d->tp = 0;	// now merged into argtype
					goto xx;
				}
			}
			n->tp = defa_type;
		xx:;
			if (n->tp == 0) error('i',"noT for %s",n->string);
		}
	
		/*	now scan the declaration list for "unused declarations"
			and delete it
		*/
		for (d=dcl; d; d=dx) {
			dx = d->n_list;
			if (d->tp) {	/* not merged with argtype list */
				/*if (d->base == TNAME)  ??? */
				switch (d->tp->base) {
				case CLASS:
				case ENUM:
					/* WARNING: this will reverse the order of
					   class and enum declarations
					*/
					d->n_list = argtype;
					f_args = argtype = d;
					break;
				default:
					 error("%n inADL not inAL",d);
				}
			}
		}
	}

	/* add default argument types if necessary */
	for (n=argtype; n; n=n->n_list) {
		if (n->tp == 0) n->tp = defa_type;
		nargs++;
	}
}

Pname cl_obj_vec;	/* set if is_cl_obj() found a array of class objects */
Pname eobj;		/* set if is_cl_obj() found an enum */

Pname type::is_cl_obj()
/*
	returns this->b_name if this is a class object
	returns 0 and sets cl_obj_vec to this->b_name
		if this is a array of class objects
	returns 0 and sets eobj to this->b_name
		if this is an enum object
	else returns 0
*/
{
	bit v = 0;
	register Ptype t = this;

	if (t == 0) return 0;
	eobj = 0;
	cl_obj_vec = 0;
xx:
	switch (t->base) {
	case TYPE:
		t = Pbase(t)->b_name->tp;
		goto xx;

	case COBJ:
		if (v) {
			cl_obj_vec = Pbase(t)->b_name;
			return 0;
		}
		else
			return Pbase(t)->b_name;

	case VEC:
		t = Pvec(t)->typ;
		v=1;
		goto xx;

	case EOBJ:
		eobj = Pbase(t)->b_name;
	default:	
		return 0;
	}
}

/* ODI notes -

   template class changes
*/


