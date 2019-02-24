#include <a.out.h>

#define	STDIN	0
#define	STDOUT	1
#define	STDERR	2

#define	PGSIZE	1024
#define	BSIZE	4096

main()
{
	struct exec a;

	if (read(STDIN, (char *)&a, sizeof(a)) != sizeof(a)) {
		fprint(STDERR, "no header\n");
		exit(1);
	}
	if (a.a_magic != 0407 && a.a_magic != 0410) {
		fprint(STDERR, "bad magic number\n");
		exit(1);
	}
	copy(a.a_text);
	if (a.a_magic == 0410)
		pad(PGSIZE - (a.a_text & (PGSIZE-1)));
	copy(a.a_data);
	pad(a.a_bss);
	exit(0);
}

copy(n)
register int n;
{
	char buf[BSIZE];
	register int rn, r;

	while (n > 0) {
		r = (n > BSIZE) ? BSIZE : n;
		rn = read(STDIN, buf, r);
		if (rn <= 0) {
			fprint(STDERR, "short\n");
			exit(1);
		}
		if (write(STDOUT, buf, rn) != rn) {
			fprint(STDERR, "write error\n");
			exit(1);
		}
		n -= rn;
	}
}

pad(n)
register int n;
{
	char buf[BSIZE];
	register int i;

	for (i = 0; i < BSIZE; i++)
		buf[i] = 0;
	while (n > 0) {
		i = (n > BSIZE) ? BSIZE : n;
		if (write(STDOUT, buf, i) != i) {
			fprint(STDERR, "write error\n");
			exit(1);
		}
		n -= i;
	}
}
