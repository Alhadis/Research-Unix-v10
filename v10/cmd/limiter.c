#include <sys/types.h>
#include <sys/filio.h>
#include <stdio.h>
#include <sys/stream.h>

extern int conn_ld;
extern int mesg_ld;
#define	msglen(mp)	((mp)->losize + ((mp)->hisize<<8))

int count;		/* max connections */
fd_set serving;		/* clients being served */
int nserving;
fd_set waiting;		/* clients waiting */
int nwaiting;
int fd;			/* mounted fd */


main(ac, av)
	int ac;
	char *av[];
{
	if(ac!=3)
		die("usage: %s path count\n", av[0]);
	fd=mountit(av[1]);
	count=atoi(av[2]);
	if(count<=0)
		die("usage: %s path count\n", av[0]);
	detach();
	doit();
}

doit()
{
	int i, n;
	fd_set afds;

	FD_ZERO(serving);
	for(;;){
		afds=serving;
		FD_SET(fd, afds);
		switch(n=select(NOFILE, &afds, 0, 10000L)){
		case -1:
			/* there's a bad fd in the group */
			afds=serving;
			for(i=0; i<NOFILE; i++)
				if(FD_ISSET(i, afds))
					dropclient(i);
			break;
		case 0:
			/* timeout, try again */
			break;
		default:
			/* look for new clients */
			if(FD_ISSET(fd, afds)){
				request();
				n--;
			}

			/* someone needs servicing */
			for(i=0; n && i<NOFILE; i++){
				if(FD_ISSET(i, afds)){
					dropclient(i);
					n--;
				}
			}
			break;
		}
	}
}

newclient(cfd)
	int cfd;
{
	FD_SET(cfd, waiting);
	nwaiting++;
	wtos();
}

dropclient(cfd)
	int cfd;
{
	FD_CLR(cfd, serving);
	nserving--;
	close(cfd);
	wtos();
}

wtos()
{
	int i=0;
	struct mesg m;

	while(nwaiting && nserving<count){
		for(;i<NOFILE;i++){
			if(FD_ISSET(i,waiting)){
				nwaiting--;
				nserving++;
				FD_SET(i, serving);
				FD_CLR(i, waiting);
				m.losize = 0;
				m.hisize = 0 >> 8;
				m.type = M_HANGUP;
				m.magic = MSGMAGIC;
				if(ioctl(i, FIOACCEPT, (void *)0)<0
				|| ioctl(i, FIOPUSHLD, &mesg_ld)<0
				|| write(i, &m, sizeof(m))!=sizeof(m)){
					close(i);
					FD_CLR(i, serving);
					nserving--;
					continue;
				}
			}
		}
	}
}

request()
{
	struct passfd pass;

	if(ioctl(fd, FIORCVFD, &pass)<0)
		abort();
	newclient(pass.fd);
}

mountit(path)
	char *path;
{
	int pfd[2];

	/* make a node to mount onto */
	if (access(path,0)<0 && creat(path, 0666)<0)
		die("can't creat %s\n", path);

	/* get the stream to mount */
	if (pipe(pfd) < 0)
		die("out of pipes\n", 0);
	if (ioctl(pfd[1], FIOPUSHLD, &conn_ld) < 0)
		die("can't push line discipline\n", 0);

	/* mount */
	if (fmount(3, pfd[1], path, 0) < 0)
		die("can't mount onto %s\n", path);
	umask(0);
	chmod(path, 0666);
	close(pfd[1]);
	return pfd[0];
}

detach()
{
	switch(fork()){
	case -1:
		die("can't fork\n", 0);
	case 0:
		break;
	default:
		exit(0);
	}
	for(fd=0; fd<NSYSFILE; fd++)
		close(fd);
	setpgrp(getpid(), getpid());
}

die(a, b)
	char *a;
{
	fprintf(stderr, a, b);
	exit(1);
}
