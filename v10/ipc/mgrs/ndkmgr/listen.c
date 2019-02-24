#include <sys/filio.h>
#include <sys/dkio.h>
#include <stdio.h>
#include <errno.h>
#include <libc.h>
#include <ipc.h>
#include "dk.h"
#include "dkwindow.h"
#include "defs.h"

/* imported */
extern int dkp_ld;
extern int unit;
extern char *dkfilename();

/* global to this module */
static int chan;
static long tstamp[256];
static int traffic;
static int listenfd;
static int newfd;		/* used to keep channel open until accept/reject */

void net_reject();

ipcinfo *
net_listen(fd)
	int fd;
{
	static char buf[256];
	static ipcinfo info;
	static char param[128];
	static stretch newipcname;
	char *fp, *tp, *physaddr;
	char *line[12];
	char *field[12];
	long ts, window;
	int n;

restart:
	physaddr="";
	listenfd = fd;

	/* get the request */
	if ((n = read(fd, buf, sizeof(buf)-1)) <= 0) {
		ABORT(errno, "bad read in listen", NULLINFO);
		return NULL;
	}
	buf[n] = '\0';

	/* initialize the info struct */
	info.rfd = info.cfd = -1;
	info.user = info.machine = NULL;
	info.uid = info.gid = -1;
	info.param = NULL;
	info.flags = IPC_OPEN;

	/* break into lines */
	setfields("\n");
	n = getfields(buf, line, 12);
	if (n < 2) {
		ABORT(EIO, "bad message from dk (1 line)", NULLINFO);
		return NULL;
	}

	/* line 0 is `chan.tstamp.traffic[.urpparms.window]' */
	setfields(".");
	window = 0;
	switch(getfields(line[0], field, 5)){
	case 5:
		/*
		 *  generic way of passing window
		 */
		traffic = atoi(field[4]);
		if(traffic > 0 && traffic <31)
			window = 1<<traffic;
		else
			window = 0;
		/*
		 *  intentional fall through
		 */
	case 3:
		chan = atoi(field[0]);
		ts = atoi(field[1]);
		if(ts == tstamp[chan]) {
			logevent("duplicate timestamp %ld ignored, ch %d\n", ts, chan);
			goto restart;
		}
		/*
		 *  1127 way of passing window
		 */
		traffic = atoi(field[2]);
		if(window == 0 && W_VALID(traffic))
			window = W_VALUE(W_ORIG(traffic));
		info.param = W_TRAF(traffic)==0 ? "light" : "heavy";
		break;
	default:
		ABORT(ENXIO, "bad message from dk(bad first line)", NULLINFO);
		return NULL;
		break;
	}

	/*
	 *  Line 1 is `my-dk-name.service[.more-things]'.
	 *  Special characters are escaped by '\'s.
	 */
	info.myname = line[1];
	info.name = "";
	for(tp=line[1]; tp && *tp; tp++) {
		if (*tp == '.') {
			if (*info.name) {
				*tp = '!';
				break;
			} else {
				*tp = '\0';
				info.name = tp+1;
			}
		} else if (*tp == '\\')
			strcpy(tp, tp+1);
	}

	/* the rest is variable length */
	switch(n) {
	case 2:
		/* no more lines */
		info.machine = NULL;
		info.user = NULL;
		info.param = NULL;
		break;
	case 3:
		/* line 2 is `source.user.param1.param2' */
		getfields(line[2], field, 3);
		info.machine = field[0];
		info.user = field[1];
		info.param = field[2];
		break;
	case 4:
		/* line 2 is `user.param1.param2' */
		getfields(line[2], field, 2);
		info.user = field[0];
		info.param = field[1];

		/* line 3 is `source.node.mod.chan' */
		info.machine = line[3];
		tp=strchr(line[3], '.');
		if(tp) {
			*tp='\0';
			physaddr=tp+1;
		}
		break;
	default:
		ABORT(ENXIO, "bad message from dk(>4 line)", NULLINFO);
		return NULL;
	}
	logevent("src(%s.%s)ch(%d)tr(%d)wo(%d)wd(%d)ts(%ld)w(%d)\n",
		info.machine, physaddr, chan, W_TRAF(traffic),
		W_VALID(traffic)?W_VALUE(W_ORIG(traffic)):0,
		W_VALID(traffic)?W_VALUE(W_DEST(traffic)):0, ts, window);

	/* open the channel */
	fp = dkfilename(traffic, unit, chan);
	tstamp[chan] = ts;
	if ((info.rfd = open(fp, 2))<0) {
		char reply[64];

		sprintf(reply, "%u.%u.%u", chan, tstamp[chan], utodkerr(errno));
		write(listenfd, reply, strlen(reply));
		logevent("can't open %s, errno %d\n", fp, errno);
		goto restart;
	}
	flabclr(info.rfd);
	set2stsrc(info.rfd, "dk!", info.machine);
	if (dkproto(info.rfd, dkp_ld) < 0) {
		close(info.rfd);
		ABORT(EBUSY, "can't push line discipline",NULLINFO);
		return NULL;
	}
	setwins(info.rfd, window);
	if (dkproto(info.rfd, dkp_ld) < 0) {
		logevent("can't push line dkp_ld errno = %d\n", errno);
		close(info.rfd);
		ABORT(EIO, "can't push line discipline",NULLINFO);
		net_reject(&info, errno, errstr);
		goto restart;
	}
	newfd = dup(info.rfd);
	fchown(info.rfd, 0, 0);
	fchmod(info.rfd, 0600);
	ioctl(info.rfd, DIOCNXCL, 0);

	if (info.user==NULL || info.user[0]=='\0')
		info.user = "_unknown_";
	if (info.machine==NULL || info.machine[0]=='\0')
		info.machine = "_unknown_";
	if (physaddr[0]!='\0'){
		strcpy(param, info.param ? info.param: "");
		strcat(param, " PSOURCE=");
		strcat(param, physaddr);
		info.param=param;
	}

	/* set the network name */
	_strcat(&newipcname, info.myname, "!", info.name);
	ipcname = newipcname.ptr;

	return (&info);
}

setwins(f, window)
	long window;
{
	char ws[5];

	if (window>0) {
		/* last possible sanity check */
		if(window<64)
			window = 64;

		/* try 3 X 1/4 */
		window >>= 2;
		ws[0] = window;
		ws[1] = window>>8;
		ws[2] = window>>16;
		ws[3] = window>>24;
		ws[4] = 3;
		ioctl(f, DIOCXWIN, ws);
	}
}

void
net_accept(ip)
	ipcinfo *ip;
{
	char reply[64];

	USE(ip);
	sprintf(reply, "%u.%u.%u", chan, tstamp[chan], 0);
	write(listenfd, reply, strlen(reply));
	close(newfd);
}

void
net_reject(ip, error, msg)
	ipcinfo *ip;
	int error;
	char *msg;
{
	char reply[64];

	sprintf(reply, "%u.%u.%u", chan, tstamp[chan], utodkerr(error));
	write(listenfd, reply, strlen(reply));
	close(ip->rfd);
	ip->rfd = -1;
	close(newfd);
	errno = error;
	errstr = msg;
}
