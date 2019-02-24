#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	<scsi.h>

#define		DEV		"/dev/scsi"

static fd = -1;
int s_id;
int s_ignua = 1;
void (*ss_extsense)(uchar *, char *, int);

ss_io(int preserve, struct scsi_cmd *cmd, int ncmd, struct scsi_return *ret, int nret, char *err)
{
	int n;
	int retv;

	err[0] = 0;
	retv = -1;
	if(fd < 0){
		if((fd = open(DEV, 2)) < 0){
			pperror(err, DEV);
			return(-1);
		}
	}
	cmd->flags |= (ncmd == 0)? SCSI_RD:SCSI_WR;
	if((n = write(fd, cmd, 16+ncmd)) != 16+ncmd){
		pperror(err, "scsiio write");
err_ret:
		close(fd);
		fd = -1;
		return(retv);
	}
	if(nret >= 0){
		if((n = read(fd, ret, 36+nret)) != 36+nret){
			if(n == 36)
				retv = 1;
			else
				pperror(err, "scsiio read");
			goto err_ret;
		}
	} else {
		if((n = read(fd, ret, 36-nret)) < 0){
			pperror(err, "scsiio read");
			goto err_ret;
		}
		ret->nread = n-36;
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
	struct scsi_cmd mycmd;
	int ignoredua = 0;

	cmd->bus_id = s_id;
again:
	if(n = ss_io(preserve, cmd, ncmd, ret, nret, err)){
		if(n < 0)
			return(n);
		strcpy(ioerr, err);
		err[0] = 0;
	} else
		ioerr[0] = 0;
	if(status = ret->scsi_stat){
		mycmd.bus_id = s_id;
		set6(mycmd, 0x03, cmd->cmd[1]&0xE0, 0, 0, 100, 0);
		if(n = ss_io(0, &mycmd, 0, ret, -100, err))
			return(n);
		if(s_ignua){	/* ignore unit attention ?? */
			if((ret->data[2]&0xF) == 6){	/* it is */
				if(ignoredua++ == 0){	/* but only ignore once */
					mycmd.bus_id = s_id;
					set6(mycmd, 0x12, cmd->cmd[1]&0xE0, 0, 0, 5, 0);
					if(n = ss_io(0, &mycmd, 0, ret, 5, err))
						return(n);
					goto again;
				}
			}
		}
		if(ss_extsense == 0)
			ss_extsense = gen_extsense;
		(*ss_extsense)(ret->data, buf, sizeof buf);
		sprintf(err, "%s; %s", ioerr[0]? ioerr : smsg[(status>>1)&0xF], buf);
		return(1);
	}
	return(0);
}
