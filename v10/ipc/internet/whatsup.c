#include <stdio.h>
#include <signal.h>
int fd;

catch(i)
	int i;
{
	char buf[32];

	sprintf(buf, "signal %d\n", i);
	write(fd, buf, strlen(buf));
	close(fd);
	exit(1);
}

main()
{
	int i;
	char c;

	unlink("/tmp/whatsup");
	fd = creat("/tmp/whatsup", 0666);
	for (i = 0; i < 32; i++)
		signal(i, catch);
	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	while(read(0, &c, 1)==1) {
		if (write(1, &c, 1)!=1)	{
			write(fd, "ERROR 1\n", 8);
			exit(1);
		}
		write(fd, &c, 1);
	}
	if(write(1, "EOF\n", 4)!=4) {
		write(fd, "ERROR 2\n", 8);
		exit(1);
	}
	write(fd, "EOF\n", 4);
	exit(0);
}
