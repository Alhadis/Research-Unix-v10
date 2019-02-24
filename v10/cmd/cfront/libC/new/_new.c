
typedef void (*PFVV)();

extern PFVV _new_handler;

extern "C" {
	extern char* malloc(unsigned);
}

extern void* operator new(long size)
{
	char* p;

	while ( (p=malloc(unsigned(size)))==0 ) {
		if(_new_handler && size)
			(*_new_handler)();
		else
			return 0;
	}
	return (void*)p;
}
