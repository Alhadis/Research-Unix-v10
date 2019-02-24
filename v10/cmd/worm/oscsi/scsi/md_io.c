#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	<sys/types.h>
#include	<sys/dsreq.h>

#define	DEV(buf, target, lun)	sprintf(buf, "/dev/scsi/sc0d%dl%d", target, lun)

static fd = -1;
int s_id;
void (*ss_extsense)(uchar *, char *, int);

ss_io(int preserve, struct scsi_cmd *cmd, int ncmd, struct scsi_return *ret, int nret, char *err)
{
	int retv;
	dsreq_t ds;
	char dev[512];

	err[0] = 0;
	retv = -1;
	if(ncmd && nret){
		sprintf(err, "both input (%d bytes) and output (%d bytes) expected", ncmd, nret);
		return(retv);
	}
	if(cmd->bus_id & 0x8000){
		sprintf(err, "reset not supported");
		return(retv);
	}
	if(fd < 0){
		DEV(dev, cmd->bus_id, ((cmd->cmd[1]>>5)&7));
		if((fd = open(dev, 2)) < 0){
			pperror(err, dev);
			return(-1);
		}
	}
	ds.ds_flags = DSRQ_SENSE;
	ds.ds_time = 30000;
	ds.ds_cmdbuf = (char *)cmd->cmd;
	ds.ds_cmdlen = 10;
	if(ncmd){
		ds.ds_databuf = (char *)cmd->data;
		ds.ds_datalen = ncmd;
		ds.ds_flags |= DSRQ_WRITE;
	} else {
		ds.ds_databuf = (char *)ret->data;
		ds.ds_datalen = nret;
		ds.ds_flags |= DSRQ_READ;
	}
	ds.ds_sensebuf = (char *)ret->sense;
	ds.ds_senselen = sizeof ret->sense;
	ds.ds_iovbuf = 0;
	ds.ds_link = 0;
	if(ioctl(fd, DS_ENTER, &ds) < 0){
		pperror(err, "DS_ENTER ioctl");
err_ret:
		close(fd);
		fd = -1;
		return(retv);
	}
	if(ds.ds_ret
		&& (ds.ds_ret != DSRT_SHORT)
		&& (ds.ds_ret != DSRT_OK)
	)	/* an error */
		fprintf(stderr, "ds_ret = #%x\n", ds.ds_ret);
	ret->type = 3;
	ret->scsi_stat = ds.ds_status;
	ret->scsi_msg = ds.ds_msg;
	ret->reg1 = ret->reg2 = 0;
	if(nret >= 0){
		if(ds.ds_datasent != nret){
			if(ds.ds_datasent == 0)
				retv = 1;
			else
				sprintf(err, "data transfer error; wanted %d, got %d", nret, ds.ds_datasent);
			goto err_ret;
		}
	} else {
		ret->nread = ds.ds_datasent;
	}
	if(!preserve){
		close(fd);
		fd = -1;
	}
	return(0);
}

static char *smsg[16] =
{
	"good", "check condition", "met/good", "reserved",
	"busy", "reserved", "reserved", "reserved",
	"intermediate good", "reserved", "intermediate good/met", "reserved",
	"reservation conflict", "reserved", "reserved", "reserved",
};

s_io(int preserve, struct scsi_cmd *cmd, int ncmd, struct scsi_return *ret, int nret, char *err)
{
	int n;
	int status;
	char buf[512];
	char ioerr[512];

	cmd->bus_id = s_id;
	if(n = ss_io(preserve, cmd, ncmd, ret, nret, err)){
		if(n < 0)
			return(n);
		strcpy(ioerr, err);
		err[0] = 0;
	} else
		ioerr[0] = 0;
	if(status = ret->scsi_stat){
		(*ss_extsense)(ret->data, buf, sizeof buf);
		sprintf(err, "%s; %s", ioerr[0]? ioerr : smsg[(status>>1)&0xF], buf);
		return(1);
	}
	return(0);
}
