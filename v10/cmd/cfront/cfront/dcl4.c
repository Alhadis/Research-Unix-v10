/*ident	"@(#)ctrans:src/dcl4.c	1.4.0.1" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

dcl4.c:
	Declaration of class and enum

*************************************************************************/

#include "cfront.h"
#include "size.h"

Pname find_vptr(Pclass);
void fix_args(Pfct f, Pclass cl)
/*
	This function is used to cope with the case where cl::cl(cl&) is
	declared AFTER f has been declared
	set n_xref bit for f
*/
{
	for (Pname a = f->argtype; a; a = a->n_list) {
		Pname n = a->tp->is_cl_obj();
		if (n && Pclass(n->tp)==cl) a->n_xref = 1;
	}
}

Pname merge_conv(Pname c1, Pname c2)
{
	if (c1 == 0) return c2;
	if (c2 == 0) return c1;
	if (c1 == c2) return c1;
	error('s',"cannot merge lists of conversion functions");
	return c1;
}

static Eppp;

char* get_classname(char* s)
/*
	retrieve the outermost class name in a vtable name
*/
{
//error('d',"get_classname(%s)",s);
	char* s1 = 0;

	while (*s) {
		s1 = s;
		for ( ; s[0] && (s[0] != '_' || s[1] && s[1] != '_'); s++);
		if (*s) s += 2;	// bypass "__"
	}

	return s1;
}

char* drop_classname(char* s)
/*
	retrieve all but the outermost class name in a vtable name
*/
{
//error('d',"drop_classname(%s)",s);
	char* r = new char[strlen(s)+1];
	sprintf(r,s);
	s = r;

	char* s1 = s;
	while (*s) {
		for ( ; s[0] && (s[0] != '_' || s[1] && s[1] != '_'); s++);
		if (*s) {
			s1 = s;
			s += 2;	// bypass "__"
		}
	}

	*s1 = '\0';

	return (*r) ? r : 0;
}
		
Pbcl classdef::get_base( char *s )
/*
	Find the base class whose name matches the argument
*/
{
//error('d',"%t::get_base(%s)",this,s);
	for (Pbcl b = baselist; b; b = b->next) {
		for ( char *s1 = s, *s2 = b->bclass->string;
		     *s1 && *s2 && *s1 == *s2;
		     s1++, s2++);
		
		if (!(*s1 || *s2))
			break;
	}

	return b;
}

int classdef::get_offset(char* s)
/*
	Get offset represented by string as viewed from "this"
*/
{
//error('d',"%t::get_offset(%s)",this,s);
	if (!s) return 0;

	Pbcl b = get_base(get_classname(s));

	return b->obj_offset + b->bclass->get_offset(drop_classname(s));
}

char* vtbl_str(char* s1, char* s2)
/*
	combine two pieces of a vtbl name
*/
{
//error('d',"vtbl_str(%s,%s)",s1,s2);
	char* s3;
	if (s1)
		if (s2) {
			s3 = new char[strlen(s1)+strlen(s2)+3];
			sprintf(s3,"%s__%s",s1,s2);
			return s3;
		}
		else
			return s1;
	else
		return s2;
}

void classdef::add_vtbl(velem* v, char* s, bit virt_flag)
/*
	add vtbl to virt_list
*/
{
// error('d',"%t->add_vtbl(%d,%s)",this,v,s);
	Pvirt vtab = new virt(this, v, s, virt_flag);

	if (virt_flag) has_vvtab = 1;

	if (!virt_list) {
		virt_list = vtab;
		return;
	}

	// If conficting vtable entries are made because of
	// a virtual base class, must be considered an error.
	for (Pvirt vt = virt_list; vt; vt = vt->next )
//		if (strcmp(vt->string,s)==0) {
                if (vt->string && strcmp(vt->string,s)==0) {
			velem* ivec = vt->virt_init;
			Pname on = ivec[0].n;
			Pname nn = v[0].n;
			Pclass ocl,ncl;
			for (int i=0; on && nn; i++,on=ivec[i].n,nn=v[i].n) {
				ocl = Pfct(on->tp)->memof;
				ncl = Pfct(nn->tp)->memof;
				if (on != nn)
					if (!ocl->has_base(ncl))
						if (!ncl->has_base(ocl))
							break;
						else {
							ivec[i].n = nn;
							ivec[i].offset = v[i].offset;
						}
			}
			if (on || nn)
				error("virtualB: ambiguous%n and%n", on, nn);
			return;
		}

	vtab->next = virt_list->next;
	virt_list->next = vtab;
}

static int Voffset;	// cope with offsets of virtuals in deep nests
int vcounter;
static vmax;
const vpChunk = 32;

