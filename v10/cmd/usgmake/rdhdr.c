#include	"ar.h"
main()
{
	int	fd, n;
	struct	ar_hdr arhdr;
	fd = open("x", 0);
	n = read(fd, &arhdr, sizeof(struct ar_hdr));
	printf("%16s\n", arhdr.ar_name);
	printf("%12s\n", arhdr.ar_date);
	printf("%10s\n", arhdr.ar_size);
	printf("%2s\n", arhdr.ar_fmag);
}
