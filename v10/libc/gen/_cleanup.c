#include <libc.h>

extern int (*_atexitfns[])();

void
_cleanup()
{
	int i, (*f)();

	for(i = NONEXIT-1; i >= 0; i--)
		if(_atexitfns[i]){
			f = _atexitfns[i];
			_atexitfns[i] = 0;	/* self defense against bozos */
			(*f)();
		}
}
