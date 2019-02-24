typedef union {
	int *i;
	char *c;
} anything;
main()
{
	register anything src,dest;
	*dest.i++ = *src.i++;
	*dest.c++ = *src.c++;
}
