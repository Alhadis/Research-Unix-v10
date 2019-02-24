typedef struct P0 {
	unsigned L9 : 8;		
} P0;
P0 x;
unsigned char sv[512];
addproc(n)
{	int j, i=4;
	((P0 *)(&sv[i]))->L9 = j = addqueue();
}
a()
{	int j;
	x.L9 = j = addqueue();
}
addqueue()
{
	return(12);
}
main()
{
	addproc(1);
	printf("%d\n", sv[4]);
	a();
	printf("%d\n", x.L9);
}
