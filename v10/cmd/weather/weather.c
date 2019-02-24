# include "stdio.h"
# include "cbt.h"
# include "ctype.h"
# include "assert.h"
# include "weath.h"
# include "signal.h"
# include "setjmp.h"
# include "math.h"
# include "sys/types.h"
# include "time.h"
# include "sys/timeb.h"
# define SAME 0

double md=1000., dist();
struct timeb ftb;
struct tm *tp;
bfile *bf;
char *index(), *timezone(), *relhum(), *getlogin(), *ctime();
struct tm *localtime();
bfile *townfile;
char *logname = "/usr/spool/weather/wlog";
int marine=0, intr();
jmp_buf env;
char *direct[] =
{"N", "NE", "E", "SE", "S", "SW", "W", "NW", "N", "xx", "yy"};

main(argc,argv)
	char *argv[];
{
char request[100];
townfile = bopen("/usr/spool/town/ustowns", 0);
if (townfile==NULL)
	{
	fprintf(stderr, "No town list - is /crp mounted?\n");
	exit(1);
	}
bf = bopen("/usr/spool/weather/air", 0);
assert(bf!=NULL);
ftime(&ftb);
tp = localtime(&ftb.time);
argc--; argv++;
while (argc>0 && argv[0][0]=='-')
	{
	switch(argv[0][1])
		{
		case 'm': marine=1; break;
		case 'h': strcpy(request, "murray hill, nj"); break;
		case 'n': strcpy(request, "newark, nj"); break;
		default:
			fprintf(stderr, "Unknown arg: %s\n", argv[0]);
			break;
		}
	argc--; argv++;
	}
if (argc>1)
	{
	fprintf(stderr, "Sorry: weather changed to expect town on standard input.\n");
	fprintf(stderr, "Default MH is now 'weather -h'\n");
	return(0);
	}
if (request[0])
	{
	wprint(request);
	record(request);
	}
else
while (1) /* this loop is basically while (gets(request)) with interrupt */
	{
	setjmp(env);
	signal(SIGINT, SIG_DFL);
	if (gets(request)==NULL)
		break;
	signal(SIGINT, intr);
	if (strcmp(request, "q")==SAME)
		break;
	wprint(request);
	record(request);
	}
return(0);
}
wprint(request)
	char *request;
{
char sna[100], rka[100], rra[300], *normalize();
double lat, lng;
mbuf key, rkey, rrec;
int townfind;
rkey.mdata = rka; rrec.mdata = rra;
request=normalize(request);
if (*request==0) return;
	townfind=0;
	key.mdata = request; key.mlen = strlen(request);
	bseek(townfile, key);
	while (bread(townfile, &rkey, &rrec)==NULL)
		{
		rkey.mdata[rkey.mlen]=0;
		rrec.mdata[rrec.mlen]=0;
		if (strncmp(rkey.mdata, request, key.mlen)!=SAME)
			break;
		townfind++;
		md=1000;
		sscanf(rra, "%lf%lf", &lat, &lng);
		caps(rka);
		printf("%s: (%.3f N, %.3f W)\n", rka, lat, lng);
		strcpy(sna, rka);
		/* floating remainder not defined, so... */
		weather(lat, lng);
		lfmfore(lat, lng);
		forecast(lat, lng);
		}
	if (townfind==0)
		fprintf(stderr, "No such town\n");
return;
}
concat (out, ac, av)
	char *out, *av[];
{
int i;
out[0]=0;
for(i=1; i<ac; i++)
	{
	strcat(out, av[i]);
	if (i+1<ac)
		strcat(out, " ");
	}
return;
}
double
dist (aplat, aplon, bplat, bplon)
	double aplon, aplat, bplon, bplat;
{
/* this code is from the picadad manual. I don't pretend
   to understand it. */
double as, bs, ac, bc, p, cd, d, er;
double rad=0.0174532925;
er = aplon-bplon;
if (er<0) er= -er;
if (er<0.001)
	{
	d = aplat-bplat;
	if (d<0) d= -d;
	return(d*69.055);
	}
as = sin(rad*aplat);
bs = sin(rad*bplat);
ac = cos(rad*aplat);
bc = cos(rad*bplat);
p = cos(er*rad);
cd = (as*bs+ac*bc*p);
return( acos(cd)*3956.56);
}
bearing (lat, lng, alat, alng)
	double lat, lng, alat, alng;
{ /* gives bearing of alat, alng from lat, lng as 0-7 */
# define PI 3.1415926
double dlat, dlng, b;
dlat = alat-lat;
dlng = (alng-lng)*cos(PI*(lat+alat)/360.);
b = atan2(-dlng, dlat); /* range -pi to +pi */
b = b*(180./PI);
if (b<0) b=360+b;
return( (int) (b+22.5)/45.0);
}
lcase (s)
	char *s;
{
int c;
for( ; c= *s; s++)
	if (isupper(c))
		*s = tolower(c);
}
caps(s)
	char *s;
{
int c;
if (islower(*s)) *s = toupper(*s);
for( s++; c= *s; s++)
	{
	if (islower(c) && (s[-1]==' ' || s[-1]=='-'))
		if (strncmp(s, "of ", 3)!=SAME)
			*s = toupper(c);
	if (*s==',') break;
	}
for(s++; c= *s; s++)
	if (islower(c)) *s=toupper(c);
}
weather(north, west)
	double north, west;
{
double d, dist(), md=5000., tnf, twf;
char tn[10], tw[10], *x, *y, fname[100], *vis(), odata[2000], *airname();
char *ampm, *tz, tb[10];
int hr, min;
struct wline *wp, *bwp;
int n, w, f, kbear, isair, windsp, wdir, gust;
n = north;
n = n-n%4;
w = west;
w = w-w%4;
bwp=NULL;
sprintf(fname, "/usr/spool/weather/obs/%.2d.%.2d", n, w);
f = open(fname, 0);
if (f<0) return;
n=read(f, odata, 2000);
assert(n<2000);
/* next line is for(wp=odata; wp<odata+n; wp++) after delinting */
for(wp=(struct wline *) odata; wp< (struct wline *)(odata+n); wp++)
	{
	if (wp->anam[0]=='-') continue;
	strncpy(tn, wp->nlat, 5); tn[5]=0;
	strncpy(tw, wp->nlng, 6); tw[6]=0;
	d = dist(tnf=atof(tn), twf=atof(tw), north, west);
	if (d<md)
		{
		md=d;
		kbear = bearing(north, west, tnf, twf);
		bwp=wp;
		}
	}
close(f);
if (bwp==NULL) return;
strncpy(tn, bwp->anam, 3); tn[3]=0;
x = airname(tn);
x = index(x, '	');
while (isspace(*x))x++;
for(y=x+1; *y != ','; y++)
	if (isupper(*y) && isalpha(y[-1]))
		*y = tolower(*y);
y = index(x, '*');
if (y==NULL)
	isair=1;
else
	{
	*y=0;
	isair=0;
	}
strncpy(tw, bwp->wweath, 6); tw[6]=0;
y = vis(tw, bwp->wcloud[0]);
if (y==NULL) return;
printf("%.1f miles %s at%s %s", md, direct[kbear],
	isair ? " the airport in" : "", x);
strncpy(tb, bwp->wtime, 4); tb[4]=0;
hr = atoi(tb);
min = hr%100;
hr = hr/100;
hr = hr - ftb.timezone/60;
if (tp->tm_isdst) hr++;
if (hr < 0) hr+=24;
ampm = (hr>11) ? "PM" : "AM";
if (hr>12) hr -= 12;
tz = timezone(ftb.timezone, tp->tm_isdst);
printf(" (%d:%.2d %s %s):\n", hr, min, ampm, tz);
strncpy(tb, bwp->wtemp, 3); tb[3]=0;
strncpy(tn, bwp->wdewpt, 3); tn[3]=0;
printf(" temperature %.3s, humidity %s, weather%s",
	bwp->wtemp, relhum(atoi(tb), atoi(tn)), y);
windsp = atoi(bwp->wwind+2);
if (windsp >10)
	{
	bwp->wwind[2]=0;
	wdir = atoi(bwp->wwind);
	if (bwp->wwind[4]=='G')
		gust = atoi(bwp->wwind+5);
	else
		gust=0;
	wdir = (wdir*10+22.5)/45.0;
	printf(",\n   winds at %d knots", windsp);
	printf(" from the %s", direct[wdir]);
	if (gust) printf(" gusting to %d", gust);
	}
printf("\n");
}
char *
vis(s, cloud)
	char *s;
{
int d;
static char temp[80];
char tb[30];
while (isspace(*s)) s++;
d= atoi(s);
while (isdigit(*s)) s++;
if (!isalpha(*s))
	{
	switch(cloud)
		{
		case 'C': s= "clear"; break;
		case 'P': s= "partly cloudy"; break;
		case 'O': s= "overcast"; break;
		default: s= "ordinary"; break;
		}
	sprintf(temp, " %s, visibility %d miles", s, d);
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
	case ' ':
	case '-': break;
	default: return(NULL);
	}
sprintf(tb, ", visibility %d miles", d);
strcat(temp, tb);
return(temp);
}
char *
airname(s)
	char *s;
{
int i;
mbuf key, rkey, rec;
static char rb[100], rkb[100];
key.mdata = s;
key.mlen = strlen(s);
i=bseek(bf, key);
if (i!=1) return(NULL);
rkey.mdata =rkb; rec.mdata = rb;
bread(bf, &rkey, &rec);
rkey.mdata[rkey.mlen]=0;
assert(strcmp(rkey.mdata, key.mdata)==0);
rkey.mdata[rkey.mlen]=0;
rec.mdata[rec.mlen]=0;
return(rec.mdata);
}
lfmfore(north, west)
	double north, west;
{
double d, dist(), md=5000., tnf, twf;
char aname[10], *x, *y, fname[60], xl[30], ln[60], lf1[60], ltime[20];
int n, w, in, jw, iw, jn;
int p1, p2, max, min, hr, dy; char *ampm, *revampm, *tz;
FILE *f;
n = north;
n = n-n%4;
w = west;
w = w-w%4;
in = jn = n;
if ( (int) north %4 >=2) jn+=4; else in-=4;
iw = jw = w;
if ( (int) west %4 >=2 ) jw+=4; else iw-=4;
aname[0]=0;
for(n=in; n<=jn; n+=4)
for(w=iw; w<=jw; w+=4)
	{
	sprintf(fname, "/usr/spool/weather/lfm/%.2d.%.2d", n, w);
	f = fopen(fname, "r");
	if (f==NULL) continue;
	while (fgets(xl, 20, f))
		{
		tnf = atof(xl+4);
		twf = atof(xl+9);
		d = dist(tnf, twf, north, west);
		if (d<md)
			{
			md=d;
			strncpy(aname, xl, 3);
			aname[3]=0;
			}
		}
	fclose(f);
	}
if (aname[0]==0) return;
sprintf(fname, "/usr/spool/weather/lfm/%s", aname);
f = fopen(fname, "r");
if (f==NULL) return;
puts("");
fgets(ln, 60, f);
trimnl(ln);
for(x=ln+1; *x!= ','; x++)
	if (isupper(*x) && isalpha(x[-1]))
		*x = tolower(*x);
y = index(ln, '*');
if (y!=NULL) *y=0;
printf("Next 48 hours at %s\n", ln);
while (fgets(lf1, 60, f))
	{
	sscanf(lf1, "%s %d %d %d %d", ltime, &max, &min, &p1, &p2);
hr = atoi(ltime+3);
dy = atoi(ltime);
hr = hr - ftb.timezone/60;
hr += 12; /* question is whether the periods end or start as shown
  in forecast ; this converts to ending */
if (hr>=24)
	{
	/* this should not be executed ever,since orign hr is 0 or 12 and
	    then we subract 4; */
	hr-=24; dy++;
	}
if (tp->tm_isdst) hr++;
if (hr<0) {
	hr += 24;
	dy = down(dy);
	}
ampm = (hr>11) ? "PM" : "AM";
revampm = (hr>11) ? "AM" : "PM";
if (hr>12) hr -= 12;
tz = timezone(ftb.timezone, tp->tm_isdst);
	printf("To %d %s %s /%d: high %d low %d, prob. precip. to %d %s %d%% to %d %s %d%%\n",
		hr, ampm, tz, dy, max, min, hr, revampm, p1, hr, ampm, p2);
	}
fclose(f);
}
int monlen[12] = 
/*Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec */
 { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
down(dy)
{ /* subtracts one from the day */
int mn;
if (--dy>0) return(dy);
mn = tp->tm_mon;
if (tp->tm_mday<10) mn--;
dy = monlen[mn];
if (dy==28 && tp->tm_year%4==0)
	dy++;
return(dy);
}
forecast(north, west)
	double north, west;
{
double d, dist(), md=5000., tnf, twf;
char aname[10], fname[100], xl[30];
int n, w, in, jw, iw, jn;
FILE *f;
n = north;
n = n-n%4;
w = west;
w = w-w%4;
in = jn = n;
if ( (int) north %4 >=2) jn+=4; else in-=4;
iw = jw = w;
if ( (int) west %4 >=2 ) jw+=4; else iw-=4;
aname[0]=0;
for(n=in; n<=jn; n+=4)
for(w=iw; w<=jw; w+=4)
	{
	sprintf(fname, "/usr/spool/weather/fore/f_%.2d.%.2d", n, w);
	f = fopen(fname, "r");
	if (f==NULL) continue;
	while (fgets(xl, 20, f))
		{
		tnf = atof(xl+4);
		twf = atof(xl+9);
		d = dist(tnf, twf, north, west);
		if (d<md)
			{
			md=d;
			strncpy(aname, xl, 3);
			aname[3]=0;
			}
		}
	fclose(f);
	}
if (aname[0]==0) return;
sprintf(fname, "/usr/spool/weather/fore/f_%s", aname);
f = fopen(fname, "r");
if (f!=NULL)
	{
	puts("");
	printfile(f);
	fclose(f);
	}
if (marine)
	{
	sprintf(fname, "/usr/spool/weather/fore/m_%s", aname);
	f = fopen(fname, "r");
	if (f!=NULL)
		{
		if (f==NULL) return;
		puts("");
		printfile(f);
		fclose(f);
		}
	}
puts("");
}
trimnl(s)
	char *s;
{
while (*s)s++;
if (*--s=='\n')*s=0;
}
intr()
{
longjmp(env, 0);
}
struct table {
        int temp;
        int rh[15];
}tab[21] = {
{100,100,94,88,83,78,73,69,65,61,57,53,45,38,32,27},
{95,100,94,88,83,78,73,68,64,60,56,53,44,37,31,26},
{90,100,94,88,83,77,73,68,64,59,56,52,44,37,31,25},
{85,100,94,88,82,77,72,67,63,58,55,51,43,36,30,25},
{80,100,94,88,82,77,72,67,62,58,54,51,42,35,29,24},
{75,100,94,87,82,76,71,66,62,58,54,50,42,34,28,23},
{70,100,93,87,81,76,71,66,61,57,53,49,41,34,28,23},
{65,100,93,87,81,75,70,65,61,57,52,49,40,33,27,22},
{60,100,93,87,81,75,70,65,60,56,52,48,39,32,26,21},
{55,100,93,86,80,75,69,64,59,55,51,47,39,31,25,20},
{50,100,93,86,80,74,69,64,59,54,50,46,38,31,25,20},
{45,100,93,86,80,74,68,63,58,54,49,46,37,30,24,19},
{40,100,93,86,79,73,68,62,57,53,49,45,36,29,23,18},
{35,100,92,85,79,73,67,61,57,52,48,44,35,28,22,18},
{30,100,92,85,78,72,66,61,56,51,47,43,34,27,21,17},
{25,100,92,85,78,72,66,60,55,51,46,42,33,26,21,16},
{20,100,92,84,77,71,65,60,54,50,45,41,33,25,20,15},
{15,100,92,84,77,70,64,59,54,49,44,40,32,25,19,14},
{10,100,92,84,76,70,64,58,53,48,43,39,31,24,18,14},
{5,100,92,83,76,69,63,57,52,47,42,38,30,23,17,13},
{0,100,91,83,75,68,63,56,51,46,41,37,29,22,16,12}
};
int ddline[15] = {0,2,4,6,8,10,12,14,16,18,20,25,30,35,40};
char *
relhum(temp, dewpt)
{
int del1,del2,dd, i, j;
static char val[20];
        dd = temp - dewpt;
        if (dd < 0)
		return("??");
        if (dd > 40)
		return("dry");
        temp = ((temp+2)/5)*5;
        for (i=0;i<15;i++) {
                if (dd <= ddline[i]) {
                        del1 = ddline[i]-dd;
                        del2 = ddline[i]-ddline[i-1];
                        break;
                }
        }
        for(j=0;j<22;j++) {
                if (j==21)
			return("??");
                else if (tab[j].temp == temp) {
			sprintf(val, "%d",
                        tab[j].rh[i]+((del1*(tab[j].rh[i-1]-tab[j].rh[i]))/del2));
			return(val);
                }
        }
}
static int nr=0;
record(s)
	char *s;
{
FILE *logfile;
if (nr != 0) return;
nr=1;
logfile = fopen(logname, "a");
if (logfile !=NULL)
	{
	fprintf(logfile, "weather %s\t%s\t%s", s, getlogin(), ctime(&ftb.time));
	fclose(logfile);
	}
}
static char nname[100];
char *
normalize(s)
	char *s;
{
/* takes a town name and makes it right format */
char *p;
while (*s==' ')s++;
lcase(s);
for (p=nname; *s; s++)
	{
	if (*s==',')
		{
		*p++=',';
		*p++=' ';
		while (s[1]==' ')s++;
		continue;
		}
	*p++ = *s;
	}
*p=0;
return(nname);
}
char *nxword();
printfile(f)
	FILE *f;
{
int seendate=0, seendot=0, seenfore=0, k;
char line[200], wd[100], *p, *np;
while (fgets(line, 200,f))
	{
	trimnl(line);
	if (seenfore) {seendate=1; seendot=1; seenfore=0;}
	for(p=line; np=nxword(p,wd); p=np)
		{
		if (abbrev(wd))
			;
		else
		if (k=propname(p))
			{
			lowercase(p+1, k, 1);
			np=p+k;
			}
		else
		if (seendate==0 || seendot==1)
			lowercase(p+1, np-p-1, 0);
		else
			lowercase(p, np-p, 0);
		if (strcmp(wd, "1982")==SAME)
			seendate=1;
		if (strcmp(wd, "FORECAST")==SAME)
			seenfore=1;
		if (strcmp(wd, ".")==SAME)
			seendot=1;
		else
			seendot=0;
		}
	puts(line);
	}
}
lowercase(p, n, k)
	char *p;
{
for (; n-- >0; p++)
	if (isupper(*p) && (k==0 || p[-1]!=' '))
		*p= tolower(*p);
}
char *
nxword(p, b)
	char *p, *b;
{
*b=0;
if (p==NULL || *p==NULL) return(NULL);
if (!isalpha (*b++ = *p++))
	{
	*b=0;
	while (isspace(*p))
		p++;
	return(p);
	}
while (isalpha(*p))
	*b++ = *p++;
*b=0;
while (isspace(*p))
	p++;
return(p);
}
abbrev(s)
	char *s;
{
char *p;
for(p=s; *p; p++)
	if (!isalpha(*p))
		return(1);
for(p=s; *p; p++)
	switch(*p)
		{
		case 'A': case 'E': case 'I': case 'O': case 'U':
			return(0);
		case 'Y':
			if (p[1]==0) return(0);
		}
return(1);
}
char wspace[4000];
char *wsp=wspace;
char *words[500];
int nwd=0;
propname(s)
	char *s;
{
char **wp;
int cn;
initlist();
for(wp=words; wp<words+nwd; wp++)
	{
	if (strncmp(*wp, s, cn=strlen(*wp))==SAME &&
		!isalpha(s[cn]) )
		return(cn);
	}
return(0);
}
initlist()
{
static int once=0;
FILE *f;
if (once>0) return;
once++;
f = fopen("/usr/spool/weather/propnames", "r");
while (fgets(wsp, 200, f))
	{
	trimnl(wsp);
	words[nwd++]=wsp;
	wsp+=strlen(wsp)+1;
	}
fclose(f);
}
