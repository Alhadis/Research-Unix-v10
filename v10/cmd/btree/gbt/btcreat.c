#include "stdio.h"
#include "cbt.h"
#define error(x, y) {fprintf(stderr, x, y); exit(1); }

int iflag, rflag;
char node[NDSZ];
char buf[512];
extern char *malloc();

main(argc, argv)
char **argv;
{	int i, fd;
	hdr *b;
	char *p;
	for(i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			for(p = argv[i] + 1; *p; p++)
				if(*p == 'i')
					iflag = 1;
				else if(*p == 'r')
					rflag = 1;
				else
					error("unknown flag %c\n", *p);
			if(i >= argc - 1)
				error("file name?", 0);
			continue;
		}
		if(!iflag) {
			sprintf(buf, "%s.F", argv[i]);
			if((fd = creat(buf, 0666)) < 0) {
				perror(buf);
				exit(1);
			}
			close(fd);
		}
		sprintf(buf, "%s.T", argv[i]);
		if((fd = creat(buf, 0666)) < 0) {
			perror(buf);
			if(!iflag) {
				sprintf(buf, "%s.F", argv[i]);
				unlink(buf);
			}
			exit(1);
		}
		b = (hdr *)node;
		b->kcnt = 0;
		if(iflag)
			b->htype |= INDEX;
		if(rflag)
			b->htype |= READONLY;
		nfree(b) = NDSZ - sizeof(hdr) - sizeof(trailer);
		if(write(fd, node, NDSZ) != NDSZ)
			perror("failed");
		close(fd);
	}
	exit(0);
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/8/9:btcreat.c\n"};
/*0011011101111110*/
