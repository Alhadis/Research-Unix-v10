#include	<jerq.h>
#include	<font.h>
#undef	xmax
#undef	ymax

#define		MAX		32767
#define		ONE		512

#define	max(a,b)	((a) > (b)? (a):(b))
#define	abs(a)	((a) >= 0? (a):-(a))

#ifdef	lint
	struct Mouse mouse;
#endif	lint

typedef struct Frame
{
	char time[20];
	Bitmap *b;
} Frame;
Frame *frames;
int maxframe;
int start, nx, ny;
int doshade = 0;
int showgrid = 0;
int ragnorok, nsleep;
#define	SPEED	128
#define	MSPEED	8192
int speed = SPEED;
int period;
short floor;
short zdata[4096];
Rectangle bound;
long dtime, rtime;
extern char *genfn();
Menu menu = { (char **)0, genfn };
int N;
Point	ctr, dpt;
int	rad;
int nf;
short xmin, xmax, ymax, ymin;

Point
grid(x, y)
{
	Point p;

	p.x = N*(x-y);
	p.y = -(x+y-ny)*N/2;
	return(add(p, dpt));
}

#define	BB(n)	bitblt(frames[n].b, frames[n].b->rect, &display, ctr, F_STORE)
#define	STOP	((own()&MOUSE) && button123())

main()
{
	int n, done, x, y;

	request(RCV|SEND|MOUSE|KBD);
	initgrey();

	dpt = div(add(Drect.origin, Drect.corner), 2);

	for(wait(RCV); (n = rcvchar()) != 'Q'; wait(RCV))
		switch(n)
		{
		case 'G':
			showgrid = 1;
			break;
		case 'S':
			doshade = 1;
			break;
		case 'X':
			nx = getn();
			break;
		case 'Y':
			ny = getn();
			break;
		case 'P':
			period = getn();
			break;
		case 'n':
			nf = getn();
			frames = (Frame *)alloc(nf*sizeof(struct Frame));
			break;
		case 'F':
			gets(frames[maxframe].time);
			showf(maxframe, frames[maxframe].time);
			read(nx*ny);
			if(maxframe == 0)
				sizeit();
			if(frames[maxframe].b = balloc(bound))
			{
				draw(frames[maxframe].b);
				BB(maxframe);
				maxframe++;
			}
			break;
		case 'I':
			floor = getn();
			break;
		}

	rectf(&display, Drect, F_CLR);
	showt();
	start = 0;
	done = 0;
	goto startoff;
	while(done == 0)
	{
		wait(MOUSE);
		if(!button3()) continue;
		switch(n = menuhit(&menu, 3))
		{
		case -1:
		case 0:
			break;
		case 1:
			if((speed >>= 1) < 1) speed = 1;
			break;
		case 2:
			if((speed <<= 1) > MSPEED) speed = MSPEED;
			break;
		case 3:
			play(start, start? start-1:maxframe-1);
			break;
		case 4:
	startoff:
			while(!STOP)
				play(start, start? start-1:maxframe-1);
			break;
		case 5:
			done = 1;
			break;
		default:
			n -= 6;
			BB(n);
			start = n;
			break;
		}
	} 
	sendnchars(2, "x\n");
	sleep(500);
}

char buf[128], *str;

/*VARARGS2*/
spr(s, n, m)
	char *s;
	unsigned n, m;
{
	str = buf;
	while(*s)
	{
		if((*str++ = *s++) == '%')
		{
			str--;
			if(n != 0xFFFF) digit(n), n = 0xFFFF;
			else	digit(m);
		}
	}
	*str++ = 0;
}

digit(n)
	unsigned n;
{
	if(n >= 10) digit(n/10);
	*str++ = (n%10) + '0';
}

showf(n, t)
	char *t;
{
	spr("reading frame %: time=", n);
	while(*str++ = *t++);
	str--;
	string(&defont, buf, &display, add(Drect.origin, Pt(5, 5)), F_STORE);
}

showt()
{
	spr("total draw time=%, read time=% Hz", (int)(dtime/maxframe), (int)(rtime/maxframe));
	string(&defont, buf, &display, Drect.origin, F_STORE);
}

char *
genfn(i)
{
	switch(i)
	{
	case 0:	spr("% frames t=%", maxframe, ragnorok); break;
	case 1:	spr("slower"); break;
	case 2:	spr("faster"); break;
	case 3:	spr("play %-%", start, start? start-1:maxframe-1); break;
	case 4:	spr("loop %-%", start, start? start-1:maxframe-1); break;
	case 5:	spr("exit"); break;
	default:
		i -= 6;
		if(i >= maxframe) return((char *)0);
		else
		{
			spr("frame % t=", i);
			strcpy(str, frames[i].time);
		}
		break;
	}
	return(buf);
}
	
getn()
{
	register i, j;

	if((i = rcvchar()) == -1)
	{
		wait(RCV);
		i = rcvchar();
	}
	i <<= 8;
	if((j = rcvchar()) == -1)
	{
		wait(RCV);
		j = rcvchar();
	}
	j &= 0xff;
	return((i|j)&0xffff);
}

gets(s)
	char *s;
{
	register k;

	for(k = -1; k; s++)
	{
		if((k = rcvchar()) == -1)
		{
			wait(RCV);
			k = rcvchar();
		}
		*s = k;
	}
}

read(n)
{
	register short *s = zdata;
	long tim;

	tim = realtime();
	while(n--)
		*s++ = getn();
	rtime += realtime()-tim;
}

