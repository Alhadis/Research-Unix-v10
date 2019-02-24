/* This may look like C code, but it is really -*- C++ -*- */

/*
	$Source: /usr3/lang/benson/work/stripped_cfront/RCS/template.c,v $ $RCSfile: template.c,v $
	$Revision: 1.4 $		$Date: 90/04/02 11:32:09 $
	$Author: sam $		$Locker:  $
	$State: Exp $
*/
#include "tree_copy.H"
#include "cfront.h"
#include <string.h>
#include "dump_tree.H"
#include "template.h"
#include <stdlib.h>
#include <ctype.h>
#include "hash.h"

/*****************************************************************************
* 									     *
* This file contains most of the implementation for a subset of parametrized *
* types as defined by the BS paper 1989 JOOP paper.  The subset chosen here  *
* was the subset relevant to the implementation of aggregates in	     *
* ObjectStore.								     *
* 									     *
* 									     *
* The following is a list of features supported by the implementation. The   *
* list of features parallels the description of templates in the JOOP paper. *
* 									     *
* 	a) Class templates supported.					     *
* 									     *
* 	b) Member function templates supported.  Type-specific member	     *
* 	functions as described at the end of the "Outline of an		     *
* 	implementation" section are also supported. Non-member function	     *
* 	templates are not supported.					     *
* 									     *
* 	c) Template arguments may be of type "type" or simple integral,	     *
* 	real, double and pointer types that are compile-time constants.	     *
* 	Deafult arguments are not suported.				     *
* 									     *
* 									     *
* Restrictions:								     *
* 									     *
* 	a) template definitions may not be nested.			     *
* 									     *
* 	b) enums, or class definitions may not be nested within a template   *
* 	class definition.						     *
* 	   								     *
* This file also supports an internal template facility to facilitate the    *
* implementation of ObjectStore data model features. The internal template   *
* facility is only used by compiler implementors, and is not user visible.   *
* 									     *
*****************************************************************************/


/*****************************************************************************
* 									     *
* TBD									     *
* 									     *
* 1) Error recovery, a never ending task, could stand improvement.	     *
* 									     *
* 2) The template copying process could probably be speeded up		     *
* substantially, by only placing "graph-like" nodes in the hash table. The   *
* current implementation plays it safe, and places all nodes in the hash     *
* table.								     *
* 									     *
* 3) Clean up $name processing, it needs to be remodularized so that class   *
* templates and tree templates share the code.				     *
* 									     *
* 4) Permit parametrized name default names within member functions. Gordon  *
* needs this, for inverse templates.					     *
* 									     *
*****************************************************************************/


extern int bound ; // is not mentioned in the header file

const int max_string_size = 1024 ;

const int default_copy_hash_size = 1000 ;

// Save and restore global state around a template instantiation
void state::save() {
  
  Cdcl = ::Cdcl ;
  Cstmt = ::Cstmt ;
  curloc   = ::curloc ;

  curr_file = ::curr_file ;
  curr_expr = ::curr_expr ;
  curr_icall = ::curr_icall ;
  curr_loop = ::curr_loop;
  curr_block = ::curr_block;
  curr_switch = ::curr_switch;

  bound    =  ::bound  ;   
  inline_restr = ::inline_restr ;
  last_line = ::last_line ;

  no_of_badcall = ::no_of_badcall;
  no_of_undcl = ::no_of_undcl ;
  
  badcall = ::badcall ;
  undcl = ::undcl ;
} ;


void state::restore() {
  ::Cdcl = Cdcl ;
  ::Cstmt = Cstmt ;
  ::curloc  = curloc ;

  ::curr_file = curr_file ;
  ::curr_expr = curr_expr ;
  ::curr_icall = curr_icall ;
  ::curr_loop = curr_loop;
  ::curr_block = curr_block;
  ::curr_switch = curr_switch;

  ::bound    =  bound  ;   
  ::inline_restr = inline_restr ;
  ::last_line = last_line ;

  ::no_of_badcall = no_of_badcall;
  ::no_of_undcl = no_of_undcl ;
  
  ::badcall = badcall ;
  ::undcl = undcl ;
} ;


void state::init()  {
  ::bound = 0 ;
  ::inline_restr = 0 ;
  ::no_of_badcall = ::no_of_undcl = 0 ;
  ::undcl = ::badcall = NULL ;
  // lastline  needs to be initialized probaly via a call to putline
  
}


bit basetype::parametrized_class()
{ return ((base == COBJ) && 
	  Ptclass(Pbase(this)->b_name->tp)->class_base == uninstantiated_template_class) ;
}

class_type_enum get_class_base (Pbase b) {
  if (b->base != COBJ) error('i', "bad argument top ::get_class_type") ;
  return Ptclass(Pbase(b)->b_name->tp)->class_base ;
}

Ptclass get_template_class (Pbase b) {
  class_type_enum t = get_class_base(b) ;

  if (! ((t == instantiated_template_class) ||
	 (t == uninstantiated_template_class)))
    error ('i', "class is not a template class") ;
  
  return Ptclass(Pbase(b)->b_name->tp) ;
}


Ptempl_inst get_templ_inst(Pbase b) {

  return (get_template_class(b))->inst ;
}

  
bit classdef::parametrized_class()
{ return (class_base == uninstantiated_template_class) ;
}

// Predicate to determine whether two classes are indeed the same. cfront
// normally relies on pointer identity, however this test is insufficient when
// parametrized class instantiationa are involved, since there are potentially
// many instances of a COBJ and CLASS for a given instantiation.
bit classdef::same_class(Pclass pc)
{
  if (this == pc) return true ;

  // An intermediate test to compensate for the fact that instantiations do
  // not cause a copy of the syntax tree to be generated. This kludge should
  // not be necessary once the template implementation is complete, and tree
  // copying is implemented.
  // Later, Sam: tree copying is now implemented, i need to remove the
  // following two statements and rerun the test suite.

  if ((this->class_base == template_class) &&
      (pc->class_base == instantiated_template_class) &&
      (Ptclass(pc)->inst->def_basetype()->b_name->tp == this))
    return true ;
  // The inverse symmetric test
  if ((pc->class_base == template_class) &&
      (this->class_base == instantiated_template_class) &&
      (Ptclass(this)->inst->def_basetype()->b_name->tp == pc))
    return true ;

  // Check whether the templates were determined to be identical after
  // instantiation. 
  if ((pc->class_base == instantiated_template_class) &&
      (this->class_base == instantiated_template_class) &&
      (Ptclass(this)->inst->same(Ptclass(pc)->inst)))
    return true ;
  return false ;
}

// determine whether two instantiations are identical; the test asumes that 
// the templates have been instantiated.
bool templ_inst::same(Ptempl_inst t)
{
  return ((forward && (forward == t->forward)) ||
	  (forward == t) || (t->forward == this)) ? true : false ;
}


                      /* Template parsing support */

// The canonical template compilation instance. 

templ_compilation *templp ;

templ_compilation::templ_compilation()
{ templates = new table(128, NULL, NULL) ;
  any_type =  new basetype(ANY, NULL);
  PERM(any_type) ;
}


// determine whether the string corresponds to a tree formal parameter
Pname templ_compilation::tree_parameter(char *s) {
  for (Plist formal = params ; formal ; formal = formal->l)
    if (strcmp(formal->f->string, s) == 0) {
      formal->f->n_used++ ;
      return formal->f ;
    }
  return 0 ;
}  
  
				       
// Determine whether the name refers to the canonical template class during
// syntax analysis.
Ptempl templ_compilation::is_template(Pname p) {
  if (p->tp && (p->tp->base == COBJ) &&
      (get_class_base(Pbase(p->tp)) == template_class))
    { Pname n = templates->look(p->string, 0) ;
      return (n ? Ptempl(n->tp) : 0) ;
    }
  return 0 ;
}


// determine whether the string names a template
Ptempl templ_compilation::is_template(char *s) {
  Pname n = templates->look(s,0) ;
  return (n ? Ptempl(n->tp) : 0) ;
}

  
// Set up the environment for parsing a template. This involves setting up a
// new nesting level into which the "type type" parameters of the template can
// be entered, so that the lexer can find them as TNAMES. The scope is
// deallocated by end().
void templ_compilation::start() 
{ templp->in_progress = true ;
  // Reinitialize the state.
  params = param_end = NULL ; owner = NULL ;
  modified_tn = 0 ;  // Initialize it here, since ::collect adds new types
}


  
// Collect each parameter as it is parsed, and add it to the list of parms.
// Validate each parameter to make sure that it is one of the acceptable
// types. 
void templ_compilation::collect(TOK parm_type, Pname n)
{
  switch (parm_type) {
  case CLASS:
    // A "type type" parameter, give it the "ANY" type normally used as a
    // wildcard match internally by the compiler in cases of error.
    n->tp =  new basetype(ANY, 0);
    n =  n->tdef() ;           // Set it up to be a typedef.
    n->lex_level = bl_level + 1 ; // Inner scope, so restore() can hack it
    n->n_template_arg = name::template_type_formal ;
    PERM(n) ; PERM(n->tp) ;
    break ;
  case STATEMENT:
  case EXPRESSION:
    // the argument is a post-syntax expression tree
    n->n_template_arg =
      ((parm_type == EXPRESSION) ? 
       name::template_expr_tree_formal : name::template_stmt_tree_formal) ;
    // canonical any_type is ok here
    n->tp = any_type ;
    PERM(n) ;
    break ;
  default:
    error("the parameter type for %n must be CLASS, not %k",
	  n, parm_type) ;
  }
  append_parameter(n) ;
}

// append the "non-type" parameter to the end of the list
void templ_compilation::append_parameter(Pname n)
{
  if (params){
    param_end->l = new name_list(n, NULL) ;
    param_end = param_end->l ;
  }else params = param_end = new name_list(n, NULL) ;
  PERM(n) ; PERM(n->tp) ;
}


