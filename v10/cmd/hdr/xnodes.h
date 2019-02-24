/*
 *	Sydney C Compiler.
 *
 *	Copyright 1984, Bruce Ellis.
 *
 *	Unauthorised possesion, sale or use prohibited.
 */

/*
 *	Note. Any modification to this enum should be matched by an equivalent
 *	entry in "sutab.c" and a string in "debug.c"
 */
typedef enum
{
	xt_and,
	xt_andand,
	xt_andeq,
	xt_argident,
	xt_argname,
	xt_arrayof,
	xt_bar,
	xt_barbar,
	xt_bareq,
	xt_basetype,
	xt_bitfield,
	xt_block,
	xt_caddr,
	xt_call,
	xt_cast,
	xt_cchar,
	xt_cdouble,
	xt_cfloat,
	xt_cint,
	xt_class,
	xt_clong,
	xt_cnull,
	xt_colon,
	xt_comma,
	xt_cuint,
	xt_culong,
	xt_do,
	xt_dot,
	xt_enum,
	xt_eq,
	xt_eqeq,
	xt_expr,
	xt_extf,
	xt_ftnreturning,
	xt_grgreq,
	xt_grt,
	xt_grteq,
	xt_grtgrt,
	xt_if,
	xt_insf,
	xt_less,
	xt_lesseq,
	xt_lessless,
	xt_literal,
	xt_lslseq,
	xt_maxof,
	xt_memblist,
	xt_minof,
	xt_minus,
	xt_minuseq,
	xt_name,
	xt_not,
	xt_noteq,
	xt_null,
	xt_obj,
	xt_percent,
	xt_pereq,
	xt_plus,
	xt_pluseq,
	xt_postdec,
	xt_postinc,
	xt_predec,
	xt_preinc,
	xt_ptrdiff,
	xt_ptrminus,
	xt_ptrminuseq,
	xt_ptrplus,
	xt_ptrpluseq,
	xt_ptrpostdec,
	xt_ptrpostinc,
	xt_ptrpredec,
	xt_ptrpreinc,
	xt_ptrto,
	xt_question,
	xt_sizeof,
	xt_slash,
	xt_slasheq,
	xt_sref,
	xt_string,
	xt_structof,
	xt_svalue,
	xt_tilda,
	xt_times,
	xt_timeseq,
	xt_uand,
	xt_uminus,
	xt_unionof,
	xt_uparrow,
	xt_uparroweq,
	xt_utimes,
	xt_while,
}
	xnodes;

#define	SAMESU	1
#define	MAXSU	255

typedef enum
{
	su_arrayof,
	su_basetype,
	su_bitfield,
	su_enum,
	su_ftnreturning,
	su_ptrto,
	su_structof,
	su_unionof,
	su_types,

	su_null,

	su_and,
	su_bar,
	su_plus,
	su_times,
	su_uparrow,
}
	suequivs;

typedef struct
{
	char	*xd_name;
	pack_t	xd_suequiv;
}
	xdopes;

#define	xunpack(t)	((xnodes)(t))
#define	sunpack(t)	((suequivs)(t))

extern xdopes	xdope[];
extern uchar	sutab[];
