/* ident "@(#)ctrans:src/template.h	1.2" */
/* -*- Mode:C++ -*- */
/*
	$Source: /usr3/lang/benson/work/stripped_cfront/RCS/template.h,v $ $RCSfile: template.h,v $
	$Revision: 1.4 $		$Date: 90/04/02 11:31:35 $
	$Author: sam $		$Locker:  $
	$State: Exp $
*/
/*
    $Header: /usr3/lang/benson/work/stripped_cfront/RCS/template.h,v 1.4 90/04/02 11:31:35 sam Exp $

    Copyright 1989 by Object Design, Inc., Burlington, Mass.
    All rights reserved.

*/


/*****************************************************************************
* 									     *
* This file contains types pertinent to the implementation of the	     *
* parametrized type facility.						     *
* 									     *
*****************************************************************************/

enum bool { true = 1, false = 0 } ;

typedef class templ *Ptempl ;

typedef class templ_inst *Ptempl_inst ;

typedef class function_template *Pfunt ;

typedef class templ_classdef *Ptclass ;

typedef class tree_template *Ptreet ;


// A Lisp style cons cell to help build lists. The parametrized type facility,
// should obviate the need for this type-unsafe nonsense.
class cons {
public:
  void *car ;
  cons *cdr ;
  cons (void *pcar, cons *pcdr) { car = pcar ; cdr = pcdr ; } ;
} ;

typedef cons *Pcons ;



/*****************************************************************************
* 									     *
* The class template_compilation holds the state, and the associated methods *
* used during template compilation. There is exactly one instance of the     *
* type, it is mainly a device used to enforce modularity.  In reality, it    *
* would never need to be instantiated since all it's members are static.     *
* However, since the type::mem for of reference is not supported as	     *
* yet(cfront 1.2), we need an instantiation to get to the members via	     *
* pointer syntax instead.						     *
* 									     *
* A templ_compilation holds the state relevant to the syntax analysis of a   *
* class or member function template definition.				     *
* 									     *
*****************************************************************************/

class templ_compilation {
  static  Plist param_end,   // make append easier
                param_tn ;   // The type names introduced by the parameters

public:
  static Ptempl  list ;      // The list of templates for the compilation
  static Plist   params ;    // The list of parameters to the template
  static Ptempl  owner  ;    // The template when compiling a member
                             //     function.
  static table   *templates ; // The templates defined during this compilation
  
  static Ptreet  tree_templates; // The list of tree templates

  static bool    in_progress; // a template compilation is in progress
  // instantiation parameter parsing in progress. Used in the lexer to ensure
  // that name string are consed in the heap, rather than being retained as
  // pointers into the lex buffer.
  static int     parameters_in_progress ;

  // the list of templates referenced by the top level definition being compiled.
  static Pcons   templ_refs ;
  static Pcons   last_cons ;
  static Ptype   any_type ;  // canonical "ANY" type for formal parms
  
  // true, if currently compiling an expression tree template
  static TOK   curr_tree_template ;

  void append_ref(Ptempl_inst ref) ;
  void start() ;
  
  void collect(TOK parm_type, Pname namep) ;
  void collect(Pname namep) ;

  void enter_parameters() ;
  void introduce_class_templ(Pname cnamep) ;
  void end(Pname class_name) ;
  Pname forward_declaration(Pname class_name) ;
  void instantiate_ref_templ() ;
  void clear_ref_templ() ;

  Pname check_tname(Pname p) ;
  bool  current_template(Pname p, Pexpr actuals) ;
  Ptempl is_template(Pname p) ;
  Ptempl is_template(char *s);
  void end_of_compilation() ;   // Done with compilation, instantiate bodies
  templ_compilation()  ;

  static Pname tree_parameter(char *s) ; // return true if the name is a tree parameter

private:
  void append_parameter(Pname p) ;
  
} ;



// The canonical template_compilation instance. templ_compilation exists as a
// class simply to provide a code and data packaging mechanism. There is
// exactly one generated instance of it.

