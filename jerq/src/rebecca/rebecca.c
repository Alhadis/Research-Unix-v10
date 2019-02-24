#include	<stdio.h>
#include	<sgtty.h>
#include	"/usr/jerq/include/jioctl.h"
#include	"rebecca.h"
#include	"piclib.h"

struct sgttyb sttybuf, sttysave;

quit()
{	ioctl(0, JTERM, (struct sgttyb *)0);
	ioctl(0, TIOCSETP, &sttysave);
	exit(0);
}

int How=1, xoff=SZ;
int gave[SZ];			/* resolution of lines sent */
int r_side[SZ], l_side[SZ];	/* offset of lines sent */
unsigned char buf[SZ*SZ];	/* the image */

unsigned char dirty[SZ];	/* lines changed since last putram */
unsigned char baf[SZ], bof[SZ];
unsigned char curval=0;
unsigned char ramtek = 0;	/* ramtek device is open or not */
unsigned char canram = 0;	/* can open ramtek device remote */
char *malloc();

struct pfile image;

putitback()
{	register unsigned char *s, *z;
	register int i, j;
	int ll, ht;

	ll = image.r.corner.x - image.r.origin.x;
	ht = image.r.corner.y - image.r.origin.y;
	for (i = 0; i < SZ && i < ht; i++)
	{	z = &(image.pixred[i*ll]);
		s = &(buf[i*SZ]);
		for (j = 0; j < SZ && j < ll; j++)
			*z++ = *s++;
	}
	return dumpf(image.name, &image);
}

reopen(str)
	char *str;
{
	int fd, ll, ht;
	register unsigned char *s, *z;
	register int i, j;

	if((fd = openf(str, &image)) == -1 || !readf(fd, &image))
		return 0;

	ll = image.r.corner.x - image.r.origin.x;
	ht = image.r.corner.y - image.r.origin.y;
	z = buf;
	for (i = 0; i < SZ; i++)
	{	s = &(image.pixred[i*ll]);
		for (j = 0; j < SZ; j++)
			*z++ = (j < ll && i < ht) ? *s++ : 255;
	}
	close(fd);

	for (i = 0; i < SZ; i++)
	{	gave[i] = 2*SZ;
		l_side[i] = SZ;
		r_side[i] = 0;
		dirty[i] = 0;
	}
	return 1;
}

salvage(r)
{	int i, ex=xoff+SZ/r;

	for (i = 0; i < SZ; i++)
		if ((i < xoff || i > ex) && i%r)
		{	gave[i] = 2*SZ;
			l_side[i] = SZ;
			r_side[i] = 0;
		}
}

Usage(str)
	char *str;
{	fprintf(stderr, "rebecca: %s\nusage: rebecca [-r] file\n", str);
	fprintf(stderr, "\t-r opens ramtek device\n");
	exit(1);
}

main(argc, argv)
	char **argv;
{	int i, x, y;
	int fd, n=1;
	char c;

	if (argc > 1 && argv[1][0] == '-')
	{	while ((c = argv[1][n++]) != '\0')
		{	switch (c) {
			case 'r': canram = 1; break;
			default : Usage("unknown option"); break;
			}
		}
		argv++; argc--;
	}
	if (argc < 2)
		Usage("missing argument");

	if ((fd = openf(argv[1], &image)) == -1)
		Usage("bad image file");
	else
	{	if (image.nchan != 1)
		{	fprintf(stderr, "warning: %d-channel image ", image.nchan);
			fprintf(stderr, "(only editing the red channel)\n");
		}
		if (image.r.corner.x - image.r.origin.x > 512
		||  image.r.corner.y - image.r.origin.y > 512)
		{	fprintf(stderr, "warning: oversized image (only ");
			fprintf(stderr, "editing a (0,0,512,512) window)\n");
		}
		sleep(6);
	}
	close(fd);
	loadjerq();

	for(;;)
	{	switch (getchar()) {
		case 'i':	initram(); putram(0, SZ-1); break;

		case '-':	How = 0; curval = (getchar()&255); break;
		case 'v':	How = 1; curval = (getchar()&255); break;
		case '+':	How = 2; curval = (getchar()&255); break;

		case 'a':	salvage(getn()); break;

		case 'b':	y=getn(); x=getn(); i = y*SZ+x;
				if (i < 0 || i > SZ*SZ) break;
				switch (How) {
				case 0:	buf[i] = min(255, (buf[i]+curval)); break;
				case 1:	buf[i] = 255-curval; break;
				case 2:	buf[i] = (buf[i]>curval)?buf[i]-curval:0;
						 break;
				}
				dirty[y] = 1;
				break;
		case 'o':	c = (reopen(argv[1]))?'1':'0';
				write(1, &c, 1);
				if (c == '1')
					putram(0, SZ-1);
				break;
		case 'p':	shovel(); break;
		case 'q':	quit();
		case 'r':	crunch( ); break;
		case 'R':	rubbersheet(); break;
		case 's':	y=getn(); x=getn(); i = y*SZ+x;
				if (i >= 0 && i < SZ*SZ)
					buf[i] = blend(x, y);
				dirty[y] = 1;
				break;
		case 'w':	c = (putitback())?'1':'0';
				write(1, &c, 1);
				break;
		case 'x':	xoff = getn(); break;
		case 'y':	liney(0); break;	/* straight */
		case 'Y':	liney(1); break;	/* runlength encoded */
		default:	break;
		}
	}
}

