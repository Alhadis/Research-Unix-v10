#include "frame.h"

redraw(f, pt)
	Frame *f;
	Point pt;
{
	register Box *b;
	int nb;
	for(nb=0,b=f->box; nb<f->nbox; nb++, b++){
		cklinewrap(f, &pt, b);
		if(b->len>=0)
			string(f->font, b->ptr, B, pt, F_XOR);
		pt.x+=b->wid;
	}
}
Point
draw(f, pt)
	Frame *f;
	Point pt;
{
	register Box *b;
	register nb, n;
	for(b=f->box,nb=0; nb<f->nbox; nb++, b++){
		cklinewrap0(f, &pt, b);
		if(pt.y==f->r.corner.y){
			f->nchars-=frstrlen(f, nb);
			delbox(f, nb, f->nbox-1);
			break;
		}
		if(b->len>0){
			n=canfit(f, pt, b);
			if(n==0)
				panic("draw: canfit==0");
			if(n!=b->len){
				splitbox(f, nb, n);
				b=&f->box[nb];
			}
			pt.x+=b->wid;
		}else{
			if(b->bc=='\n')
				pt.x=f->left, pt.y+=f->font->height;
			else
				pt.x+=newwid(f, pt, b);
		}
	}
	return pt;
}
frstrlen(f, nb)
	register Frame *f;
	register nb;
{
	register n;
	for(n=0; nb<f->nbox; nb++)
		n+=LEN(&f->box[nb]);
	return n;
}
