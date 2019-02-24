#ifndef __ASSERT
#define __ASSERT

#ifdef NDEBUG
#define assert(b)
#else
#define assert(b) (b || (fprintf(stderr, "assertion failed: file %s, line %d\n", \
	__FILE__, __LINE__), abort()))
#endif

#endif
