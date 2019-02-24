/*	@(#)ioctl.c	3.3	*/
#include <sgtty.h>

static struct termcb termst;
	static int termfl, termtyp;

Ioctl() {
	register char *p;

	/*
	 * SAVE TERMINAL INFO ... CB MOD (tjc)
	 */
	ioctl(1,DIOCGETT,&termst);
	termfl = termst.st_flgs;
	termtyp = termst.st_termt;
	if((p = getenv("TERM")) != 0)
		if( (!strcmp(p+5, "VIRTUAL")) && (!strcmp(p+5, "virtual")))
			return;
	termst.st_termt = 0;
	termst.st_flgs = 0;
	ioctl(1,DIOCSETT,&termst);

}

unIoctl()
{

	/*
	 * RESTORE TERMINAL TYPE ... CB MOD
	 */

	termst.st_termt = termtyp;
	termst.st_flgs = termfl;
	ioctl(1,DIOCSETT,&termst);
}

exit(i)
	int i;
{
	unIoctl();
	_exit(i);
}

