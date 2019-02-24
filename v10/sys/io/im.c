/*
	Imagitek Scanner -- Andrew Hume
		1100 (?), on DR-11/W
	Ninth Edition Unix (related to BSD 4.1c)
*/

int imdebug = 0;
int imdebug1 = 0;
int imdebug2 = 0;
#define		IMKEEPINT		100/**/

enum states {
	IDLE,		/*0 idle or finishing off read */
	WRITEPROBE,	/*1 waiting for scanner ack */
	WRITE,		/*2 started dma to scanner */
	WRITEDMA,	/*3 got dma after WRITE */
	WRITEATTN,	/*4 got attn after WRITE */
	READPROBE,	/*5 waiting for ATTN from scanner */
	READ,		/*6 ready to start dma from scanner */
	RESETTING	/*7 imreset(dev) */
};
#include "sys/param.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/ubaddr.h"
#include "sys/systm.h"

#include "sys/im.h"

struct imdevice {
	short	wcr;		/* Unibus word count reg */
	u_short	bar;		/* Unibus address register */
	u_short	csr;		/* Unibus status/command reg */
	u_short	idr;		/* Input Data Register */
};
#define	eir	csr		/* Error and Information Register */
#define	odr	idr		/* Output Data Register */

/*
 * Unibus status/command register bits
 */
#define	GO		0x0001
#define	HSTRB		0x0002		/* host strobe fn1 */
#define	HINTR		0x0004		/* host interrupt fn2 */
#define	HRESET		0x0008		/* host reset fn3 */
#define XMEM		0x0030
#define xmem(a)		(((a) >> 12)&XMEM)
#define	IENABLE		0x0040
#define	READY		0x0080
#define	SSTRB		0x0200		/* scanner strobe statc */
#define	SINTR		0x0400		/* scanner interrupt statb */
#define	SRESET		0x0800		/* scanner interface reset stata */
#define	MAINT		0x1000
#define	ATTN		0x2000		/* attention */
#define	NEX		0x4000
#define	ERROR		0x8000

/*
 * config glue
 */
int imopen(), imclose(), imread(), imwrite(), imioctl();

extern struct im im[];
extern struct ubaddr imaddr[];
extern int imcnt;
struct cdevsw imcdev = cdinit(imopen, imclose, imread, imwrite, imioctl);

/*
 * IOCTL
 */
#define	IMRESET	(('i'<<8)|1)
#define	IMWAIT	(('i'<<8)|2)

#define DMAPRI (PZERO-1)
#define WAITPRI (PZERO+1)
#define	FUNCMASK	(HSTRB|HINTR|HRESET)

/*
	this is a finite state automata driven by the interrupt routine
*/


#ifdef	IMKEEPINT
typedef struct iminterrupt
{
	short csr, ostate, nstate, attn;
	long time;
} iminterrupt;
iminterrupt imi[IMKEEPINT], *nimi;
#endif
#define		IMN	1000
static short wcnt[IMN], rcnt[IMN], rscnt[IMN], wi, ri;

imopen(d, f)
{
	register struct im *imp;
	register int dev;

	if((dev = minor(d)) >= imcnt) {
		u.u_error = ENODEV;
		return;
	}
	if((imp = &im[dev])->open) {
		u.u_error = EBUSY;
		return;
	}
	if((imp->addr = (struct imdevice *)ubaddr(&imaddr[dev])) == 0
	  || ubbadaddr(imaddr[dev].ubno, (caddr_t)imp->addr, sizeof(u_short))) {
		printf("im%d absent\n", dev);
		u.u_error = ENODEV;
		return;
	}
	imp->addr->csr = 0;
	imp->open = 1;
	imp->state = IDLE;
#ifdef	IMKEEPINT
	nimi = imi;
#endif
	wi = ri = 0;
}

imclose(dev)
{
	register struct im *imp = &im[minor(dev)];

	if(imp->state != IDLE)
		imreset(imp);
	imp->open = 0;
#ifdef	IMKEEPINT
	if(imdebug1){
		iminterrupt *i;

		for(i = imi; i < nimi; i++)
			printf("%d: csr=0x%x attn=%d ostate=%d nstate=%d\n",
				i->time, i->csr&0xFFFF, i->attn, i->ostate, i->nstate);
	}
#endif
	if(imdebug2){
		register i;

		printf("read: ");
		for(i = 0; i < ri; i++)
			printf("%d:%d[%d] ", i, rcnt[i], rscnt[i]);
		printf("\nwrite: ");
		for(i = 0; i < wi; i++)
			if(wcnt[i]) printf("%d:%d ", i, wcnt[i]);
		printf("\n");
	}
}

