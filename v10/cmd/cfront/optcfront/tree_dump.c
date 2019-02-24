/* ident "@(#)ctrans:src/tree_dump.c	1.2" */
/* -*- Mode:C++ -*- */
/* tree_dump.C -- utilities for displaying the cfront tree. */
/* BIM 890530 */
/* rewritten 890712 to use the tree-walking facilities */

/*
	$Source: /usr3/lang/benson/work/stripped_cfront/RCS/tree_dump.c,v $ $RCSfile: tree_dump.c,v $
	$Revision: 1.1 $		$Date: 89/11/20 08:50:22 $
	$Author: benson $		$Locker:  $
	$State: Exp $


*/

#include "tree_dump.h"
#include <stdio.h>
#include <malloc.h>
#include "template.h"
#include "token_names.h"

const int Indent_Increment = 4;
static char badnamebuf[100];

char *
token_name(TOK t)
{
    char * bnb = badnamebuf;

    if((int) t == -1) return "-1";
    if(t > DUMMY_LAST_NODE) {
bad:
	printf_to_string(bnb, sizeof(badnamebuf), 
			 "<unknown token %d>", (int) t);
	return bnb;
    } 
    int x;
    for (x = 0; x < DUMMY_LAST_NODE; x++) {
	if(token_names[x].val == t) return token_names[x].string;
    }
    goto bad;
}

class displayer{
  public:
    dcn_arg * arg;
    Pnode node;
    Pnode node_address;
    displayer (dcn_arg& d) { arg = &d; indent = 0; print_null_values = 0; };
    void do_node (Pnode& node, node_class cl, tree_node_action& action, int depth, Pnode);
  private:
    enum ppc {print_ptr_copy = 0, print_ptr_copied = 1};
    int indent;
    int err;
    int print_null_values;
    void printf (const char *, ...);
    void catprintf (const char *, ...);
    void print_ptr (const char *, Pnode);
    void print_ptr (Pnode, ppc = print_ptr_copy);
    void minimal ();
    void a_basetype ();
    void a_basecl() ;
    void a_type() ;
    void a_node() ;
    void a_name ();
    void a_expr (int from_name = 0);
    void a_fct ();
    void a_stmt ();
    void a_enumdef ();
    void a_table();
    void a_classdef ();
    void a_gen ();
    void a_vec ();
    void a_ptr ();
    void a_virt ();
    void a_nlist ();
    void a_iline ();
    void a_elist ();
    void a_ia ();
    void a_by_name ();
    void print_loc(loc&);
    void * display_address () { return node_address; }
    void fetch (void * addr, unsigned long length, void *& taddr);
    void fetch_string (void * addr, unsigned long length, void *& taddr);
    void free_fetched(void *);
    void flag (int, const char *);
    void print_string (const char *, void *);
    void print_string_brief (void *);
    void null_field (const char *);
    void nz_printf(const char *format, const int i) ;
    void nz_printf(const char *format, const char *p) ;
    void nz_printf(const char *format, const long l) ;
    void ind();
};

const char many_spaces[] = "                                                                                                                                                                                                        ";

inline void
displayer::ind()
{
    arg->output_stream->write(many_spaces, indent);
}

/* call me for full lines */

void
displayer::printf (const char * format, ...)
{
    va_list args;
    va_start(args, format);

    ind();
    vostream_printf (format, args, *arg->output_stream);
    arg->output_stream->write("\n", 1);
    va_end (args);
}

void
displayer::catprintf (const char * format, ...)
{
    va_list args;
    va_start(args, format);

    vostream_printf (format, args, *arg->output_stream);
    va_end (args);
}

void
displayer::minimal ()
{
    print_ptr(node, print_ptr_copied);
}    

void
displayer::print_ptr(const char * label, Pnode node)
{
    if(node || print_null_values) {
	ind();
	catprintf("%s:\t", label);
	print_ptr(node);
	catprintf("\n");
    }
}


