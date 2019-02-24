/*ident	"@(#)ctrans:src/cfront.h	1.2.5.35" */
/***********************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

	When reading cfront code please remember that C++ was not available
	when it was originally written. Out of necessity cfront is written
	in a style that takes advantage of only few of C++'s features.

cfront.h:

	Here is all the class definitions for cfront, and most of the externs

***********************************************************************/

/*	WARNING:
	This program relies on non-initialized class members being ZERO.
	This will be true as long as they are allocated using the "new" operator
	from alloc.c
 */

#include "token.h"
#include "typedef.h"

#ifndef GRAM
extern char* prog_name;		// compiler name and version
extern int inline_restr;	// inline expansion restrictions 
#endif

extern Pname name_free;		// free lists
extern Pexpr expr_free;
extern Pstmt stmt_free;

		/* "spy" counters: */
//extern int Nspy;
//extern int Nline, Ntoken, Nname, Nfree_store, Nalloc, Nfree;
//extern int Nn, Nbt, Nt, Ne, Ns, Nstr, Nc, Nl;
//extern int NFn, NFtn, NFpv, NFbt, NFf, NFs, NFc, NFe, NFl;

///extern TOK	lex();
extern TOK	tlex();
extern Pname	syn();

extern void	ext(int);

extern char* 	make_name(TOK);
extern void 	make_dummy();
extern Pname	dummy_fct;
extern Pname    really_dominate(Pname, Pname, bit);
extern int      exact1(Pname, Ptype);

extern int friend_check(Pclass start,Pclass stop, Pfct f);

struct loc		// a source file location
{
	short	file;	// index into file_name[], or zero
	short	line;
#ifndef GRAM
	void	put(FILE*);
	void	putline();
#endif
};

extern Loc curloc;
extern int curr_file;
extern char* src_file_name;
extern loc noloc; // dummy null location
extern bit binary_val;
extern bit stmtno;

struct ea {	// fudge portable printf-like formatting for error()
	union {
		void* p;
		long i;
	};

	ea(const void* pp) { p = pp; }
	ea(long ii)   { i = ii; }
	ea() {}
};

extern ea* ea0;

overload error;
int error(const char*);
int error(const char*, const ea&, const ea& = *ea0, const ea& = *ea0, const ea& = *ea0);
int error(int, const char*);
int error(int, const char*, const ea&, const ea& = *ea0, const ea& = *ea0, const ea& = *ea0);
int error(loc*, const char*, const ea& = *ea0, const ea& = *ea0, const ea& = *ea0, const ea& = *ea0);
int error(int, loc*, const char*, const ea& = *ea0, const ea& = *ea0, const ea& = *ea0, const ea& = *ea0);
extern char emode;

extern int error_count;
extern int vtbl_opt;
extern int debug_opt;
extern int warning_opt;
extern int ansi_opt;
extern int strict_opt;
extern FILE* out_file;
extern FILE* in_file;
extern char scan_started;

extern int bl_level;
extern Ptable ktbl;		// keywords and typedef names
extern Ptable gtbl;		// global names
extern Ptable ptbl;		
extern Ptable lcl_tbl;
extern char* oper_name(TOK);
extern Pname def_name;
extern Pname pdef_name;
extern Pclass ccl;
extern Pbase defa_type;
extern Pbase moe_type;
extern Pbase mptr_type;

#ifndef GRAM
extern Pstmt Cstmt;		// current statement, or 0
extern Pname Cdcl;		// name currently being declared, or 0

extern Pbase any_type;
extern int largest_int;
#endif

extern Pbase int_type;
extern Pbase char_type;
extern Pbase short_type;
extern Pbase long_type;
extern Pbase uint_type;
extern Pbase float_type;
extern Pbase double_type;
extern Pbase ldouble_type;
extern Pbase void_type;

#ifndef GRAM
extern Pbase uchar_type;
extern Pbase ushort_type;
extern Pbase ulong_type;
extern Ptype Pchar_type;
extern Ptype Pint_type;
extern Ptype Pvptr_type;
extern Ptype Pfctvec_type;
extern Ptype Pvoid_type;
extern Pbase zero_type;
extern Ptype size_t_type;

extern int byte_offset;
extern int bit_offset;
extern int max_align;
extern int const_save;
extern int const_ptr;
extern int cm_const_save;
extern bit const_problem;
#endif

extern Pexpr dummy;	/* the empty expression */
extern Pexpr zero;
extern Pexpr one;
extern Pname sta_name;	/* qualifier for unary :: */

#define DEL(p) if (p && (p->permanent==0)) p->del()
#define PERM(p) p->permanent=1
#define UNPERM(p) p->permanent=0

