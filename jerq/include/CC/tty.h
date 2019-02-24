/*
 * List of special characters, used by tty emulation
 */
#ifndef	TTY_H
#define	TTY_H
struct Ttychars {
	char	flags0;		/* mode flags (low bits) */
	char	flags1;		/* mode flags (high bits) */
	char	erase;		/* erase character */
	char	kill;		/* kill character */
	char	intrc;		/* interrupt */
	char	quitc;		/* quit */
	char	startc;		/* start output */
	char	stopc;		/* stop output */
	char	eofc;		/* end-of-file */
	char	brkc;		/* input delimiter (like nl) */
};

#define	JTTYC	128		/* higher than all jioctl's */

struct ttycmesg {
	char		cmd;		/* always JTTYC */
	char		chan;		/* channel to be set; 0 ==> global */
	struct Ttychars	ttychars;
};

#define	TRAW	0040
#define	TCRMOD	0020	
#define	TECHO	0010
#define	TCBREAK	0002

#endif	TTY_H
