#include <signal.h>
#include <errno.h>
#include <sgtty.h>

char *pexpw();

char *
pexpw(fd, prompt)
char *prompt;
{
	int (*sf)();
	/* struct pexclude x; */
	struct sgttyb usual, special;
	register char *p;
	register i;
	static char pbuf[9];

	sf = signal(SIGINT, SIG_IGN);
	p = pbuf;

	/* if(pex(fd, -1, &x)==0) */
	{
		/*
		if(x.farpid>0 && x.farcap == 0) {
			p = 0;
			goto out;
		}
		*/
		ioctl(fd, TIOCGETP, &usual);
		special.sg_flags = usual.sg_flags & ~ECHO;
		ioctl(fd, TIOCSETP, &special);

		write(fd, prompt, strlen(prompt));
		for(i=0;;) {
			char c;
			if(read(fd, &c, 1) != 1 || c == '\n')
				break;
			if(i<8)
				pbuf[i++] = c;
		}
		while(i<9)
			pbuf[i++] = 0;
		ioctl(fd, TIOCSETP, &usual);
	}
	/*
	else
		p = 0;
	*/
out:
	/* unpex(fd, -1); */
	(void) signal(SIGINT, sf);
	if(p)write(fd,"\n",1);
	return(p);
}
