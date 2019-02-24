#ifndef NDEBUG
#define assert(e) (void)((e)||_assert(__FILE__,__LINE__))
#else
#define assert(e) (void)1
#endif