void
displayer::print_ptr(Pnode node, ppc copy_ptr)
{
    void * string = 0;
    void * ncopy;
    void * na = (void *)node;

    if(node) {

	if(copy_ptr == print_ptr_copy) {
	    fetch(na, sizeof(node), ncopy);
	    node = Pnode(ncopy);
	} else ncopy = node;

	switch(node->base) {
	case CLASS:
	    free_fetched(ncopy);
	    fetch(na, sizeof(classdef),ncopy);
	    node = Pnode(ncopy);
	    string = Pclass(node)->string;
	    break;
	case ENUM:
	    free_fetched(ncopy);
	    fetch(na, sizeof(enumdef),ncopy);
	    node = Pnode(ncopy);
	    string = Penum(node)->string;
	    break;
	case NAME:
	case TNAME:
	case PUBLIC:
	case PROTECTED:
	    if(!node->baseclass) {
		free_fetched(ncopy);
		fetch(na, sizeof(name),ncopy);
		node = Pnode(ncopy);
		string = Pname(node)->string;
	    }
	    break;
	}
	if(string) {
	    catprintf("<%s |0x%p ", token_name(node->base), na);
	    print_string_brief(string);
	    catprintf(">");
	}
	else catprintf("<%s |0x%p>", token_name(node->base), na);
	free_fetched (ncopy);
    }
    else catprintf("Null");
}

void
displayer::do_node (Pnode& n, node_class cl, tree_node_action& action,
	 int depth, Pnode na)
{
    int forced_min = 0;
    int printed_min = 0;
    int prune = 0;

    switch(cl) {
      case nc_table:
	if(depth > 0 && !arg->walk_tables)prune = 1;
      case nc_fct:
      case nc_classdef:
      case nc_enumdef:
	if(arg->stop_at_top && depth > 1) prune = 1;
	break;
    }

    if((arg->max_depth > 0) && (depth >= arg->max_depth))
	prune = 1;

    if(prune || arg->max_depth == 0)
	action = tna_stop;
    else action = tna_continue;

    node = n;
    node_address = na;

    if(arg->verbose == dt_brief || prune) {
	printed_min = 1;
	minimal();
    }
    else {
	switch(cl)
	    {
	      default:
	    case nc_unused:
		ostream_printf(*arg->error_stream, "Unknown node %d\n", node->base);
		forced_min = 1;

	    case nc_eof:
		minimal();
		printed_min = 1;
		break;

	    case nc_virt:
		a_virt ();
		break;

	    case nc_nlist:
		a_nlist();
		break;

	    case nc_gen:
		a_gen();
		break;

	    case nc_vec:
		a_vec();
		break;

	    case nc_ptr:
		a_ptr();
		break;

	    case nc_fct:
		a_fct();
		break;

	    case nc_table:
		a_table();
		break;

	    case nc_basetype:
		a_basetype();
		break;

	    case nc_name:
		a_name();
		break;

	    case nc_expr:
		a_expr();
		break;

	    case nc_stmt:
		a_stmt();
		break;

	    case nc_enumdef:
		a_enumdef();
		break;

	    case nc_classdef:
		a_classdef();
		break;

	    case nc_baseclass:
		a_basecl ();
		break;

	    case nc_iline:
		a_iline();
		break;

	    case nc_ia:
		a_ia();
		break;

	    }
	if(printed_min) catprintf(">");
    }
}


void
displayer::flag(int f, const char * n)
{
    if(f)catprintf("%s", n);
}

void
displayer::free_fetched (void * addr)
{
    if (arg->fetcher != null_tfp) /* not in the same address space. */
	free ((char *)addr);
}    


void
displayer::fetch (void * addr, unsigned long length, void *& taddr)
{
    if (arg->fetcher == null_tfp) {	/* in the same address space. */
	taddr = addr;
	err = 0;
    } else {
	taddr = (void *)malloc ((unsigned int)length);
	err = arg->fetcher (arg->fetcher_info,
			    addr,
			    length,
			    0,
			    taddr);
    }
}

