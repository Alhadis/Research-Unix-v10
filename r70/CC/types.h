/*
 * Basic system types and major/minor device constructing/busting macros.
 */

#ifndef _TYPES_
#define	_TYPES_

/* major part of a device */
#define	major(x)	((int)(((unsigned)(x)>>8)&0377))

/* minor part of a device */
#define	minor(x)	((int)((x)&0377))

/* make a device number */
#define	makedev(x,y)	((dev_t)(((x)<<8) | (y)))

typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef	struct	_physadr { int r[1]; } *physadr;
typedef	long	daddr_t;
typedef	char *	caddr_t;
typedef	u_short	ino_t;
typedef	long	swblk_t;
typedef	long	size_t;
typedef	long	time_t;
typedef	long	label_t[14];
typedef	u_short	dev_t;
typedef	long	off_t;
typedef long	portid_t;

#ifndef NBBY
#include <sys/param.h>
#ifndef NBBY
#define	NBBY		8		/* number of bits in a byte */
#define	NBPW		sizeof(int)	/* number of bytes in an integer */
#endif
#endif NBBY
/*
 *	Set of fds used with the select system call.
 *	The macros depend on NBPW and NBBY; assume NOFILE <= 128.
 */
#define FDWORDS		(128+NBPW*NBBY-1)/(NBPW*NBBY)
typedef struct { unsigned long fds_bits[FDWORDS]; } fd_set;
#define FD_SET(n,s)	(s).fds_bits[(n)/(NBPW*NBBY)] |= 1<<((n)%(NBPW*NBBY))
#define FD_CLR(n,s)	(s).fds_bits[(n)/(NBPW*NBBY)] &= ~(1<<((n)%(NBPW*NBBY)))
#define FD_ISSET(n,s)	((s).fds_bits[(n)/(NBPW*NBBY)] & (1<<((n)%(NBPW*NBBY))))
#define FD_ZERO(s)	{int i; for(i=0;i<FDWORDS;i++)s.fds_bits[i]=0; }

#endif _TYPES_
