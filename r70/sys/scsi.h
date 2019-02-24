#define	SCSICMD		10	/* max cmd size */
#define	SCSIDATA	(4*1024)
#define	SCSIRET		36	/* includes csr&per */
#define	SCSITOHOST	0x10000

#define	OPEN	1		/* somebody has the device open */
#define	USED	2		/* ever opened */
#define	NEXTWR	4		/* next cmd must be write */
#define	DONE	8		/* io done */
#define	PEND	16		/* we expect an interrupt */
#define	ERRSNSE	32		/* do sense on error */

struct bag
{
	struct { short msg_len; short pad; struct mscmd msg; } cmd;
	struct { short msg_len; short pad; struct msend msg; } rsp;
	struct {
		short ca_cmdint;
		short ca_rspint;
		long ca_rspdsc[1];
		long ca_cmddsc[1];
	} ca;
};
/*
	US design
*/
struct scsipkt
{
	long cmd;
	long data;
	long status;
	long count;
	short bus_id;
};
struct playpen
{
	struct scsipkt upkt;		/* must be first */
	unsigned char cmd[SCSICMD];
	unsigned char data[SCSIDATA];
	unsigned char status[SCSIRET];
};

struct scsi {
	int flag;
	struct udadevice *addr;
	uaddr_t u1, u2;
	ubm_t ub1, ub2;
	struct buf *b1, *b2;
	struct bag *junk;
	unsigned char *data;
	short sa;
	short timeout;
	unsigned long tran_id;
	char status[SCSIRET];
	struct scsireg *reg;
	struct playpen pp;
};
