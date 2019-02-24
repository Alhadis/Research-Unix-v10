#include <jerq.h>
#include <jerqio.h>
#define _SYSEXIT	4

exit(i)
{
	register int i;
	register FILE *f;
	for (i = 1, f = &_iob[1]; i < _NFILE; i++, f++)
		fflush(f);
	tag(_SYSEXIT);
	jputshort(i);
	jflush();
	jgetshort();
	Ivoid(17)();
}
