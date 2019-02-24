/*
 * This software is provided solely for use with
 * the National Instruments GPIB11-2.
 *
 * Copyright 1980, 1983 National Instruments
 *
 * Jeffrey Kodosky
 * REV D:  10/30/83
 * (4.1bsd mods)
 *
 * utterly hopeless
 */
#include "sys/param.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/ubaddr.h"
#include "sys/ttyio.h"

#include "sys/callout.h"

static init(), irload(), lun(), wsetup(), rsetup(), ibdone(), gts(), tcs();
static unhold(), xfer(), ldaux(), wait100us();

#define TRI	0	/* 4 for Three-state highspeed timing; 0 for Normal timing */
#define EXT	1	/* 2 for Extended; 1 for Normal GPIB addressing */
#define MSA	0140	/* Msa&037 for Extended; 0140 for Normal GPIB addressing */
#define MA	025	/* GPIB address */
#define	DONE	0x0080	/* this is what they wanted */


#define SIGSRQ	SIGINT


#define INACTIVE   0		/* Software controller states */
#define IDLE	   1
#define INCHARGE   2
#define STANDBY	   3

#define GO	1		/* Control/status register bits */
#define OUT	2
#define SEL	4
#define ECC	010
#define XBA	060
#define IE	0100
#define LMR	0200
#define CIC	0400
#define ATN	01000
#define EOI	02000
#define OCSW	02000
#define TCS	04000
#define DAV	04000
#define SRQ_IE	010000
#define SRQ	010000
#define INT	020000
#define DMA_ENAB 020000
#define NEX	040000
#define REN	040000
#define IFC	0100000

#define DIR	0		/* Internal register addresses */
#define DOR	0
#define ISR1	1
#define IMR1	1
#define ISR2	2
#define IMR2	2
#define SPS	3
#define SPM	3
#define ADS	4
#define ADM	4
#define CPT	5
#define AUX	5
#define AD0	6
#define ADR	6
#define AD1	7
#define EOS	7

#define ERR_IE	    4		/* Internal register bits */
#define END_IE	  020
#define CPT_IE	 0200
#define DMAI_ENAB 020
#define DMAO_ENAB 040
#define SRQS	 0100
#define RSV	 0100
#define TA	    2
#define LA	    4
#define LON	 0100
#define TON	 0200
#define DL	  040
#define DT	 0100
#define ARS	 0200

#define _CLKR	  040		/* Hidden register addresses & offsets */
#define _PPR	 0140
#define _AUXA	 0200
#define _AUXB	 0240
#define CLKR	    0
#define PPR	    1
#define AUXA	    2
#define AUXB	    3

#define U	  020		/* Hidden register bits */
#define BIN	  020
#define S	  010
#define REOS	    4
#define HLDE	    2
#define HLDA	    1
#define CPT_ENAB    1
#define PACS	    1		/* Software status bits */
#define MON	    2

#define IST	011		/* Special interface functions */
#define NIST	  1
#define VSC	017
#define NVSC	  7
#define SEOI	  6
#define FH	  3
#define IR	  2
#define PON	  0

#define OK	   1	/* Error codes */
#define ENONE	  -1	/* No command byte available (READCOMMAND) */
#define ECACFLT	  -2	/* ATN not unasserted after IFC sent (bus problem) */
#define ENOTCAC	  -3	/* Not Active Controller for operation requiring CAC (software problem) */
#define ENOTSAC	  -4	/* Not System Controller for operation requiring SAC (software problem) */
#define EIFCLR	  -5	/* IFC caused operation to abort (bus problem) */
#define ETIMO	  -6	/* Operation did not complete within allotted time (bus problem) */
#define ENOFUN	  -7	/* Non-existent function code (software problem) */
#define ETCTIMO	  -8	/* Take control not completed within allotted time (bus problem) */
#define ENOIBDEV  -9	/* No Listeners addressed or no devices connected (bus problem) */
#define EIDMACNT -10	/* Internal DMA completed without bcr going to 0 (hardware problem) */
#define ENOPP	 -11	/* PP operation attempted on three-state GPIB (software problem) */
#define EITIMO	 -12	/* Internal DMA did not complete within allotted time (hardware problem) */
#define EINEXM	 -13	/* Internal DMA aborted due to non-existent memory (software/hardware problem) */
#define ENEXMEM	 -14	/* GPIB DMA aborted due to non-existent memory (software/hardware problem) */
#define ECNTRS	 -15	/* Bar and bcr are inconsistent following GPIB DMA (hardware problem) */

