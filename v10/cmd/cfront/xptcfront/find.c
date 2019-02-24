/*ident	"@(#)ctrans:src/find.c	1.5" */
/******************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1986 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

find.c:

	name lookup and visibility checks

*******************************************************************/

#include "cfront.h"

Pname undef(Pname n, Ptable tbl, TOK f)
{
	switch (f) {
	case CCON:
		error("illegalF call: explicit call ofK %s()",n->string);
		break;
	case 0:		error("%nU",n);			break;
	case CALL:	error("UF%n called",n);		break;
	case REF:
	case DOT:	error("M%nU",n);		break;
	case ADDROF:	error("address ofU%n",n);	break;
	}

	if (tbl == gtbl) {
		Pname nn = tbl->insert(n,0);
		if (f == CALL) {
			nn->tp = new fct(defa_type,0,0);
			nn->n_sto = nn->n_scope = EXTERN;
		}
		else
			nn->tp = any_type;
		delete n;
		return nn;
	}
	
	n->n_table = tbl;
	n->tp = any_type;
	return n;
}



static int mptr;	// &C::m
static Pname me;	// name of fct requesting access to name using find_name()
static Pfct mef;	// fct requesting access to name using find_name()
Pclass tcl;		// class of original ``this''
Pclass mec;	 	// class requesting access to name using find_name()
int mex;

Ptable lcl_tbl = 0;	// handle local function reference of local const for dimen size


