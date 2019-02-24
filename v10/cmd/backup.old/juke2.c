#include	<fio.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<signal.h>
#include	"backup.h"
#include	<ipc.h>
#include	<sys/filio.h>
#include	<errno.h>
#include	"juke.h"

/*
	server code
*/
Server *servers = 0;
#define		NSLOTS		16
static struct
{
	char *dev;
	Server *s;
} slot[NSLOTS];
static int nslots;

serverinit(s)
	char **s;
{
	for(nslots = 0; *s; nslots++){
logprint("slot %d: dev %s\n", nslots, *s);
		slot[nslots].dev = *s++;
		slot[nslots].s = 0;
	}
}

queueclient(c)
	Client *c;
{
	Server *s;
	Disk *d;

	for(s = servers; s; s = s->next)
		for(d = c->disks; d; d = d->next)
			if(strcmp(s->name, d->name) == 0)
					queuedisk(d, s);
}

queuedisk(d, s)
	Server *s;
	Disk *d;
{
	Disk *dd;

	if(s->disks == 0)
		s->disks = d;
	else {
		for(dd = s->disks; dd->snext; dd = d->snext)
			if(d == dd)
				return;
		if(d == dd)
			return;
		dd->snext = d;
	}
logprint("queue disk(c=%d) to server %s\n", d->c, d->name);
	if(!s->busy)
		feedserver(s);
}

ackserver(s)
	register Server *s;
{
	char buf[1];

	read(s->fd, buf, 1);
logprint("ack server %s: disk %d done\n", s->name, s->disks);
	diskdone(s->disks);
	if(s->disks = s->disks->snext)
		feedserver(s);
	else
		s->busy = 0;
	/* have a look around in case something happens in outside world */
	sched();
}

feedserver(s)
	register Server *s;
{
	int fpipe[2], i, *p;
	Disk *d;

	s->busy = 1;
	d = s->disks;
	d->busy = 1;
logprint("feeding disk %d to server %s\n", d, s->name);
	if(pipe(fpipe) < 0)
		logprint("%s: fpipe failed errno=%d\n", me, errno);
	if(ioctl(s->fd, FIOSNDFD, &fpipe[1]) < 0)
		logprint("%s: send input fd failed; errno=%d\n", me, errno);
	if(ioctl(s->fd, FIOSNDFD, &d->c->fd) < 0)
		logprint("%s: send output fd failed; errno=%d\n", me, errno);
	close(fpipe[1]);
	/*
		now send requests for this disk
	*/
	Finit(fpipe[0], (char *)0);
	Fprint(fpipe[0], "%s\n", d->c->userinfo);
	for(i = 0, p = d->files; i < d->n; i++, p += 2)
		Fprint(fpipe[0], "v%d/%d\n", p[0], p[1]);
	Fprint(fpipe[0], "bye\n");
	Fclose(fpipe[0]);
logprint("feedserver %s: names sent\n", s->name);
}

Server *
newserver(name)
	char *name;
{
	int i;
	Server *s;

	for(i = 0; slot[i].s && (slot[i].s->busy); i++)
		if(i >= nslots)
			return(0);
	if(slot[i].s)
		close(slot[i].s->fd);
	else
		slot[i].s = (Server *)malloc(sizeof(Server));
logprint("newserver(%s) in slot %d\n", name, i);
	s->busy = 0;
	s->name = strdup(name);
	s->fd = spawn(slot[i].dev, "/cs/worm");
	s->disks = 0;
	s->next = servers;
	servers = s;
	return(s);
}

spawn(n1, n2)
	char *n1, *n2;
{
	int p[2], pid;

	if(pipe(p) < 0){
		logprint("%s: pipe fail errno=%d\n", me, errno);
		exit(1);
	}
	if((pid = fork()) < 0){
		logprint("%s: pipe fail errno=%d\n", me, errno);
		exit(1);
	}
	if(pid)
		close(p[0]);
	else {
		close(p[1]);
		dup2(p[0], 0);
		execl("/usr/lib/backup/wormy", "wormy", n1, n2, 0);
		logprint("%s: pipe fail errno=%d\n", me, errno);
		exit(1);
	}
	return(p[1]);
}
