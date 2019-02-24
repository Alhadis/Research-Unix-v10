#include <sys/types.h>
#include <sys/stat.h>
#include <sys/filio.h>
#include <stdio.h>
#include <errno.h>
#include <sys/dkio.h>

extern int errno;
extern int uxp_ld;
extern int dkp_ld;
extern char *netfiles;	/* /dev/dk/dk by default */

static char *cnames[] = {
	"%sctl0",
	"%sctl",
	"%sctl2",
	0
};
static char *names[] = {
	"%s0%c%c",
	"%s%c%c",
	"%s2%c%c",
	0
};

static	char alph[] = "0123456789abcdefghijklmnopqrstuvwxyz";

static int high[] = {256, 256};
static int low[] = {1, 1};

/*
 *	figure out which set of dk channels to use
 */
static
dkindex(traffic, unit)
	int traffic;
	int unit;
{
	switch(unit) {
	case '0':
		return 0;
	case '2':
		return 2;
	case 'b':
		return traffic;
	default:
		return 1;
	}
}

/*
 *	Open a free dk channel and remember the `type' in namefmt.
 *	If one device doesn't work, try another.
 */
static int
dkchan2(traffic, unit, chanp, ctlfdp)
	int traffic;
	int unit;
	int *chanp;
	int *ctlfdp;
{
	int missing=0;
	char outname[64];
	register i,j;
	int k;
	int fd;
	int ai;
	int nfd;

	/*
	 *  get the control channel and ask for a suggested channel
	 */
	*ctlfdp = dkctlchan(traffic, unit);
	if(*ctlfdp<0)
		return -1;
	k = 0;
	if(ioctl(*ctlfdp, DIOCCHAN, &k)>0 && k>0)
		low[0] = high[1] = k;

	/*
	 *  starting with the suggested channel, loop through all
	 *  channels looking for one not in use
	 */
	ai=dkindex(traffic, unit);
	for (j=0; j<2; j++) {
		for (i=low[j]; i<high[j]; i++) {
			sprintf(outname, names[ai], netfiles, alph[i/10], alph[i%10]);
			if ((fd = open(outname, 2)) >= 0){
				if ((nfd = open(outname, 2)) >= 0) {
					close(nfd);
					close(fd);
					continue;	/* already in use */
				}
				flabclr(fd);	/* for secure unix */
				break;
			}
			if (errno==ENOENT) 
				if (++missing>5)
					break;
		}
		if (fd>=0)
			break;
	}
	if (fd < 0)
		return -1;
	low[0] = high[1] = i+1;
	*chanp = i;
	return fd;
}
int
dkchan(trafp, unit, chanp, ctlfdp)
	int *trafp;
	int unit;
	int *chanp;
	int *ctlfdp;
{
	int fd;

	fd = dkchan2(*trafp, unit, chanp, ctlfdp);
	if(fd >= 0)
		return fd;
	if (*trafp == 0)
		*trafp = 2;
	else
		*trafp = 0;
	return dkchan2(*trafp, unit, chanp, ctlfdp);
}

/*
 * open common control channel
 */
dkctlchan(traffic, unit)
{
	register i ;
	char dkctlfile[32];
	int ai;

	ai=dkindex(traffic, unit);
	sprintf(dkctlfile, cnames[ai], netfiles);
	i = open(dkctlfile, 2);
	if (i < 0)
		return -1;
	ioctl(i, DIOCNXCL, 0);
	if (ioctl(i, FIOLOOKLD, 0) != uxp_ld) {
		if (dkproto(i, dkp_ld) < 0) {
			fprintf(stderr, "can't push dkp_ld on %s\n", dkctlfile) ;
			close(i) ;
			return -1;
		}
		if (ioctl(i, FIOPUSHLD, &uxp_ld) < 0) {
			fprintf(stderr, "can't push unixp_ld on %s\n", dkctlfile) ;
			close(i) ;
			return -1;
		}
		if (ioctl(i, DIOCLHN, (char *)0) < 0) {
			fprintf(stderr, "can't be manager on %s\n", dkctlfile) ;
			close(i);
			return -1;
		}
	}
	return i;
}

/*
 * find the file name corresponding to a given channel.
 * assumes dkctlchan has been called.
 * Used for incoming calls.
 */
char *
dkfilename(traffic, unit, chan)
{
	static char name[64];

	sprintf(name, names[dkindex(traffic, unit)], netfiles, alph[chan/10], alph[chan%10]);
	return(name);
}

/*
 * Make sure that URP protocol is enabled on a datakit file.
 */
int
dkproto(file, linedis)
{
	if (ioctl(file, KIOCISURP, (char *)0) < 0)
		return(ioctl(file, FIOPUSHLD, &linedis));
	ioctl(file, KIOCINIT, (char *)0);
	return(0);
}

/*
 * find the traffic type and channel number corresponding to an fd.
 * Used for redial.
 */
int
dkid(fd, unit, trafficp, chanp, ctlfdp)
	int fd;
	int unit;
	int *trafficp;
	int *chanp;
	int *ctlfdp;
{
	struct stat s;
	struct stat s2;

	/*
	 *  find major minor
	 */
	if(fstat(fd, &s)<0)
		return -1;

	/*
	 *  compare major/minor of fd against that of device 3
	 */
	if(stat(dkfilename(0, unit, 3), &s2)>=0){
		if(major(s.st_rdev)==major(s2.st_rdev)){
			if(minor(s.st_rdev)>minor(s2.st_rdev)-3){
				*trafficp = 0;
				*chanp = minor(s.st_rdev)-minor(s2.st_rdev)+3;
				*ctlfdp = dkctlchan(*trafficp, unit);
				if(*ctlfdp < 0)
					return -1;
				return 0;
			}
		}
	}
	if(stat(dkfilename(2, unit, 3), &s2)>=0){
		if(major(s.st_rdev)==major(s2.st_rdev)){
			if(minor(s.st_rdev)>minor(s2.st_rdev)-3){
				*trafficp = 2;
				*chanp = minor(s.st_rdev)-minor(s2.st_rdev)+3;
				*ctlfdp = dkctlchan(*trafficp, unit);
				if(*ctlfdp < 0)
					return -1;
				return 0;
			}
		}
	}
	return -1;
}