// collect non "type type" parameters. The tp field of the name
// indicates the type of the formal parameter. 
void templ_compilation::collect(Pname n) 
{
  // The grammar alone should be sufficient to protect against undesirable
  // types. Any additional checks go here.
  n->n_template_arg = name::template_expr_formal ;
  append_parameter(n) ;
}
  


// validate the type for a non-type formal, and make it a const.
static void check_non_type_formal(Pname n) {

  switch (n->tp->base) {
  case ZTYPE:
  case CHAR:
  case SHORT:
  case INT:
  case LONG:
  case FLOAT:
  case DOUBLE:
  case FIELD:
  case EOBJ:
  case COBJ:
  case TYPE:
  case ANY:
    { // a basetype node
      TOK bad_base = 0 ;
      
      if (Pbase(n->tp)->b_volatile)
	bad_base = VOLATILE ;
      if (Pbase(n->tp)->b_typedef)
	bad_base = TYPEDEF ;
      if (Pbase(n->tp)->b_inline)
	bad_base = INLINE ;
      if (Pbase(n->tp)->b_virtual)
	bad_base = VIRTUAL ;
      if (bad_base)
	error ("bad %k declarator for template formal %n", bad_base, n) ;
      
      Pbase b = new basetype(0, 0) ;
      *b= *Pbase(n->tp) ;
      b->b_const = 1 ;
      n->tp = b ;
      break  ;
    }
  case PTR:
    { Pptr b = new ptr(0,0) ;
      *b = *Pptr(n->tp) ;
      
      b->rdo = 1;
      n->tp = b ;
      break ;
    }
  case RPTR:
  case VEC:
    break;		// constant by definition
  default:
    error ("bad parameter type %t for formal parameter %n", n->tp, n) ;
  }
  return ;
}
		


// The template parameters if any, have been parsed. Member function templates
// may choose to default their template arguments to the class arguments, if
// so, make the defaulting happen.
void templ_compilation::enter_parameters()
{
  for (Plist list = params ; list ; list = list->l) {
    Pname n = list->f ;
    switch(n->n_template_arg) {
    case name::template_type_formal:
      // Set them up for restoration
      modified_tn = new name_list(n,modified_tn);
      // Bring the names out of hiding
      n->n_key = 0 ;
      break ;
    case name::template_expr_formal:
      check_non_type_formal(n) ;
      n->tp->dcl(gtbl) ;
      break ;
    case name::template_expr_tree_formal:
    case name::template_stmt_tree_formal:
      // simply note it, the guts of the processing takes place when the
      // copy of the syntax tree is generated.
      break ;
    default:
      error ('i', "bad template formal" ) ;
    }
  }
  // Save away the list of modified_tn, since the body processing will clobber
  // it.
  param_tn = modified_tn ;
  modified_tn = 0 ;
}



// Resolve the forward declaration of a template to its true definition. The
// template and class type data structures must be reused, since there may be
// outstanding references to them.
void templ::resolve_forward_decl(Plist params, Pclass c) {
  check_formals(params) ; 
  formals = params ;
  defined = true ;
  definition_number = ++ definition_tick ;
  members = c->mem_list ;
}

void templ::instantiate_forward_decl() {
  for (Ptempl_inst i = insts ; i ; i = i->next)
    if (Ptclass(Pbase(i->tname->tp)->b_name->tp)->class_base ==
	instantiated_template_class &&
	! i->forward)
      { // reinstantiate it
	i->instantiate(true) ;
      }
}

  

// verify thet the qualifier used to declare the member function matches the
// template arguments in name, ie.
// template <class P, class Q, ..> c<P,Q,..>::member_function() {}
// match it's Ps and Qs.
bool templ_inst::check_qualifier(Plist formals)
{
  Pexpr actual = actuals ;
  for (Plist formal = formals ; formal && actual ; formal = formal->l,
       actual = actual->e2)
    
    switch (formal->f->n_template_arg) {
      
    case name::template_type_formal:
      { Pbase b = Pbase(actual->e1->tp) ;
	
	if (! ((b->base == TYPE) &&
	       (b->b_name->base == TNAME) &&
	       (strcmp (Pname(b->b_name)->string, formal->f->string) == 0)))
	  return false ;
	break ;
      }
      
    case name::template_expr_formal:
      if (! ((actual->e1->base == NAME) &&
	     (strcmp(Pname(actual->e1)->string, formal->f->string) == 0)))
	return false ;
      break ;
      
    case name::template_expr_tree_formal:
    case name::template_stmt_tree_formal:
    default:
      error ('i',"bad template formal") ;
    }

  return true ;
}


// make the class template visible when compiling the template class
// defintion, so that it can be refernced while compiling the class body.
void templ_compilation::introduce_class_templ(Pname namep)
{
  owner = is_template(namep) ;
  // create a template definition if one did not already exist, due to a
  // forward declaration
  if (!owner){
    owner = new templ(params, namep) ;
    Pname lookup_name = templp->templates->insert(new name(namep->string), 0);
    lookup_name->tp = Ptype(owner) ;  // lie, to permit use of the table
  }
}
			    
			    
// The body of the template has been parsed. Finish the definition of the
// template class. 
void templ_compilation::end(Pname p) 
{
  bool forward_definition = false ;
  // Restore the name environment to the state before the template parameters
  // were processed.
  modified_tn = param_tn ;
  restore() ;
  modified_tn = 0 ;

  if (curr_tree_template) {
    // create an expression template
    new tree_template(curr_tree_template,
		      p->string, params, p->n_initializer, templ_refs) ;
  }else {
    if (!p->tp) {
      error ("a class, or member function definition was expected") ;
      return ;
    }
    switch(p->tp->base){
      
    case CLASS:
      // Create the template type to represent the parsed template, and enter it
      // into the global table. This is achieved simply by modifying the TNAME
      // that was entered into ktbl to represent the class definition.
      Pname namep = ktbl->look(p->string, 0) ;

      // check for nested definitions, they aren't supported currently. 2.1 is
      // a good time to start supporting them, since they are nested for real.
      for (Pname nn = Pclass(p->tp)->mem_list ; nn ; nn = nn->n_list)
	switch (nn->base) {
	case NAME:
	  switch(nn->tp->base) {
	  case CLASS:
	    error("class declaration %s not permitted within a parametrized\
 class", nn->string) ;
	      break ;
	    case ENUM:
	      error("enum declaration %s not permitted within a parametrized\
  class", nn->string) ;
	      break ;
	  }
	  break ;
	  
	case TNAME:
	  error("typedef %s not permitted within a parametrized class",
		nn->string) ;
	  break ;
	}

      owner = is_template(namep);
      if (owner) {
	Pclass c = Pclass(owner->basetype()->b_name->tp) ;
	// ignore it, if it is a forward declaration following a real
	// definition
	if (owner->defined && (Pclass(p->tp)->mem_list != owner->members))
	  error("template class %s multiply defined", p->string) ;

	forward_definition=bool((c->defined & DEF_SEEN) && (!owner->defined));
	if (forward_definition) owner->resolve_forward_decl(params, c) ;
      }else 
	// a forward declaration
	introduce_class_templ(namep) ;

      if (templ_refs) owner->templ_refs = templ_refs ;
      break ;
    
    case FCT:
      { Pname qual = p->n_qualifier ;
	// the function must belong to a declared template class
	if (! qual) {
	  error('s', "only template member functions may be parametrized currently") ;
	  return ;
	}
	if (qual->tp && (qual->tp->base == COBJ))
	  switch (get_class_base(Pbase(qual->tp))) {
	  case uninstantiated_template_class:
	    owner = Ptclass(Pbase(qual->tp)->b_name->tp)->inst->def ;
	    // verify that the formals specified, match the template formals
	    // in name, note that the length was already matched when the
	    // instantiation was generated.
	    if (!get_template_class
		(Pbase(qual->tp))->inst->check_qualifier(params))
	      error ("qualifier parameters must match the template formals") ;
	    break ;
	  case template_class:
	    // the template reference was without any of the formals
	    owner = is_template(qual) ;
	    if (! owner->has_tree_expr_formals())
	      error('w',"qualifier %n for %n must specify template parameters",
		     qual, p) ;
	    break ;
	  default:
	    error ("qualifier %n for %n wasn't a template class", qual, p) ;
	    return ;
	  }
	Pfunt ft= owner->collect_function_member(p) ;
	if (! Pfct(p->tp)->body)
	  error ("the template function member %n must have a body", p) ;
	ft->templ_refs = templ_refs ;
	ft->formals = params ;
	owner->check_formals(params) ;
	break ;
      }
    default: error ("class, or member function definition expected.") ;
    }
  }
  // Note the template references from this definition

  clear_ref_templ() ;
  param_end = params = 0;  // Indicates the end of template processing.
  curr_tree_template = 0 ;

  if (forward_definition) owner->instantiate_forward_decl() ;
  owner = 0 ;
}


				     

// Clear the list of templates referenced during the syntax analysis of a top
// level definition. Note that since this list is produced during syntax
// analysis, it does not recognize instantiations that may actualy turn out to
// be identical at instantiation after the substitution of actual parameters.
// Thus, the list may be longer than it would be after actual argument
// substitution. 
void templ_compilation::clear_ref_templ() {
  for (Pcons p = templ_refs ; p ; p = p->cdr)
    Ptempl_inst(p->car)->refp = false ;
  templ_refs = 0 ; last_cons = 0 ;
}


// Instantiate templates that were referenced by a non-template definition,
// after syntax analysis has been completed on it.
void templ_compilation::instantiate_ref_templ() {
  for (Pcons p = templ_refs ; p ; p = p->cdr)
    Ptempl_inst(p->car)->instantiate() ;
  clear_ref_templ() ;
}