void
displayer::fetch_string (void * addr, unsigned long length, void *& taddr)
{
    if (arg->fetcher == null_tfp) {	/* in the same address space. */
	taddr = addr;
	err = 0;
    } else {
	taddr = (void *)malloc ((unsigned int)length);
	err = arg->fetcher (arg->fetcher_info,
			    addr,
			    length,
			    1,
			    taddr);
    }
}

/* Print the field, only if it has a non_zero value, or requested to print */
/* always */
void
displayer::nz_printf(const char *format, const int i)
{
    if (i || print_null_values) {
	printf(format, i) ;
    }
} 

void
displayer::nz_printf(const char *format, const char *p)
{
    if (p || print_null_values) {
	printf(format, (p ? p : "0")) ;
    }
} 


void 
displayer::nz_printf(const char *format, const long i)
{
    if (i || print_null_values) {
	printf(format, i) ;
    }
} 

void 
displayer::a_node() 
{
    struct node &n = *node ;

    nz_printf("$node", n.permanent);
//    nz_printf("n_key:\t%d", (int)n.n_key) ;
    nz_printf("permanent:\t%d", n.permanent) ;
    nz_printf("baseclass:\t%d", n.baseclass) ;
}
  
  

void 
displayer::a_type()
{
    struct type& t = *Ptype(node);
    int show_up =  t.defined  /* || t.inline_temp_index != 0 */;

    if(show_up) {
	printf("$type") ;
	nz_printf("defined:\t%d", t.defined) ;
	// nz_printf("inline_temp_index:\t%d", t.inline_temp_index);
	indent += Indent_Increment;
    }
    a_node();
    if(show_up) indent -= Indent_Increment;
} 
  

  
  

void
displayer::null_field(const char * name)
{
  nz_printf( "%s:\t0", name);
}


void 
displayer::a_basecl()
{
    struct basecl& bc = *Pbcl(node);

    printf("$basecl %s |0x%p",
	   token_name(node->base),
	   node_address);
    
    nz_printf ("ppp:\t%s", token_name(bc.ppp)) ;
    nz_printf ("allocated:\t%d", bc.allocated) ;
    nz_printf ("promoted:\t%d", bc.promoted) ;
    print_ptr("bclass", bc.bclass);
    nz_printf ("ptr_offset:\t%d", bc.ptr_offset) ;
    nz_printf ("obj_offset:\t%d", bc.obj_offset) ;
    indent += Indent_Increment;
    a_node();			/* do the supertype. */
    indent -= Indent_Increment;
}




void
displayer::a_basetype ()
{
    struct basetype& bt = *Pbase(node);
  
    printf("$basetype %s |0x%p",
	   token_name(node->base),
	   node_address);

    ind();
    catprintf("Flags:\t");
    flag(bt.b_unsigned, "unsigned ");
    flag(bt.b_signed, "signed ");
    flag(bt.b_volatile, "volatile ");
    flag(bt.b_const, "const ");
    flag(bt.b_typedef, "typedef ");
    flag(bt.b_virtual, "virtual ");
    flag(bt.b_short, "short ");
    flag(bt.b_long, "long ");
    catprintf("\n");
    nz_printf( "b_bits:\t%d", bt.b_bits);
    nz_printf( "b_offset:\t%d", bt.b_offset);
    nz_printf( "b_sto:\t%s", bt.b_sto ? token_name(bt.b_sto) : 0);
    switch(bt.discriminator(0)) {
      case 1:
	print_ptr("b_fieldtype", bt.b_fieldtype);
	break;
      case 2:
	nz_printf( "b_linkage:\t%s", bt.b_linkage ? "C" : "0(C++)");
    }

    print_ptr("b_name", bt.b_name);
    print_ptr("b_table", bt.b_table);
    // print_ptr("b_field", bt.b_field);
    print_ptr("b_xname", bt.b_xname);

    indent += Indent_Increment;
    a_type();			/* do the supertype. */
    indent -= Indent_Increment;
    
}

