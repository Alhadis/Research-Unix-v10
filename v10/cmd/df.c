#include <stdio.h>
#include <fstab.h>
#include <libc.h>
#include <sys/param.h>
#include <sys/filsys.h>
#include <sys/stat.h>
/*
 * df
 */

#define NFS	64	/* Max number of filesystems */

struct mtab mtab[NFS];
int nmtab;
#define	L10BS	7
#define	L10IS	6
#define PCTFW	3
int	DEVNMLG = 3;		/* length of longest device name */
int	DIRNMLG = 3;		/* length of longest mount point name */

#define	BUNIT	1024		/* report in units of this size */

char *mpath();

int	iflag;
struct	filsys sblock;
int	fi;

main(argc, argv)
register int argc;
register char **argv;
{
	register int i;

	nochk(1,0);	/* for secure unix */
	nochk(2,0);
	while (argc >= 1 && argv[1][0]=='-') {
		switch(argv[1][1]) {

		case 'i':
			iflag++;
			break;

		default:
			fprintf(stderr, "usage: df [-i] [filsys...]\n");
			exit(0);
		}
		argc--, argv++;
	}
	readtab();
	printf("%-*.*s %-*.*s %*.*s %*.*s %*.*s",
		DIRNMLG, DIRNMLG, "dir",
		DEVNMLG, DEVNMLG, "dev",
		L10BS, L10BS, "kbytes",
		L10BS, L10BS, "used",
		L10BS, L10BS, "free");
	printf(" %*.*s", PCTFW + 1, PCTFW + 1, "%use");
	if (iflag)
		printf(" %*.*s %*.*s %*.*s",
			L10IS, L10IS, "iused",
			L10IS, L10IS, "ifree",
			PCTFW + 1, PCTFW + 1, "%ino");
	putchar('\n');
	if(argc <= 1) {
		for (i = 0; i < nmtab; ++i)
			dfree(mtab[i].spec);
		exit(0);
	}
	for(i=1; i<argc; i++)
		dfree(argv[i]);
	exit(0);
}

dfree(file)
char *file;
{
	register daddr_t i;
	long	blocks;
	long	free;
	long	used;
	struct	stat stbuf;
	dev_t dev;
	char *spec;

	if (stat(file, &stbuf) < 0) {
		perror(file);
		return;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFBLK)
		spec = file;
	else {
		dev = stbuf.st_dev;
		for (i = 0; i < nmtab; i++) {
			spec = mtab[i].spec;
			if (stat(spec, &stbuf) < 0)
				continue;
			if (stbuf.st_rdev == dev)
				break;
		}
		if (i == nmtab) {
			fprintf(stderr, "%s: can't find filesystem\n", file);
			return;
		}
	}
	if((fi = open(spec, 0)) < 0) {
		perror(spec);	/* but we already did a stat! */
		return;
	}
	lseek(fi, SUPERB*BSIZE(stbuf.st_rdev), 0);
	if (read(fi, (char *)&sblock, sizeof(sblock)) != sizeof(sblock)) {
		perror(spec);
		memset((char *)&sblock, 0, sizeof(sblock));
	}
	blocks = (long) sblock.s_fsize - (long)sblock.s_isize;
	free = sblock.s_tfree;
	used = blocks - free;
	blocks *= BSIZE(stbuf.st_rdev) / BUNIT;
	free *= BSIZE(stbuf.st_rdev) / BUNIT;
	used *= BSIZE(stbuf.st_rdev) / BUNIT;
	printf("%-*.*s %-*.*s %*ld %*ld %*ld",
		DIRNMLG, DIRNMLG, mpath(spec),
		DEVNMLG, DEVNMLG, spec,
		L10BS, blocks, L10BS, used, L10BS, free);
	printf(" %*.0f%%", 
		PCTFW, blocks == 0 ?
		0.0 : (double) used / (double) blocks * 100.0);
	if (iflag) {
		int inodes = (sblock.s_isize - 2) * INOPB(stbuf.st_rdev);
		used = inodes - sblock.s_tinode;
		printf(" %*ld %*ld %*.0f%%",
			L10IS, used,
			L10IS, sblock.s_tinode, 
			PCTFW, inodes == 0 ?
			0.0 : (double) used / (double) inodes * 100.0);
	}
	printf("\n");
	close(fi);
}

/*
 * Given a name like /dev/rp0h, returns the mount point, like /usr.
 */
char *mpath(spec)
char *spec;
{
	register int i;

	for (i=0; i<nmtab; i++)
		if (strcmp(spec, mtab[i].spec) == 0)
			return mtab[i].file;
	return "";
}

/*
 * hack: uninteresting mtab slots (empty or not type 0)
 * always sort to the end.  they were also omitted from the count.
 * hence they are ignored.
 */
mtabcmp(mp0, mp1)
struct mtab *mp0;
struct mtab *mp1;
{
	if (mp0->file[0] == 0 || mp0->type != 0)
		return (1);
	if (mp1->file[0] == 0 || mp1->type != 0)
		return (-1);
	return (strncmp(mp0->file, mp1->file, sizeof (mp0->file)));
}

/*
 * read the list of filesystems, and sort it
 * assumes type 0 is the only interesting filesystem
 */
readtab()
{
	register int i, n;
	int f;
	register struct fstab *fsp;

	mtab[0].file[0] = '/';
	/*
	 * cheap hack because root isn't in mtab
	 */
	if ((fsp = getfsfile(mtab[0].file)) != NULL)
		strcpy(mtab[0].spec, fsp->fs_spec);
	if ((f = open(MTAB, 0)) < 0) {
		nmtab = 1;
		return;
	}
	n = read(f, (char *)&mtab[1], sizeof(mtab)-sizeof(mtab[0]));
	close(f);
	n /= sizeof(mtab[0]);
	nmtab = 0;
	for (i = 0; i <= n; i++) {
		if (mtab[i].file[0] == 0 || mtab[i].type != 0)
			continue;
		nmtab++;
		if (DEVNMLG < (f = strlen(mtab[i].spec)))
			DEVNMLG = f;
		if (DIRNMLG < (f = strlen(mtab[i].file)))
			DIRNMLG = f;
	}
	qsort((char *)&mtab[0], n+1, sizeof(mtab[0]), mtabcmp);
}
