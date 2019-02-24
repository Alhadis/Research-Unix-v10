#ifndef __STDDEF
#define __STDDEF

typedef	long	ptrdiff_t;
typedef unsigned int	size_t;
typedef char	wchar_t;

#define	NULL	0
#define offsetof(t,x) ((int)&(((t*)0)->x))

#endif
