/* For debugging, read ASCII numbers on stdin, write them in
 * compressed and expanded form on stdout.  Numbers without
 * a decimal point are taken to be unsigned integers.
 */

#include <stdio.h>

char *
nonbln(s)
char *s;
{
	char c;
	if (s)
		for(; c = *s; s++)
			if (c > ' ') return s;
	return 0;
	}

char *
nextbln(s)
char *s;
{
	while(*s > ' ') s++;
	return s;
	}

 char *
Alloc(n)
 int n;
{
	extern char *malloc();
	register char *rv;
	rv = malloc(n);
	if (!rv) {
		fprintf(stderr, "malloc(%d) failure!\n", n);
		exit(1);
		}
	return rv;
	}

fatal(msg)
 char *msg;
{
	fprintf(stderr, "*** %s ***\n", msg);
	exit(1);
	}

 static char *memlast;

 char *
mem(n)
 int n;
{
	static char buf[512];
	if (n > sizeof(buf)) {
		fprintf(stderr, "mem(%d) failure!\n", n);
		exit(1);
		}
	memlast = buf+n;
	memset(buf, '?', n);
	return buf;
	}

main()
{
	char buf[1024], c, *s, *s1, *z;
	extern char *cds();

	while(s = fgets(buf,sizeof(buf),stdin)) {
		while(s = nonbln(s)) {
			s1 = nextbln(s);
			c = *s1;
			*s1 = 0;
			z = cds(s,0);
			*memlast = '?';
			printf("%s --> %s (len %d)\n",
					s, z, strlen(z));
			if (memlast[-1])
				printf("expected memlast[-1] = 0, but got %c\n",
					memlast[-1]);
			if (z[0] != '-' && z[-1] != '-')
				printf("z[0] = %c, z[-1] = %c, and neither is -\n",
					z[0],z[-1]);
			*s1 = c;
			s = s1;
			}
		}
	}
