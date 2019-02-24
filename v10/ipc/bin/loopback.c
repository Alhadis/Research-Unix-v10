#include <stdio.h>
#include <libc.h>
#include <ipc.h>

timeout()
{
	fprintf(stderr, "timeout\n");
	exit(1);
}

main(ac,av)
	int ac;
	char *av[];
{
	char buf[8*1024];
	char buf2[8*1024];
	int fd, n;
	unsigned int i, j, count, size, bytes;
	char *ipcpath();
	long start, end;

	bytes=1024*1024;
	size=1024;
	switch(ac) {
	default:
		fprintf(stderr, "usage: loopback host [size] [bytes]\n");
		exit(1);
	case 4:
		bytes=atoi(av[3]);
	case 3:
		size=atoi(av[2]);
		if(size>sizeof(buf))
			size=sizeof(buf);
	case 2:
		break;
	}
	count=bytes/size;

	/*
	 * set up pattern
	 */
	for(i=0; i<sizeof(buf); i++)
		buf[i] = i&0xff;

	fd = ipcopen(ipcpath(av[1], "dk", "loopback"), "heavy");
	if (fd<0){
		fprintf(stderr, "error connecting to %s: %s\n", av[1], errstr);
		exit(1);
	}
	printf("connected to %s\n", av[1]);
	fflush(stdout);

	/*
	 * separate reader and writer
	 */
	switch(fork()) {
	case -1:
		perror("forking:");
		exit(1);
	case 0:
		for(i=0; i<count; i++){
			alarm(60);
			if(write(fd, buf, size)!=size) {
				perror("write:");
				exit(1);
			}
		}
		exit(0);
	default:
		start=time((long *)0);
		for(i=0; i<count; i++){
			alarm(60);
			for(j=0; j<size; j+=n){
				n=read(fd, buf2+j, size-j);
				if(n<=0){
					perror("read");
					exit(1);
				}
			}
			if(memcmp(buf, buf2, size)){
				fprintf(stderr,"bad data\n");
				exit(1);
			}
		}
		break;
	}
	end=time((long *)0);
	printf("%d*%d bytes xferred in %d secs = %d bytes/sec\n", count, size,
		end-start, (size*count)/(end-start));
	exit(0);
}
