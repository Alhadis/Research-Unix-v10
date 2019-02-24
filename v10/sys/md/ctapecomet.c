/*
 * driver for comet (and nebula?) console cassette
 */

#include "sys/param.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/conf.h"

/*
 * registers -- all IPRs
 */
#define	CSRS	0x1c		/* receive status */
#define	CSRD	0x1d		/* receive data */
#define	CSTS	0x1e		/* transmit status */
#define	CSTD	0x1f		/* transmit data */

/*
 * bits in status registers
 */

#define	DONE	0200
#define	IE	0100
#define	BRK	01		/* transmit a break */

/*
 * packet types
 */

#define	DATA	01	/* data packet */
#define	CTL	02	/* command */
#define	INIT	04	/* init handshake */
#define	CONT	020	/* continue */
#define	STOP	023	/* stop (flow control) */

/*
 * opcodes in control packets
 */

#define	RESET	01	/* reset controller */
#define	READ	02	/* read a block */
#define	WRITE	03	/* write a block */
#define	END	0100	/* response from tu58 */

/*
 * switches
 */

#define	MRSP	010

/*
 * status codes in END packet
 */

#define	SOK	0
#define	SSOFT	1	/* ok but had to retry */
#define	SEOT	(-2)	/* ran off end of tape */
#define	SBBLK	(-55)	/* block number too big */

/*
 * misc sizes
 */

#define	TBSIZE	512	/* size of a block on the tape */
#define	PMAX	sizeof(struct dpkt)	/* biggest packet size */
#define	DMAX	128	/* max bytes of data per packet */
#define	CCNT	10	/* count for CTL packets */

/*
 * prototype control packet
 */

struct cpkt {
	char cp_flag;	/* CTL */
	unsigned char cp_cnt;	/* count == 012 */
	char cp_op;	/* op code */
	char cp_mod;	/* modifiers */
	char cp_unit;	/* unit number */
	char cp_sw;	/* switches (eg MRSP) */
	u_short cp_seq;	/* sequence number (always 0) */
	u_short cp_bcnt;	/* byte count */
	u_short cp_bno;	/* desired block number */
};

#define	ce_sts	cp_mod	/* for END packet, success code */
#define	ce_xsts	cp_bno	/* extended status */

/*
 * data packet
 */

struct dpkt {
	char cp_flag;	/* DATA */
	unsigned char cp_cnt;	/* count of data bytes following */
	char cd_data[DMAX];
};

typedef union packet {
	struct cpkt C;
	struct dpkt D;
	char B[PMAX];
	u_short W[PMAX / sizeof(u_short)];
} PACKET;

/*
 * quick and dirty pseudo-clist
 */

#define	CBDAT	512
#define	QHI	400
#define	QLO	200

struct cbuf {
	int cb_cc;
	unsigned char *cb_rptr;
	unsigned char *cb_wptr;
	unsigned char cb_data[CBDAT];
};

static struct cbuf ctuin, ctuout;
static int ctuflag;

int ctuopen(), ctuclose(), cturead(), ctuwrite();
struct cdevsw ctucdev = cdinit(ctuopen, ctuclose, cturead, ctuwrite, nodev);

#define	TBUSY	01	/* output busy */
#define	OPEN	02	/* device already open */
#define	WAIT	04	/* someone waiting for input */
#define	OSLEEP	010	/* someone waiting for output to drain */

#define	DEBUG	0100000	/* debugging flag */

#define	IPRI	28
#define	OPRI	29

#define	hibyte(x)	(((u_char *)&x)[1])
#define	lobyte(x)	(((u_char *)&x)[0])
#define	hiword(x)	(((u_short *)&x)[1])
#define	loword(x)	(((u_short *)&x)[0])

ctuopen(dev, mode)
{
	if (ctuflag & OPEN) {
		u.u_error = EBUSY;
		return;
	}
	ctuflag |= OPEN;
	ctuinit();
}

ctuclose(dev)
{

	mtpr(CSTS, 0);
	mtpr(CSRS, 0);
	ctuflag &=~ OPEN;
}

