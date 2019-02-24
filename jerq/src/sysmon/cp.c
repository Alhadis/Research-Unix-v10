#include <stdio.h>
#include <nlist.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

char *labels[]={
	"IDLE",
	"USER",
	"KERNEL",
	"WAIT",
	"SXBRK",
};

struct	nlist nl[] = {
	{ "avenrun",1 },
#define	X_AVENRUN	0
    {"sysinfo",0},
#define SYSINFO		1
	{ "", 0 },
};
int	kmem;
int	mem;
long	avenrun[3];
struct sysinfo	si, oi;

main(argc, argv)
	char **argv;
{
	register int i, j, err, n, time=10, sum=0;
	char *cp;

	if ((kmem = open("/dev/mem", 0)) < 0) {
		fprintf(stderr, "No kmem\n");
		exit(1);
	}
	nlist("/unix", nl);
#ifdef DEBUG
	printf("addr %s = %08x\n", nl[SYSINFO].n_name, nl[SYSINFO].n_value);
#endif

	if (nl[SYSINFO].n_value==0) {
		fprintf(stderr, "No namelist\n");
		exit(1);
	}
	if ((err=lseek(kmem, (long)(nl[SYSINFO].n_value-0x80000000), 0))<0)
		perror("seek");
	if ((n=read(kmem, &si, sizeof(si))) != sizeof(si))
		printf("Only read %d\n", n);
	oi = si;
	sleep(time);
	if ((err=lseek(kmem, (long)(nl[SYSINFO].n_value-0x80000000), 0))<0)
		perror("seek");
	if ((n=read(kmem, &si, sizeof(si))) != sizeof(si))
		printf("Only read %d\n", n);
	for (i = 0; i<5; i++) {
		int n=si.cpu[i]-oi.cpu[i];
		sum += n;
		printf("%6s	%5d\n", labels[i], n);
	}
	printf("sum=%d  time=%d\n", sum, time);
	printf("\n");
	exit(0);
}
