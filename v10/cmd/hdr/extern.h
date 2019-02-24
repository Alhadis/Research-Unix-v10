/*
 *	Sydney C Compiler.
 *
 *	Copyright 1984, Bruce Ellis.
 *
 *	Unauthorised possesion, sale or use prohibited.
 */

extern formarg	Ea[];
extern formarg	Pa[];
extern int	errored;
extern int	global_lineno;
extern st_node	*global_filename;

extern char	*str_alloc();
extern char	*strcpy();
extern cfrag	*statement();
extern cnode	*translit();
extern cnode	**flatten();
extern cnode	**code_gen();
extern double	atof();
extern ident	*declaration();
extern ident	*find_ident();
extern ident	*find_label();
extern ident	*is_typedefed();
extern ident	*refind_ident();
extern int	get_fill();
extern long	comp_constant();
extern long	size_of_in_bits();
extern st_node	*find_str();
extern token	*tok_fill();
extern xnode	*bexpr();
extern xnode	*block();
extern xnode	*cast();
extern xnode	*decl_cast();
extern xnode	*decl_item();
extern xnode	*expr();
extern xnode	*extract_field();
extern xnode	*factor();
extern xnode	*find_type();
extern xnode	*find_type();
extern xnode	*get_shared_type();
extern xnode	*rexpr();
extern xnode	*su_optim();
extern xnode	*traverse();
