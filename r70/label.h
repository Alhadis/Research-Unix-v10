/*
 * The size and shape of a security label.
 * Logically this stuff belongs in param.h and types.h,
 * but it seems wise to keep it in one easily-checked
 * place, rather than diffused.
 */

#ifndef L_BITS

#define LABSIZ 60	/* just fits with di_mode in a disk inode */

#define L_UNDEF	0	/* no valid value yet */
#define L_YES	1	/* the universally permissive label */
#define L_NO	2	/* the universally restrictive label */
#define L_BITS	3	/* this one is in the lattice */

#define F_LOOSE	 0	/* label may rise automatically */
#define F_FROZEN 1	/* label may not rise automatically */
#define F_RIGID	 2	/* permafrost; changeable with T_EXTERN */
#define F_CONST  3	/* changeable by nobody */

#define T_SETPRIV	1	/* may set file privs */
#define	T_SETLIC	2	/* may change licences */
#define	T_NOCHK	4	/* exempt from label checking */
#define	T_EXTERN 8	/* may change shape of label arena */
#define	T_UAREA	16	/* may write in u area */
#define	T_LOG	32	/* may execute syslog() call */

struct labpriv {
	unsigned int	lp_junk : 16,
			lp_flag : 2,
			lp_fix : 2,	/* frozen */
			lp_t : 6,	/* capabilities */
			lp_u : 6;	/* licences */
};

struct label {
	struct labpriv	lb_priv;
	unsigned char	lb_bits[LABSIZ];
#	define		lb_flag   lb_priv.lp_flag
#	define		lb_fix   lb_priv.lp_fix
#	define		lb_t      lb_priv.lp_t
#	define		lb_u      lb_priv.lp_u
};

#ifdef KERNEL
#include "sys/jlabel.h"
#endif

#endif L_BITS
