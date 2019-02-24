/*
	SCSI Pass-Thru driver for the TD Systems UD? -- Andrew Hume
	Ninth Edition Unix
*/

#include "sys/param.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/systm.h"
#include "sys/pte.h"
#include "sys/map.h"
#include "sys/ubaddr.h"
#include "sys/conf.h"

#include	"sys/uda.h"
#include	"sys/mscp.h"

#include "sys/scsi.h"
#include	<scsi.h>

#define	mscp_scsi	m_fmt

struct udadevice {
	short	udaip;		/* initialization and polling */
	short	udasa;		/* status and address */
};
#define	UDA_ERR		0100000	/* error bit */
#define	UDA_STEP4	0040000	/* step 4 has started */
#define	UDA_STEP3	0020000	/* step 3 has started */
#define	UDA_STEP2	0010000	/* step 2 has started */
#define	UDA_STEP1	0004000	/* step 1 has started */
#define	UDA_GO		0000001	/* start operation, after init */

#define	UDA_OWN		0x80000000	/* UDA owns this descriptor */
#define	UDA_INT		0x40000000	/* allow interrupt on ring transition */

#define WAITPRI 	(PZERO+1)
#define	SCSILTMT	300		/* long timeout in seconds */
#define	SCSISTMT	10		/* short timeout in seconds */
#define	ERROR		0x8000		/* in csr */
#define	SCSIINTR	0700
#define	SETDSC(dsc, fl)	dsc = (dsc&~(UDA_OWN|UDA_INT))|fl

static memset(p, c, n)
	register char *p, c;
	register n;
{
	while(n-- > 0)
		*p++ = c;
}

int scsiopen(), scsiclose(), scsiread(), scsiwrite();

extern struct scsi scsi[];
extern struct ubaddr scsiaddr[];
extern int scsicnt;
struct cdevsw scsicdev =
	cdinit(scsiopen, scsiclose, scsiread, scsiwrite, nodev);
int scsi_dbg = 0;

scsiopen(dev)
	dev_t dev;
{
	register struct scsi *p;

	if((dev = minor(dev)) >= scsicnt) {
		u.u_error = ENODEV;
		return;
	}
	if((p = &scsi[dev])->flag&OPEN) {
		u.u_error = EBUSY;
		return;
	}
	if(((p->addr = (struct udadevice *)ubaddr(&scsiaddr[dev])) == 0)
	  || ubbadaddr(scsiaddr[dev].ubno, (caddr_t)p->addr, sizeof(u_short))) {
		printf("scsi%d absent\n", dev);
		u.u_error = ENODEV;
		return;
	}
	if((p->flag&USED) == 0)
		if(scsistart(dev))
			return;
	p->flag = USED|OPEN|NEXTWR|DONE;
}

scsistart(dev)
	dev_t dev;
{
	register struct scsi *p = &scsi[minor(dev)];

	p->flag |= DONE;
	p->b1 = geteblk();
	p->b1->b_bcount = BUFSIZE;
	p->b1->b_flags = B_BUSY;
	clrbuf(p->b1);
	p->ub1 = ubmbuf(scsiaddr[minor(dev)].ubno, p->b1, USLP);
	p->u1 = ubadbuf(scsiaddr[minor(dev)].ubno, p->b1, p->ub1);
	p->data = (unsigned char *)p->b1->b_un.b_addr;
	p->b2 = geteblk();
	p->b2->b_bcount = BUFSIZE;
	p->b2->b_flags = B_BUSY;
	clrbuf(p->b2);
	p->ub2 = ubmbuf(scsiaddr[minor(dev)].ubno, p->b2, USLP);
	p->u2 = ubadbuf(scsiaddr[minor(dev)].ubno, p->b2, p->ub2);
	p->junk = (struct bag *)p->b2->b_un.b_addr;
	p->timeout = SCSISTMT;
	if(scsiinit(dev) == 0){
		scsiclose(dev);
		p->flag = 0;
		u.u_error = ENXIO;
		return(1);
	}
	printf("scsi%d run\n", minor(dev));
	return(0);
}

scsiclose(dev)
	dev_t dev;
{
	register struct scsi *p = &scsi[minor(dev)];

	if((p->flag&DONE) == 0){	/* wait for I/O to complete */
		(void)tsleep((caddr_t)p, PZERO+1, p->timeout);
	}
	p->flag = USED;
}