void
displayer::print_loc(loc& loc)
{
  catprintf("file %d line %d", (int)loc.file, (int)loc.line);
}


void
displayer::print_string_brief(void * addr)
{
  void * tmp;
  
  fetch_string (addr, 1000, tmp);
  *arg->output_stream << (char *)tmp;
  free_fetched (tmp);
}

void
displayer::print_string(const char * label, void * addr)
{
  ind();
  catprintf("%s:\t", label);
  print_string_brief(addr);
  *arg->output_stream << "\n";
}

void
displayer::a_expr(int from_name)
{
    struct expr& e = *Pexpr(node);
  
    if(from_name) printf("$expr");
    else printf("$expr %s |0x%p",
		token_name(node->base),
		node_address);
  
    print_ptr("tp", e.tp);
  
    switch(e.discriminator(1)) {
      case 0: break;
      case 1:
	print_ptr("e1", e.e1);
	break;
      case 2:
	printf("i1:\t%ld", e.i1);
	break;
      case 3:
	print_string("string", (void *)e.string);
    }

    switch(e.discriminator(2)) {
      case 0: break;
      case 1:
	print_ptr("e2", e.e2);
	break;
      case 2:
	printf("i2:\t%ld", e.i2);
	break;
      case 3:
	print_string("string2", (void *)e.string2);
	break;
      case 4:
	print_ptr("n_initializer", e.n_initializer);
    }

    switch(e.discriminator(3)) {
      case 0:
	break;
      case 1: print_ptr ("tp2", e.tp2); break;
      case 2: print_ptr ("fct_name", e.fct_name); break;
      case 3: print_ptr ("cond", e.cond); break;
      case 4: print_ptr ("mem", e.mem); break;
      case 5: print_ptr ("as_type", e.as_type); break;
      case 6: print_ptr ("n_table", e.n_table); break;
      case 7: print_ptr ("il", e.il); break;
      case 8: print_ptr ("query_this", e.query_this); break;
    }

  indent += Indent_Increment;
  a_node();
  indent -= Indent_Increment;

}