extern templ_compilation *templp ;

// should actually be static member functions of templ_compilation
Pname parametrized_typename (Pname p, Pexpr actuals) ;
Pbase parametrized_basetype (Pname p, Pexpr actuals) ;




// the basis for class and member function templates
class basic_template {
  friend templ_compilation ;
protected:
  Plist       formals ;        // The formal arguments to the template
  Pcons       templ_refs ;     // The templates referenced by this template

  // check class constraints placed on formals while processing member
  // function bodies. 
  bool check_constraints(Pexpr actuals) ;
  
  // ensure that use of formals is consistent across, class, member and
  // forward declarations
  void check_formals(Plist formals) ;

public:
  // used to order template class definitions for instantiations. Not being
  // used as yet.
  int        definition_number ;

  // used to generate the definition numbers, used by the above member.
  static int definition_tick ;
	 
  Plist get_formals() {return formals ;}
} ;



// the template for a class
class templ  : public basic_template { 
  Pbase       basep ;          // COBJ basetype for the template
  Ptempl_inst insts  ;         // instantiations of the template
  Pfunt       fns  ;           // member function declarations
  Pfunt       fns_end ;        // last fun in the above list
  
  // Use these state variables to set up the correct state for error
  // processing. They are used by the "error" routines for statement numbers.
  Pname       Cdcl ;
  Pstmt       Cstmt ;
  
  friend templ_inst ;
  friend function_template ;

  // used to detect loops during instantiation; a count greater than two is
  // indicative of a non-terminating instantiation sequence
  int         open_instantiations ;
  
  Ptempl_inst get_match(Pexpr actuals,
			Ptempl_inst exclude,
			bool match_instantiated_only) ;
  bool check_actual_args (Pexpr actuals) ;

public:
  Ptempl      next ;           // connects all the class templates in the comp
  Pname       namep ;          // the TNAME for the template class
  bool        defined ;        // the actual definition, not just a forward
  			       // declaration has been seen.

  Pname       members ;        // note the members to catch redefinition errors

  Ptempl_inst get_inst(Pexpr actuals, Ptempl_inst exclude = 0) ;
  templ(Plist parms, Pname p) ;
  void resolve_forward_decl(Plist parms, Pclass c) ;
  void instantiate_forward_decl() ;
  
  // The uninstantiated base type
  Pbase basetype() {return basep; }
  // The basetype for a specific instantiation
  Pbase inst_basetype(Pexpr actuals) ;

  Pname typename(Pexpr actuals) ;
  
  Pfunt collect_function_member(Pname fname) ;
  bool  has_tree_expr_formals() ;
  bool instantiate_bodies() ;
} ;



// Member function templates
class function_template : public basic_template {
  Pname       fn ;             // The name of the member function
  Pfunt       next ;           // connects the list of member functions

  friend templ ;
  friend templ_inst ;

public:
  function_template (templ & owner, Plist params, Pname n) ;
} ;



// compiler internal expression templates, used to implement Objectstore constructs
class tree_template : public basic_template {
  TOK         kind ;          // STATEMENT or EXPRESSION
  Pnode       e ;             // the post-syntax tree representing the
			      // expression or statement constituting the
                              // template.
  Ptreet      next ;          // the next expression template
  static      int count ;     // the number of instantiations
				 
public:
  char        *string ;       // the name used for template lookup
  tree_template(TOK tree_kind, char *s, Plist params, Pnode tree,
		Pcons references) ;
  static  Ptreet get(char *s) ;
  Pname   get_parameter(char *s) ;
  static  void test() ;
  Pexpr expand(Pexpr actuals) ;
} ;




// Global state variables that must be saved around an instantiation. The
// saving of state was required in the implementstion that interspersed decl
// processing and instantiation, rather than the current strategy, which only
// forces instantiations at the top level outside of any dcl-processing
// context. It is retained in case we ever go back to the "interspersed" style
// of instantiation. 
class state {

public:
  Pname       Cdcl ;      // the global variables used by the error routines
  Pstmt       Cstmt ;
  Pname       dcl_list ;  // Holds the list of typedef names that are hidden
  Loc         curloc ;
  
