#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lib.h"

static int
chksum(struct tarbuf *buf)
{
	char *p;
	int sum;

	p = (char *) buf;
	for (sum = 0; p < buf->chksum; ++p)
		sum += *(unsigned char *) p;
	sum += ' ' * sizeof buf->chksum;
	for (p = buf->chksum + sizeof buf->chksum; p < (char *) (buf + 1); ++p)
		sum += *(unsigned char *) p;
	return sum;
}

/* opposite of strncpy */
static void
cpystrn(char *dst, char *src, int n)
{
	while (n-- && (*dst++ = *src++))
		;
	*dst = '\0';
}

static int
getoct(unsigned long *dst, char *src, int n)
{
	*dst = 0;
	while (n && *src == ' ')
		++src, --n;
	while (n && *src >= '0' && *src <= '7') {
		*dst = 8 * *dst + *src - '0';
		++src, --n;
	}
	while (n) {
		if (*src && *src != ' ')
			return 1;
		++src, --n;
	}
	return 0;
}

static void
putoct(char *dst, unsigned long val, int n)
{
	char temp[24], *sp;

	sp = temp;
	do {	/* at least one digit */
		*sp++ = val % 8 + '0';
		val /= 8;
	} while (val);
	while ((n - 1) - (sp - temp))
		*dst++ = ' ', --n;
	while (sp > temp)
		*dst++ = *--sp, --n;
	while (n--)
		*dst++ = ' ';
}

int
thdrget(struct tarhdr *hdr, struct tarbuf *buf)
{
	int nerr, len, id, bufsum, mysum;
	char name[sizeof buf->uname], *np;
	unsigned long otmp;

	nerr = 0;
	memset(hdr, 0, sizeof *hdr);
	cpystrn(hdr->name, buf->prefix, sizeof buf->prefix);
	if (len = strlen(hdr->name))
		hdr->name[len++] = '/';
	cpystrn(hdr->name + len, buf->name, sizeof buf->name);
#define OCT(V, S) nerr += getoct(&otmp, S, sizeof S), V = otmp
	OCT(hdr->mode, buf->mode);
	OCT(hdr->uid, buf->uid);
	OCT(hdr->gid, buf->gid);
	OCT(hdr->size, buf->size);
	OCT(hdr->mtime, buf->mtime);
	OCT(bufsum, buf->chksum);
	mysum = chksum(buf);
	nerr += bufsum != mysum;
	hdr->typeflag = buf->typeflag[0];
	cpystrn(hdr->linkname, buf->linkname, sizeof buf->linkname);
	len = strlen(hdr->name);
	if (memcmp(buf->magic, TMAGIC, TMAGLEN) == 0
	&& memcmp(buf->version, TVERSION, TVERSLEN) == 0) {
		if (hdr->typeflag == DIRTYPE && hdr->name[len - 1] != '/')
			hdr->name[len++] = '/', hdr->name[len] = '\0';
		cpystrn(name, buf->uname, sizeof buf->uname - 1);
		id = uidnum(name);
		if (id != -1)
			hdr->uid = id;
		cpystrn(name, buf->gname, sizeof buf->gname - 1);
		id = gidnum(name);
		if (id != -1)
			hdr->gid = id;
		OCT(hdr->devmajor, buf->devmajor);
		OCT(hdr->devminor, buf->devminor);
	} else {
		if (hdr->name[len - 1] == '/')
			hdr->typeflag = DIRTYPE;
		np = uidstr(hdr->uid);
		if (np)
			cpystrn(hdr->uname, np, sizeof buf->uname - 1);
		np = gidstr(hdr->gid);
		if (np)
			cpystrn(hdr->gname, np, sizeof buf->gname - 1);
		hdr->devmajor = 0;
		hdr->devminor = 0;
	}
	if (hdr->typeflag == AREGTYPE)
		hdr->typeflag = REGTYPE;
#undef OCT
	return nerr;
}

int
thdrput(struct tarbuf *buf, struct tarhdr *hdr)
{
	int npref, mysum;
	char *slash, *s;

	memset(buf, 0, sizeof *buf);
	if (strlen(hdr->name) > sizeof buf->name) {
		/* split on a '/' */
		npref = 0;
		while (slash = strchr(hdr->name + npref + 1, '/')) {
			npref = slash - hdr->name;
			if (strlen(slash + 1) <= sizeof buf->name)
				break;
		}
		if (!slash || npref > sizeof buf->prefix)
			return 1;
		strncpy(buf->name, slash + 1, sizeof buf->name);
		*slash = '\0';		/* evil */
		strncpy(buf->prefix, hdr->name, sizeof buf->prefix);
		*slash = '/';
	} else
		strncpy(buf->name, hdr->name, sizeof buf->name);
	if (hdr->uname[0])
		hdr->uid = uidnum(hdr->uname);
	else if (s = uidstr(hdr->uid)) {
		hdr->uname[TUGNAMEMAX] = '\0';
		strncpy(hdr->uname, s, TUGNAMEMAX);
	}
	if (hdr->gname[0])
		hdr->gid = gidnum(hdr->gname);
	else if (s = gidstr(hdr->gid)) {
		hdr->gname[TUGNAMEMAX] = '\0';
		strncpy(hdr->gname, s, TUGNAMEMAX);
	}
#define OCT(S, V) putoct(S, V, sizeof S)
	OCT(buf->mode, hdr->mode);
	OCT(buf->uid, hdr->uid);
	OCT(buf->gid, hdr->gid);
	OCT(buf->size, hdr->size);
	OCT(buf->mtime, hdr->mtime);
	buf->typeflag[0] = hdr->typeflag;
	strncpy(buf->linkname, hdr->linkname, sizeof buf->linkname);
	strncpy(buf->magic, TMAGIC, TMAGLEN);
	strncpy(buf->version, TVERSION, TVERSLEN);
	strncpy(buf->uname, hdr->uname, TUGNAMEMAX);
	strncpy(buf->gname, hdr->gname, TUGNAMEMAX);
	if (hdr->typeflag == BLKTYPE || hdr->typeflag == CHRTYPE) {
		OCT(buf->devmajor, hdr->devmajor);
		OCT(buf->devminor, hdr->devminor);
	}
	mysum = chksum(buf);
	OCT(buf->chksum, mysum);
	return 0;
#undef OCT
}
