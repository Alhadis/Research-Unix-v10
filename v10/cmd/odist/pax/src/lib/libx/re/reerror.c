/*
 * AT&T Bell Laboratories
 *
 * regular expression error routine
 */

#include <re.h>
#include <error.h>

#if __STDC__ || __cplusplus
void
reerror(const char* s)
#else
void
reerror(s)
char*	s;
#endif
{
	liberror("re", 3, "regular expression: %s", s);
}