u_int
imminphys(bp)
	register struct buf *bp;
{

	if(bp->b_bcount > 65536)	/* may be able to do twice as much */
		bp->b_bcount = 65536;
}

imstrategy(bp)
register struct buf *bp;
{
	register struct im *imp = &im[minor(bp->b_dev)];
	register int s;
	uaddr_t uaddr;
	u_short csr;

	if(bp->b_bcount<=0 || (bp->b_bcount&1) || ((int)bp->b_un.b_addr&1)){
		bp->b_flags |= B_ERROR;
		iodone(bp);
		return;
	}
	imp->ubm = ubmbuf(imaddr[minor(bp->b_dev)].ubno, bp, UBDP|USLP);
	uaddr = ubadbuf(imaddr[minor(bp->b_dev)].ubno, bp, imp->ubm);
	s = spl5();
	csr = IENABLE| xmem(uaddr) | ((bp->b_flags&B_READ) ? 0: HINTR);
	imp->addr->wcr = -(bp->b_bcount>>1);	/* it's a word count */;
	imp->addr->bar = uaddr;
	imp->addr->csr = csr;	/* No GO bit; let function codes settle */
	imp->addr->csr = csr|GO;
	if(tsleep((caddr_t)imp, DMAPRI+1, 15) != TS_OK){
		printf("im dma timeout: csr=0x%x wcr=%d\n",
			imp->addr->csr, imp->addr->wcr);
		/* stop the dma */
		imslam(imp->addr);
		imp->addr->wcr = 0xFFFF;	/* gently; -1 first... */
		imp->addr->wcr = 0x0000;	/* then zero */
		/* reset device */
		imreset(imp);
		bp->b_flags |= B_ERROR;
	}
	splx(s);
	ubmfree(imaddr[minor(bp->b_dev)].ubno, imp->ubm);
	bp->b_resid = -(imp->addr->wcr<<1);
	iodone(bp);
}

imread(dev)
{
	register struct im *imp = &im[minor(dev)];
	int start;

	start = time*HZ+lbolt;
	if(imdebug)
		printf("read: ");
	while((imp->state == WRITEDMA) || (imp->state == WRITEATTN) ||
			(imp->state == READPROBE)){
		if(tsleep((caddr_t)imp, DMAPRI+1, 15) != TS_OK){
			imreset(imp);
			return;
		}
	}
	rscnt[ri] = time*HZ+lbolt-start;
	if(imp->state != READ){
		u.u_error = EGREG;
		return;
	}
	u.u_count += 2;
	imp->addr->csr = IENABLE;	/* HSTRB ??*/
	physio(imstrategy, &imp->buf, dev, B_READ, imminphys);
	u.u_count -= 2;
	imp->addr->csr = IENABLE;
	rcnt[ri++] = time*HZ+lbolt-start;
}

imwrite(dev)
{
	register struct im *imp = &im[minor(dev)];
	register int start, s, x;

	start = time*HZ+lbolt;
	if(imp->state != IDLE){
		u.u_error = EGREG;
		return;
	}
	if(imp->addr->csr&(SINTR|SRESET)){	/* is it offline ? */
		printf("im: offline csr=0x%x\n", imp->addr->csr);
		u.u_error = ENXIO;
		return;
	}
	s = spl5();
	imp->state = WRITEPROBE;
	imp->addr->csr = IENABLE | HINTR | HSTRB;	/* poke scanner */
	if(((x = tsleep((caddr_t)imp, DMAPRI+1, 15)) != TS_OK) ||
	   (imp->state != WRITE)){
		printf("im%d: wtimo - type %d, state %d\n", x, imp->state);
		imreset(imp);
		return;
	}
	imp->addr->csr = HINTR;
	splx(s);
	physio(imstrategy, &imp->buf, dev, B_WRITE, imminphys);
	/*
		the write is done although we may not have both interrupts
	*/
	wcnt[wi++] = time*HZ+lbolt-start;
}

imslam(addr)
register struct imdevice *addr;
{
	register i;

	addr->csr = MAINT;
	for(i = 0; i < 10; i++)
		;
	addr->csr = 0;
	for(i = 0; i < 10; i++)
		;
}

