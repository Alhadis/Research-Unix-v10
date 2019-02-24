#include <stdio.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/ino.h>
#include <time.h>

#define tp(x,y) {if(j&x) putchar(y); else putchar('-');}

struct stat sbuf;
struct	dinode	dsbuf;

main(argc,argv) char *argv[];
{
	int i,j;
	int f, iflag=0;
	unsigned ino;
	char *tm;

	for(i=1;i<argc;i++) {
		if (strcmp(argv[i], "-i")==0) {
			iflag++;
			i++;
			f = open(argv[i], 0);
			if (f<0) {
				printf("stat: can't open %s\n", argv[i]);
				exit(2);
			}
			continue;
		}
		if (iflag) {
			ino = atoi(argv[i]);
			lseek(f, (ino-1)*64L+1024, 0);
			j = 0;
			if (read(f, &dsbuf, sizeof(dsbuf)) != sizeof(dsbuf))
				j = -1;
		sbuf.st_mode = dsbuf.di_mode;
		sbuf.st_nlink = dsbuf.di_nlink;
		sbuf.st_uid = dsbuf.di_uid;
		sbuf.st_gid = dsbuf.di_gid;
		sbuf.st_size = dsbuf.di_size;
		sbuf.st_atime = dsbuf.di_atime;
		sbuf.st_mtime = dsbuf.di_mtime;
		sbuf.st_ctime = dsbuf.di_ctime;
		sbuf.st_rdev = 0;
		sbuf.st_dev = 0;
		} else if (strcmp(argv[i], "-")==0)
			j = fstat(atoi(argv[i]+1));
		else
			j = stat(argv[i],&sbuf);
		if(j==-1) {
			perror(argv[i]);
			continue;
		}
		j=sbuf.st_mode;
		switch(j&S_IFMT) {
		case S_IFREG: putchar('-'); break;
		case S_IFDIR: putchar('d'); break;
		case S_IFCHR: putchar('c'); break;
		case S_IFBLK: putchar('b'); break;
		default: putchar('?'); break;
		}
		tp(S_ISUID,'u');
		tp(S_ISGID,'g');
		tp(S_ISVTX,'t');
		tp(0400,'r');
		tp(0200,'w');
		tp(0100,'x');
		tp(040,'r');
		tp(020,'w');
		tp(010,'x');
		tp(4,'r');
		tp(2,'w');
		tp(1,'x');
		putchar(' ');
		printf("M%d m%d Mr%d mr%d u%d g%d L%d %ld i%u: %s\n",
			major(sbuf.st_dev),minor(sbuf.st_dev),
			major(sbuf.st_rdev),minor(sbuf.st_rdev),
			sbuf.st_uid,sbuf.st_gid,sbuf.st_nlink,
			sbuf.st_size,sbuf.st_ino,argv[i]);
		ptime(&sbuf.st_atime);
		ptime(&sbuf.st_mtime);
		ptime(&sbuf.st_ctime);
		printf("\n\n");
	}
}

ptime(t)
long *t;
{
	register struct tm *ctp;

	ctp = localtime(t);
	printf("  %02d:%02d:%02d %02d/%02d/%02d",
	  ctp->tm_hour, ctp->tm_min, ctp->tm_sec,
	  ctp->tm_year, ctp->tm_mon+1, ctp->tm_mday);
}
