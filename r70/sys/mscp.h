/*
 * DSA Mass Storage Control Protocol packet formats
 */

typedef union {
	long	SL_l;
	short	SL_s[2];
} _SL;

/*
 * arbitrary (port specific) buffer descriptor
 * this is arranged for the convenience of non-buffer-using packets
 * specific port drivers should cast it into whatever
 * their particular addressing structure requires
 */

typedef union {
	long	BD_l[3];
	short	BD_s[6];
	struct {
		long BD_pl;
		long BD_q[2];	/* quadword at the end */
	} BD_x;
} BUFDESC;

/*
 * command packet
 */

struct mscmd {
	long	m_crf;		/* reference number */
	short	m_unit;		/* device unit number */
	short	m__r0;
	char	m_opcd;		/* command opcode */
	char	m__r1;
	short	m_mod;		/* modifiers to opcode */
	_SL	m__u0;
#define	m_bcnt	m__u0.SL_l	/* byte count */
#define	m_otrf	m_bcnt		/* or, outstanding reference number */
#define	m_rbn	m_bcnt		/* or, replacement block number */
#define	m_vrsn	m__u0.SL_s[0]	/* or, mscp version number */
#define	m_cntf	m__u0.SL_s[1]	/* and controller flags */
#define	m_rcnt	m__u0.SL_l	/* or, record count */
#define	m_unfl	m__u0.SL_s[1]	/* unit flags */
	BUFDESC	m_buff;		/* buffer descriptor */
#define	m_htmo	m_buff.BD_s[0]	/* or, host timeout interval */
#define	m_time	m_buff.BD_x.BD_q	/* and two longwords of time */
#define	m_fcnt	m_buff.BD_l[0]	/* or, file count */
	long	m_lbn;		/* block number */
#define	m_dvpm	m_lbn		/* device dependent online parameters */
	short	m_fmt;		/* tmscp device format */
};

/*
 * end/attn message
 */

struct msend {
	long	m_crf;		/* reference number */
	short	m_unit;		/* device unit number */
	short	m__r0;
	char	m_opcd;		/* endcode */
	char	m_flgs;		/* end message flags */
	short	m_sts;		/* status bits */
	_SL	m__u0;
#define	m_bcnt	m__u0.SL_l	/* byte count */
#define	m_otfl	m__u0.SL_l	/* or, outstanding cmd ref number */
#define	m_vrsn	m__u0.SL_s[0]	/* or, mscp version number */
#define	m_cntf	m__u0.SL_s[1]	/* and controller flags */
#define	m_mlun	m__u0.SL_s[0]	/* or, multi-unit code */
#define	m_unfl	m__u0.SL_s[1]	/* and unit flags */
	BUFDESC	m_buff;		/* buffer descriptor */
#define	m_cmst	m_buff.BD_l[0]	/* or, command status */
#define	m_unti	m_buff.BD_x.BD_q	/* and quadword unit id */
#define	m_cnti	m_buff.BD_x.BD_q	/* or quadword controller id */
#define	m_ctmo	m_buff.BD_s[0]	/* or, controller timeout */
	long	m_fbbk;		/* first bad block encountered */
#define	m_medi	m_fbbk		/* or, media type id */
	short	m_shun;		/* shadow unit */
	short	m__r2;
	_SL	m__u1;
#define	m_unsz	m__u1.SL_l	/* unit size */
#define	m_trck	m__u1.SL_s[0]	/* or, track size */
#define	m_grp	m__u1.SL_s[1]	/* and group size */
#define	m_menu	m__u1.SL_s[0]	/* or, format menu (and group size?) */
	_SL	m__u2;
#define	m_vser	m__u2.SL_l	/* volume serial number */
#define	m_cyl	m__u2.SL_s[0]	/* or, cylinder size */
	short	m_rcts;		/* size of RCT */
	char	m_rbns;		/* replacement blocks per track */
	char	m_rctc;		/* number of copies of replacement table */
};

/*
 * opcodes
 */

#define	OPGUS	03	/* get unit status */
#define	OPSCC	04	/* set controller characteristics */
#define	OPSEX	07	/* serious exception */
#define	OPAVL	010	/* available */
#define	OPONL	011	/* online */
#define	OPSUC	012	/* set unit characteristics */
#define	OPFLS	023	/* flush cache ? */
#define	OPRPL	024	/* replace bad block */
#define	OPRD	041	/* read */
#define	OPWR	042	/* write */
#define	OPWRM	044	/* write tape mark */
#define	OPPOS	045	/* reposition tape */
#define	OPAVA	0100	/* available attn */
#define	OPEND	0200	/* end flag */

#define	MDPRI	01	/* OPRPL: primary replacement block */
#define	MDSPD	01	/* OPAVL: spin down */
#define	MDREW	02	/* OPAVL: rewind */
#define	MDOBJ	04	/* OPPOS: object count */
#define	MDREV	010	/* OPPOS: reverse */
#define	MDUNL	020	/* OPPOS: unload */
#define	MDXCL	040	/* OPONL: exclusive access */
#define	MDIMM	0100	/* some TMSCP: do it now */
#define	MDCSX	020000	/* any: clear serious exception */
#define	MDCDL	010000	/* many: clear cache data loss woe */

