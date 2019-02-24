_printnum(n)
unsigned n;
{
	char d = n%10 + '0';
	if(n /= 10)
		_printnum(n);
	write(2, &d, 1);
}
