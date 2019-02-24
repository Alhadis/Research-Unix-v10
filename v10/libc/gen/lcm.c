long
lcm(a, b)
{
	long c;
	if(a==0 && b==0)
		return(0L);
	c = a/gcd(a,b)*(long)b;
	return(c>=0? c: -c);
}
