#include <sys/param.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/pioctl.h>
#include <sys/stat.h>
#include <utmp.h>

#define	UADDR	(0x80000000-(UPAGES*NBPG))

/*
 * figure out the name of the tty
 * on which i originally logged in
 * by guess and by hack
 */

main()
{
	char *tty;
	char *mytty();

	if ((tty = mytty()) == 0) {
		printf("no login tty\n");
		exit(1);
	}
	printf("%s\n", tty);
	exit(0);
}

static char *
procname(pid)
int pid;
{
	static char buf[20];

	sprint(buf, "/proc/%d", pid);
	return (buf);
}

#define	MAXME	20

struct ttyi {
	ino_t ino;
	char name[14];	/* 8 + "/dev/" + \0 */
};

char *
mytty()
{
	static struct ttyi mt[MAXME];
	register int i;
	register int e;
	int pid;
	int fd;
	struct proc p;
	struct user u;
	char *myname;
	char *getlogin();

	if ((myname = getlogin()) == NULL)
		return (NULL);
	if ((e = mtinit(myname, mt)) == 0)
		return (NULL);
	for (pid = getpid();; close(fd), pid = p.p_ppid) {
		if ((fd = open(procname(pid), 0)) < 0)
			break;
		if (ioctl(fd, PIOCGETPR, &p) < 0)
			break;
		if (p.p_ppid == pid)
			break;
		lseek(fd, UADDR, 0);
		if (read(fd, (char *)&u, sizeof(u)) != sizeof(u))
			break;
		if (strncmp(u.u_logname, myname, sizeof(u.u_logname)) != 0)
			break;
		for (i = 0; i < e; i++)
			if (mt[i].ino == u.u_ttyino) {
				close(fd);
				return (mt[i].name);
			}
	}
	if (fd >= 0)
		close(fd);
	return (NULL);
}

static
mtinit(myname, mt)
char *myname;
register struct ttyi *mt;
{
	struct utmp ut[20];
	int fd;
	register struct utmp *u, *e;
	register int i;
	register int n;
	struct stat st;

	if ((myname = getlogin()) == NULL)
		return (0);
	if ((fd = open("/etc/utmp", 0)) < 0)
		return (0);
	i = 0;
	while ((n = read(fd, (char *)ut, sizeof(ut))) > 0) {
		e = (struct utmp *)((char *)ut + n);
		for (u = ut; u < e; u++) {
			if (u->ut_name[0] == 0)
				continue;
			if (strncmp(u->ut_name, myname, sizeof(u->ut_name)) != 0)
				continue;
			sprintf(mt[i].name, "/dev/%.8s", u->ut_line);
			if (stat(mt[i].name, &st) < 0)
				continue;
			mt[i].ino = st.st_ino;
			if (++i >= MAXME) {
				close(fd);
				return (i);
			}
		}
	}
	close(fd);
	return (i);
}
