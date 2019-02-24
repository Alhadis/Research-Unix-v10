extern "C" {
	extern free(char*);
}

extern void operator delete(void* p)
{
	if (p) free( (char*)p );
}
