#include <termio.h>
static struct	termio save_tio;

setraw(on)
	int on;
{
	struct termio tio;

	ioctl(0, TCGETA, (char *)&tio);
	if (on) {
		tio.c_lflag &= ~ICANON;
		tio.c_iflag &= ~ICRNL;
		tio.c_cc[VMIN] = 1;
		tio.c_lflag &= ~ECHO;
		tio.c_cc[VINTR] = -1;
	} else {
		tio.c_lflag |= ICANON;
		tio.c_iflag |= ICRNL;
		tio.c_cc[VMIN] = 1;
		tio.c_lflag |= ECHO;
		tio.c_cc[VINTR] = -1;
	}
	ioctl(0, TCSETA, (char *)&tio);
}


setecho(on)
	int on;
{
	struct termio tio;

	ioctl(0, TCGETA, (char *)&tio);
	if (on) {
		tio.c_lflag |= ECHO;
	} else {
		tio.c_lflag &= ~ECHO;
	}
	ioctl(0, TCSETA, (char *)&tio);
}

savetty()
{
	ioctl(0, TCGETA, (char *)&save_tio);
}

restoretty()
{
	ioctl(0, TCSETA, (char *)&save_tio);
}
