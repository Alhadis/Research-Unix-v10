#include <libc.h>

extern "C" {
	extern int __dtors();
	extern void _main();
}

void _main()
{
	typedef void (*PFV)();
	extern PFV _ctors[];
	for (PFV* pf=_ctors; *pf; pf++) (**pf)();
	onexit (__dtors);
}
