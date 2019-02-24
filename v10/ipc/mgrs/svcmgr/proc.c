/*
 * stuff concerned with the table
 * of processes we've spawned
 * almost everything in this file should go away;
 * it's needed only to keep /etc/utmp up to date
 */

#include "mgr.h"
#include <sys/types.h>
#include <utmp.h>
#include <libc.h>
#include <signal.h>
#include <wait.h>

typedef struct {
	int pid;
	char *tty;
	char *service;
} Proc;

#define	MAXPROC	1024
Proc proc[MAXPROC];
Proc *maxproc;
static int debug=1;

/*
 * make ourselves into a new process
 * returns 0 if we're the parent, or we can't
 * returns 1 in the child
 */
newproc(rp)
	Request *rp;
{
	int pid;
	register Proc *p;
	char *ttyname(), *tty;

	/*
	 *  clean up any utmp entries for incalling tty
	 */
	checkkids();
	tty = ttyname(rp->i->cfd>=0 ? rp->i->cfd : rp->i->rfd);
	if(tty)
		for (p = proc; p < &proc[MAXPROC]; p++)
			if(p->pid && strcmp(tty, p->tty)==0)
				deadproc(p->pid, 0);

	/*
	 *  create a new process
	 */
	switch(pid=fork()){
	case -1:
		logevent("can't fork; gave up\n");
		_ipcabort(0, "", rp->i);
		return(0);
	case 0:
		closesvcs();
		return (1);
	default:
		break;
	}

	_ipcabort(0, "", rp->i);
	if (tty==NULL){
		if(debug)
			logevent("newproc(%d, non-tty)\n", pid);
		return(0);
	}

	/*
	 *  remember proc so we can take it out of utmp when
	 *  it dies.
	 */
	for (p = proc; p < &proc[MAXPROC]; p++)
		if(p->pid == 0) {
			if(debug)
				logevent("newproc(%d, %s)\n", pid, tty);
			if ((p->tty = strdup(tty)) != NULL){
				p->pid = pid;
				p->service = strdup(rp->s->name);
			}
			return (0);
		}

	/* too bad */
	logevent("no internal proc; didn't bother\n");
	return (0);
}

/*
 *  clean up after any dead children
 */
checkkids()
{
	int pid;
	int status;

	for(;;){
		pid = wait3(&status, WNOHANG, 0);
		if(pid<=0)
			break;
		deadproc(pid, status);
	}
}

deadproc(pid, status)
	int pid;
	int status;
{
	register Proc *p;
	int e, t;

	if(debug)
		logevent("deadproc(%d, %x)\n", pid, status);

	if(pid <= 0)
		logevent("pid == %d: something is wrong\n", pid);

	/*
	 *  see if we have a local record of this process
	 */
	for (p = proc; p < &proc[MAXPROC]; p++)
		if (p->pid == pid)
			break;
	if(p==&proc[MAXPROC]) {
		logevent("non-tty proc %d died\n", pid);
		return;
	}

	/*
	 *  remove any knowledge of this process
	 */
	rmutmp(p->tty);
	p->pid = 0;
	free(p->tty);
	p->tty = 0;

	e = (status>>8)&0177;
	t = status&0377;
	if(p->service) {
		if(status){
			if(t&0200)
				logevent("%s exit(%d) signal(%d) core dumped\n",
					 p->service, e, t&~0200);
			else
				logevent("%s exit(%d) signal(%d)\n", p->service, e, t);
		}
		free(p->service);
	}
}

/*
 * this should really be somewhere else ...
 */

rmutmp(tty)
	char *tty;
{
	static struct utmp ut, vt;
	int fd;
	long time();

	if(debug)
		logevent("rmutmp(%s)\n", tty);
	if (strncmp(tty, "/dev/", 5) == 0)
		tty += 5;
	strncpy(vt.ut_line, tty, sizeof(vt.ut_line));
	vt.ut_time = time((long *)0);
	if ((fd = open("/etc/utmp", 2)) >= 0) {
		while (read(fd, (char *)&ut, sizeof(ut)) == sizeof(ut))
			if (strncmp(tty, ut.ut_line, sizeof(ut.ut_line)) == 0) {
				lseek(fd, (long)-sizeof(ut), 1);
				write(fd, (char *)&vt, sizeof(vt));
				break;
			}
		close(fd);
	}
	if ((fd = open("/usr/adm/wtmp", 1)) >= 0) {
		lseek(fd, 0L, 2);
		write(fd, (char *)&vt, sizeof(vt));
		close(fd);
	}
}