#define RQC_STB	 (RSV | 1)	/* Service request status bytes */
#define RQT_STB	 (RSV | 2)
#define RQL_STB	 (RSV | 4)

#define RQC	1		/* Asynchronous op codes */
#define CAC	2
#define TAC	3
#define LAC	4
#define CWT	5
#define WSRQ	6

#define TCT	011		/* GPIB multiline messages */
#define PPC	5
#define PPU	025
#define SCG	0140

#define IN	0
#define ITIMO	25		/* Internal loopcount timeout */
#define GTIMO	10		/* Default GPIB timeout in seconds */
#define TCTIMO	100
#define MONHWAT 32

#define RD	(DMA_ENAB|TCS|IN|GO)
#define WT	(DMA_ENAB|TCS|OUT|GO)
#define RDIR	(DMA_ENAB|IN|SEL|GO)
#define WTIR	(DMA_ENAB|OUT|SEL|GO)

static struct buf ibbuf;
static short iboddc, ibodda;

#define in(a)		ib.addr->a
#define out(a,v)	ib.addr->a=(v)
#define Exclude		sps=spl5()
#define Unexclude	splx(sps)
#define Try(f)		if((x=(f))<0) return x

struct	ib {
	short unsigned	bcr, bar, csr, ccf;	/* Unibus device registers */
	char	internal[8];		/* Internal registers */
	char	hidden[4];		/* Hidden registers */
	char	cstate, istr, op;
	int	ans;
	int	timo;			/* Watchdog timer */
	struct proc *owner;		/* GPIB owning process */
	short unsigned	rdbcr, rdbar, rdcsr;
	char	rdinternal[8];
	int	arg[3];

	struct ib *addr;
	int ubno;
	ubm_t ubm, ioubm;
	uaddr_t uaddr, iouaddr;
} ib;

static int status(), spbyte();
static int command(), transfer(), clear(), remote(), local(), ppoll();
static int passctrl(), setstat(), monitor(), readcmd(), setparam(), testsrq();

int (*ibfn[])() =
{
	command, transfer, clear, remote, local, ppoll,
	passctrl, setstat, monitor, readcmd, setparam, testsrq,
	status, spbyte
};

#define NFNS	((sizeof ibfn)/(sizeof ibfn[0]))

/*
 * config glue
 */
int gpibopen(), gpibclose(), gpibread(), gpibwrite(), gpibioctl();

extern struct ubaddr gpibaddr[];
extern int gpibcnt;
struct cdevsw gpibcdev =
	cdinit(gpibopen, gpibclose, gpibread, gpibwrite, gpibioctl);


gpibopen(d, f)
{
	register int dev, s;
	register struct ib *p;

	if((dev = minor(d)) >= gpibcnt || gpibcnt > 1) {
		u.u_error = ENODEV;
		return;
	}
	if((p = &ib)->owner) {
		u.u_error = EBUSY;
		return;
	}
	if((p->addr = (struct ib *)ubaddr(&gpibaddr[dev])) == 0
	  || ubbadaddr(gpibaddr[dev].ubno, (caddr_t)p->addr, sizeof(u_short))) {
		printf("gpib%d absent\n", dev);
		u.u_error = ENODEV;
		return;
	}
	p->ubno = gpibaddr[dev].ubno;
	p->ubm = ubmalloc(p->ubno, sizeof(struct ib), USLP);
	p->uaddr = ubmaddr(p->ubno, p, sizeof(struct ib), p->ubm);
	s = spl5();
	if((ib.ans=init())<0){
		ubmfree(p->ubno, p->ubm);
		u.u_error= EIO;
	}
	else ib.owner= u.u_procp;
	splx(s);
}

gpibclose(dev)
{
	register int sps, x;

	Exclude;
	ubmfree(ib.ubno, ib.ubm);
	ib.cstate= INACTIVE;
	ib.csr= 0;
	out(csr,LMR);
	ib.owner= 0;
	Unexclude;
}

