typedef unsigned char uchar;

struct scsi_cmd
{
	unsigned long id;
	uchar bus_id;		/* SCSI id of destination device */
	uchar flags;
	uchar cmd[10];		/* SCSI command */
	uchar data[4096];	/* optional data */
};

struct scsi_return
{
	unsigned long id;
	uchar scsi_stat;	/* scsi status byte */
	uchar scsi_msg;		/* scsi message byte */
	uchar flags;
	uchar type;		/* 1=td 2=us */
	unsigned short reg1;	/* td=sa, us=per */
	unsigned short reg2;	/* td=mscp, us=per */
	unsigned char sense[22];
	char pad[2];
	uchar data[4096];	/* any data */
	uchar nread;		/* chars read(-8) if ret count was -ve */
};

#define	set6(x,a,b,c,d,e,f)	(x).flags=0,(x).cmd[0]=(a),(x).cmd[1]=(b),(x).cmd[2]=(c),\
	(x).cmd[3]=(d),(x).cmd[4]=(e),(x).cmd[5]=(f)
#define	set10(x,a,b,c,d,e,f,g,h,i,j)	(x).flags=0,(x).cmd[0]=(a),(x).cmd[1]=(b),(x).cmd[2]=(c),\
	(x).cmd[3]=(d),(x).cmd[4]=(e),(x).cmd[5]=(f),(x).cmd[6]=(g),(x).cmd[7]=(h),\
	(x).cmd[8]=(i),(x).cmd[9]=(j)
#define	setdiag(x,lun,n)	(x).flags=0,(x).cmd[0]=0x1C,(x).cmd[1]=(lun)<<5,(x).cmd[2] = 0,\
	(x).cmd[3]=(n)>>8,(x).cmd[4]=(n),(x).cmd[5]=0

extern s_io(int, struct scsi_cmd *, int, struct scsi_return *, int, char *);/* return 0 on no error, does sense on error */
extern ss_io(int, struct scsi_cmd *, int, struct scsi_return *, int, char *);/* return 0 on no error */
extern int s_ignua;	/* should s_io ignore unit attentions? */
extern void (*ss_extsense)(uchar *, char *, int);
extern int s_start(int, char *);
extern int s_stop(int, char *);
extern int s_eject(int, char *);
extern int s_id;
extern unsigned long longat(uchar *);