Pexpr find_name(Pname n, Pclass cl, Ptable tbl, int f, Pname m)
/*
	in function ``m'' find the true name for "n",
	implicitly define if undefined

	f==CALL:	n()		cl == cc->cot
	f==REF:		p->n		cl == class of *p
	f==DOT:		obj.n		cl == class of obj
	f==ADDROF:	&n		cl == cc->cot
	f==0		n   		(none of the above)

	"tbl" defines local scope (block or global)

*/
{
	if ( n == 0 ) error('i',"find_name(n==0,cl==%t,tbl==%d,f==%k,m==%n)",cl,tbl,f,m);
	Pname q = n->n_qualifier;
	char* s = n->string;
	Pexpr ee;
	DB( if(Ddebug>=3) {
		error('d',"find_name %s::%s cl %t",q?q->string:"",s,cl);
		error('d',"	f %d m %n",f,m);
		if(q&&q->tp&&q->tp->base==COBJ)
			fprintf(stderr," -- cb==%d\n",Pclass(q->tp)->c_body);
	});

	tcl = cl;

	//if (n) {
		mex = 1;
		if (me = m) {
			mef = Pfct(me->tp);
                        if (mef->base!=FCT) 
                                error('i',"mef %d %k",mef,mef->base);
			mec = mef->memof;
		}
		else {
			mef = 0;
			mec = cl;
		}
	//}
	//else
	//	mex = 0;

	if (n->base == MDOT) error('i',"find (mdot %n)",n);

	if (n->n_table) { me = 0; return n; }

//error('d',"q %n %t f %k",q,q?q->tp:0,f);
	if (q) {			// qualified name:	q::s

		if (q == sta_name) {	// explicitly global:	::s
			Pname nn = gtbl->look(s,0);
			if (nn == 0) { me = 0; return undef(n,gtbl,f); }
			nn->use();
			delete n;
			me = 0;
			return nn;
		}

		{
		  Pname aq = q; // actual q
		  while ( aq->tp->base == TYPE ) aq = Pbase(aq->tp)->b_name;
		  if (aq->tp->base != COBJ) {
		    error("Qr%nnot aCN",q);
		    me = 0;
		    return undef(n,gtbl,f);
		  }
		  q = aq;
		}
		Pclass qcl = Pclass(Pbase(q->tp)->b_name->tp);

		Pclass bcl = cl;

		if (cl==0 || f==ADDROF)
			bcl = cl = qcl; // Pclass(Pbase(q->tp)->b_name->tp);
		else {
		//	if (strcmp(q->string,cl->string)) {	// really a base?
		//		bcl = cl->is_base(q->string);
			if (strcmp(qcl->string,cl->string)) {	// really a base?
				bcl = cl->is_base(qcl->string);
				if (bcl == 0) {
					if (f==REF || f==DOT) {
						error("%s is not aBC of %t",q->string,cl);
						me = 0;
						return undef(n,cl->memtbl,7);
					}
					goto sss;
				}
				// else try in base or for static
			}
		}

		if (f == ADDROF) mptr = 1;		// &C::m
		ee = cl->find_name(s,bcl==cl?0:bcl);	// really a member?
		mptr = 0;

		if (ee == 0) {
		sss:
//error('d',"sss %k",f);
			if (f!=REF && f!=DOT) {
				// try for static member of other class:
				Pclass qcl = Pclass(Pbase(q->tp)->b_name->tp);
				mptr = 1;
				if ( cl && cl->csu == ANON ) {
					mec = (cc-1)->cot;
					ee = qcl->find_name(s,qcl);
				} else
					ee = qcl->find_name(s,qcl);
				mptr = 0;
				if (ee && ee->base==NAME) {
				//	switch (ee->tp->base) {
				//	case FCT:
				//	case OVERLOAD:
				//		break;
				//	default:
						delete n;
						me = 0;
						return ee;
				//	}
				}
			}
			error("QdN%n::%n not found in %t",q,n,cl);
			me = 0;
			return undef(n,bcl?bcl->memtbl:cl->memtbl,7);
		}

		if (ee->base==REF && ee->e1==0) {	// &C::f, no ``this''
//error('d',"ee %k %d f %k",ee->base,ee->e1,f);
			switch (f) {
			case 0:
			case CALL:	//SSS
				{	Pexpr mm = ee->mem;
					while (mm->base==REF || mm->base==MDOT) mm = mm->mem;
					if (mm->base==NAME)
						switch (mm->tp->base) {
						case FCT:
						case OVERLOAD:
							goto addrof;
						default:
							if (Pname(mm)->n_stclass == STATIC) goto addrof;
						}
				}
				error("O orOP missing forM%n",n);
					
			case ADDROF:
			addrof:
				Pexpr x = ee;
				ee = ee->mem;
				delete x;
			case REF:
			case DOT:
				break;
			default:
				error("QdN%n::%n used in nonC context",q,n);
			}
		}

		delete n;
		me = 0;
		return ee;
	}

	if (f!=DOT && f!=REF) {	// not .s or ->s: look for local, global, and member

		Pname nn = tbl->look(s,0);
		if (nn) {	// local, global, or member
			if (nn->n_table->t_name) {	// global or member
				if (cl) {		// might be member
					if ((ee = cl->find_name(s,0))==0 &&
						cl->in_class) 
					{ // nested class
						Pclass nstd_cl = cl->in_class;
						for (; nstd_cl; nstd_cl=nstd_cl->in_class) 
						{
// error( 'd', "find_name( %s ): nested class %t",s,nstd_cl ); 
							if (ee=nstd_cl->find_name(s,0)) 
							{
								if ( cl->nested_sig == 0 ) {
									error('w',"%s, accessed within nestedC%t, is visible bothGly and within enclosingC%t -- using %n (anachronism)", s, cl, nstd_cl, nn );
									ee = 0;
								}
								break;
							}
						}	
					} // nested class
				
					if ( ee ) 
					{  // class member name
						if (ee->base==REF && ee->e1==0 )
						{
							Pexpr mm = ee->mem;
							while (mm->base==REF || mm->base==MDOT) mm = mm->mem;
							if (mm->base==NAME)
								switch (mm->tp->base) {
								default:
									if (Pname(mm)->n_stclass != STATIC)
										break;
								case FCT:
								case OVERLOAD:
									delete n;
									me = 0;
									return mm;
								}
							error("O orOP missing for%n",Pref(ee)->mem);
						}
						delete n;
						me = 0;
						return ee;
					  }  // if ee
				}  // if cl
			} // else local, global, or member

			nn->use();
			delete n;
			me = 0;
			return nn;
		}

		if (cl) {
			if ( cl->csu == ANON ) {
				mec = (cc-1)->cot;
				ee = (cc-1)->cot->find_name(s,0);
			} else
				ee = cl->find_name(s,0);
			if (ee) {
			if (ee->base==REF && ee->e1==0) {
				Pexpr mm = ee->mem;
				while (mm->base==REF || mm->base==MDOT) mm = mm->mem;
				if (mm->base==NAME)
					switch (mm->tp->base) {
					default:
						if (Pname(mm)->n_stclass != STATIC)
							break;
					case FCT:
					case OVERLOAD:
						delete n;
						me = 0;
						return mm;
					
					}
				error("O orOP missing for%n",Pref(ee)->mem);
			}
				delete n;
				me = 0;
				return ee;
			}
		}

		if ( lcl_tbl && (nn=lcl_tbl->look(s,0)) ) {
			nn->use();
			delete n;
			me = 0;
			return nn;
		}

		if (nn = gtbl->look(s,0)) {	// global
			nn->use();
			delete n;
			me = 0;
			return nn;
		}

		me = 0;
		return undef(n,gtbl,f);
	}

	if (ee = cl->find_name(s,cl)) {	// .s or ->s
		delete n;
		me = 0;
		return ee;
	}

	if(!strcmp(s,cl->string)) {
		me = 0;
		return undef(n,gtbl,CCON);
	}

	me = 0;
	return undef(n,gtbl,f);
}

int classdef::check_dup(Pclass cl, TOK bb)
/*
	 see if cl is a base of this; return 0 if no clash
*/
{
	for (Pbcl b = baselist; b; b=b->next) {
		if (cl == b->bclass) {
			if (bb!=VIRTUAL) {
				if (b->base==VIRTUAL)
					error('w',"%t inaccessible because of virtual%t in%t",cl,cl,this);
				else
					error('w',"%t inaccessible because of%t in%t",cl,cl,this);
				return 1;
			}
			else if (b->base!=VIRTUAL) {
				error('w',"virtual %t inaccessible because of%t in%t",cl,cl,this);
				return 1;
			}
		}
		if (b->bclass->check_dup(cl,bb)) return 1;
	}
	return 0;
}


