#ifndef __STDLIB
#define __STDLIB

#include <stddef.h>

#define	ERANGE		38
#define	RAND_MAX	((unsigned)1<<31)-1)
#define EXIT_FAILURE	1
#define EXIT_SUCCESS	0

typedef int	div_t;
typedef long	ldiv_t;

extern int	abort(void);
extern int	abs(int);
extern int	atexit(void (*)(void));
extern double	atof(const char*);
extern int	atoi(const char*);
extern long	atol(const char*);
extern void*	bsearch(const void*, const void*, size_t, size_t, int (*)(const void*, const void*));
extern void*	calloc(size_t, size_t);
extern div_t	div(int);
extern void	exit(int);
extern void	free(void*);
extern char*	getenv(const char*);
extern long	labs(long);
extern ldiv_t	ldiv(long, long);
extern void*	malloc(size_t);
extern void	qsort(void*, size_t, size_t, int (*)(const void*, const void*));
extern int	rand(void);
extern void*	realloc(void*, size_t);
extern void	srand(unsigned int);
extern double	strtod(const char*, char**);
extern long	strtol(const char*, char**, int);
extern unsigned long	strtoul(const char*, char**, int);
extern int	system(const char *);

#endif
