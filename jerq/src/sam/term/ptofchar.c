#include "frame.h"

Point
ptofcharptb(f, p, pt, bn)
	register Frame *f;
	Posn p;
	Point pt;
	register bn;
{
	register uchar *s;
	register Box *b;
	register l;
	for(b=&f->box[bn]; bn<f->nbox; bn++,b++){
		cklinewrap(f, &pt, b);
		if(p<(l=LEN(b))){
			if(b->len>0)
				for(s=b->ptr; p>0; s++, p--){
					pt.x+=f->font->info[*s].width;
					if(*s==0 || pt.x>f->r.corner.x)
						panic("ptofchar");
				}
			break;
		}
		p-=l;
		advance(f, &pt, b);
	}
	return pt;
}
Point
ptofchar(f, p)
	Frame *f;
	Posn p;
{
	return ptofcharptb(f, p, Pt(f->left, f->r.origin.y), 0);
}
Point
ptofcharnb(f, p, nb)	/* doesn't do final advance to next line */
	Frame *f;
	Posn p;
{
	Point pt;
	int nbox;
	nbox=f->nbox;
	f->nbox=nb;
	pt=ptofcharptb(f, p, Pt(f->left, f->r.origin.y), 0);
	f->nbox=nbox;
	return pt;
}
Point
grid(f, p)
	register Frame *f;
	Point p;
{
	p.y-=f->r.origin.y;
	p.y-=p.y%f->font->height;
	p.y+=f->r.origin.y;
	if(p.x>f->r.corner.x)
		p.x=f->r.corner.x;
	return p;
}
Posn
charofpt(f, pt)
	register Frame *f;
	Point pt;
{
	Point qt;
	register bn;
	register uchar *s;
	register Box *b;
	Posn p;
	pt=grid(f, pt);
	qt.x=f->left, qt.y=f->r.origin.y;
	for(b=f->box,bn=0,p=0; bn<f->nbox && qt.y<pt.y; bn++,b++){
		cklinewrap(f, &qt, b);
		if(qt.y>=pt.y)
			break;
		advance(f, &qt, b);
		p+=LEN(b);
	}
	for(; bn<f->nbox && qt.x<=pt.x; bn++,b++){
		cklinewrap(f, &qt, b);
		if(qt.y>pt.y)
			break;
		if(qt.x+b->wid>pt.x){
			if(b->len<0)
				advance(f, &qt, b);
			else for(s=b->ptr; (qt.x+=f->font->info[*s++].width)<=pt.x; p++)
				if(*s==0)
					panic("end of string in charofpt");
		}else{
			p+=abs(b->len);
			advance(f, &qt, b);
		}
	}
	return p;
}
