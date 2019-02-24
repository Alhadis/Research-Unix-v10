#define	VGETSTATE	(('v'<<8)|0)
#define	VSETSTATE	(('v'<<8)|1)

#define	VC_ERROR	0100000
#define	VC_UNUSED1	0040000
#define	VC_SPP		0020000
#define	VC_BTCNT	0010000
#define	VC_ADDR		0004000
#define	VC_PP		0002000
#define	VC_SWPBT	0001000
#define	VC_UNUSED2	0000400	
#define	VC_READY	0000200
#define	VC_IENABLE	0000100
#define	VC_AD17		0000040
#define	VC_AD16		0000020
#define	VC_REMOTE	0000016
#define	VC_REMOTE2	0000010
#define	VC_REMOTE1	0000004
#define	VC_REMOTE0	0000002
#define	VC_DMAGO	0000001
#define VC_MASK		(VC_SPP | VC_PP | VC_SWPBT | VC_REMOTE)

/* sc_state bits (other than bits defined above) */
#define	VCSC_CMNDS	VC_REMOTE
#define	VCSC_OPEN	VC_UNUSED1

/* remote functions */
#define	VC_RLTER	(01 << 1)
#define	VC_CLEAR	(02 << 1)
#define	VC_RESET	(03 << 1)
#define	VC_RFFED	(04 << 1)
#define	VC_REOTR	(05 << 1)
#define	VC_RESET_ALL	(06 << 1)
