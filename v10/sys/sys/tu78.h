/*
 * TM78/TU78 driver
 */

#define	TM78DRIVES	4	/* max drives per formatter */

struct	tm78 {
	struct device *cc_addr;
	struct tu78 *cc_actf, *cc_actl;
	struct mbaddr *cc_mbaddr;
	char	cc_flags;
	char	cc_errcnt;
	char	cc_drives[TM78DRIVES];
};

struct	tu78 {
	struct tm78 *sc_ctl;
	struct tu78 *sc_next;
	struct buf *sc_actf, *sc_actl;
	daddr_t	sc_blkno;
	daddr_t	sc_nxrec;
	char	sc_dens;
	char	sc_slave;
	short	sc_flags;
	unsigned short	sc_dsreg;
	unsigned short	sc_erreg;	/* just for debugging */
	unsigned short	sc_resid;	/* just for debugging */
};
