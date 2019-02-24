#ifndef __ASSERT
#define __ASSERT

void assert(int);

#endif /* __ASSERT */

#undef assert
#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
extern int _assert(char *, unsigned);
#define assert(e) ((void)((e)||_assert(__FILE__, __LINE__)))
#endif /* NDEBUG */
