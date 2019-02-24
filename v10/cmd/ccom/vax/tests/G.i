int i; unsigned int ui;
main()
{
	if(ui++ < 0) a(); else b();
	if((i -= (unsigned )1) < 0) a(); else b();
	ui /= (unsigned)12;
	i /= (unsigned)12;
	ui /= 12;
	i /= 12;
	ui %= (unsigned)12;
	i %= (unsigned)12;
	ui %= 12;
	i %= 12;
	ui -= 1;
	i -= (unsigned) 1;
}
