# include "stdio.h"
# include "cbt.h"
# include "ctype.h"
# include "assert.h"
# include "math.h"
# include "/usr/weather/progs/weath.h"
# define SAME 0

char gdport[200], gdhap[200];
char *strchr();
double md=1000., dist(), xabs();
int abear;
bfile *bf;
bfile *happenfile;
char *direct[] =
{"N", "NE", "E", "SE", "S", "SW", "W", "NW", "N", "xx", "yy"};

main(argc,argv)
	char *argv[];
{
char line[100], request[100], rka[100], rra[300], from[100], fromloc[100];
char sna[100], *s, *excite();
mbuf key, rkey, rrec;
bfile *townfile;
FILE *fpat, *fair, *hpat, *hap;
double lat, lng, fromlat, fromlng;
int a1, a2, b1, b2, x, y, townfind, nf, j;
townfile = bopen("//usr/spool/town/ustowns", 0);
if (townfile==NULL)
	{
	fprintf(stderr, "No town list - is /crp mounted?\n");
	exit(1);
	}
happenfile = bopen("//usr/spool/town/happen", 0);
assert(happenfile!=NULL);
bf = bopen("/usr/spool/weather/air", 0);
assert(bf!=NULL);
fpat = fopen("/usr/spool/town/cityindex", "r");
assert(fpat!=NULL);
fair = fopen("/usr/spool/town/citypatch", "r");
assert(fair!=NULL);
hpat = fopen("/usr/spool/town/hapindex", "r");
hap = fopen("/usr/spool/town/haplist", "r");
rkey.mdata = rka; rrec.mdata = rra;
nf=0;
if (argc>1)
	concat (from, argc, argv);
else
	strcpy(from, "murray hill, nj");
lcase(from);
key.mdata = from; key.mlen = strlen(from);
bseek(townfile, key);
while (bread(townfile, &rkey, &rrec)==NULL)
	{
	if (strncmp(from, rkey.mdata, key.mlen)==SAME)
		{
		rrec.mdata[rrec.mlen]=0;
		rkey.mdata[rkey.mlen]=0;
		strcpy(fromloc, rrec.mdata);
		strcpy(request, rkey.mdata);
		nf++;
		}
	else
		break;
	}
if (nf<1)
	printf("No such place: %s\n", from);
else if (nf>1)
	printf("More than one such place: %s\n", from);
else
	{
	sscanf(fromloc, "%lf%lf", &fromlat, &fromlng);
	caps(request);
	printf("   [Reference point: %s (%.3f N, %.3f W)]\n", request, fromlat, fromlng);
	}
while (gets(request))
	{
	if (strlen(request)==0)
		continue;
	if (strcmp(request, "q")==SAME)
		break;
	lcase(request);
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
		printf("%s: (%.3f N, %.3f W)", rka, lat, lng);
		strcpy(sna, rka);
		if (nf==1)
			printf(" %.1f miles %s",
				dist(fromlat, fromlng, lat, lng),
				direct[bearing(fromlat, fromlng, lat, lng)]);
		printf("\n");
		printf("Zip code %5.5s.  ", rra+15);
		for(s=rra; *s; s++)
			;
		s-=3;
		switch(*s)
			{
			case '0': puts("Less than 2500 people."); break;
			case '2': puts("2,500 to 5,000 people."); break;
			case '3': puts("Between 5,000 and 10,000 people."); break;
			case '4': puts("10,000 to 25,000 people."); break;
			case '5': puts("25,000 to 50,000 people."); break;
			case '6': puts("50,000 to 100,000 people."); break;
			case '7': puts("Between 100,000 and 250,000 people."); break;
			case '8': puts("Population over 250,000 but below 500,000."); break;
			case '9': puts("Population exceeds 500,000."); break;
			default: puts(""); break;
			}
		a1 = lat/4;
		/* floating remainder not defined, so... */
		if ( lat - 4.0*a1 >2.0)
			a2 = a1+1;
		else
			a2 = a1-1;
		b1 = lng/4;
		if (lng - 4.0*b1 > 2.0)
			b2 = b1+1;
		else
			b2 = b1-1;
		if (*s!= '8' && *s!='9')
			{
			rewind(fpat);
			while (fgets(line, 100, fpat))
				{
				x = atoi(line);
				y = atoi(line+3);
				if ((x==a1 || x==a2) && (y==b1 || y==b2))
					ckair(line+6, lat, lng, fair);
				}
			if (md<1000.)
				{
				caps(gdport+6);
				if (strncmp(rka, gdport+6, strlen(rka)))
				printf(" Nearest city: %s, %5.1f miles %s\n",
				gdport+6, md, direct[abear]);
				}
			}
		weather(lat, lng);
		lcase(rka);
		j = bseek(happenfile, rkey);
		if (j==1)
			{
			bread(happenfile, NULL, &rrec);
			rrec.mdata[rrec.mlen]=0;
			bput(rrec.mdata);
			}
		else
			{
			printf("Nothing much has happened lately in %s.\n", sna);
			rewind (hpat);
			md=1000.;
			if (xabs(lat-(4.0*a1+2.0)) <= 1.80)
				a2=a1;
			if (xabs(lng-(4.0*b1+2.0)) <= 1.80)
				b2=b1;
			while (fgets(line, 100, hpat))
				{
				x = atoi(line);
				y = atoi(line+3);
				if ( (x==a1 || x==a2) && (y==b1 || y==b2))
					ckcit (line+6, lat, lng, hap);
				}
			if (md<1000.)
				{
				strcpy(rka, gdhap+6);
				caps(gdhap+6);
				rkey.mlen = strlen(rka);
				j = bseek (happenfile, rkey);
				if (j==1)
					{
					bread(happenfile, NULL, &rrec);
					if (md>0)
					printf("But, %.1f miles away in %s %s, \n", md, excite(md), gdhap+6);
					else
					printf("But, nearby in %s %s, \n", excite(md+1.5), gdhap+6);
					rrec.mdata[rrec.mlen]=0;
					bput(rrec.mdata);
					}
				}
			}
		printf("\n");
		}
	if (townfind==0)
		fprintf(stderr, "Town name not in our file\n");
	}
