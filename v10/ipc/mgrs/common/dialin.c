#include <sys/param.h>
#include <stdio.h>
#include <sys/filio.h>
#include <signal.h>
#include <ipc.h>
#include <libc.h>
#include <setjmp.h>
#include "defs.h"

jmp_buf xyz;

/* preeclared */
void dodialin();

/* global */
int pid;
int debug;

/* imported */
extern char *av0;
extern int net_announce();
extern void net_accept();
extern void net_reject();
extern ipcinfo *net_listen();

/* take alarm */
static int
ding() {
	logevent("time out\n");
	longjmp(xyz, 1);
}

/* loop on calls into the CPU */
void
dodialin(mtpt, netname, param)
	char *mtpt;
	char *netname;
	char *param;
{
	int fd;
	ipcinfo info;

	signal(SIGPIPE, SIG_IGN);
	pid = getpid();

	/*
	 *  plug into network name space
	 */
	for(;;){
		info.name = netname;
		info.user = getlogin();
		info.param = param;
		info.rfd = info.cfd = -1;

		/*
		 *  announce yourrself.  alarm is for avoiding
		 *  the single thread deadlock
		 */
		signal(SIGALRM, ding);
		alarm(30);
		if(setjmp(xyz)==0)
			fd = net_announce(&info);
		else
			fd = -1;
		alarm(0);
		signal(SIGALRM, SIG_IGN);
		if (fd>=0)
			break;

		/*
		 *  it didn't work, complain and try again
		 */
		logconsole("%s: can't announce to network as %s (%s)\n",
			av0, netname, errstr);
		sleep(60);
	}
	if (*info.machine)
		netname = info.machine;
	logconsole("%s: announced to network as %s\n", av0, netname);

	/* loop on requests */
	for(;;) {
		ipcinfo *ip;
		int rv;
		static stretch newsrc;
		static stretch newmyname;

		/*
		 *  get a request from the network
		 */
		ip = net_listen(fd);
		if (ip == NULL) {
			logevent("broken listen (%s)\n", errstr);
			break;
		}
		if(debug)
			logevent("dialin: rcved\n");

		/*
		 *  be careful not to write on an unmounted file
		 */
		ip->flags |= IPC_CAREFUL;

		/*
		 *  default service is login
		 */
		if (ip->name==NULL || *ip->name=='\0')
			ip->name = "login";

		/*
		 *  tack network name onto requestor, requestee, and
		 *  name of this service.
		 */
		_strcat(&newsrc, mtpt, "!", ip->machine);
		ip->machine = newsrc.ptr;
		_strcat(&newmyname, mtpt, "!", ip->myname);
		ip->myname = newmyname.ptr;

		/*
		 *  pass request to internal service provider.  once
		 *  again avoid internal single thread deadlock.
		 */
		if (ip->flags & IPC_CREAT) {
			net_reject(ip, EINVAL, "bad request");
			logstatus("in", ip);
			continue;
		}
		if(setjmp(xyz)==0){
			signal(SIGALRM, ding);
			alarm(30);
			rv = ipcdial(ip);
			alarm(0);
		} else {
			/* here after timeout */
			rv = -1;
		}
		signal(SIGALRM, SIG_IGN);
		if(debug)
			logevent("dialin: passed\n");
		if (rv<0) {
			net_reject(ip, errno, errstr);
			logstatus("in", ip);
			continue;
		} else
			close(rv);

		/*
		 *  tell requestor that the connection was accepted
		 */
		errno = 0; errstr = "";
		logstatus("in", ip);
		net_accept(ip);
	}
	close(fd);
}
