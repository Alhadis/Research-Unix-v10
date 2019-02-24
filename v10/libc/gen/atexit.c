#include	<libc.h>

int (*_atexitfns[NONEXIT])();

atexit(f)
int (*f)();
{
	int i;

	for(i=0; i<NONEXIT; i++)
		if(!_atexitfns[i]){
			_atexitfns[i] = f;
			return(1);
		}
	return(0);
}