void
displayer::a_name()
{
    struct name& n = *Pname(node);


    ind();
    catprintf("$name %s |0x%p ",
	      token_name(node->base),
	      node_address);
    print_string_brief((void *)n.string);
    *arg->output_stream << "\n";

    nz_printf("n_key:\t%d", (int)n.n_key) ;
    nz_printf( "n_oper:\t\t%s", n.n_oper ?
	      token_name(n.n_oper) : 0);
    nz_printf( "n_sto:\t\t%s", n.n_sto ?
	      token_name(n.n_sto) : 0);
    nz_printf( "n_stclass:\t%s", n.n_stclass ?
	      token_name(n.n_stclass) : 0);
    nz_printf( "n_scope:\t%s", n.n_scope ?
	      token_name(n.n_scope) : 0);
    nz_printf( "n_union:\t%d", n.n_union);
    nz_printf( "n_evaluated:\t%d", n.n_evaluated);
    nz_printf( "n_xref:\t\t%d", n.n_xref);
    nz_printf( "lex_level:\t%d", n.lex_level);
    nz_printf( "n_protect:\t%s", n.n_protect ?
	      token_name(n.n_protect) : 0);
    if (n.n_dcl_printed || print_null_values) {
	ind();
	catprintf("n_dcl_printed:\t%d", n.n_dcl_printed);

	switch(n.n_dcl_printed)
	    {
	    case 0:
		catprintf("(not)\n");
		break;
	    case 1:
		printf("(declaration)\n");
		break;
	    case 2:
		printf("(definition)\n");
		break;
	    }
	
    }
    if(n.n_template_arg) {
	ind();
	catprintf( "n_template_arg:\t");
	switch(n.n_template_arg) 
	    { 
	    case template_type_formal:
		catprintf("template_type_formal\n");
		break;
	    case template_expr_formal:
		catprintf("template_expr_formal\n");
		break;
	    case template_stmt_tree_formal:
		catprintf("template_stmt_formal\n");
		break;
	    case template_expr_tree_formal:
		catprintf("template_expr_tree_formal\n");
		break;
	    case template_actual_arg_dummy:
		catprintf("template_actual_arg_dummy\n");
		break;
	    } ;
    }
    nz_printf( "n_addr_taken:\t%d", n.n_addr_taken);
    nz_printf( "n_used:\t\t%d", n.n_used);
    nz_printf( "n_assigned_to:\t%d", n.n_assigned_to);
    ind();
    catprintf("loc:\t\t");
    print_loc(n.where);
    catprintf("\n");
    nz_printf( "n_offset:\t%d", n.n_offset);
    // if(n.output_string) { print_string("output_string", (void *)n.output_string); }
    nz_printf( "n_val:\t\t%ld", n.n_val);

    print_ptr("n_list", n.n_list);
    print_ptr("n_tbl_list", n.n_tbl_list);
    if(n.n_gen_fct_name)
	print_string("n_gen_fct_name", n.n_gen_fct_name);
    if(n.n_template_arg_string) {
	print_string("n_template_arg_string",
		     n.n_template_arg_string);
    }
    switch(n.discriminator(0)) {
    case 2: print_ptr("n_realscope", n.n_realscope); break;
    case 1: print_ptr("n_qualifier", n.n_qualifier); break;
    }
    nz_printf("n_val:\t%ld", n.n_val);

    indent += Indent_Increment;
    a_expr(1);			/* do the supertype. */
    indent -= Indent_Increment;
}

void
displayer::a_fct()
{
    struct fct& f = *Pfct(node);

    printf("$fct %s |0x%p",
	   token_name(node->base),
	   node_address);


    printf("nargs:\t\t%d", f.nargs);
    printf("nargs_known:\t%d%s", f.nargs_known,
	   f.nargs_known == 0 ? " UNKNOWN" :
	   (f.nargs_known == 1 ? " KNOWN" :
	    (f.nargs_known == ELLIPSIS ? "ELLIPSIS" : "" )));
    nz_printf( "f_vdef:\t\t%d", f.f_vdef);
    printf("f_inline:\t%d%s", f.f_inline,
	   f.f_inline == 0 ? "" :
	   f.f_inline == 1 ? " inline" :
	   f.f_inline == 2 ? " inline in expansion" : "");
    nz_printf( "f_const:\t\t%d", f.f_const);
    nz_printf( "f_static:\t\t%d", f.f_static);
    nz_printf( "f_virtual:\t%d", f.f_virtual);
    nz_printf( "f_imeasure:\t%d", f.f_imeasure);
    print_string("f_signature", (void *)f.f_signature);

    print_ptr("returns", f.returns);
    print_ptr("argtype", f.argtype);
    print_ptr("s_returns", f.s_returns);
    print_ptr("f_this", f.f_this);
    print_ptr("memof", f.memof);
    print_ptr("body", f.body);
    print_ptr("f_init", f.f_init);
    print_ptr("f_expr", f.f_expr);
    print_ptr("last_expanded", f.last_expanded);
    print_ptr("f_result", f.f_result);
    print_ptr("f_args", f.f_args);
    print_ptr("local_classes", f.local_class);
    
    indent += Indent_Increment;
    a_type();			/* do the supertype. */
    indent -= Indent_Increment;
}

