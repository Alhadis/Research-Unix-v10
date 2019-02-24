main(argc, argv)
	char **argv;
{
	long first, last, t;
	char buf[32768], buf1[32768], *bufp;
	int fd, n, i;
	char *worm = "/dev/worm0";
	long bands[50][2];
	int nbands;
	long loop;
	double tseek, tbl;
	float floop;

	if(argc < 3){
		print("Usage: tstsk [device] firstblock firstnonblock ...\n");
		exit(1);
	}
	if((argc&1) == 0)
		worm = *++argv;
	if((fd = open(worm, 0)) < 0){
		perror(worm);
		exit(1);
	}
	nbands = 0;
	while(*++argv){
		first = atol(*argv);
		last = atol(*++argv);
		if((first < 0) || (last <= first)){
			print("bad first=%ld last=%ld\n", first, last);
			exit(1);
		}
		bands[nbands][0] = first;
		bands[nbands][1] = last;
		nbands++;
	}
	tseek = tbl = 0;
	last = 0;
	fillbuf(buf, 32);
	for(loop = 0;; loop++){
		i = nrand(nbands);
		first = bands[i][0] + lrand()%(bands[i][1]-bands[i][0]);
		n = 20;
		if(first+n > bands[i][1])
			first = bands[i][1]-n;
		if(first < bands[i][0])
			first = bands[i][0], n = bands[i][1]-first;
		tbl += n;
		lseek(fd, first*1024, 0);
		last -= first;
		if(last < 0) last = -last;
		tseek += last;
		bufp = &buf[1024*(first%5)];
		if(read(fd, buf1, n*1024) != n*1024){
			print("block %ld: ", first);
			perror("read");
			exit(1);
		}
		if(memcmp(bufp, buf1, n*1024)){
			print("block %ld: bytes differ\n", first);
			exit(1);
		}
		if(loop && ((loop%100) == 0)){
			t = time((long *)0);
			floop = loop+1;
			print("loop %ld: ave blocks=%.1f, ave seek=%.1fk at %s",
				loop, tbl/floop, tseek/floop, ctime(&t));
		}
		last = first+n;
	}
}
