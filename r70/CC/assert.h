#ifndef __ASSERT_H
#define __ASSERT_H 1
#ifndef NDEBUG
extern "C" {
	int _assert (char*, int);
}
#define assert(e) (void)((e)||_assert(__FILE__,__LINE__))
#else
#define assert(e) (void)1
#endif
#endif