gpibioctl(dev,cmd,addr)
	caddr_t addr;
{
	register int sps, x;

	Exclude;
	switch(cmd){
	case TIOCGETP:
		ib.arg[0]= ib.ans;
		ib.arg[1]= in(csr);
		ib.arg[2]= in(bcr);
		if(copyout(ib.arg,addr,sizeof ib.arg)) u.u_error= EFAULT;
		break;
	case TIOCSETP:
		if(copyin(addr,ib.arg,sizeof ib.arg)){
			u.u_error= EFAULT;
			break;
		}
		if((x=ib.arg[0])<0 || x>=NFNS){
			ib.ans= ENOFUN; 
			u.u_error= EIO;
		}
		else if((ib.ans= (*ibfn[x])())<0) u.u_error= EIO;
		break;
	default: 
		u.u_error= ENOTTY;
		break;
	}
	Unexclude;
}

static
command()
{
	ib.op= CWT;
	return OK;
}

static
transfer()
{
	return gts(EXT);
}

static
clear()
{
	register int x;

	Try(unhold());
	out(csr, ib.csr | IFC);
	wait100us();
	if(in(csr)&ATN) return ECACFLT;
	ib.cstate= INCHARGE;
	out(csr, (ib.csr |= ATN | SRQ_IE | CIC));
	return lun(TON);
}

static
remote()
{
	out(csr, (ib.csr |= REN));
	return OK;
}

static
local()
{
	out(csr, (ib.csr &= ~REN));
	return OK;
}

static
ppoll()
{
	register int x;

	Try(tcs());
	Try(lun(LON));
	out(csr, (ib.csr |= EOI));
	Try(xfer(RDIR, &ib.rdinternal[CPT], 1, CPT));
	out(csr, (ib.csr &= ~EOI));
	return (ib.rdinternal[CPT]&0377|0400);
}

static
passctrl()
{
	return ENOIBDEV;
}

static
setstat()
{
	return OK;
}

static
monitor()
{
	return OK;
}

static
readcmd()
{
	return ENONE;
}

static
setparam()
{
	ib.timo= ib.arg[1];
	ib.internal[0]= ib.arg[2];
	ib.internal[EOS]= ib.arg[2]>>8;
	return OK;
}

static
testsrq()
{
	int ibtimer();

	if(in(csr)&SRQ) return OK;
	if(ib.csr&CIC) out(csr, (ib.csr |= SRQ_IE));
	if(ib.arg[1]){
		ib.op= WSRQ;
		if(ib.timo)
			timeout(ibtimer,0,ib.timo*HZ);
		sleep(&ibbuf,10);
		return ib.ans;
	}
	return ENONE;
}

static
status()
{
	register char *c;
	register int ct;

	c= (caddr_t)&ib;
	u.u_base= (caddr_t)ib.arg[1];
	if((u.u_count=ib.arg[2]) > sizeof ib)
		u.u_count= sizeof ib;
	ct= u.u_count;
	while(passc(*c++)>=0) ;
	return ct;
}

static
spbyte()
{
	register int x;

	return OK;
}

static
init()
{
	register char *cp;
	register int x;

	out(csr, LMR);
	ib.csr= 0;
	cp= &ib.hidden[0];
	*cp++= _CLKR | 6;
	*cp++= _PPR | U;
	*cp++= _AUXA;
	*cp++= _AUXB | TRI | CPT_ENAB;

	cp= &ib.internal[0];
	*cp++= 0;
	*cp++= CPT_IE | ERR_IE;
	*cp++= 0;
	*cp++= 0;
	*cp++= EXT;
	*cp++= ib.hidden[CLKR];
	*cp++= ARS | MSA;
	*cp++= 0;

	ib.istr= 0;
	ib.op= 0;
	ib.ans= 0;
	ib.timo= GTIMO;
	Try(irload());
	ib.cstate= IDLE;
	out(csr, ib.csr= IE);
	return 0;
}

static
ibstop()
{
	register int x;

	out(csr, (ib.csr &= ~(DMA_ENAB|GO)));
	ib.op= 0;
	ibdone(&ibbuf);
	return ETIMO;
}

static
irload()
{
	register int x;

	Try(xfer(WTIR,&ib.internal[ISR1],7,ISR1));
	ib.internal[AUX]= ib.hidden[AUXA];
	ib.internal[ADR]= MA;
	Try(xfer(WTIR,&ib.internal[AUX],2,AUX));
	Try(xfer(WTIR,&ib.hidden[AUXB],1,AUX));
	x= ib.internal[ADM];
	ib.internal[ADM]= 0;
	return lun(x);
}

