static double big = 7.20575940379e16;
#define ptsiz 50
#define tabsiz 1000
#define tsiz8 8000
#define whsiz 48
double nn;
char table[tabsiz];
int pt[] = {
	  2,  3,  5,  7, 11, 13, 17, 19, 23, 29,
	 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
	 73, 79, 83, 89, 97,101,103,107,109,113,
	127,131,137,139,149,151,157,163,167,173,
	179,181,191,193,197,199,211,223,227,229,
};
double wheel[] = {
	10, 2, 4, 2, 4, 6, 2, 6, 4, 2,
	 4, 6, 6, 2, 6, 4, 2, 6, 4, 6,
	 8, 4, 2, 4, 2, 4, 8, 6, 4, 6,
	 2, 4, 6, 2, 6, 6, 4, 2, 4, 6,
	 2, 6, 4, 2, 4, 2,10, 2,
};
char bittab[] = {
	1, 2, 4, 8, 16, 32, 64, 128,
};

main(argc, argp)
int argc;
char *argp[];
{
	register i;
	double k, temp, v;
	double limit = big;
	double atof(), sqrt();

	if(argc > 1){
		nn = atof(argp[1]);
	}else{
		scanf("%lf", &nn);
	}
	if(argc > 2){
		limit = atof(argp[2]);
		if(limit < nn) exit(0);
		if(limit > big) ouch();
	}
	if((nn<0) || (nn>big)) ouch();
	if(nn == 0) exit();

	if(nn < 230.){
		for(i=0; i<ptsiz; i++){
			if(pt[i] < nn) continue;
			if(pt[i] > limit) exit(0);
			printf("%d\n", pt[i]);
		}
	nn = 230.;
	}

	modf(nn/2, &temp);
	nn = 2.*temp + 1;
/*
 *	clear the sieve table.
 */
	while(1){
		for(i=0;i<tabsiz;i++)
			table[i] = 0;
/*
 *	run the sieve.
 */
		v = sqrt(nn+tsiz8);
		mark(nn,(double)3);
		mark(nn,(double)5);
		mark(nn,(double)7);
		for(i=0,k=11;k<=v;k=k+wheel[i]){
			mark(nn,k);
			if(++i == whsiz) i = 0;
		}
/*
 *	now get the primes from the table
 *	and print them.
 */
		for(i=0; i<tsiz8; i += 2){
			if(table[i>>3] & bittab[i&07]) continue;
			temp = nn + i;
			if(temp > limit) exit(0);
			printf("%.0f\n", temp);
		}
		nn += tsiz8;
	}
}

mark(nn,k)
double nn,k;
{
	double t1;
	register j;
	register k1 = k;

	modf(nn/k, &t1);
	j = k*t1 - nn;
	if(j < 0)
		j += k1;
	for( ; j < tsiz8; j += k1){
		table[j>>3] |= bittab[j&07];
	}
}

ouch()
{
	printf("OUCH!\n");
	exit(1);
}
