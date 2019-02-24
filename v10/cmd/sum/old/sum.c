#include <stdio.h>
/*
 * Sum bytes in file mod 2^16
 */

#include <stdio.h>
#define BUFSIZE	1024
#define MAXSTR	512
/* disk block size for backward compatability */
#define	BLOKSIZE	1024

extern char	*fgets();

char	*ego;

main (argc, argv)
	int	argc;
	char	*argv[];
{
	char	s[MAXSTR];
	int	i, errcode = 0;

	ego = argv[0];

	if (argc < 2)
		sumfile ((char *) 0);
	else	for (i = 1; i < argc; i++)
			if (*argv[i] != '-')
				errcode += sumfile (argv[i]);
			else	switch (argv[i][1]) {
				case 0:
					while(fgets(s,sizeof s, stdin)) {
						s[strlen(s)-1] = 0;
						errcode += sumfile (s);
					}
					break;
				default:
					goto usage;
				}

	exit (errcode);

usage:
	fprintf (stderr, "usage: %s [-] [file]...\n", ego);
	exit (1);
}


sumfile (path)
	char	*path;
{
	static unsigned char	buf[BUFSIZE];
	register unsigned char	*s;
	register int		n, fd;
	register long		nbytes = 0;
	register unsigned int	sum = 0;

	if (!path)
		fd = 0;
	else	if (0 > (fd = open (path, 0))) {
			fprintf (stderr, "%s: %s: can't open\n", ego, path);
			return (1);
		}


	for (;;)
		if (0 >= (n = read (fd, buf, BUFSIZE))) {
			(void) close (fd);
			if (n < 0) {
				fprintf (stderr, "%s: %s: read error\n",
				    ego, path? path: "<standard input>");
				return (1);
			}
			/* should produce same format as old version always */
			printf ("%05u%6ld", sum,
				(nbytes + (BLOKSIZE - 1)) / BLOKSIZE);
			if (path)
				printf(" %s", path);
			putchar('\n');
			fflush(stdout);
			return (0);
		}
		else	for (nbytes += n, s = buf; n; n--, s++)
				if (sum & 1)
					sum = 0xffff & ((sum>>1) + *s + 0x8000);
				else	sum = 0xffff & ((sum>>1) + *s);
}
