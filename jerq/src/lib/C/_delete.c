#include <CC/jerq.h>
extern void operator delete(void* p)
{
	if (p) free( (char*)p );
}