cturead(dev)
{
	PACKET pkt;
	register PACKET *pk = &pkt;

	if (u.u_count & 01)
		u.u_count--;	/* tu58 prefers even byte count */
	pk->C.cp_flag = CTL;
	pk->C.cp_cnt = CCNT;
	pk->C.cp_op = READ;
	pk->C.cp_mod = 0;
	pk->C.cp_unit = 0;
	pk->C.cp_sw = MRSP;
	pk->C.cp_seq = 0;
	pk->C.cp_bcnt = u.u_count;
	pk->C.cp_bno = Ltol(u.u_offset) / TBSIZE;
	ctuput(pk);
	for (;;) {
		if (ctuget(pk) == 0) {
			u.u_error = EIO;
			break;
		}
		if (u.u_count == 0)
			break;
		if (pk->C.cp_flag != DATA)
			break;
		iomove(pk->D.cd_data, pk->C.cp_cnt, B_READ);
		if (u.u_error)
			break;
	}
	if (pk->C.cp_flag != CTL || pk->C.cp_op != END)
		u.u_error = EIO;
	if (u.u_error) {
		ctuinit();
		return;
	}
	if (pk->C.ce_sts != SOK
	&&  pk->C.ce_sts != SSOFT
	&&  pk->C.ce_sts != SEOT	/* operation overlapped end of tape */
	&&  pk->C.ce_sts != SBBLK) {	/* operation past end of block */
		u.u_error = EIO;
		printf("err on TU58: %o %o\n", pk->C.ce_sts & 0377, pk->C.ce_xsts);
		return;
	}
}

ctuwrite(dev)
{
	PACKET pkt;
	register PACKET *pk = &pkt;

	if (u.u_count & 01)
		u.u_count--;
	pk->C.cp_flag = CTL;
	pk->C.cp_cnt = CCNT;
	pk->C.cp_op = WRITE;
	pk->C.cp_mod = 0;
	pk->C.cp_unit = 0;
	pk->C.cp_sw = MRSP;
	pk->C.cp_seq = 0;
	pk->C.cp_bcnt = u.u_count;
	pk->C.cp_bno = Ltol(u.u_offset) / TBSIZE;
	ctuput(pk);
	for (;;) {
		if (ctuget(pk) == 0) {
			u.u_error = EIO;
			break;
		}
		if (pk->C.cp_flag != CONT)
			break;
		if (u.u_count == 0)
			break;
		pk->C.cp_cnt = min(u.u_count, DMAX);
		iomove(pk->D.cd_data, pk->C.cp_cnt, B_WRITE);
		if (u.u_error)
			break;
		pk->C.cp_flag = DATA;
		ctuput(pk);
	}
	if (pk->C.cp_flag != CTL || pk->C.cp_op != END)
		u.u_error = EIO;
	if (u.u_error) {
		ctuinit();
		return;
	}
	if (pk->C.ce_sts != SOK
	&&  pk->C.ce_sts != SSOFT
	&&  pk->C.ce_sts != SEOT) {
		u.u_error = EIO;
		printf("err on TU58: %o %o\n", pk->C.ce_sts & 0377, pk->C.ce_xsts);
		return;
	}
}

/*
 * send a packet to the tu58
 */

ctuput(pk)
register PACKET *pk;
{
	register u_short *wp;
	register char *cp;
	register int n;
	long sum;

	if (ctusbc(pk->C.cp_flag)) {
		ctputc(pk->C.cp_flag, &ctuout);
		ctustart();
		return;
	}
	while (ctuout.cb_cc >= QHI) {
		ctuflag |= OSLEEP;
		sleep((caddr_t)&ctuout, OPRI);
	}
	wp = pk->W;
	sum = 0;
	n = (pk->C.cp_cnt / sizeof(u_short)) + 1;
	while (--n >= 0) {
		sum += *wp++;
		if (hiword(sum)) {	/* wrap carry around */
			sum++;
			hiword(sum) = 0;
		}
	}
	cp = pk->B;
	n = pk->C.cp_cnt + 2;
	while (--n >= 0)
		ctputc(*cp++, &ctuout);
	ctputc(lobyte(sum), &ctuout);
	ctputc(hibyte(sum), &ctuout);
	ctustart();
}

/*
 * retrieve a packet from the tu58
 * returns zero if packet is illegal (eg bad checksum)
 * if no packet, wait for one
 */

