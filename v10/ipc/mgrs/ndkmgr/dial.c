#include <sys/types.h>
#include <sys/stat.h>
#include <sys/filio.h>
#include "dk.h"
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <ipc.h>
#include <libc.h>
#include "defs.h"
#include <dkwindow.h>
#include <sys/dkio.h>

/* imported */
extern	int	uxp_ld;
extern	int	dkp_ld;
extern 	int	errno;
extern int unit;

enum { dial, announce, redial };


static int
gettraffic(ip)
	ipcinfo *ip;
{
	char *dp;

	/* get type of request and properties */
	for(dp = ip->param; *dp; dp++) {
		for(;*dp && *dp==' '; dp++);
		if (strncmp(dp, "heavy", 5)==0 && (dp[5]=='\0'||dp[5]==' '))
			return 2;
		for(;*dp && *dp!=' '; dp++);
	}
	return 0;
}

int
net_dial(ip)
	ipcinfo *ip;
{
	int fd;
	int ctlfd;
	int traffic;
	int chan;

	traffic = gettraffic(ip);
	fd = dkchan(&traffic, unit, &chan, &ctlfd);
	if (fd < 0)
		return ABORT(EBUSY, "out of output channels", NULLINFO);
	return dkdial(fd, ctlfd, ip, traffic, chan, dial);
}

int
net_announce(ip)
	ipcinfo *ip;
{
	int fd;
	int ctlfd;
	int traffic;
	int chan;

	/*
	 * hack: assertively set unit
	 *   when announcing, use traffic type to decide on interface to use.
	 *   otherwise, we could announce on the wrong interface.
	 *   bug: dkchan tries the other interface, so could still
	 *   end up on the wrong one if the right one is full
	 */
	traffic = gettraffic(ip);
	if (unit == 'b') {
		switch(traffic){
		case 0:
			unit = '0';
			break;
		case 2:
			unit = '2';
			break;
		}
	}
	fd = dkchan(&traffic, unit, &chan, &ctlfd);
	if (fd < 0)
		return ABORT(EBUSY, "out of output channels", NULLINFO);
	return dkdial(fd, ctlfd, ip, traffic, chan, announce);
}

int
net_redial(ip)
	ipcinfo *ip;
{
	int traffic;
	int chan;
	int ctlfd;

	/*
	 *  find unit, traffic, and channel
	 */
	if(dkid(ip->cfd, unit, &traffic, &chan, &ctlfd)<0)
		return -1;
	return dkdial(ip->cfd, ctlfd, ip, traffic, chan, redial);
}

int
dkdial(fd, listener, ip, traffic, chan, operation)
	ipcinfo *ip;
{
	int i;
	struct dialout d;
	char dialstr[128];
	char dialtone[2];
	register char *dp, *cp;
	struct dialout reply;
	int al_catch();
	long window;
	char ws[8];

	/*
	 *  restart URP on the channel
	 */
	signal(SIGALRM, al_catch);
	dkproto(fd, dkp_ld);
	ioctl(fd, FIOPUSHLD, &uxp_ld);

	/* set up the parameter block for the initial request */
	switch(operation){
	case dial:
		d.type = T_SRV;
		d.srv = D_DIAL;
		break;
	case announce:
		d.type = T_SRV;
		d.srv = D_SERV;
		break;
	case redial:
		d.type = T_CHG;
		d.srv = D_REDIAL;
		break;
	}
	d.param0l = chan;
	d.param0h = chan>>8;
	d.param1l = traffic;
	d.param1h = traffic>>8;
	d.param2l = 0;
	d.param2h = 0;
	d.param3l = 0;
	d.param3h = 0;
	d.param4l = ip->uid;
	d.param4h = ip->uid>>8;

	/*
	 *  Make dial string.
	 *  Convert first two '!'s to '.'s to comply with generic's form
	 *  of destination.service[.more-things].  Escape any '.'s in the
	 *  service part with '\'s.
	 */
	for(i=0, cp=ip->name, dp=dialstr; *cp; cp++)
		if (i<2 && *cp=='!'){
			*dp++ = '.';
			i++;
		} else if (i==1 && *cp=='.') {
			*dp++ = '\\';
			*dp++ = '.';
		} else if (*cp=='\n') {
			close(fd);
			return ABORT(EACCES, "illegal dial string", NULLINFO);
		} else
			*dp++ = *cp;
	*dp = '\0';
	switch(operation){
	case dial:
		strcat(dialstr, "\n");
		strcat(dialstr, ip->user);
		break;
	case redial:
		strcat(dialstr, "\n");
		strcat(dialstr, ip->user);
		strcat(dialstr, "\n");
		strcat(dialstr, ip->machine);
		break;
	}

	/*
	 *  send request on common signalling channel
	 */
	alarm(15);
	i = write(listener, (char *)&d, sizeof(d));
	close(listener);
	if (i != sizeof(d)) {
		alarm(0);
		close(fd);
		ip->cfd = -1;
		return ABORT(EIO, "can't dial out on dk", NULLINFO);
	}

	/*
	 *  wait for dial tone (REDIAL only)
	 */
	if(operation==redial && (read(fd, dialtone, 1)!=1 || *dialtone!='O')) {
		alarm(0);
		close(fd);
		ip->cfd = -1;
		return ABORT(EIO, "no dialtone on dk", NULLINFO);
	}

	/*
	 *  dial out
	 */
	alarm(60);
	ioctl(fd, DIOCSTOP, 0);		/* hold back user data */
	write(fd, dialstr, strlen(dialstr));
	if(operation==redial){
		alarm(0);
		return fd;
	}

	/*
	 *  get reply back (REDIAL never gets here)
	 */
	i = read(fd, (char *)&reply, sizeof reply);
	alarm(0);
	if (i <= 0) {
		close(fd);
		return ABORT(EIO,"remote system doesn't respond",
				(ipcinfo *)NULL);
	}

	/* 
	 *  successful reply, analyze it
	 */
	i = (reply.param1h<<8) | reply.param1l;
	if (reply.srv!=D_OPEN) {
		close(fd);
		dktouerr(i&0x7f);
		return -1;
	}
	if (W_VALID(i)){
		/*
		 *  a 1127 window negotiation
		 */
		window = W_VALUE(W_DEST(i));
	} else if(i>2 && i<31){
		/*
		 *  a generic window negotiation
		 */
		window = 1<<i;
	} else
		window = 0;
	logevent("out window(%d)\n", window);
	dkproto(fd, dkp_ld);
	ioctl(fd, FIOPOPLD, 0);		/* dump rdk */
	setwins(fd, window);
	ip->myname = "";
	ip->machine = "";
	return fd;
}

al_catch()
{
}
