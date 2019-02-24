# define MAXHEAD 15
char (*font)[MAXHEAD][2];
int (*lefline)[MAXHEAD];
 /* t4.c: read table specification */
main()
{
int icol, i;
char x[1000];
font = x;
lefline = x+500;
icol=3;
i=7;
font[3][7][0]='a';
		{
		font[icol][i][0] = lefline[icol][i] = 0;
		}
printf("value of font[3][7][0] is %o\n", font[3][7][0]);
}
