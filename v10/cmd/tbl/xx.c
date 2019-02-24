 /* t4.c: read table specification */
# include "t..c"
int oncol;
getspec()
{
int icol, i;
qcol = findcol()+1;/* must allow one extra for line at right */
qcol++; /* just to be safe */
garray(qcol);
sep[-1]= -1;
for(icol=0; icol<qcol; icol++)
	{
	sep[icol]= -1;
	evenup[icol]=0;
	cll[icol][0]=0;
	for(i=0; i<MAXHEAD; i++)
		{
		csize[icol][i][0]=0;
		vsize[icol][i][0]=0;
		font[icol][i][0] = lefline[icol][i] = 0;
		ctop[icol][i]=0;
		style[icol][i]= 'l';
		}
	}
nclin=ncol=0;
oncol =0;
left1flg=rightl=0;
readspec();
fprintf(tabout, ".rm");
for(i=0; i<ncol; i++)
	fprintf(tabout, " %2s", reg(i, CRIGHT));
fprintf(tabout, "\n");
}
