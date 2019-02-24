/* Copyright AT&T Bell Laboratories, 1994 */
#include <stddef.h>
#include <string.h>

enum { S = sizeof(long) };

void *
memmove(void *to, const void *from, size_t n)
{
	char *out = to;
	const char *in = from;
	if(out > in) {
		in += n;
		out += n;
		if((in-out)%S == 0) {
			for( ; (in-(char*)0)%S && n>0; n--)
				*--out = *--in;
			for( ; n>=S; n-=S) {
				out -= S;
				in -= S;
				*(long*)out = *(long*)in;
			}
		}
		for( ; n>0; n--)
			*--out = *--in;
	} else {
		if((in-out)%S == 0) {
			for( ; (in-(char*)0)%S && n>0; n--)
				*out++ = *in++;
			for( ; n>=S; n-=S) {
				*(long*)out = *(long*)in;
				out += S;
				in += S;
			}
		}
		for( ; n>0; n--)
			*out++ = *in++;
	}
	return(to);
}
