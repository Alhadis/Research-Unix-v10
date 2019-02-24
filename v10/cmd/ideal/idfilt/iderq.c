#include <jerq.h>

#define	TRUE	1
#define	FALSE	0

extern getdots();
extern putdots();
extern void idmaxx ();
extern void idmaxy ();
extern void idminx ();
extern void idminy ();
extern void idwidth ();
extern void idcolwid ();
extern char *scanstr();
extern char *scanfloat();
extern idline ();
extern idarc ();
extern idleft ();
extern idcenter ();
extern idright ();
extern idspline ();
extern idknot ();
extern idendspline ();
extern yeserase ();
extern noerase();
extern idendbound();

char input[10000];
char *ip = input;

long maxx, maxy;
long minx, miny;
long width = 4;
long colwid = 6;
long scale;

#define	boolean	int

boolean maxxset, maxyset;
boolean minxset, minyset;
boolean widset, colset;
boolean boundset;

boolean veryfirst = TRUE;

boolean wanterase = TRUE;

POINT textpos;
char temp[100];

print (s)
char *s;
{
	jmoveto(textpos);
	jstring ("*");
	jstring(s);
	jstring ("*");
	textpos.x = 30;
	textpos.y += 15;
}

main (argc, argv)
int argc;
char *argv[];
{
	jinit();
	qinit();
	binit();
	aciainit();
	spl0();
	jBonW();
	cursinhibit();
textpos.x = 30;
textpos.y = XMAX+20;
	for (;;) {
		getdots();
		if (wanterase == TRUE)
			jrectf(screenmap.rect, F_CLR);
		putdots();
	}
}

getdots()
{
	register char *start, *ip;
	ip = input;
	jputchar('*');
	do {
		start = ip;
		while ((*ip++ = jgetchar()) != '\n')
			jputchar('*');
	} while (strncmp (start, ".IS", 3));
	ip = input;
	do {
		start = ip;
		while ((*ip++ = jgetchar()) != '\n')
			jputchar('*');
		*ip = '\0';
	} while (strncmp (start, ".IE", 3) && strncmp (start, ".IF", 3));
	maxxset = minxset = FALSE;
	maxyset = minyset = FALSE;
	widset = colset = boundset = FALSE;
}

#define	getfloats(n)	for (i = 0; i < n; i ++) ip = scanfloat (&f[i], ip);

putdots()
{
	register char *ip;
	char cmd[10];
	long f[10];
	int i;
	POINT pos;
	char *ip2;

	ip = input;
	while (*ip) {
		ip = scanstr(cmd,ip);
		if (!boundset) {
			if (strcmp (cmd, "...maxx") == 0) {
				getfloats(1);
				idmaxx (f[0]);
			} else if (strcmp (cmd, "...maxy") == 0) {
				getfloats(1);
				idmaxy (f[0]);
			} else if (strcmp (cmd, "...minx") == 0) {
				getfloats(1);
				idminx (f[0]);
			} else if (strcmp (cmd, "...miny") == 0) {
				getfloats(1);
				idminy (f[0]);
			} else if (strcmp (cmd, "...width") == 0) {
				getfloats(1);
				idwidth (f[0]);
			} else if (strcmp (cmd, "...colwid") == 0) {
				getfloats(1);
				idcolwid (f[0]);
			} else if (strcmp (cmd, "...obbox") == 0) {
				if (!veryfirst) {
					maxxset = maxyset = TRUE;
					minxset = minyset = TRUE;
					boundset = TRUE;
				}
			} else if (strcmp (cmd, "...noerase") == 0) {
				idnoerase ();
			} else if (strcmp (cmd, "...yeserase") == 0) {
				idyeserase ();
			} else {
				idendbound ();
				veryfirst = FALSE;
			}
		}
		if (boundset) {
			if (strcmp (cmd, "...line") == 0) {
				getfloats(4);
				idline (f[0], f[1], f[2], f[3]);
			} else if (strcmp (cmd, "...circle") == 0) {
				getfloats(3);
				idcircle (f[0], f[1], f[2]);
			} else if (strcmp (cmd, "...arc") == 0) {
				getfloats(9);
				idarc (f[0], f[1], f[2], f[3], f[4], f[5], f[6], f[7], f[8]);
			} else if (strcmp (cmd, "...left") == 0) {
				getfloats(2);
				ip2 = ip;
				while (*ip2++ != '\n')
					;
				*--ip2 = '\0';
				idleft(f[0],f[1],ip);
				ip = ip2+1;
			} else if (strcmp (cmd, "...center") == 0) {
				getfloats(2);
				ip2 = ip;
				while (*ip2++ != '\n')
					;
				*--ip2 = '\0';
				idcenter(f[0],f[1],ip);
				ip = ip2+1;
			} else if (strcmp (cmd, "...right") == 0) {
				getfloats(2);
				ip2 = ip;
				while (*ip2++ != '\n')
					;
				*--ip2 = '\0';
				idright(f[0],f[1],ip);
				ip = ip2+1;
			} else if (strcmp (cmd, "...spline") == 0) {
				getfloats(2);
				idspline (f[0], f[1]);
			} else if (strcmp (cmd, "...knot") == 0) {
				getfloats(2);
				idknot (f[0], f[1]);
			} else if (strcmp (cmd, "...endspline") == 0) {
				idendspline ();
			}
		}
	}
}

