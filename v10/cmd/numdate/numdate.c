/*
 *	numdate -- command interface to getdate()
 */

#include <stdio.h>
#include <sys/types.h>

char *malloc();

main (argc, argv)
	int argc;
	char **argv;
{
	register char *buf, *p, *arg;
	register int i, n;
	time_t t, getdate();

	/* calculate the length of the date string */
	n = 1;
	for (i = 1; i < argc; i++)
		n += strlen (argv[i]) + 1;

	/* allocate memory for the string */
	buf = malloc (n);
	if (buf == NULL) {
		fprintf (stderr, "numdate: out of memory\n");
		exit (1);
	}

	/* build the string */
	p = buf;
	for (i = 1; i < argc; i++) {
		arg = argv[i];
		while (*arg)
			*p++ = *arg++;
		if (i < argc - 1)
			*p++ = ' ';
	}
	*p++ = '\0';

	/* attempt to parse it */
	t = getdate (buf, (time_t) 0);
	free (buf);

	/* failed? */
	if (t == -1) {
		fprintf (stderr, "numdate: bad argument\n");
		exit (1);
	}

	/* success! */
	printf ("%ld\n", t);

	return 0;
}
