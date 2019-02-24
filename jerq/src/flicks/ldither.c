/*
	usage: dither n for an nxn dither array (n must be a power of 2)
*/
int dith[1024][1024];
int sq;

main(argc, argv)
	char **argv;
{
	register n, *i, k, j;

	if((n = atoi(argv[1])) <= 0)
		n = 1;
	if(n > 1024)
		n = 1024;
	sq = n;
	dither(0, 0, n, n, 1);
	print();
}

dither(lx, ly, hx, hy, n)
{
	int mx = (hx+lx)/2, my = (hy+ly)/2;
	register i, j, *p;

	if((hx-lx) <= 1) return;
	
	for(j = ly; j < my; j++)
		for(i = lx, p = &dith[j][i]; i < mx; i++)
			*p++ += 3*n;
	for(j = ly; j < my; j++)
		for(i = mx, p = &dith[j][i]; i < hx; i++)
			*p++ += n;
	for(j = my; j < hy; j++)
		for(i = lx, p = &dith[j][i]; i < mx; i++)
			*p++ += 0;
	for(j = my; j < hy; j++)
		for(i = mx, p = &dith[j][i]; i < hx; i++)
			*p++ += 2*n;
	dither(lx, ly, mx, my, 4*n);
	dither(mx, ly, hx, my, 4*n);
	dither(lx, my, mx, hy, 4*n);
	dither(mx, my, hx, hy, 4*n);
}

print()
{
	register n = sq, *i, k, j;
	register double z = sq*sq;
	extern double pow();

	for(k = n-1; k >= 0; k--)
	{
		for(j = 0, i = &dith[k][j]; j < n; j++)
			printf("%d, ", (int)pow(z, *i++/z));
		printf("\n");
	}
}
