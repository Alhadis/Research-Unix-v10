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

	Fprint(1, "the letter 'c' is 'c'\n");
	dumpfb(1);
	Fseek(1, -3, 1);
	dumpfb(1);
	Fputc(1, '\n');
	dumpfb(1);
	exit(0);
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
