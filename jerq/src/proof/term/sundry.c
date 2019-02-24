#include <jerq.h>
#include "layer.h"
#include "font.h"
#include "../comm.h"
#include "term.h"

Rectangle
rscale(r)
	Rectangle r;
{
	r.origin.x = muldiv(r.origin.x, resolution, 100);
	r.origin.y = muldiv(r.origin.y, resolution, 100);
	r.corner.x = muldiv(r.corner.x, resolution, 100);
	r.corner.y = muldiv(r.corner.y, resolution, 100);
	return(r);
}

Rectangle
riscale(r)
	Rectangle r;
{
	r.origin.x = muldiv(r.origin.x, 100, resolution);
	r.origin.y = muldiv(r.origin.y, 100, resolution);
	r.corner.x = muldiv(r.corner.x, 100, resolution);
	r.corner.y = muldiv(r.corner.y, 100, resolution);
	return(r);
}

sendn(c)
{
	if(!dying)
	{
		send(c >> 8);
		send(c);
	}
}

send(c)
{
	char cc = c;

	if(!dying) sendnchars(1, &cc);
}

sends(s)
	char *s;
{
	if(!dying) sendnchars(strlen(s)+1, s);
}

edit(s)
	char *s;
{
	char buf[40];
	Point p;
	int c;
	register i;

	strcpy(buf, s);
	p = mouse.xy;
	disp(buf, p);
	for(i = strlen(buf); (c = inkbd()) != '\r';)
	{
		disp(buf, p);
		p = mouse.xy;
		switch(c)
		{
		case '\b':
			if((buf[i] != ' ') && (i > 0))
				buf[--i] = 0;
			break;
		case '@':
			while(buf[i] != ' ') i--;
			buf[++i] = 0;
			break;
		case -1:
			buf[i++] = 'M';
			buf[i] = 0;
			goto done;
		default:
			buf[i++] = c;
			buf[i] = 0;
			break;
		}
		disp(buf, p);
	}
	disp(buf, p);
done:
	strcpy(s, buf);
}

disp(s, p)
	char *s;
	Point p;
{
	string(&defont, "\001", &display,
		string(&defont, s, &display, p, F_XOR), F_XOR);
}

inkbd()
{
	register k;

	if((k = kbdchar()) == -1)
	{
		while(wait(KBD|MOUSE))
		{
			if(own()&KBD) break;
			if(button123()) break;
		}
		k = kbdchar();
	}
	return(k);
}

dmap(x)
{
	register long z = x;

	z *= 100;
	z += resolution/2;
	z /= resolution;
	return((int)z);
}

Point
pdmap(p)
	Point p;
{
	p = sub(p, arena.layer.origin);
	return(add(Drect.origin, Pt(dmap(p.x), dmap(p.y))));
}

Point
plmap(p)
	Point p;
{
	p.x = dmap(p.x);
	p.y = dmap(p.y);
	return(p);
}

synch(s)
	char *s;
{
	rectf(&display, Drect, F_XOR);
	string(&defont, s, &display, Drect.origin, F_XOR);
	wait(KBD); (void)kbdchar();
	string(&defont, s, &display, Drect.origin, F_XOR);
	rectf(&display, Drect, F_XOR);
}

Atoi(s, n)
	register char *s;
{
	int old, sign = 1;

	while(*s == ' ') s++;
	if((*s == '-') || (*s == '+'))
	{
		if(*s == '-') sign = -1;
		old = n;
		s++;
	}
	else
		old = 0;
	for(n = 0; (*s >= '0') && (*s <= '9');)
		n = 10*n + *s++ - '0';
	if(*s) return(-1);	/* bad char */
	return(old + n*sign);
}
