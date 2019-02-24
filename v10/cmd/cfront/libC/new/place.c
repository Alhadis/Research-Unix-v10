#include <new.h>

void* operator new(long, void* p)
{
	return p;
}
