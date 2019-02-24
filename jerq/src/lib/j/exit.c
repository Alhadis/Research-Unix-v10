#include <jerq.h>
#include <jerqio.h>
#define _SYSEXIT	4

exit()
{
	register int i;
	register FILE *f;
	for (i = 1, f = &_iob[1]; i < _NFILE; i++, f++)
		fclose(f);
	tag(_SYSEXIT);
	jputshort(i);
	jflush();
	jgetshort();
	reboot();
}
