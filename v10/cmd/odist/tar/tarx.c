#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
#include <libv.h>
#include "tar.h"
#include "misc.h"

#ifndef S_ISDIR
#define S_ISDIR(M) (((M) & S_IFMT) == S_IFDIR)
#endif

#define MIN(A, B) ((A) < (B) ? (A) : (B))

int fflag;
int tflag;
int vflag;

int nerr;

struct chtmug {
	char *path;
	time_t mtime;
	int mode;
	int uid;
	int gid;
	struct chtmug *next;
};

struct chtmug *chtmuglist;

void
chtmug(char *path, time_t mtime, int mode, int uid, int gid)
{
	struct utimbuf ut;

	time(&ut.actime);
	ut.modtime = mtime;
	utime(path, &ut);
	chmod(path, mode);
	chown(path, uid, gid);
	chmod(path, mode);	/* fix possibly cleared s[ug]id bits */
}

void
chtmugs(void)
{
	struct chtmug *ch;

	/* we handle the chtmug list in the reverse order of which it
	   was built, which is just right for nested directories created
	   from tar files */
	for (ch = chtmuglist; ch; ch = ch->next)
		chtmug(ch->path, ch->mtime, ch->mode, ch->uid, ch->gid);
}

void
mkchtmug(char *path, time_t mtime, int mode, int uid, int gid)
{
	struct chtmug *ch;

	ch = xmalloc(sizeof (struct chtmug));
	ch->path = xstrdup(path);
	ch->mtime = mtime;
	ch->mode = mode;
	ch->uid = uid;
	ch->gid = gid;
	ch->next = chtmuglist;
	chtmuglist= ch;
}

void
mkdirs(char *path)
{
	char *slash;

	slash = strrchr(path, '/');
	if (slash == path)
		return;
	if (slash) {
		*slash = '\0';
		if (access(path, F_OK) != 0) {
			mkdirs(path);
			mkdir(path, 0777);
		}
		*slash = '/';
	}
}

int
isdir(char *path)
{
	struct stat st;

	if (stat(path, &st) == 0)
		return S_ISDIR(st.st_mode);
	return 0;
}

int
extract(FILE *ifp)
{
	struct tarbuf buf;
	struct tarhdr hdr;
	int fd, blocks;
	FILE *ofp;
	static char zeroes[TSIZE];

	if (fread(&buf, sizeof buf, 1, ifp) != 1)
		return 0;
	if (memcmp(&buf, zeroes, sizeof zeroes) == 0)
		return 0;
	if (thdrget(&hdr, &buf) != 0) {
		++nerr;
		fprintf(stderr, "tarx: bad input header\n");
		return 0;
	}
	if (tflag)
		printf("%s\n", hdr.name);
	if (vflag)
		fprintf(stderr, "%s\n", hdr.name);
	mkdirs(hdr.name);
	switch (hdr.typeflag) {
	case REGTYPE:
		blocks = (hdr.size + TSIZE - 1) / TSIZE;
		fd = creat(hdr.name, hdr.mode);
		if (fd < 0) {
			remove(hdr.name);
			fd = creat(hdr.name, hdr.mode);
			if (fd < 0) {
				++nerr;
				fprintf(stderr, "tarx: can't create '%s'\n",
					hdr.name);
				discard(ifp, blocks * TSIZE);
				return 1;
			}
		}
		ofp = fdopen(fd, "w");
		fpcopy(ofp, ifp, hdr.size);
		fclose(ofp);
		close(fd);	/* paranoia */
		discard(ifp, blocks * TSIZE - hdr.size);
		chtmug(hdr.name, hdr.mtime, hdr.mode, hdr.uid, hdr.gid);
		break;

	case DIRTYPE:
		mkchtmug(hdr.name, hdr.mtime, hdr.mode, hdr.uid, hdr.gid);
		if (!isdir(hdr.name)) {
			remove(hdr.name);
			if (mkdir(hdr.name, hdr.mode) < 0) {
				++nerr;
				fprintf(stderr, "tarx: can't mkdir '%s'\n",
					hdr.name);
				return 1;
			}
		}
		break;

	case LNKTYPE:
		remove(hdr.name);
		if (link(hdr.linkname, hdr.name) < 0) {
			++nerr;
			fprintf(stderr, "tarx: can't link '%s' to '%s'\n",
				hdr.linkname, hdr.name);
			return 1;
		}
		break;

	case SYMTYPE:
		remove(hdr.name);
		if (symlink(hdr.linkname, hdr.name) < 0) {
			++nerr;
			fprintf(stderr, "tarx: can't symlink '%s' to '%s'\n",
				hdr.linkname, hdr.name);
			return 1;
		}
		break;

	default:
		fprintf(stderr, "tarx: unknown typeflag '%c'\n", hdr.typeflag);
		return 1;
	}
	return 1;
}

void
usage(void)
{
	fprintf(stderr, "usage: tarx [-ftv]\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	int c, i;
	prog = argv[0];

	while ((c = getopt(argc, argv, "ftv")) != EOF)
		switch (c) {
		case 'f':
			++fflag;
			break;
		case 't':
			++tflag;
			break;
		case 'v':
			++vflag;
			break;
		default:
			usage();
			break;
		}
	if (optind != argc)
		usage();
	if (fflag) {
		setbuf(stdin, NULL);
		extract(stdin);
	} else
		while (extract(stdin))
			;
	chtmugs();
	if (ferror(stdin))
		return 1;
	return nerr ? 1 : 0;
}
