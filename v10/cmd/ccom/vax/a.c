main()
{	int i, j;
	i = 4;
	if(i & 5)
		print("5\n");
	if(i & 4)
		print("4\n");
	j = 3;
	if((i+j)& 16)
		print("16\n");
	if((i+j) & 9)
		print("9\n");
	if((i+j)&4)
		print("4\n");
	if(!((i+j) & 16))
		print("!16\n");
}