int Nvis;
TOK Nvirt;
TOK ppbase;

Pclass classdef::is_base(char* s)
/*
	is "s" a public base class of this?
*/
{
//error('d',"%s->is_base(%s) %k",string,s,ppbase);
	TOK pp = ppbase;
	for (Pbcl b = baselist; b; b=b->next) {
/*
		if (b->ppp!=PUBLIC
		&& cc->cot!=this
		&& (cc->nof==0 || this->has_friend(Pfct(cc->nof->tp))==0))
			ppbase = b->ppp<pp?pp:b->ppp;	// PUBLIC<PROTECTED<PRIVATE
*/
		if (strcmp(s,b->bclass->string) == 0) {
		if (b->ppp!=PUBLIC
		&& cc->cot!=this
		&& (cc->nof==0 || this->has_friend(Pfct(cc->nof->tp))==0))
			ppbase = b->ppp<pp?pp:b->ppp;	// PUBLIC<PROTECTED<PRIVATE
			Nvirt = b->base;
			return b->bclass;
		}
		else {
			Pclass bc = b->bclass->is_base(s);
			if (bc) {
		if (b->ppp!=PUBLIC
		&& cc->cot!=this
		&& (cc->nof==0 || this->has_friend(Pfct(cc->nof->tp))==0))
			ppbase = b->ppp<pp?pp:b->ppp;	// PUBLIC<PROTECTED<PRIVATE
				return bc;
			}
		}
	}
	ppbase = pp;
	return 0;
}

bit classdef::has_base(Pclass cl)
/*
	is "cl" a base of this?
*/
{
// error('d', "%t->has_base( %t ) cc->cot: %t", this, cl, cc->cot?cc->cot:0 );

	if (this == 0) return 0;
	for (Pbcl b = baselist; b; b=b->next) {
// error('d', "b: %t ppp: %k", b->bclass, b->ppp );
		if (b->bclass==cl) {
			if (b->ppp!=PUBLIC
			&& cc->cot!=this
			&& (cc->nof==0 ||
				this->has_friend(Pfct(cc->nof->tp))==0) 
			&& this->has_friend(cc->cot)==0)
				Nvis = 1;	// no standard coercion
			Nvirt = b->base;
			return 1;
		}
		if (b->bclass->has_base(cl)) return 1;
	}
	return 0;
}

int Noffset;
Pexpr Nptr;
char *Nalloc_base;
clist* vcllist;

int clist::onlist(Pclass c)
{
	for (clist* p = this; p; p = p->next)
		if (p->cl == c) return 1;
	return 0;
}

void clist::clear()
{
	if (this == 0) return;
	clist* p = next;
	while (p) {
		clist* q = p->next;
		delete p;
		p = q;
	};
	delete this;
}

Pbcl Nvbc_alloc;
int is_unique_base(Pclass cl, char* s, int offset, int in_base )
/*
	is "s" a unique base class of this?
*/
{
	int i = 0;
	static Pclass priSeen = 0;
// error('d',"is_unique_base(cl: %t, s: %s,%d,%d)",cl,s,offset,in_base);
	for (Pbcl b = cl->baselist; b; b=b->next) {
		int no = 0;
		if (b->base!=VIRTUAL)
			no = offset + b->obj_offset;
		else if (in_base)
			continue;
		if (strcmp(s,b->bclass->string) == 0) {
			Noffset = no;
			i++;
			if ((b->ppp!=PUBLIC || priSeen )
				&& (cc->cot!=cl || cc->cot != priSeen )
				&& (cc->nof==0 || cl->has_friend(Pfct(cc->nof->tp))==0)
			      		|| (priSeen && priSeen->has_friend(Pfct(cc->nof->tp))==0))
						Nvis = 1;	// no standard coercion

			if (b->base==VIRTUAL) {
				Nptr = new mdot(s,0);
				if (b->allocated == 0) {
					Nvbc_alloc = 0;
					Nalloc_base = cl->has_allocated_base(s);
				}
			}
		}
		else {
			if (b->base==VIRTUAL) {
				if (vcllist->onlist(b->bclass) )continue;
				vcllist = new clist(b->bclass,vcllist);
			}

			Pclass clscope = 0;
			if ( cc && cc->c_this ) {
				Ptype t = Pptr(cc->c_this->tp)->typ;
				clscope = Pclass(Pbase(t)->b_name->tp);
			}
// error('d', "cl: %t %d clscope: %t %d", cl, cl, clscope, clscope);
			if (b->ppp != PUBLIC && 
				cl != clscope && priSeen == 0 ) priSeen = cl;

			int ii = is_unique_base(b->bclass,s,no,1);
			if (in_base == 0) priSeen = 0;
// error('d',"base %t i %d ii %d",b->bclass,i,ii);
// error('d',"base %t %k allocated: %d", b->bclass, b->base, b->allocated);
			i += ii;
			if (ii==1 && b->base==VIRTUAL) {
			 	Nptr = new mdot(b->bclass->string,0);
				if (b->allocated == 0) {
					Nvbc_alloc = 0;
					Nalloc_base = cl->has_allocated_base(b->bclass->string);
				}
			}
		}
	}

	return i;
}

