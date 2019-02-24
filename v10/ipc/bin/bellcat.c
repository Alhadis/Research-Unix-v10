#include <sys/types.h>
#include <fio.h>
#include <ipc.h>

#define ustring "usage: scriptcon dest script-file\n"

usage()
{
	write(2, ustring, sizeof(ustring)-1);
	exit(1);
}

main(ac, av)
	int ac;
	char *av[];
{
	int sfd;
	int cfd;
	char *lookfor;
	char *sendback;
	fd_set fds;

	if (ac<3)
		usage();
	sfd = open(av[2], 0);
	if (sfd<0){
		fprint(2, "can't open %s\n", av[2]);
		exit(1);
	}
	cfd = ipcopen(ipcpath(av[1], "dk", ""), "");
	if (cfd<0){
		fprintf(stderr, "can't open %s\n", av[2]);
		exit(1);
	}

	/*
	 *  do the script
	 */
	while((lookfor=Frdline(sfd))!=NULL){
		await(cfd, lookfor);
		if((sendback=Frdline(sfd))!=NULL)
			fprint(cfd, "%s\n", sendback);
	}
	close(sfd);

	/*
	 *  shuttle bytes back and forth
	 */
	FD_ZERO(fds);
	for(;;) {
		FD_SET(cfd, fds);
		FD_SET(0, fds);
		switch(select(NOFILE, &fds, (struct fd_set *)0, 1000)) {
		case -1:
			return -1;
		case 0:
			continue;
		}
		if (FD_ISSET(cfd, fds))
			if (pass(cfd, 1)<0)
				exit(0);
		if (FD_ISSET(0, fds))
			if (pass(0, cfd)<0)
				exit(0);
	}
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

	if(len>sizeof(buf)-2)
		len = sizeof(buf)-2;
	len = strlen(str);
	for(c=0; c<len; c++)
		buf[c] = '\0';   
	while((c=Fgetc(fd))>=0){
		buf[len] = c;
		strcpy(buf, buf+1);
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

	if ((n=read(from, buf, sizeof(buf)))<=0)
		return -1;
	if (write(to, buf, n)!=n)
		return -1;
	return 0;
}
