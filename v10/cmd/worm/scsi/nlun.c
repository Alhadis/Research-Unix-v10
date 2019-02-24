#include	<stdio.h>
#include	"scsi.h"
#include	"juke.h"

int nlun = 1;

void
setnlun(void)
{
	char buf[512];

	for(nlun = 0; nlun < NLUN; nlun++){
		sprintf(buf, "/dev/worm%d", nlun);
		if(access(buf, 0) < 0)
			return;
	}
}
