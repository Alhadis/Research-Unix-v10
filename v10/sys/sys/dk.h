/*
 * for datakit dr11 driver
 */

struct	dk {
	struct	queue *dkrq;
	char	flag;
	u_char	chan;
	short	isize;
	short	ialloc;
	char	icnt;
	struct	block *ibuf;		/* current input buffer */
	struct	block *ofirst, *olast;	/* list of blocks to output */
};