struct node {
	TOK	base;
	bit	permanent;
#ifdef DBG
	bit	displayed;	// avoid invinite recursion in display functions
	bit	allocated;	// set when not on free list
	long	id;
#endif
};
#ifdef DBG
extern long node_id;
extern int Adebug;
#define DBID() { node::id = ++::node_id; node::allocated=1; displayed=0; \
	if(Adebug>=1)fprintf(stderr,"\n*** allocated %d base %d\n",id,base); }
#else
#define DBID() /**/
#endif

struct table : node {
/*	a table is a node only to give it a "base" for debugging */
	bit	init_stat;	/* ==0 if block(s) of table not simplified,
				   ==1 if simplified but had no initializers,
				   ==2 if simplified and had initializers.
				*/
	short	size;
	short	hashsize;
	short	free_slot;	/* next free slot in entries */
	Pname*	entries;
	short*	hashtbl;
	Pstmt	real_block;	/* the last block the user wrote,
				   not one of the ones cfront created
				*/
	Ptable	next;		/* table for enclosing scope */
	Pname	t_name;		/* name of the table */

	table(short, Ptable, Pname);

	Pname	look(char*, TOK);
	Pname	insert(Pname, TOK);
#ifndef GRAM
	void	grow(int);
	void	set_name(Pname n)	{ t_name = n; };
	Pname	get_mem(int);
	int	max()			{ return free_slot-1; };
	void	dcl_print(TOK,TOK);
	void	del();
#endif
};

#ifndef GRAM
extern bit Nold;
extern bit vec_const, fct_const;
#endif

extern void restore();
extern void set_scope(Pname);
extern Plist modified_tn;

// local class
extern Plist local_tn;
extern Plist local_blk;
extern Plist local_class;
extern void local_hide( Pname ); 
extern char *make_local_name(Pclass, int=0);

// nested type
extern Plist nested_tn;
extern Plist nested_type;
extern Pname curr_scope; 
extern Pname curr_fct; 
extern void nested_hide(Pname); 
extern int is_empty(Pclass,bit=0);

extern Pbase start_cl(TOK, Pname, Pbcl);
extern void end_cl();
extern Pbase end_enum(Pname, Pname);

/************ types : basic types, aggregates, declarators ************/

#ifndef GRAM
extern bit new_type;
extern Pname cl_obj_vec;
extern Pname eobj;
#endif


#define DEFINED 01	/* definition fed through ?::dcl() */
#define SIMPLIFIED 02	/* in ?::simpl() */
#define DEF_SEEN 04	/* definition seen, but not processed */
#define REF_SEEN 010	/* reference seen (classdef) */
			/*   used for class members in norm.c */
#define IN_ERROR 010

struct type : node {
	bit	defined;	/* flags DEF_SEEN, DEFINED, SIMPLIFIED, IN_ERROR
					not used systematically yet
				*/
	bit	lex_level;
	Pclass  in_class; 	// nested type
	Pname	in_fct;		// local type
	char    *lcl;
	char	*nested_sig;
	char	*signature(char*);
	Ptype	tlist;
	bit	check(Ptype, TOK);
#ifndef GRAM
	void	print();
	void	dcl_print(Pname);
	void	base_print();
	void	del();

	Pname	is_cl_obj();	/* sets cl_obj_vec */
	Pptr	is_ref();
	Pptr	is_ptr();
	Pptr	is_ptr_or_ref();
	bit	is_unsigned();
	void	dcl(Ptable);
	int	tsizeof(int = 0);
	bit	tconst();
	TOK	set_const(bit);
	int	align();
	TOK	kind(TOK,TOK);
	TOK	integral(TOK oo)	{ return kind(oo,'I'); };
	TOK	numeric(TOK oo)		{ return kind(oo,'N'); };
	TOK	num_ptr(TOK oo)		{ return kind(oo,'P'); };
	bit	vec_type();
	Ptype	deref();
	inline Pptr	addrof();
	Pfct	memptr();
#endif
};

struct enumdef : type {	/* ENUM */
	bit	e_body;
	short	no_of_enumerators;
	unsigned char strlen;	// strlen(string)
	char*	string;		// name of enum
	Pname	mem;
	Pbase	e_type;		// type representing the enum
		enumdef(Pname n)	{ base=ENUM; mem=n; };
#ifndef GRAM
	void	print();
	void	dcl_print(Pname);
	void	dcl(Pname, Ptable);
	void	simpl();
#endif
};

struct velem {
	Pname	n;
	int	offset;
};

