#include <CC/jerq.h>

typedef void (*PFVV)();

extern PFVV _new_handler;

extern void* operator new(long size)
{
	char* p;

	while ( (p=alloc(unsigned(size)))==0 ) {
		if(_new_handler)
			(*_new_handler)();
		else
			return 0;
	}
	return (void*)p;
}