int classdef::do_virtuals(Pvirt vtab, char* str, int leftmost, bit virt_flag)
/*
	make vtbl for b in "this"
	match up virtuals and assign virtual indices for the base or delegate "bcl"
	first base class shares ``this'' and vtbl with this class
*/
{
	if (vmax<vcounter) vmax = vcounter;
        int vpsz = (vmax+vcounter)/vpChunk+1;   // fragmentation prevention
        vpsz *= vpChunk;
	Pname* vp = new Pname[vpsz];
	velem* ivec = vtab ? vtab->virt_init : 0;
	int vo = Voffset;
	int vc = 0;
	int changed = 0;

// error('d',"%t->do_virtuals(%d,%s) voffset %d",this,vtab,str,Voffset);
// error('d',"virt_count %d vpsz %d vcounter %d",virt_count,vpsz,vcounter);

if (ivec) {	// vtbl replacement for ivec

	if (vtab->is_vbase) {
		str = 0;
		Voffset = get_offset(vtab->string);
	}
	else
		Voffset = Voffset + vtab->vclass->get_offset(vtab->string);
	Pname vn;
	for (int i=0; vn=ivec[i].n; i++) {

	/*	go through virtual table's list of virtuals:
		first see if the function is simply inherited
		if not, check for a match
		if not, then add as new
	*/
	//	Pname vn = ivec[i];
                if ( i >= vpsz ) { // resize vp vector
                        int tvpsz = vpsz + vpChunk;
                        Pname *tvp = new Pname[ tvpsz ];
                        for ( int j = 0; j < i; ++j )
                                tvp[ j ] = vp[ j ];
                        delete [vpsz] vp;
                        vp = tvp;
//error( 'd',"resizing: i: %d vpsz: %d tvpsz: %d", i, vpsz, tvpsz );
                        vpsz = tvpsz;
                }

		char* s = Pchar(vn->n_tbl_list);
		Pname n = memtbl->look(s?s:vn->string, 0);

// error('d',"vn %n %s n %n %d",vn,s,n,Voffset);
// error('d',"n %n %k", n, n?n->base:0 );
		if (n == 0 || 				// FCT + FCT
			n->base == PUBLIC ) {		// base::FCT
		inher:			// inherit
// 			if (vn->n_initializer) error("cannot inherit pure virtualF%n",vn);
                        if (vn->n_initializer) c_abstract = 1;
			vp[i] = vn;
                        if ( ivec[i].offset && vtab->is_vbase )
                                vp[i]->n_offset = Voffset - vo;
                        else
                                vp[i]->n_offset = ivec[i].offset;
			continue;
		}

		Pfct f = Pfct(n->tp);
// error ('d', "f %d", f );
		if (f == 0 ) continue;
// error('d',"f %t %d",f,f->f_virtual);

		if (s && f->base==OVERLOAD) {	// OVERLOAD + OVERLOAD
			// vn is overloaded and s is its name
			for (Plist gl=Pgen(f)->fct_list; gl; gl=gl->l)
				if (gl->f == vn)  goto inher;
		}

		Pfct vnf = Pfct(vn->tp);
						// re-define?
		switch (f->base) {
		default:
			error('w',&n->where,"%n hides virtual%n",n,vn);
			vp[i] = vn;	// not a new overloaded: inherit 
			if ( ivec[i].offset && vtab->is_vbase )
       				vp[i]->n_offset = Voffset - vo;
			else
       				vp[i]->n_offset = ivec[i].offset;
			break;
		case FCT:			// derived::FCT
		{
			vhack = 1;
			if (vnf->check(f,VIRTUAL) == 0) { // derived::FCT match base::FCT
// error('d',"vnf1 %t f %t vcheck %d",vnf,f,Vcheckerror);
				// VTOK: virtual, but no index assigned
				// you can only inherit an index from your first base
				if (Vcheckerror) error("bad virtualT match for %n",vn);
				if (f->f_virtual==VTOK) f->f_virtual = i+1;
				vp[i] = n;
				vp[i]->n_offset = Voffset;
				changed = 1;
			}
			else {
// error('d',"vnf2 %t f %t vcheck %d",vnf,f,Vcheckerror);
				if (Vcheckerror)
					error("bad virtualT match for %n",vn);
				else
					switch (f->f_virtual) {
						case 0:
						case VTOK:
							error('w',&n->where,"%n hides virtual%n",n,vn);
					}
				vp[i] = vn;	// not a new overloaded: inherit
                                if ( ivec[i].offset && vtab->is_vbase )
                                        vp[i]->n_offset = Voffset - vo;
                                else
                                        vp[i]->n_offset = ivec[i].offset;
			}
			vhack = 0;
			break;
		}
		case OVERLOAD:			// derived::OVERLOAD
		{				
			vhack = 1;
			int hit = 0;
			for (Plist gl=Pgen(f)->fct_list; gl; gl=gl->l) {
				// try each fct from derived class
				Pname fn = gl->f;
				Pfct f = Pfct(fn->tp);
			
				if (f->check(vnf,VIRTUAL) == 0) {	// derived::FCT
					if (Vcheckerror) error("bad virtualT match for %n",vn);
					if  (f->f_virtual==VTOK) f->f_virtual = i+1;
					vp[i] = fn;
					vp[i]->n_offset = Voffset;
					changed = 1;
					goto found;
				}
				else {
					if (Vcheckerror) error("bad virtualT match for %n",vn);
				}
				if (Vcheckerror == 0)
					switch (f->f_virtual) {
						case 0:
						case VTOK:
							hit = 1;
				}
	
			}

			if (hit) 
				error('w',&n->where,"%n hides virtual%n ofT %t",n,vn,vn->tp);

			vp[i] = vn;	// not a new overloaded: inherit
                        if ( ivec[i].offset && vtab->is_vbase )
                                vp[i]->n_offset = Voffset - vo;
                        else
                                vp[i]->n_offset = ivec[i].offset;
		found:
			vhack = 0;
			break;
		}
		}
	}

	Voffset = vo;
	vc = i;
}

// error( 'd', "do_virtuals: out of loop: vc: %d vpsz: %d changed: %d", vc, vpsz,changed );

	if (leftmost) {
		/*
			add new virtuals:
			`VTOK' marks ``new virtual, no index assigned''.
			You can only be new once (no base or first base).
		*/
		int i;
		for (Pname nn=memtbl->get_mem(i=1); nn; nn=memtbl->get_mem(++i) ) {
			Pfct f = Pfct(nn->tp);

                	if ( vc >= vpsz ) { // resize vp vector
                        	int tvpsz = vpsz + vpChunk;
                        	Pname *tvp = new Pname[ tvpsz ];
                        	for ( int j = 0; j < vc; ++j )
                                	tvp[ j ] = vp[ j ];
                        	delete [vpsz] vp;
                        	vp = tvp;
                        	vpsz = tvpsz;
                	}

// error('d',"f %n %t",nn,f);
                        if (f)
                        switch (f->base) {
			case FCT:
//error('d',"fv %d",f->f_virtual);
				if (f->f_virtual == VTOK) {
					// declared virtual, or
					// virtual in some base
					f->f_virtual = ++vc;
					vp[f->f_virtual-1] = nn;
                                        vp[f->f_virtual-1]->n_offset = 0;
					f->f_vdef = 1;
					changed = 2;
				}
				break;
			case OVERLOAD:
			{	for (Plist gl=Pgen(f)->fct_list; gl; gl=gl->l) {
					Pname fn = gl->f;
					Pfct f = Pfct(fn->tp);

 
                 		if ( vc >= vpsz ) { // resize vp vector
                         		int tvpsz = vpsz + vpChunk;
                         		Pname *tvp = new Pname[ tvpsz ];
                         		for ( int j = 0; j < vc; ++j ) {
                                 		tvp[ j ] = vp[ j ];
 					}
                         		delete [vpsz] vp;
                         		vp = tvp;
                         		vpsz = tvpsz;
                 		}
 

					if (f->f_virtual == VTOK) {
						f->f_virtual = ++vc;
						vp[f->f_virtual-1] = fn;
						vp[f->f_virtual-1]->n_offset = 0;
						f->f_vdef = 1;
						changed = 2;
					}
				}
				break;
			}
			}
		}
// error('d',"%s changed %d has_vvtab %d",string,changed,has_vvtab);
// error('d',"vc %d vpsz %d",vc,vpsz);
		virt_count = 0;
		if (changed)
			virt_count = vc;
		else if (has_vvtab) {
			virt_merge = 1;
			if (vc && vtab->is_vbase)
				leftmost = 0;
		}
	}

// error('d',"vc %d ch %d vp[%d] virt_count %d",vc,changed,vpsz,virt_count);
	if (changed || !leftmost) {
		// vc==0 if all explicit virtuals in fact were declared in base
		velem* v = new velem[vc+1];

		for (int i=0; i<vc; i++) {
			v[i].n = vp[i];
			v[i].offset = vp[i]->n_offset;
		}
		v[vc].n = 0;

		if (leftmost)
			add_vtbl(v,0,0);
		else
			add_vtbl(v,vtbl_str(vtab->string,str),virt_flag||vtab->is_vbase);
		delete vp;
		vcounter = 0;
		return 1;
	}

	delete vp;
	vcounter = 0;
	return 0;
}

int classdef::all_virt(Pclass bcl, char* s, int leftmost, bit virt_flag)
{
	int i = 0;
	if (bcl->virt_count) {
		for (Pvirt blist = bcl->virt_list; blist; blist = blist->next) {
			// if (has_base(blist->vclass))
			//	if (blist->vclass==bcl
			//		// bcl is virtual in both
			//	|| ((Nvirt=0,blist->vclass->has_base(bcl)) && Nvirt==VIRTUAL
			//		&& (Nvirt=0,has_base(bcl)) && Nvirt==VIRTUAL
			//	   )
			//	)
			if (virt_merge && !(virt_flag || blist->is_vbase))
				continue;

			i += do_virtuals(blist, s, leftmost, virt_flag);
			if (!i && leftmost && !virt_merge)
				return 0;

			leftmost = 0;
		}
	}

	// finding virt_list stops recursive step
	// if vtables found and updated, return number
	if (i) return i;

	for (Pbcl b = bcl->baselist; b; b = b->next) {
	//	if (b->base==PTR) continue;
		if (b->promoted) continue;
// error('d',"b %t vl %d bl %d",b->bclass,b->bclass->virt_list,b->bclass->baselist);
		if (leftmost && b->base == VIRTUAL) {
			i += do_virtuals(0, 0, 1, 0);
			if (!i && !virt_merge)
				return 0;
			leftmost = 0;
		}
		int vo = Voffset;
		Voffset += b->obj_offset;
// error('d',"offset %t %d",b->bclass,Voffset);

		if (b->base==VIRTUAL)
			i += all_virt(b->bclass, b->bclass->string, leftmost, 1);
		else
			i += all_virt(b->bclass, vtbl_str(b->bclass->string,s), leftmost, virt_flag);
		if (!i && leftmost && !virt_merge)
			return 0;

		Voffset = vo;
		leftmost = 0;
	}

	// if recursion updated vtables, return number
	if (i) return i;

	// no vtables updated in recursion
	// look for new virtuals
	if (leftmost)
		return do_virtuals(0, 0, 1, 0);
	else
		return 0;
}

Pexpr copy_obj(Pexpr l, Pexpr r, int sz)
/*
	generate:
	struct _s { char[sz]; };
	*(struct _s*)this->m = *(struct _s*)arg.mem;
*/
{
	if ( !sz ) sz = 1;
//error('d',"copy(%d)",sz);
	char* s = make_name('S');
	fprintf(out_file,"struct %s { char v[%d]; };\n",s,sz);
	Pname n = new name(s);
	Ptype t = new basetype(COBJ,n);
	t = new ptr(PTR,t);

	l = new texpr(CAST,t,l);//new cast(t,l);
	l = l->contents();

	r = new texpr(CAST,t,r);//new cast(t,r);
	r = r->contents();

	return new expr(ASSIGN,l,r);
}

/*
Pname make_default_ctor(Pclass cl)
{
//error('d',"make_ctor %t",cl);
	Pname cn = ktbl->look(cl->string,0);
	if (cn) cn = Pbase(cn->tp)->b_name;
	cc->stack();
	cc->not = cn;
	cc->cot = cl;
	Pname fn = new name(cl->string);
	Pfct f = new fct(defa_type,0,1);
	fn->tp = f;
	fn->n_oper = TNAME;
//	fn->n_sto = STATIC;

	Pfct(f)->f_inline = 1;
	f->body = new block(curloc,0,0);

	Pname nn = fn->dcl(cl->memtbl,PUBLIC);
	delete fn;

	cc->unstack();
	nn->simpl();
	if (debug_opt) nn->dcl_print(0);
//error('d',"make_ctor->");
	return nn;
}

Pname make_default_dtor(Pclass cl)
{
//error('d',"make_dtor %t",cl);
	Pname cn = ktbl->look(cl->string,0);
	if (cn) cn = Pbase(cn->tp)->b_name;
	cc->stack();
	cc->not = cn;
	cc->cot = cl;
	Pname fn = new name(cl->string);
	Pfct f = new fct(defa_type,0,1);
	fn->tp = f;
	fn->n_oper = DTOR;
//	fn->n_sto = STATIC;

	Pfct(f)->f_inline = 1;
	f->body = new block(curloc,0,0);

	Pname nn = fn->dcl(cl->memtbl,PUBLIC);
	delete fn;

	cc->unstack();
	nn->simpl();
	if (debug_opt) nn->dcl_print(0);
//error('d',"make_dtor->");
	return nn;
}
*/