struct virt {
	Pvirt	next;
	velem*	virt_init;	// vector of vtbl initializers (zero-terminated)
	Pclass	vclass;		// for class vclass
	char*	string;
	bit	is_vbase;	// vtable for virtual base
	bit	printed;
	virt(Pclass cl, velem* v, char* s, bit flag) {vclass=cl; virt_init=v; string=s; is_vbase=flag; next=0; }
};

enum { C_VPTR=1, C_XREF=2, C_ASS=4, C_VBASE=8 };

struct classdef : type {	/* CLASS */
	bit	c_body;		/* print definition only once */
	TOK	csu;		/* CLASS, STRUCT, UNION, or ANON */
	bit	obj_align;
	bit	c_xref;
		// 1 set:	has vptr(s) 
		// 2 set:	X(X&) exists
		// 4 set:	operator=(X&) exists
		// 8 set:	has vbaseptr(s)
		
	bit	virt_count;	// number of virtual functions
				// starting at max base class virt_count in
	bit	virt_merge;	// set when no virtual functions, but
				// need to merge virtual base classes
	bit	c_abstract;	// abstract class: don't instansiate
	bit	has_vvtab;	// set if class has vtable from virtual base
	unsigned char strlen;	// strlen(string)
	Pbcl	baselist;	// list of base classes
	char*	string;		/* name of class */
	Pname	mem_list;
	Ptable	memtbl;
	int	obj_size;
	int	real_size;	/* obj_size - alignment waste */
	Plist	friend_list;
	Pname	pubdef;
	Plist	tn_list;	// list of member names hiding type names
	Plist	nest_list;	// list of nested types
	Ptype	this_type;
	Pvirt	virt_list;	// vtbl initializers
	Pname	c_ctor;		// constuctor:
				// possibly overloaded, possibly inherited
	Pname	c_dtor;		// destructor
	Pname	c_itor;		/* constructor X(X&) */
	Pname	conv;		/* operator T() chain */
	struct toknode *c_funqf, *c_funqr; // token Q for parsing function defs after class def

	classdef(TOK);
	TOK	is_simple()	{ return (csu==CLASS)?0:csu; };
#ifndef GRAM
	void	print();
	void	dcl_print(Pname);
	void	simpl();

	void	print_members();
	void	dcl(Pname, Ptable);

//	bit	has_friend(Pname);
	bit	has_friend(Pclass);
	bit	has_friend(Pfct);

	bit	has_base(Pclass cl);
	bit	baseof(Pname);
	bit	baseof(Pclass);
	Pclass	is_base(char*);

	Pname	has_oper(TOK);
	Pname	has_ctor()	{ return c_ctor; }
	Pname	has_dtor()	{ return c_dtor; }
	Pname	has_itor()	{ return c_itor; }
	Pname	has_ictor();
	Pname	make_itor(int);
	Pexpr	find_name(char*, Pclass);
	int	do_virtuals(Pvirt, char*, int, bit);
	int	all_virt(Pclass, char*, int, bit);
	void	add_vtbl(velem*, char*, bit);
	void	print_all_vtbls(Pclass);
	void	print_vtbl(Pvirt);
	void	really_print(Pvirt);
	int	check_dup(Pclass, TOK);
	int	has_allocated_base(Pclass);
	char 	*has_allocated_base(char*);
	int	get_offset(char*);
	Pbcl	get_base(char*);
	Pexpr	get_vptr_exp(char*);
	Pexpr	find_in_base(char*, Pclass);
#endif
};

#ifndef GRAM

class clist {
	Pclass cl;
	clist* next;
public:
	clist(Pclass c, clist* n) { cl=c; next=n; }
	int onlist(Pclass);
	void clear();
};
	
extern clist * vcllist;

struct vl {
	struct vl* next;
	Pvirt vt;
	classdef* cl;

	vl(classdef* c, Pvirt v, struct vl* n)
		{ cl = c; vt = v; next = n; }
};

extern vl* vlist;

extern int nin;
extern int Nvis;
extern int Noffset;
extern TOK Nvirt;
extern Pexpr Nptr;
extern Pbcl Nvbc_alloc;
extern char *Nalloc_base;
extern Pexpr rptr(Ptype,Pexpr,int);
extern Pexpr vbase_args(Pfct, Pname);
extern Pexpr cdvec(Pname,Pexpr,Pclass,Pname,int,Pexpr,Pexpr=0);

extern Pexpr find(char*, Pclass, int);
extern Pexpr find_name(Pname, Pclass, Ptable, int, Pname);
extern Pname find_virtual(Pclass,Pname);
extern Pname vfct(Pclass, char*);
extern int Vcheckerror;
extern int vhack;
extern int ignore_const;	