void
displayer::a_stmt ()
{
    struct stmt& s = *Pstmt(node);

    ind();
    catprintf("$stmt %s |0x%p ",
	      token_name(node->base),
	      node_address);
    print_loc(s.where);
    *arg->output_stream << "\n";

    ind();
    *arg->output_stream << "where:\t";
    print_loc(s.where);
    *arg->output_stream << "\n";

    print_ptr("s", s.s);
    print_ptr("s_list", s.s_list);
    print_ptr("memtbl", s.memtbl);
    switch(s.discriminator(0)) {
      case 1: print_ptr("d", s.d); break;
      case 2: print_ptr("e2", s.e2); break;
      case 3: print_ptr("has_default", s.has_default); break;
      case 4: nz_printf("case_value:\t0x%x", s.case_value);
      case 5: print_ptr("ret_tp", s.ret_tp); break;
    }
    switch(s.discriminator(1)) {
      case 1: print_ptr("e", s.e); break;
      case 2: nz_printf("own_tbl:\t%d", s.own_tbl); break;
      case 3: print_ptr("s2", s.s2); break;
    }

    switch(s.discriminator(2)) {
	case 1: print_ptr("for_init", s.for_init); break;
	case 2:	print_ptr("else_stmt",s.for_init); break;
	case 3: print_ptr("case_list", s.case_list); break;
    }
}

void
displayer::a_enumdef ()
{
   struct enumdef& e = *Penum(node);

   printf("$enumdef %s |0x%p",
	  token_name(node->base),
	  node_address);

   nz_printf( "e_body:\t\t%d", e.e_body);
   nz_printf( "no_of_enumerators: %d", 
	     e.no_of_enumerators);
   nz_printf( "strlen:\t%d", e.strlen);
   print_string ("string", (void *)e.string);

   print_ptr("mem", e.mem);
   print_ptr("e_type", e.e_type);
   indent += Indent_Increment;
   a_type();			/* do the supertype. */
   indent -= Indent_Increment;
}

void
displayer::a_classdef ()
{
    struct classdef& c = *Pclass(node);

    ind();
    catprintf("$classdef %s |0x%p ",
	      token_name(node->base),
	      node_address);
    print_string_brief(c.string);
    *arg->output_stream << "\n";
    ind();
    catprintf("class_base:\t");
    switch(c.class_base) 
	{ 
	  case vanilla_class:
	    *arg->output_stream << "vanilla_class\n";
	    break;
	  case template_class:
	    *arg->output_stream << "template_class\n";
Template_Class:	    
	    nz_printf("inst:\t\t%p", (int)Ptclass(&c)->inst);
	    break;
	  case instantiated_template_class:
	    *arg->output_stream << "instantiated_template_class\n";
	    goto Template_Class;
	  case uninstantiated_template_class:
	    *arg->output_stream << "uninstantiated_template_class\n";
	    goto Template_Class;
	  case relationship_class:
	    *arg->output_stream << "relationship_class\n";
	    break;
	  case defining_class:
	    *arg->output_stream << "defining_class\n";
	    break;
	}

    nz_printf( "c_body:\t\t%d", c.c_body);
    printf("csu:\t\t%s", 
	   token_name(c.csu));
    nz_printf( "obj_align:\t%d", c.obj_align);
    if(c.c_xref) {
	ind();
	catprintf( "c_xref:\t%x", c.c_xref);
	flag(c.c_xref & 1, "has-vptr ");
	flag(c.c_xref & 2, "X(X&)-defined ");
	flag(c.c_xref & 4, "operator=(X&)-defined ");
	flag(c.c_xref & 8, "has-vbaseptr(s) ");
	*arg->output_stream << "\n";
    }
    nz_printf( "virt_count:\t%d", c.virt_count);
    nz_printf( "virt_merge:\t%d", c.virt_merge);
    nz_printf( "c_abstract:\t%d", c.c_abstract);
    nz_printf( "has_vvtab:\t%d", c.has_vvtab);
    nz_printf( "strlen:\t%d", c.strlen);
    print_string ("string", (void *)c.string);
    nz_printf( "obj_size:\t%d", c.obj_size);
    nz_printf( "real_size:\t%d", c.real_size);
    nz_printf("lex_level:\t%d", c.lex_level);
    if(c.lcl)print_string("lcl", c.lcl);

    print_ptr("baselist", c.baselist);
    print_ptr("mem_list", c.mem_list);
    print_ptr("memtbl", c.memtbl);
    print_ptr("friend_list", c.friend_list);
    print_ptr("pubdef", c.pubdef);
    print_ptr("tn_list", c.tn_list); /* not brief */
    print_ptr("in_class", c.in_class);
    print_ptr("in_fct", c.in_fct);
    print_ptr("this_type", c.this_type);
    print_ptr("virt_list", c.virt_list); 
    print_ptr("c_ctor", c.c_ctor);
    print_ptr("c_dtor", c.c_dtor);
    print_ptr("c_itor", c.c_itor);
    print_ptr("conv", c.conv);
    indent += Indent_Increment;
    a_type();			/* do the supertype. */
    indent -= Indent_Increment;
}


