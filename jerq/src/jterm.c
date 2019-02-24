#include "/usr/jerq/include/jioctl.h"
#include <sys/ttyio.h>
#include <sys/filio.h>

extern int tty_ld;

main()
{
	struct tchars tc;
	struct sgttyb sg;
	if (ioctl(0, FIOLOOKLD, (char *)0) == tty_ld) {
		ioctl(0, TIOCGETC, &tc);
		ioctl(0, TIOCGETP, &sg);
		ioctl(0, FIOPOPLD, (char *)0);
		ioctl(0, TIOCSETC, &tc);
		ioctl(0, TIOCSETP, &sg);
	}
	ioctl(0, JTERM, (char *)0);
	exit(0);
}
