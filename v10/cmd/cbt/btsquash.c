#include "stdio.h"
#include "cbt.h"

extern int errno;
mbuf key, value;
char kbuf[NDSZ], vbuf[32767];
bfile *bfd;
FILE *pfd;
extern FILE *popen();
char iflag, rflag;
char dest[FNAMELEN];

main(argc, argv)
char **argv;
{
	int oflag = 0;

	dest[0] = 0;
	if(argc > 2){
		if(strncmp(argv[1], "-o", 2))
			goto usage;
		strcpy(dest, &argv[1][2]);
		argv[1] = argv[2];
		argc--;
		oflag = 1;
	}
	if(argc != 2) {
usage:
		fprintf(stderr, "usage; %s [-odest] file-name\n", argv[0]);
		exit(1);
	}
	if(dest[0] == 0)
		sprintf(dest, "TS%d", getpid());
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
	sprintf(kbuf, "%s %s", vbuf, dest);
	if(system(kbuf) != 0) {
		fprintf(stderr, "%s failed\n", kbuf);
		exit(1);
	}
	sprintf(kbuf, "cbt build -r %s", dest);
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
	if(oflag == 0){
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
	}
	exit(0);
}

fail(s)
char *s;
{
	perror(s);
	exit(2);
}