void idmaxx (x)
long x;
{
	if (!maxxset) {
		maxx = x;
		maxxset = TRUE;
	}
}

void idmaxy (y)
long y;
{
	if (!maxyset) {
		maxy = y;
		maxyset = TRUE;
	}
}

void idminx (x)
long x;
{
	if (!minxset) {
		minx = x;
		minxset = TRUE;
	}
}

void idminy (y)
long y;
{
	if (!minyset) {
		miny = y;
		minyset = TRUE;
	}
}

void idwidth (wid)
long wid;
{
	if (!widset) {
		width = wid;
		widset = TRUE;
	}
}

void idcolwid (wid)
long wid;
{
	if (!colset) {
		colwid = wid;
		colset = TRUE;
	}
}

char *scanstr(object, source)
char *object, *source;
{
	while (*source != '\0' && (*source == ' ' || *source == '\n'))
		source++;
	while (*source != '\0' && *source != ' ' && *source != '\n')
		*object++ = *source++;
	*object = '\0';
	return (source);
}

#define isdigit(ch)	((ch) >= '0' && (ch) <= '9')
#define	PRECISION	1000

char *scanfloat(f, source)
long *f;
char *source;
{
	int i;
	boolean negflag;
	long part[2];
	int len[2];
	long n;
	negflag = FALSE;
	while (!isdigit(*source) && *source != '.' && *source != '-')
		source++;
	if (*source == '-') {
		negflag = TRUE;
		source ++;
	}
	part[0] = part[1] = 0;
	len[0] = len[1] = 0;
	for (i = 0; i < 2; i++) {
		while (isdigit(*source)) {
			part[i] = 10*part[i] + *source++ - '0';
			len[i]++;
		}
		if (*source != '.')
			break;
		source++;
	}
	n = 1;
	for (i = 0; i < len[1]; i ++)
		n *= 10;
	*f = part[0]*PRECISION + (PRECISION*part[1])/n;
	if (negflag)
		*f *= -1;
	return (source);
}

#define	jx(x)	XMAX*((x)-minx)/scale
#define	jy(y)	XMAX*(maxy-(y))/scale

idline (x0, y0, x1, y1)
long x0, y0, x1, y1;
{
	POINT p;
	p.x = jx(x0);
	p.y = jy(y0);
	jmoveto(p);
	p.x = jx(x1);
	p.y = jy(y1);
	jlineto(p,F_STORE);
}

#define	abs(x)	((x)>0?(x):-(x))
	
idcircle (x0, y0, r)
long x0, y0, r;
{
	POINT p;
	p.x = jx(x0);
	p.y = jy(y0);
	jcircle(p, (short)(XMAX*abs(r)/scale), F_STORE);
}

idarc (x0, y0, x1, y1, x2, y2, t1, t2, r)
long x0, y0, x1, y1, x2, y2, t1, t2, r;
{
	POINT p0, p1, p2;
	p0.x = jx(x0);
	p0.y = jy(y0);
	p1.x = jx(x1);
	p1.y = jy(y1);
	p2.x = jx(x2);
	p2.y = jy(y2);
	jarc (p0, p2, p1, F_XOR);
}

idleft (x0, y0, s)
long x0, y0;
char *s;
{
	POINT p;
	p.x = jx(x0);
	p.y = jy(y0);
	jmoveto(p);
	jstring(s);
}

idcenter (x0, y0, s)
long x0, y0;
char *s;
{
	POINT p;
	p.x = jx(x0) - strwidth(s)/2;
	p.y = jy(y0);
	jmoveto(p);
	jstring(s);
}

idright (x0, y0, s)
long x0, y0;
char *s;
{
	POINT p;
	p.x = jx(x0) - strwidth(s);
	p.y = jy(y0);
	jmoveto(p);
	jstring(s);
}

idspline (x0,y0)
long x0,y0;
{
}

idknot (x0,y0)
long x0,y0;
{
}

idendspline ()
{
}

idnoerase ()
{
	wanterase = FALSE;
}

idyeserase ()
{
	wanterase = TRUE;
}

idendbound ()
{
	if (boundset)
		return;
	boundset = TRUE;
	idmaxx ((long)6);
	idminx ((long)-6);
	idmaxy ((long)6);
	idminy ((long)-6);
	scale = maxx - minx;
	if (scale < maxy - miny)
		scale = maxy - miny;
}
