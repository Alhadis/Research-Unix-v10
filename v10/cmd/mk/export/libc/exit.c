#include	<libc.h>

extern int (*_onexitfns[])();

void
exit(status)
{
	int i, (*f)();

	for(i = NONEXIT-1; i >= 0; i--)
		if(_onexitfns[i]){
			f = _onexitfns[i];
			_onexitfns[i] = 0;	/* self defense against bozos */
			(*f)();
		}
	_exit(status);
}
