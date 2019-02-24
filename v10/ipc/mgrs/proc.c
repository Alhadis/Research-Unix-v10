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

typedef struct {
	int pid;
	char *tty;
	char *service;
} Proc;

#define	MAXPROC	1024
Proc proc[MAXPROC];
Proc *maxproc;
static int sigchld;
static int children;

procdied()
{
	sigchld = 1;
}

ding()
{
}

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

	signal(SIGCHLD, procdied);
	switch(pid=fork()){
	case -1:
		logevent("can't fork; gave up\n");
		_ipcabort(0, "", rp->i);
		return(0);
	case 0:
		closesvcs();
		return (1);
	default:
		children++;
		break;
	}
	tty = ttyname(rp->i->cfd>=0 ? rp->i->cfd : rp->i->rfd);
	_ipcabort(0, "", rp->i);
	if (tty==NULL)
		return(0);
	for (p = proc; p < &proc[MAXPROC]; p++)
		if(p->pid && strcmp(tty, p->tty)==0)
			checkkids(p->pid);
	for (p = proc; p < &proc[MAXPROC]; p++)
		if(p->pid == 0) {
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
 * clean up after any dead children
 */
checkkids(spid)
{
	int pid;
	int status;

	if(sigchld){
		sigchld = 0;
		while(children){
			signal(SIGALRM, ding);
			alarm(2);
			errno = 0;
			pid = wait(&status);
			if(errno && pid>=0){
				alarm(0);
				logevent("pid = %d errno = %d\n", errno, pid);
				break;
			}
			alarm(0);
			if(pid<0)
				break;
			deadproc(pid, status, spid!=pid);
		}
	}
}

deadproc(pid, status, doutmp)
	int pid;
	int status;
	int doutmp;
{
	register Proc *p;
	int e, t;

	logevent("deadproc(%d, %ux, %d)\n", pid, status, doutmp);
	e= (status>>8)&0177;
	t= status&0377;

	for (p = proc; p < &proc[MAXPROC]; p++)
		if (p->pid == pid)
			break;
	if(p==&proc[MAXPROC]) {
		logevent("non-tty proc %d died\n", pid);
		return;
	}
	children--;
	rmutmp(p->tty, doutmp);
	p->pid = 0;
	free(p->tty);
	p->tty = 0;
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

rmutmp(tty, doutmp)
	char *tty;
	int doutmp;
{
	static struct utmp ut, vt;
	int fd;
	long time();

	logevent("rmutmp(%s, %d)\n", tty, doutmp);
	if (strncmp(tty, "/dev/", 5) == 0)
		tty += 5;
	strncpy(vt.ut_line, tty, sizeof(vt.ut_line));
	vt.ut_time = time((long *)0);
	if (doutmp && (fd = open("/etc/utmp", 2)) >= 0) {
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
