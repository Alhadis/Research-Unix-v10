typedef void (*PFV)();

extern void _exit(int i)
{
	extern PFV _dtors[];
	PFV* pf = _dtors;
	while (*pf) pf++;
	while (_dtors < pf) (**--pf)();
}