// Compile all template member body instantiations. Set in motion the
// compilation of the graph of instantiation bodies. Note that compilation of
// a body may in turn initiate the instantiation of templates that had not
// previously been instantiated.
void templ_compilation::end_of_compilation() {
  bool change = false ;
  do {
    change = false ;
    for (Ptempl p = list ; p ; p = p->next)
      change = ( change | p->instantiate_bodies() ? true : false);
  } while (change) ;
}


// A predicate to validate that a tname without template parameters is legit
// in the scope, ie. that it does not need actual template arguments.
// Currently, a tname without parameters is ok within the class definition,
// but parameters are required within the member definition. They should not
// be required within the member function either to be consonance with their
// use in the class definition.
Pname templ_compilation::check_tname(Pname p) {
  Ptempl t = is_template(p) ;

  if (p->n_template_arg) p->n_used++ ;
  if (!t) return p ;
  if (in_progress && ((owner && (owner->namep == p)) ||
		      (!owner && t->basetype()->b_name->tp == ccl)))
     return p ;
  error ("%n needs template instantiation arguments.", p) ;
  return p ;
}


// This function determine when the parameters specified to a template are
// redundant, and really refer to the current template class. Thus,
//
// template c<class p1, class p2> c<p1,p2>::foo { ... } ;
// has the redundant template specification c<p1, p2> and can simply be a
// reference to a "c" instead, 
bool templ_compilation::current_template(Pname p, Pexpr actuals) {
  if (in_progress &&
      ((owner  && (owner->namep == p)) ||
       (!owner && ((p->tp->base == COBJ) &&
		   (Pbase(p->tp)->b_name->tp == ccl)))))
    { // Check whether the formal and actual types are identical
      Pexpr actual = actuals;
      for (Plist formal = params ; formal && actual ; formal = formal->l,
	   actual = actual->e2)
	if ((formal->f->tp == actual->e1->tp) || 
	    ((actual->e1->tp && (actual->e1->tp->base == TYPE)) &&
	     (Pbase(actual->e1->tp)->b_name->tp == formal->f->tp)))
	  continue ;
	else break ;
      if (!formal && !actual) return true ;
    }
  return false ;
}
  
  
	
// Add a new member function to the list of functions for the template class.
Pfunt templ::collect_function_member(Pname fname) {
  PERM(fname) ; PERM(fname->tp) ; PERM(Pfct(fname->tp)->body) ;
  return new function_template (*this, templp->params, fname) ;
}


// Check the formals specified for a member function or a forward definition
// of a class, against the formals for the class.
void basic_template::check_formals(Plist f2) {
  
  for (Plist f1 = formals; f1 && f2 ; f1 = f1->l, f2 = f2->l)
    if (f1->f->base != f2->f->base) 
      switch (f1->f->n_template_arg) {
      case name::template_type_formal:
	error ("template formal parameter mismatch,\
%n must be a type formal parameter.", f2->f) ;
	break ;
      case name::template_expr_formal:
	error ("formal parameter mismatch, %n must be a parameter of type %t",
	       f2->f, f2->f->tp) ;
	break ;
      case name::template_expr_tree_formal:	
      case name::template_stmt_tree_formal:
	error ("formal parameter mismatch, %n must be an expression formal parameter",
	       f2->f) ;
	break ;
      default:
	error ("formal parameter mismatch between class formal: %n,\
 and member formal: %n",
	       f1->f, f2->f) ;
      }else if (f1->f->n_template_arg == name::template_expr_formal) {
	// the types should be identical
	if (f1->f->tp->check(f2->f->tp, 0) == 1)
	  error ("type mismatch between member formal %n, and class formal %n",
		 f2->f, f1->f) ;
      }
  if (f1)
    error ("insufficient formal parameters, \
class parameter %n onwards are missing", f1->f) ;
  if (f2)
    error ("excess formal parameters, \
parameter %n onwards is not defined for the class", f2->f) ;
  
}

// predicate to determine whether the template has expression tree formals
bool templ::has_tree_expr_formals() {
  
  for (Plist formal= formals; formal ; formal = formal->l)
    if (formal->f->n_template_arg == name::template_expr_tree_formal)
      return true ;

  return false ;
}


// Check that those formals that require class actual arguments, due to their
// use in member function bodies get them.
bool basic_template::check_constraints(Pexpr actual)

{
  bool ret = true ;
  
  for (Plist formal = formals ; formal && actual ; formal = formal->l,
       actual = actual->e2)
    if ((formal->f->n_template_arg == name::template_type_formal) &&
	formal->f->n_template_formal_must_be_class) {

	  Pname n = Pname(actual->e1) ;

	  if (n && n->tp && n->tp->is_cl_obj())
	    continue ;
	  error("expected a class type actual, not %t, for the \"type type\" formal %s",
		n->tp, formal->f->string);
	  ret = false ;
	}
  return ret ;
}


// Check actual template arguments, against the formals.
bool templ::check_actual_args(Pexpr actual)
{
  for (Plist formal = formals ; formal && actual ; formal = formal->l,
       actual = actual->e2)
    switch (formal->f->n_template_arg) {
    case name::template_type_formal:
      {
	// A "type type" parameter, any actual type that was accepted by the
	// parse is acceptable here, just make sure that it is indeed a type.
	// If it was parsed as a type, the grammar should have created a name
	// node, and attached the type to it, having marked the name as a
	// template_actual_arg_dummy.

	Pname n = Pname(actual->e1) ;
	
	if (!((n->base == NAME) &&
	    (n->n_template_arg == name::template_actual_arg_dummy))) {
	  error ("template %s argument mismatch, the template formal:%n\
 required a type actual parameter.", namep->string, formal->f) ;
	  // recover from the error with a safe expression
	  n = new name("") ;
	  n->tp = any_type ;
	  actual->e1 = n ;
	}
	if ((formal->f->n_template_formal_must_be_class) &&
	    !(n->tp && n->tp->is_cl_obj()))
	  error("expected a class type actual, not %t, for the \"type type\" formal %s",
		n->tp, formal->f->string) ;
	break ;
      }
    case name::template_expr_formal:
      // checking can only be done at instantiation, so postpone it 
      break ;
    case name::template_expr_tree_formal:
    case name::template_stmt_tree_formal:
      // anything is acceptable, it is a case of "implementor beware". Any
      // illegalities will only be detected when dcl processing takes place.
      break ;
    default:
      error ('i',"bad template formal") ;
    }
  // If we provide for optionals, this is where the processing should get done.
  if (formal)
    error ("too few arguments supplied for template %s", namep->string) ;
  if (actual && actual->e1) {
    error ("too many arguments supplied for template %s", namep->string) ;
  }
  return bool(~(formal || actual)) ;
}




// Append to the list of references.
void templ_compilation::append_ref(Ptempl_inst ref) 
{ cons *p = new cons(ref,0) ;
  if (last_cons)
    last_cons->cdr = p ;
  else templ_refs =  p ;
  last_cons = p ;
} ;


// Note the reference by a definition to the template, so that the template
// can be instantiated before the definition is processed. 
Ptempl_inst templ_inst::note_ref() 
{  if ((! refp)) {
    refp = true ;
    templp->append_ref(this) ;
  }
  return this ;
}
  
	    
	    
// Get an instantiation for the template with the given set of actuals. If one
// exists, return it, otherwise create a new one. 
Ptempl_inst templ::get_inst(Pexpr actuals, Ptempl_inst exclude)
{ Ptempl_inst inst = get_match(actuals, exclude, false) ;
  
  return (inst ? inst : new templ_inst(actuals, this))->note_ref() ;
}


// Find an instantiation that has the same set of actuals, exclude the
// instantaition passed in from the set of candidates
Ptempl_inst templ::get_match(Pexpr actuals,       
			     Ptempl_inst exclude,  // don't match this one
			     // Only instantiated templates match
			     bool match_instantiated_only)    
{  
  for (Ptempl_inst p = insts ; p ; p = p->next)
    if ((p != exclude) &&
	(p->actuals_match(actuals)) &&
	(match_instantiated_only ?
	 (Pclass(Pbase(p->tname->tp)->b_name->tp)->class_base ==
	  instantiated_template_class)
	 : true))
      return (p->forward ? p->forward : p) ;
  return NULL ;
}


// provides the basetype created for a given set of actuals.
Pbase templ::inst_basetype(Pexpr actuals)
{
  return (check_actual_args(actuals) ?
	  Pbase(get_inst(actuals)->tname->tp) : basep) ;
}


// Validate that the name denotes a parametrized type, and prodouce a TNAME
// for the instantiation.
Pname parametrized_typename (Pname p, Pexpr actuals) 
{ 
  Ptempl t = templp->is_template(p) ;
  // A template instantiation with redundant actuals identical to the formals
  // of the current template
  if (templp->current_template(p, actuals)) return p ;
  if (t) {
    Pname tname = t->typename(actuals)  ;
    return (tname ? tname : p) ;
  }
  error ("%n has type arguments but is not a parametrized class", p) ;
  return p ;
}


Pbase parametrized_basetype (Pname p, Pexpr actuals) 
{ Ptempl t = templp->is_template(p) ;
  // A template instantiation with redundant actuals identical to the formals
  // of the current template 
  if (templp->current_template(p, actuals)) return Pbase(p->tp) ;
  if (t) return t->inst_basetype(actuals) ;
  error ("%n is not a parametrized class", p) ;
  return new basetype(TYPE, p);
}


// Similar primitive for obtaining the typename associated with an
// instantiation. 
Pname templ::typename(Pexpr actuals) 
{
  return (check_actual_args(actuals) ? get_inst(actuals)->tname : 0) ;
}

// predicate to get past all the type nodes
static Ptype real_type (Ptype t)
{
  while (t->base == TYPE) t = Pbase(t)->b_name->tp ;
  return t ;
}

static int expr_match(Pexpr a1, Pexpr a2) ;

