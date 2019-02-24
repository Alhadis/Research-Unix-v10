/*
 *	Code flow analysis.
 */

/*
 *	A flow set for an expression defines three sets of pure variables.
 *
 *	cut	- those whose lifetime must be cut within the expression
 *		  by a pure assignment
 *	mod	- those which must be modified by the expression
 *	ref	- those which may be referenced by the expression
 *		  the expression must be able to use the value of
 *		  the variable as it is at the beginning of the expression
 *	hide	- possible read after last write
 *	cmod	- might be modified
 */
typedef struct
{
    xnode	*f_cut;
    xnode	*f_mod;
    xnode	*f_ref;
    xnode	*f_hide;
    xnode	*f_cmod;
}
	flow_set;

#define	is_pure(d)	(((d)->id_value.i & PURE) != 0)

#define	make_set(p, id)	(\
				(*(p)) = new_xnode(),\
				(*(p))->x_left = NULL,\
				(*(p))->x_value.d = (id)\
			)

#define	pure_var(p)	(\
				(p)->x_what == xt_dot\
				&&\
				(p)->x_left != NULL\
				&&\
				(p)->x_left->x_what == xt_name\
				&&\
				is_pure((p)->x_left->x_value.d)\
			)

/*
 *	Life time defines.
 */
#define	L_NONE		0
#define	L_ACTIVE	1
#define	L_EQUIV		2
#define	L_DEAD		-1
#define	L_HEAD		-2

#define	L_ORD(i)	(-1 - (i))

/*
 *	Equivalence path definition values.
 */
#define	E_NONE		0
#define	E_SOME		1
#define	E_ALL		2
#define	E_SUSP		3

extern ident	*alloced_ids;
extern int	eord;
extern char	*equiv_value;
extern int	equiv_max;
extern int	trace_argument;
extern cnset	expressions;
extern cnset	mip_dead_ends;
extern xnode	*err_set;
extern xnode	*eq_set;
extern xnode	*merr_set;

extern int	flow_flags();
extern int	is_member();
extern xnode	*copy_xlist();
extern xnode	*flow_diff();
extern xnode	*flow_inter();
extern xnode	*flow_pack();
extern xnode	*flow_union();
extern void	add_element();
extern void	alloc_ids();
extern void	equiv_init();
extern void	eval_check();
extern void	expr_flow();
extern void	free_xlist();
extern void	life_list();
extern void	make_pure();
extern void	mod_check();
extern void	print_errs();
extern void	print_list();
extern void	print_set();
extern void	sync_back();
extern void	trace_back();
extern void	trace_life();

#define	E_ROUND(i)	(((i) & ~0xF) + 0x10)
