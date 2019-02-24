/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * cmd library definitions
 */

#include <limits.h>
#include <stdio.h>
#include <errno.h>

#define EXITBITS	8			/* # exit status bits	*/
#define EXITMAX		((1<<EXITBITS)-1)	/* max exit status	*/

#define EXITED(x)	(!((x)&EXITMAX))
#define EXITSTATUS(x)	(((x)>>EXITBITS)&EXITMAX)

#define TERMINATED(x)	(((x)&EXITMAX)!=0)
#define TERMSTATUS(x)	(((x)&EXITMAX)|(1<<EXITBITS))
#define TERMSIGNAL(x)	((x)&(EXITMAX>>1))
#define TERMCORE(x)	((x)&(1<<(EXITBITS-1)))

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

struct cmdinfo			/* info for each cmd			*/
{
	struct cmdinfo*	next;	/* next in list				*/
	int		pid;	/* cmd pid, 0 if free			*/
	short		fd;	/* cmdopen() connection if >= 0		*/
	short		status;	/* wait() exit status if pid < 0	*/
};

#define cmds	_cmd_info_	/* hide external symbol			*/

extern struct cmdinfo*	cmds;	/* cmd list				*/

extern int		errno;

extern int		cmdwait();
extern long		lseek();
extern char*		malloc();
extern void		sigcritical();