Ptype find_arg_type(Pclass cl)
// first determine argument type
{
	int i;
	int mod = 0;
	for (Pbcl b = cl->baselist; b; b = b->next) {
		Pclass bcl = b->bclass;
		switch (b->base) {
		case VIRTUAL:
		case NAME:	//	generate :b(*(b*)&arg)
		{
			Pname itor = bcl->has_itor();
			if (itor && itor->tp->base==FCT) {
				Pname a = Pfct(itor->tp)->argtype;
				Pptr p = a->tp->is_ref();
				if (p && p->typ->tconst()==0) {
					mod = 1;
					goto ll1;
				}
			}
		}
		}
	}
ll1:
    if (mod == 0) {
	for (Pname m=cl->memtbl->get_mem(i=1); m; m=cl->memtbl->get_mem(++i) ) {
		Pname cln;
		// ignore static members
		if (m->n_evaluated || m->n_stclass==STATIC) continue;

		if (cln = m->tp->is_cl_obj()) {
			Pname itor = Pclass(cln->tp)->has_itor();
			if (itor && itor->tp->base==FCT) {
				Pname a = Pfct(itor->tp)->argtype;
				Pptr p = a->tp->is_ref();
				if (p && p->typ->tconst()==0) {
					mod = 1;
					goto ll2;
				}
			}
		}
	}
    }
ll2:
//error('d',"mod %d",mod);
		Pbase bp = new basetype(INT,0);
		*bp = *Pbase(Pptr(cl->this_type)->typ);
		if (mod == 0) bp->b_const = 1;
		return new ptr(RPTR,bp);
}

Pname classdef::make_itor(int def)
/*
	make cn::cn(const cn&) :bases_and_members_of_cn {}
*/
{
//error('d',"%t->make_itor(%d) %d",this,def,obj_size);
	Pstmt s;
	Pname e;
	int i;
	Pname arg = new name(make_name('A'));
	arg->tp = find_arg_type(this);

//	c_xref = 2;  // now it has X(X&)
	c_xref |= C_XREF;  // now it has X(X&)

    if (def) {
	// define itor
	int slow = 0;	// slow==0 => copy using vector copy
	int first = 1;
	Pexpr es = 0;
	s = new estmt(SM,no_where,0,0);
	e = 0;


	if (warning_opt && 128<obj_size)
		error('w',"copying a %d byte object (ofC %s)",obj_size,string);

	if (baselist) slow = 1;
		
	//for (Pbcl v = baselist; v; v = v->next)
	//	if (v->base == VIRTUAL) {
	//		slow = 1;
	//		break;
	//	}

	// initialize bases:
    if (slow) {
	for (Pbcl b = baselist; b; b = b->next) {
		Pclass bcl = b->bclass;
		Ptype pt = bcl->this_type;
//error('d',"base %t %k offset %d ptroffset %d",bcl,b->base,b->obj_offset,b->ptr_offset);
		switch (b->base) {
		case VIRTUAL:
		case NAME:	//	generate :b(*(b*)&arg)
		{
			Pexpr b2 = new name(arg->string);
			b2 = b2->address();
			b2 = new texpr(CAST,pt,b2);//new cast(pt,b2);
			b2->i2 = 1;
			b2 = b2->contents();

			Pname ee = new name(bcl->string);
			ee->base = TNAME;
			ee->n_initializer = b2;
			if (e) ee->n_list = e;
			e = ee;
			break;
		}
		}
	}
    }

	// initialize members
	for (Pname m=memtbl->get_mem(i=1); m; m=memtbl->get_mem(++i) ) {
		// ignore static members
		if (m->n_evaluated || m->n_stclass==STATIC) continue;

		if (strcmp(m->string,"__vptr")==0) {
// error('d',"%t vptr: %d",this,first);
			if (first==0 && m->n_offset) {
				Pexpr th = new expr(THIS,0,0);
				Pexpr a = new name(arg->string);
				a = a->address();
				Pexpr ee = copy_obj(th,a,m->n_offset);
				es = es ? new expr(CM,es,ee) : ee;
			}
			slow = 1;
			first = 0;
			continue;
		}
		Ptype mt = m->tp;
	tx:
		switch (mt->base) {
		case TYPE:
			mt = Pbase(mt)->b_name->tp;
			goto tx;
		case VEC:
		{	Pname cn = Pvec(mt)->typ->is_cl_obj();
//error('d',"vec %n %t xref %d",m,mt,cn?Pclass(cn->tp)->c_xref:0);
			if (cn && Pclass(cn->tp)->c_xref&(C_XREF|C_VBASE)) {
				error('s',"copy of %n[], no memberwise copy for%n",cn,cn);
				slow = 1;	// make sure an assignment operator
						// is generated so that there will
						// be no more error messages
			}

			if (slow && mt->tsizeof()) {
			/*
				generate:
				struct _s { char[sizeof(m)]; };
				*(struct _s*)this->m = *(struct _s*)arg.mem;
			*/
				Pexpr l = new name(m->string);
				Pexpr r = new name(m->string);
				r = new ref(DOT,new name(arg->string),r);
				Pexpr ee = copy_obj(l,r,mt->tsizeof());
				es = es ? new expr(CM,es,ee) : ee;
				break;
			}
		}
		case FCT:
		case OVERLOAD:
		case CLASS:
		case ENUM:
			break;
		case COBJ:
		{	Pclass mcl = Pclass(Pbase(mt)->b_name->tp);
			if (slow==0	// if slow, previous members have
					// already been copied
			&& mcl->c_xref&(C_VBASE|C_XREF)) {
				slow = 1;
				if (first==0 && m->n_offset) {
						//AAA copy up to here
					Pexpr th = new expr(THIS,0,0);
					Pexpr a = new name(arg->string);
					a = a->address();
					Pexpr ee = copy_obj(th,a,m->n_offset);
					es = es ? new expr(CM,es,ee) : ee;
				}
			}
			// no break
		}

		default:
			if (slow) {
//error('d',"slow %s %s",m->string,arg->string);
				Pname ee = new name(m->string);
				ee->n_initializer = new ref(DOT,new name(arg->string),new name(m->string));
				if (e) ee->n_list = e;
				e = ee;
			}
		}
		first = 0;
	}
	
	if (slow == 0) {
	/*	really simple just copy:
		the only problem was a vptr which can be
		ignored since X(X&) is going to reset it anyway
		don't use assignment of this struct to avoid operator=
	*/
		Pexpr th = new expr(THIS,0,0);
		Pexpr a = new name(arg->string);
		a = a->address();
		Pexpr ee = copy_obj(th,a,obj_size);
		es = es ? new expr(CM,es,ee) : ee;
	}

	s->e = es;
	Pname cn = ktbl->look(string,0);
	if (cn) cn = Pbase(cn->tp)->b_name;
	cc->stack();
	cc->not = cn;
	cc->cot = this;
}
	Pname fn = new name(string);
	Pfct f = new fct(defa_type,arg,1);
	fn->tp = f;
	fn->n_oper = TNAME;
//	fn->n_sto = STATIC;

	Pfct(f)->f_inline = def?1:ITOR;	// ITOR means ``define itor() if used''
	if (def) {
		f->body = new block(curloc,0,s);
		f->f_init = e;
	}

	Pname nn = fn->dcl(memtbl,PUBLIC);
	delete fn;

	if (def) {
		cc->unstack();
		nn->simpl();
		if (debug_opt) nn->dcl_print(0);
	}
//error('d',"make_itor->");
	return nn;
}

