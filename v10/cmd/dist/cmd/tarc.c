#define _POSIX_SOURCE
#define _RESEARCH_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libv.h>
#include "../lib/ftwlk.h"
#include "../lib/lib.h"

#define MIN(A, B) ((A) < (B) ? (A) : (B))

int qflag;
int vflag;

int nerr;

struct hardlink {
	char *name;
	int ino;
	int dev;
	struct hardlink *next;
};

struct hardlink *links;

struct hardlink *
findlink(char *name, int ino, int dev)
{
	struct hardlink *h;

	for (h = links; h; h = h->next)
		if (ino == h->ino && dev == h->dev)
			return h;
	h = xmalloc(sizeof (struct hardlink));
	h->name = xstrdup(name);
	h->ino = ino;
	h->dev = dev;
	h->next = links;
	links = h;
	return 0;	/* make a new entry, but say we didn't find it */
}

#ifndef S_ISREG
#define S_ISREG(M) (((M) & S_IFMT) == S_IFREG)
#endif

void
zeropad(long from, long to)
{
	static char zero[512];
	static int n;

	while (from < to) {
		n = MIN(to - from, sizeof zero);
		if (fwrite(zero, 1, n, stdout) != n) {
			fprintf(stderr, "tarc: zero padding failed\n");
			exit(1);
		}
		from += n;
	}
}

int
dofile(char *name, struct stat *st, int code, struct FTWLK *f)
{
	struct tarhdr hdr;
	struct tarbuf buf;
	struct hardlink *hl;
	FILE *fp;
	long cc;
	char qname[4 * TNAMEMAX + 1];

	if (code == FTWLK_NSL || code == FTWLK_SL)
		lstat(name, st);	/* fix ftwlk braindamage */

	memset(&hdr, 0, sizeof hdr);
	if (strlen(name) > TNAMEMAX) {
		fprintf(stderr, "tarc: file name too long '%s'\n", name);
		exit(1);
	}

	strcpy(hdr.name, name);
	hdr.mode = st->st_mode & TMASK;
	hdr.uid = st->st_uid;
	hdr.gid = st->st_gid;
	hdr.size = st->st_size;
	hdr.mtime = st->st_mtime;

	switch (code) {
	case FTWLK_D:
		if (strlen(name) <= TNAMEMAX - 1)
			strcat(hdr.name, "/");
		hdr.typeflag = DIRTYPE;
		break;

	case FTWLK_F:
		if (st->st_nlink > 1
		&& (hl = findlink(name, st->st_ino, st->st_dev))
		&& strlen(hl->name) <= TLINKMAX) {
			hdr.typeflag = LNKTYPE;
			strcpy(hdr.linkname, hl->name);
		} else if (S_ISREG(st->st_mode))
			hdr.typeflag = REGTYPE;
		else {
			fprintf(stderr, "tarc: not a regular file '%s'\n",
				name);
			++nerr;
			return 0;
		}
		break;

	case FTWLK_SL:
		if (f->level == 0) {
			f->quit = FTWLK_FOLLOW;
			return 0;
		}
		hdr.typeflag = SYMTYPE;
		cc = readlink(name, hdr.linkname, sizeof hdr.linkname);
		if (cc < 0) {
			fprintf(stderr, "tarc: can't read symlink '%s'\n",
				name);
			++nerr;
			return 0;
		}
		break;

	default:
		return 0;
	}

	if (hdr.typeflag != REGTYPE)
		hdr.size = 0;
	if (qflag)
		quote(qname, hdr.name);
	else
		strcpy(qname, hdr.name);
	if (vflag)
		fprintf(stderr, "%s\n", qname);
	if (thdrput(&buf, &hdr) != 0) {
		fprintf(stderr, "tarc: failed making header for '%s'\n", name);
		++nerr;
		return 0;
	}
	if (hdr.typeflag == REGTYPE && !(fp = fopen(name, "r")))
		return 0;
	if (fwrite(&buf, sizeof buf, 1, stdout) != 1) {
		fprintf(stderr, "tarc: write failed\n");
		exit(1);
	}
	if (hdr.typeflag == REGTYPE) {
		cc = fpcopy(stdout, fp, st->st_size);
		if (cc < 0) {
			fprintf(stderr, "tarc: file copy failed '%s'\n", name);
			exit(1);
		}
		if (cc != st->st_size)
			fprintf(stderr, "tarc: warning: size change '%s'\n",
				name);
		zeropad(cc, (st->st_size + TSIZE - 1) / TSIZE * TSIZE);
		fclose(fp);
	}
	return 0;
}

void
usage(void)
{
	fprintf(stderr, "usage: tarc [-qv] files...\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	int i, c;

	prog = argv[0];
	while ((c = getopt(argc, argv, "qv")) != EOF)
		switch (c) {
		case 'q':
			++qflag;
			break;
		case 'v':
			++vflag;
			break;
		default:
			usage();
			break;
		}
	for (i = optind; i < argc; ++i)
		ftwlk(argv[i], dofile, 10);
	zeropad(0, 2 * TSIZE);
	return nerr ? 1 : 0;
}