/*
int classdef::has_allocated_base(Pclass bcl)

	search the list of !first base classes for this virtual base
	space will have been allocated in !first bases for virtual bases
	declared in !first classes

	in addition bcl may bave been specified explicitly as a base

{
	int off;
	for (Pbcl l = baselist; l; l=l->next) {
		if (l->base == VIRTUAL) continue; // another non-allocated virtual base
		if (l==baselist) continue;	// first base

		Pclass bc = l->bclass;
		off = l->obj_offset;

		for (Pbcl ll = bc->baselist; ll; ll=ll->next) {
			// cannot share non-virtual base
			if (ll->base != VIRTUAL) continue;
			if (ll->bclass==bcl) return off + ll->obj_offset;
		}
	}
	return 0;
}
*/

int classdef::has_allocated_base(Pclass bcl)
/*
	search the list of base classes for this virtual base
	space will be allocated in first virtual version found.
	return offset.

	A virtual base cannot have offset 0 (its pointer at least is ahead)
*/
{
//error('d',"%t->has_allocated_base(%t) ",this,bcl);
	for (Pbcl l = baselist; l; l=l->next) {
		if (l->base==VIRTUAL
		&& l->bclass==bcl
		&& l->obj_offset)
			return l->obj_offset;

		if (l->base==VIRTUAL || l!=baselist) {
			// allocated as an object,
			// not unravelled as a set of members
			int i = l->bclass->has_allocated_base(bcl);
			if (i) return l->obj_offset + i;
		}
	}
	return 0;
}

char *classdef::has_allocated_base(char *str) 
/*
 *	str is an unallocated virtual base class of this
 *	return the name of the second or subsequent base class 
 *		containing the member ``struct str *P<str>''
 */
{
// error('d',"%t::has_allocated_base(%s) baselist: %t",this,str,baselist->bclass);
	for (Pbcl l = baselist; l; l=l->next) {
		if (l->base == VIRTUAL) {
			if ( l->allocated ) Nvbc_alloc = l; 
			continue; 
		}

		Pclass bc = l->bclass;
		for (Pbcl ll = bc->baselist; ll; ll=ll->next) {
			if (ll->base != VIRTUAL) continue;
			if (ll->allocated && 
			    strcmp( str, ll->bclass->string) == 0 )
				{ 
					if (bc == baselist->bclass) {
						// NfirstBase=1; 
						return 0;
					}
					return bc->string;
				}
		}
	}
	return 0;
}

/*
int allocated_base(Pclass cl,Pclass bcl)
{
static second;
int s2 = second;
	for (Pbcl l = cl->baselist; l; l=l->next) {
		if (l->base==VIRTUAL
		&& l->bclass==bcl
		&& l->obj_offset
		&& (second || l!=cl->baselist)) return (second=s2,1);
		int i = allocated_base(l->bclass,bcl);
		if (i) return (second=s2,1);
		second = 1;
	}
	second = s2;
	return 0;
}
*/
Pname vfct(Pclass cl, char* s)
/*
	Called for each name "s" in a vtbl for "cl"
	Find the "s" to go in the vtbl.
	The "s" that caused the vtbl entry to be created
		is found if nothing else is
*/
{
	Pname n = cl->memtbl->look(s,0);
	if (n) return n;

	for (Pbcl b = cl->baselist; b; b=b->next) {
		Pname nn = vfct(b->bclass,s);
		if (nn) {
//error('d',"nn %nn",nn);
			if (n && n!=nn) {
				Pclass ncl = Pclass(n->n_table->t_name->tp);
				Pclass nncl = Pclass(nn->n_table->t_name->tp);
//error('d',"ncl %t nncl %t",ncl,nncl);
				if (nncl->is_base(ncl->string))
					n = nn;		// use nn
			}
			else
				n = nn;
		}
	}

	return n;
}

Pexpr rptr(Ptype t, Pexpr p, int offset)
/*
	return rvalue of offset off pointer:
	(t)((char*)p+offset)
*/
{
	if ( t == 0 ) error( 'i', "rptr(), t==0 (type passed for cast)" );
	Pexpr pp = p;
//error('d',"rptr %t %d",t,offset);
	if (offset) {
		Pexpr i = new ival(offset);
	//	pp = new texpr(CAST,Pchar_type,pp);
		pp = new cast(Pchar_type,pp);
		pp = new expr(PLUS,pp,i);
	}
	pp = new cast(t,pp);
	return pp;
}
/*
Pexpr lptr(Ptype t, Pexpr p, int offset)

	return lvalue of offset off pointer:
	*(t*)((char*)p+offset)

{
	Pexpr pp = p;
	if (offset) {
		Pexpr i = new ival(offset);
	//	pp = new texpr(CAST,Pchar_type,pp);
		pp = new cast(Pchar_type,pp);
		pp = new expr(PLUS,pp,i);
	}
	pp = new cast(t->addrof(),pp);
	pp = new expr(DEREF,pp,0);
	pp->tp = t;
	return pp;
}
*/
int friend_check(Pclass start,Pclass stop, Pfct f)
/*
	is `f' a friend of a class between `start' and `stop'
	in a class DAG?
*/
{
//error('d',"friend_check(%t,%t)",start,stop);
	
	if (start->has_friend(f)) return 1;
	if (stop == start) return 0;
	for (Pbcl b = start->baselist; b; b = b->next) {
		if (b->bclass->has_friend(f)) return 1;
		if (friend_check(b->bclass,stop,f)) return 1;
	}
	return 0;
}

