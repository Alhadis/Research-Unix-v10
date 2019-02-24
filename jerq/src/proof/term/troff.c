#include <jerq.h>
#include "layer.h"
#include "font.h"
#include "../comm.h"
#include "term.h"

Point cursor, (*pmap)();	/* current point */
char *nc;
Font *curfont;
static fontno, fontchanged, psize;
Texture cup =
{
	0x0100, 0x00E0, 0x0010, 0x03E0, 0x0400, 0x0FE0, 0x123C, 0x1FE2,
	0x101A, 0x101A, 0x1002, 0x103C, 0x1810, 0x6FEC, 0x4004, 0x3FF8
};

troff(bp, s, e)
	int *s;
	Bitmap *bp;
{
	int c;
	char *end;

	for(nc = &reader.data[*s], end = &reader.data[e]; nc < end;)
	{
		if(cursor.y > arena.layer.corner.y)
			goto done;
		if(gap && (own()&MOUSE) && button2())
			Speed(2);
		c = *nc++&0xFF;
		if(c < 128)
		{
			if(drawchar(bp, c) < 0)
			{
				nc--;
				goto done;
			}
		}
		else if(c < C_PAGE)
			cursor.x += c - C_HOR;
		else if(c >= C_SIZE)
		{
			psize = c-C_SIZE+SIZE_ORIG;
			fontchanged++;
		}
		else if((c >= C_FONT) && (c <= C_FONT+10))
		{
			fontno = c-C_FONT;
			fontchanged++;
		}
		else switch(c)
		{
		case C_BSIZE:
			psize = *nc++;
			fontchanged++;
			break;
		case C_ABSH:
			cursor.x = inshort();
			break;
		case C_ABSV:
			cursor.y = min(inshort(), arena.page.corner.y);
			break;
		case C_DEV:
			switch(*nc++)
			{
			case RES:
				if((c = inshort()) != resolution)
				{
					resolution = c;
					arena.page = rscale(rpage);
					arena.layer = rscale(rsubp(Drect, Drect.origin));
				}
				break;
			case FONT:
				ldfont(*nc++);
				fontchanged++;
				break;
			case MODE:
				Mode = *nc++;
				break;
			}
			break;
		case C_DRAW:
			draw(bp);
			break;
		}
	}
done:
	*s = nc - reader.data;
}

/************************************************************
	I/O
*/

inshort()
{
	short i;

	i = *nc++ << 8;
	i |= *nc++&0xff;
	return(i);
}

Point inpoint()
{
	Point p;

	p.x = inshort();
	p.y = inshort();
	return(p);
}

/************************************************************
	Font management
*/
#define	NFMAP	2*NFONTS

Font *curfont;
Font *fnts[NFONTS], **nfnt = fnts;
typedef struct Fmap
{
	char name[10];	/* SS.xx/xxx */
	Font *f;
} Fmap;
Fmap fmap[NFMAP], *nfmap = fmap;
static char fnames[11][10];

static
drawchar(bp, c)
	Bitmap *bp;
{
	register Fontchar *fp;
	Point p;
	Rectangle r;

	if(fontchanged)
	{
		fontchanged = 0;
		c = loadfont(fontno, psize, c);
		if(c < 0) return(-1);
	}
	if(c == 0)
	{
		curfont = &defont;
		c = '?';
	}
	fp = curfont->info + c;
	r.origin.x=fp->x;
	r.corner.x=(fp+1)->x;
	r.origin.y=fp->top;
	r.corner.y=fp->bottom;
	p = (*pmap)(cursor);
	p.y += fp->top - curfont->ascent;
	if(gap) delay();
	bitblt(curfont->bits, r, bp, p, Mode);
	return(0);
}

ldfont(n)
{
	register char *s;

	for(s = fnames[n];;)
		if((*s++ = *nc++) == 0)
			return;
}

loadfont(n, s, c)
{
	char name[64], file[64];

	sprintf(name, "%s.%d", fnames[n], s);
	if(strcmp(fnames[n], "S") == 0)
	{
		sprintf(file, "%s/%d", name, c);
		if(findit(file))
		{
			fontchanged++;	/* because we mapped */
			return(1);
		}
		sprintf(file, "%s/ALL", name);
		if(findit(file)) return(c);
	}
	else
		if(findit(name)) return(c);
	send(HWANT);
	sends(fnames[n]);
	send(s);
	send(c);
	pstate = S_FONT;
	fontchanged++;
	return(-1);
}

