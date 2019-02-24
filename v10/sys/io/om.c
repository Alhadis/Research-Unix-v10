/*% cyntax -c -DKERNEL % && cc -c -DKERNEL %
 * Metheus 3610 driver via dr11-w for 9Vr2
 */
#include "sys/param.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/ubaddr.h"
#include "sys/om.h"
#define	OMTIMEOUT	15	/* seconds */
int omopen(), omclose(), omread(), omwrite(), omioctl();
struct cdevsw omcdev=cdinit(omopen, omclose, omread, omwrite, nodev);
int omcnt;			/* defined in conf */
extern struct ubaddr omaddr[];		/* defined in conf */
#define	OPEN	1		/* somebody has the device open */
#define	RUNNING	2		/* dma in progress */
#define	LATE	4		/* dma ready to time out */
#define	NOTIME	8		/* don't bother with timeout */
int omtiming;			/* timeout exists */
/*
 * dr11-w hardware registers
 */
struct omreg{
	short wc;
	short addr;
	short csr;
	short data;
};
/*
 * csr bits
 */
#define	ERROR	0100000
#define	ATTN	020000
#define	READY	0200
#define	IENABLE	0100
#define	XMEM	060
#define	xmem(a) (((a)>>12)&XMEM)
#define	WAKEUP	010
#define	READ	02
#define	GO	01
int omtimeout();
omopen(dev, flags){
	register d=minor(dev);
	register struct om *p=&om[d];
	if(d>=omcnt
	|| p->flag&OPEN){
		u.u_error=ENODEV;
		return;
	}
	if((p->reg=(struct omreg *)ubaddr(&omaddr[d]))==0
	|| ubbadaddr(omaddr[d].ubno, (caddr_t)&p->reg->csr, sizeof(short))){
		printf("om%d absent\n", d);
		u.u_error=ENODEV;
		return;
	}
	p->flag|=OPEN;
	if(omtiming==0) {
		omtiming=1;
		timeout(omtimeout, (caddr_t)0, OMTIMEOUT*HZ);
	}
}
omclose(dev){
	om[minor(dev)].flag&=~OPEN;
}
omstrategy(bp)
struct buf *bp;
{
	register d=minor(bp->b_dev), csr, pri;
	register struct om *p=&om[d];
	register struct omreg *omreg;
	register uaddr_t ubad;
	if(bp->b_bcount&1){
		bp->b_flags|=B_ERROR;
		iodone(bp);
		return;
	}
	p->ubm=ubmbuf(omaddr[d].ubno, bp, UBDP|USLP);
	ubad=ubadbuf(omaddr[d].ubno, bp, p->ubm);
	pri=spl5();
	omreg=p->reg;
	omreg->wc=-(bp->b_bcount>>1);
	omreg->addr=ubad;
	csr=IENABLE|xmem(ubad);
	if(bp->b_flags&B_READ) csr|=READ;
	omreg->csr=csr;
	omreg->csr=csr|GO;
	if ((bp->b_flags & B_READ) == 0) {
		p->flag |= RUNNING;
		splx(pri);
		return;
	}
	/*
	 * reading: no timeout (might wait forever for mouse data)
	 * hence must allow signals
	 * hence do our own sleep
	 */
	p->flag |= (RUNNING|NOTIME);
	if (tsleep((caddr_t)bp, PZERO+1, 0) != TS_OK) {
		/* signal: tear down by hand */
		omreg->csr = ATTN|WAKEUP|IENABLE;
		bp->b_flags |= B_ERROR;
		ubmfree(omaddr[d].ubno, p->ubm);
		iodone(bp);
		p->flag &=~ (RUNNING|NOTIME|LATE);
	}
	splx(pri);
}
omread(dev){
	physio(omstrategy, &om[minor(dev)].buf, dev, B_READ, minphys);
}
omwrite(dev){
	physio(omstrategy, &om[minor(dev)].buf, dev, B_WRITE, minphys);
}
om0int(d){
	register struct buf *bp;
	register struct om *p=&om[d];
	register struct omreg *omreg=p->reg;
	register csr=omreg->csr;
	if(p->flag&RUNNING && csr&(READY|ATTN)){
		omreg->csr=0;
		bp=&p->buf;
		if(csr&ERROR)
			bp->b_flags|=B_ERROR;
		bp->b_resid=(-omreg->wc)<<1;
		ubmfree(omaddr[d].ubno, p->ubm);
		iodone(bp);
		p->flag&=~(RUNNING|NOTIME|LATE);
	}
}
omtimeout(){
	register struct om *p, *ep;
	register int s;
	ep=&om[omcnt];
	s=spl5();
	for(p=&om[0];p!=ep;p++) if((p->flag&(RUNNING|NOTIME))==RUNNING){
		if((p->flag&LATE)==0)
			p->flag|=LATE;
		else {
			p->reg->csr=ATTN|WAKEUP|IENABLE;
			ubmfree(omaddr[p-om].ubno, p->ubm);
			p->buf.b_flags|=B_ERROR;
			iodone(&p->buf);
			p->flag&=~(RUNNING|LATE);
		}
	}
	splx(s);
	timeout(omtimeout, (caddr_t)0, OMTIMEOUT*HZ);
}
