#include	<math.h>

double
fn(x, y, t, wl)
	float t, wl;
{
	return(sin(t + 2*PI*sqrt(x*x + (float)y*y)/wl));
}

main(argc, argv)
	char **argv;
{
	register i, j, n, t, k;
	register float tim, delta;

	if((n = atoi(argv[1])) <= 4)
		n = 16;
	if((t = atoi(argv[2])) <= 1)
		t = 4;
	delta = 2*PI/t;
	for(k = 0, tim = 0; k < t; k++)
	{
		printf("%d %.1f\n", n*n, tim);
		for(i = 0; i < n; i++)
			for(j = 0; j < n; j++)
				printf("%d %d %g\n", j, i, fn(j-n/2, i-n/2, tim, n/3.0));
		tim += delta;
	}
}
