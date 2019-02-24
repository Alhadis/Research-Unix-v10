unsigned short
sum(s, n, summ)
	register unsigned char *s;
	register n;
	register unsigned short summ;
{
	while(n--)
		if(summ&1)
			summ = 0xFFFF&((summ>>1) + 0x8000 + *s++);
		else
			summ = 0xFFFF&((summ>>1) + *s++);
	return(summ);
}

/*
main()
{
	char buf[512];
	register unsigned short summ;
	int n;

	summ = 0;
	while((n = read(0, buf, sizeof buf)) > 0)
		summ = sum(buf, n, summ);
	print("%d\n", summ);
	exit(0);
}
*/
