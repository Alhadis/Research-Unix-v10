# include "stdio.h"
# include "ctype.h"
# include "assert.h"
# include "cbt.h"
# include "weath.h"
# define SURFACE 1
# define FORECAST 2
mbuf key, rec, rkey;
char kb[100], rb[100], rkb[100];
bfile *bf;
char *index(), *vis(), *airport();
double atof();

main()
{
char line[200], line2[200];
int kind=0, k;
bf = bopen("/usr/spool/weather/air", 0);
assert(bf!=NULL);
while (gets(line))
	{
	k = strlen(line);
	if (k==0) continue;
	if (k==8 && strncmp(line+3, "00", 2)==0)
		{
		switch(atoi(line+5))
			{
			case 200: /* new england */
			case 201: /* eastern states */
			case 202: /* midatlantic */
			case 203: /* southeastern */
			case 204: /* great lakes */
			case 205: /* ohio valley */
			case 206: /* northern plains */
			case 207: /* great plains */
			case 208: /* gulf coast */
			case 209: /* northern rockies */
			case 210: /* southwestern */
			case 211: /* pacific northwest */
			case 212: /* pacific */
			case 290: /* alaska */
			case 291: /* hawaii */
				kind = SURFACE;
				continue;
			case 431: kind = FORECAST; continue;
			}
		kind=0;
		continue;
		}
	switch(kind)
		{
		case SURFACE:
			surf(line); break;
		case FORECAST:
			if (strlen(line)>5 && line[3]==' ' &&
				isupper(line[0]) && isupper(line[1]))
			{
				gets(line2);
				fore(line, line2);
			}
			break;
		}
	}
}
surf(line)
	char *line;
{
int nv, nw, i; char *v[50], *w[50], *s, *p, *aname;
double north, west;
if (strlen(line)<10) return;
if (!isalpha(line[0])) return;
s = index(line, '/');
if (s==NULL) return;
while (s>line && *s!= ' ')s--;
*s++ = 0;
nv = getargs(line, v);
nw = getargs(s, w);
aname = airport(v[0]);
if (aname==NULL) return;
for(i=0; i<nw; i++)
	if (strlen(w[i])==4 && isdigit(w[i][0]))
		break;
if (i==nw || i<2) return;
p = vis(v[nv-1]);
if (p==NULL ) return;
printf("at %s (%s)\n temp %s, weather%s\n", v[0], aname, w[i-2], p);
north = atof(aname);
west = atof(index(aname, ' '));
update( v[0], north, west, w[i-2], v[nv-1]);
}
char *
vis(s)
	char *s;
{
int d;
static char temp[50];
d= atoi(s);
while (isdigit(*s)) s++;
if (*s==0)
	{
	sprintf(temp, " clear, visibility %d miles", d);
	return(temp);
	}
temp[0]=0;
for( ; *s; s++)
switch(*s)
	{
	case 'A': strcat(temp, " hail"); break;
	case 'D': strcat(temp, " dust"); break;
	case 'F': strcat(temp, " fog"); break;
	case 'I': if (s[1]=='P') strcat(temp, " sleet"); break;
	case 'K': strcat(temp, " smoke"); break;
	case 'L': strcat(temp, " drizzle"); break;
	case 'R': strcat(temp, " rain"); break;
	case 'W': strcat(temp, " showers"); break;
	case 'S': strcat(temp, " snow"); break;
	case 'T': strcat(temp, " thunderstorms"); break;
	case 'Z': strcat(temp, " freezing"); break;
	case '-': break;
	default: return(NULL);
	}
return(temp);
}
fore( l1, l2)
	char *l1, *l2;
{
char *s;
int p1, p2, p3, p4, max1, min1, max2, min2;
l1[3]=0;
s=airport(l1);
if(s==NULL) return;
printf("For airport %s, %s\n", l1, s);
sscanf(l1+4, "%*s %d %d %d %d", &p1, &p2, &p3, &p4);
sscanf(l2, "%*s %d %d %d %d", &max1, &min1, &max2, &min2);
printf("next 24 hours temps max %d min %d, prob precip %d %d\n",max1, min1, p1, p2);
}
char *
airport(s)
	char *s;
{
int i;
key.mdata = s;
key.mlen = strlen(s);
i=bseek(bf, key);
if (i!=1) return(NULL);
rkey.mdata =rkb; rec.mdata = rb;
bread(bf, &rkey, &rec);
assert(strcmp(rkey.mdata, key.mdata)==0);
rkey.mdata[rkey.mlen]=0;
rec.mdata[rec.mlen]=0;
return(rec.mdata);
}
update (acode, north, west, temp, weath)
	double north, west;
	char *acode, *temp, *weath;
{
char fname[30], odata[2000], *p, *s;
struct wline *wp;
int n, w, f;
n = north;
n = n-n%4;
w = west;
w = w-w%4;
sprintf(fname, "/usr/spool/weather/o%.2d.%.2d", n, w);
f = open(fname, 2);
if (f>=0)
	{
	n=read(f, odata, 2000);
	assert(n<2000);
	for(wp=odata; wp<odata+n; wp++)
		{
		if (strncmp(wp->anam, acode, 3)==0)
			{
			strncpy(wp->wtemp, temp, 3);
			strncpy(wp->wweath, weath, 7);
			lseek (f, 0L, 0);
			write(f, odata, n);
			return;
			}
		}
	}
else
	{
	f = creat(fname, 0666);
	wp = odata;
	n=0;
	}
sprintf(wp->anam, "%.3s%5.2f%6.2f%3.3s%7.7s\n", acode, north,
	west, temp, weath);
lseek (f, 0L, 0);
write(f, odata, n+sizeof(*wp));
close(f);
}
