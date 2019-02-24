/*
 * ANSI C atexit()
 * arrange for func to be called LIFO on exit()
 */

#include "FEATURE/atexit"

#ifdef _lib_atexit

int	_lib_dummy;

#else

typedef void (*PFV)();

#include "FEATURE/onexit"

#ifdef _lib_onexit

int
atexit(func)
PFV	func;
{
	return(onexit(func));
}

#else

#include "FEATURE/on_exit"

#ifdef _lib_on_exit

int
atexit(func)
PFV	func;
{
	return(on_exit(func));
}

#else

#include "FEATURE/_cleanup"

struct list
{
	struct list*	next;
	PFV		func;
};

static struct list*	funclist;

extern char*		malloc();

int
atexit(func)
PFV	func;
{
	register struct list*	p;

	if (!(p = (struct list*)malloc(sizeof(struct list)))) return(-1);
	p->func = func;
	p->next = funclist;
	funclist = p;
	return(0);
}

int
exit(code)
int	code;
{
	register struct list*	p;

	for (p = funclist; p; p = p->next)
		(*p->func)();
#ifdef _lib__cleanup
	_cleanup();
#endif
	_exit(code);
}

#endif

#endif

#endif