/*ARGSUSED*/
imioctl(dev, cmd, addr, flag)
caddr_t addr;
{
	register struct im *imp = &im[minor(dev)];

	switch(cmd) {

	case IMRESET:
		spl5();
		imreset(imp);
		if((tsleep((caddr_t)imp, WAITPRI, 15) != TS_OK) || (imp->state != IDLE)){
			spl0();
			u.u_error = EIO;
			imp->state = IDLE;
			return;
		}
		u.u_error = 0;
		spl0();
		break;

	case IMWAIT:
		if(imp->state != READPROBE){
			if(imdebug)
				printf("im: wait state=%d\n", imp->state);
			return;
		}
		if((tsleep((caddr_t)imp, WAITPRI, 150) != TS_OK) || (imp->state != READ)){
			u.u_error = EIO;
			return;
		}
		u.u_error = 0;
		break;

	default:
		u.u_error = ENOTTY;
		break;
	}
}

im0int(dev)
{
	register struct im *imp = &im[dev];
	u_short csr;
	int attn = 0;
	int ostate = imp->state;
	int dma;

	if((csr = imp->addr->csr) & ATTN)
		attn = 1;
	else if(csr&ERROR) {
		imp->addr->eir |= ERROR;
		printf("im: read goo CSR %x EIR %x BAR %x WCR %x\n",
			csr, imp->addr->eir, imp->addr->bar, imp->addr->wcr);
		imp->addr->csr = csr&~FUNCMASK;
		imreset(imp);
	}
#ifdef	IMKEEPINT
	nimi->time = time*HZ+lbolt;
	nimi->csr = csr;
	nimi->attn = attn;
	nimi->ostate = ostate;
	nimi->nstate = -1;
	if(++nimi >= &imi[IMKEEPINT])
		nimi--;
#endif
	if(imdebug)
		printf("imintr: csr=0x%x ostate=%d attn=%d ", csr, ostate, attn);
	if(!(csr&READY))
		return;
	switch(imp->state) {

	case IDLE:
		break;

	case WRITEPROBE:
		if(attn)
			imp->state = WRITE;
		else
			imreset(imp);
		break;

	case WRITE:
		dma = (csr&READY) && (imp->addr->wcr == 0);
		if(attn){
			if(dma){
				imp->addr->csr = IENABLE;
				imp->state = READPROBE;
			} else
				imp->state = WRITEATTN;
		} else {
			if(!dma)
				printf("im: phooey! wcr=%d\n", imp->addr->wcr);
			/* bad value: imp->state = WRITEDMA;*/
			imp->addr->csr = IENABLE;
			imp->state = READPROBE;
		}
		break;

	case WRITEDMA:
		if(!attn)
			printf("im: scrunt\n");
		imp->addr->csr = IENABLE;
		imp->state = READPROBE;
		break;

	case WRITEATTN:
		if(attn)
			printf("im: poot\n");
		imp->addr->csr = IENABLE;
		imp->state = READPROBE;
		break;

	case READPROBE:
		if(attn)
			imp->state = READ;
		else
			imreset(imp);
		break;

	case READ:
		/*
			because we may not get both an attn and a dma done
			drop into idle. if we get neither, something will screw
			up; that's ok because the user is in trouble anyway.
		*/
		dma = (csr&READY) && (imp->addr->wcr == 0);
		if(imdebug)
			printf("DMA=%d: ", dma);
		if((csr & (SRESET|SINTR|SSTRB)) != SSTRB)
			imreset(imp);
		else
			imp->state = IDLE;
		break;

	case RESETTING:
		if(!attn || (csr & SSTRB)){
			printf("im: bad reset\n");
		}
		imp->addr->csr &= ~FUNCMASK;
		imp->state = IDLE;
		break;

	default:
		printf("im0int: unknown state %d\n", imp->state);
		imreset(imp);
	}
	if(imdebug)
		printf("new state=%d\n", imp->state);
#ifdef	IMKEEPINT
	nimi[-1].nstate = imp->state;
#endif
	wakeup((caddr_t)imp);
}

imreset(imp)
register struct im *imp;
{
	register int s;
	u_short csr;

	s = spl5();
	if(imdebug)
		printf("imreset: state=%d\n", imp->state);
	if(imp->addr->csr&SRESET)
		return;
	imp->state = RESETTING;
	csr = IENABLE | HINTR | HRESET | HSTRB;
	imp->addr->csr = csr;
	splx(s);
}
