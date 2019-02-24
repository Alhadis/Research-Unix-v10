static char failed[] = "Assertion failed: file ";
static char line[] = ", line ";
_assert(f,l)
char *f;
{
	_cleanup();
	write(2,failed,strlen(failed));
	write(2,f,strlen(f));
	write(2,line,strlen(line));
	_printnum((unsigned)l);
	write(2,"\n",1);
	abort();
}
