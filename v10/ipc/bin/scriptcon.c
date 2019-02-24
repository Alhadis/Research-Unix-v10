#include <sys/param.h>
#include <fio.h>
#include <ipc.h>
#include <sys/ttyio.h>
#include <signal.h>
#include <libc.h>

struct sgttyb old;
struct tchars chars;
int cfd;

done()
{
	cleanup();
	exit(0);
}

intr()
{
	write(cfd, &chars.t_intrc, 1);
	signal(SIGINT, intr);
}

dirty()
{
	struct sgttyb new;

	ioctl(0, TIOCGETP, &old);
	ioctl(0, TIOCGETC, &chars);
	new = old;
	new.sg_flags &= ~ECHO;
	new.sg_flags |= CBREAK;
	signal(SIGINT, intr);
	signal(SIGQUIT, done);
	signal(SIGHUP, done);
	signal(SIGPIPE, done);
	ioctl(0, TIOCSETP, &new);
}

cleanup()
{
	ioctl(0, TIOCSETP, &old);
}

usage()
{
	fprint(2, "usage: scriptcon dest script-file\n");
	exit(1);
}

main(ac, av)
	int ac;
	char *av[];
{
	int sfd;
	char *lookfor;
	char *sendback;
	fd_set fds;

	if (ac!=3)
		usage();
	sfd = open(av[2], 0);
	if (sfd<0){
		fprint(2, "can't open %s\n", av[2]);
		exit(1);
	}
	cfd = ipcopen(ipcpath(av[1], "dk", ""), "");
	if (cfd<0){
		fprint(2, "can't open %s\n", av[2]);
		exit(1);
	}

	/*
	 *  do the script
	 */
	while((lookfor=Frdline(sfd))!=NULL){
		await(cfd, lookfor);
		if((sendback=Frdline(sfd))!=NULL){
			fprint(cfd, "%s\n", sendback);
		}
	}
	close(sfd);

	/*
	 *  shuttle bytes back and forth
	 */
	dirty();
	FD_ZERO(fds);
	for(;;) {
		FD_SET(cfd, fds);
		FD_SET(0, fds);
		switch(select(NOFILE, &fds, (struct fd_set *)0, 1000)) {
		case -1:
			cleanup();
			return(0);
		case 0:
			continue;
		}
		if (FD_ISSET(cfd, fds))
			if (pass(cfd, 1)<0)
				break;
		if (FD_ISSET(0, fds))
			if (pass(0, cfd)<0)
				break;
	}

	cleanup();
	return(0);
}

/*
 *  wait for the specified string on the fd
 */
await(fd, str)
	int fd;
	char *str;
{
	char buf[128];
	int c;
	int len;

	len = strlen(str);
	if(len>sizeof(buf)-2)
		len = sizeof(buf)-2;
	for(c=0; c<len+2; c++)
		buf[c] = '\0';   
	while((c=Fgetc(fd))>=0){
		buf[len] = c&0x7f;
		memcpy(buf, buf+1, len+1);
		if(strcmp(buf, str)==0)
			break;
	}
}

/*
 *  copy one record from one fd to another
 */
pass(from, to)
	int from, to;
{
	char buf[4096];
	int n;

	if ((n=read(from, buf, sizeof(buf)))<0)
		return -1;
	if (n == 0) {
		if (to == cfd) {
			write(to, &chars.t_eofc, 1);
			return (0);
		}
		return (-1);
	}
	if (write(to, buf, n)!=n)
		return -1;
	return 0;
}