// Check whether the actuals provided match the actuals for this instantiation.
// The actuals match the formals, iff they are the same type or parametrized
// type. 
bool templ_inst::actuals_match(Pexpr check_actuals)
{ Pexpr ae1, ae2 ;     // the cons cells
  Plist formal = def->formals ;
  
  // The lists should be the same length, since check_actuals will have taken
  // appropriate action.
  for (ae1=actuals, ae2=check_actuals ; formal && ae1 && ae2 ;
       ae1=ae1->e2, ae2=ae2->e2, formal = formal->l)

    switch (formal->f->n_template_arg) {
    
    case name::template_type_formal:
      { Ptype t1 = ae1->e1->tp, t2 = ae2->e1->tp ;
	if (t1->check(t2,255) == 1)
	  return false ;
	break ;
      }
    case name::template_expr_formal:
      if (! expr_match(ae1->e1, ae2->e1)) return false ;
      break ;
      
    case name::template_expr_tree_formal:
    case name::template_stmt_tree_formal:
      // these are internal instantiations and consequently never match
      return false ;
      
    default:
      error ('i', "bad template formal") ;
    }
  return true ;
}


extern char emode ;


// This set of overloaded fuctions are used to accumulate name strings 
void stradd(char *&target, char *source) {
  while (*target++ = *source++) ;
  target-- ;
}
	
void stradd(char *&p, long i) {
  if (!emode) {
    *p++ = 'L' ;
  }
  char s[16] ;
  
  sprintf(s,"%ld", i) ;
  stradd(p,s) ;
}

void stradd(char *&p, Pname n) {
  if (!emode){
    char s[256] ;
    
    sprintf(s,"%d", strlen(n->string)) ;
    stradd(p,s) ;
  }
  stradd(p, n->string) ;
}

    

// produce a unique string suitable for use within a name; if in emode ie.
// printing in the context of an error, print a pretty name instead.  
char * mangled_expr(char *p, Pexpr e, bool mangle_for_address = false)
{
  static int mangle_address = 0 ;
  if (e == 0) return p;

  switch (e->base) {
  case ADDROF:
  case G_ADDROF:
    // unary using e2
    // rely on the
    mangle_address++ ;
    p = mangled_expr(p, e->e2) ;
    mangle_address -- ;
    break ;
    
  case NAME:
    if (mangle_address || mangle_for_address)
      stradd(p, Pname(e)) ;
    else if (Pname(e)->n_evaluated)
      stradd(p, Pname(e)->n_val) ;
    else if (Pname(e)->n_initializer)
      p = mangled_expr(p, Pname(e)->n_initializer, mangle_for_address) ;
    else error ('i', "couldn't mangle %n", e) ;
    break ;
      
  case IVAL:
    stradd(p, ((ival *)e)->i1) ;
    break ;

  case CAST:
    { // an IVAL hiding below the cast ?
      if (e->e1->base == IVAL) 
	stradd(p, ((ival *)e->e1)->i1) ;
      else error ('i', "unexpected cast") ;
      break ;
    }
  case ICON:
  case CCON:
  case FCON:
    *p++ = 'L' ;
    strcpy(p, e->string) ;
    // Sam: there needs to be a better encoding scheme, but it can wait.
    if (!emode)
      while (*p)
      if (! (isalnum(*p)))
	switch(*p) {
	case '+':
	  *p++ = 'p' ;
	  break ;
	  
	case '-':
	  *p++ = 'm' ;
	  break ;

	case '.':
	  *p++ = 'D' ;
	  break ;

	case 'e':
	  *p++ = 'E' ;
	  break ;
	
	default:
	  error ('i', "bad character in constant") ;
	  break ;
	} else p++ ;
    break ;
    
  case ZERO:
    *p++ = '0' ;
    break ;
    
  default:
    error ('i', "can't mangle %k", e->base) ;
  }
  return p ;
}

// this function is invoked once at the top level
char *mangled_expr(char *p, Pname n) {
  if (n->n_evaluated) {
    stradd(p, n->n_val) ;
    return p ;
  }
  return mangled_expr(p, n->n_initializer,
		      (real_type(n->tp)->base == PTR ? true : false)) ;
}

// Generate a template instantiation name suitable for printing when it is
// presented to the user.
void templ_inst::print_pretty_name() 
{
  extern char emode ;
  
  fprintf(out_file, " %s<", (def->namep ? def->namep->string : "?")) ;
  Plist formal = inst_formals ;
  emode ++ ;  // so that dcl_print generates c++ rather than c names
  
  for (Pexpr ae1=actuals; formal && ae1  ; ae1=ae1->e2, formal = formal->l) {
    switch (formal->f->n_template_arg) {
    case name::template_type_formal:
      ae1->e1->tp->dcl_print(0) ;
      break ;
    case name::template_expr_formal:
      { char buff[256] ;
       
       mangled_expr(buff, ae1->e1) ;
       fprintf(out_file, "%s", buff) ;
       break ;
      }
    case name::template_expr_tree_formal:
    case name::template_stmt_tree_formal:
      // doesn't participate in the name generation
      break ;
    default:
      error ('i', "unexpected formal") ;
    }

    // this comma is unfortunately misplaced, since it follows a space printed
    // out by dcl_print
    if (formal->l) fprintf(out_file, ", ") ;
  }
  fprintf(out_file, ">") ;
  emode -- ;
}

	

// there are different representations for ICON based upon whether it has been
// evaluated. 
int check_for_const(Pexpr a1, Pexpr a2) {
  Neval = 0 ;
  if ((a1->base == NAME) &&
      ((a2->base == ICON) || (a2->base == IVAL) || (a2->base == ZERO)))
    { Pname n = Pname(a1) ;
      return (n->n_evaluated && (n->n_val == a2->eval())) ;
    }else if (((a1->base == ICON) || (a1->base == IVAL) || (a1->base == ZERO))
	      && (a2->base == NAME))
      { Pname n = Pname(a2) ;
	return (n->n_evaluated && (n->n_val == a1->eval())) ;
      }
  return false ;
}
  

// get past the template parameter names to get the the real expression
static Pexpr real_expression(Pexpr e) 
{
  while (e && (e->base == NAME) &&
	 (Pname(e)->n_template_arg == name::template_expr_formal) &&
	 (! Pname(e)->n_evaluated) && Pname(e)->n_initializer)
    e = Pname(e)->n_initializer ;
  return e ;
}
	 

// determine whether two expressions supplied as actual arguments to
// a "template_expr_formal"  formal argument match. 
static int expr_match(Pexpr a1, Pexpr a2)
{
  static int addr_of = 0 ;

  a1 = (addr_of ? a1 : real_expression(a1)) ;
  a2 = (addr_of ? a2 : real_expression(a2)) ;
  
  if (a1 == a2) return true;

  // 
  if (a1->base != a2->base) return check_for_const(a1, a2) ;
  
  switch (a1->base) {
  case QUEST:
    // a ternary operator
    return ( expr_match(a1->cond, a2->cond) &&
	     expr_match(a1->e1, a2->e1) &&
	     expr_match(a1->e2, a2->e2)) ;
  
  case PLUS: case MINUS: case MUL: case DIV: case MOD:
  case ER: case OR: case ANDAND: case OROR:
  case LS: case RS:
  case EQ: case NE: case LT: case LE: case GT: case GE:
    // binary operator
    return (expr_match(a1->e1, a2->e1) &&
	     expr_match(a1->e2, a2->e2)) ;
  case UMINUS: case UPLUS:
  case NOT: case COMPL:
    return (expr_match(a1->e2, a2->e2)) ;
  case ADDROF:
    {
      // unary using e2
      addr_of ++ ;
      int result = (expr_match(a1->e2, a2->e2)) ;
      addr_of -- ;
      return result ;
    }
  case CAST:
    return (expr_match(a1->e1, a2->e1)) ;
  case NAME:
    if (! addr_of) {
      Pname n1 = Pname(a1), n2 = Pname(a2) ;

      if ((n1->n_evaluated) && n2->n_evaluated && (n1->n_val == n2->n_val))
	return true ;
      if (n1->n_initializer &&  (! n2->n_initializer))
	return check_for_const(n2, n1->n_initializer) ;
      if (n2->n_initializer && (! n1->n_initializer))
	return check_for_const(n1, n2->n_initializer) ;
      if (n1->n_initializer &&  n2->n_initializer)
	return expr_match(n1->n_initializer,  n2->n_initializer) ;
    }
    return false ;
  case DOT:
    return (expr_match(a1->e1, a2->e1)) ;
  case REF:
    return (expr_match(a1->e1, a2->e1)) ;
  case IVAL:
    return (ival *)a1->i1 == (ival *)a2->i1 ;
  case ICON:
  case CCON:
  case FCON:
  case STRING:
    return (strcmp (a1->string, a2->string) == 0) ;
  case ZERO:
    return true ;
  case SIZEOF:
    { long l1 = a1->eval(), l2 = a2->eval() ;
      return (l1 == l2) ;
    }
  }
  return false ;
}     


static char * non_type_argument_signature(Pexpr e, char *p) {
  p = e->tp->signature(p) ;
  return mangled_expr(p, e) ;
}
  


// generate array suffix information for an array signature. cfront does not
// normally generate this as part of the type signature, since it only deals
// with argument signatures, and arrays can't be arguments.
static void add_array_type_suffix(char *&s, Ptype t) {
  t = real_type(t) ;
  if (t->base == VEC) {
      Pvec v = Pvec(t);
      int dim;
      char a[32] ;
      Neval = 0 ;
      if(v->dim == 0) 
	  dim = v->size / v->typ->tsizeof();
      else dim = (int)v->dim->eval();
    sprintf(a, "A%d", dim) ;
    stradd(s,a) ;
    add_array_type_suffix(s, Pvec(t)->typ);
  }
}
    
			   
			   
