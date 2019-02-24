#include <jerq.h>
#include <font.h>

extern struct Proc *proc;
extern Bitmap *bp, blit;
extern Rectangle rect;
extern Texture deadmouse;

struct Proc *
Getproc()
{
	register struct Proc *pd;
	cursswitch(&deadmouse);
	while ((pd = getproc()) == 0)
		sleep(6);
	return (pd != P ? pd : 0);
}

Rectangle
kbdrect()
{
	extern Point kbdp;
	Rectangle r;
	r=Drect; r.origin.y=r.corner.y-defont.height-4;
	kbdp=add(r.origin,Pt(2,3));
	return r;
}

checkshape(flag)
{
	Rectangle r, s;
	if (flag || (P->state & (MOVED|RESHAPED))) {
		P->state &= ~(MOVED|RESHAPED);
		r = kbdrect();
		s = Drect; s.corner.y = r.origin.y;
		rectf(&display, r, F_STORE);
	}
}

flash(p, r)
register struct Proc *p; Rectangle r;
{
	register Bitmap *b = 0;
	if (p) {
		b = (Bitmap *)(p->layer);
		rect = r;
		proc = p;
		bp = 0;
	} else if (r.corner.x > r.origin.x && r.corner.y > r.origin.y) {
		proc = 0;
		bp = b = &blit;
		rect = r;
	}
	if (b) {
		visible(0);
		rectf(b, r, F_XOR); nap(20); rectf(b, r, F_XOR);
		visible(1);
	}
}

visible(flag)
{
	static Rectangle prevrect;
	static int state = 1;
	if (state == flag)
		return;
	else if (flag)
		state = 1, reshape(prevrect, 0), checkshape(1);
	else if (bp && rectXrect(P->layer->rect, rect))
		state = 0, prevrect = P->layer->rect, reshape(Rect(0,0,0,0), 0);
}
