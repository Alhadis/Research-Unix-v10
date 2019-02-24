#define	NBLK	100
#define	BSIZE	512
char	buf[BSIZE*NBLK];
main(argc, argv)
char **argv;
{
	long i;
	register j;
	register fi, fo;
	int cfi;
	int err, k;
	long SIZE, atol();

	if (argc!=5) {
		printf("dskcpy infile rawinfile outfile size (512)\n");
		exit(1);
	}
	SIZE = atol(argv[4]);
	printf("Copying %ld blocks\n", SIZE);
	fi = open(argv[2], 0);
	cfi = open(argv[1], 0);
	if (fi<0 || cfi<0) {
		printf("read open\n");
		exit(1);
	}
	fo = open(argv[3], 1);
	if (fo < 0) {
		printf("write open\n");
		exit(1);
	}
	err = 0;
	for (i=0; i<SIZE; i+=NBLK) {
		if (err) {
			lseek(fi, i*BSIZE, 0);
			lseek(fo, i*BSIZE, 0);
			lseek(cfi, i*BSIZE, 0);
		}
		err = 0;
		if (read(fi, buf, BSIZE*NBLK) != BSIZE*NBLK) {
			printf("PRE %D\n", i);
			err++;
		}
		if (write(fo, buf, BSIZE*NBLK) != BSIZE*NBLK)
			err++;
		if (err) {
			for (j=0; j<NBLK; j++) {
				if ((i+j)>=SIZE)
					break;
				lseek(cfi, (i+j)*BSIZE, 0);
				lseek(fo, (i+j)*BSIZE, 0);
				if (read(cfi, buf, BSIZE) != BSIZE) {
					printf("RE %D\n", i+j);
					for (k=0;k<BSIZE;k++)
						buf[k] = 0;
				}
				if (write(fo, buf, BSIZE) != BSIZE)
					printf("WE %D\n", i+j);
			}
		}
	}
	printf("done\n");
	return(0);
}
