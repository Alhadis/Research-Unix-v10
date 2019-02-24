/*
 * Routine to convert old style encrypted files into new (1983) format.
 *
 * cvcrypt [-r] oldfile newfile
 *
 * Prompts for key.
 * -r option converts from new to old.
 *
*/

#include <stdio.h>

#define BSIZE 1024

char *getpass();
char iobuf[BSIZE];
char perm[1024];

main(argc, argv)
int argc;
char *argv[];
{
	int ninbuf;
	long count;
	int fdi, fdo;
	int reverse = 0;
	int i;
	int warning = 0;

	if((argv[1][0] == '-') && (argv[1][1] == 'r')){
		reverse = 1;
		argc--;
		argv++;
	}

	if (argc  != 3){
		fprintf(stderr, "Usage: %s [-r] oldfile newfile\n", argv[0]);
		exit(1);
	}else{
		crinit(getpass("Enter key:"), perm);
	}

	fdi = open(argv[1], 0);
	if(fdi <0){
		fprintf(stderr, "Cannot open %s for reading.\n", argv[1]);
		exit(1);
	}
	fdo = open(argv[2], 1);
	if(fdo  != -1){
		fprintf(stderr, "File %s already exists.\n", argv[2]);
		exit(1);
	}
	fdo = creat(argv[2], 0666);
	if(fdo < 0){
		fprintf(stderr, "Cannot open %s for writing.\n", argv[2]);
		exit(1);
	}

	count = 0;
	while(1){
		ninbuf = read(fdi, iobuf, BSIZE);
		if(ninbuf == 0) exit(0);
		if(ninbuf < 0){
			error("crypt: read error.\n");
			exit(1);
		}
		if(reverse == 0){
			ocrblock(perm, iobuf, ninbuf, count);
		}else{
			crblock(perm, iobuf, ninbuf, count);
		}
		if(warning == 0){
			for(i=0; i<ninbuf; i++){
				if((iobuf[i] & 0200) != 0){
					warning = 1;
					fprintf(stderr, "Wrong key or non-ascii file; conversion continued.\n");
					break;
				}
			}
		}
		if(reverse == 0){
			crblock(perm, iobuf, ninbuf, count);
		}else{
			ocrblock(perm, iobuf, ninbuf, count);
		}
		count += ninbuf;
		if(write(fdo, iobuf, ninbuf) != ninbuf){
			error("crypt: write error.\n");
			exit(1);
		}
	}
}

crblock(permp, buf, nchar, startn)
char *permp;
char *buf;
long startn;
{
	register char *p1;
	int n1;
	int n2;
	int n3;
	register char *t1, *t2, *t3;
	char *t4;

	t1 = permp;
	t2 = &permp[256];
	t3 = &permp[512];
	t4 = &permp[768];

	n1 = startn&0377;
	n2 = (startn>>8)&0377;
	p1 = buf;
	while(nchar--) {
		n3 = t4[n1];
		*p1 = t2[(t3[(t1[(*p1+n3)&0377]+n2)&0377]-n2)&0377]-n3;
		n1++;
		if(n1==256){
			n1 = 0;
			n2++;
			if(n2==256) n2 = 0;
		}
		p1++;
	}
}


/*
 * Besides initializing the encryption machine, this routine
 * returns 0 if the key is null, and 1 if it is non-null.
 */
crinit(keyp, permp)
char	*keyp, *permp;
{
	register char *t1, *t2, *t3;
	char *t4;
	register i;
	int ic, k, temp, pf[2];
	unsigned random;
	char buf[13];
	long seed;

	t1 = permp;
	t2 = &permp[256];
	t3 = &permp[512];
	t4 = &permp[768];
	if(*keyp == 0)
		return(0);
	strncpy(buf, keyp, 8);
	while (*keyp)
		*keyp++ = '\0';
	buf[8] = buf[0];
	buf[9] = buf[1];
	if (pipe(pf)<0)
		pf[0] = pf[1] = -1;
	if (fork()==0) {
		close(0);
		close(1);
		dup(pf[0]);
		dup(pf[1]);
		execl("/usr/lib/makekey", "-", 0);
		execl("/lib/makekey", "-", 0);
		exit(1);
	}
	write(pf[1], buf, 10);
	if (wait((int *)NULL)==-1 || read(pf[0], buf, 13)!=13)
		error("crypt: cannot generate key");
	close(pf[0]);
	close(pf[1]);
	seed = 123;
	for (i=0; i<13; i++)
		seed = seed*buf[i] + i;
	for(i=0;i<256;i++){
		t1[i] = i;
		t3[i] = 0;
	}
	for(i=0; i<256; i++) {
		seed = 5*seed + buf[i%13];
		random = seed % 65521;
		k = 256-1 - i;
		ic = (random&0377) % (k+1);
		random >>= 8;
		temp = t1[k];
		t1[k] = t1[ic];
		t1[ic] = temp;
		if(t3[k]!=0) continue;
		ic = (random&0377) % k;
		while(t3[ic]!=0) ic = (ic+1) % k;
		t3[k] = ic;
		t3[ic] = k;
	}
	for(i=0; i<256; i++){
		t2[t1[i]&0377] = i;
		t4[i] = (t1[i] + t3[i]) & 0377;
	}
	return(1);
}


error(s)
char *s;
{
		fprintf(stderr, s);
}
ocrblock(permp, buf, nchar, startn)
char *permp;
char *buf;
long startn;
{
	register char *p1;
	int n1;
	int n2;
	int n3;
	register char *t1, *t2, *t3;
	char *t4;

	t1 = permp;
	t2 = &permp[256];
	t3 = &permp[512];
	t4 = &permp[768];

	n1 = startn&0377;
	n2 = (startn>>8)&0377;
	p1 = buf;
	while(nchar--) {
		*p1 = t2[(t3[(t1[(*p1+n1)&0377]+n2)&0377]-n2)&0377]-n1;
		n1++;
		if(n1==256){
			n1 = 0;
			n2++;
			if(n2==256) n2 = 0;
		}
		p1++;
	}
}