loadjerq()
{
	if (system("/usr/jerq/bin/32ld /usr/jerq/mbin/rebecca.m"))
		exit(1);

	signal(SIGQUIT, quit);
	ioctl(0, TIOCGETP, &sttysave);
	sttybuf = sttysave;
	sttybuf.sg_flags |= RAW;
	sttybuf.sg_flags &= ~ECHO;
	ioctl(0, TIOCSETP, &sttybuf);
}

liney(how)
{	int i, j, ej, x, y;
	int res, ores, magn;

	y = getn(); i = y*SZ;
	res = getn();
	magn = getchar()&1;

	if (magn)
	{	j  = xoff;
		ej = min(SZ, (xoff + SZ/res));
		if (j < l_side[y])
		{	if (ej < r_side[y])
				ej = l_side[y];
			else
				r_side[y] = ej;

			l_side[y] = j;
		} else
		{	j = r_side[y];
			r_side[y] = ej;
		}
		sendn(j);
		res = 1;
	} else
	{	j  = 0;
		ej = SZ;
	}

	if (y < 0 || y >= SZ)
	{	sendn(0);
		return;
	}
	ores = gave[y];
/*	fprintf(stderr, "line %d (%d), from %d to %d\n", y, magn, j, ej);*/
	for (x = 0; j < ej; j += res)
		if (ores == 2*SZ || j%ores)
			baf[x++] = 255-buf[i+j];

	if (how)
	{	x = runlength(x);
		sendn(x/2);
		write(1, bof, x);
	} else
	{	sendn(x);
		write(1, baf, x);
	}

	if (res > 1) gave[y] = res;
}

runlength(n)
{	register int i, j, cnt;
	unsigned char c;

	for (i = j = 0; i < n; )
	{	cnt = 1;
		c = baf[i++];
		while (i < n && c == baf[i] && cnt < 255) cnt++, i++;
		bof[j++] = (unsigned char) cnt;
		bof[j++] = c;
	}
	return j;
}

#define clamp(n)	((n<0)?0:((n>=SZ)?SZ-1:n))

blend(ptx, pty)
{	register x, y, z=0;

	for (y = -1; y <= 1; y++)
	for (x = -1; x <= 1; x++)
		z += buf[clamp(pty+y)*SZ+clamp(ptx+x)];
	z /= 9;
	return (z<0)?0:(z>255)?255:z;
}

crunch()
{	register int y, si, ei;
	int sx, ex, sy, ey;

	sy = max(0, getn());  sx = max(0, getn());
	ey = min(SZ, getn()); ex = min(SZ, getn());

	for (y = sy; y < ey;  y++)
	{	si = y*SZ+sx; ei = y*SZ+ex;
		switch (How) {
		case 0:	for ( ; si < ei; si++)
				buf[si] = min(255, (buf[si]+curval));
			break;
		case 1:	while (si < ei)
				buf[si++] = 255-curval;
			break;
		case 2:	for ( ; si < ei; si++)
				buf[si] = (buf[si]>curval)?buf[si]-curval:0;
			break;
		}
	}
	putram(sy, ey-1);
}

getn()
{	register n = getchar();

	n <<= 8;
	n |= getchar();
	return(n&0xFFFF);
}

send(n)
{	unsigned char c = n;
	write(1, &c, 1);
}

sendn(n)
{	send(n>>8);
	send(n&255);
}

unsigned char *saver;
int W=0, D=0;
Point *qmap;

