char *alloc();
char *malloc(n)
	unsigned n;
{
	return alloc((unsigned long)n);
}
