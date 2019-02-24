#ifdef	KERNEL
#include "../h/param.h"
#include "../h/dir.h"
#include "../h/user.h"
#include "../h/tty.h"
#include "../h/buf.h"
#include "../h/proc.h"
#include "../h/systm.h"
#ifdef VMUNIX
#include "../h/clist.h"
#include "../h/file.h"
#define	ttiocb	sgttyb
#endif
#ifdef VMUNIX
#define	ttioccomm	ttioctl
#define	ioc_ispeed	sg_ispeed
#define	ioc_ospeed	sg_ospeed
#define	ioc_erase	sg_erase
#define	ioc_kill	sg_kill
#define	ioc_flags	sg_flags
#endif

#define	NTR	20
#define	xseq	tr->tr_xseq
#define	lseq	tr->tr_lseq
#define	rseq	tr->tr_rseq
#define	avbytes	tr->tr_avbytes
#define	trschars tr->tr_schars
#define	rbytes	tr->tr_rbytes
#define	cbytes	tr->tr_cbytes
#define	trstate	tr->tr_state
#define	trcntl	tr->tr_cntl
#define	tramsg tr->tr_amsg
#define	trbsize tr->tr_bsize
#define	trsent	tr->tr_sent
#define	inpq	tr->tr_inpq
#define	troutq	tr->tr_outq
#define	trctlq	tr->tr_ctlq
#define	trecho	tr->tr_echo
#define	trenq	tr->tr_enq
#define	trepoch	tr->tr_epoch
#define	trtime	tr->tr_time
#define	q4	tr->tr_q4

#define	DCLS	register struct tr *tr; register s;
#define	DHIWAT	150
#define	DLOWAT	50
#define	q1	tp->t_rawq
#define	q2	tp->t_canq
#define	q3	tp->t_outq
#define	WAKEUP(a)	wakeup(a)
#define	LOCK		s = spl6()
#define	UNLOCK		splx(s)
#endif

struct triocmb {
	char type;
	char seq;
	short cmd;	/* param0 */
	struct ttiocb triocb;
};
#define	trioc	(tr->tr_iocb)


struct tr {
	struct tty *tr_tty;	/* device tty struct */
	struct tty *tr_utty;	/* user's tty struct */
	struct clist *tr_q4;
	short tr_state;		/* state bits */
	short tr_cntl;		/* meta packet control bits */
	short tr_amsg;		/* async receive register */
	short tr_avbytes;	/* max transmission allowed */
	short tr_rbytes;	/* size of <data> */
	short tr_cbytes;	/* current input record residual */
	short tr_bsize;		/* size of buffer request */
	short tr_schars;	/* singleton bytes in q1 */
	short tr_echo;
	short tr_enq;
	short tr_epoch;
	struct clist	tr_inpq;
	struct clist	tr_outq;
	struct clist	tr_ctlq;
	struct triocmb tr_iocb;
	char tr_sig;
	char tr_flag;
	char tr_xseq;			/* output sequence number */
	char tr_lseq;			/* last xseq acknowledged */
	char tr_rseq;			/* last seq received */
	char tr_time;			/* timeout delay */
};

#ifdef	KERNEL
struct tr trvec[NTR];
#endif


/*
 * control codes
 */
#define	CHK	040		/* enable checking of seq number */
#define	INIT	0100		/* allowable response to RESEQ msg */

#define	P_DATA		(01|INIT|CHK)
#define	P_CHAR		(02|INIT|CHK)
#define	P_MORE		(03|INIT|CHK)
#define P_CMD		(04|INIT|CHK)
#define	P_ERROR		(05|INIT|CHK)

#define	P_ACK		06
#define	P_REJ		07
#define	P_ALLOC		010

#define	P_ENQ		011
#define	P_RESP		012
#define	P_SYN		013
#define	P_RESYN		(014|INIT)
#define	P_IOCTL		(015|INIT)
#define	P_IOANS		016
#define	P_SIGNAL	023
#define	P_SIGANS	024
#define	NMSG		(024-010)

struct msg {
	char type;
	char seq;
	short param0;
	short param1;
	short param2;
	short param3;
	short param4;
	short param5;
	short param6;
};


/*
 * state bits
 */
#define	RXMIT	01		/* flag to start retransmission */
#define	R_ACK	02		/* have received an ack */
#define	LIVE	04		/* connection is live */
#define	T_ASYNC	010
#define	R_IOCTL	020		/* have received an ioctl */
#define	R_SYN	040		/* have received P_SYN */
#define	BREAD	0100
#define	S_REJ	0200		/* have sent one REJ */
#define	S_ENQ	0400		/* sending ENQs */
#define	R_ZERO	01000		/* zero length read pending */
#define	BWRITE	02000
#define	D_IOCTL	04000



/*
 * cntrl msg flags
 */
#define	X_CLOSE		01
#define	X_CLOSED	02
#define	X_SYN		04
#define	X_RESYN		010
#define	X_REJ		020
#define	X_ACK		040
#define X_ALL		0100
#define	X_ENQ		0200
#define	X_RESP		0400
#define	X_BREQ		01000
#define	X_BANS		02000
#define	X_IOCTL		040000
#define	X_IOANS		010000
#define	X_SIG		020000
#define	X_SIGANS	040000



/*
 * Circular trace buffer (debugging)
 */
struct trec {
	char dev;
	char state;
	char code;
	short seq;
	short p0;
	short p1;
};
#define	R	0100
#define S	0
#define	Tblock	0200
#define	Tmove	0201
#define	Trelease 0202
#define	Tmark	0203
#define	Tflush	0204
#define	Trxint	0205
#define	Topen	0206
#define	Tclose	0207
#define	Tsleep	0210
#define	Twrite	0211
#define	Tbwrite	0212
#define	Tbwait	0213
#define	Tread	0220
#define	Trend	0221
#define	Tsmeta	0222
#define	Tablock	0223
#define	Tbusy	0224
#define	Trxmeta 0225
#define Tlive	0226
#define	Tbread	0227
#define	Trxmit	0230
#define	Trzero	0231
#define	Trproc	0232
#define	Tdowrite 0233
#define	Tgetblk	0234
#define	Tbrelse	0235
#define	Tovf	0236
#define	Tseq	0237
#define	Tmeta	0240
#define	Tucount	0241
#define	Txint	0242
#define	Tadjust	0243
#define	TSioctl	0244
#define	Tdoctl	0245
#define	Tcounts 0246
#define	Ttimes 0247
#define	Tdel	0250

#define	TSIZE	250
