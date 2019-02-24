/*  program to be run on panther to drive Mergenthaler 202 */
/*  half [LL [NL]] < ifile  */
/*  extracted from /n/panther/usr/guest/ken/tfb.c on 2 Jul 85 */
#include <stdio.h>

int LL = 300;
int NL = 300;
unsigned char *buff;

main(argc, argv)
	char **argv;
{
	char *malloc();
	
	if (argc > 1)
	{	sscanf(argv[1], "%d", &LL); NL=LL;
		if (argc > 2)
			sscanf(argv[2], "%d", &NL);	
	}
	fprintf(stderr, "ll %d, nl %d\n", LL, NL);

	buff = (unsigned char *) malloc(LL*NL);

	Read(0, buff, LL*NL);
	typit();
	exit(0);
}

#define	putint(x)	fputc((x)>>8, f), fputc(x, f)
typit()
{
	register j, h, oh, c, d;
	register unsigned char *p;
	FILE *f;
	int D, IH, i, P, F, J;

	f = fopen("/dev/202", "w");
	if(!f) {
		fprintf(stderr, "cannot open: /dev/202\n");
		exit(1);
	}

	fprintf(stderr,"starting to typeset...");
	IH = 685;
	P = 32;
	D = (P*12)/16;
	F = 87;
	J = 0;

	fputc(255, f); fputc('s', f); putint(P);
	fputc(255, f); fputc('f', f); putint(F);
	p = buff;
	for(i=0; i<NL; i++) {
		d = (i*100)/NL;
		fputc(255, f); fputc('p', f); putint(d);
		fputc(255, f); fputc('n', f);
		J -= D;
		while(J <= -54) {
			fputc(255, f); fputc('v', f); putint(16);
			J += 54;
		}
		fputc(255, f); fputc('j', f); putint(J/2);
		h = IH;
		oh = h;
		fputc(255, f); fputc('H', f); putint(h);
		for(j=0; j<LL; j++) {
			c = *p++ & 0377;
			c = (c+6) >> 2;
			h += D;
			if(c >= 64)
				continue;
			d = h-oh;
			if(d >= 256) {
				fputc(255, f); fputc('H', f); putint(h);
			} else {
				fputc(254, f);
				fputc(d, f);
			}
			oh = h;
			fputc(c, f);
		}
	}

	/* advance the paper a bit at the end */
	for(i=0;i<10;i++){ fputc(255, f); fputc('v', f); putint(16); }

	fputc(255, f); fputc('x', f); fputc('s', f); fputc('\n', f);
	fclose(f);
	fputc('\n',stderr);
}


Read(f, a, n)
	char *a;
{
	int nr, total=0;
	while(total<n){
		nr = read(f, a+total, n-total);
		if(nr<0) error("read error");
		if(nr==0) error("premature EOF");
		total += nr;
	}
}

error(s)
  char *s;
{
  fputs(s,stderr);
  putc('\n',stderr);
  exit(1);
}