// Generate a mangled template instantiation name. The syntax of as template
// mangled class name is of the form:
//   original_name__<argument type signatures>__unique_id
// Each non-type argument is replaced by a unique id.
char *templ_inst::mangled_name(char *ip)
{ static tree_formal_id = 0 ;
	 
  char *start = ip ;
  ip = start ;
  strcpy(ip, (def->namep ? def->namep->string : "?")) ;
  ip= start + strlen(start) ;
  stradd(ip, "__pt__") ;

  {
    char  a [max_string_size], *p = a ;
  
    Plist formal = inst_formals ;
  
    for (Pexpr ae1=actuals ; ae1 ; ae1=ae1->e2, formal = formal->l)
      switch (formal->f->n_template_arg) {
      case name::template_expr_formal:
	*p++ = 'X' ;
	// the formal must have been bound
	p = non_type_argument_signature(formal->f, p) ;
	break ;
      
      case name::template_type_formal:  
	p = ae1->e1->tp->signature(p) ;
	add_array_type_suffix(p,ae1->e1->tp) ;
	break ;
      
      case name::template_expr_tree_formal:
      case name::template_stmt_tree_formal:
	*p++ = 'Y' ;
	stradd(p, tree_formal_id++) ; 
	break ;
      
      default:
	error ("bad template formal:%d", formal->f->base) ;
	break ;
      }
    
    *p = 0 ;
    sprintf(ip, "%d_", strlen(a)+1) ;
    ip = start + strlen(start) ;
    strcpy(ip,a) ;
  }
    
  return start ;
}



// The C compiler barfs when it is passed on through C++ as an automatic
// variable 
const char leader[]= "\t" ;

// Explain the location of an instantiation in greater detail, since it may be
// far removed from it's textual definition.
void templ_inst::print_error_loc() {
  if (! head) return ; // No active instantiations

  extern void print_loc() ;
  state current_state ;
  char buffer[max_string_size] ;

  for (int i = 0 ; i < max_string_size; i++) buffer[i] = 0 ;

  current_state.save() ;
  fprintf (out_file, "%sanomaly detected during the instantiation of",
	   leader) ;
  print_pretty_name() ;
  fprintf(out_file, "\n") ;
  if (!head->next_active) {
    // A more compact message for a single level of instantiation
    context.restore() ;
    fprintf (out_file, leader) ;
    print_loc() ;    
    fprintf (out_file, "was the site of the instantiation\n") ; 
  }else {
    // The instantiation chain is longer than one
    fprintf (out_file, "%sthe instantiation path was:\n", leader) ;
    for (Ptempl_inst p = head ; p ; p = p->next_active) {
      p->context.restore() ;
      print_loc() ;
      fprintf (out_file, " template:") ;
      p->print_pretty_name() ;
      fprintf(out_file, "\n") ;
    }
  }
  current_state.restore() ;
}

  

// Generate a class name for the instantiated class. It is constructed in a
// manner similar to the names used in the construction of overloaded functions
char *templ_inst::instantiation_string() {
  char  inst_name[max_string_size] ;
  for (int i = 0 ; i < max_string_size; i++) inst_name[i] = 0 ;
  mangled_name(inst_name) ;
  return strdup(inst_name) ;
} 
  

    
// Change the names for the class, constructors, and destructors to reflect
// the new class instantiation name.
void classdef::modify_inst_names(char *s)
{
  char *old = string ;
  string = s ;   // Change the class name
  // Change the constructor names
  for (Pname p=mem_list; p; p=p->n_list)
    if (p->tp && (p->tp->base==FCT) && (!strcmp(old, p->string)))
      p->string = s ;
}

// Get past the fake template argument name typename types
Ptype non_template_arg_type(Pbase t) {
  if ((t->base == TYPE) &&
      (t->b_name->n_template_arg == name::template_type_formal))
    return non_template_arg_type(Pbase(t->b_name->tp)) ;
  else return t ;
}

// follow the chain until we hit a non
void non_template_arg_non_type(Pname n) {
  Pexpr i = n->n_initializer; 
  while (i &&
	 (i->base == NAME) &&
	 (Pname(i)->n_template_arg == name::template_expr_formal)) 
    {
      if (Pname(i)->n_initializer) {
	n->n_initializer = Pname(i)->n_initializer ;
	i = n->n_initializer ;
	continue ;
      }
      if (Pname(i)->n_evaluated) {
	n->n_evaluated = 1 ;
	n->n_val = Pname(i)->n_val ;
	return ;
      }
    }
  return ;
}
      


// Now that the actuals are truly resolved, ie. semantics is complete, and the
// template is about to be instantiated.
void forward_template_arg_types(Plist formal, Pexpr actuals)
{   
  for (Pexpr actual = actuals ; formal && actual ;
       formal = formal->l, actual = actual->e2)
    switch(formal->f->n_template_arg){
    case name::template_type_formal:
      actual->e1->tp = non_template_arg_type(Pbase(actual->e1->tp)) ;
      break ;
    case name::template_expr_formal:
      break ;
    case name::template_expr_tree_formal:
    case name::template_stmt_tree_formal:
      break ;
    default:
      error ('i', "bad template formal") ;
    }
}
     
// determine whether the expression supplied as an actual argument to
// atemplate formal of type "template_expr_formal" is suitable. We are nore
// restrictive than we need to be, simply so that the debugger can have an
// easier time. All expressions must be of the form constant integer
// expression, a float or double literal, or the address of a variable, or an
// array, or function
static int  suitable_const_expr(Pname n)
{
  if (n->n_evaluated) return 1 ;

  if (!n->n_initializer) return 0 ;

  switch (n->n_initializer->base) {

  case CAST:
    { // if it is a cast of an integer value, it's fine.
      if (n->n_initializer->e1->base == IVAL)
	return 1 ;
      else return 0 ;
    }
  
  case FCON:
  case ZERO:
    return 1  ;
  
  case ADDROF:
  case G_ADDROF:
    {
      Pname an = Pname(n->n_initializer->e2) ;
      if (an->base != NAME) return 0 ;
      if (an->n_stclass == STATIC) return 1 ;
      return 0 ;
    }
  case NAME:
    {
      Pname an = Pname(n->n_initializer) ;
      if ((an->n_stclass == STATIC) &&
	  (an->tp->base == VEC))
	// assumes that decl processing won, so that the formal could only
	// have been apointer
	return 1 ;
      return 0 ;
    }
  default:
    return 0 ;
  }
}



static bool is_stmt_node(Pnode p) {
  if (!p) return false ;
  switch(p->base) {
  case BREAK: case CONTINUE: case DEFAULT:
  case SM: case WHILE: case DO: case SWITCH: case RETURN: case CASE:
  case FOR:
  case IF:
  case BLOCK:
  case PAIR: // ?
    return true ;
  }
  return false ;
}
	


static void bind_tree_expr_formal(Pname f, Pexpr actual) 
{
  if (actual->e1->base == TNAME)
    error ('i', "a TNAME is not a valid argument for the formal %n",
	    f) ;
  if (is_stmt_node(actual->e1)) {
    error ("an expression node was expected for the expression formal parameter %n", f) ;
    actual->e1 = zero ;
  }
  // bind the formal to the expression, it will be accessed from here by
  // the hook function during the copy
  f->n_initializer = actual->e1 ;
}



// Bind the formals to the types passed in as the actuals, for the
// instantiation, bind the non-type names to their expressions.
void templ_inst::bind_formals()
{ Pexpr actual ;
  Plist formal ;
  
  for (formal = inst_formals, actual = actuals ;
       formal && actual ; formal = formal->l, actual = actual->e2)
    switch (formal->f->n_template_arg) {
      case name::template_type_formal:
	{
	  formal->f->tp = non_template_arg_type(Pbase(actual->e1->tp)) ;
	  PERM(formal->f->tp) ;
	  break ;
	}
	
      case name::template_expr_formal:
	{
	  actual->e1 = actual->e1->typ(gtbl);
	  if (formal->f->tp->check(actual->e1->tp,ASSIGN) == 1)
	    error("template argument mismatch, expected %t for formal %n, not %t",
		  formal->f->tp, formal->f, actual->e1->tp) ;
	  // hide the global name around decl processing of the formal name
	  Pname g = gtbl->look (formal->f->string, 0) ;
	  if (g) g->n_key = HIDDEN ;
	  // bind the non type arguments to their expressions
	  // parameters that are bound at syntax analysis, these parameters are
	  // bound during dcl processing, so ensure that they can be found.
	  formal->f->n_initializer = actual->e1 ;
	  formal->f->simpl() ;
	  formal->f = formal->f->dcl(gtbl, STATIC) ;
	  formal->f->n_key = HIDDEN ;
	  
	  PERM(formal->f) ;
	  non_template_arg_non_type(formal->f) ;
	  if (!suitable_const_expr(formal->f))
	      error("template argument for formal:%s, is not a suitable constant.",
		    formal->f->string) ;
	  if (g) g->n_key = 0 ;
	  break ;
	}
	
      case name::template_expr_tree_formal:
	{
	  bind_tree_expr_formal(formal->f, actual) ;
	  break ;
	}
      case name::template_stmt_tree_formal:
	{ 
	  error("a statement tree may not be specified for a class template") ;
	  break ;
	}
	
      default:
	error ('i', "bad template formal") ;
      }
  
  // now that the formals are bound, compute the instantiation string
  char *inst_name = instantiation_string() ;
  tname->string = instantiation_string() ;

  for (formal = inst_formals; formal ; formal = formal->l)
    if (formal->f->n_template_arg_string)
	error('i', "attempt to bind a template parameter multiple times") ;
    else
      formal->f->n_template_arg_string =
	strcat(strcpy(calloc(strlen(formal->f->string)+strlen(inst_name)+1,1),
		      formal->f->string), inst_name) ;
}