/*
 * status codes
 */

#define	STMSK	037	/* mask for generic status code */
#define	STSUC	0	/* success */
#define	STCMD	1	/* invalid command */
#define	STABO	2	/* aborted */
#define	STOFL	3	/* unit offline */
#define	STAVL	4	/* unit available */
#define	STMFE	5	/* media format error */
#define	STWPR	6	/* write protected */
#define	STCMP	7	/* compare error */
#define	STDAT	010	/* data error */
#define	STHST	011	/* host data access error */
#define	STCNT	012	/* controller error */
#define	STDRV	013	/* drive error */
#define	STTPM	016	/* tape mark encountered */
#define	STRDT	020	/* tape record truncated */
#define	STPL	021	/* tape position lost */
#define	STSEX	022	/* serious exception */
#define	STDIA	037	/* internal diagnostic message */

#define	STRST	0377	/* our software only -- controller reset */
#define	STBCK	STDAT	/* us design optical disk: blank check */
#define	STBK2	0x48	/* someone else's blank check */

/*
 * end message flags
 */

#define	EFCDL	02	/* TMSCP: cached data lost */
#define	EFEOT	010	/* TMSCP: encountered end-of-tape */

#define	MSCPVER	0	/* MSCP version number */

/*
 * controller flags
 */

#define	CFATN	0200	/* enable attention messages */
#define	CFMSC	0100	/* enable misc error log messages */
#define	CFOTH	040	/* enable other hosts' errors */
#define	CFTHS	020	/* enable this host's errors */

/*
 * unit flags
 */

#define	UFWBK	0100	/* tape: enable write-back */
#define	UFWPH	020000	/* write-protected in hardware */

/*
 * TMSCP densities
 * m_fmt
 */

#define	TFMASK	0377	/* mask for all tape format bits, without device type */

/*
 * error log packet
 */

struct mserl {
	long	l_crf;		/* command reference number */
	short	l_unit;		/* unit number */
	short	l_seq;		/* error sequence number */
	char	l_fmt;		/* format of packet */
	char	l_flgs;
	short	l_evnt;		/* event code */
	long	l_cnti[2];	/* controller id */
	char	l_csvr;		/* ctl software version */
	char	l_chvr;		/* ctl hardware version */
	short	l_mlun;		/* multi-unit code */
	long	l_unti[2];	/* unit id */
#define	l_badr	l_unti[0]	/* or, bad memory address */
	char	l_usvr;		/* unit software version */
	char	l_uhvr;		/* unit hardware version */
	char	l_lvl;		/* transfer error level */
	char	l_rtry;		/* transfer error retry */
	long	l_vser;		/* volume serial number */
	long	l_hdcd;		/* header code */
	long	l_sdi[3];	/* sdi info */
};

/*
 * format codes (l_fmt)
 */

#define	FMCNT	0	/* controller error */
#define	FMBAD	1	/* host buffer address error */
#define	FMDSK	2	/* disk transfer error */
#define	FMSDI	3	/* SDI error */
#define	FMSMD	4	/* small disk error */
#define	FMTAPE	5	/* tape transfer error */
#define	FMSTC	6	/* STI communication error */
#define	FMSTD	7	/* STI drive error */
#define	FMSTF	8	/* STI formatter error */
#define	FMRPL	9	/* bad block replacement by controller */

/*
 * flags
 */

#define	LFSUC	0200	/* operation successful */
#define	LFCON	0100	/* operation continuing */
#define	LFRPR	040	/* bad block replacement required */
#define	LFRPE	020	/* error during bad block replacement */
#define	LFSNR	01	/* sequence number reset */

/*
 * purely software things:
 * table of entry points for a port driver
 *
 * the idea is
 *	call init at open time
 *	to do io, call get to get a packet; fill it in;
 *	call map to set up io mapping if it's a read or write;
 *	call send to send the mscp packet to the controller.
 *	if there's no map associated, that's it;
 *	the packet is freed when the controller takes it.
 *	if there's a map, the packet isn't freed until unmap is called
 *	to flush and free up the map too
 */

struct msportsw {
	int (*mp_init)();		/* init the port, if needed */
	struct mscmd *(*mp_get)();	/* get a packet */
	int (*mp_map)();		/* store mapping into packet */
	int (*mp_send)();		/* send packet */
	int (*mp_unmap)();		/* free mapping resources and packet */
};

#ifdef KERNEL
extern struct msportsw *msportsw[];
extern int nmsport;
#endif

/*
 * address token for class drivers;
 * indexed by (minor device) unit number
 */

struct msaddr {
	char ctl;	/* which controller */
	char ctype;	/* type of controller */
	short unit;	/* MSCP unit number thereon */
};

#define	MSMAXID	2	/* number of connection IDs */