extern int access_control; 
extern int mex;
extern Pclass mec;
extern Pclass tcl;
extern Pname me;
extern Pfct mef;
#endif

struct basetype : type
	/*	ZTYPE CHAR SHORT INT LONG FLOAT DOUBLE
		FIELD EOBJ COBJ TYPE ANY
	*/
	/*	used for gathering all the attributes
		for a list of declarators

		ZTYPE is the (generic) type of ZERO
		ANY is the generic type of an undeclared name
	*/
{
	bit	b_unsigned;
	bit	b_signed;
	bit	b_volatile;
	bit	b_const;
	bit	b_typedef;
	bit	b_inline;
	bit	b_virtual;
	bit	b_short;
	bit	b_long;
	bit	b_bits;		/* number of bits in field */
	bit	b_offset;	// bit offset of field
	TOK	b_sto;		/* AUTO STATIC EXTERN REGISTER 0 */
	Pname	b_name;		/* name of non-basic type */
	Ptable	b_table;	/* memtbl for b_name, or 0 */
	Pname	b_xname;	/* extra name */
	union {
	Ptype	b_fieldtype;
	char*	b_linkage;
	};

	basetype(TOK, Pname);

	Pbase	type_adj(TOK);
	Pbase	base_adj(Pbase);
	Pbase	name_adj(Pname);
	Pname   aggr();
	Pbase	check(Pname);
#ifndef GRAM
	void	dcl_print();
 	Pbase	arit_conv(Pbase);
#endif
};

enum Linkage { linkage_default, linkage_C, linkage_Cplusplus };
extern Linkage linkage;
void set_linkage(char*);

struct fct : type		// FCT
{
	TOK	nargs;
	TOK	nargs_known;	// 0 if unknown, 1 if known, or ELLIPSIS
	bit  	last_stmt;
	bit	f_vdef;		// 1 if this is the first virtual definition
				// of this function
	bit	f_inline;	// 1 if inline, 2 if being expanded, else 0
	bit	f_const;	// one if member function that may be called for
				// a const object, else 0
	bit	f_static;	// 1 if static member function, else 0
	short	f_virtual;	// index in virtual table, or 0 meaning non-virtual
	short	f_imeasure;	// some measure of the size of an inline function
	Ptype	returns;
	Pname	argtype;
	Ptype	s_returns;
	Pname	f_this;
	Pclass	memof;		// member of class memof
	Pblock	body;
	Pname	f_init;		// base and member initializers
//	Pname	f_baseinit;	// base initializers
//				// null name => single base class init;
	Pexpr	f_expr;		// body expanded into an expression
	Pexpr	last_expanded;
	Pname	f_result;	// extra second argument of type X&
	Pname	f_args;		// argument list including args added by cfront
	Linkage	f_linkage;
	char*	f_signature;	// character encoding of function type
	Plist local_class;	// list of local classes
	fct(Ptype, Pname, TOK);
	void	argdcl(Pname,Pname);
#ifndef GRAM
	Ptype	normalize(Ptype);
	void	dcl_print();
	void	dcl(Pname);
	Pexpr	base_init(Pclass, Pexpr, Ptable, int);
	Pexpr	mem_init(Pname, Pexpr, Ptable);
	void	init_bases(Pclass, Pexpr);
	bit	declared() { return nargs_known; };
	void	simpl();
	int	ctor_simpl(Pclass, Pexpr);
	Pstmt	dtor_simpl(Pclass, Pexpr);
	Pexpr	expand(Pname,Ptable,Pexpr);
	void	sign();
#endif
};


struct name_list {
	Pname	f;
	Plist	l;
	name_list(Pname ff, Plist ll) { f=ff; l=ll; };
};

#ifndef GRAM
struct gen : type {		// OVERLOAD
	Plist	fct_list;

	gen() { base = OVERLOAD; }
	Pname	add(Pname);
	Pname  	find(Pfct, bit);
	Pname	match(Pname, Pfct, bit);
};
#endif

struct pvtyp : type {
	Ptype typ;
};

struct vec : pvtyp		// VEC
				// typ [ dim ]
{
	Pexpr	dim;
	int	size;	

	vec(Ptype t, Pexpr e) { base=VEC; typ=t; dim=e; DBID(); };
#ifndef GRAM
	Ptype	normalize(Ptype);
#endif
};

struct ptr : pvtyp		// PTR, RPTR i.e. reference
{
	Pclass	memof;		// pointer to member of memof: memof::*
	bit	rdo;		// "*const"

	ptr(TOK b, Ptype t) { base=b; typ=t; DBID(); }
#ifndef GRAM
	Ptype	normalize(Ptype);
#endif
};