int make_assignment(Pname cn)
/*
	write the function:

	X& X::operator=(const X&) { assign all bases and members }

	return 1 is a function is really synthesized
*/
{
	Pclass cl = Pclass(cn->tp);
	Pstmt s = new estmt(SM,no_where,0,0);
	Pexpr e = 0;
	Pname arg = new name(make_name('A'));
	basetype* bp = new basetype(INT,0);
	*bp = *Pbase(Pptr(cl->this_type)->typ);
	bp->b_const = 1;
//	arg->tp = new ptr(RPTR,bp);
	arg->tp = find_arg_type(cl);
//error('d',"make_assignment %t %d",cl,cl->obj_size);
//	cl->c_xref |= 4;  // now it has X::operator=(const X&)

	if (warning_opt && 128<cl->obj_size)
		error('w',"copying a %d byte object (ofC %s)",
			cl->obj_size,cl->string);
{
	int slow = 0;	// slow==0 => copy using vector copy
	int first = 1;	// first==1 => first member of (derived) class

	if (cl->baselist) slow = 1;	// be dumb and safe
//	for (Pbcl v = cl->baselist; v; v = v->next)
//		if (v->base==VIRTUAL || v->c_xref&(C_VBASE|C_ASS)) {
//			slow = 1;
//			break;
//		}

    if (slow) {
	for (Pbcl b = cl->baselist; b; b = b->next) {
		Pclass bcl = b->bclass;
		Ptype pt = bcl->this_type;
		switch (b->base) {
		case NAME:
		{	// generate: *(bcl*)this = *(bcl*)&arg;
//error('d',"base %t",bcl);
			Pexpr b1 = new expr(THIS,0,0);
			b1 = new texpr(CAST,pt,b1);
			b1 = b1->contents();

			Pexpr b2 = new name(arg->string);
			b2 = b2->address();
			b2 = new texpr(CAST,pt,b2);
			b2->i2 = 1;
			b2 = b2->contents();

			Pexpr ee = new expr(ASSIGN,b1,b2);
			e = e ? new expr(CM,e,ee) : ee;
			break;
		}
		case VIRTUAL:
			if (warning_opt)
				error('w',"copying an object ofC%n with a virtualBC",cn);
			if (b->ptr_offset) {
				// copy object, but not pointer
				// generate: *(bcl*)this->Pw = *(bcl*)arg->Pw;

				// I don't know how to avoid copying the object
				// once for each pointer

				Pexpr b1 = new expr(THIS,0,0);
				b1 = new mdot(bcl->string,b1);
				b1->i1 = 3;
				b1->tp = pt;
				b1 = new expr(DEREF,b1,0);//b1->contents();

				Pexpr b2 = new name(arg->string);
				b2 = b2->address();
				b2 = new mdot(bcl->string,b2);
				b2->i1 = 3;
				b2->tp = pt;
				b2 = new expr(DEREF,b2,0);//b2->contents();

				Pexpr ee = new expr(ASSIGN,b1,b2);
				e = e ? new expr(CM,e,ee) : ee;
			}
			break;
		}
	}
    }

	int i;
	for (Pname m=cl->memtbl->get_mem(i=1); m; m=cl->memtbl->get_mem(++i) ) {
		// ignore static members
		if (m->n_evaluated || m->n_stclass==STATIC) continue;

		if (strcmp(m->string,"__vptr")==0) {	// don't copy vptrs
							// we may be copying
							// into a base class object
		if (first==0 && m->n_offset) {
					// copy up to (but not including) vptr
					// don't copy if first member
				Pexpr th = new expr(THIS,0,0);
				Pexpr a = new name(arg->string);
				a = a->address();
				Pexpr ee = copy_obj(th,a,m->n_offset);
				e = e ? new expr(CM,e,ee) : ee;
			}
			slow = 1;
			first = 0;
			continue;
		}
		Ptype mt = m->tp;
	tx:
		switch (mt->base) {
		case TYPE:
			mt = Pbase(mt)->b_name->tp;
			goto tx;
		case VEC:
		{	Pname cn = Pvec(mt)->typ->is_cl_obj();
			if (cn && Pclass(cn->tp)->c_xref&(C_ASS|C_VBASE)) {
				error('s',"copy of %n[], no memberwise copy for%n",cn,cn);
				slow = 1;	// make sure an assignment operator
						// is generated so that there will
						// be no more error messages
			}

			if (slow && mt->tsizeof()) {
				// protect against sizeof(mt)==0: char[]
			/*
				generate:
					struct _s { char[sizeof(m)]; };
					*(struct _s*)this->m = *(struct _s*)arg.mem;
			*/	
				Pexpr l = new name(m->string);
				Pexpr r = new name(m->string);
				r = new ref(DOT,new name(arg->string),r);
				Pexpr ee = copy_obj(l,r,mt->tsizeof());
				e = e ? new expr(CM,e,ee) : ee;
				break;
			}
		}
		case FCT:
		case OVERLOAD:
		case CLASS:
		case ENUM:
			break;
		case RPTR:
			error("cannot assignC%t:RM%n",cl,m);
			break;
		case COBJ:
//error('d',"cobj %n %d %d",m,slow,Pclass(Pbase(mt)->b_name->tp)->c_xref);
			if (slow==0
			&& Pclass(Pbase(mt)->b_name->tp)->c_xref&(C_VBASE|C_ASS)) {
				// must use its assignment operation
				if (first==0 && m->n_offset) {
						// copy up to this member 
					Pexpr th = new expr(THIS,0,0);
					Pexpr a = new name(arg->string);
					a = a->address();
					e = copy_obj(th,a,m->n_offset);
				}
				slow = 1;
			}
			// no break: copy cobj itself
		default:
 //error('d',"defa %n %d",m,slow);
			if (slow) {
				if (m->tp->tconst()) error("cannot assignC%t: const M%n",cl,m);
				Pname ms = new name(m->string);
				Pname as = new name(arg->string);
				Pexpr ee = new ref(DOT,as,new name(m->string));
				ee = new expr(ASSIGN,ms,ee);
				e = e ? new expr(CM,e,ee) : ee;
			}
		}
		first = 0;
	}

        if (slow == 0) {
        /*      really simple just copy:
        */
//error('d',"slow");
/*
                Pexpr th = new expr(THIS,0,0);
                Pexpr a = new name(arg->string);
                a = a->address();
                Pexpr ee = copy_obj(th,a,cl->tsize());
                e = e ? new expr(CM,e,ee) : e;
*/
//error('d',"%n simple assignment",cn);
	//	cl->c_xref ^= C_ASS; // Didn't mean it: No X::operator=(X&)
		return 0;
        }

}

	Pexpr rv = new expr(THIS,0,0);
	rv = new expr(DEREF,rv,0);//b1->contents();
	
	s->e = e ? new expr(CM,e,rv) : e;
	s->s_list = new estmt(RETURN,no_where,rv,0);

	cc->stack();
	cc->not = cn;
	cc->cot = cl;

	cl->c_xref |= C_ASS;  // now it has X::operator=(const X&)

	Pname fn = new name(oper_name(ASSIGN));
	Pfct f = new fct(new ptr(RPTR,Pptr(cl->this_type)->typ),arg,1);
	f->f_inline = 1;
	fn->tp = f;
	fn->n_oper = ASSIGN;
	fn->n_sto = STATIC;
	Pname nn = fn->dcl(cl->memtbl,PUBLIC);
	delete fn;
	Pfct(nn->tp)->body = new block(curloc,0,s);
	Pfct(nn->tp)->dcl(nn);

	cc->unstack();
	nn->simpl();
//error('d',"make_assign->");
	return 1;
}

