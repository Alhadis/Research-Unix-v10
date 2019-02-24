#include	<fio.h>
#include	<libc.h>

#define	TMP	"/tmp/tstsk"
#define	FN(x)	(unsigned char)((x&0xFF)^(x/251))

main(argc, argv)
	char **argv;
{
	register n, c, k;
	int len, fd, cd;
	char *b1, *b2, *p;

	if(argc != 1){
		Fprint(2, "Usage: tstsk\n");
		exit(1);
	}
	len = 1000000;
	if(sizeof(int) == 2)
		len &= 0x7FFF;
	if((fd = creat(TMP, 0666)) < 0){
		perror(TMP);	
		exit(1);
	}
	close(fd);
	if((fd = open(TMP, 2)) < 0){
		perror(TMP);	
		exit(1);
	}
	unlink(TMP);
	if((b2 = malloc(len)) == 0){
		Fprint(2, "malloc(%d) failed\n", len);
		exit(1);
	}
	for(n = 0, p = b2; n < len; n++)
		*p++ = FN(n);
	if(write(fd, b2, len) != len){
		perror("write");
		exit(1);
	}
	lseek(fd, 0L, 0);
	Finit(fd, (char *)0);
	for(n = 0; n < 500; n++){
		k = nrand(len);
		c = nrand(sizeof(Fbuffer))+100;
		if(k+c > len) c = len-k;
		test(fd, k, c, n);
	}
done:
	Fprint(2, "tstsk: %d bytes, %d seeks, succeeded\n", len, n);
	exit(0);
}

test(fd, off, n, count)
{
	long offset;
	int c;

	offset = off;
	if(Fseek(fd, offset, 0) != offset){
		fprint(2, "fseek(%ld) != %ld!!\n", offset, offset);
		exit(1);
	}
	while(n-- > 0){
		if((c = Fgetc(fd)) != FN(offset)){
			fprint(2, "tst %d: char mismatch; %ld: %x, expected %x\n",
				count, offset, c, FN(offset));
			dumpfb(fd);
			exit(1);
		}
		offset++;
	}
}

dumpfb(fd)
{
	register Fbuffer *f = Ffb[fd];
	register unsigned char *c;
	register long off;
	int i;

	fprint(2, "buf@%ld: next=%ld end=%ld lnext=%ld buf=%ld offset=%ld\n",
		f, f->next, f->end, f->lnext, f->buf, f->offset);
	off = f->offset - (f->end - f->buf);
	for(c = f->buf; c < &f->buf[64]; ){
		fprint(2, "%7ld:", off);
		for(i = 0; i < 16; i++, off++)
			fprint(2, " %2x", *c++);
		fprint(2, "\n");
	}
}