#ifndef GRAM
inline Pptr type::addrof() { return new ptr(PTR,this); }

extern bit vrp_equiv;
#endif


/****************************** constants ********************************/

		/* STRING ZERO ICON FCON CCON ID */
		/* IVAL FVAL LVAL */

/***************************** expressions ********************************/

#ifndef GRAM
extern Pexpr next_elem();
extern void new_list(Pexpr);
extern void list_check(Pname, Ptype, Pexpr, Ptable=0);
extern Pexpr ref_init(Pptr,Pexpr,Ptable);
extern Pexpr class_init(Pexpr,Ptype,Pexpr,Ptable);
extern Pexpr check_cond(Pexpr, TOK, Ptable);
extern Pexpr ptof(Pfct,Pexpr,Ptable);
extern void dosimpl(Pexpr, Pname);
extern int ref_initializer;
extern int ntok;

extern void ptbl_init(int);
extern void ptbl_add_pair(char*,char*);
extern char *ptbl_lookup(char*);
extern char *st_name(char*);
#endif

struct expr : node	/* PLUS, MINUS, etc. */
	/* IMPORTANT:	all expressions are of sizeof(expr) */
	/*	DEREF		=>	*e1 (e2==0) OR e1[e2]
		UMINUS		=>	-e2
		INCR (e1==0)	=>	++e2
		INCR (e2==0)	=>	e1++
		CM		=>	e1 , e2
		ILIST		=>	LC e1 RC   (an initializer list)
		a Pexpr may denote a name
	*/
{
	union {
		Ptype	tp;
		int	syn_class;
		char 	*string4;
	};
	union {
		Pexpr	e1;
		long	i1;
		char*	string;
	};
	union {
		Pexpr	e2;
		int	i2;
		char*	string2;
		Pexpr	n_initializer;
		Ptype	tpdef;  // local and nested typedef info
	};
	union {			/* used by the derived classes */
		Ptype	tp2;
		Pname	fct_name;
		Pexpr	cond;
		Pexpr	mem;
		Ptype	as_type;
		Ptable	n_table;
		Pin	il;
		char*	string3;
	};

	expr(TOK, Pexpr, Pexpr);
	~expr();
#ifndef GRAM
	void	del();
	void	print();
	Pexpr	typ(Ptable);
	long	eval();
	unsigned long ueval(long,long);
	int	lval(TOK);
	Ptype	call_fct(Ptable);
	Pexpr	address();
	Pexpr	contents();
	void	simpl();
	Pexpr	expand();
	bit	not_simple();
	Pexpr	try_to_overload(Ptable);
	Pexpr	docast(Ptable);
	Pexpr	dovalue(Ptable);
	Pexpr	donew(Ptable);
	void	simpl_new();
	void	simpl_delete();
#endif
};

struct texpr : expr {		// CAST NEW VALUE (also ICALL)
#ifdef __cplusplus
	texpr(TOK bb, Ptype tt, Pexpr ee) : expr (bb,ee,0) { this=0; tp2=tt; }
#else
	texpr(TOK bb, Ptype tt, Pexpr ee) : (bb,ee,0) { this=0; tp2=tt; }
#endif

};

struct cast : expr {		// CAST
#ifdef __cplusplus
	cast(Ptype tt, Pexpr ee) : expr (CAST,ee,0) { this=0; tp=tp2=tt; }
#else
	cast(Ptype tt, Pexpr ee) : (CAST,ee,0) { this=0; tp=tp2=tt; }
#endif
};

struct ival : expr {		// IVAL
#ifdef __cplusplus
	ival(long ii) : expr (IVAL,0,0) { this=0; i1 = ii;}
#else
	ival(long ii) : (IVAL,0,0) { this=0; i1 = ii;}
#endif
};

struct call : expr {		// CALL
#ifdef __cplusplus
	call(Pexpr aa, Pexpr bb) : expr (CALL,aa,bb) { this=0; }
#else
	call(Pexpr aa, Pexpr bb) : (CALL,aa,bb) { this=0; }
#endif
#ifndef GRAM
	void	simpl();
	Pexpr	expand(Ptable);
#endif
};

struct qexpr : expr {		// QUEST	cond ? e1 : e2
#ifdef __cplusplus
	qexpr(Pexpr ee, Pexpr ee1, Pexpr ee2) : expr (QUEST,ee1,ee2) { this=0; cond=ee; }
#else
	qexpr(Pexpr ee, Pexpr ee1, Pexpr ee2) : (QUEST,ee1,ee2) { this=0; cond=ee; }
#endif
};

