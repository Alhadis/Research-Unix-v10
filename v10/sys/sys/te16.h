/*
 * TM03/TE16 et al driver
 */

#define	TM03DRIVES	8	/* max drives per formatter */

struct	tm03 {
	struct device *cc_addr;
	struct te16 *cc_actf, *cc_actl;
	struct mbaddr *cc_mbaddr;
	char	cc_flags;
	char	cc_errcnt;
	char	cc_drives[TM03DRIVES];
};

struct	te16 {
	struct tm03 *sc_ctl;
	struct te16 *sc_next;
	struct buf *sc_actf, *sc_actl;
	short	sc_flags;
	daddr_t	sc_blkno;		/* current tape block */
	daddr_t	sc_nxrec;		/* first nonexistent tape block */
	short	sc_dens;		/* density + slave unit -- httc */
	unsigned short	sc_erreg;	/* copies of recent register values */
	unsigned short	sc_dsreg;
	unsigned short	sc_resid;
};
