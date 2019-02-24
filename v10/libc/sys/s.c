struct Long { int lo; unsigned hi; };

main()
{
	Long llseek(), ll;
	extern errno;

	for (;;)
		scanf("%x %x", &ll.hi, &ll.lo);
		errno = 0;
		ll = llseek(0, ll, 0);
		printf("%x%.8x %d", ll.hi, ll.lo, errno);
	}
}