struct ref : expr {		// REF DOT	e1->mem OR e1.mem
#ifdef __cplusplus
	ref(TOK ba, Pexpr a, Pexpr b) : expr (ba,a,0) { this=0; mem=b; }
#else
	ref(TOK ba, Pexpr a, Pexpr b) : (ba,a,0) { this=0; mem=b; }
#endif
};

struct mdot : expr {		// MDOT		a.b
#ifdef __cplusplus
	mdot(char* a, Pexpr b) : expr (MDOT,0,0) { this=0; string2=a; mem=b; }
#else
	mdot(char* a, Pexpr b) : (MDOT,0,0) { this=0; string2=a; mem=b; }
#endif
};

struct text_expr : expr	{	// TEXT (vtbl_name)
#ifdef __cplusplus
	text_expr(char* a, char* b) : expr (TEXT,0,0)
#else
	text_expr(char* a, char* b) : (TEXT,0,0)
#endif
	{ this=0; string=a; string2=b; }
};
char* vtbl_name(char*,char*);
/************************* names (are expressions) ****************************/

struct basecl : node {	// NAME		=> base class
			// VIRTUAL	=> virtual base class
	TOK	ppp;		// private / public / protected
	bit	allocated;	// allocated virtual base
        bit     promoted;       // non-explicit, promoted virtual base
	Pclass	bclass;
	Pexpr	init;		// base class initializers for ctors
	int	ptr_offset;	// pointer's relative position in derived class
	int	obj_offset;	// object's relative position in derived class
	Pname*	virt_init;	// vector of vtbl table initializers
	basecl*	next;

        basecl(Pclass cl, basecl* n) { bclass=cl; next=n; promoted=0; init=0;}
};
extern TOK ppbase;

struct name : expr {	// NAME TNAME and the keywords in the ktbl
	TOK	n_oper;		// name of operator or 0
	TOK	n_sto;		// EXTERN STATIC AUTO REGISTER ENUM 0
	TOK	n_stclass;	// STATIC AUTO REGISTER 0
	TOK	n_scope;	// EXTERN STATIC FCT ARG PUBLIC 0 
	TOK	n_key;	/* for names in table: class */
	bit	n_evaluated;	// 0 or n_val holds the value
	bit	n_xref;		// argument of type X(X&)
	unsigned char	lex_level;
	TOK	n_protect;	// PROTECTED (<=>n_scope==0) or 0
	bit	n_dcl_printed;	// 1: declaration printed
				// 2: definition printed
				// 0: declaration not printed
        char    *n_anon;	// nested anonymous unions
	short	n_union;	// 0, or union index
	short	n_addr_taken;
	short	n_used;
	short	n_assigned_to;
	Loc	where;
	int	n_offset;	// byte offset in frame or struct
	Pname	n_list;
	Pname	n_tbl_list;
	union {
		Pname	n_qualifier;	// name of containing class
		Ptable	n_realscope;	/* for labels (always entered in
					   function table) the table for the
					   actual scope in which label occurred.
					*/
	};
	long	n_val;		// the value of n_initializer
				// also used as the argument number
				// for inline arguments
	name(char* =0);
	~name();

	Pname	normalize(Pbase, Pblock, bit);
	Pname	tdef();
	Pname	tname(TOK);
	void	hide();
	void	unhide()	{ n_key=0; n_list=0; };
#ifndef GRAM
	Pname	dcl(Ptable,TOK);
	int	no_of_names();
	void	use()		{ n_used++; };
	void	assign();
	void	take_addr();
	void	check_oper(Pname);
	void	simpl();
	void	del();
	void	print();
	void	dcl_print(TOK);
	void	field_align();
	Pname	dofct(Ptable,TOK);
#endif
};

extern int friend_in_class;
extern int in_class_dcl;

// from parser
extern int in_class_decl;
extern int parsing_class_members;
extern int in_mem_fct;
extern int in_arg_list;
extern Ptype in_typedef;
extern int	defer_check;	 // redefinition typedef check delay
extern int	declTag;	 // !1: inline, virtual mod permitted
extern Pname in_tag;
extern Pname statStat;
extern int DECL_TYPE;

/******************** statements *********************************/

struct stmt : node {	/* BREAK CONTINUE DEFAULT */
/*	IMPORTANT: all statement nodes have sizeof(stmt) */
	Pstmt	s;
	Pstmt	s_list;
	Loc	where;
	union {
		Pname	d;
		Pexpr	e2;
		Pstmt	has_default;
		int	case_value;
		Ptype	ret_tp;
	};
	union {
		Pexpr	e;
		bit	own_tbl;
		Pstmt	s2;
	};
	Ptable	memtbl;
	union {
		Pstmt	for_init;
		Pstmt	else_stmt;
		Pstmt	case_list;
	//	bit	empty;
		Loc	where2; // location of } at end of block
	};

