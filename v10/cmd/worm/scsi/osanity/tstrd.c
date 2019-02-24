main(argc, argv)
	char **argv;
{
	long first, last, t;
	char buf[32768], buf1[32768], *bufp;
	int fd, n;
	char *worm = "/dev/worm0";

	if((argc < 3) || (argc > 4)){
		print("Usage: tstrd [device] firstblock firstnonblock\n");
		exit(1);
	}
	if(argc > 3)
		worm = *++argv;
	if((fd = open(worm, 0)) < 0){
		perror(worm);
		exit(1);
	}
	first = atol(argv[1]);
	last = atol(argv[2]);
	if((first < 0) || (last <= first)){
		print("bad first=%ld last=%ld\n", first, last);
		exit(1);
	}
	print("reading blocks %ld - %ld inclusive on %s\n", first, last-1, worm);
	fillbuf(buf, 32);
	bufp = &buf[1024*(first%5)];
	lseek(fd, first*1024, 0);
	while(first < last){
		n = last-first;
		if(n > 25) n = 25;
		if(read(fd, buf1, n*1024) != n*1024){
			print("block %ld: ", first);
			perror("read");
			exit(1);
		}
		if(memcmp(bufp, buf1, n*1024)){
			print("block %ld: bytes differ\n", first);
			exit(1);
		}
		if((first%5000) == 0){
			t = time((long *)0);
			print("done block %ld: %s", first, ctime(&t));
		}
		first += n;
	}
	exit(0);
}
