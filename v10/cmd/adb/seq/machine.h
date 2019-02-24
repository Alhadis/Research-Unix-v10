#include <sys/vm.h>

#define	PAGSIZ	(NBPG*CLSIZE)

#define	MAXSTOR 0x1000000	/* top of the stack segment */
#define	USRBLK	0xff0200	/* address of user block in core image */
#define	MAXFILE 0xffffffff