  int         curr_file ;
  Pexpr       curr_expr ;
  Pin         curr_icall ;
  Pstmt       curr_loop;
  Pblock      curr_block;
  Pstmt       curr_switch;

  int         bound  ;   
  int         inline_restr ;
  Loc         last_line ;

  int         no_of_badcall;
  int         no_of_undcl ;
  
  Pname       badcall ;
  Pname       undcl ;

  state() {} ; // prevent used before set warnings.
  void save() ;
  
  void init() ; 
  void restore() ; 
} ;


class pointer_hash ; 
class tree_copy_info ;


// A template starts out being uninstantiated, and is class_instantiated when
// there is a refrence to it with actual arguments. It is body_instantiated at
// the end of compilation, when all its function members are instantiated.
enum inst_status { uninstantiated, class_instantiated, body_instantiated };

// templ_inst captures the arguments used in the instantiation of a template.
// These instantiations are rooted in the templ object. 
class templ_inst {
  
  friend class template_instantiation ;

  Pname      tname ;         // The instantiation name, it is the TNAME that
                             // leads up to an actual instantiation of the class
  Pname      namep ;         // The version of TNAME in the ktbl
  Pexpr      actuals ;       // instantiation arguments, chained using ELIST
			     // as an expression "cons" node, e1 is the car
			     // and e2 the cdr. The car points to a name node.
  Ptempl_inst next ;         // The linked list of instantiations for this
                             // template.
  Ptempl_inst next_active ;  // The list of currently active instantiations.
  state       context ;      // the context of this instantiation
  Ptempl_inst forward ;      // This instantiation is the same as the one
                             // pointed to.

  // Contains the list of global names that are hidden during an
  // instantiation.
  Plist hidden_globals ;

  // The class correspondence table. This table is initialized 
  // when the class definition is instantiated. Subsequently, it is used to
  // initial member correspondence tables before the copy process is
  // initiated.
  pointer_hash        *corr ;

  // the instantiation's copy of the formals
  Plist  inst_formals ;
       
  inst_status status ;
  
  friend class templ ;
  friend class templ_classdef ;
  friend class tree_template ;
  friend Pcons make_ref_copy(pointer_hash &h, tree_copy_info &info,
			     Pcons old_templ_refs);
  
  templ_inst (Pexpr act,  Ptempl owner) ;
  bool actuals_match(Pexpr check_actuals) ;
  void instantiate_match(Ptempl_inst match) ;
  void kludge_copy(Pbase source_base) ;
  // create a copy of the class type subtree preparatory to the ensuing
  // instantiation. Return a non-zero value, only if there was no need to
  // create a copy, ie. an identical instantiation already existed.
  Ptempl_inst class_copy(Pcons &templ_refs, bool recopy) ;
  Pcons ref_copy(pointer_hash &h, tree_copy_info &info, Pcons old_templ_refs) ;
    
  // save and restore state around the template instantiation
  void save_state(Pname p) ;
  void restore_state() ;
  
  // Used to collect references to this template by a definition
  Ptempl_inst note_ref() ;
  char *instantiation_string() ;
  
  void expose_parameter_names() ;
  void hide_parameter_names() ;
  
public:
  Ptempl        def ;           // The template definition, for which this is an
			     // instantiation.
  bool         refp ;        // A flag used to note template references during
			     // a C++ definition
  void print_error_loc() ;   // Wants to be a static function
  // Bind the formals before an instantiation
  void bind_formals() ;

  Ptempl_inst canonical_instantiation() {
    return ( forward ? forward : this ) ;
  }

  // get the class associated with this instantiation
  Pclass get_class() { return Pclass(Pbase(tname->tp)->b_name->tp) ;}
			      