play(a, b)
{
	long target;
	int step;
#define	INC(x)	if(++x == maxframe) x = 0

	step = period*60;
	if(nf > 1) step /= nf-1;
	step = muldiv(step, SPEED, speed);
	target = realtime();
	a--;
	do
	{
		INC(a);
		BB(a);
		for(target += step; target > realtime(); sleep(1));
	} while((a != b) && !STOP);
	rectf(&display, raddp(frames[0].b->rect, sub(ctr, frames[0].b->rect.origin)), F_CLR);
	sleep(muldiv(20, SPEED, speed));
}

Point
conv(x, y, z)
{
	Point p;
	p = grid(x, y);
	p.y -= muldiv(z, rad, MAX);
	return(p);
}

Texture backgnd =
{
	0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA,
	0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA,
};

#define	Z(x,y)	zdata[(x)+nx*(y)]
#define	POLY(x1,y1,x2,y2,x3,y3,x4,y4)	poly(b,conv(x1,y1,Z(x1,y1)),conv(x2,y2,Z(x2,y2)),conv(x3,y3,Z(x3,y3)),conv(x4,y4,Z(x4,y4)),light(x1,y1-1),F_STORE)
#define	SEG(x1,y1,x2,y2)	{int z1,z2; z1=Z(x1,y1); z2=Z(x2,y2); if((z1>=floor)&&(z2>=floor))segment(b,conv(x1,y1,z1),conv(x2,y2,z2),F_OR);}

draw(b)
	Bitmap *b;
{
	register j, x, y;
	int lasty;
	long tim;

	if(doshade)
		texture(b, b->rect, &backgnd, F_STORE);
	else
		rectf(b, b->rect, F_CLR);
	tim = realtime();
	if(showgrid && (doshade == 0))
	{
		for(y = 0; y < ny; y++)
		{
			for(x = 0; x < nx; x++)
			{
				if(y) SEG(x, y-1, x, y);
				if(x) SEG(x-1, y, x, y);
			}
			wait(CPU);
		}
	}
	else
		for(j = nx+ny-3; j > 0; j--)
		{
			y = min(ny-1, j);
			lasty = x = j-y;
			while(y != lasty)
			{
				POLY(x, y, x, y-1, x+1, y-1, x+1, y);
				x++;
				y--;
			}
			wait(CPU);
		}
	dtime += realtime() - tim;
}

#define	NGREY	16

Texture grey[NGREY];

poly(bm, p1, p2, p3, p4, illum, mode)
	Bitmap *bm;
	Point p1, p2, p3, p4;
{
	illum /= ONE/NGREY;
	if(doshade) ppfill(&p1, 4, bm, &grey[illum], mode);
	if(showgrid)
	{
		if(illum == 0) mode = F_CLR;
		segment(bm, p1, p2, mode);
		segment(bm, p2, p3, mode);
		segment(bm, p3, p4, mode);
		segment(bm, p4, p1, mode);
	}
}

int lx, ly, lz;

light(X, Y)
{
	int x, y, z;
	int res;
	long a, len, m;

	if(doshade == 0) return(ONE-1);
	x = (Z(X+1, Y)-Z(X, Y) + Z(X+1, Y-1)-Z(X, Y-1))/2;
	y = (Z(X, Y)-Z(X, Y-1) + Z(X+1, Y)-Z(X+1, Y-1))/2;
	z = (abs(X)+abs(Y));
	len = sqrt(x*(long)x + y*(long)y + z*(long)z);
	if(len == 0) len++;
	x = muldiv(x, ONE, len);
	y = muldiv(y, ONE, len);
	z = muldiv(z, ONE, len);
	res = muldiv(x, lx, ONE) + muldiv(y, ly, ONE) + muldiv(z, lz, ONE);
	if(res < 0) res = 0;
	return(res);
}

initgrey()
{
	register mask, k, level, j, w, i;
	Rectangle r;

	lx = -50; ly = 360; lz = 360;

	r = Drect;
	r.corner.x = ((NGREY-1)*(long)Drect.origin.x+Drect.corner.x)/NGREY;
	for(i = 0; i < NGREY-1; i++)
	{
		mask = 1;
		w = 0;
		level = ONE*i/(NGREY-1);
		for(j = 0; j < 16; j++)
		{
			for(k = 0; k < 16; k++)
			{
				if(nrand(ONE) > level) w |= mask;
				mask <<= 1;
			}
			grey[i].bits[j] = w;
			w = 0;
			mask = 1;
		}
		/*texture(&display, r, &grey[i], F_STORE);
		r = raddp(r, Pt(r.corner.x-r.origin.x, 0));*/
	}
}

nrand(n)
{
	return(muldiv(rand(), n, 32767));
}

#define		FUDGE		20

sizeit()
{
	register i, j;
	Point p;
	Bitmap *b;

	for(N = 10; N > 3; N--)
	{
		rad = 4*N;
		xmin = XMAX; xmax = 0; ymax = 0; ymin = YMAX;
		for(i = 0; i < nx; i++)
			for(j = 0; j < ny; j++)
			{
				p = conv(i, j, Z(i, j));
				if(p.x < xmin) xmin = p.x;
				if(p.x > xmax) xmax = p.x;
				if(p.y > ymax) ymax = p.y;
				if(p.y < ymin) ymin = p.y;
			}
		xmin -= FUDGE; xmax += FUDGE; ymin -= FUDGE; ymax += FUDGE;
		i = xmax-xmin;
		j = ymax-ymin;
		if((i > (Drect.corner.x-Drect.origin.x)) || (j > (Drect.corner.y-Drect.origin.y)))
			continue;
		if(b = balloc(Rect(0, 0, i, nf*j)))
		{
			bfree(b);
			break;
		}
	}
	bound = raddp(Rect(xmin, ymin, xmax, ymax), Pt(0, 0));
	ctr = sub(dpt, Pt((xmax-xmin)/2, (ymax-ymin)/2));
}
