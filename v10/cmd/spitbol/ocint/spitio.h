/*
	spitbol i/o control blocks
*/

struct	bfblk {
		int	typ;
		int	len;
		int	siz;
		int	rem;
		int	off;
		char	buf[1];
};

struct	ioblk {
		int	typ;
		int	len;
		struct	scblk	*fnm;
		int	pid;
		struct	bfblk	*buf;
		int	fdn;
		int	flg;
};


#define	IO_INP	1
#define	IO_OUP	2
#define IO_APP	4
#define	IO_OPN	8
#define IO_EOF	16
#define	IO_ERR	32
#define	IO_SYS	64
#define	IO_WRC	128
#define	IO_PIP	256
#define	IO_DED	512

#define	RECSIZ	1024
#define	BUFSIZ	1024

struct	fcblk {
		int	typ;
		int	len;
		int	rsz;
		struct	ioblk	*iob;
};
