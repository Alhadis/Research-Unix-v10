/*
double
dgcd(a,b)
double a,b;
{
	double n,d,q,r;

	n = a;
	d = b;
	while(d != 0){
		modf(n/d, &q);
		r = n - q*d;
		n = d;
		d = r;
	}
	if(n<0) n = -n;
	return(n);
}
*/

/*
 * dinv computes and returns the g.c.d g = (a,b)
 * and then provides numbers c,d such that
 * a*c + b*d = g
 *
 * cf. Gauss D.A. Sect. 27.
 */
double
dinv(a, b, c, d)
double a, b;
double *c, *d;
{
	double num, den, q, r;
	double temp;
	double oldx = 0;
	double newx = 1;
	double oldy = 1;
	double newy = 0;

	num = b;
	den = a;
	while(den != 0){
		modf(num/den, &q);
		r = num - q*den;
		temp = newx;
		newx = oldx - q*newx;
		oldx = temp;
		temp = newy;
		newy = oldy - q*newy;
		oldy = temp;
		num = den;
		den = r;
	}
	if(num <0){
		num = -num;
		oldx = -oldx;
		oldy = -oldy;
	}
	*c = oldx;
	*d = oldy;
	return(num);
}