scsiwrite(dev)
	dev_t dev;
{
	register count;
	register struct scsi *p = &scsi[minor(dev)];
	register struct mscmd *cmd = &p->junk->cmd.msg;
	unsigned char flag, bus_id;
	long readback;

	if(p->flag&NEXTWR)
		p->flag &= ~NEXTWR;
	else {
		u.u_error = EGREG;
		return;
	}
	if(copyin(u.u_base, &p->tran_id, 4) || copyin(u.u_base+4, &bus_id, 1)
			|| copyin(u.u_base+5, &flag, 1)
			|| copyin(u.u_base+6, &readback, 4)
			|| copyin(u.u_base+10, &cmd->mscp_scsi, SCSICMD)){
		u.u_error = EFAULT;
		return;
	}
	if(flag & SCSI_RESET){
		if(scsiinit(dev) == 0){
			printf("scsi%d: reset failed\n", minor(dev));
			p->flag = 0;
			u.u_error = ENXIO;
			return;
		}
		printf("scsi%d: reset\n", minor(dev));
		p->flag = USED|OPEN|NEXTWR|DONE;
		u.u_count = 0;
		return;
	}
	p->timeout = (flag&SCSI_LTMOUT)? SCSILTMT:SCSISTMT;
	count = u.u_count - (10+SCSICMD);
	u.u_base += 10+SCSICMD;
	if((count < 0) || (count > SCSIDATA)){
		u.u_error = EINVAL;
		return;
	}
	memset(p->data, 0xEE, BUFSIZE);
	if(flag&SCSI_RD){
		cmd->m__r1 = 0106;
		count = readback;
	} else if(flag&SCSI_WR){
		if(copyin(u.u_base, p->data, count)){
			u.u_error = EFAULT;
			return;
		}
		cmd->m__r1 = 0107;	
	} else {
		u.u_error = EINVAL;
		return;
	}
	cmd->m_opcd = (flag & SCSI_BRESET)? 0160 : 0130;
	if(scsi_dbg)
		printf("c=%d uc=%d, dir=0%o, bus_id=%d, new count=%d timeout=%d\n",
			count, u.u_count, cmd->m__r1, bus_id, count, p->timeout);
	cmd->m_unit = bus_id;
	cmd->m_bcnt = count;
	cmd->m_fcnt = p->u1;
	p->flag = (p->flag&~DONE)|PEND;
	if(flag&SCSI_SENSE)
		p->flag |= ERRSNSE;
	p->junk->ca.ca_cmdint = 0;
	p->junk->ca.ca_rspint = 0;
	SETDSC(p->junk->ca.ca_cmddsc[0], UDA_OWN);
	bus_id = p->addr->udaip;	/* start controller */
	u.u_count = 0;
}

scsiread(dev)
	dev_t dev;
{
	register struct scsi *p = &scsi[minor(dev)];
	register count;

	if(p->flag&NEXTWR){
		u.u_error = EGREG;
		return;
	} else
		p->flag |= NEXTWR;
	if((p->flag&DONE) == 0){
		if(tsleep((caddr_t)p, PZERO+1, p->timeout) != TS_OK){
			u.u_error = ENXIO;
			return;
		}
	}
	if(p->sa&0x8000){
		printf("scsi%d: error sa=#%x\n", minor(dev), p->sa&0xFFFF);
		u.u_error = EIO;
		scsiinit(dev);
		return;
	}
	count = u.u_count - SCSIRET;
	if(count > p->junk->rsp.msg.m_bcnt)
		count = p->junk->rsp.msg.m_bcnt;
	if((count < 0) || (count > SCSIDATA)){
		u.u_error = EINVAL;
		return;
	}
	((unsigned long *)p->status)[0] = p->tran_id;
	p->status[4] = p->junk->rsp.msg.m_fbbk;
	p->status[5] = p->junk->rsp.msg.m_fbbk>>8;
	p->status[6] = 0;	/* flags */
	p->status[7] = 1;	/* viking */
	((short *)p->status)[4] = p->sa;
	((short *)p->status)[5] = p->junk->rsp.msg.m_sts;
	if(copyout(p->status, u.u_base, SCSIRET)){
		u.u_error = EFAULT;
		return;
	}
	if(count)
		if(copyout(p->data, u.u_base+SCSIRET, count)){
			u.u_error = EFAULT;
			return;
		}
	if(p->junk->rsp.msg.m_sts){
		printf("scsi%d: stat=#%x\n", minor(dev), p->junk->rsp.msg.m_sts);
		u.u_error = (p->junk->rsp.msg.m_sts == 0x80a)? ESRCH : EIO;
		return;
	}
	u.u_count -= SCSIRET+count;
	SETDSC(p->junk->ca.ca_rspdsc[0], UDA_OWN|UDA_INT);
}