void
displayer::a_virt()
{
    virt& v = *Pvirt(node);
    void * vna;
    void * vna_e;
    int vx;

    printf("$virt %s |0x%p ",
	   token_name(node->base),
	   node_address);
    
    print_ptr("next", v.next);
    if(v.n_init) {
	printf("n_init:\t%d", v.n_init);
	printf("virt_init:");
	for (vna = v.virt_init, vx = 0; vx < (v.n_init - 1);
	     vna = (char *)vna + sizeof (velem), vx++)
	    {
		fetch(vna, sizeof (velem), vna_e);
		struct velem * ve = (struct velem *) vna_e;
		ind();
		catprintf("%d offset %d\t", vx, ve->offset);
		print_ptr(ve->n);
		free_fetched(vna_e);
	    }
    }
    print_ptr("vclass", v.vclass);
    nz_printf( "\nis_vbase:\t %d", v.is_vbase);
    nz_printf( "\nprinted:\t %d", v.printed);
}

void
displayer::a_table ()
{
   struct table& t = *Ptable(node);
   int x;
   int y;
   void * nlist;
   void * slist;

   printf("$table %s |0x%p",
	  token_name(node->base),
	  node_address);

    printf("init_stat:\t%d%s", t.init_stat,
	   t.init_stat == 0 ? " not simplified" :
	   t.init_stat == 1 ? " simplified, no inits" :
	   t.init_stat == 2 ? " simplified, inits" :
	   "");
   printf("size:\t%d", t.size);
   printf("hashsize:\t%d", t.hashsize);
   printf("free_slot:\t%d", t.free_slot);

   /* it looks like there is an array of Pnames here ... */
   *arg->output_stream << "\n";
   ind();
   *arg->output_stream << "entries:\n";
   fetch((void *)t.entries, t.size * sizeof(Pname), nlist);
   Pname * tmp_nlist = (Pname *)nlist;
   for(x = 0; x < t.size; x ++) {
       if(tmp_nlist[x]) {
	   ind();
	   *arg->output_stream << x;
	   *arg->output_stream << "\t";
	   print_ptr(tmp_nlist[x]);
	   unsigned char tmp_name_s[sizeof(name)];
	   void * tmp_name;
	   tmp_name = (void *)&tmp_name_s[0];
	   fetch(tmp_nlist[x], sizeof(name), tmp_name);
	   catprintf(" key %d\n", Pname(tmp_name)->n_key);
       }
   }
   free_fetched(nlist);
   /* and then an array of shorts for the hash table. */
   /* we really need a tabular format here, alright. */
   ind();
   *arg->output_stream << "hashtbl:\n";
   fetch(t.hashtbl, t.hashsize * sizeof(short), slist);
   short * tmp_slist = (short *)slist;
   for(x = 0;; x++ ) {
       for (y = 0; y < 11; y ++) {
       if(((x * 12) + y) > t.hashsize) break;
	   catprintf("%6d ", tmp_slist[(x*12)+y]);
       }
       *arg->output_stream << "\n";
       if(((x * 12) + y) > t.hashsize) break;
   }

   free_fetched (slist);

   print_ptr("t_name", t.t_name);
   print_ptr("real_block", t.real_block);
   print_ptr("name", t.t_name);
   print_ptr("next", t.next);
}

