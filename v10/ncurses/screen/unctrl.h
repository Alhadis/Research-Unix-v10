/*
 * unctrl.h
 *
 * 1/26/81 (Berkeley) @(#)unctrl.h	1.1
 * @(#)unctrl.h	1.3	(MRH)	8/23/82
 */

#ifndef unctrl
extern char	*_unctrl[];

# define	unctrl(ch)	(_unctrl[(unsigned) ch])
#endif
