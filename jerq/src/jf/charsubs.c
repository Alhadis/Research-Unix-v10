#include "jf.h"

#undef	Rpt
#define	Rpt(o,c)	o,c

char *bit_menu[]={	/* N.B. menu order follows Code definition */
	"F_STORE",
	"F_OR",
	"F_CLR",
	"F_XOR",
	NULL
};
Menu bitmenu={ bit_menu };

bitfunc()		/* perform bitblt operation between characters */
{
	int fcode, dstc; Point psrc, pdst, pprev;
	Rectangle rsrc, charect();
	Font *srcfp, *dstfp; Fontdisp *dstfdp; Editdisp *srcedp, *dstedp;

	if ((fcode=menuhit3(&bitmenu)) < 0) return 0;

	cursswitch(&target);
	if (buttons(DOWN) == 3) { mousetrack(); srcedp=mtk.edp; psrc=mouse.xy; }
	else { buttons(UP); return 0; }

	pprev=psrc;
	for (;wait(MOUSE);nap(2)) {
		if (!ptinrect(mouse.xy,Drect)) continue;
		pdst=mouse.xy;
		if (!eqpt(pprev,pdst)) {
			segment(&display,psrc,pprev,F_XOR);
			pprev=pdst;
			segment(&display,psrc,pprev,F_XOR);
		}
		if (!button3()) { mousetrack(); dstedp=mtk.edp; break; }
	}
	segment(&display,psrc,pdst,F_XOR);
	if (dstedp == EDNULL || srcedp == EDNULL) return 0;

	srcfp=srcedp->fdp->fp; rsrc=charect(srcfp,srcedp->c);

	dstc=dstedp->c; dstfdp=dstedp->fdp; dstfp=dstfdp->fp;
	pdst=charect(dstfp,dstc).origin;

	bitblt(srcfp->bits,rsrc,dstfp->bits,pdst,fcode);
	fdispstr(dstfdp,dstc); fdispflp(dstfdp,dstc);
	editdisp(dstedp);
	return 1;
}

char *shift_menu[]={	/* N.B. menu order follows Texture *arrows */
	"left / right",
	"up / down",
	"flip",
	"transpose",
	NULL
};
Menu shiftmenu={ shift_menu };

shiftfunc()		/* shift/rotate character bitwise */
{
	int rcode, bselect, c; Rectangle r, charect();
	Fontdisp *fdp; Font *fp;

	if ((rcode=menuhit3(&shiftmenu)) < 0) return 0;

	cursswitch(arrows[rcode]); rcode *= 2;

	for (;;) {
		bselect=buttons(DOWN)-1; buttons(UP);
		if (bselect > 1) break;
		mousetrack();
		if (mtk.edp == EDNULL) continue;

		fdp=mtk.edp->fdp; fp=fdp->fp; c=mtk.edp->c;
		r=charect(fp,c);

		switch (bselect+rcode) {
			case 0:
				brol(fp->bits,r); break;
			case 1:
				bror(fp->bits,r); break;
			case 2:
				brou(fp->bits,r); break;
			case 3:
				brod(fp->bits,r); break;
			case 4:
				bhflip(fp->bits,r); break;
			case 5:
				bvflip(fp->bits,r); break;
			case 6:
			case 7:
				btrans(fp->bits,r); break;
		}

		fdispstr(fdp,c); fdispflp(fdp,c);
		editdisp(mtk.edp);
	}
	return 1;
}

