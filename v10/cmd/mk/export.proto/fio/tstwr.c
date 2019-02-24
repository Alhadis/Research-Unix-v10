#include	<fio.h>
#include	<libc.h>

#define	TMP	"/tmp/tstwr"

main(argc, argv)
	char **argv;
{
	register n, c, k;
	int len, fd, cd;
	char *b1, *b2, *p;

	if(argc < 2){
		Fprint(2, "Usage: tstwr file\n");
		exit(1);
	}
	len = flen(argv[1]);
	if((fd = open(argv[1], 0)) < 0){
		perror(argv[1]);	
		exit(1);
	}
	if((b1 = malloc(len)) == 0){
		Fprint(2, "malloc(%d) failed\n", len);
		exit(1);
	}
	if(read(fd, b1, len) != len){
		perror("read");
		exit(1);
	}
	close(fd);
	if((fd = creat(TMP, 0666)) < 0){
		perror(TMP);	
		exit(1);
	}
	close(fd);
	if((fd = open(TMP, 2)) < 0){
		perror(TMP);	
		exit(1);
	}
/*	unlink(TMP);	/**/
	if((b2 = malloc(len)) == 0){
		Fprint(2, "malloc(%d) failed\n", len);
		exit(1);
	}
	for(p = b1, cd = 0; cd < len;){
		n = 1000*nrand(6) + nrand(199);
		if(n > len-cd) n = len-cd;
		Fprint(1, "%d putc's\n", n);
		cd += n;
		while(n--)
			Fputc(fd, *p++);
		if(len == cd)
			continue;
		n = 1000*nrand(12) + nrand(199);
		if(n > len-cd) n = len-cd;
		cd += n;
		Fprint(1, "%d write\n", n);
		if(Fwrite(fd, p, (long)n) != (long)n) break;
		p += n;
	}
done:
	Fflush(fd);
	lseek(fd, 0L, 0);
	if((n = read(fd, b2, len)) != len){
		fprint(2, "read %d, len=%d: ", n, len);
		perror("read2");
		exit(1);
	}
	if(memcmp(b1, b2, len)){
		Fprint(2, "memcmp failed!\n");
		exit(1);
	}
	Fprint(2, "tstwr: %d bytes, succeeded\n", len);
	exit(0);
}

#include	<sys/types.h>
#include	<sys/stat.h>

flen(s)
	char *s;
{
	struct stat sb;
	int n;

	if(stat(s, &sb) < 0){
		perror(s);
		exit(1);
	}
	n = sb.st_size;
	if(sizeof(int) == 2)
		n &= 0x7FFF;
	return(n);
}
