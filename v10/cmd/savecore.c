#include <stdio.h>
#include <libc.h>
#include <errno.h>
#include <fstab.h>	/* only for compatibility hack/guess */

/*
 * dump header.  manifest in the system;
 * address and magic number are guaranteed not to change
 */

#define	MAGICADDR 0x20		/* where the header always will be */
#define	DUMPMAG	0x79646e61	/* magic number */
#define	DUMPBAD	0		/* not the magic number */
#define	DUMPBLK	512		/* dump size in these units */
struct dumpmagic {
	long magic;
	long time;		/* when the dump was written */
	long len;		/* 512-byte blocks in dump */
};

struct dumpmagic *magic;
struct dumpmagic *dumpdata();
char *outfile, *infile;		/* only so we can print them */

#define	NBLKS	16		/* blocks of dump to read at once */

main(argc, argv)
int argc;
char **argv;
{
	char *writedir;
	char *readname;
	int fr, to;

	if (argc < 2) {
		fprintf(stderr, "usage: savecore target dump\n");
		exit(1);
	}
	setbuf(stdout, NULL);
	writedir = argv[1];
	readname = NULL;
	if (argc > 2)
		readname = argv[2];
	if ((fr = opendump(readname)) < 0)
		exit(2);		/* nothing to copy */
	if ((to = openout(writedir)) < 0)
		exit(1);
	printf("%s %s: %ld bytes of crash dump from %s",
		outfile, infile, magic->len*DUMPBLK, ctime(&magic->time));
	if (copydump(fr, to))
		dumpinval(fr);
	copysys();
	exit(0);
}

/*
 * open an output file:
 * if it exists and is a directory, make a new file there
 * if it doesn't, or is a regular file, just copy it to that name
 */

openout(name)
char *name;
{
	int fd;
	int i;
	static char namebuf[20];

	if ((fd = creat(name, 0666)) >= 0) {
		outfile = name;
		return (fd);
	}
	if (errno != EISDIR) {
		perror(name);
		return (-1);
	}
	if (chdir(name) < 0) {
		perror(name);
		return (-1);
	}
	for (i = 0; ; i++) {
		sprintf(namebuf, "z.%d", i);
		if (access(namebuf, 0) < 0)
			break;
	}
	if ((fd = creat(namebuf, 0666)) >= 0) {
		outfile = namebuf;
		return (fd);
	}
	perror(namebuf);
	return (-1);
}

/*
 * copy /unix to a place in the directory,
 * with a suffix matching the dump we just wrote
 * if output was just a filename, don't bother.
 * why bother anyway?
 */
copysys()
{
}

/*
 * open the crash dump
 * if no name was given, make a guess:
 * try the swap devices in /etc/fstab
 * if that doesn't work, guess at the default swap device.
 * the dogma is that the name should be supplied anyway,
 * and the guesses are for compatibility
 */
char *swapguess[] = { "/dev/ra01", "/dev/hp01", "/dev/em01", NULL };

int
opendump(name)
char *name;
{
	int fd;
	register struct fstab *fp;
	register int i;

	if (name) {
		if ((fd = open(name, 2)) < 0) {
			perror(name);
			return (-1);
		}
		if ((magic = dumpdata(fd)) == NULL) {
			close(fd);
			return (-1);
		}
		infile = name;
		return (fd);
	}
	/*
	 * the hard part
	 */
	printf("I can guess your dump device!\n");
	while ((fp = getfsent()) != NULL) {
		if (fp->fs_ftype != FSSWAP)
			continue;
		if ((fd = open(fp->fs_spec, 2)) < 0)
			continue;
		if ((magic = dumpdata(fd)) == NULL) {
			printf("Is it %s?  No.\n", fp->fs_spec);
			close(fd);
			continue;
		}
		printf("It's %s.\n", fp->fs_spec);
		infile = fp->fs_spec;
		return (fd);
	}
	for (i = 0; swapguess[i]; i++) {
		if ((fd = open(swapguess[i], 2)) < 0)
			continue;
		if ((magic = dumpdata(fd)) == NULL) {
			printf("Is it %s?  No.\n", swapguess[i]);
			close(fd);
			continue;
		}
		printf("It's %s.\n", swapguess[i]);
		infile = swapguess[i];
		return (fd);
	}
	printf("I give up!\n");
	return (-1);
}

/*
 * copy the crash dump
 */
copydump(fr, to)
int fr, to;
{
	char buf[NBLKS*DUMPBLK];
	register int n, r;
	register long b;

	lseek(fr, (long)0, 0);
	for (b = magic->len * DUMPBLK; b > 0; b -= n) {
		if (b > sizeof(buf))
			r = sizeof(buf);
		else
			r = b;
		if ((n = read(fr, buf, r)) <= 0) {
			if (n < 0)
				perror("read dump");
			else
				fprintf(stderr, "premature EOF on dump: %ld missing\n", b);
			return (1);	/* accept it anyway */
		}
		if (write(to, buf, n) != n) {
			perror("write dump");
			return (0);
		}
	}
	return (1);
}

static char hbuf[DUMPBLK];

/*
 * read the header out of a dump,
 * and check it for validity
 */
struct dumpmagic *
dumpdata(fd)
int fd;
{
	register struct dumpmagic *mp;

	if (lseek(fd, 0L, 0) < 0)
		return (NULL);
	if (read(fd, hbuf, sizeof(hbuf)) != sizeof(hbuf))
		return (NULL);
	mp = (struct dumpmagic *)&hbuf[MAGICADDR];
	if (mp->magic != DUMPMAG)
		return (NULL);
	return (mp);
}

/*
 * scribble on the header,
 * so we won't read this dump again
 */
dumpinval(fd)
int fd;
{

	((struct dumpmagic *)&hbuf[MAGICADDR])->magic = DUMPBAD;
	lseek(fd, 0L, 0);
	if (write(fd, hbuf, sizeof(hbuf)) != sizeof(hbuf))
		perror("taint header");
}