charopcl(fdp,c,bit)	/* open or close character for editing */
Fontdisp *fdp; int c, bit;
{
	Editdisp *edp, *prevedp, *newedp(); Rectangle r, charect(), rsupport();
	Font *fp; Fontchar *ich;

	for (prevedp=EDNULL,edp=fdp->edp; edp != EDNULL; prevedp=edp,edp=edp->edp)
		if (c == edp->c) break;
	if ((edp!=EDNULL) == bit) return 0;

	fp=fdp->fp; ich=fp->info+c;

	if (bit) {
		if ((edp=newedp()) == EDNULL) return 0;
		edp->edp=EDNULL; edp->fdp = fdp; edp->c = c;
		if (!editdisp(edp)) { edp->fdp=FDNULL; return 0; }
		if (prevedp == EDNULL) fdp->edp = edp; else prevedp->edp=edp;

	} else {
		r=rsupport(fp->bits,charect(fp,c),(Bitmap *)0);
		ich->top=r.origin.y; ich->bottom=r.corner.y;

		if (prevedp == EDNULL) fdp->edp = edp->edp;
		else prevedp->edp = edp->edp;

		edp->fdp=FDNULL;

		drclr(edp->r);
	}
	fdispflp(fdp,c);
	return 1;
}

Point
edisphit(edp,pc)	/* return selected pixel, or -1 if none */
Editdisp *edp; Point pc;
{
	Point p;
	if (ptinrect(pc,Rpt(edp->r.origin,sub(edp->r.corner,Pt(1,edp->size))))) {
		p=div(sub(pc,edp->r.origin),edp->size);
	} else {
		p.x = -1;
		p.y = -1;
	}
	return(p);
}

edispset(edp,pxl,bit)	/* set value of pixel */
Editdisp *edp; Point pxl; int bit;
{
	int c, *waddr, wbit, row, col;
	Point p; Font *fp; Fontdisp *fdp;

	c=edp->c; fdp=edp->fdp; fp=fdp->fp;

	p.x=fp->info[c].x + pxl.x; p.y=pxl.y;

	waddr=addr(fp->bits,p);
	wbit = 1<<(WORDSIZE - 1 - p.x%WORDSIZE);

	if (((*waddr & wbit) != 0) == bit) return 0;

	*waddr ^= wbit;

	col=c%fdp->ncpl;
	row=c/fdp->ncpl;
	p=add(fdp->r.origin,Pt(WMARG+col*fdp->cbx,WMARG+row*fdp->cby));
	p=add(p,pxl);
	drflip(Rpt(p,add(p,Pt(1,1))));

	p=add(edp->r.origin,mul(pxl,edp->size));
	drflip(Rpt(p,add(p,Pt(edp->size,edp->size))));

	return 1;
}

editdisp(edp)	/* display character in large format */
register Editdisp *edp;
{
	Fontdisp *fdp; int c,size;
	int i; Rectangle r, charect(); Font *fp; Fontchar *ich;

	fdp  = edp->fdp; fp=fdp->fp;
	size = edp->size;
	c    = edp->c;
	ich  = fp->info + c;

	if (!fontgrow(fp,c,fdp->mwidth)) return 0;
	if (ich->left && (ich->left & 0x80) == 0) {
		bitblt(fp->bits,
			Rect(ich->x,0,(ich+1)->x-ich->left,fp->height),
			fp->bits,Pt(ich->x+ich->left,0),F_STORE);
		rectf(fp->bits,Rect(ich->x,0,ich->x+ich->left,fp->height),
			F_CLR);
		ich->left=0;
	}

	edp->r.corner = add(edp->r.origin,mul(Pt(fdp->mwidth,1+fp->height),size));
	edp->r.corner.x += 1;

	drclr(edp->r);
	magnify(fp->bits,charect(fp,c),&display,edp->r.origin,Pt(size,size));

	r.origin=edp->r.origin;
	r.corner=add(r.origin,Pt(fdp->mwidth*size,1));
	for (i=0; i<=fp->height; i++) {
		drflip(r);
		r=raddp(r,Pt(0,size));
	}

	r.origin=edp->r.origin;
	r.corner=add(r.origin,Pt(1,fp->height*size));
	for (i=0; i<=fdp->mwidth; i++) {
		drflip(r);
		r=raddp(r,Pt(size,0));
	}

	r.origin=add(edp->r.origin,Pt(0,((2*fp->ascent+1)*size-WBORD)/2));
	if (ich->left & 0x80)
		r.origin.x += (0x100 - ich->left)*size;
	r.corner=add(r.origin,Pt(max(ich->width*size,size/2),WBORD));
	drflip(r);

	return 1;
}