	stmt(TOK, loc, Pstmt);
	~stmt();
#ifndef GRAM
	void	del();
	void	print();
	void	dcl();
	void	reached();
	Pstmt	simpl();
	Pstmt	expand();
	Pstmt	copy();
#endif
};

#ifndef GRAM
extern char* Neval;
extern Ptable scope;
extern Ptable expand_tbl;
extern Pname expand_fn;
#endif

struct estmt : stmt	/* SM WHILE DO SWITCH RETURN CASE */
	/*	SM (e!=0)	=>	e;
		in particular assignments and function calls
		SM (e==0)	=>	;	(the null statement)
		CASE		=>	case e : s ;
	*/
{
#ifdef __cplusplus
	estmt(TOK t, loc ll, Pexpr ee, Pstmt ss) : stmt (t,ll,ss) { this=0; e=ee; }
#else
	estmt(TOK t, loc ll, Pexpr ee, Pstmt ss) : (t,ll,ss) { this=0; e=ee; }
#endif
};

struct ifstmt : stmt	/* IF */
	// else_stme==0 =>	if (e) s
	// else_stmt!=0 =>	if (e) s else else_stmt
{
	ifstmt(loc ll, Pexpr ee, Pstmt ss1, Pstmt ss2)
#ifdef __cplusplus
		: stmt (IF,ll,ss1) { this=0; e=ee; else_stmt=ss2; };
#else
		: (IF,ll,ss1) { this=0; e=ee; else_stmt=ss2; };
#endif
};

struct lstmt : stmt	/* LABEL GOTO */
	/*
		d : s
		goto d
	*/
{
#ifdef __cplusplus
	lstmt(TOK bb, loc ll, Pname nn, Pstmt ss) : stmt (bb,ll,ss) { this=0; d=nn; }
#else
	lstmt(TOK bb, loc ll, Pname nn, Pstmt ss) : (bb,ll,ss) { this=0; d=nn; }
#endif
};

struct forstmt : stmt {	// FOR
	forstmt(loc ll, Pstmt fss, Pexpr ee1, Pexpr ee2, Pstmt ss)
#ifdef __cplusplus
		: stmt (FOR,ll,ss) { this=0; for_init=fss; e=ee1; e2=ee2; }
#else
		: (FOR,ll,ss) { this=0; for_init=fss; e=ee1; e2=ee2; }
#endif
};

struct block : stmt {	// BLOCK	{ d s }
#ifdef __cplusplus
	block(loc ll, Pname nn, Pstmt ss, loc rr = noloc ) : stmt (BLOCK,ll,ss)
#else
	block(loc ll, Pname nn, Pstmt ss, loc rr = noloc ) : (BLOCK,ll,ss)
#endif
		{ this=0; d=nn; where2=rr; }
#ifndef GRAM
	void	dcl(Ptable);
	Pstmt	simpl();
#endif
};

#ifndef GRAM
struct pair : public stmt {	// PAIR
#ifdef __cplusplus
	pair(loc ll, Pstmt a, Pstmt b) : stmt (PAIR,ll,a) { this=0; s2 = b; }
#else
	pair(loc ll, Pstmt a, Pstmt b) : (PAIR,ll,a) { this=0; s2 = b; }
#endif
};
#endif

struct nlist {
	Pname	head;
	Pname	tail;
		nlist(Pname);
	void	add(Pname n)	{ tail->n_list = n; tail = n; };
	void	add_list(Pname);
};

extern Pname name_unlist(nlist*);

struct slist {
	Pstmt	head;
	Pstmt	tail;
		slist(Pstmt s)	{ /*Nl++;*/ head = tail = s; };
	void	add(Pstmt s)	{ tail->s_list = s; tail = s; };
};

extern Pstmt stmt_unlist(slist*);

struct elist {
	Pexpr	head;
	Pexpr	tail;
		elist(Pexpr e)	{ /*Nl++;*/ head = tail = e; };
	void	add(Pexpr e)	{ tail->e2 = e; tail = e; };
};

extern Pexpr expr_unlist(elist*);

#ifndef GRAM 
extern class dcl_context * cc;

struct dcl_context {
	Pname	c_this;	/* current fct's "this" */
	Ptype	tot;	/* type of "this" or 0 */
	Pname	not;	/* name of "this"'s class or 0 */
	Pclass	cot;	/* the definition of "this"'s class */
	Ptable	ftbl;	/* current fct's symbol table */
	Pname	nof;	/* current fct's name */