// Expose the non-type parameter names so that they are visible during decl
// processing. Conflicting global names are hidden, so that they are not
// found.
void templ_inst::expose_parameter_names() {

  if (hidden_globals)
    error ('i', "an expose without a hide of global names") ;
  
  for (Plist formal = inst_formals ; formal  ; formal = formal->l)
    if (formal->f->n_template_arg == name::template_expr_formal) {
      // Hide any visible globals
      Pname gname = gtbl->look(formal->f->string, 0) ;
      if (gname) {
	// an existing global name, hide it
	gname->n_key = HIDDEN ;
	// note them for future restoration
	hidden_globals = new name_list(gname,hidden_globals);
      }
      formal->f->n_key = 0 ;	// bring it out of hiding
      if (formal->f != gtbl->look(formal->f->string,0))
	error('i', "parameter could not be located in the global table") ;
    }
}


// Hide the non-type parameter names after an instantiation, and restore any
// globals that may have been hidden during the process.
void templ_inst::hide_parameter_names() {
  
  for (Plist formal = inst_formals ; formal  ; formal = formal->l)
    if (formal->f->n_template_arg == name::template_expr_formal) {
      formal->f->n_key = HIDDEN ;
    }
  for (; hidden_globals; hidden_globals= hidden_globals->l)
    hidden_globals->f->n_key= 0 ;
  hidden_globals = 0 ;
}



// Primitives for saving and restoring the compilation state around a template
// instantiation. It also maintains the stack of template instantiations.
void templ_inst::save_state(Pname p)  {
  if (next_active) error ('i', "circular instantiation of a template") ;
  context.save() ;
  if (templ_inst::head)
    templ_inst::head->hide_parameter_names() ;
  next_active = templ_inst::head ; templ_inst::head = this ;
  context.init() ;
  Cdcl = p ; Cstmt = NULL ;
  curr_file = (Cdcl) ? Cdcl->where.file : 0;
  expose_parameter_names() ;
} 


void templ_inst::restore_state() {
  context.restore() ;
  hide_parameter_names() ;
  templ_inst::head = next_active ; next_active = NULL ;
  if (templ_inst::head)
    templ_inst::head->expose_parameter_names() ;
} 


// Copy over the class definition subtree starting from COBJ down to the
// CLASSDEF node. This minimal subtree has to exist during syntax analysis,
// and already contains pointers into it.
void templ_inst::kludge_copy(Pbase pbc)
{
  // copy just the COBJ ->b_name NAME ->tp CLASS path for now, note that the
  // preceding path of the tree is pre-allocated, since syntax analysis needs
  // to generate pointers to these objects.
  Pbase pb = Pbase(tname->tp) ;
  Pname save_b_name = pb->b_name ;
  Ptype save_tp = pb->b_name->tp ;

  if ((pb->base != COBJ) || (pbc->base != COBJ))
    error ('i', "cobj nodes were expected here") ;
  
  *pb = *pbc ;
  pb->b_name = save_b_name ;
  *pb->b_name = *pbc->b_name ;
  pb->b_name->tp = save_tp ;
  *Pclass(pb->b_name->tp) = *Pclass(pbc->b_name->tp) ;
  Pclass(pb->b_name->tp)->class_base = instantiated_template_class ;
  
}


// these statics probably belong in templ_inst and shouldn't be dangling
// around

static Pbase cobj_node ;
static Pname  cname_node ;
static Pclass class_node ;
	     
static void syntax_tree_copy_hook(void *,
			     Pnode &,
			     node_class,
			     tree_node_action &action,
			     int& never_see_again)
{
  never_see_again = 1;
  action = tna_continue;
  return;
}


// create a copy of the expression tree
static Pnode copy_syntax_tree(Pnode n, int no_types = 0) {
  pointer_hash       cht(default_copy_hash_size) ;
    
  tree_copy_info  info ;

  if(no_types) info.node_hook = syntax_tree_copy_hook;

  copy_tree (n, info, &cht);

  return n ;
}
 


// hook to perform the copying of the pre-allocated class subtree
bool templ_inst::copy_hook(Pnode &node)
{
  switch (node->base) {
  case COBJ:
    if (node == cobj_node) return false ;
    
    if (node == def->namep->tp) {
      *cobj_node =  *Pbase(node) ;
      node = cobj_node ;
    }
    break ;
  case NAME:
    if (node == cname_node) return false ;
    if (node == sta_name) return false ;

    if (node == Pbase(def->namep->tp)->b_name) {
      *cname_node= *Pname(node) ;
      node = cname_node ;		  
    }else { // check for tree expression formals
      char *s = Pname(node)->string ;
      Pname f =  0 ;
      if (s && (*s == '$') && (f = get_parameter(s+1)))
	{
	  if(Pname(node)->n_list) 
	    error ('i', "n_list set in tree template formal.");
	  node = copy_syntax_tree(Pname(f)->n_initializer) ;
	  return false ;
	}
    }
    break ;
  case CLASS:
    if (node == class_node) return false ;
    
    if (node == Pbase(def->namep->tp)->b_name->tp) {
      *class_node = *Pclass(node) ;
      node = class_node ;
    }
    break ;
  }
  return true ;
}


/*

This hook function used during a class copy.
		       
*/
static void copy_hook(void /* Ptempl_inst */ *p, Pnode &node,
		      node_class, tree_node_action &action,
		      int& never_see_again)
{
  action = (Ptempl_inst(p)->copy_hook(node) ? tna_continue : tna_stop ) ;
  never_see_again = (action != tna_stop);
  return ;
}      

    
    
void establish_class_subtree_correspondence(pointer_hash &h, Pname key_tname,
					    Pname value_tname) 
{
  h[int(key_tname)] = int(value_tname) ;
  h[int(key_tname->tp)] = int(value_tname->tp) ;
  h[int(Pbase(key_tname->tp)->b_name)] =
    int(Pbase(value_tname->tp)->b_name) ;
  h[int(Pbase(key_tname->tp)->b_name->tp)] =
    int(Pbase(value_tname->tp)->b_name->tp) ;
}




Pcons make_ref_copy(pointer_hash &h, tree_copy_info &info, Pcons old_templ_refs)
{
  cons dummy(0,0), *last = &dummy ;
  
  for (Pcons pc = old_templ_refs ; pc ; pc = pc->cdr) {
    
    Ptempl_inst t = Ptempl_inst(pc->car) ;
    expr   dummy(ELIST, 0, 0) ;
    elist  list(&dummy) ;

    // copy the trees corresponding to the actuals
    for (Pexpr actual = t->actuals ; actual ; actual = actual->e2) {
      Pnode root = actual->e1 ;
      copy_tree (root, info, &h);
      // make sure that references to enclosing formals are resolved
      root = Pexpr(root)->typ(gtbl);
      list.add(new expr(ELIST, Pexpr(root), 0)) ;
    }
    Pexpr new_actuals = list.head->e2 ;
    
    // get one if it exists, create one otherwise.
    Ptempl_inst  treal = t->def->get_inst(new_actuals, t) ;
    Pname new_tname = treal->tname ;
    
    last = last->cdr = new cons(treal,0);

    establish_class_subtree_correspondence(h, t->tname, new_tname) ;
  }

  return dummy.cdr ;
}




/*
   Remap the template references from within the body of the template. This
action is similar to the normal tree copy operation; it would normally have
been done during the syntax phase, that produced the tree, but since there
isn't one, for the instantiated body, it must be done here.

*/					     
Pcons templ_inst::ref_copy(pointer_hash &h, tree_copy_info &info, Pcons old_templ_refs)
{
  expose_parameter_names() ;
  Pcons new_refs = make_ref_copy(h,info,old_templ_refs) ;
  hide_parameter_names() ;
  return new_refs ;
}

static bool is_forward_instantiation(Pbase b_base, Pbase f_base)
{
  return bool(b_base->b_name->tp->defined && f_base->b_name->tp->defined) ;
}


/*****************************************************************************
* 									     *
* If the template instantiation is found to be unique after the decl	     *
* processing of the actuals, create a copy of the post syntax graph for the  *
* class. The edges of the graph are determined by "type nodes" that have     *
* already been defined, and TNAME nodes that are in the global keyword	     *
* table. Special care is also taken to avoid copying nodes whose identity    *
* must be maintained, since cfront uses them for fast type checks, these     *
* nodes always have the "defined" flag turned on and so are never copied.    *
* 									     *
* Copying of the pre-allocated class sub-tree for the template: COBJ	     *
* ->b_name NAME ->tp CLASS						     *
* 									     *
* is handled by the class_copy hook above, that is invoked during the course *
* of the copy.								     *
* 									     *
* 									     *
* Template references from within the class need special handling, since     *
* each instantiation of the class, results in a potentially new template     *
* instantiation.							     *
* 									     *
*****************************************************************************/
Ptempl_inst  templ_inst::class_copy(Pcons &templ_refs, bool recopy) 
{
  bool forward = false ; // a forward class is instantiated twice
		
  // associate the formals with their types, and their expressions
  if (recopy) {
    // remove the class def node from the table, so that it's attributes are
    // copied. 
    corr->del(int(Pbase(def->namep->tp)->b_name->tp)) ;
    corr->del(int(Pbase(def->namep->tp)->b_name)) ;
    corr->del(int(def->namep->tp)) ;
    
    corr->del(int(tname->tp)) ;
    corr->del(int(Pbase(tname->tp)->b_name)) ;
    corr->del(int(Pbase(tname->tp)->b_name->tp)) ;
  }else corr = new pointer_hash(default_copy_hash_size) ;
  
  { // copy the formals & install them in the correspondence table
    name_list dummy_formal(0,0) ; 
    Plist last = &dummy_formal ;
      
    for (Plist formal = def->formals ; formal ; formal = formal->l) {
      Pname copy_name = new name("") ;
      *copy_name = *formal->f ;
      copy_name->n_tbl_list = 0 ;
      last = last->l = new name_list(copy_name, 0) ;
      (*corr)[int(formal->f)] = (int)copy_name ;
    }
    inst_formals = dummy_formal.l ;
  }
  
  bind_formals() ;
  if ( ! recopy && ktbl->look(tname->string, 0)) {    
    // formal binding may result in detecting identical instantiations
    Ptempl_inst ti = def->get_match(actuals, this, true) ;
    if (ti) return ti ;
    error('i', "Generated template instantiation name %swas not unique",
	  tname->string) ;
  }
  
  { 
    tree_copy_info  info ;
    info.node_hook = ::copy_hook ;
    info.hook_info = this ;
    
    (*corr)[int(def->namep)] = int(tname) ; // make the tnames correspond
    
    templ_refs = ref_copy(*corr, info, templ_refs) ;
    Pnode root = def->basep ;	// start the copy at the cobj node

    // deal with these nodes differently during the copy, ie. the nodes
    // themselves are not copied, but their attributes are.
    cobj_node = (Pbase)tname->tp ;
    cname_node = Pbase( tname->tp)->b_name ;
    class_node = Pclass(Pbase( tname->tp)->b_name->tp) ;
    
    copy_tree (root, info, corr);

    {  // dump the tree if the flag is set
       extern int dump_tree ;
       extern dcn_arg dump_tree_arg;

       if (dump_tree) display_cfront_node (dump_tree_arg, root);
     }
  }
  // Perform name modifications for the class, so that it is an
  // instantiation-specific name.
  cname_node->string = tname->string ;
  if (!recopy)
    namep =  ktbl->insert(tname, 0) ;
  else class_node->defined &= ~(DEFINED|SIMPLIFIED) ;
  namep->tp = cobj_node;
    
  class_node->modify_inst_names(cname_node->string) ;
  return 0 ;
}


