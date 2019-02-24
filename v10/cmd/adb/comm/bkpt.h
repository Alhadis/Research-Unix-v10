/*
 * breakpoints
 */

#define	BKPTCLR	0	/* not a real breakpoint */
#define BKPTSET	1	/* real, ready to trap */
#define BKPTSKIP 2	/* real, skip over it next time */
#define	BKPTTMP	3	/* temporary; clear when it happens */

struct bkpt {
	ADDR	loc;
	WORD	ins;
	int	count;
	int	initcnt;
	int	flag;
	char	comm[MAXCOM];
	struct bkpt *nxtbkpt;
};
typedef struct bkpt	BKPT;

BKPT	*scanbkpt();
extern	BKPT *bkpthead;
