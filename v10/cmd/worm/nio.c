#include	<libc.h>
#include	"worm.h"

Read(s, buf, n)
	register Superblock *s;
	char *buf;
	long n;
{
	register k;
	int len;

	n *= s->blocksize;
	if(n != (int)n){
		fprint(2, "bad arg to Read n=%ld\n", n);
		abort();
	}
	len = BIGBLOCK;
	while(n){
		if(n < len) len = n;
fprint(2, "reading fd=%d len=%d\n", s->fd, len);
		if((k = read(s->fd, buf, len)) != len){
fprint(2, "k=%d\n", k);
			if(k && (errno != ENXIO)){
				perror("Read");
				exit(1);
			}
			return(1);
		}
		n -= len;
		buf += len;
	}
	return(0);
}

Write(s, buf, n)
	register Superblock *s;
	char *buf;
	long n;
{
	register k;
	int len;
	char msg[256];

	n *= s->blocksize;
	if(n != (int)n){
		fprint(2, "bad arg to Write n=%d\n", n);
		abort();
	}
	len = BIGBLOCK;
	while(n){
		if(n < len) len = n;
		if((k = write(s->fd, buf, len)) != len){
			sprint(msg, "Write %d blks (%dB)", n/s->blocksize, n);
			perror(msg);
			return(1);
		}
		n -= len;
		buf += len;
	}
	return(0);
}

Seek(s, blk)
	register Superblock *s;
	long blk;
{
	bigseek(s->fd, blk, s->blocksize, 0);
}

bigseek(fd, a, b, ptr)
{
#define	SEEK64	/**/
#ifdef	SEEK64
#include	<sys/param.h>
	llong_t off;
	extern llong_t Llmul(), ltoL();

	off = Llmul(ltoL(a), b);
	llseek(fd, off, ptr);
fprint(2, "seek(fd=%d, %dx%d\n", fd, a, b);
#else
	lseek(fd, a*(long)b, ptr);
#endif
}
