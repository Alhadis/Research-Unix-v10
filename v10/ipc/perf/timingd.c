#include <stdio.h>
#include <sys/ioctl.h>
main()
{
	struct passfd pass;
	int fd, nfd;
	char c;
	extern int errno;

	if((fd=domount("quux"))<0)
		exit(errno);
	fprintf(stderr, "domount(quux) returns %d\n", fd);
	for(;;) {
		if (ioctl(fd, FIORCVFD, &pass)<0) {
			close(fd);
			exit(2);
		}
		fprintf(stderr, "ioctl(fd, FIORCVFD, &pass) returns %d\n",pass.fd);
		nfd = pass.fd;
		(void)ioctl(nfd, FIOACCEPT, &pass);
		fprintf(stderr, "ioctl(fd, FIOACCEPT, &pass) returned\n");
		read(nfd, &c, 1);
		close(nfd);
	}
}

domount(name)
	char *name;
{
	int pfd[2];
	extern int conn_ld;
	
	if (pipe(pfd) < 0)
		return -1;
	if (ioctl(pfd[1], FIOPUSHLD, &conn_ld) < 0)
		return -1;

	/* mount */
	if (fmount(3, pfd[1], name, 0) < 0)
		return -1;
	close(pfd[1]);
	return(pfd[0]);
}
