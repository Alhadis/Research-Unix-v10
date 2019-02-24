#include "jf.h"

char *size_menu[]={
	"char width",
	"char left",
	"max width",
	"all widths",
	"nchars",
	"height",
	"ascent",
	NULL
};
Menu sizemenu={ size_menu };

sizefunc()
{
	int scode, ikbd; char str[10], *itoa(); Point p;
	Fontdisp *fdp=FDNULL; Editdisp *edp; Font *fp, *newfp, *fontrange();
	Fontchar *ich;

	if ((scode=menuhit3(&sizemenu)) < 0) return 0;
	else if (scode == 0) return chwidth();

	cursswitch(&target);
	if (buttons(DOWN) == 3) { mousetrack(); fdp=mtk.fdp; edp=mtk.edp; }
	else { buttons(UP); cursswitch((Texture *)0); return 0; }
	buttons(UP);
	if (scode == 1) {
		if (edp == EDNULL) return 0;
		fdp = edp->fdp; fp=fdp->fp;
		ich=fp->info + edp->c;
		ikbd = SIGNED(ich->left);
	} else {
		if (fdp == FDNULL) return 0;
		fp=fdp->fp;
		switch (scode) {
			case 2: ikbd = fdp->mwidth; break;
			case 3: ikbd = fp->info[fp->n].width; break;
			case 4: ikbd = fp->n; break;
			case 5: ikbd = fp->height; break;
			case 6: ikbd = fp->ascent; break;
		}
	}
	cursswitch(&deadmouse);
	drstore(rkbd); p=drstring(" (",drstring(size_menu[scode],pkbd));
	p=drstring("): ",drstring(itoa(ikbd),p));
	if (kbdstring(str,10,p) <= 0) { drclr(rkbd); return 0; }
	ikbd = atoi(str);

	switch (scode) {
		case 1:
			ich->left=max(-127,min(127,ikbd)); break;
		case 2:
			fp->info[fp->n].width=max(0,min(255,ikbd));
			fdp->mwidth = maxwidth(fp); break;
		case 3:
			for (ich=fp->info; ich < fp->info+fp->n; ich++)
				ich->width = ikbd;
			break;
		case 4:
			for (edp=fdp->edp; edp != EDNULL; edp=edp->edp)
				edp->fdp=FDNULL;
			fdp->edp=EDNULL;
			if ((newfp=fontrange(fp,ikbd)) == FNULL) return 0;
			fdp->fp=newfp; break;
		case 5:
			if (!fontheight(fp,ikbd)) return 0;
			break;
		case 6:
			fp->ascent=max(0,min(fp->height,ikbd)); break;
	}
	spaceman();
	return 1;
}

chwidth()
{
	Fontdisp *fdp; Font *fp; int b, c, w; Fontchar *ich;

	cursswitch(&widthmark);
	for (;;) {
		b=buttons(DOWN); buttons(UP);
		if (b == 3) break;
		mousetrack();
		if (mtk.edp == EDNULL) continue;
		c=mtk.edp->c; fdp=mtk.edp->fdp; fp=fdp->fp;
		ich=fp->info+c;
		if (b == 1) {
			w = 0;
		} else {
			w = mtk.pxl.x + 1;
			if (ich->left & 0x80)
				w += ich->left - 0x100;
		}
		if (w < 0 || ich->width == w) continue;
		ich->width=w;
		editdisp(mtk.edp);
	}
	return 1;
}
