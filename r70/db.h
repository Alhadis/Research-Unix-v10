/*
 *		      =========
 *		     //
 *		    //			"db" database management system
 *	      ==========
 *	     //	  //			    Copyright (C) 1983
 *	    //	 //				Electroacoustic Creations
 *	   //	=========			    Forestville NSW
 *	  //
 *	 //
 *	=========
 */


/*
 *	Database types.
 */
typedef struct Db_t	db_t;
typedef struct Rec_t	rec_t;
typedef long		iaddr_t;

/*
 *	Field:	fundamental data type.
 *
 *	f_data	- points to the data (plus a NUL)
 *	f_size	- is the size in bytes
 */
typedef struct Field_t
{
	char	*f_data;
	int	f_size;
}
	field_t;

/*
 *	Multiple.
 *
 *	Count plus array of fields.
 */
typedef struct Mult_t
{
	field_t	*m_vec;
	int	m_count;
}
	mult_t;

/*
 *	A component, single or multiple.
 */
typedef union comp_t
{
	field_t	c_f;
	mult_t	c_m;
}
	comp_t;

/*
 *	A record (instance).  Member of a linked list.
 */
struct Rec_t
{
	comp_t	*r_vec;
	rec_t	*r_next;
};

/*
 *	A vector of addresses plus a count.
 */
typedef struct Addr_t
{
	iaddr_t	*a_vec;
	int	a_count;
}
	addr_t;

/*
 *	An entry descriptor.
 */
typedef struct Ent_t
{
	int	e_count;
	char	**e_nvec;
	int	*e_fvec;
}
	ent_t;

#if	vax
#define SAVE_LEN	10
#endif
#if	pdp11
#define SAVE_LEN	3
#endif

#ifndef	SAVE_LEN
@ "SAVE_LEN not defined"
#endif

typedef struct
{
	int	save_vec[SAVE_LEN];
}
	save_t;

/*
 *	Database error types.
 */
typedef enum Errtype
{
	et_bad_address,
	et_change_unique,
	et_communicate,
	et_delete_nonempty,
	et_incomplete,
	et_internal,
	et_length,
	et_locked,
	et_memory,
	et_mismatch,
	et_no_key,
	et_not_found,
	et_not_locked,
	et_not_unique,
	et_not_your_lock,
	et_readonly,
	et_redefinition,
	et_syntax,
	et_system,
	et_timeout,
	et_too_many_locked,
	et_unknown,
}
	errtype;

extern void	d_a_data();
extern iaddr_t	d_a_end();
extern void	d_a_mend();
extern void	d_a_mstart();
extern void	d_a_pstart();
extern void	d_a_pdata();
extern void	d_a_pend();
extern void	d_a_start();
extern void	d_all_unlock();
extern void	d_c_data();
extern void	d_c_end();
extern void	d_c_mend();
extern void	d_c_mstart();
extern void	d_c_start();
extern void	d_close();
extern addr_t	*d_cp_addr();
extern ent_t	*d_cp_ent();
extern void	d_d_all();
extern void	d_d_at();
extern void	d_d_key();
extern void	d_d_re();
extern void	d_dele();
extern addr_t	*d_diff_addr();
extern rec_t	*d_f_all();
extern rec_t	*d_f_at();
extern rec_t	*d_f_ents();
extern rec_t	*d_f_key();
extern rec_t	*d_f_re();
extern addr_t	*d_fa_all();
extern addr_t	*d_fa_key();
extern addr_t	*d_fa_re();
extern void	d_fr_addr();
extern void	d_fr_ent();
extern field_t	*d_getd();
extern ent_t	*d_gete();
extern field_t	*d_geti();
extern field_t	*d_getn();
extern addr_t	*d_inter_addr();
extern void	d_lock();
extern db_t	*d_open();
extern void	d_putd();
extern void	d_pute();
extern void	d_puti();
extern void	d_putn();
extern void	d_save();
extern void	d_stats();
extern void	d_sync();
extern void	d_restore();
extern addr_t	*d_uniq_addr();
extern addr_t	*d_union_addr();
extern void	d_unlock();
extern void	d_wait_lock();

extern char	*d_error;
extern db_t	*d_errdb;
extern errtype	d_errtype;
extern int	d_buffs;

#define	d_catch(d)	setjmp((int *)d)

#define	D_RDONLY	17
#define	D_RDWR		18

#define	NC_ADDR	8

/*
 *	Name flags.
 */
#define	N_MULT		0x01
#define	N_UNIQ		0x02
#define	N_KEY		0xF0

#define	K_WORD		0x20
#define	K_ALL		0x40