fontmsg(name)
	char *name;
{
	char buf[128];

	strcpy(buf, "loading ");
	strcat(buf, name);
	string(&defont, buf, &display, add(Pt(10,3), Drect.origin), F_XOR);
}

static
findit(name)
	char *name;
{
	register Fmap *f;

	for(f = fmap; f != nfmap; f++)
		if(strcmp(f->name, name) == 0)
		{
			curfont = f->f;
			return(1);
		}
	return(0);
}

addfont(rname, mname)
	char *rname, *mname;
{
	Font **where;
	Fmap *f;
	char buf[64];
	extern inchar();

	/* select font slot */
	if(nfnt == &fnts[NFONTS-1])
	{
		where = nfnt;
		for(f = fmap; f != nfmap; f++)
			if(f->f == *where) f->name[0] = 0;
	}
	else
		where = nfnt++;
	if(mname[0])
	{
		Texture *t;

		strcpy(buf, mname);
		if(strcmp(mname, rname))
		{
			strcat(buf, ", wanted ");
			strcat(buf, rname);
		}
		fontmsg(buf);
		t = cursswitch(&cup);
		*where = infont(inchar);
		(void)cursswitch(t);
		fontmsg(buf);
		if(strcmp(mname, rname)) addname(mname, *where);
	}
	else
		*where = &defont;
	addname(rname, *where);
}

addname(name, font)
	char *name;
	Font *font;
{
	strcpy(nfmap->name, name);
	nfmap->f = font;
	if(nfmap != &fmap[NFMAP-1])
		nfmap++;
}

/**************************************************
	draw stuff
*/
draw(bp)
	Bitmap *bp;
{
	Point p, q, c;
	int a, b;

	a = *nc++;
	switch(a)
	{
	case LINE:
		p = (*pmap)(cursor);
		q = (*pmap)(add(cursor, inpoint()));
		delay();
		segment(bp, p, q, Mode);
		point(bp, q, Mode);
		break;
	case CIRCLE:
		p = (*pmap)(cursor);
		a = dmap(inshort()/2);
		p.x += a;	/* point was left edge */
		delay();
		circle(bp, p, a, Mode);
		break;
	case ELLIPSE:
		a = dmap(inshort()/2);
		b = dmap(inshort()/2);
		p = add((*pmap)(cursor), Pt(a, 0));
		delay();
		ellipse(bp, p, a, b, Mode);
		break;
	case ARC:
		p = (*pmap)(cursor);
		q = add(cursor, inpoint());
		c = (*pmap)(q);
		q = (*pmap)(add(q, inpoint()));
		delay();
		arc(bp, c, p, q, Mode);
		break;
	case SPLINE:
		spline(*nc++, bp);
		break;
	}
}

static Point pts[200];

static
spline(n, bp)
	Bitmap *bp;
{
	register Point *pp;
	register long w, t1, t2, t3, scale=1000; 
	register int i, j, steps=10, f; 
	Point p, q;

	p = cursor;
	pts[0] = pts[1] = (*pmap)(cursor);
	for(i = 0, pp = &pts[2]; i < n; i++)
		*pp++ = (*pmap)(p = add(p, inpoint()));
	*pp = pp[-1];
	n += 2;
	f = Mode;
	pp = pts;
	p = pp[0];
	for(i = 0; i < n-1; i++)
	{
		for(j = 0; j < steps; j++)
		{
			w = scale * j / steps;
			t1 = w * w / (2 * scale);
			w = w - scale/2;
			t2 = 3*scale/4 - w * w / scale;
			w = w - scale/2;
			t3 = w * w / (2*scale);
			q.x = (t1*pp[i+2].x + t2*pp[i+1].x + 
				t3*pp[i].x + scale/2) / scale;
			q.y = (t1*pp[i+2].y + t2*pp[i+1].y + 
				t3*pp[i].y + scale/2) / scale;
			segment(bp, p, q, f);
			p = q;
		}
		delay();
	}
}
