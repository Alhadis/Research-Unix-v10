#include	<fio.h>
#include	"backup.h"

#define		EXIT(n)		(unlock(), unlink(template), exit(n))

main(argc, argv)
	char **argv;
{
	char bdir[FNAMELEN];
	int fd, tfd;
	long minsize, maxsize, n, len;
	static char template[] = "/tmp/logtmpXXXXXX";
	char buf[16*1024];

	if(argc != 4){
		fprint(2, "Usage: logtrim file min max\n");
		exit(1);
	}
	if((minsize = atol(argv[2])*1024) <= 0){
		fprint(2, "logtrim: minsize(=%s) must be > 0\n", argv[2]);
		exit(1);
	}
	if((maxsize = atol(argv[3])*1024) <= 0){
		fprint(2, "logtrim: maxsize(=%s) must be > 0\n", argv[3]);
		exit(1);
	}
	if(maxsize <= minsize){
		fprint(2, "logtrim: maxsize=%d must be > minsize=%d\n", maxsize, minsize);
		exit(1);
	}
	if((fd = open(argv[1], 0)) < 0){
		perror(argv[1]);
		exit(2);
	}
	mktemp(template);
	close(creat(template, 0666));
	if((tfd = open(template, 2)) < 0){
		perror(template);
		exit(3);
	}
	backupdir(bdir);
	lock(bdir);
	len = 0;
	while((n = read(fd, buf, sizeof buf)) > 0){
		if(write(tfd, buf, (int)n) != n){
			perror("logtrim tmp write");
			EXIT(4);
		}
		len += n;
	}
	if(n < 0){
		perror("logtrim tmp read");
		EXIT(5);
	}
	if(len <= maxsize)
		EXIT(0);
	/* tmp file is ready to copy back */
	close(fd);
	if((fd = creat(argv[1], 0777)) < 0){
		perror(argv[1]);
		EXIT(6);
	}
	if(nlseek(tfd, len-minsize))
		EXIT(7);
	while((n = read(tfd, buf, sizeof buf)) > 0){
		if(write(fd, buf, (int)n) != n){
			perror("logtrim log write");
			EXIT(8);
		}
	}
	if(n < 0){
		perror("logtrim log read");
		EXIT(9);
	}
	EXIT(0);
}

nlseek(fd, n)
	long n;
{
	char buf[16*1024];
	register char *s;
	register buflen;

	buflen = sizeof buf;
	while(n > 0){
		if(buflen > n)
			buflen = n;
		lseek(fd, n-buflen, 0);
		if(read(fd, buf, buflen) != buflen){
			perror("logtrim: nlseek read");
			return(1);
		}
		for(s = buf+buflen; s > buf;)
			if(*--s == '\n'){
				n += (s-buf)+1-buflen;
				goto done;
			}
		n -= buflen;
	}
done:
	lseek(fd, n, 0);
	return(0);
}
