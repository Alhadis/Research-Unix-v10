/*
 * adb - common definitions
 * something of a grab-bag
 */

#include <ctype.h>

#include "mtype.h"

#define	DBNAME	"adb\n"

typedef	int	BOOL;

#define	NVARS	36
#define MAXPOS	80
#define MAXLIN	128
#define	ARB	512
#define MAXCOM	64
#define MAXARG	32
#define LINSIZ	512
#define	MAXSYM	255

#define EOR	'\n'
#define SPC	' '
#define TB	'\t'

#define STRIP	0177

#define	STDIN	0
#define	STDOUT	1

#define	NULL	0

#define	TRUE	(-1)
#define	FALSE	0


/*
 * run modes
 */

#define	SINGLE	1
#define	CONTIN	2


/*
 * common globals
 */

extern	WORD	expv, adrval;
extern	int	expsp;
extern	int	adrflg;
extern	WORD	cntval;
extern	int	cntflg;
extern	WORD	loopcnt;
extern	ADDR	maxoff;
extern	ADDR	localval;
extern	ADDR	maxfile;
extern	ADDR	maxstor;

extern	ADDR	dot;
extern	int	dotsp;
extern	WORD	dotinc;

extern	WORD	var[];

extern	int	xargc;

extern	BOOL	wtflag;
extern	char	*corfil, *symfil;
extern	int	fcor, fsym;
extern	BOOL	mkfault;

extern	int	pid;
extern	int	signo;
extern	int	sigcode;

extern	char	*errflg;

/* result type declarations */

ADDR	inkdot();
char	*exform();
WORD	round();