bit classdef::has_friend(Pfct f)
/*
	does this class have function "f" as its friend?
*/
{
// error('d',"%t->has_friend(%t) %d %d ",this,f,f,f->base);

	for (Plist l=friend_list; l; l=l->l) {
		Pname fr = l->f;
		Ptype frt = fr->tp;
//error('d',"frt %t %d %d",frt,frt,frt->base);
		switch (frt->base) {
		case FCT:
			if (f == frt) return 1;
			break;
		case OVERLOAD:
			l->f = fr = Pgen(frt)->fct_list->f;	// first fct
			if (fr->tp == f) return 1;
		case CLASS:
			break;
		default:
			error('i',"bad friend %k",fr->tp->base);
		}
	}
	return 0;
}

bit classdef::has_friend(Pclass cl)
/*
	does this class have class "cl" as its friend?
*/
{
// error('d',"%t->has_friend(%t) ",this,cl);

	for (Plist l=friend_list; l; l=l->l) {
		Pname fr = l->f;
		Ptype frt = fr->tp;
		switch (frt->base) {
		case CLASS:
// error('d',"class friend: %t", frt);
			if ((frt == cl) || cl->same_class(Pclass(frt))) 
				return 1;
		case FCT:
		case OVERLOAD:
			break;
		default:
			{error('i',"bad friend %k",frt->base);}
		}
	}
	return 0;
}

Pname find_virtual(Pclass cl, Pname s)
/*
	does ``cl'' have a virtual function ``s'' in some base class
*/
{
	for (Pbcl b = cl->baselist; b; b = b->next) {
		Pclass bcl = b->bclass;
		Pname n;
		if (n = bcl->memtbl->look(s->string,0)) {
// error('d', "find_virtual: n: %d base: %k", n, n->base );
			if ( n->base == PUBLIC ) // x::foo;
				continue;
			Pfct f = Pfct(n->tp);
			if (f->base == OVERLOAD) {
				for (Plist gl=Pgen(f)->fct_list; gl; gl=gl->l) {
					n = gl->f;
					// if (f != s->tp) continue;
					// Replaced by:
					if (n->tp->check(s->tp,VIRTUAL)) continue;
					if (Pfct(n->tp)->f_virtual) return n;
				}
			}
			// else if (f->f_virtual)
			else if (f->f_virtual && n->tp->check(s->tp,VIRTUAL)==0)
				return n;
		}
		else if (n = find_virtual(bcl,s))
			return n;
	}
	return 0;
}

Pname dummy_fct;

static int 
is_accessible(Pname n, Pclass this_class, bit noCdcl = 0)
// 0 means n is not accessible to this_class
{
// error('d',"is_accessible(%n,%t)", n, this_class
// error('d',"%n: %k mef %t",n,n->n_protect,mef);
// error('d',"		this_class %d %t",this_class,this_class);
// error('d',"		mec  %d %t",mec,mec);
// error('d',"		tcl  %d %t",tcl,tcl);
// error('d',"mec->has_base(this_class) %d",mec?mec->has_base(this_class):0);
// error('d',"tcl->has_base(mec) %d",tcl?tcl->has_base(mec):0);
// error('d',"tcl->has_base(this_class) %d",tcl?tcl->has_base(this_class):0);
// error('d',"tcl->has_friend(mef) %d",tcl?tcl->has_friend(mef):0);

	if (this_class==mec || 
		(mec && this_class->has_friend(mec)) || 
		(mef && this_class->has_friend(mef)))
			return 1;
	
	if ( n->n_protect && tcl ) {
		if ( mec ) 
		{
			if ( tcl->has_friend(mec) ||
				( mec->has_base(this_class) && 
					(tcl==mec || tcl->has_base(mec))))
						return 1;
		}	

		if ( mef ) 
		{
			if ( tcl->has_base(this_class) &&
				// && tcl->has_friend(mef))
				friend_check(tcl,this_class,mef))
					return 1;
		}
	}

	// call from check_visibility
	if ( noCdcl ) return 0; 
	
	if ( Cdcl && Cdcl->base == NAME && 
		Cdcl->n_stclass == STATIC && 
		Cdcl->n_initializer && 
		Cdcl->n_qualifier ) 
	{
		Pbase bn = Pbase(Cdcl->n_qualifier->tp);
		Pclass cl = Pclass(bn->b_name->tp);

		if ( cl == this_class ||
			(n->n_protect && cl->has_base(this_class)) || 
			cl->has_friend(this_class))
				return 1;
	}

	return 0;
}

