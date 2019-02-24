# define MAXHEAD 15
extern char (*font)[MAXHEAD][2];
extern int (*lefline)[MAXHEAD];
 /* t4.c: read table specification */
getspec()
{
int icol, i;
icol=3;
i=7;
		{
		font[icol][i][0] = lefline[icol][i] = 0;
		}
}