	void	stack()		{ cc++; *cc = *(cc-1); };
	void	unstack()	{ cc--; };
};

#define MAXCONT	20
extern dcl_context ccvec[MAXCONT];
#endif 

extern void yyerror(char*);


#ifndef GRAM
extern char* line_format;

extern Plist stat_mem_list;
extern Plist isf_list;
extern Pstmt st_ilist;
extern Pstmt st_dlist;
extern Ptable sti_tbl;
extern Ptable std_tbl;
extern int need_sti( Pexpr e, Ptable tbl = 0, bit is_static_ok = 0 ); 
Pexpr try_to_coerce(Ptype, Pexpr, char*, Ptable);
extern bit can_coerce(Ptype, Ptype);
extern Ptype np_promote(TOK, TOK, TOK, Ptype, Ptype, TOK);
extern bit enum_promote;
extern int suppress_error;
extern void new_key(char*, TOK, TOK);
extern void delete_local();

extern int over_call(Pname, Pexpr);
extern Pname overFound;
extern Pname Nover;
extern Pname Ncoerce;
extern Nover_coerce;
struct ia {
	Pname	local;	// local variable for argument
	Pexpr	arg;	// actual arguments for call
	Ptype	tp;	// type of formal argument
};

struct iline {
	Pname	fct_name;	/* fct called */
	Pin	i_next;
	Ptable	i_table;
	int	i_slots;	// no of arg slots pointer to by i_args
	ia*	i_args;
};

extern Pexpr curr_expr;
extern Pin curr_icall;
#define FUDGE111 11111
#define VTOK 22222
#define ITOR 77

extern Pstmt curr_loop;
extern Pblock curr_block;
extern Pstmt curr_switch;
extern loc last_line;  // last #line + number of '\n's output since
extern int last_ll;    // 0 or line of current stmt/dcl being printed
extern FILE* out_file; // output file descriptor
extern bit Cast;
extern loc no_where;

extern no_of_undcl;
extern no_of_badcall;
extern Pname undcl, badcall;

extern long str_to_long(const char*);
extern int c_strlen(const char* s);
#endif

#ifndef GRAM
extern Pname vec_new_fct;
extern Pname new_fct;
extern Pname del_fct;
extern Pname vec_del_fct;

extern int Nstd; // standard coercion used (derived* =>base* or int=>long or ...)

extern int stcount;	// number of names generated using make_name()

Pexpr replace_temp(Pexpr,Pexpr);
void make_res(Pfct);

extern int Pchecked;
Pexpr ptr_init(Pptr,Pexpr,Ptable);
Pexpr call_ctor(Ptable, Pexpr p, Pexpr ctor, Pexpr args, int d = REF, Pexpr vb_args = 0);
Pexpr call_dtor(Pexpr p, Pexpr dtor, Pexpr arg = 0, int d = DOT, Pexpr vb_args = 0);
void check_visibility(Pname, Pname, Pclass, Ptable, Pname);

int make_assignment(Pname);

extern Pname make_tmp(char, Ptype, Ptable);
Pexpr init_tmp(Pname, Pexpr, Ptable);

extern int is_unique_base(Pclass, char*, int, int = 0);
extern Pexpr rptr(Ptype, Pexpr, int);

extern int read_align(char*);
extern void new_init();

extern void Eprint(Pexpr);
extern Pexpr cast_cptr(Pclass ccl, Pexpr ee, Ptable tbl, int real_cast);
extern Pexpr mptr_assign(Pexpr,Pexpr);
extern Pclass Mptr;

#endif

extern bit fake_sizeof;	// suppress error message for ``int v[];''

extern TOK lalex();


#ifdef DBG
extern void display_expr( Pexpr, char* = 0, int = 0 );
extern void display_stmt( Pstmt, char* = 0, int = 0 );
extern void display_type( Ptype );
extern void display_namelist( Plist, char* = 0, int = 0 );

extern fprintf(const FILE*, const char* ...);
extern void process_debug_flags( char* );
#define DB(a) a
extern int Adebug; // allocation (ctor/del) debugging
extern int Ddebug; // dcl debugging
extern int Edebug; // expr debugging
extern int Ldebug; // lex/lalex debugging
extern int Mdebug; // trace function matching
extern int Ndebug; // norm debugging
extern int Pdebug; // print debugging
extern int Rdebug; // run() debugging
extern int Sdebug; // simpl debugging
extern int Tdebug; // typ debugging
#define Ydebug yydebug
extern int Ydebug; // yacc debugging

#else

#define DB(a) /**/

#endif

/* end */
