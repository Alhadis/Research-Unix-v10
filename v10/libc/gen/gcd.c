/* The test if(b<0) is inside the loop to avoid anomalies
   with the most negative integer */

gcd(a,b)
register a, b;
{
	register t;
	if(b==0)
		return(abs(a));
	for(;;) {
		if(b<0)
			b = -b;
		t = a % b;
		if(t==0)
			return(b);
		a = b;
		b = t;
	}
}
