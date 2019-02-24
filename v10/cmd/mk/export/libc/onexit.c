#include	<libc.h>

int (*_onexitfns[NONEXIT])();

atexit(f)
int (*f)();
{
	onexit(f);
}

onexit(f)
int (*f)();
{
	int i;

	for(i=0; i<NONEXIT; i++)
		if(!_onexitfns[i]){
			_onexitfns[i] = f;
			return(1);
		}
	return(0);
}