static
lun(newadm)
	char newadm;
{	/* Note: rsv is cleared and not restored*/
	register int x;

	if(ib.internal[ADM]==newadm) return OK;
	ib.internal[ADM]= newadm;
	ib.internal[AUX]= PON;
	Try(xfer(WTIR,&ib.internal[ADM],2,ADM));
	Try(xfer(WTIR,&ib.hidden[PPR],1,AUX));
	return (ib.istr&S)? ldaux(IST): OK;
}

gpibwrite(dev)
{
	register int sps;
	int ibstrategy();

	Exclude;
	if((ib.ans=wsetup())>=0)
		physio(ibstrategy, &ibbuf, dev, B_WRITE, minphys);
	ib.op= 0;
	if(ib.ans<0) u.u_error= EIO;
	Unexclude;
}

static
wsetup()
{
	register x;

	ib.ccf= 0;
	ib.csr &= ~ECC;
	if(ib.op==CWT)
		if((x=tcs())<0){
			return x;
		}
		else {	
			Try(lun(TON));
			ib.op= CAC;
		}
	else {	
		if((x=gts(TON))<0){
			return x;
		}
		if(ib.internal[0]&2){
			ib.ccf= SEOI; 
			ib.csr |= ECC;
		}
		ib.op= TAC;
	}
	ibodda= (int)u.u_base&1;
	u.u_base -= ibodda;
	iboddc= ((ibodda + u.u_count + 1)&~1) - u.u_count;
	u.u_count += iboddc;
	return OK;
}

gpibread(dev)
{
	register int sps;
	int ibstrategy();

	Exclude;
	if((ib.ans=rsetup())>=0)
		physio(ibstrategy, &ibbuf, dev, B_READ, minphys);
	ib.op= 0;
	if(ib.ans<0) u.u_error= EIO;
	Unexclude;
}

static
rsetup()
{
	register int x;

	ib.ccf= 0;
	ib.csr &= ~ECC;
	if((x=gts(LON))<0){
		return x;
	}
	ib.hidden[AUXA] |= (ib.internal[0]&(2<<4)? REOS:0) | (ib.internal[0]&(1<<4)? BIN:0);
	if(ib.internal[0]&(4<<4)) ib.internal[IMR1] &= ~END_IE;
	else {	
		ib.internal[IMR1] |= END_IE;
		if(ib.cstate==STANDBY) ib.hidden[AUXA] |= HLDE;
	}
	ib.internal[AUX]= ib.hidden[AUXA];
	Try(xfer(WTIR, &ib.internal[IMR1], 7, IMR1));
	if(ib.cstate==STANDBY){
		ib.ccf= ib.hidden[AUXA]= ib.hidden[AUXA]&~HLDE|HLDA;
		ib.csr |= ECC;
	}
	ib.op= LAC;
	ibodda= (int)u.u_base&1;
	u.u_base -= ibodda;
	iboddc= ((ibodda + u.u_count + 1)&~1) - u.u_count;
	u.u_count += iboddc;
	return OK;
}

ibstrategy(bp)
	register struct buf *bp;
{
	register int x, rw;

	ib.ioubm = ubmbuf(ib.ubno, bp, USLP);
	ib.iouaddr = ubadbuf(ib.ubno, bp, ib.ioubm);
	rw= bp->b_flags&B_READ? RD:(ib.op==RQC? OUT:WT);
	if((x=xfer(rw, ib.iouaddr+ibodda, bp->b_bcount-iboddc, 0))<0){
		ib.ans= x;
		ibdone(bp);
	}	
}

static
ibdone(bp)
	register struct buf *bp;
{
	register int x;

	iodone(bp);
	ubmfree(ib.ubno, ib.ioubm);
}


static
gts(newadm)
	char newadm;
{
	register int x;

	Try(unhold());
	if(ib.cstate==STANDBY)
		return (ib.internal[ADM]==newadm)? OK : ENOIBDEV;
	if(ib.cstate==IDLE) return ENOTCAC;
	Try(lun(newadm));
	out(csr, (ib.csr &= ~ATN));
	ib.cstate= STANDBY;
	return OK;
}

static
tcs()
{
	if(ib.cstate==INCHARGE) return OK;
	if(ib.cstate==IDLE) return ENOTCAC;
	out(csr, (ib.csr |= ATN));
	ib.cstate= INCHARGE;
	return unhold();
}

static
unhold()
{
	register int x;

	if(ib.hidden[AUXA]&(HLDE|HLDA)){
		ib.hidden[AUXA]= _AUXA;
		Try(ldaux(FH));
		return xfer(WTIR, &ib.hidden[AUXA], 1, AUX);
	}
	return OK;
}