ctuget(pk)
register PACKET *pk;
{
	register u_short *wp;
	register char *cp;
	register int n;
	long sum;
	u_short xsum;

	pk->C.cp_flag = ctuchr();
	if (ctusbc(pk->C.cp_flag))
		return (1);
	pk->C.cp_cnt = ctuchr();
	cp = &pk->B[2];
	n = pk->C.cp_cnt;
	if (n > PMAX)
		return (0);
	while (--n >= 0)
		*cp++ = ctuchr();
	lobyte(xsum) = ctuchr();
	hibyte(xsum) = ctuchr();
	wp = pk->W;
	n = (pk->C.cp_cnt / sizeof(u_short)) + 1;
	sum = 0;
	while (--n >= 0) {
		sum += *wp++;
		if (hiword(sum)) {	/* wrap carry around.  ugh. */
			sum++;
			hiword(sum) = 0;
		}
	}
	if (ctuflag & DEBUG)
		printf("ctu %x\n", pk->C.cp_flag);
	if (sum != xsum) {
		printf("ctu: sum %x is %x\n", xsum, sum);
		return (0);
	}
	return (1);
}

/*
 * is this type a single byte packet?
 */

ctusbc(type)
register int type;
{

	switch (type) {
	case DATA:
	case CTL:
		return (0);

	default:
		return (1);
	}
}

/*
 * hard reset
 * called on open or error
 */

ctuinit()
{

	mtpr(CSRS, 0);
	mtpr(CSTS, BRK);
	DELAY(250000);
	mtpr(CSTS, 0);
	ctflushq(&ctuin);
	ctflushq(&ctuout);
	mtpr(CSRS, IE);
	ctputc(INIT, &ctuout);
	ctputc(INIT, &ctuout);
	ctustart();
	while (ctuchr() != CONT)
		;
}

/*
 * wait for one character from tu58
 */

ctuchr()
{
	register int s;

	s = spl7();
	while (ctuin.cb_cc == 0) {
		ctuflag |= WAIT;
		sleep((caddr_t)&ctuin, IPRI);
	}
	splx(s);
	return (ctgetc(&ctuin));
}

/*
 * start output from queue
 */

ctustart()
{
	register int c;
	register int s;

	s = spl7();
	if (ctuflag & TBUSY)
		return;
	if ((c = ctgetc(&ctuout)) >= 0) {
		ctuflag |= TBUSY;
		mtpr(CSTD, c);
		mtpr(CSTS, IE);
	}
	if (ctuflag & OSLEEP && ctuout.cb_cc < QLO) {
		wakeup((caddr_t)&ctuout);
		ctuflag &=~ OSLEEP;
	}
	splx(s);
}

/*
 * transmitter interrupt
 */
ctu1int(dev)
{

	ctuflag &=~ TBUSY;
	ctustart();
}

/*
 * receiver interrupt
 */
ctu0int(dev)
{
	static char cont = CONT;

	while (mfpr(CSRS) & DONE) {
		ctputc(mfpr(CSRD), &ctuin);
		ctuput((PACKET *)&cont);
		if (ctuin.cb_cc > QHI) {
			printf("ctu input overrun\n");
			ctflushq(&ctuin);
		}
	}
	if (ctuflag & WAIT) {
		ctuflag &=~ WAIT;
		wakeup((caddr_t)&ctuin);
	}
}

/*
 * fake clist routines
 */

ctflushq(q)
register struct cbuf *q;
{
	register int s;

	s = spl7();
	q->cb_cc = 0;
	q->cb_rptr = q->cb_wptr = q->cb_data;
	splx(s);
}

int
ctgetc(q)
register struct cbuf *q;
{
	register int s;
	register int c;

	s = spl7();
	if (q->cb_cc <= 0)
		c = -1;
	else {
		q->cb_cc--;
		c = *q->cb_rptr++;
		if (q->cb_rptr >= &q->cb_data[CBDAT])
			q->cb_rptr = q->cb_data;
	}
	splx(s);
	return (c);
}

ctputc(c, q)
char c;
register struct cbuf *q;
{
	register int s;

	s = spl7();
	q->cb_cc++;
	*q->cb_wptr++ = c;
	if (q->cb_wptr >= &q->cb_data[CBDAT])
		q->cb_wptr = q->cb_data;
	splx(s);
}