void
displayer::a_gen ()
{
    struct gen& g = *Pgen(node);

    printf("$gen %s |0x%p",
	   token_name(node->base),
	   node_address);

    print_ptr ("fct_list", g.fct_list);
    indent += Indent_Increment;
    a_type();			/* do the supertype. */
    indent -= Indent_Increment;
}

void
displayer::a_vec ()
{
   struct vec& v = *Pvec(node);

   printf("$vec %s |0x%p",
	  token_name(node->base),
	  node_address);
   printf("size:\t\t%d", v.size);
   print_ptr ("typ", v.typ);
   print_ptr ("dim", v.dim);
}

void
displayer::a_ptr ()
{
    struct ptr& p = *Pptr(node);

    printf("$ptr %s |0x%p",
	   token_name(node->base),
	   node_address);
    ind();
    printf("rdo:\t\t%d", p.rdo);
    ind();

    print_ptr ("typ", p.typ);
    print_ptr ("memof", p.memof);
}


void
displayer::a_nlist()
{
    name_list& n = *Plist(node);

    printf("$name_list %s |0x%p ",
	   token_name(node->base),
	   node_address);
    
    print_ptr("f", n.f);
    print_ptr("l", n.l);
}

void
displayer::a_iline()
{
    struct iline& i = *(struct iline *)node;

    printf("$iline %s |0x%p ",
	   token_name(node->base),
	   node_address);
    
    print_ptr("fct_name", i.fct_name);
    print_ptr("i_next", i.i_next);
    print_ptr("i_table", i.i_table);
    nz_printf("i_slots:\t%d", i.i_slots);
    print_ptr("i_args", i.i_args);
}

void
displayer::a_ia()
{
    struct ia& i = *(struct ia *)node;

    printf("$ia %s |0x%p ",
	   token_name(node->base),
	   node_address);
    
    print_ptr("local", i.local);
    print_ptr("arg", i.arg);
    print_ptr("tp", i.tp);
}

static int fetcher (void * info,
		    void * pointer,
		    unsigned long length,
		    int zero_stop,  /* for character strings.*/
		    void * target)
{
    displayer * d = (displayer *)info;
    if(d->arg->fetcher)
	return d->arg->fetcher(d->arg->fetcher_info, pointer, length,
			       zero_stop, target);
}

static void 
do_node (Pnode& node, node_class cl, void * info, tree_node_action& action,
	 int depth, Pnode na, tree_walk_tree&, int&)
{
    displayer * d = (displayer *)info;
    d->do_node(node, cl, action, depth, na);
    d->arg->output_stream->write("\n", 1);
}


/* Note -- declared extern "C" in tree_dump.h. */

void
display_cfront_node (dcn_arg& arg, Pnode n)
{
    tree_walk_control twc;
    displayer d (arg);
    
    if(arg.verbose == dt_normal)arg.max_depth = 0;
    twc.callback_info = (void *)&d;
    if(arg.fetcher) twc.fetcher = fetcher;
    twc.action_proc = do_node;
    twc.error_stream = arg.error_stream;
    twc.nodes_seen_hash = arg.nodes_seen_hash;
    twc.resolve_by_name = 0;	/* we want to display the by_name nodes */
    twc.dont_chase_lists_top = 1; /* don't display n_list or s_list peers
			             at the top level. */

    walk_tree(twc, n);

    arg.error_stream->flush();
    arg.output_stream->flush();
}


extern "C" void _fcout();
void _fcout () { cout.flush(); }
