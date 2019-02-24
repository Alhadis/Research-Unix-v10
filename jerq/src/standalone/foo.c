#include <jerq.h>
#include <setup.h>

#define NPROC 16

extern int end;

main()
{
	int i,j;

	*DADDR=0;
	BonW();
	qinit();
	aciainit(baud_speeds[VALBAUD]);
	binit();
	kbdinit();
	cursinit();
	allocinit(((char *)&end), ((char *)&end)+256*1024L);
	gcinit();
/*
	Lgrey(&display);
*/
	spl0();
	while(1);
	exit();
}

#undef ringbell
extern void ringbell();

deb_bell(i)
int i;
{
	int j;

	for(;i>0;i--) {
		WonB();
		for(j=0; j<10000; j++)
			;
		BonW();
	}
	for(j=0; j<100000; j++)
		;
}
