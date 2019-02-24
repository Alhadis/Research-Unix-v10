/*
 *	Print the system load average,
 *	and the time of day.
 */

#include <stdio.h>
#include <nlist.h>

struct nlist nl[] = {
	{"_avenrun"},
	{0}
};

double avenrun[3];

main(argc,argv)
char **argv;
{
	register int i,kmem;
	long maddr;
	long t;
	int count = -1, interval = 0;
	register char *dunix = "/unix", *dkmem = "/dev/kmem";
	extern char _sobuf[BUFSIZ];

	setbuf (stdout, _sobuf);

	switch (argc) {
	case 5:
		dkmem = argv[4];
		/* no break */
	case 4:
		dunix = argv[3];
	case 3:
		count = atoi (argv[2]);
	case 2:
		interval = atoi (argv[1]);
	case 1:
		break;
	default:
		err("usage: load [interval [count  [/unix  [/dev/kmem]]]]");
	}
	
	nlist(dunix, nl);
	if (nl[0].n_type==0) err("no namelist");
	if ((kmem=open(dkmem,0))<0) err("no kmem");
	if (0==strcmp(dkmem,"/dev/kmem")) maddr=0;
	else maddr=0x80000000;
	time (&t);
	printf ("    1m    5m   15m\n");

	do {
		register int i;

		kseek(kmem,nl[0].n_value,0,maddr);
		kread(kmem,avenrun,sizeof(avenrun));

		for (i = 0; i < sizeof(avenrun)/sizeof(*avenrun); i++)
			printf ("%6.1f", avenrun[i]);
		putchar ('\n');
		fflush (stdout);

		if (count && interval)
			sleep (interval);
	} while (--count && interval);
}

err(why)
char *why;
{
	extern int errno;
	fprintf (stderr,"load: %s",why);
	if (errno)
		fprintf (stderr, " (%d)", errno);
	fprintf (stderr, "\n");
	exit(2);
}

kseek(fd,offset,whence,maddr)
long offset;
{
	if (-1==lseek(fd,offset&~maddr,whence)) err("kmem seek error");
}

kread(fd,buf,siz)
char *buf;
{
	if (siz!=read(fd,buf,siz)) err("kmem read error");
}
