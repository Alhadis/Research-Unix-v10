
/*
	structure definitions for blocks within spitbol compiler.
*/

struct	chfcb {
		int	typ;
		int	len;
		struct	chfcb	*nxt;
		struct	fcblk	*fcb;
};

struct	icblk {
		int	typ;
		int	val;
};

struct	scblk {
		int	typ;
		int	len;
		char	str[1];
};

