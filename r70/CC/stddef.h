#ifndef __STDDEF
#define __STDDEF

#define NULL 0
#define offsetof(ty,mem) ((size_t) &(((ty *)0)->mem))

typedef long ptrdiff_t;
typedef unsigned size_t;
typedef char wchar_t;

#endif