Pexpr classdef::find_name(char* s, Pclass cl, int access_only)
/*
	look for "s" in "this" class and its base classes
	if (cl)
		accept only a member of "cl" or "cl"'s base classes
		(that is cl::s was seen)

	complicated by hiding rules: private, protected, friend
	
*/
{
// error('d',"%s->find_name(%s,%s) memtbl %d",string,s,cl?cl->string:"",memtbl);
	Pname n = memtbl->look(s,0);

	if (n) {
		if (n->tp) {
			switch (n->tp->base) {
			case OVERLOAD:
				break;
			case FCT:
				if (Pfct(n->tp)->f_virtual==0) {
					if (n->n_dcl_printed==0) n->dcl_print(0);
					break;
				}
			default:
                                 if (c_body==1) dcl_print(0);
			}
		}

		if (n->base == PUBLIC) {
			if (mex && n->n_scope==0) {
				if ( is_accessible(n,this) == 0 ) 
				{
					if ( me == dummy_fct )
						error("G scope cannot access%n: %sM",n,n->n_protect?"protected":"private");
					else error("%n cannot access%n: %sM",me,n,n->n_protect?"protected":"private");
					mex = 0;	// suppress further error messages
				}
			}
			mex = 0; // don't have find_in_base complain about n
			return find_in_base(s, cl);
		}

		if (cl==0 || cl==this) {
			if (mptr==0
			&& n->n_stclass!=STATIC
			&& n->n_stclass!=ENUM) {
				Ptype t = n->tp;

// error('d',"cl %d this %d",cl,this);

				if (mex && n->n_scope==0 && n->tp->base!=OVERLOAD) {
					if ( is_accessible(n,this)==0 )
					{
						if ( me == dummy_fct )
							error("G scope cannot access%n: %sM",n,n->n_protect?"protected":"private");
						else error("%n cannot access%n: %sM",me,n,n->n_protect?"protected":"private");
						mex = 0;	// suppress further error messages
					}
				}

				Pname th = cc->c_this;
				Pexpr r = new ref(REF,th,n);

				if ( access_only == 0 ) {
					if (th) th->use();
					n->use();
				}
				r->tp = t;
				return r;
			}

			if (mex && n->n_scope==0 && n->tp->base!=OVERLOAD) {
				if ( is_accessible(n,this)==0 )
				{
					if ( me == dummy_fct )
						error("G scope cannot access%n: %sM",n,n->n_protect?"protected":"private");
					else error("%n cannot access%n: %sM",me,n,n->n_protect?"protected":"private");
					mex = 0;	// suppress further error messages
				}
			}
			n->use();
			return n;
		}
	}

	if ((cl==0 || cl==this)	// not qualified to a base class
	&& csu!=UNION
	&& csu!=ANON
	&& strcmp(s,"__as")==0) { // assignment is special: you cannot inherit it
		if (baselist==0
		|| (baselist && baselist->bclass->obj_size!=obj_size)) {
			Pname cn = ktbl->look(string,0);
			if (cn->tp->base == COBJ) cn = Pbase(cn->tp)->b_name;
			Pname x = gtbl->look("__as",0);
			if (x) {	// what if there is an (illegal) global assignment operation?
					// hack hack don't declare global assignment operations!
				Pfct f = Pfct(x->tp);
				if (f->base == FCT) {
					Pptr r = f->argtype->tp->is_ref();
					if (r) {
						Pname cnn = r->typ->is_cl_obj();
						if (cnn && cn==cnn) return 0;
					}
				} else {
					for (Plist gl = Pgen(f)->fct_list; gl; gl=gl->l) {
						Pptr r = Pfct(gl->f)->argtype->tp->is_ref();
						if (r) {
							Pname cnn = r->typ->is_cl_obj();
							if (cnn &&cn==cnn) return 0;
						}
					}
				}
			}
			return make_assignment(cn) ? find_name(s,cl) : 0;
		}
	}

	return find_in_base(s, cl);
}

static Pclass rootClass;
static Pbcl pubVClass;
static struct PendingMessage { 
    Pbcl bc;
    Pname mf;
    char *nm;
} *pM;