/*
This hook function is responsible for the replacement of references to
expression when copying function bodies
*/
static void function_copy_hook(void *current_templ_inst,
			     Pnode &node,
			     node_class,
			     tree_node_action &action,
			       int& never_see_again)
{
  never_see_again = 1;

  switch (node->base) {
  case SM_PARAM:
    error ('i', "statement parameters not permitted for member functions") ;
    break ;
    
  case NAME:
    { 
      if(node == sta_name) {
	  action = tna_stop;
	  return;
      }
      char *s = Pname(node)->string ;
      Pname f =  0 ;
      if (s && (*s == '$') &&
	  (f = Ptempl_inst(current_templ_inst)->get_parameter(s+1))) {
	      if(Pname(node)->n_list) 
		  error ('i', "n_list set in tree template formal.");
	      node = copy_syntax_tree(Pname(f)->n_initializer) ;
	      action = tna_stop ;
	      never_see_again = 0;
	  }else action = tna_continue ;
      return ;
    }
  default:
    action = tna_continue ;
    return ;
  }
}



/* 

Create a copy of a function member, as part of the instantiation of a function
body. The correspondence table is first initialized with the contents of the
correspondence table used to instantiate the class. Copying is initiated
in this context.

*/

Pname templ_inst::function_copy(Pfunt fnt, Pcons &templ_refs)
{
  pointer_hash fcorr(*corr) ; // initialize it with the old hash table
  
  {     
    tree_copy_info  info ;
    Pnode           root = fnt->fn ; 

    // establish a correspondence between the formals used for the class
    // template, andthe formals used for the function template, all references
    // to the function template formals will be replaced by references to the
    // instantiated class template formals after the copy has been completed.
    
    for (Plist fformal = fnt->formals, cformal = inst_formals ;
	 fformal ; fformal = fformal->l, cformal = cformal->l)
      {
	fcorr[int(fformal->f)] = int(cformal->f) ;
	if (fcorr[int(fformal->f)] != int(cformal->f))
	  error ('i', "hash table bug") ;
      }

    info.node_hook = ::function_copy_hook ;
    info.hook_info = this ;

    templ_refs = ref_copy(fcorr, info,  templ_refs) ;
    if (fcorr[int(def->namep)] != int(tname))
      error ('i', "template to instantiation typename correspondence is missing") ;
    
    copy_tree (root, info, &fcorr);

    {  // dump the tree if the flag is set
      extern int dump_tree ;
      extern dcn_arg dump_tree_arg;

      if (dump_tree)
	display_cfront_node (dump_tree_arg, root);
    }
    return Pname(root) ;
  }
}
  
     
/*****************************************************************************
* 		       							     *
* A matching template was found at instantiation time, which was not	     *
* detected at syntax analysis time. This can happen, when an instantiation   *
* has as its arguments not real types but template arguments, so that	     *
* matches cannot be detected until the templates are bound. Note that it is  *
* also possible to match a template that is in the process of being	     *
* instantiated further up the instantiation call chain. In such cases, the   *
* kludge_copy operation will copy over an incomplete class subtree, which    *
* will be recopied with the completed one after the instantiation is	     *
* completed, in templ_inst::instantiate.				     *
* 									     *
*****************************************************************************/
void templ_inst::instantiate_match(Ptempl_inst match)
{ Pbase pb = Pbase(match->tname->tp) ;
  kludge_copy(pb) ;
  forward = match ;		// Note the fact that this template was matched
}


Pclass current_instantiation = 0 ;

// Do the class declaration instantiation.
void templ_inst::instantiate(bool reinstantiate)
{
  Pcons templ_refs = def->templ_refs  ;
  if (! reinstantiate) {
    switch (Ptclass(Pbase(tname->tp)->b_name->tp)->class_base) {
    case instantiated_template_class: 
      return ;
    case uninstantiated_template_class: 
      break ;
    case vanilla_class:
    case template_class:		// the canonical template class
    default: 
      error ('i', "attempt to instantiate a non-template class") ;
    }
    status = class_instantiated ;
    // Check whether, the template has already been instantiated, if so use it.
    forward_template_arg_types(def->formals, actuals) ;
    Ptempl_inst match = def->get_match(actuals, this, true) ;
    
    if (match || (match = class_copy(templ_refs, false))){
      instantiate_match(match) ;
      return ;
    }
  }else class_copy(templ_refs, true) ;

  Pbase pb = Pbase(tname->tp) ;
  
  // Save the state around decl processing
  { save_state(def->namep) ;
    
    if (def->open_instantiations++ > 1) {
      error ("an infinite instantiation sequence was initiated") ;
      def->open_instantiations-- ;
      return ;
    }
    // Mark the class as instantiated so that there are no circular
    // instantiations. 
    Pclass(pb->b_name->tp)->class_base = instantiated_template_class ;

    // if it is a forward reference, rely on the usual compilation to
    // provide an error message, if indeed it is an error, and not a
    // benign forward reference such as: friend class foo<X,Y> 
	  
    if (def->basep->b_name->tp->defined) {

      // Put out the typedefs for the template parameters do this before the
      // call to name::dcl below, since dcl processing will emit c declarations
      // that make use of the type
      for (Plist formal = inst_formals; formal ; formal = formal->l)
	switch(formal->f->n_template_arg) {
	case name::template_expr_tree_formal:
	case name::template_stmt_tree_formal:
	  break ;
	default:
	  formal->f->dcl_print(0) ;
	}
      // Instantiate the parametrized types referenced by this template
      for (Pcons pc = templ_refs ; pc ; pc = pc->cdr)
	Ptempl_inst(pc->car)->instantiate() ;
      
      if (!((pb->b_name->dcl(gtbl, EXTERN) == 0 ) || error_count)) {
	pb->b_name->simpl() ;
	
	if (pb->b_name->tp->base != CLASS)
	  error('i', "a classdef was expected in templ_inst::instantiate") ;
	
	current_instantiation =  Pclass(pb->b_name->tp) ;
	pb->b_name->dcl_print(0) ;
	if (! (pb->b_name->tp->defined & DEFINED))
	  error ('i', "dcl class is not yet defined") ;
	current_instantiation = 0 ;
      }
    }

    // bash every template instantiation class that has been forwarded to
    // it, with the decl processed version.
    for (Ptempl_inst clone = def->insts ; clone ; clone = clone->next)
      if (clone != this) {
	if (clone->forward == this)
	  clone->kludge_copy(Pbase(tname->tp)) ;
	else {
	  // resolve references to forward declarations
	  if (this == def->get_match(clone->actuals, clone, true)) {
	    clone->kludge_copy(Pbase(tname->tp)) ;
	    clone->forward = this ;
	  }
	}
      }
    Pclass result = Pclass(pb->b_name->tp);
      
    // dcl_print the member functions, so that they can be referenced
    int i = 0 ;
    for (Pname fn= Pclass(pb->b_name->tp)->memtbl->get_mem(i=1); fn;
	 fn=Pclass(pb->b_name->tp)->memtbl->get_mem(++i))
      if ((fn->base == NAME) && (fn->tp->base == FCT))
	fn->dcl_print(0) ;
    
    restore_state() ;
    def->open_instantiations-- ;
  }
}

/* Template Constructors  */


templ::templ(Plist parms, Pname p) {
  namep   = p ;
  formals = parms ;

  if (!formals) error ("a parametrized type must have parameters !") ;

  basep   = Pbase(namep->tp) ;

  Pclass(basep->b_name->tp)->class_base = template_class ;
  defined = ((basep->b_name->tp->defined & DEF_SEEN) ? true : false) ;
  if (defined)
    members = Pclass(basep->b_name->tp)->mem_list ;
  
  PERM(namep) ; PERM(namep->tp) ;
  // Chain on to the list of templates for the compilation.
  next = templp->list ;
  templp->list = this ;
}


