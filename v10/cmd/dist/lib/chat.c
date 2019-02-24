#define _POSIX_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "lib.h"

#define MIN(A, B) ((A) < (B) ? (A) : (B))

int
getline(char line[])
{
	int i;

	i = 0;
	do
		if (read(0, line + i, 1) != 1)
			return 0;
	while (line[i++] != '\n' && i < MAXLINE - 1);
	if (line[i - 1] != '\n')
		return 0;
	line[i] = 0;
	return 1;
}

long
fdcopy(int dst, int src, long size, unsigned long *crc)
{
	char buf[4096];
	long cc, total;
	int aux;

	total = 0;
	if (crc) {
		crcinit(&aux);
		*crc = 0;
	}
	while (total < size) {
		if ((cc = read(src, buf, MIN(sizeof buf, size - total))) <= 0)
			break;
		if (crc)
			*crc = crcincr((unsigned char *) buf, cc, *crc, &aux);
		if (write(dst, buf, cc) != cc)
			break;
		total += cc;
	}
	return total;
}

int
sendfile(int fd)
{
	long len;
	char mesg[MAXLINE];
	unsigned long crc;

	/* size header */
	len = lseek(fd, 0L, 2);
	lseek(fd, 0L, 0);
	sprintf(mesg, "file length=%ld\n", len);
	write(1, mesg, strlen(mesg));

	/* body */
	if (fdcopy(1, fd, len, &crc) != len) {
		eprintf("sendfile fdcopy failure");
		return 0;
	}

	/* crc footer */
	sprintf(mesg, "file crc=%lu\n", crc);
	write(1, mesg, strlen(mesg));

	/* acknowledge */
	if (!getline(mesg) || strcmp(mesg, "ok\n") != 0) {
		eprintf("sendfile acknowledgement failure");
		return 0;
	}

	return 1;
}

int
recvfile(int fd)
{
	long len;
	char mesg[MAXLINE];
	unsigned long crc, trycrc;

	/* size header */
	if (!getline(mesg) || sscanf(mesg, "file length=%ld\n", &len) != 1) {
		eprintf("recvfile header munged");
		return 0;
	}

	/* body */
	if (fdcopy(fd, 0, len, &crc) != len) {
		eprintf("recvfile fdcopy failure");
		return 0;
	}

	/* crc footer */
	if (!getline(mesg) || sscanf(mesg, "file crc=%lu\n", &trycrc) != 1) {
		eprintf("recvfile footer munged");
		return 0;
	}
	if (crc != trycrc) {
		eprintf("recvfile crc failure");
		return 0;
	}

	/* acknowledge */
	sprintf(mesg, "ok\n");
	write(1, mesg, strlen(mesg));
	return 1;
}
