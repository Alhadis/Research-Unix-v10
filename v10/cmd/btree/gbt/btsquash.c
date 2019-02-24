#include "stdio.h"
#include "cbt.h"

extern int errno;
mbuf key, value;
char kbuf[NDSZ], vbuf[32767];
bfile *bfd;
FILE *pfd;
extern FILE *popen();
char iflag, rflag;

main(argc, argv)
char **argv;
{
	if(argc != 2) {
		fprintf(stderr, "usage; %s file-name\n", argv[0]);
		exit(1);
	}
	bfd = bopen(argv[1], 0);
	if(bfd == NULL)
		fail(argv[1]);
	strcpy(vbuf, "cbt creat ");
	if(bfd->path[0]->htype & INDEX) {
		iflag = 1;
		strcat(vbuf, "-i ");
	}
	if(bfd->path[0]->htype & READONLY) {
		rflag = 1;
		strcat(vbuf, "-r");
	}
	sprintf(kbuf, "%s TS%d", vbuf, getpid());
	if(system(kbuf) != 0) {
		fprintf(stderr, "%s failed\n", kbuf);
		exit(1);
	}
	/* this refers to the installed version? */
	sprintf(kbuf, "cbt build -r TS%d", getpid());
	pfd = popen(kbuf, "w");
	if(pfd == NULL)
		fail(kbuf);
	key.mdata = kbuf;
	value.mdata = vbuf;
	errno = 0;
	while(bread(bfd, &key, &value) != EOF) {
		(void) fwrite((char *)&key.mlen, 1, sizeof(key.mlen), pfd);
		(void) fwrite(key.mdata, 1, key.mlen, pfd);
		(void) fwrite((char *)&value.mlen, 1, sizeof(value.mlen), pfd);
		(void) fwrite(value.mdata, 1, value.mlen, pfd);
		if(ferror(pfd))
			fail("write to build");
	}
	if(errno)
		fail("extracting");
	if(pclose(pfd) != 0)
		fail("pipe close");
	sprintf(kbuf, "%s.T", argv[1]);
	sprintf(vbuf, "TS%d.T", getpid());
	unlink(kbuf);
	link(vbuf, kbuf);
	if(iflag)
		exit(0);
	sprintf(kbuf, "%s.F", argv[1]);
	sprintf(vbuf, "TS%d.F", getpid());
	unlink(kbuf);
	link(vbuf, kbuf);
	if(errno)
		perror("linking");
	exit(0);
}

fail(s)
char *s;
{
	perror(s);
	exit(2);
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/4/26:btsquash.c\n"};
/*0100011001101001*/
