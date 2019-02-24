#include	<sys/param.h>
main(argc, argv)
	char **argv;
{
	Long off;
	long a, b;

	a = atoi(argv[1]);
	b = atoi(argv[2]);
	off = Llmul(ltoL(a), b);
	print("%d(#%x) x %d(#%x) = #%ux%0.8ux(%.0f)\n", a, a, b, b, off.hi, off.lo,
		((float)off.lo) + 65536.0*65536.0*off.hi);
	if(llseek(open("/dev/null", 0), off, 0) < 0)
		perror("seek");
}
