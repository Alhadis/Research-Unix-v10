#include <stdio.h>
#include <fstab.h>

struct mtab *mtab;

int nmtab;
int mdirty;
int errs;

char mfile[] = MTAB;

main(argc, argv)
int argc;
char **argv;
{

	if (argc < 2) {
		fprintf(stderr, "usage: umount [-a] [file ...]\n");
		exit(1);
	}
	readmtab();
	sync();
	if (argc == 2 && strcmp(argv[1], "-a") == 0)
		umountall();
	else {
		while (--argc > 0)
			umountone(*++argv);
	}
	writmtab();
	sync();
	exit(errs);
}

/*
 * unmount everything
 * in reverse order in case of nesting
 */
umountall()
{
	register int i;

	for (i = nmtab - 1; i >= 0; i--) {
		if (mtab[i].file[0] == 0)
			continue;
		if (funmount(mtab[i].file) < 0) {
			perror(mtab[i].file);
			errs++;
			continue;
		}
		mtab[i].file[0] = 0;
		mdirty++;
	}
}

umountone(f)
char *f;
{
	register int i;

	if (funmount(f) < 0) {
		perror(f);
		errs++;
		return;
	}
	for (i = nmtab - 1; i >= 0; i--)
		if (strcmp(mtab[i].file, f) == 0) {
			mtab[i].file[0] = 0;
			mdirty++;
			break;
		}
}

#define	CHUNK	(20*sizeof(struct mtab))

readmtab()
{
	char *base, *buf;
	int n, nr, tot;
	int fd;
	char *malloc(), *realloc();

	if ((base = malloc(CHUNK)) == NULL) {
		fprintf(stderr, "umount: no mem\n");
		exit(1);
	}
	if ((fd = open(mfile, 0)) < 0) {
		perror("read /etc/mtab");
		exit(1);
	}
	buf = base;
	nr = CHUNK;
	tot = 0;
	while ((n = read(fd, buf, nr)) > 0) {
		tot += n;
		buf += n;
		if ((nr -= n) <= 0) {
			if ((base = realloc(base, CHUNK)) == NULL) {
				fprintf(stderr, "umount: no mem\n");
				exit(1);
			}
			buf = base + tot;
			nr = CHUNK;
		}
	}
	mtab = (struct mtab *)base;
	nmtab = tot / sizeof(struct mtab);
	close(fd);
}

writmtab()
{
	int fd;
	register int i;

	if (mdirty == 0)
		return;
	for (i = nmtab - 1; i >= 0; i--)
		if (mtab[i].file[0])
			break;
	if ((fd = creat(mfile, 0666)) < 0) {
		perror("write /etc/mtab");
		errs++;
		return;
	}
	i = (char *)&mtab[i+1] - (char *)mtab;
	if (write(fd, (char *)mtab, i) != i) {
		perror("write /etc/mtab");
		errs++;
		return;
	}
	close(fd);
}