void classdef::dcl(Pname cname, Ptable tbl)
{
	int bvirt = 0;
	int dvirt = 0;
	int scope = PUBLIC;
	int protect = 0;
	int st = 1;		// nothing private or protected seen: a struct
	// int nstd = (in_class && nested_sig)?2:(in_class?1:0); // nested class

	int byte_old = byte_offset;
	int bit_old = bit_offset;
	int max_old = max_align;
	int boff = 0;

	int in_union = 0;
	int usz;
	int make_ctor = 0;
	int make_dtor = 0;

	/* this is the place for paranoia */
	if (this == 0) error('i',"0->Cdef::dcl(%p)",tbl);
	if (base != CLASS) error('i',"Cdef::dcl(%d)",base);
	if (cname == 0) error('i',"unNdC");
	if (cname->tp != this) error('i',"badCdef");
	if (tbl == 0) error('i',"Cdef::dcl(%n,0)",cname);
	if (tbl->base != TABLE) error('i',"Cdef::dcl(%n,tbl=%d)",cname,tbl->base);
	DB( if(Ddebug>=1) error('d',&cname->where,"classdef::dcl %s tbl %d gtbl %d",string,tbl,gtbl); );
// error('d',&cname->where,"classdef::dcl %s tbl %d gtbl %d",string,tbl,gtbl); 

	switch (csu) {
	case UNION:
		in_union = UNION;
		break;
	case ANON:
		in_union = ANON;
		break;
	case CLASS:
		scope = 0;
	}

	max_align = AL_STRUCT;

	if (lex_level) {
// error('d',&cname->where,"%t::dcl in_class: %t lex_level %d ",this,in_class, lex_level); 
		if ( in_class )
			lex_level = 0;
		else {
    			in_fct = cc->nof;
    			if ( lcl == 0 ) lcl = make_name( 'L' );
		}
	}

	if (strlen == 0) strlen = ::strlen(string);
	if (baselist) {
		/*
			check base classes.
			duplicates were removed in start_cl() in norm.c.
			remove bad classes.
			add virtual bases from bases to the list.

			check against
				class b : a {}
				class c : a, b {}	// first a inaccessible
		*/
		
		Pbcl ll = 0;
		Pbcl lll = 0;
		Pbcl vlist = 0;
		for (Pbcl lx, l=baselist; l; l=lx) {	// remove bad bases
			Pclass cl = l->bclass;
			/* restriction lifted: nested and local classes
			int b_nstd = (cl->in_class && cl->nested_sig) 
					?2:(cl->in_class?1:0); 

			if (b_nstd) 
				error((b_nstd==2)?0:'w',"%s derived from nestedC %s", string, cl->string);
			if (cl->lex_level) 
				error('w',"%s derived from localC  %s", string, cl->string);
			*/

			lx = l->next;

//error('d',"base1 %t %k init %d",cl,l->ppp,l->init);
			// ``class'' => private base ``struct'' => public base
			if (l->ppp == 0) {
				l->ppp = csu==CLASS ? PRIVATE : PUBLIC;
#ifndef OLD
				if (l->ppp == PRIVATE) error('w',"B%t private by default: please be explicit ``: private%t",cl,cl);
#endif
			}
			// if you have a ``class'' as base you cannot remain a
			// ``mere struct''
			if (cl && cl->csu == CLASS) st = 0;

			if ((cl->defined&(DEFINED|SIMPLIFIED)) == 0) {
				error("BC %sU",cl->string);
				continue;
			}
			else
				(void)cl->tsizeof();	// ensure printout

			if (cl->csu==UNION || cl->csu==ANON) {
				error("C derived from union");
				continue;
			}

			if (in_union) {
				error("derived union");
				continue;
			}

			if (l->base==VIRTUAL) {
			// constraint removed ...
			//	if (cl->has_ictor()==0 && cl->has_ctor()) {
			//		error("virtualB%t has no defaultIr",cl);
			//		continue;
			//	}
				// order of virtual classes doesn't matter
				l->next = vlist;
				vlist = l;
			}
			else {	// keep ordinary base classes in order
				if (ll == 0) {
					lll = l;
					l->next = 0;
				}
				else
					ll->next = l;
				ll = l;
			}
		}
		if (ll) {
			ll->next = vlist;	// put virtual bases at end
			baselist = lll;
		}
		else
			baselist = vlist;

		lll = 0;
		for (l=baselist; l; l=l->next) {	// detect unmanageable duplicates
			Pclass b = l->bclass;
			for (ll=baselist; ll; ll=ll->next)
				if (b!=ll->bclass && ll->bclass->check_dup(b,l->base)) {
					if (lll)
						lll->next = l->next;
					else
						baselist = l->next;
					goto mmm;
				}
						
			lll = l;
			mmm:;
		}

		for (l=baselist; l; l=l->next) {	// promote virtual bases
			Pclass b = l->bclass;
			for (ll=b->baselist; ll; ll=ll->next) {
				if (ll->base == VIRTUAL) {
					Pclass v = ll->bclass;
					for (Pbcl lll=baselist; lll; lll=lll->next)
						if (lll->bclass == v) goto nnn;
		//			error('w',"%t is virtualB of%t",v,this);
					baselist = new basecl(v,baselist);
					baselist->base = VIRTUAL;
					baselist->promoted = 1;
// needs a more complete check of visibility rules
//error('d',"promote %t %k",v,ll->ppp);
					baselist->ppp = ll->ppp;
				}
				nnn:;
			}
		}

		ll = 0;
		lll = 0;
		l=baselist;
		baselist = 0;
		vlist = 0;
		for (; l; l=lx) {
			// sort virtual bases so that no virtual base
			// is ahead of its own virtual base
			lx = l->next;
			if (l->base == VIRTUAL) { // add to sorted vlist
						  // each class before its bases
				if (vlist == 0) {
					vlist = l;
					l->next = 0;
				}
				else {
					Pclass lb = l->bclass;
					Pbcl v_prev = 0;
					for (Pbcl vx, v = vlist; v; v=vx) {
						Pclass vb = v->bclass;
						vx = v->next;

						if (lb->has_base(vb)) {
							// put l ahead of v
							l->next = v;
							if (v_prev)
								v_prev->next = l;
							else
								vlist = l;
							break;
						}
						if (vx == 0) {
							// stick l at end
							v->next = l;
							l->next = 0;
							break;
						}	
						v_prev = v;
					}
				}
			}
			else {	// keep in order
				if (ll == 0) {
					lll = l;
					l->next = 0;
				}
				else
					ll->next = l;
				ll = l;
			}
		}

		if (ll) {
			ll->next = vlist;	// put virtual bases at end
			baselist = lll;
		}
		else
			baselist = vlist;

		for (l=baselist; l; l=l->next) { // allocate base class objects
			Pclass cl = l->bclass;
//error('d',"base %t %k init %d",cl,l->ppp,l->init);
			if (l->base == VIRTUAL) {	// : virtual bclass
				// pointer and object for virtual base MAY
				// be allocated at the end - but not here
				c_xref |= C_VBASE;
				dvirt += cl->virt_count;
			}
			else {				// : bclass =>allocate
				int ba = cl->align();
				if (max_align<ba) max_align = ba;

				if (cl == baselist->bclass) {
					// pad to ensure alignment:
					boff = cl->real_size;
					// not obj_size-real_size, we can
					// optimize vbase object away
					int xtra = boff%ba;
					// align
					if (xtra) boff += ba-xtra;
				}
				else {	// let C handle the padding:
					int xtra = boff%ba;
					if (xtra) boff += ba-xtra;	// align
					l->obj_offset = boff;
					// don't use waste
					boff += cl->obj_size;
				}
				bvirt += cl->virt_count;
			}

                        if (cl->has_vvtab) has_vvtab = 1;
			c_xref |= cl->c_xref;

//error('d',"%t: base %t conv %d base conv %d",this,cl,conv,cl->conv);
			conv = merge_conv(conv,cl->conv);
		}
	}

	memtbl->set_name(cname);

//	int nmem = mem_list->no_of_names();
	int nmem = 0;
	int fct_mem = 0;
	{	for (Pname m = mem_list; m; m=m->n_list) {
			nmem++;
			if (m->tp && m->tp->base==FCT) fct_mem++;
		}
	}
	if (nmem) memtbl->grow((nmem<=2)?3:nmem);

	cc->stack();
	cc->not = cname;
	cc->cot = this;

	byte_offset = usz = boff;
	bit_offset = 0;
	
	int real_virts = 0;
	Pbase bt = new basetype(COBJ,cname);
	bt->b_table = memtbl;
	Ptype cct = bt->addrof();
	// for strict opt type of `this': X *const
	// '2' distinguishes this case from a real constant object
	if (strict_opt) Pptr(cct)->rdo = 2;
	this_type = cc->tot = cct;
	PERM(cct);
	PERM(bt);

	for (Pname px, p=mem_list; p; p=px) {
	/*
		look at each member;
		declare it and determine its visibility
		calculate offsets and sizes
	*/
		px = p->n_list;
// error( 'd', "p: %n %k n_scope: %d", p, p->base, p->n_scope );

		switch (p->base) {
		case PUBLIC:
			scope = PUBLIC;
			protect = 0;
			goto prpr;

		case PRIVATE:
			scope = 0;
			protect = 0;
			goto prpr;

		case PROTECTED:
			scope = 0;
			protect = PROTECTED;
		prpr:
			if (in_union == ANON) error(&p->where,"%k in anonymous unionD",p->base);
			continue;

		case PR:	// visibility control:	C::M
		{
			char* qs = p->n_qualifier->string;
			char* ms = p->string;
			TOK ppp = scope?PUBLIC:(protect?PROTECTED:PRIVATE);

			p->base = NAME;
			p->n_scope = scope;
			p->n_protect = protect;

			if (strcmp(ms,qs) == 0) ms = "__ct";

			ppbase = PUBLIC;

			if (is_base(qs) == 0) {
				error("%kQr %s not aBC of %s",ppp,qs,string);
				continue;
			}

			mex = 1;
			tcl = mec = this;
			me = 0;
			mef = 0;
			c_body = 0; // this search must not be interpreted as a use
			Pname os = Cdcl;
			Cdcl = p;
			access_control = 1;
			Pexpr ee = find_name(ms,0);
			access_control = 0;
			Cdcl = os;
			c_body = 1;

// error('d', "ee: %k ", ee->base );

//			while (ee->base == MDOT || ee->base == REF) 
                        while (ee && (ee->base == MDOT || ee->base == REF))
				ee = ee->mem;
			Pname mx = Pname(ee);

// error('d', "ee: %k mx: %n", ee->base, mx );

			if (mx == 0) {
				error("C %s does not have aM %s",qs,ms);
				continue;
			}

			if (mx->tp->base == OVERLOAD) {
				error('s',"%k specification of overloaded%n",ppp,mx);
				continue;
			}

			TOK pp = mx->n_scope?PUBLIC:mx->n_protect?PROTECTED:PRIVATE;
			
// error('d',"mx %n pp %k ppp %k",mx,pp,ppp);
			if (ppp != pp) {
				error(&p->where,"%kM%n specified%k",pp,mx,ppp);
				continue;
			}	
			
		
			p->n_qualifier = mx;
			Pname m = memtbl->insert(p,0);
			m->base = PUBLIC;
			if (Nold) error("twoDs ofCM%n",p);
			continue;
		}
		}

// error('d',"mem%n tp %d %k scope %d",p,p->tp->base,p->tp->base,scope);

		if (scope==0) { 
			if (p->n_sto != STATIC) st = 0;
		}
		else 
		if ( p->tp->base == TYPE ) {
			Pname nn = p->tp->is_cl_obj();
			if (nn) { 
// error( 'd', "nn: %n %k tp %t %k", nn, nn->base, nn->tp, nn->tp->base );
    				if (Pclass(nn->tp)->csu == CLASS) st = 0;
			}
		}

		if (p->tp->base == FCT) {
			int ff = 0;
			Pfct f = Pfct(p->tp);
			Pblock b = f->body;
			f->body = 0;
			if (b) 
				f->f_inline = 1;
			/* restriction lifted: nested classes
			else
			if (nstd) 
				error((nstd==2)?0:'w',&p->where,"non-inlineMF%n in nestedC %s",p,string);
			*/
			else
			if (lex_level) 
				error('w',&p->where,"non-inlineMF%n in localC %s",p,string);

			switch (p->n_sto) {
			case FRIEND:
				ff = 1;
				break;
			case STATIC:	// accept static member functions
//error('d',"inline %d",f->f_inline);
			//	if (f->f_inline==0)
				/* restriction lifted: nested classes
				if (nstd) 
					error((nstd==2)?0:'w',"staticMF%n in nestedC %s",p,string);
				else
				*/
				if (lex_level) 
					error('w',"staticMF%n in localC %s",p,string);
				f->f_static = 1;
				p->n_sto = 0;
				break;
			case AUTO:
		//	case STATIC:
			case REGISTER:
			case EXTERN:
				error(&p->where,"M%n cannot be%k",p,p->n_sto);
				p->n_sto = 0;
			}

			if (f->f_virtual) real_virts++;

			Pname m = p->dcl(memtbl,scope);
			if (m == 0 || m->tp->base != FCT) continue;
                        if (m->n_initializer) {
                                c_abstract = 1;
                                if (m->n_oper == DTOR)
                                       error('w',"please provide an out-of-line definition: %n {}; which is needed by derived classes",m);
                        }
			if (ff == 0) m->n_protect = protect;
			if (b) {
				if (m->tp->defined&DEFINED || Pfct(m->tp)->body )
					error(&p->where,"two definitions of%n",m);
				else
					Pfct(m->tp)->body = b;
			//	Pfct(m->tp)->f_inline = 1;
			}
			if (ff==0 && p->where.line!=m->where.line)
				error(&p->where,"%n cannot be redeclared inCD",p);
		}
		else {
			Eppp = scope?scope:protect?protect:0;

			if (p->base == TNAME) {
				// typedef names are exported to
				// surrounding non-class scope
				// NOTE: name should actually be declared in
				//       the innermost block of the current
				//       function, but this info isn't available
				if ( cc->nof ) {
					if (p->tp && 
						Pbase(p->tp)->base != COBJ && 
						Pbase(p->tp)->base != EOBJ) {
							Pname n = cc->ftbl->look(p->string,0);
							if (n && n->base != TNAME && 
                                                                 n->lex_level &&
								n->tp != p->tp ) {
									error('s',"transitional model of nestedTs within%n (%s asTdef and%t)",cc->nof,p->string,n->tp); 
									error('i', "cannot recover from previous errors" );
							}
					}
					else p->dcl(cc->ftbl,scope);
				}
				else
					p->dcl(gtbl,scope);

				// typedefs need to be generated outside class
                                if ( p->tp && Pbase(p->tp)->base != COBJ ) {
                                        Pname n = gtbl->look(p->string,0);
					if ( n && lex_level == 0 && n->base != TNAME )
                                                error(&p->where,"%nredefined: identifier and typedef",p); 
                                	p->dcl_print(0);
                                }
                                continue;
                        };

			if (p->n_initializer) {
				error(&p->where,"Ir forM%n",p);
				p->n_initializer = 0;
			}

			if (p->tp->base==OVERLOAD) {
				for (Plist gl=Pgen(p->tp)->fct_list; gl; gl=gl->l) { 
					Pname nn = gl->f;
					Pfct ff = Pfct(nn->tp);
					if ( ff->f_virtual ) real_virts++;
				}
			}

// error('d',"lex_level %d p %n lex %d nested_scope: %k",lex_level,p,p->lex_level, nested_scope);

			TOK is_friend = p->n_sto;
			if ( lex_level && p->tp->base == VEC ) lcl_tbl = tbl;
			if ( protect && p->tp->base == COBJ && 
				Pclass(Pbase(p->tp)->b_name->tp)->csu == ANON ) 
					p->n_protect = protect;
			Pname m = p->dcl(memtbl,scope);
			lcl_tbl = 0;
			Eppp = 0;
			if (m == 0) continue;

			m->n_protect = protect;

			if (m->n_stclass==STATIC) {
				if (in_union) error("staticM%n in union",m);
				if ( is_friend != FRIEND ) {
					/* restriction lifted: nested classes
					if (nstd)
						error((nstd==2)?0:'w',"staticM%n in nestedC %s",m,string);
					else
					*/
					if (lex_level)
						error(strict_opt?0:'w',"staticM%n in localC %s (anachronism)",m,string);
				}
			//	Ptype t = m->tp;
			//	Pname cn = t->is_cl_obj();
			//	if ((cn && Pclass(cn->tp)->has_ctor())
			//	|| t->is_ref()
			//	|| (t->tconst() && vec_const == 0))
				m->n_sto = EXTERN;
				if (tbl == gtbl) stat_mem_list = new name_list(m,stat_mem_list);
				if (m->n_initializer) error('s',"staticM%nWIr",m);
			}

			if (in_union) {
				if (usz < byte_offset) usz = byte_offset;
				byte_offset = 0;
				if(in_union==ANON) m->n_offset+=byte_old;
			}
		}
	}

	/* restriction lifted: nested and local classes
	if (real_virts) {
		if (nstd)
			error((nstd==2)?0:'w',"nestedC%s contains%d virtualF",string,real_virts);
		else
		if (lex_level)
			error('w',"localC%s contains%d virtualF",string,real_virts);
	}
	*/

	if (st && csu==CLASS) csu = STRUCT;	// nothing private => STRUCT
	if (st==0 && csu==STRUCT) csu = CLASS;	// all is not public => CLASS

	if (in_union) byte_offset = usz;

	// now look look at the members

	Pname ct = has_ctor();
	Pname dt = has_dtor();

	int i;

	int omex = mex; mex = 0;
	Pname on = has_oper(NEW);
	Pname od = has_oper(DELETE);
	mex = omex;

	if (dt && ct==0 && Pfct(dt->tp)->f_virtual == 0 ) error('w',"%s has%n but noK",string,dt);
	if (on && od==0) error('w',"%s has%n but no operator delete()",string,on);
	if (od && on==0) error('w',"%s has%n but no operator new()",string,od);

	if (dt==0 && od && od && od->n_table==memtbl) make_dtor = 1;

	for (Pname m=memtbl->get_mem(i=1); m; m=memtbl->get_mem(++i) ) {
	/*
		The members have been declared.
		now look at each to see if it needs defining
	*/
		Ptype t = m->tp;

		if (t == 0) continue;	// public declarations
// error('d', "m %n %d", m, m->n_scope );

		switch (t->base) {
		default:
			if (ct==0
			&& m->n_stclass!=ENUM
			&& m->n_stclass!=STATIC) {
				if (t->is_ref()) error("R%n inC %sWoutK",m,string);
				if (t->tconst()
				&& vec_const==0
				&& m->n_evaluated==0)
					error("const%n inC %sWoutK",m,string);
			}
		case VEC:
			break;
		case FCT:
		case OVERLOAD:
		case CLASS:
		case ENUM:
			continue;
		}

		Pname cn = t->is_cl_obj();
		if (cn == 0) cn = cl_obj_vec;
		if (cn == 0) continue;

		Pclass cl = Pclass(cn->tp);
                c_xref |= cl->c_xref;
//error('d',"m %n %t %d",m,cl,cl->c_xref);
		if (cl->has_ctor()) {
			if (m->n_stclass == STATIC)
				; // error('s',"staticM%n ofC%nWK",m,cn);
			else if (in_union) {
				if (strncmp("__C",string,3) == 0)
				    error("M %s ofC%nWK in union",m->string,cn); // tagless union
				else error("M%n ofC%nWK in union",m,cn);
			}
			else if (ct == 0) {
				// does mctor have a default constructor?
				if (make_ctor==0 && cl->has_ictor()==0)
					error("%t needs aK; it has aMWK requiringAs",this);
				make_ctor = 1;
			}
		}

		if (cl->has_dtor()) {
			if (m->n_stclass==STATIC)
				; // error('s',"staticM%n ofC%nW destructor",m,cn);
			else if (in_union) {
				if (strncmp("__C",string,3) == 0) // tagless union
				    error("M %s ofC%nW destructor in union",m->string,cn);
				else error("M%n ofC%nW destructor in union",m,cn);
			}
			else if (dt==0)
				make_dtor = 1;
		}
	}

	if (has_dtor()==0 && make_dtor==0) {
		// can dtor be inherited (from single base class)?
		Pclass bcl = 0;
		for (Pbcl b = baselist; b; b = b->next) {
			switch (b->base) {
			case NAME:
				if (bcl == 0) {
					Pname d = b->bclass->has_dtor();
					if (d==0) break;
					if (strict_opt == 0) {
						if (d->n_scope==PUBLIC
						&& Pfct(d->tp)->f_virtual==0) {
							bcl = b->bclass;
							break;
						}
					}
				}
				// two dtors or non-public dtors force creation
                                make_dtor = 1;
                                goto zbzb;
                        case VIRTUAL:
                                if (b->bclass->has_dtor()) make_dtor = 1;
                                goto zbzb;
			}
		}
		if (bcl) c_dtor = bcl->has_dtor();
	zbzb:;
	}

	if (make_dtor && dt==0) {	// make x::~x() {}

		// must be done before vtbls are made in case dtor is virtual
//error('d',"%t: make_dtor",this);
		Pname n = new name(string);
		Pfct f = new fct(defa_type,0,1);
		f->f_inline = /*IDTOR; */ 1;
		n->tp = f;
		n->n_oper = DTOR;
		dt = n->dcl(memtbl,PUBLIC);
		delete n;
		Pfct(dt->tp)->body  = /*0; */ new block(curloc,0,0);
		dt = has_dtor();
	}

	if (virt_count && find_vptr(this)==0) {
		// we only need a vptr if this class
		// has virtual functions and none
		// of its first bases have vptrs
//error('d',"%s virt_count %d",string,virt_count);
		Pname vp = new name("__vptr");
		vp->tp = Pvptr_type;
		(void) vp->dcl(memtbl,0);
		delete vp;
		c_xref |= C_VPTR;
	}
	else {
//error("byte_offset %d bit_offset %d bitsinbyte %d",byte_offset,bit_offset,BI_IN_BYTE);
//error("   div %d mod %d",bit_offset/BI_IN_BYTE,bit_offset%BI_IN_BYTE);
	    // no more bit fields. absorb bit_offset
	    if (bit_offset) {
		byte_offset += (bit_offset/BI_IN_BYTE + (bit_offset%BI_IN_BYTE?1:0));
		bit_offset = 0;
	    }
	}
//error("byte_offset %d bit_offset %d",byte_offset,bit_offset);

	for (Pbcl b = baselist; b; b = b->next) { // allocate virtual base pointers
		if (b->base != VIRTUAL) continue;
		Pclass bcl = b->bclass;

		// search non-virtual bases for this virtual base
		for (Pbcl bb = baselist; bb; bb = bb->next)
			if (bb->base == NAME) {
				for (Pbcl l = bb->bclass->baselist; l; l=l->next)
					if (l->base==VIRTUAL && l->bclass==bcl) goto eee;
			}
		{
		if (obj_align<AL_WPTR) obj_align = AL_WPTR;
		if (max_align<AL_WPTR) max_align = AL_WPTR;
		int waste = byte_offset%AL_WPTR;
		if (waste) byte_offset += AL_WPTR-waste;	// align
		b->ptr_offset = byte_offset+1;			// ensure != 0
		byte_offset += sizeof(int*);
		}
	eee:;
	}

	real_size = byte_offset;	// the rest may be optimized away

	for (b = baselist; b; b = b->next) {	// allocate virtual class objects
		if (b->base != VIRTUAL) continue;
		Pclass bcl = b->bclass;

		// if necessary
		if (b->obj_offset = has_allocated_base(bcl)) continue;

		int ba = bcl->align();
		if (obj_align<ba) obj_align = ba;
		if (max_align<ba) max_align = ba;
		int waste = byte_offset%ba;
		if (waste) byte_offset += ba-waste;	// align
		b->obj_offset = byte_offset;		// offset in this
		b->allocated = 1;
//error('d',"virtual %t in %t at %d",bcl,this,b->obj_offset);
		byte_offset += bcl->tsizeof();
	}

	// no more data members.
	// pad object (so that copying into a base object
	// doesn't destroy derevid class members):
	if (byte_offset==0) {	// empty struct: waste a member
		Pname c = new name (make_name('W'));
		c->tp = char_type;
		(void) c->dcl(memtbl,0);
		real_size = byte_offset = 1;
	}
	if (byte_offset < SZ_STRUCT) byte_offset = SZ_STRUCT;
	int waste = byte_offset%max_align;
//error('d',"max_align %d waste %d byte_offset %d",max_align,waste,byte_offset);
	if (waste) byte_offset += max_align-waste;
	obj_size = byte_offset;
	obj_align = max_align;

	// make vtbls
	// this cannot be done until the bases
	// have been allocated in this class
	// so that the offsets (deltas) are known
	if (all_virt(this,0,1,0)) {
		if (has_ctor()==0) make_ctor = 1;
	}
	else if (has_vvtab)
		error("virtualB: conflicting vtable initialization");

// error('d',"%t->classdef: virt_count: %d virt_merge: %d",this,virt_count,virt_merge);

	Pname hito = has_itor();

	if (hito) c_xref |= C_XREF;	// has user defined X(X&)
//error('d',"%t hito %d ctor %d",this,hito,make_ctor);
	if (hito==0 && c_xref&(C_VPTR|C_VBASE|C_XREF)) {
		// X(X&) needed if bitwise copy is illegal
		// or if any constructor is defined
		
		hito = make_itor(0);
			// if the base has B::B(void)
			// the derived should have D::D(void)
		if (baselist) {
			int mc = 1;	// can make and ictor
			for (Pbcl b = baselist; b; b = b->next) {
				if (b->bclass->has_ctor()
				&& b->bclass->has_ictor()==0)
						mc = 0;
			}
			make_ctor = mc;
/*
			int mc = 1;	// can make an ictor
			for (Pbcl b = baselist; b; b = b->next) {
				if (b->bclass->has_ctor()) {
					Pname c = b->bclass->has_ictor();
						// no copy constructor?
						// non-public copy constructor?
					if (c==0 || c->n_scope==0) mc = 0;
				}
			}
			make_ctor = mc;
*/
		}
		else
			make_ctor =  1;
	}

	if (c_ctor==0 && make_ctor==0) {	// can ctor be inherited (from single base class)?
/*
		int bb = 0;
		for (Pbcl b = baselist; b; b = b->next) {
			switch (b->base) {
			case NAME:
			{	Pname c = b->bclass->has_ctor();
			//	if (b->bclass->has_ctor() == 0) break;
				if (c == 0) break;
				if (c->n_scope==PUBLIC && bb++==0) break;
			}
				// no break: two bases: needs ctor
			case VIRTUAL:
				make_ctor = 1;	// virtual base: need ctor
				goto zaza;
			}
		}
*/
		Pname btor = 0;
		Pclass bc = 0;
		for (Pbcl b = baselist; b; b = b->next) {
			switch (b->base) {
			case NAME:
			{	Pname c = b->bclass->has_ctor();
				if (c == 0) break;
				if (c->n_scope==PUBLIC && b==baselist) {
					bc = b->bclass;
					btor = bc->has_ictor();
					break;
				}
			}
				// no break: two bases: needs ctor
			case VIRTUAL:
				make_ctor = 1;	// virtual base: need ctor
				goto zaza;
			}
		}
	//	c_ctor = btor;
//error('d',"btor %n",btor);
		if (bc) {
			if (btor)
				make_ctor = 1;
			else
				error("K needed for %s, BC%t hasK",string,bc);
		}
	zaza:;
	}

	if (make_ctor && ct==0) {	// make x::x() {}
//error('d',"%t: make_ctor",this);
		Pname n = new name(string);
		Pfct f = new fct(defa_type,0,1);
		f->f_inline = 1; // ICTOR; // ICTOR means ``define ctor() if used''
		n->tp = f;
		n->n_oper = TNAME;
		ct = n->dcl(memtbl,PUBLIC);
		delete n;
		Pfct(ct->tp)->body = /* 0; */ new block(curloc,0,0);
	}

	defined |= DEFINED;

	if (ansi_opt) {
		char* s = csu==UNION || csu==ANON ? "union" : "struct";
		fprintf(out_file,"%s %s;",s,string);
	}
//error('d',"defined %s",string);

	// fix argument lists for inlines
	for (p=memtbl->get_mem(i=1); p; p=memtbl->get_mem(++i)) {
		Pfct f = Pfct(p->tp);
		if (f==0) continue;	// public declarations

		switch (f->base) {
		case FCT:
			/* prohibit something like the following:
				typedef int t1;
				class x {
					typedef int t1;
					t1 foo();
				};
			*/
			if (f->returns->base == TYPE) {
				Pname nn = Pbase(f->returns)->b_name;
				while ( nn->tp->base == TYPE ) 
					nn = Pbase(nn->tp)->b_name;
				if (nn->n_key == NESTED) {
					Ptype tt;
					if ( nn->tp->base != EOBJ && 
						nn->tp->base != COBJ )
							tt = nn->tpdef;
					else  tt = Pbase(nn->tp)->b_name->tp; 
					if ( tt && tt->nested_sig )
						error('w',"nested %s as returnT for non-inlineMF, use %t::%s %n{} in definition (anachronism)",nn->string,tt->in_class,nn->string,p);
				}
			}

			if (hito && f->argtype) fix_args(f,this);
			if (p->n_oper == CTOR) f->s_returns = this_type;
			if (f->body) p->n_sto = STATIC;
			break;
		case OVERLOAD:
		{	Pgen g = Pgen(f);
			for (Plist gl=g->fct_list; gl; gl=gl->l) {
				Pname n = gl->f;
				Pfct f = Pfct(n->tp);

				if (hito && f->argtype) fix_args(f,this);
				if (n->n_oper == CTOR) f->s_returns = this_type;
				if (f->body) n->n_sto = STATIC;
			}
		}
		}
	}

	// define members defined inline
	for (p=memtbl->get_mem(i=1); p; p=memtbl->get_mem(++i)) {
		Pfct f = Pfct(p->tp);
		if (f==0) continue;	// public declarations

		switch (f->base) {
		case FCT:
			if (f->body) {
				f->dcl(p);
				p->simpl();
			}
			break;
		case OVERLOAD:
		{	Pgen g = Pgen(f);
			for (Plist gl=g->fct_list; gl; gl=gl->l) {
				Pname n = gl->f;
				Pfct f = Pfct(n->tp);

				if (f->body) {
					f->dcl(n);
					n->simpl();
				}
			}
		}
		}
	}

	byte_offset = byte_old;
	bit_offset = bit_old;
	max_align = max_old;

	cc->unstack();	// friends are not in class scope

	// fix arguments lists for friends defined inline 
	for (Plist fl=friend_list; fl; fl=fl->l) {
		Pname p = fl->f;
		Pfct f = Pfct(p->tp);

		switch (f->base) {
		case FCT:
			if (hito && f->argtype) fix_args(f,this);
			if (f->body &&
			   (f->defined&(DEFINED|SIMPLIFIED)) == 0) 
				p->n_sto = STATIC;
			else 
			if (p->n_scope == STATIC)
                        	error(strict_opt?0:'w',"static%n declared friend toC%t",p,this);
			break;
		case OVERLOAD:
		{	Pgen g = Pgen(f);
			for (Plist gl=g->fct_list; gl; gl=gl->l) {
				Pname n = gl->f;
				Pfct f = Pfct(n->tp);

				if (hito && f->argtype) fix_args(f,this);
				if (f->body &&
				   (f->defined&(DEFINED|SIMPLIFIED)) == 0) 
					n->n_sto = STATIC;
				else 
				if (p->n_scope == STATIC)
                                	error(strict_opt?0:'w',"static%n declared friend toC%t",p,this);
			}
		}
		}
	}

	// define friends defined inline and modify return types if necessary
	for (fl=friend_list; fl; fl=fl->l) {
		Pname p = fl->f;
		Pfct f = Pfct(p->tp);

		switch (f->base) {
		case FCT:
			if (f->body &&
			   (f->defined&(DEFINED|SIMPLIFIED)) == 0) {
				f->dcl(p);
				p->simpl();
			}
			break;
		case OVERLOAD:
		{	Pgen g = Pgen(f);
			for (Plist gl=g->fct_list; gl; gl=gl->l) {
				Pname n = gl->f;
				Pfct f = Pfct(n->tp);

				if (f->body &&
				   (f->defined&(DEFINED|SIMPLIFIED)) == 0) {
					f->dcl(n);
					n->simpl();
				}
			}
		}
		}
	}

	if (tbl != gtbl) this->simpl();

        if ( statStat && strcmp(statStat->string,string)==0) {
//error('d', "classdef::dcl: statstat: %n this %s", statStat, string);
                statStat->hide();
                statStat = 0;
        }
// error('d',"classdef::dcl defined: %d",defined);
	// catch refs to this class in body of nested class function
	// (i.e., ref to member of this class...)
	if ( (defined&REF_SEEN) != 0 ) dcl_print(0);

     if ( debug_opt ) {
	for (p=memtbl->get_mem(i=1); p; p=memtbl->get_mem(++i)) {
		Pfct f = Pfct(p->tp);
		if (f==0) continue;	// public declarations

		switch (f->base) {
			case FCT:
				if (f->body) {
					if ( c_body == 1 ) dcl_print(0);
					p->dcl_print(0);
				}
				break;
			case OVERLOAD: 
			{	
				Pgen g = Pgen(f);
				for (Plist gl=g->fct_list; gl; gl=gl->l) {
					Pname n = gl->f;
					Pfct f = Pfct(n->tp);
					if (f->body) {
						if ( c_body == 1 ) dcl_print(0);
						p->dcl_print(0);
					}
				}
			}
		}
	}

	for (fl=friend_list; fl; fl=fl->l) {
		Pname p = fl->f;
		Pfct f = Pfct(p->tp);

		switch (f->base) {
			case FCT:
				if (f->body &&
			   	   (f->defined&(DEFINED|SIMPLIFIED)) == 0) 
					p->dcl_print(0);
				break;
			case OVERLOAD:
			{	
				Pgen g = Pgen(f);
				for (Plist gl=g->fct_list; gl; gl=gl->l) {
					Pname n = gl->f;
					Pfct f = Pfct(n->tp);

					if (f->body &&
				   	   (f->defined&(DEFINED|SIMPLIFIED)) == 0) 
						p->dcl_print(0);
				}
			}
		}
	}
    } // end, if (debug_opt)
}

