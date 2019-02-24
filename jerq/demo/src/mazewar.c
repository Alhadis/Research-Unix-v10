#include "stdio.h"
#include "sgtty.h"
#include "signal.h"

char *umpire = "/tmp/maze";
char *game = "/usr/jerq/demo/maze";

struct sgttyb tty, svtty;

main()
{	int fd;
	char *ttyname();
	char *mytty;
	fd = open(umpire, 2);
	if(fd < 0) {
		perror(umpire);
		exit(1);
	}
	mytty = ttyname(0);
	chmod(mytty, 0666);
	ioctl(0, TIOCGETP, &svtty);
	tty = svtty;
	tty.sg_flags |= RAW;
	tty.sg_flags &= ~ECHO;
	ioctl(0, TIOCSETP, &tty);
	if(!boot(game)) {
		perror(game);
		quit();
	}
	write(fd, mytty, strlen(mytty) + 1);
	read(fd, mytty, 1);
	close(fd);
	quit();
}

quit()
{
	boot("/usr/jerq/lib/term");
	ioctl(0, TIOCSETP, &svtty);
	exit(0);
}

boot(s)
char *s;
{
	if(system("/usr/jerq/bin/68ld", "68ld", "-", s))
		return(0);
	return(1);
}

system(s, t, u, v)
char *s, *t, *u, *v;
{	int status, pid, w;
	if((pid = fork()) == 0) {
		execl(s, t, u, v, 0);
		exit(127);
	}
	while((w = wait(&status)) != pid && w != -1)
		;
	if(w == -1)
		status = -1;
	return(status);
}
