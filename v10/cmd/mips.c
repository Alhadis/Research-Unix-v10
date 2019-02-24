#define	N	200
int	b1[10];
int	b2[10];

main()
{
	double t;
	register i, a = 0;
	int n;

	times(b1);
	for(n=0; n<N; n++)
	for(i=1000; --i;) {
		a++;a++;a++;a++;a++;a++;a++;a++;a++;a++;
		a++;a++;a++;a++;a++;a++;a++;a++;a++;a++;
		a++;a++;a++;a++;a++;a++;a++;a++;a++;a++;
		a++;a++;a++;a++;a++;a++;a++;a++;a++;a++;
		a++;a++;a++;a++;a++;a++;a++;a++;a++;a++;
		a++;a++;a++;a++;a++;a++;a++;a++;a++;a++;
		a++;a++;a++;a++;a++;a++;a++;a++;a++;a++;
		a++;a++;a++;a++;a++;a++;a++;a++;a++;a++;
		a++;a++;a++;a++;a++;a++;a++;a++;a++;a++;
		a++;a++;a++;a++;a++;a++;a++;a++;a++;
	}
	a = a;
	times(b2);
	if(b2[0])
		t = b2[0]-b1[0]; else
		t = b2[1]-b1[1];
	t = (6.*N)/t;
	printf("%.2f\n", t);
}
