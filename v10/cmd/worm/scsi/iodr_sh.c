#include	"scsi.h"
#include	"juke.h"

j_shelf_to_drive(int sh, Side side, int dr, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	set6(cmd, 0xD6, dr<<5, 0, (sh<<1)|side, 0, 0);
	return(s_io(0, &cmd, 0, &ret, 0, err));
}

j_drive_to_shelf(int dr, int sh, Side side, char *err)
{
	struct scsi_cmd cmd;
	struct scsi_return ret;

	if(sh < 0)
		set6(cmd, 0xD7, dr<<5, 0, 0, 0, 0);
	else
		set6(cmd, 0xD7, (dr<<5)|1, 0, (sh<<1)|side, 0, 0);
	return(s_io(0, &cmd, 0, &ret, 0, err));
}

int
j_empty_drive(long tlimit, char *buf)
{	
	int i, tstop;
	
	tstop = time((long *)0) + tlimit;
	while(time((long *)0) <= tstop){
		setnlun();		/* in case it changes */
		/* look for empty drives */
		for(i = 0; i < nlun; i++)
			if(!j_status.lun[i].diskin)
				return(i);
		/* look for spun down drives */
		for(i = 0; i < nlun; i++){
			if(!j_status.lun[i].ready){
				if(j_drive_to_shelf(i, -1, SIDEA, buf))
					return(-1);
				else
					return(i);
			}
		}
		sleep(10);
		if(j_getstatus(buf))	/* get the jukebox status */
			return(-1);
	}
	return(-1);
}
