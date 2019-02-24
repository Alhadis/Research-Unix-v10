#define	_POSIX_SOURCE
#include	<stddef.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#include	<unistd.h>
#include	"../scsi.h"
#include	"../jukeface.h"

static
myread(int drive, long block, struct scsi_return *ret, char *err)
{
	struct scsi_cmd cmd;

	cmd.bus_id = s_id;
	set10(cmd, 0x28, drive<<5, block>>24, block>>16, block>>8, block, 0, 0, 1, 0);
	return(s_io(0, &cmd, 0, ret, 1024, err));
}

j_rvolid(int drive, char *err)
{
	struct scsi_return ret;
	long b, lastb;
	char buf[1024];
	int debug = 0;

	err[0] = 0;
	if(j_start(drive, err) < 0)
		return(-1);
	if(myread(drive, 0L, &ret, err) == 0){
		memset(buf, 0, 1024);
		if(memcmp(buf, ret.data, 1024)){
			if(debug)
				fprintf(stderr, "superblok at 0\n");
			goto done;	/* found a superblock at 0 */
		}
	}
	for(b = 1, lastb = -1;;){
hack:
		if(debug)
			fprintf(stderr, "read block %d\n", b);
		if(myread(drive, b, &ret, err))
			break;
		lastb = b;
		b = ((long *)ret.data)[9];
	}
	if(lastb < 0){
		if(b == 1){	/* for disks with a bad block 1 */
			b = 2;
			goto hack;
		}
		if(debug)
			fprintf(stderr, "tried for superblock at blocks 1,2\n");
		sprintf(err, "no superblock");
		j_stop(drive, buf);
		return(1);
	}
	if(myread(drive, lastb, &ret, err) < 0){
		j_stop(drive, buf);
		fprintf(stderr, "read fail on block %d (b=%d)\n", lastb, b);/**/
		return(-1);
	}
	if(debug)
		fprintf(stderr, "superblock at %d\n", lastb);
done:
	strncpy(err, (char *)&ret.data[42], 128);
	err[127] = 0;
	j_stop(drive, buf);
	return(0);
}

static
mywrite(int drive, long block, struct scsi_cmd *cmd, struct scsi_return *ret, char *err)
{
	set10((*cmd), 0x2A, drive<<5, block>>24, block>>16, block>>8, block, 0, 0, 1, 0);
	return(s_io(0, cmd, 1024, ret, 0, err));
}

j_wvolid(int drive, char *vol_id, char *err)
{
	char tmpfile[L_tmpnam];
	char buf[512];
	struct scsi_return ret;
	struct scsi_cmd cmd;
	FILE *fp;
	int n;

	printf("mkfs %s\n", vol_id);
	/* first get the capacity/size for mkfs to make a valid superblock */
	tmpnam(tmpfile);
	if((fp = fopen(tmpfile, "w+r")) == NULL){
		sprintf(err, "%s: %s", tmpfile, strerror(errno));
		return(-1);
	}
	if(j_start(drive, err) < 0)
		return(-1);
	set10(cmd, 0x25, drive<<5, 0, 0, 0, 0, 0, 0, 0, 0);
	if(n = s_io(0, &cmd, 0, &ret, 8, err))
		return(n);
	switch(longat(&ret.data[0]))
	{
	case 1637999:		/* sony 12in clv single density */
		sprintf(buf, "worm mkfs -n %d -f %s %s", 1600000, tmpfile, vol_id);
		break;
	case 3275999:		/* sony 12in clv double density */
		sprintf(buf, "worm mkfs -n %d -f %s %s", 3250000, tmpfile, vol_id);
		break;
	default:
		fprintf(stderr, "warning: bad capacity %d\n", longat(&ret.data[0]));
		sprintf(buf, "worm mkfs -f %s %s", tmpfile, vol_id);
		break;
	}
	if(system(buf)){
		sprintf(err, "%s: error", buf);
		return(-1);
	}
	unlink(tmpfile);
	fseek(fp, 1024L, 0);
	if(fread(cmd.data, 1, 1024, fp) == 0){
		sprintf(err, "mkfs read: %s", strerror(errno));
		return(-1);
	}
	fclose(fp);
	if(mywrite(drive, 1L, &cmd, &ret, err))
		return(-1);
	unlink(tmpfile);
	j_stop(drive, err);
	return(0);
}
