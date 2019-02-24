/*
 * process client requests
 */

#include "faceproto.h"
#include "faces.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>		/* just for NULL */

/*
 * process one request
 * returns 1 if all is well,
 * 0 if client communication fell down
 */
dorequest(fd)
int fd;
{
	char msg[F_DATA+FMAXDATA+1];
	register unsigned char *p;
	int n, len;
	File *f;
	long off;
	int bsize;

	p = (unsigned char *)msg;
	if ((n = gread(fd, msg, F_DATA)) != F_DATA) {	/* read header */
		if (n != 0)		/* don't fuss about EOF */
			log("fd %d: bad header: read %d\n", fd, n);
		return (0);
	}
	len = frfshort(p, F_LEN);
	if (len < 0 || len > FMAXDATA) {
		log("fd %d: ill message len %d\n", fd, len);
		return (0);
	}
	if (len && (n = gread(fd, msg+F_DATA, len)) != len) {
		log("fd %d: bad data read: want %d got %d\n", fd, len, n);
		return (0);
	}
	msg[F_DATA + len] = 0;
	switch(p[F_TYPE]) {
	case DOSTAT:
		if ((f = lookfile(p + F_DATA)) == NULL
		||  dostat(f) < 0) {
			toflong(p, F_P1, -1);
			len = 0;
		} else {
			copystat(f, p + F_DATA);
			toflong(p, F_P1, 0);
			len = STLEN;
		}
		break;

	case DOREAD:
		off = frflong(p, F_P1);
		bsize = frflong(p, F_P2);
		if (bsize <= 0 || bsize > FMAXDATA
		||  (f = lookfile(p + F_DATA)) == NULL
		||  (len = doread(f, msg + F_DATA, bsize, off)) < 0) {
			toflong(p, F_P1, -1);
			len = 0;
		} else
			toflong(p, F_P1, len);
		break;

	default:
		log("fd %d: ill msg %d\n", fd, p[F_TYPE]);
		return (0);
	}
	tofshort(p, F_LEN, len);
	len += F_DATA;		/* header */
	if ((n = write(fd, msg, len)) != len) {
		log("fd %d: write %d returned %d\n", fd, len, n);
		return (0);
	}
	return (1);
}

dostat(f)
register File *f;
{
	struct stat st;

	if (isdir(f))
		f->size = f->nfiles * FDLEN;
	else {
		if (stat(f->data, &st) < 0)
			return (-1);
		f->size = st.st_size;
		f->ta = st.st_atime;
		f->tm = st.st_mtime;
		f->tc = st.st_ctime;
	}
	return (0);
}

copystat(f, s)
register File *f;
register unsigned char *s;
{

	tofshort(s, ST_DEV, 0);		/* junk */
	tofshort(s, ST_INO, f->ino);
	if (isdir(f))
		tofshort(s, ST_MODE, STDIR|0555);
	else
		tofshort(s, ST_MODE, 0444);
	tofshort(s, ST_NLINK, f->nlinks);
	tofshort(s, ST_UID, 0);		/* junk */
	tofshort(s, ST_GID, 0);		/* junk */
	tofshort(s, ST_RDEV, 0);	/* junk */
	toflong(s, ST_SIZE, f->size);
	toflong(s, ST_ATIME, f->ta);
	toflong(s, ST_MTIME, f->tm);
	toflong(s, ST_CTIME, f->tc);
}

/*
 * reading regular files might be sped up
 * by caching the file descriptor somewhere
 */
doread(f, buf, len, off)
register File *f;
char *buf;
int len;
long off;
{
	int fd;

	if (isdir(f)) {
		if (f->data == NULL)
			dirdata(f);
		if (off + len > f->size)
			len = f->size - off;
		if (len < 0 || off < 0)
			return (0);
		memcpy(buf, f->data + off, len);
		return (len);
	}
	if ((fd = open(f->data, 0)) < 0) {
		log("%s: cannot open\n", f->data);
		return (-1);
	}
	lseek(fd, off, 0);
	len = read(fd, buf, len);
	close(fd);
	return (len);
}

/*
 * gather data that may come in dribs and drabs
 */

int
gread(fd, buf, size)
int fd;
char *buf;
int size;
{
	register int n, tot;

	tot = 0;
	while (size > 0) {
		if ((n = read(fd, buf, size)) <= 0)
			break;
		buf += n;
		size -= n;
		tot += n;
	}
	if (tot)
		return (tot);
	return (n);
}
