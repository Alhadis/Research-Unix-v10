# include "stdio.h"
# include "cbt.h"
# include "ctype.h"
# include "assert.h"
# include "math.h"
# include "/usr/theo/PLOT/pplot.h"
# define SAME 0
# define torad 0.0174532925

double dist(), atof();
double clat, clng;
double radius=50.0;
char countypat[100];
char *countyname = "/usr/spool/town/countymap";
char statepname[100];
char *statename = "/usr/spool/town/statemap";
double radlat, radlong;
int labx[500];
int laby[500];
int nlab=0;
int ctyflg=0;
int countyline=0, numnames=1000;

# define abs(x) ( (t=(x))>= 0 ? t : -t )
main(argc,argv)
	char *argv[];
{
char line[100], request[100], rka[100], rra[300];
char *s, *strchr();
long l1, l2;
char nline[100];
mbuf key, rkey, rrec;
bfile *townfile;
FILE *cntypat, *countyf, *namepat, *namef, *pointf=NULL;
FILE *statepat, *state, *patch, *fdata, *bigname, *bigpat;
double lat, lng;
int townfind, x, y, j, use, a1, a2;
townfile = bopen("/usr/spool/town/ustowns", 0);
argc--; argv++;
while (argc-- > 0)
	{
	if (argv[0][0]=='-')
	switch(argv[0][1])
		{
		case 'f':
			countyname = statename = argv[1];
			argv++; argc--;
			break;
		case 'n':
			numnames = atoi(argv[0]+2);
			break;
		case 'c':
			countyline = atoi(argv[0]+2);
			ctyflg = 1;
			break;
		}
	argv++;
	}
if (townfile==NULL)
	{
	fprintf(stderr, "No town list\n");
	exit(1);
	}
sprintf(countypat, "%s.pat", countyname);
sprintf(statepname, "%s.pat", statename);
cntypat = fopen(countypat, "r");
assert(cntypat!=NULL);
countyf = fopen(countyname, "r");
assert(countyf!=NULL);
namepat = fopen("/usr/spool/town/ftownpat", "r");
assert(namepat!=NULL);
namef = fopen("/usr/spool/town/ftown", "r");
assert(namef!=NULL);
bigpat = fopen("/usr/spool/town/bigpat", "r");
assert(bigpat!=NULL);
bigname = fopen("/usr/spool/town/big", "r");
assert(bigname!=NULL);
state = fopen(statename, "r");
assert(state!=NULL);
statepat = fopen(statepname, "r");
assert(statepat!=NULL);
rkey.mdata = rka; rrec.mdata = rra;
plopen("");
while (gets(request))
	{
	s = strchr(request, '\t');
	if (s!=NULL)
		{
		*s++ = 0;
		radius= atof(s);
		s = strchr(s, ' ');
		if (s!=NULL)
			{
			while (*s==' ')
				s++;
			if (*s)
				pointf = fopen(s, "r");
			}
		}
	if (strlen(request)==0)
		continue;
	lcase(request);
	townfind=0;
	if (ctyflg == 0) countyline = (radius<100.);
	nlab=0;
	key.mdata = request; key.mlen = strlen(request);
	bseek(townfile, key);
	while (bread(townfile, &rkey, &rrec)==NULL)
		{
		rkey.mdata[rkey.mlen]=0;
		rrec.mdata[rrec.mlen]=0;
		if (strncmp(rkey.mdata, request, key.mlen)!=SAME)
			break;
		sscanf(rra, "%lf%lf", &lat, &lng);
		townfind++;
		}
	if (townfind==0)
		{
		fprintf(stderr, "Can't find that place\n");
		continue;
		}
	if (townfind>1)
		{
		fprintf(stderr, "Ambiguous - specify state, please\n");
		continue;
		}
	perase();
	prange (-5000, 5000, -5000, 5000);
	caps(rka);
	clat = lat; clng=lng;
	/* standard correction latitude = 40.0 N*/
	clng = clng * cos(torad*40.0);
	radlat = radius/dist(clat, clng, clat+1., clng);
	radlong = radius*cos(torad*40.0)/dist(clat, clng, clat, clng+1.);
	a1 = lat/4.; a2 = lng/4.;
	use=0;
	if (countyline)
		{
		patch = cntypat; fdata = countyf;
		}
	else
		{
		patch = statepat; fdata = state;
		}
	rewind(patch);
	fgets(line, 100, patch);
	while (fgets(nline, 100, patch))
		{
		x = atoi(line);
		y = atoi(line+3);
		if (okpat(x, y, lat, lng))
			{
			ctydraw( atol(line+6), atol(nline+6), fdata);
			use++;
			}
		if (x==a1 && y==a2)
			{l1 = atol(line+6); l2 = atol(nline+6);}
		strcpy(line, nline);
		}
	if (use==0)
		ctydraw (l1, l2, fdata);
	lcase(rka);
	if (radius > 200.)
		{
		patch = bigpat; fdata = bigname;
		}
	else
		{
		patch = namepat; fdata = namef;
		}
	if (patch!=NULL && numnames>0)
		{
		rewind(patch);
		use=0;
		fgets(line, 100, patch);
		while (fgets(nline, 100, patch))
			{
			x = atoi(line);
			y = atoi(line+3);
			if (okpat(x,y, lat, lng))
				{
				ckcit (atol(line+6), atol(nline+6), fdata);
				use++;
				}
			if (a1==x && a2==y)
				{l1 = atol(line+6); l2 = atol(nline+6);}
			strcpy(line, nline);
			}
		if (use==0)
			{
			ckcit( l1, l2, fdata);
			}
		}
	if (pointf!=NULL)
		{
		pltpts(pointf);
		fclose(pointf);
		}
	fflush(stdout);
	}
exit(0);
}
okpat(ax, ay, lat, lng)
	double lat, lng;
{
double d;
d = dist(ax*4., ay*4., lat, lng);
if (d<radius) return(1);
d = dist((ax+1)*4., ay*4., lat, lng);
if (d<radius) return(1);
d = dist(ax*4., (ay+1)*4., lat, lng);
if (d<radius) return(1);
d = dist((ax+1)*4., (ay+1)*4., lat, lng);
if (d<radius) return(1);
return(0);
}
ctydraw (pos1, pos2, ff)
	FILE *ff;
	long pos1, pos2;
{
float a[4];
int xa, ya, xb, yb;
register int t;
double alat, alng, blat, blng;
fseek (ff, pos1, 0);
while (fread (a, 4, sizeof(float), ff))
	{
	if (ftell(ff)>= pos2) break;
	alat = a[0];
	alng = a[1];
	blat = a[2];
	blng = a[3];

	xa = (clng - alng)*5000/radlong;
	ya = (alat - clat)*5000/radlat;
	xb = (clng - blng)*5000/radlong;
	yb = (blat - clat)*5000/radlat;
	if (abs(xa)<5000 && abs(xb)<5000 && abs(ya)<5000 && abs(yb)<5000)
		{
		pline(xa,ya,xb,yb);
		continue;
		}
	if ((abs(xa)<5000 && abs(ya)<5000) || (abs(xb)<5000 && abs(yb)<5000) )
		{
		partial(&xa, &ya, &xb, &yb);
		pline(xa, ya, xb, yb);
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
ckcit(pos1, pos2, countyf)
	FILE *countyf;
	long pos1, pos2;
{
char nam[50], *s;
int xa, yb, i, xthres, ythres, sz;
register int t;
double alat, alng;
float a[2];
fseek (countyf, pos1, 0);
while (fread(a, 2, sizeof(float), countyf))
	{
	if (ftell(countyf) >=pos2) break;
	alat = a[0];
	alng = a[1];
	sz = getc(countyf);
	s=nam;
	while (i= getc(countyf))
		{
		if (i=='\n' || i==0) break;
		*s++ = i;
		}
	*s=0;
	switch (sz)
		{
		case '9': xthres=0; ythres=0; break;
		case '8': xthres=500; ythres=150; break;
		case '7': xthres=1000; ythres=300; break;
		case '6': xthres=1500; ythres=400; break;
		default : xthres=2000; ythres=400; break;
		}
	xa = (clng-alng)*5000/radlong;
	yb = (alat-clat)*5000/radlat;
	if (abs(xa)<5000 && abs(yb)<5000)
		{
		for(i=0; i<nlab; i++)
			{
			if (abs(xa-labx[i])<xthres && abs(yb-laby[i])<ythres)
				break;
			}
		if (i<nlab) continue;
		labx[nlab]=xa; laby[nlab]=yb;
		nlab++;
		assert(nlab<500);
		ppoint(xa,yb);
		caps(nam);
		ptext(nam);
		}
	if (nlab>numnames) break;
	}
}
pltpts(fi)
	FILE *fi;
{
char ln[100], *s;
double alat, alng;
int x, y;
register int t;
while (fgets(ln, 100, fi))
	{
	trimnl(s=ln);
	while (isspace(*s)) s++;
	alat = atof(s);
	while (*s && !isspace(*s))s++;
	while (isspace(*s))s++;
	alng = atof(s);
	while (*s && !isspace(*s)) s++;
	while (isspace(*s)) s++;
	if (s==NULL || alat <0.0 || alng < 0.0)
		continue;
	alng = alng * cos(torad*40.0); /* crummy map projection */
	x = (clng - alng)*5000/radlong;
	y = (alat - clat)*5000/radlat;
	if (abs(x)<5000 && abs(y)<5000)
		{
		pmove(x,y);
		ptext(s);
		}
	}
}
partial (x0p, y0p, x1p, y1p)
	int *x0p, *y0p, *x1p, *y1p;
{ /* pick part of line within 5000 limit */
int ax, ay, bx, by, cx, cy;
double a, b; /* y= ax + b*/
register int t;
ax = *x0p; ay= *y0p; bx = *x1p; by = *y1p;
if (abs(ax)>5000 || abs(ay)>5000)
	{
	cx = ax; ax = bx; bx = cx;
	cy = ay; ay = by; by = cy;
	}
/* ax, ay is inside; bx, by is outside */
if (ax == bx) /* vertical line*/
	{
	if (by>5000) by=5000;
	else
	if (by< -5000) by= -5000;
	}
else
if (ay == by) /* horizontal line */
	{
	if (bx>5000) bx=5000;
	else
	if (bx< -5000) bx= -5000;
	}
else
	{/* normal case, with slope */
	a = ( (double) (by-ay) ) / (bx-ax);
	b = ay - a*ax;
	if (bx>5000)
		{
		bx=5000;
		by = a*bx+b;
		}
	else
	if (bx< -5000)
		{
		bx = -5000;
		by = a*bx+b;
		}
	/* now bx is in range.  what about by */
	if (by> 5000)
		{
		by = 5000;
		bx = (by -b)/a;
		}
	else
	if (by< -5000)
		{
		by = -5000;
		bx = (by - b) /a;
		}
	}
*x0p = ax;
*x1p = bx;
*y0p = ay;
*y1p = by;
}
trimnl(s)
	char *s;
{
while (*s)s++;
if (*--s== '\n')*s=0;
}
