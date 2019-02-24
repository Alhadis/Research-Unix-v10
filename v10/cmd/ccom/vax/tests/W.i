unsigned long fds_bits[32];
int md;
clearit(){
	fds_bits[md/(unsigned)32] = ~(1<<(md%(unsigned)32));
}
int x[32], i;
a()
{
	x[f()] = ~(1 << f());
}
b()
{	register int *d, m;
	int r;
	*d = r&m | (~r)&*d;
}
