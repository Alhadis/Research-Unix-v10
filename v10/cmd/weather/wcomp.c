# include "stdio.h"
# include "ctype.h"
# include "assert.h"
# include "cbt.h"
# include "weath.h"
# include "time.h"
struct tm *gmtime();
# define SURFACE 1
# define LFMFORE 2
# define FORECAST 3
# define MARINE 4
mbuf key, rec, rkey;
char kb[100], rb[100], rkb[100];
int day;
bfile *bf;
char *index(), *vis(), *airport();
double atof();

main()
{
char line[400], line2[400];
int kind=0, k;
long clock;
time(&clock);
day = gmtime(&clock)->tm_yday;
chdir ("/usr/spool/weather");
bf = bopen("air", 0);
assert(bf!=NULL);
while (gets(line))
	{
	k = strlen(line);
	if (k==0) continue;
	if (k==8 && strncmp(line+3, "00", 2)==0)
		{
		unfore();
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
			case 431:
				kind = LFMFORE; continue;
			case 307:
			case 308:
			case 309:
				kind = FORECAST; continue;
			case 313:
				kind = MARINE; continue;
			}
		kind=0;
		continue;
		}
	switch(kind)
		{
		case SURFACE:
			surf(line); break;
		case LFMFORE:
			if (strlen(line)>5 && line[3]==' ' &&
				isupper(line[0]) && isupper(line[1]))
			{
				gets(line2);
				if (startwith(line2, "POF"))
					gets(line2);
				lfmfore(line, line2);
			}
			else
			if (strncmp(line, "DATE/GMT", 8)==0)
				lfmtimeset(line);
			break;
		case FORECAST:
		case MARINE:
			realfore(line, kind); break;
		}
	}
}
surf(line)
	char *line;
{
int nv, nw, i, cloud; char *v[50], *w[50], *s, *p, *aname;
double north, west;
if (strlen(line)<10) return;
if (!isalpha(line[0])) return;
s=line;
while ( (s=index (s, '/')) && !hasnxt(s))
	s++;
if (s==NULL) return;
while (s>line && *s!= ' ')s--;
*s++ = 0;
nv = getargs(line, v);
nw = getargs(s, w);
aname = airport(v[0]);
if (aname==NULL) return;
for(i=0; i<nw; i++)
	{ int q;
	q = strlen(w[i]);
	if (q==4 && isdigit(w[i][0])) break;
	if (q>4 && isdigit(w[i][0]) && w[i][4]=='G') break;
	}
if (i==nw || i<2) return;
p = vis(v[nv-1]);
if (p==NULL ) return;
north = atof(aname);
west = atof(index(aname, ' '));
s= v[nv-2]; /* last kind of cloud cover */
while (isdigit(*s) || *s=='-') s++;
if (strcmp(s, "OVC")==0)
	cloud = 'O';
else
if (strcmp(s, "BKN")==0)
	cloud = 'P';
else
if (strcmp(s, "SCT")==0)
	cloud = 'P';
else
if (strcmp(s, "CLR")==0)
	cloud = 'C';
else
	cloud = 'X';
if (nottime(v[2])) return;
if (!alldigs(w[i-2])) return; /* temp must be digits */
if (!alldigs(w[i-1])) return; /* dewpoint must be digits */
update( v[0], north, west, v[2], w[i-2], w[i-1], v[nv-1], w[i], cloud, w[i+1]);
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
	sprintf(temp, " ordinary, visibility %d miles", d);
	return(temp);
	}