void enumdef::dcl(Pname n, Ptable tbl)
{
//	if (this == 0) error('i',"0->enumdef::dcl(%p)",tbl);
	Pname px;
	Pname p = mem;
	DB( if(Ddebug>=1) error('d',&n->where,"enumdef(%n,%d)",n,tbl); );
	Pbase b = new basetype(EOBJ,n);
	b->b_const = 1;

#define FIRST_ENUM 0
	int enum_count = FIRST_ENUM;
	no_of_enumerators = mem->no_of_names();
	int largest = 0;

	if (p == 0) mem = new name(make_name('e'));

	for (; p; p=px) {
		px = p->n_list;
// error( 'd', "p %n", p );
		if (p->n_initializer) {
			Pexpr i = p->n_initializer->typ(tbl);
			Neval = 0;
			long ii = i->eval();
			if (largest_int<ii) error("long enumerator");
			enum_count = int(ii);
			if (Neval) error("%s",Neval);
			DEL(i);
			p->n_initializer = 0;
		}
		p->n_evaluated = 1;
		largest |= enum_count;
		p->n_val = enum_count++;
		p->tp = b;

		Pname nn = tbl->insert(p,0);
		if (Nold) {
			if (nn->n_stclass == ENUM) {
			//	error( (p->n_val!=nn->n_val)?0:'w',"enumerator%n declared twice",nn);
		//	if (p->n_val!=nn->n_val)
			error("enumerator%n declared twice",nn);
			
			}
			else
				error("incompatibleDs of%n",nn);
		}
		else {
			nn->n_stclass = ENUM; // no store will be allocated
			if (Eppp == PROTECTED)
				nn->n_protect = PROTECTED;
			else if (Eppp == PUBLIC)
				nn->n_scope = PUBLIC;
		}
		p->string = nn->string;
//		delete p;
	}

//	mem = 0;

//	chose a shorter representation for the enum?
//	if (largest&0133 == largest)
//		e_type = char_type;
//	else if largest&077777 == largest)
//		e_type = short_type;
//	else
		e_type = int_type;

	defined |= DEFINED;
}


