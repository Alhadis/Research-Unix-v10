#include <sgtty.h>
#include <sys/filio.h>

struct sgttyb save_sgb;
int pushed_ld = 0;


setecho(on)
	int on;
{
	struct sgttyb sgb;

	ioctl(0, TIOCGETP, &sgb);
	if (on)
		sgb.sg_flags |= ECHO;
	else
		sgb.sg_flags &= ~ECHO;
	ioctl(0, TIOCSETP, &sgb);
}

setraw(on)
	int on;
{
	struct sgttyb sgb;

	ioctl(0, TIOCGETP, &sgb);
	if (on)
		sgb.sg_flags |= RAW;
	else
		sgb.sg_flags &= ~RAW;
	ioctl(0, TIOCSETP, &sgb);
}

settty()
{
	/* make sure interface is usable by humans */
	struct tchars tcbuf;
	extern int tty_ld;

	if(ioctl(0, TIOCGETC, &tcbuf) < 0) {
		/* no tty ld (of any type) present; push one */
		if(ioctl(0, FIOPUSHLD, &tty_ld) < 0)
			printf("No tty processing supported; sorry\n");
		pushed_ld++;
	} else
		ioctl(0, TIOCGETP, &save_sgb);
}

restoretty()
{
	if(pushed_ld)
		ioctl(0, FIOPOPLD, (void *)0);
	else
		ioctl(0, TIOCSETP, &save_sgb);
}
