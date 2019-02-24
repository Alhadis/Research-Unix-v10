#include <jerq.h>

#undef	Rpt
#define	Rpt(o,c)	o,c

brol(bits,r)	/* rotate rectangle bitwise left */
Bitmap *bits; Rectangle r;
{
	bhflip(bits,Rpt(Pt(r.origin.x+1,r.origin.y),r.corner));
	bhflip(bits,r);
}

bror(bits,r)	/* rotate rectangle bitwise right */
Bitmap *bits; Rectangle r;
{
	bhflip(bits,Rpt(r.origin,Pt(r.corner.x-1,r.corner.y)));
	bhflip(bits,r);
}

brou(bits,r)	/* rotate rectangle bitwise up */
Bitmap *bits; Rectangle r;
{
	bvflip(bits,Rpt(Pt(r.origin.x,r.origin.y+1),r.corner));
	bvflip(bits,r);
}

brod(bits,r)	/* rotate rectangle bitwise down */
Bitmap *bits; Rectangle r;
{
	bvflip(bits,Rpt(r.origin,Pt(r.corner.x,r.corner.y-1)));
	bvflip(bits,r);
}

bvflip(bits,r)	/* flip rectangle vertically */
Bitmap *bits; Rectangle r;
{
	_bflip(bits,Rpt(r.origin,Pt(r.corner.x,r.origin.y+1)),
	           Rpt(Pt(r.origin.x,r.corner.y-1),r.corner),
	           Pt(0,1), (r.corner.y-r.origin.y)/2);
}

bhflip(bits,r)	/* flip rectangle horizontally */
Bitmap *bits; Rectangle r;
{
	_bflip(bits,Rpt(r.origin,Pt(r.origin.x+1,r.corner.y)),
	           Rpt(Pt(r.corner.x-1,r.origin.y),r.corner),
	           Pt(1,0), (r.corner.x-r.origin.x)/2);
}

static
_bflip(bits,r0,r1,dr,n)
register Bitmap *bits; Rectangle r0, r1; Point dr; register int n;
{
	while (--n >= 0) {
		bitblt(bits,r0,bits,r1.origin,F_XOR);
		bitblt(bits,r1,bits,r0.origin,F_XOR);
		bitblt(bits,r0,bits,r1.origin,F_XOR);
		r0 = raddp(r0,dr);
		r1 = rsubp(r1,dr);
	}
}

btrans(bits,r)	/* transpose rectangle bitwise */
Bitmap *bits; Rectangle r;
{
	int size;

	if ((size=min(r.corner.x-r.origin.x,r.corner.y-r.origin.y)) > 1) {
		_btrans(bits,Rpt(r.origin,add(r.origin,Pt(size,size))));
	}
}

static
_btrans(bits,r)	/* transpose rectangle bitwise */
Bitmap *bits; Rectangle r;
{
	int son, soff; Rectangle u, l;

	soff=r.corner.x-r.origin.x;
	son =(soff+1)/2; soff=soff/2;

	l.origin.x=r.origin.x;
	l.origin.y=r.origin.y+son;
	l.corner  =add(l.origin,Pt(soff,soff));

	u.origin.x=r.origin.x+son;
	u.origin.y=r.origin.y;
	u.corner  =add(u.origin,Pt(soff,soff));

	bitblt(bits,l,bits,u.origin,F_XOR);
	bitblt(bits,u,bits,l.origin,F_XOR);
	bitblt(bits,l,bits,u.origin,F_XOR);

	if (soff > 1) {
		_btrans(bits,l);
		_btrans(bits,u);
	}

	if (son > 1) {
		u.origin=r.origin;
		u.corner=add(r.origin,Pt(son,son));
		l.origin=sub(r.corner,Pt(son,son));
		l.corner=r.corner;
		_btrans(bits,l);
		_btrans(bits,u);
	}
}