  void instantiate(bool reinstantiate = false) ;
  static Ptempl_inst head ;  // Head of the list of active instantiations.
  void print_pretty_name() ;
  char *mangled_name(char *buffer) ;
  // The uninstantiated basetype
  Pbase def_basetype() { return def->basep ; } ;
  // A general way of determining whether two template instantiations are
  // the same
  bool  same(Ptempl_inst t) ;
  bool copy_hook(Pnode&) ;
  // return a copy of the function tree starting with it's name
  Pname function_copy(Pfunt fnt, Pcons &templ_refs) ;

  // special check for instantiations used in qualifiers for template function
  // member declarations. 
  bool check_qualifier(Plist formals) ;
  Pname get_parameter(char *s) ;
} ;



// Experimental debugging toggle
extern int zdebug  ;


// The class node used for template classes.
// Rep invariant:
// class_base == uninstantiated_template_class ||
// class_base  == instantiated_template_class

class templ_classdef : public classdef {
public:
  Ptempl_inst inst ;              // a pointer to the instantiation; the
				  // instantiation also points back to it via
                                  // the tname ->cobj->name->class path
  templ_classdef(Ptempl_inst i) ;
  Pname unparametrized_tname() { return inst->def->namep ; } 
  void instantiate() { inst->instantiate() ; }

} ;


// Safe accessor functions for navigating through COBJ base classes

extern class_type_enum get_class_base (Pbase b) ;

extern Ptclass get_template_class (Pbase b) ;

extern Ptempl_inst get_templ_inst(Pbase b) ;
 

/*
   $Log:	template.h,v $
 * Revision 1.4  90/04/02  11:31:35  sam
 * Made comments current.
 * 
 * Revision 1.3  90/03/30  18:50:55  sam
 * 1) Added the introduce_class_templ member function to the templ_compilation 
 * class.
 * 2) Rationalized the definition of class templ. Made basic_template be it's
 * base class, instead of type. The latter was the base type, simply so that
 * it could be stored in a table associating strings with types. Don't have
 * to resolve to such kludgery in the PT world.
 * 3) Added the member open_instantiations to detect non-terminating
 * instantiations
 * 
 * Revision 1.2  90/03/27  10:16:27  sam
 * > Merged in revision 1.13 from the main line of development 
 * 
 * Revision 1.1  89/11/20  08:50:58  benson
 * Initial revision
 * 
 * Revision 1.11  89/10/16  15:25:00  sam
 * use pointer_hash rather than Hash as the type of the correspondence table.
 * 
 * Revision 1.10  89/09/26  16:44:34  sam
 * fix the forward declaration of template classes
 * 
 * Revision 1.9  89/09/18  16:37:37  sam
 * provide for error recovery upon argument mismatch in a template instantiation
 * 
 * 
 * Revision 1.8  89/09/15  09:31:50  benson
 * move tree_template.string into the public section.
 * 
 * Revision 1.7  89/08/30  13:02:12  sam
 * added support for dealing with __expressions in class formal templates
 * 
 * Revision 1.6  89/08/28  09:42:05  sam
 * Support for nested references in internal templates. These nested 
 * references are instantiated whenever an expansion takes place.
 * 
 * Revision 1.5  89/08/23  10:26:00  sam
 * support for BS style formal syntax. refer to 1.5 templates.c for more 
 * detailed comments.
 * 
 * Revision 1.4  89/08/11  14:57:09  sam
 *  implementation of multiple instantiation templates.
 * 
 * Revision 1.3  89/07/27  11:33:40  sam
 * the comments in template.c 1.3 describe the modifications.
 * 
 * Revision 1.2  89/07/07  14:34:11  sam
 * Added the templ_classdef::unparametrized_type_name() function member, as part
 * of the base init fix.
 * 
 * Revision 1.1  89/06/29  09:21:32  benson
 * Initial revision
 * 
 * Revision 1.1  89/06/22  16:29:29  sam
 * Initial revision
 * 


   end_log
*/



/*
   $Log $


   end_log
*/