Pexpr classdef::find_in_base(char* s, Pclass cl)
{
	Pbcl bc = 0;
	Pexpr e = 0;

// error('d',"%s->find_in_base(%s %s)",string,s,cl?cl->string:"");

	if ( me == 0 ) mef = 0;
	if (rootClass == 0) rootClass = this;
	for (Pbcl b=baselist; b; b=b->next) {
		Pclass ccl = cl==this?0:cl;
// error('d',"try %t %s %k",b->bclass,b->bclass?b->bclass->string:"?", b->base);
		Pexpr ee = b->bclass->find_name(s,ccl);

		if (ee) {
			if (c_body == 1) {	// look for first use (through this)
				if (b!=baselist || b->base==VIRTUAL)
					dcl_print(0);
				else {
					Pexpr ex = ee;
					while ((ex->base==MDOT && ex->i1==1)
					|| (ex->base==REF && ex->e1==cc->c_this)) ex = ex->mem;
					switch (ex->tp->base) {
					case OVERLOAD:
						break;
					case FCT:
						if (Pfct(ex->tp)->f_virtual==0) break;
					default:
						dcl_print(0);
					}
				}
			}

			if (e) {
// error( 'd', "find_in_base: b( %s ): %k %k ",b->bclass->string,b->base,b->ppp);
				Pexpr ex = e;
				int evb = 0; 	// number of vbase indirections
				int eb = 0;

// note that this does not catch enum members
				while (ex->base == MDOT
				|| (ex->base==REF && ex->e1==cc->c_this)) {
					if (ex->base==MDOT) evb += int(ex->i1);
					ex = ex->mem;
				}

				Pexpr eex = ee;
				int eevb = b->base==VIRTUAL; 	// number of vbase indirections (incl. possibly this one)
				int eeb = b->base!=VIRTUAL;

				while (eex->base == MDOT
				|| (eex->base==REF && eex->e1==cc->c_this)) {
					if (eex->base==MDOT) eevb += int(eex->i1);
					eex = eex->mem;
				}

// error('d', "find_in_base: ex: %k eex: %k, eevb: %d evb: %d", ex->base, eex->base, eevb, evb );
// relying on simple counts for sub-object identification isn't good enough

				if (ex != eex)	{
// error('d',"ex %k tp %k eex %k tp %k", ex->base, ex->tp->base, eex->base, eex->tp->base );
				//	if (!mqua) {
						Pclass ocl, ncl;
						if ( ex->tp->base == FCT ) 
     							ocl = Pfct(ex->tp)->memof;
						else 
						if ( ex->tp->base == OVERLOAD )
							ocl = Pfct(Pgen(ex->tp)->fct_list->f->tp)->memof;
						else ocl = Pclass(ex->n_table->t_name->tp);
//						else ocl = 0;

						if ( eex->tp->base == FCT ) 
     							ncl = Pfct(eex->tp)->memof;
						else 
						if ( eex->tp->base == OVERLOAD )
							ncl = Pfct(Pgen(eex->tp)->fct_list->f->tp)->memof;
						 else ncl = Pclass(eex->n_table->t_name->tp);
//						else ncl = 0;

//						Pclass ocl = Pfct(ex->tp)->memof;
//						Pclass ncl = Pfct(eex->tp)->memof;
						int eb = ocl?ocl->has_base(ncl):0;
						int eeb = ncl?ncl->has_base(ocl):0;

//error('d',"eb %d eeb %d evb %d eevb %d",eb,eeb,evb,eevb);
						if (eb==0 && eeb==0) {
							// different
							error("ambiguous%n and%n",ex,eex);
							break;
						}
						else if (eb) {	// ex dominates
							if (eevb<evb) error("ambiguous%n and%n (different sub-objects)",ex,eex);
						}
						else {	// eex dominates
							e = ee;
							bc = b;
							if (evb<eevb) error("ambiguous%n and%n (different sub-objects)",ex,eex);
						}
						if (evb==0 && eevb==0) {
							error("ambiguous%n and%n (different sub-objects)",ex,eex);
						}
				//	}
				//	else {
				//		// different
				//		error("ambiguous%n and%n",ex,eex);
				//		break;
				//	}
				}
				else if (ex->base==NAME
				&& (Pname(ex)->n_evaluated || Pname(ex)->n_sto==EXTERN)) {
//error('d',"enum");
				}
				else if (evb==0 && eevb==0) {
//error('d',"e %k",e->base);
					// no virtual base => different
					error("ambiguous%n and%n (no virtualB)",ex,eex);
					break;
				}
				else if ((evb && eevb==0) || (eevb && evb==0)) {
					// only one virtual base => different
					error("ambiguous%n and%n (one not in virtualB)",ex,eex);
					break;
				}
			}
			else {
				e = ee;
				bc = b;
			}
		}
	}

	if (rootClass == this ) {
    		if ( pM ) {
			// deferred until all base classes of ``this'' examined
			error("%n cannot access %s: %s is a privateBC",pM->mf,pM->nm,pM->bc->bclass->string);
        		delete pM; pM=0;
    		}
    		rootClass = 0;
    		pubVClass = 0;
	}

	if (e == 0) return 0;

	if (mex) {
		if ( bc->ppp==PRIVATE ) {	// private base
			if (this==mec || 
                           (mec && has_friend(mec)) || 
 			   (mef && has_friend(mef)))
				;
			else 
			if ( bc->base == VIRTUAL &&
     			     rootClass != this ) { 
				// if one instance is public, it dominates
				if ( pubVClass == 0 ||
                                     strcmp(pubVClass->bclass->string,bc->bclass->string)) {
					pM = (PendingMessage *) new char[sizeof(*pM)];
					pM->bc = bc; pM->mf = me; pM->nm = s;
				}
			}
			else {
				if ( me == dummy_fct )
					error("G scope cannot access %s: %s is a privateBC",s,bc->bclass->string);
				else error("%n cannot access %s: %s is a privateBC",me,s,bc->bclass->string);
				mex = 0;
			}
		}
		else { // public base class
		if ( bc->base == VIRTUAL ) {
			if ( pM && 
			     strcmp(bc->bclass->string,pM->bc->bclass->string)==0 ) {
				delete pM;
				pM=0;
				pubVClass = bc; 
				} // previous private member becomes public
			else pubVClass = bc; // ignore subsequent private instances
	 		}
		}
	}

	if (e->base==NAME
	&& Pname(e)->n_stclass==STATIC) {	// static member
		Pname(e)->use();
		return e;
	}
	if (e->base != NAME)
	if (bc->base == VIRTUAL) {	// this->mem => this->Pbclass->mem
		e->mem = new mdot(bc->bclass->string,e->mem);
		e->mem->i1 = 1;
		e->mem->tp = e->mem->mem->tp;
	}
	else if (bc!=baselist) {	// not first base
		if (e->e1 == cc->c_this) {	// this->mem => this->Obcl.mem
			e->mem = new mdot(bc->bclass->string,e->mem);
			e->mem->tp = e->mem->mem->tp;
		}
		else {	// this->p->mem => this->Obcl.p->mem
			Pexpr ee = e;
			while (ee->e1->base == REF) ee = ee->e1;
			ee->mem = new mdot(bc->bclass->string,ee->mem);
			ee->mem->tp = ee->mem->mem->tp;	
		}
	}

	return e;
}