exit(0);
}
bput(s)
	char *s;
{
int col=0;
char buff[2048], *p;
p=buff;
while (*s)
	{
	if (col==0)
		{
		while (col++ < 5)
			*p++ = ' ';
		}
	if (*s==' ' && col>60)
		{
		*p++ = '\n';
		col=0;
		}
	else
		{
		*p++ = *s;
		col++;
		}
	s++;
	}
*p=0;
printf("%s", buff);
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
ckair(pos, lat, lng, fair)
	FILE *fair;
	char *pos;
	double lat, lng;
{
char airport[200], opat[5], *s;
long fpt, atol();
double alat, alng, atof(), d;
fpt = atol(pos);
fseek (fair, fpt, 0);
opat[0]=0;
while (fgets(airport, 200, fair))
	{
	if (opat[0] && strncmp(opat, airport, 5)!=SAME)
		break;
	strncpy(opat, airport, 5);
	for(s=airport; *s!='\t'; s++)
		;
	*s++ = 0;
	alat= atof(s);
	while (*s!=' ')s++;
	alng = atof(++s);
	while (*s++ != ' ');
	d = dist(lat, lng, alat, alng);
	if (md>d)
		{
		md=d;
		strcpy(gdport, airport);
		abear = bearing (lat, lng, alat, alng);
		}
	}
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
ckcit(pos, lat, lng, fair)
	FILE *fair;
	char *pos;
	double lat, lng;
{
char citline[200], opat[5], *s;
long fpt, atol();
double alat, alng, atof(), d;
fpt = atol(pos);
fseek (fair, fpt, 0);
opat[0]=0;
while (fgets(citline, 200, fair))
	{
	if (opat[0] && strncmp(opat, citline, 5)!=SAME)
		break;
	strncpy(opat, citline, 5);
	for(s=citline; *s!='\t'; s++)
		;
	*s++ = 0;
	if (!ishap(s))continue;
	alat= atof(s);
	while (*s!=' ')s++;
	alng = atof(++s);
	while (*s++ != ' ');
	d = dist(lat, lng, alat, alng);
	if (md>d)
		{
		md=d;
		strcpy(gdhap, citline);
		}
	}
}
ishap(s)
	char *s;
{
while (*s)s++;
if (s[-3]=='x') return(1);
else return(0);
}
double
xabs(x)
	double x;
{
return (x>= 0. ? x : -x);
}
char *
excite(x)
	double x;
{
/* choose at random from a list of words */
switch(( (int)x)%5)
	{
	case 0: return("exciting");
	case 1: return("swinging");
	case 2: return("nearby");
	case 3: return("the bright lights of");
	case 4: return("booming");
	}
return("exciting");
}
weather(north, west)
	double north, west;
{
double d, dist(), md=5000., tnf, twf;
char tn[10], tw[10], *x, *y, fname[100], *vis(), odata[2000], *airname();
struct wline *wp, *bwp;
int n, w, f, kbear, isair;
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
for(wp=(struct wline *)odata; (char *)wp <odata+n; wp++)
	{
	if (wp->anam[0]== '-')
		continue;
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
x = strchr(x, '	');
while (isspace(*x))x++;
for(y=x+1; *y != ','; y++)
	if (isupper(*y) && y[-1]!=' ')
		*y = tolower(*y);
y = strchr(x, '*');
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
printf("%.1f miles %s at%s %s,\n", md, direct[kbear],
	isair ? " the airport in" : "", x);
printf(" temperature %.3s, weather%s\n", bwp->wtemp, y);
}
char *
vis(s, cloud)
	char *s;
{
int d;
static char temp[50];
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
