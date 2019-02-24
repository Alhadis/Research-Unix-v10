#ifndef	Malloc

#define Malloc(type,n)		(type *)malloc((n)*sizeof(type))
#define Calloc(type,n)		(type *)calloc((n),sizeof(type))
#define Realloc(type,ptr,n)	(type *)realloc(ptr,(n)*sizeof(type))
#define Free(ptr)		(ptr ? (free(ptr), ptr=0) : ptr)

char *malloc(), *calloc(), *realloc();

#endif