templ_inst::templ_inst (Pexpr act, Ptempl owner)
{
  // Set up the basetype for the class, so that nodes that need to point to it
  // during syntax processing can do so. Theese objects are merely
  // place-holders during syntax analysis, and are actually filled in during
  // the copy phase of instantiation.
  Pclass c ;
  
  def = owner ;
  tname = new name(def->namep->string) ;
  tname->base = TNAME ;
  tname->tp = new basetype(COBJ, new name(def->namep->string)) ;
  Pbase(tname->tp)->b_name->tp = c = new templ_classdef(this) ;
  PERM(tname) ; PERM(tname->tp) ; PERM(Pbase(tname->tp)->b_name) ;
  PERM(Pbase(tname->tp)->b_name->tp) ;

  // initialize the member list so that set_scope can do the right thing
  c->mem_list = Pclass(def->basetype()->b_name->tp)->mem_list ;
  
  actuals = act ;
  next = owner->insts ;
  owner->insts = this ;
}


templ_classdef::templ_classdef(Ptempl_inst i): (CLASS) {
  inst = i ;
  class_base = uninstantiated_template_class ;
  string = unparametrized_tname()->string ;
}


// Create a new function template.
function_template::function_template (templ &owner, Plist params, Pname n)
{
  definition_number = ++definition_tick ;
		      
  if (owner.fns_end)
    owner.fns_end->next = this ;
  else owner.fns = this ;
  owner.fns_end = this ;
  formals   = params ;
  fn        = n ;
  PERM(n) ; PERM(n->tp) ;
}


// create the tree template
tree_template::tree_template(TOK tree_kind, char *s, Plist params, Pnode tree,
			     Pcons references) 
{
  string = s ;
  formals = params;
  e = tree ;
  if (!e)
    error('w', "the internal template %s does not have an associated body", s);
  templ_refs = references ;
  kind = tree_kind ;

  // add the expression template too the list of known templates
  if (get(s)) {
      error ("duplicate definition of the internal template %s", s) ;
      return ;
    }
  // link it in
  next = templ_compilation::tree_templates ;
  templ_compilation::tree_templates = this ;

  // check that formals weren't misspelt
  for (Plist formal = formals ; formal ; formal = formal->l)
    if (! formal->f->n_used)
      error('w', "the formal parameter %n was not referenced within the template body",formal->f) ;
}


// lookup an internal expression template
Ptreet tree_template::get(char *s) {
  for (Ptreet o = templ_compilation::tree_templates ; o ; o = o->next)
    if (strcmp(o->string, s) == 0) return o ;
  return 0 ;
}
 
 
Pname tree_template::get_parameter(char *s) {
  for (Plist formal = formals ; formal ; formal = formal->l) {
    if (strcmp(formal->f->string, s)== 0)
      return formal->f ;
  }
  return 0 ;
}



// Sam: some remodularization seems to be in order here, so that this method
// can be shared
Pname templ_inst::get_parameter(char *s) {
  for (Plist formal = inst_formals ; formal ; formal = formal->l) {
    if (strcmp(formal->f->string, s)== 0)
      return formal->f ;
  }
  return 0 ;
}

Pexpr tree_template::expand(Pexpr)  
{
  error('i', "no support for tree templates") ;
  return 0 ;
}

  
// Instantiate each function member body. It assumes that the class
// declaration has been instantiated. The return value indicates whether an
// instantiation of bodies actually took place. This function is only invoked
// at the end of a file compilation, after all source text has been processed. 
bool templ::instantiate_bodies(){
  bool change = false ;
  // Perform the instantiation of the member function bodies.
  if (!fns) return change ;
  
  for (Ptempl_inst inst = insts ;  inst ; inst = inst->next)
    if (!inst->forward && (inst->status == class_instantiated)) {
      // Set up the environment for the declaration, and subsequent compilation
      // of the function bodies.
      inst->status = body_instantiated ; change = true ;

      {  Pclass ic = inst->get_class() ; int i ;
	// note all the overriding definitions explicitly provided by the user
	for (Pname fn= ic->memtbl->get_mem(i=1); fn; fn=ic->memtbl->get_mem(++i))
	     if ((fn->base == NAME) && (fn->tp->base == FCT) && Pfct(fn->tp)->body)
	  fn->n_redefined = 1 ;
       }

      for (Pfunt fnt = fns;  fnt ; fnt = fnt->next) {
	Pcons templ_ref_copy = fnt->templ_refs ;
	Pname fn = inst->function_copy(fnt, templ_ref_copy) ;
	
	// Change the qualifier to be the name of the instantiated, rather than
	// the parametrized class name
	fn->n_qualifier = inst->namep ;
	fn->n_table = 0; fn->n_tbl_list = 0 ;
	
	// Note that the formals were bound to the actuals when the class decl
	// was instantiated, so the binding is not redone.
	
	// Modify constructor and destructor names.
	if (!strcmp(fn->string, namep->string))
	  fn->string = inst->namep->string ;	
	{ inst->save_state(fn) ;
	  // Instantiate the parametrized types referenced by this template
	  for (Pcons pc = templ_ref_copy ; pc ; pc = pc->cdr)
	    Ptempl_inst(pc->car)->instantiate() ;

	  // ensure that "type type" formals constrained to be class definitions are
	  if (!fnt->check_constraints(inst->actuals))continue ;

	  if ( ((fn = fn->dcl(gtbl, EXTERN)) == 0) || error_count) {
	    inst->restore_state() ;
	    continue ;
	  }
	  fn->simpl() ;
	  fn->dcl_print(0) ;
	  inst->restore_state() ;
	}
      }
    }
  return change ;
}

			   

static char rcsinfo[] = "$Header: /usr3/lang/benson/work/stripped_cfront/RCS/template.c,v 1.4 90/04/02 11:32:09 sam Exp $";


/*
$Log:	template.c,v $
 * Revision 1.4  90/04/02  11:32:09  sam
 * 1) Made comments current.
 * 2) Removed some references to tree_templates.
 * 
 * Revision 1.3  90/03/30  20:13:40  sam
 * The focal point for all the changes:
 * 1) increased the table size for tables used to maintain templates
 * 2) Declare a class template immediately after the class name has been 
 * encountered, rather than at the end of the class definition.
 * 3) Support overiding member definitions
 * 4) Enforce class constraints on template formals, so that type type arguments
 * can be used to declare variables with constructor arguments.
 * 5) Lay the groundwork for solving the circular instantiation problem.
 * 
 * Revision 1.2  90/03/27  10:17:27  sam
 * merged in revisions: 1.28, 1.30, 1.32, 1.33 from the main devo line
 * 
 * Revision 1.1  89/11/20  08:50:56  benson
 * Initial revision
 * 
 * Revision 1.20  89/11/01  14:16:30  sam
 * 1) set mem_list for an instantiated class, so that set_scope can setup scope
 * correctly.
 * 2) fix the bug where templ_refs were getting smashed by an incomplete
 * template definition that followed a complete definition.
 * 
 * Revision 1.19  89/10/31  14:43:46  benson
 * 1) Fix name mangling for arrays with no dim field.
 * 2) fix the spelling of parameter in an error message.
 * 
 * Revision 1.18  89/10/16  15:20:09  sam
 * 1) make use of the new pointer hash tables.
 * 2) fix a bug where the type definitions of formals were sometimes dcl_printed 
 * after references to them in the generated c code.
 * 
 * Revision 1.17  89/10/10  08:42:39  benson
 * manage emode with ++ and --.`
 * 
 * Revision 1.16  89/09/26  16:45:11  sam
 * fix the forward declaration of template classes
 * 
 * Revision 1.15  89/09/22  14:03:47  benson
 * add a way to ahve an expr formal of an internal template that is
 * not copied.
 * 
 * Revision 1.14  89/09/18  16:38:44  sam
 * provide for error recovery upon argument mismatch in a template instantiation
 * 
 * 
 * Revision 1.13  89/09/15  09:32:09  benson
 * 
 * Support the magic variable __current_template_name.
 * In an internal (expr or stmt) template, this is replaced
 * by a char constant containing the template name. Handy for 
 * tracing.
 * 
 * Revision 1.12  89/09/13  09:20:24  benson
 * rename aggregate to collection.
 * 
 * Revision 1.11  89/09/12  12:57:14  sam
 * 1) suppressed arguments for zero argument internal templates.
 * 2) account for "zero" base nodes when matching actual arguments.
 * 3) fix bad %d format effector in generation of unique type names for internal 
 * internal templates.
 * 
 * Revision 1.10  89/09/06  16:23:26  sam
 * parametrized names are now mangled in a form that can be demangled by the 
 * debugger, demangler etc.
 * 
 * Revision 1.9  89/09/02  22:05:48  benson
 * bug fixes in the area of multiple use of formals in internal
 * templates.
 * 
 * Revision 1.8  89/08/30  13:03:54  sam
 * added support for dealing with __expressions in class formal templates
 * 
 * Revision 1.7  89/08/29  13:40:04  benson
 * disentangle the character string name of a generic function
 * from n_table_list.
 * 
 * Revision 1.6  89/08/28  09:43:47  sam
 * refer to comments in 1.6 of templates.h
 * 
 * Revision 1.5  89/08/23  10:20:04  sam
 * Changes in three major areas:
 * 
 * 1) The syntax for formal parameters has been brought into alignment with the BS syntax.
 * 
 * 2) The name encryption has been modified so that the debugger can unmung template
 * names.
 * 
 * 3) Restrictions have been placed on non-type formals to facilitate their
 * handling. They may be integer expressions, float or double literals, or 
 * pointers to static variables.
 * 
 * 4) Gyrations in the code from the days of single instantiation templates
 * have been removed.
 * 
 * Revision 1.4  89/08/11  14:56:29  sam
 *  implementation of multiple instantiation templates.
 * 
 * Revision 1.3  89/07/27  11:29:17  sam
 * Major changes were made her. They covered the following general area:
 * 1) Support for type expression actuals 
 * 2) Support "normal" formal parameters.
 * 3) Support for tree templates
 * 4) statement and expression tree formals for tree templates. They are
 * incomplete for the regular templates.
 * 
* Revision 1.2  89/07/07  14:31:41  sam
* The check in templ_inst::same did not cover all cases, this fixes it.
* Also added a guard the is_template predicate to protect against user errors.
* 

end_log
*/
