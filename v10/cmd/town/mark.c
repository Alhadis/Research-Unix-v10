# include "stdio.h"
# include "assert.h"
# include "cbt.h"
main(argc,argv)
	char *argv[];
{
bfile *bloc;
FILE *fpat, *find;
char name[100], loc[100], ibuff[100];
mbuf key, val;
double lat, lng, atof();
long atol(), lb;
int x, y, k, found;
bloc = bopen("//usr/spool/town/ustowns", 0);
find = fopen("//usr/spool/town/hapindex", "r");
assert(bloc!=NULL);
assert(find!=NULL);
while (gets(name))
	{
	fpat = fopen("//usr/spool/town/haplist", "r");
	key.mdata = name; key.mlen = strlen(name);
	k = bseek(bloc, key);
	if (k!=1) continue;
	val.mdata = loc;
	bread(bloc, NULL, &val);
	val.mdata[val.mlen]=0;
	lat = atof(val.mdata);
	lng = atof(val.mdata+7);
	x = lat/4.;
	y = lng/4.;
	sprintf(loc, "%02d.%02d", x, y);
	rewind (find);
	while (fgets(ibuff, 100, find))
		{
		if (strncmp(ibuff, loc, 5)==0)
			break;
		}
	if (feof(find))continue;
	lb = atol(ibuff+6);
	fseek (fpat, lb, 0);
	found=0;
	while (fgets(ibuff, 100, fpat))
		{
		if (strncmp(loc, ibuff, 5)!=0)
			break;
		if (strncmp(name, ibuff+6, strlen(name)))
			continue;
		lb = ftell (fpat);
		found=1;
		break;
		}
	fclose(fpat);
	if (found)
		{
		k = open("//usr/spool/town/haplist", 2);
		if (k>0)
			{
			lseek (k, lb-3, 0);
			write (k, "x", 1);
			close(k);
			}
		}
	}
}
