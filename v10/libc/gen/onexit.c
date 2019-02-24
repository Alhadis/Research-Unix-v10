onexit(f)
	int (*f)();
{
	return(atexit(f));
}
