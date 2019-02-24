#include <sys/ioctl.h>
#include <stdio.h>

static	char *names[] = {
	"/dev/dk/dk0%c%d",
	"/dev/dk/dk2%c%d",
	"/dev/dk/dkctl0",
	"/dev/dk/dkctl2",
	"/dev/dk/dk%c%d",
	"/dev/dk/dkctl",
	0
};

static	char *namefmt;
static	char alph[] = "0123456789abcdefghijklmnopqrstuvwxyz";

/*
 * open channel 1 to controller
 *   leave record of file name
 */
dkctlchan(traffic)
register traffic;
{
	register i ;
	extern int rdk_ld, dkp_ld ;
	char dk01file[32];

	namefmt = traffic? names[1]:names[0];
	sprintf(dk01file, namefmt, alph[0], 1);
	i = open(dk01file, 2) ;
	if (i < 0) {
		sprintf(dk01file, traffic?names[3]:names[2]) ;
		i = open(dk01file, 2) ;
		if (i < 0) {
			namefmt = names[4];
			sprintf(dk01file, namefmt, alph[0], 1);
			i = open(dk01file, 2);
			if (i < 0) {
				sprintf(dk01file, names[5]) ;
				i = open(dk01file, 2);
				if (i < 0)
					return (i) ;
			}
		}
	}
	ioctl(i, DIOCNXCL, 0);
	if (ioctl(i, FIOLOOKLD, 0) != rdk_ld) {
		if (dkproto(i, dkp_ld) < 0) {
			fprintf(stderr, "can't start dkp on %s\n", dk01file) ;
			close(i) ;
			return (-1) ;
		}
		if (ioctl(i, FIOPUSHLD, &rdk_ld) < 0) {
			fprintf(stderr, "can't push rdk_ld on %s\n", dk01file) ;
			close(i) ;
			return (-1) ;
		}
		if (ioctl(i, DIOCLHN, (char *)0) < 0) {
			fprintf(stderr, "can't be manager on %s\n", dk01file) ;
			close(i);
			return(-1);
		}
	}
	return (i) ;
}

/*
 * find the file name corresponding to a given channel.
 * assumes dkctlchan has been called.
 * Used for incoming calls.
 */

char *
dkfilename(chan)
{
	static char name[64];

	if (namefmt==NULL)
		return(NULL);
	sprintf(name, namefmt, alph[chan/10], chan%10);
	return(name);
}