rubbersheet()
{	Rectangle j;
	Point a, b, c, d, tp;
	int n, m, i, miny, maxy, minx, maxx;

	a.x = getn(); a.y = getn();
	b.x = getn(); b.y = getn();
	c.x = getn(); c.y = getn();
	d.x = getn(); d.y = getn();
	j.origin.x = getn(); j.origin.y = getn();
	j.corner.x = getn(); j.corner.y = getn();

	miny = maxy = a.y;
	if (b.y < miny) miny = b.y; else if (b.y > maxy) maxy = b.y;
	if (c.y < miny) miny = c.y; else if (c.y > maxy) maxy = c.y;
	if (d.y < miny) miny = d.y; else if (d.y > maxy) maxy = d.y;
	minx = maxx = a.x;
	if (b.x < minx) minx = b.x; else if (b.x > maxx) maxx = b.x;
	if (c.x < minx) minx = c.x; else if (c.x > maxx) maxx = c.x;
	if (d.x < minx) minx = d.x; else if (d.x > maxx) maxx = d.x;

	for (i = miny; i <= maxy; i++)
	{	gave[i] = 2*SZ;
		if (maxx < l_side[i] || minx > r_side[i])
			continue;

		if (maxx-r_side[i] > l_side[i]-minx)
			l_side[i] = maxx;
		else
			r_side[i] = minx;
	}

	W = j.corner.x - j.origin.x;
	D = j.corner.y - j.origin.y;

	saver = (unsigned char *) malloc(W*D*sizeof(unsigned char));
	qmap  = (Point *) malloc(W*D*sizeof(Point));
	if (!saver || !qmap) return;

	for (n = j.origin.y, i=0; n < j.corner.y; n++)
	for (m = j.origin.x; m < j.corner.x; m++, i++)
		saver[i] = buf[n*SZ+m];

	quadrimap(a, b, c, d);
	putram(miny, maxy);

	free(saver);
}

shovel()
{	register int i;

	for (i = 0; i < SZ; i++)
		if (dirty[i]) putram(i, i);
}

initram()
{	int cm[SZ];
	register int i;

	if (ramtek || !canram) return;

	openrm();
	for (i = 0; i < 512; i++)
		cm[i] =i;
	lam(512, cm);

	ramtek = 1;
}

putram(sy, ey)
{	int i, y;
	short w[SZ];

	if (!ramtek) return;

	for (y = sy; y <= ey; y++)
	{	for (i = 0; i < SZ; i++)
			w[i] = (short) buf[y*SZ+i];
		wi(w, SZ, 1, 0, y);
		dirty[y] = 0;
	}
}

quadrimap(a, b, c, d)
	Point a, b, c, d;
{
	Point sp, ep;
	float dx, dy;
	int i, j, k, g, h, ix, iy;

	for (j = k = 0; j < D; j++)
	{	dy = (float) j / (float) D;

		sp.x = a.x + (int) (dy * (float)(d.x - a.x));
		sp.y = a.y + (int) (dy * (float)(d.y - a.y));
		ep.x = b.x + (int) (dy * (float)(c.x - b.x));
		ep.y = b.y + (int) (dy * (float)(c.y - b.y));

		for (i = 0; i < W; i++, k++)
		{	dx = (float) i / (float) W;

			qmap[k].x = sp.x + (int) (dx * (float)(ep.x - sp.x));
			qmap[k].y = sp.y + (int) (dx * (float)(ep.y - sp.y));
		}
	}
	for (j = k = 0; j < D; j++)
	for (i = 0; i < W; i++, k++)
	{	sp.y = qmap[(j>0)?(k-W):k].y;
		sp.x = qmap[(i>0)?(k-1):k].x;
		ep.y = qmap[(j<D-1)?(k+W):k].y;
		ep.x = qmap[(i<W-1)?(k+1):k].x;
		ix = (ep.x >= sp.x)?1:-1;
		iy = (ep.y >= sp.y)?1:-1;
		for (h = sp.y; h != ep.y+iy; h += iy)
		for (g = sp.x; g != ep.x+ix; g += ix)
			buf[h*SZ+g] = saver[k];
	}
	for (j = k = 0; j < D; j++)
	for (i = 0; i < W; i++, k++)
	{	sp.y = qmap[(j>0)?(k-W):k].y;
		sp.x = qmap[(i>0)?(k-1):k].x;
		ep.y = qmap[(j<D-1)?(k+W):k].y;
		ep.x = qmap[(i<W-1)?(k+1):k].x;
		ix = (ep.x >= sp.x)?1:-1;
		iy = (ep.y >= sp.y)?1:-1;
		for (h = sp.y; h < ep.y*iy; h += 3*iy)
		for (g = sp.x; g < ep.x*ix; g += 3*ix)
			buf[h*SZ+g] = blend(g, h);
	}
/*
	liner(a.x, a.y, b.x, b.y);
	liner(b.x, b.y, c.x, c.y);
	liner(c.x, c.y, d.x, d.y);
	liner(d.x, d.y, a.x, a.y);
*/
}

liner(x1, y1, x2, y2)
	short x1, y1, x2, y2;
{
	short i;

	if (abs(x1-x2) > 1 || abs(y1-y2) > 1)
	{	liner(x1, y1, (x1+x2)/2, (y1+y2)/2);
		liner((x1+x2)/2, (y1+y2)/2, x2, y2);
	} else
	{	for (i = x2-2; i < x2+2; i++)
		{	if (i >= 0)
				buf[y2*SZ+x2] = blend(x2, y2);
	}	}
}