int has_virt(Pclass cl)
{
	if (cl->virt_count) return 1;
	for (Pbcl b = cl->baselist; b; b = b->next)
		if (b->bclass->virt_count || has_virt(b->bclass)) return 1;
	return 0;
}

Pname find_vptr(Pclass cl)
/*
	find virtual function table
	in memtbl or memtbl of ``first bases''
*/
{
//error('d',"find_vptr %t",cl);
	while (cl) {
		Pname vp = cl->memtbl->look("__vptr",0);
		if (vp) return vp;
		Pbcl b = cl->baselist;
		cl = 0;
		for (; b; b = b->next)
			if (b->base == NAME) {
				cl = b->bclass;
				break;
			}
	}
//error('d',"return 0");
	return 0;
}

void make_dummy()
/* a function with no special privileges */
{
        Pname x = new name(".." /*"__static_initializer"*/);
        x->tp = new fct(Pvoid_type,0,1);
        dummy_fct = x->dcl(gtbl,EXTERN);
//	dummy_fct->string = "";
        delete x;
}

void check_visibility(Pname n, Pname q, Pclass cl, Ptable tbl, Pname fn)
/*
	"fn" calls "n" a member function of "cl"
	fn can be zero (for functions called in arguments to static constructors) 
*/
{
// error('d',"check_visibility(%n, %t, %d, %n)",n,cl,tbl,fn);
	if (fn==0) {
                if (dummy_fct == 0) make_dummy();
                fn = dummy_fct;
	}
	Pname nn = new name;
        char* s = n->n_gen_fct_name;    // overloaded name
	nn->string = s?s:n->string;
	nn->n_qualifier = q;
	Pname nx = Pname(find_name(nn,cl,tbl,REF,fn)); // nn deleted by find_name
	if (nx->tp->base != OVERLOAD) return;

	// can we get here?

	// overloaded not checked by find_name()
	// (since it looks for NAMEs not functions)

	for (Plist gl=Pgen(nx->tp)->fct_list; gl; gl=gl->l) {
		Pname nn = gl->f;
		if (n == nn) {
			if (nn->n_scope) return;	// public member

			Pfct omef = mef;
			Pclass omec = mec;
			Pclass otcl = tcl;

			Pclass ncl = Pclass(nn->n_table->t_name->tp);
			Pname fncn = fn->n_table->t_name;

			mec = fncn?Pclass(fncn->tp):0; 
			Pfct f = mef = Pfct(fn->tp);
			tcl = cl;

			int ok = is_accessible( n, ncl, 1 );

			// restore global values
			mef = omef; mec = omec; tcl = otcl;

			if ( ok ) return;

			if ( Cdcl && Cdcl->base == NAME && 
				Cdcl->n_stclass == STATIC && 
				Cdcl->n_initializer && 
				Cdcl->n_qualifier )
			{
				Pbase bn = Pbase(Cdcl->n_qualifier->tp);
				Pclass ccl = Pclass(bn->b_name->tp);

				if ( ccl == cl || ccl->has_friend(f) ||
          				(n->n_protect && ccl->has_base(ncl)))
						return;
			}

			if ( fn == dummy_fct )
				error("G scope cannot access%n: %sM",nn,nn->n_protect?"protected":"private");
			else error("%n cannot access%n: %sM",fn,nn,nn->n_protect?"protected":"private");
			return;
		}
	}
	error('i',"visibility check failed");
}