scsi0int(dev)
	dev_t dev;
{
	register struct scsi *p = &scsi[minor(dev)];
	register s;

	if((p->flag&PEND) == 0){
		printf("scsi%d: unexpected interrupt\n", minor(dev));
		return;
	}
	if(p->junk->ca.ca_rspint == 0){
		printf("scsi%d: rspint=0 cmdint=%d rsp=#%x cmd=#%x\n", minor(dev), p->junk->ca.ca_cmdint, p->junk->ca.ca_rspdsc[0], p->junk->ca.ca_cmddsc[0]);
		return;
	}
	p->sa = p->addr->udasa;
	s = spl6();
	p->flag = (p->flag&~PEND)|DONE;
	splx(s);
	wakeup((caddr_t)p);
}

/*
 * hardware initialization handshake
 * for simplicity, don't bother with init interrupts
 * returns nonzero if ok
 */

#define	UDA_STEPS	(UDA_ERR|UDA_STEP4|UDA_STEP3|UDA_STEP2|UDA_STEP1)

scsiinit(d)
int d;
{
	register struct scsi *p = &scsi[minor(d)];
	register struct udadevice *udaddr;
	register int i;
	time_t out;
	int s;

	udaddr = p->addr;
	out = time + 11;
	s = spl0();
printf("scsi1");
	udaddr->udaip = 0;
	while((udaddr->udasa & (UDA_ERR|UDA_STEP1)) == 0 && time <= out)
		;
	if((udaddr->udasa & UDA_STEPS) != UDA_STEP1) {
		steperr("1", udaddr->udasa, d);
		splx(s);
		return (0);
	}
	udaddr->udasa = UDA_ERR|(SCSIINTR/4);
	/* no diagnostic wrap, no interrupts during initialization */
printf("2");
	out = time + 21;
	while((udaddr->udasa & (UDA_ERR|UDA_STEP2)) == 0 && time <= out)
		;
	if((udaddr->udasa & UDA_STEPS) != UDA_STEP2) {
		steperr("2", udaddr->udasa, d);
		splx(s);
		return (0);
	}
	i = p->u2;	/* unibus address of bag */
	i += (long)&p->junk->ca.ca_rspdsc[0] - (long)p->junk;
	udaddr->udasa = (short)i;
	out = time + 11;
printf("3");
	while((udaddr->udasa & (UDA_ERR|UDA_STEP3)) == 0 && time <= out)
		;
	if((udaddr->udasa & UDA_STEPS) != UDA_STEP3) {
		steperr("3", udaddr->udasa, d);
		splx(s);
		return (0);
	}
	udaddr->udasa = (i >> 16)&0x3F;
	out = time + 11;
printf("4");
	while((udaddr->udasa & (UDA_ERR|UDA_STEP4)) == 0 && time <= out)
		;
	if((udaddr->udasa & UDA_STEPS) != UDA_STEP4) {
		steperr("4", udaddr->udasa, d);
		splx(s);
		return (0);
	}
	i = p->u2 + (long)&p->junk->rsp.msg.m_crf - (long)p->junk;
	p->junk->ca.ca_rspdsc[0] = UDA_OWN|UDA_INT|i;
	i = p->u2 + (long)&p->junk->cmd.msg.m_crf - (long)p->junk;
	p->junk->ca.ca_cmddsc[0] = i;
	if (udaddr->udasa & UDA_ERR) {
		steperr("5", udaddr->udasa, d);
		splx(s);
		return (0);
	}
	udaddr->udasa = UDA_GO;		/* finish init */
	/* finish cmd packet init */
	p->junk->cmd.msg_len = 44;
	p->junk->cmd.msg.m_crf = (long)p->junk;
	p->junk->cmd.msg.m_opcd = 0130;
	splx(s);
printf(" done\n");
	return (1);
}

steperr(str, sa, dev)
	char *str;
{
	printf("scsi%d: step%s error, sa=#%x\n", minor(dev), str, sa&0xFFFF);
}

/*
	dreck to figure out how much we should read back
*/

scsilen(cmd)
	unsigned char *cmd;
{
	switch(cmd[0])
	{
	case 0x03:	return(cmd[4]);
	case 0x08:	return(cmd[4]*1024);
	case 0x12:	return(cmd[4]);
	case 0x1A:	return(cmd[4]);
	case 0x1C:	return(cmd[3]*256 + cmd[4]);
	case 0x25:	return(8);
	case 0x28:	return(1024*(256*cmd[7] + cmd[8]));
	case 0x2C:	return(6);
	case 0x2D:	return(6);
	case 0x4D:	return(1*(256*cmd[7] + cmd[8]));
	case 0xC2:	return(1024);
	case 0xC3:	return(4096);
	case 0xD3:	return(20);
	}
	return(0);
}