temp[0]=0;
for( ; *s; s++)
switch(*s)
	{
	case 'A': strcat(temp, " hail"); break;
	case 'D': strcat(temp, " dust"); break;
	case 'F': strcat(temp, " fog"); break;
	case 'H': strcat(temp, " haze"); break;
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
char lfmt1[10], lfmt2[10];
lfmfore( l1, l2)
	char *l1, *l2;
{
char *s, fname[100], xl[20], *p, *t;
FILE *tf;
int p1, p2, p3, p4, max1, min1, max2, min2;
l1[3]=0;
s=airport(l1);
if(s==NULL) return;
sscanf(l1+4, "%*s %d %d %d %d", &p1, &p2, &p3, &p4);
sscanf(l2, "%s %d %d %d %d", xl, &max1, &min1, &max2, &min2);
if (p1<0 || p1>100) return;
if (p2<0 || p2>100) return;
if (p3<0 || p3>100) return;
if (p4<0 || p4>100) return;
if (max1>150 || max2>150 || min1>150 || min2>150) return;
if (strncmp(xl, "MN", 2)==0)
	{
	int t;
	t = max1; max1 = min1; min1 = t;
	t = max2; max2 = min2; min2 = t;
	}
sprintf(fname, "lfm/%s", l1);
tf = fopen(fname, "w");
if (tf)
	{
	p = index(s, '\t');
	if (p==NULL) return;
	fprintf(tf, "%s\n", p+1);
	fprintf(tf, "%s %d %d %d %d\n", lfmt1, max1, min1, p1, p2);
	fprintf(tf, "%s %d %d %d %d\n", lfmt2, max2, min2, p3, p4);
	fclose(tf);
	airmark ("lfm/", l1);
	}
return;
}
lfmtimeset(s)
	char *s;
{
sscanf(s, "%*s %s %*s %s", lfmt1, lfmt2);
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
update (acode, north, west, wtime, temp, humid, weath, wind, cloud, press)
	double north, west;
	char *acode, *temp, *weath, *wind, *humid, *wtime, *press;
{
char fname[30], odata[2000], *p, *s, wtt[100];
struct wline *wp;
int n, w, f;
n = north;
n = n-n%4;
w = west;
w = w-w%4;
sprintf(fname, "obs/%.2d.%.2d", n, w);
f = open(fname, 2);
if (f>=0)
	{
	n=read(f, odata, 2000);
	assert(n<2000);
	for(wp=(struct wline *)odata; ( (char *)wp ) <odata+n; wp++)
		{
		if (atoi(wp->wday)+1<day)
			strncpy(wp->anam, "---", 3);
		}
	for(wp=(struct wline *)odata; ( (char *)wp ) <odata+n; wp++)
		{
		if (strncmp(wp->anam, acode, 3)==0)
			break;
		}
	if ((char *)wp >=odata+n)
	for(wp=(struct wline *)odata; ( (char *)wp ) <odata+n; wp++)
		{
		if (strncmp(wp->anam, "---", 3)==0)
			break;
		}
	}
else
	{
	f = creat(fname, 0666);
	wp =  (struct wline *)odata;
	n=0;
	}
sprintf(wtt, "%.3s%5.2f%6.2f%4.4s%3.3s%3.3s%-6.6s%-7.7s%c%3.3s%03d\n",
	acode, north, west, wtime, temp, humid, weath, wind, cloud, press, day);
strncpy(wp->anam, wtt, sizeof(*wp));
lseek (f, 0L, 0);
if (n<= ( (char *)wp - odata )) n+= sizeof(*wp);
write(f, odata, n);
close(f);
}
nottime(s)
	char *s;
{
int c;
if (strlen(s)!=4) return(1);
while ( c = *s++ )
	if (!isdigit(c)) return(1);
return(0);
}
static FILE *ff = NULL;
realfore(line, kind)
	char *line;
{
char fname[50];
if (kind==FORECAST && strncmp(line+3, " FP", 3)==0)
	{
	unfore();
	line[3]=0;
	sprintf(fname, "fore/f_%s", line);
	ff = fopen(fname, "w");
	airmark ("fore/f_", line);
	}
if (kind==FORECAST && strncmp(line, "FPUS", 4)==0)
	{
	unfore();
	line[10]=0;
	sprintf(fname, "fore/f_%s", line+7);
	ff = fopen(fname, "w");
	airmark ("fore/f_", line+7);
	}
if (kind==MARINE && strncmp(line+3, " FZ", 3)==0)
	{
	unfore();
	line[3]=0;
	sprintf(fname, "fore/m_%s", line);
	ff = fopen(fname, "w");
	airmark ("fore/f_", line);
	}
if (ff!=NULL)
	fprintf(ff, "%s\n", line);
}
unfore()
{
if (ff!=NULL) fclose(ff);
}
airmark(code, aname)
	char *code, *aname;
{
char fname[100], *s, xl[30];
double nor, wes; int n, w;
FILE *tf;
s=airport(aname);
if (s==NULL) return;
sscanf(s, "%lfN %lfW", &nor, &wes);
	n = nor; n = n - n%4;
	w = wes; w = w - w%4;
	sprintf(fname, "%s%.2d.%.2d", code, n, w);
	tf = fopen(fname, "r");
	if (tf!=NULL)
		{
		while (fgets(xl, 20, tf))
			{
			xl[3]=0;
			if (strcmp(xl, aname)==0)
				{
				fclose(tf);
				return;
				}
			}
		fclose(tf);
		}
	tf = fopen(fname, "a");
	if (tf==NULL) return;
	fprintf(tf, "%3.3s %5.2f %6.2f\n", aname, nor, wes);
	fclose(tf);
}
hasnxt(s)
	char *s;
{ /* checks for more than one slash */
if (s==NULL) return(1);
s++;
while (*s && !isspace(*s))
	if (*s=='/')
		return(1);
	else
		s++;
return(0);
}
alldigs(s)
	char *s;
{
int c;
while (c = *s++)
	if (!isdigit(c) && c!='-')
		return(0);
return(1);
}
startwith(s, t)
	char *s, *t;
{/* true if s starts with t*/
while (isspace(*s)) s++;
if (strncmp(s, t, strlen(t)) == 0)
	return(1);
return(0);
}
