ranf(max)
int	max;
{
	double	d;
	register int	t;

	if (max <= 0)
		return (0);
	d = rand()/32768.0;
	return(t = d*max);
}


double franf()
{
	double		t;
	t = rand();
	return (t / 32767.0);
}