static
xfer(rw,bp,n,fr)
{	/* fr is internal reg addr  */
	register int i, x;
	int ibtimer();

	if(n<=0) return OK;
	if(rw&SEL){
		out(bcr, (-n<<8) | fr & 7);
		bp += ib.uaddr - (int)&ib;
		out(bar, bp);
		out(csr, ib.csr & (REN|SRQ_IE|EOI|ATN|CIC) | (bp>>12) & XBA | rw);
		for(i=ITIMO; !((x=in(csr))&DONE); )
			if(--i<=0) return EITIMO;
		if(x&NEX) return EINEXM;
		if(in(bcr)&0177400) return EIDMACNT;
		out(csr, ib.csr & (REN|SRQ_IE|ATN|CIC|IE));
		return n;
	}
	ib.internal[IMR2]= rw&OUT? DMAO_ENAB:DMAI_ENAB;
	Try(xfer(WTIR,&ib.internal[IMR2],1,IMR2));
	out(bcr, ib.bcr= -n);
	out(bar, ib.bar= bp);
	out(ccf, ib.ccf);
	out(csr, (ib.csr= ib.csr & (REN|SRQ_IE|TCS|ATN|CIC|ECC) | (bp>>12) & XBA | IE | rw));
	ib.ans= 0;
	if(ib.timo) timeout(ibtimer,0,ib.timo*HZ);
	return OK;
}

static
ldaux(a)
{
	ib.internal[AUX]= a;
	return xfer(WTIR, &ib.internal[AUX], 1, AUX);
}

ibtimer(id)
{
	int sps;

	Exclude;
	printf("timer (%o)\n",ibtimer);
	ib.ans= ibstop();
	Unexclude;
}

gpib0int()
{
	register int x, i;
	register short unsigned y, z;

	ib.rdcsr= in(csr);
	ib.rdbcr= in(bcr);
	ib.rdbar= in(bar);
	if((ib.rdcsr&SRQ) && (ib.csr&SRQ_IE)){
		out(csr, (ib.csr &= ~SRQ_IE) & ~GO);
		if(ib.op==WSRQ){
			ib.op= 0;
			ib.ans= OK;
			untimeout(ibtimer,0);
			wakeup(&ibbuf);
		}
		else if(ib.owner) psignal(ib.owner,SIGSRQ);
	}
	if(ib.rdcsr&INT){
		out(csr, ib.csr & ~(DMA_ENAB|GO));
		ib.rdcsr= in(csr);
		ib.rdbcr= in(bcr);
		ib.rdbar= in(bar);
		if((x=xfer(RDIR, &ib.rdinternal[ISR1], 5, ISR1))<0) goto quit;
		if(ib.rdinternal[ISR1]&ERR_IE) ib.ans= ENOIBDEV;
		if(ib.internal[IMR1]&END_IE){
			ib.internal[IMR1] &= ~END_IE;
			if((x=xfer(WTIR,&ib.internal[IMR1],1,IMR1))<0) goto quit;
		}	
	}
	if((ib.rdcsr&DONE) && (ib.csr&GO)){
		ib.csr &= ~GO;
		if(ib.timo) untimeout(ibtimer,0);
		if(ib.rdcsr&NEX) ib.ans= ENEXMEM;
		x = y = ib.rdbcr - ib.bcr;
		z = ib.rdbar - ib.bar;	/* complete batshit */
		if(z != y){
			x= ECNTRS;
			printf("\trdbcr,rdbar,rdcsr= %o %o %o\n",ib.rdbcr,ib.rdbar,ib.rdcsr);
			printf("\tbcr,bar,csr= %o %o %o\n",ib.bcr,ib.bar,ib.csr);
		}
		ibbuf.b_resid= -ib.rdbcr;
		if(ib.ans==0)
quit:			
			ib.ans= x;
		ib.op= 0;
		ibdone(&ibbuf);
	}
}

static
wait100us()
{
	register int i;

	for(i=100; i-->0; ) ;
}

untimeout(fn,arg)
	register int (*fn)();
	caddr_t arg;
{
	register struct callout *p, *q;
	int s;

	s= spl7();
	for(p= &calltodo; q=p->c_next; p=q)
		if(q->c_func==fn && q->c_arg==arg){
			p->c_next= q->c_next;
			q->c_next= callfree;
			callfree= q;
			if(p=p->c_next)
				p->c_time += q->c_time;
			break;
		}
	splx(s);
}
