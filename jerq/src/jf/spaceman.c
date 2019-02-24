#include "jf.h"

spaceman()		/* allocate real estate, keep logical structure */
{
	Fontdisp *fdp;
	Rectangle r; Point p, es; int i, j, ncol, nrow, c;

	rkbd=Drect; rkbd.origin.y=Drect.corner.y-defont.height-2*WBORD;
	pkbd=add(rkbd.origin,Pt(WBORD,WBORD));

	r = Drect; r.corner.y = rkbd.origin.y; drclr(Drect);
	es.x=0; es.y=0;

	for (i=0; i<MFDISP; i++) if (fdisp[i].fp) {
		fontdisp(&fdisp[i],r);
		r.origin.y = fdisp[i].r.corner.y+EDSIZE*WBORD;
		es.x = max(es.x,fdisp[i].mwidth);
		es.y = max(es.y,(int)fdisp[i].fp->height);
	}

	es=mul(add(es,Pt(2*WMARG+WBORD,2*WMARG+WBORD)),EDSIZE);
	ncol=(r.corner.x-r.origin.x-WBORD*EDSIZE)/es.x;
	nrow=(r.corner.y-r.origin.y-WBORD*EDSIZE)/es.y;

	nedisp=0;
	p.y  = r.origin.y;
	for (j=0; j<nrow && nedisp<MEDISP; j++) {
		p.x  = r.origin.x+EDSIZE*WBORD;
		for (i=0; i<ncol && nedisp<MEDISP; i++) {
			edisp[nedisp  ].size    =EDSIZE;
			edisp[nedisp++].r.origin=p;
			p.x += es.x;
		}
		p.y += es.y;
	}

	for (i=0; i<MEDISP; i++) if ((fdp=edisp[i].fdp)) {
		c=edisp[i].c; fdispflp(fdp,c);
		if (i<nedisp) {
			editdisp(&edisp[i]);
		} else {
			edisp[i].r.corner=edisp[i].r.origin;
			charopcl(fdp,c,0);
		}
	}

	return;
}

mousetrack()		/* see what the mouse is up to */
{
	int i; Point edisphit();

	mtk.fdp=FDNULL;
	mtk.edp=EDNULL;

	for (i=0; i<MFDISP; i++) {
		if (fdisp[i].fp == FNULL) continue;
		mtk.c=fdisphit(&fdisp[i],mouse.xy);
		if (mtk.c >= 0) {
			mtk.fdp=&fdisp[i];
			return;
		}
	}

	for (i=0; i<nedisp; i++) {
		if (edisp[i].fdp == FDNULL) continue;
		mtk.pxl=edisphit(&edisp[i],mouse.xy);
		if (mtk.pxl.x >= 0) {
			mtk.edp=&edisp[i];
			return;
		}
	}

	return;
}

Fontdisp *
newfdp()	/* allocate new font display slot */
{
	register int i;
	for (i=0;i<MFDISP;i++) if (fdisp[i].fp == FNULL) return &fdisp[i];
	return FDNULL;
}

Editdisp *
newedp()	/* allocate new edit display slot */
{
	register int i;
	for (i=0;i<nedisp;i++) if (edisp[i].fdp == FDNULL) return &edisp[i];
	return EDNULL;
}
