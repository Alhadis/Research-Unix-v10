/*	Plotting Routines	*/

#include "jped.h"

extern short discture();

extern short dspl_arc();

extern char *menuCL[];
extern char font_notav[];

short eff_size;

extern short effs();

/* plot all objects */

short plot_all()
{
	register i;
	if(D_ON) return(0);
	rectf(&display,board,F_CLR);
	if(grsize) plot_grid();
	for(i=0;i<no_obj;i++) if(obj[i]) plot_one(obj[i]);
	return(0);
}

short plot_grid() {
	register x, y;
	if(!grsize) return(-1);
	for(x=gri(board.origin.x,xshift); x<board.corner.x; x += grsize)
		for(y=gri(board.origin.y,yshift); y<board.corner.y; y += grsize)
			point(&display,Pt(x,y),dr_mode);
	return(-1);
}

short dr_link(o1,o2)
	register objP o1, o2;
{
	if(drawpar) dline(*(o1->p0),*(o2->p0),4);
}

short mapfont(k){
	if(k<9) return(0);
	if(k<12) return(1);
	if(k<16) return(2);
	if(k<18) return(3);
	return(4);
}


char * find_sep(s)
	char * s;
{
	register char * p;
	for(p=s; *p; p++) if(*p==SEPAR) break;
	if(*p) return(p);
	else return((char *)0);
}

/* plot one obj */

short plot_one(a) objP a;
{
	register  i, txl;
	register char *nlp;
	char adjc;
	Point p1, p2;
	register char  *txtp;
	Font *kf;

	if(!a) return(-1);
	if(!a->status) return(0);

	if(nosk && !drawpar) eff_size = effs(a->ps);

	if(drawpar && a->type != 'f')  bmark(*(a->p0),ball);
	if(a->pen && a->type != 'f') {	/* object with color */
		p1 = *(a->p0);
		if(drawpar) pstring(p1,menuCL[a->pen]);
	}
	/* possible bug */
	if(!fullob(a) && !(no_line(a)) ) 
	if(a->size=='a' || a->size=='b' || a->size=='c') {
		plot_arrow(a,a->size);
	}

	switch(a->type){
	case 'f':
		for(i=0;i<a->size;i++) {
			if(!a->mem[i]) continue;
			plot_one(a->mem[i]);
			dr_link(a->mem[0],a->mem[i]);
		}
		break;
	case 'p':
	case 'P':
		pl_poly(a,a->p0,a->p0 + a->n-1);
		break;
	case 's':
	case 'S':
		if(!nosk || drawpar) pl_poly(a,a->p0,a->p0 + a->n-1);
		if(nosk) {
			if(fullob(a)) ppfill(a);
			else spline(a);
		}
		break;
	case 'c':
		if(fullob(a) && nosk )
		discture(&display,*(a->p0),a->size,T_tab[a->color-'E'],dr_mode);
		else pcirc(*(a->p0),a->size);
		break;
	case 'Q':
		if(nosk) ppfill(a);
		else pl_poly(a,a->p0,a->p0 + a->n-1);
		break;
	case 't':
		kf = 0;
		if(def_font){
			i = mapfont(a->size);
			if(!fontrcv[i] && !font_notav[i]) {
				sendstring("FF");
				s_short((short)i);
				s_flush();
				if(g_char()=='F') loadfont();
				else font_notav[i] = 1;
			}
			kf = fontrcv[i];
		}
		if(!kf) kf = &defont;
		p1 = *(a->p0);
		for(i=0,nlp=a->txt;*nlp;nlp++) if(*nlp==SEPAR) i++;
		i++;	/*number of lines*/
		if(i%2) p1.y -= (kf->height/2);
		i /= 2;
		if(i) p1.y -= (i*kf->height);
		p2 = p1;
		txtp = a->txt + 2;	/*assume that all starts with \[CRL]*/
		adjc = a->txt[1];
		do{
			nlp = find_sep(txtp);
			if(nlp) {
				*nlp = 0;	/* replace nl by null */
			}
			txl = strwidth(kf,txtp);
			if(adjc == 'C') txl /= 2;
			if(adjc == 'C' || adjc == 'R') p2.x -= txl;
			string(kf,txtp,&display,p2,dr_mode);
			if(nlp) {
				*nlp = SEPAR;	/* restore */
				txtp = nlp + 1;	/* go past last spot */
				p2.x = p1.x;
				p2.y += kf->height;
			}
		} while(nlp);
		break;
	}

	eff_size = 0;
	return(0);
}

/* -- plot polygon -- */

short pl_poly(aa,pa,pb) objP aa; pointP pa, pb;
{
	register pointP pp, qq, opp, pp1;
	register j, k,  klisto;
	short mv_first, mv_last;
	/* klisto = 1 when polygon is closed */

	if(aa == 0) return(-1);
	if(nosk && !drawpar) eff_size = effs(aa->ps);

	klisto = klosed(aa->type);
	if(fullob(aa)) klisto = 1;
	pp1 = aa->p0 + aa->n - 1;

	if(pa < aa->p0) {
		pa = aa->p0;
		mv_first = 1;
	}
	else if(pa->x < SILLY) { pa++; mv_first = 1; }
		else mv_first = 0;
	if(pb > pp1) {
		pb = pp1;
		mv_last = 1;
	}
	else if(pb->x < SILLY) { pb--; mv_last = 1; }
		else mv_last = 0;

	if(mv_last) {
		pdline(aa->color,*pa,*pb);
		if(klisto){
			for(qq=pa-1;qq>=aa->p0;qq--){
				if(qq->x < SILLY) break;
			}
			qq++;
			pdline(aa->type,*pb,*qq);
		}
	}
	else if(mv_first) {
		pdline(aa->color,*pa,*pb);
		if(klisto) {
			for(qq=pb+1;qq<=pp1;qq++){
				if(qq->x < SILLY) break;
			}
			qq--;
			pdline(aa->type,*pa,*qq);
		}
	}
	else { 	/* mv_last = mv_first = 0 */
		k = pb - pa + 1;
		qq = pa;
		pp = qq + 1;
		opp = qq;
		if(pa != aa->p0 || pb != pp1) klisto = 0;
		for(j=1;j<k;j++,pp++,opp++) {
			if(pp->x < SILLY) {	/* break in contour */
				if(klisto) {
					pdline(aa->color,*qq,*opp);
					qq = pp + 1;
				}
				j++; pp++, opp++;
			}
			else pdline(aa->color,*opp,*pp);
		}
		if(klisto) {
			pdline(aa->color,*opp,*qq);
		}
	}
	eff_size = 0;
	return(0);
}

/* spline plotting routine */

/* draw lines around double points */
short ssline(p1,p2,k)
	Point p1, p2;
	short k;
{
	Point pm;
	pm.x = (p1.x+p2.x)/2;
	pm.y = (p1.y+p2.y)/2;
	pdline((char)k,pm,p2);
}

short spline(aa) objP aa;
{
	register pointP pa, pb;
	register pointP pp, opp;
	register char * pt;
	short kl;
	if(aa->type != 's' && aa->type != 'S') return(-1);
	pa = aa->p0;
	pb = aa->p0 + aa->n;
	if(pa>=pb) return(-1);
	kl = aa->type=='S'? 1: 0;
	for(opp=pp=pa,pt=aa->txt;pp<pb;pp++) {
		if(pp->x < SILLY) {
			dspline(opp,pp,pt,kl,aa->color);
			opp = pp + 1;
			pt += opp - pa;
		}
	}
	dspline(opp,pp,pt,kl,aa->color);
}

extern long sqrt();

short dspline(pa,pb,pt,kk,cc)	/*	[pa,pb)	*/
	pointP pa, pb;
	char * pt;
	char cc;
{
	register pointP pp;
	Point p1, p2, p3;
	long midp, n, xx, yy;
	long X1, X2, X3, Y1, Y2, Y3;
	long Xm, Ym, Xz, Yz;

	if(kk) p2 = *(pb-1);
	else p2 = *pa;
	p3 = *pa;
	pp = pa;
	if(*pt=='0' && !kk) { pp++; p3 = *pp; pt++; }
	for(; pp<pb; pp++, pt++){
		p1 = p2;
		p2 = p3;
		if(*pt>'0') ssline(p1,p2,cc);
		if(pp != pb-1) p3 = *(pp+1);
		else {
			if(kk) p3 = *pa;
			else break;	/* nothing more to plot */
		}
		if(*pt>'0') ssline(p3,p2,cc);
		else {
			midp = p3.x+p1.x;
			X1 = 50*midp - 100*(long)p2.x;
			X2 = 100*(long)(p2.x-p1.x);
			midp = p2.x+p1.x;
			X3 = 50*midp;
			midp = p3.y+p1.y;
			Y1 = 50*midp - 100*(long)p2.y;
			Y2 = 100*(long)(p2.y-p1.y);
			midp = p2.y+p1.y;
			Y3 = 50*midp;
			midp = p3.x+p1.x;
			Xm = 25*midp + 75*(long)p2.x;
			midp = p3.y+p1.y;
			Ym = 25*midp + 75*(long)p2.y;
			midp = p2.x + p3.x;
			Xz = 50*midp;
			midp = p2.y + p3.y;
			Yz = 50*midp;
			xx = (X3-Xm)/10;	yy = (Y3-Ym)/10;
			midp = sqrt(xx*xx + yy*yy);
			xx  = (Xz-Xm)/10;	yy = (Yz-Ym)/10;
			n = sqrt(xx*xx + yy*yy);
			n += midp;
			n = (n+30)/60;
			if(cc==DASH && n>3) dspl_arc(X1,Y1,X2,Y2,X3,Y3,n);
			else spl_arc(X1,Y1,X2,Y2,X3,Y3,n);
		}
	}
	return(0);
}

short spl_arc(X1,Y1,X2,Y2,X3,Y3,n)
	long X1,Y1,X2,Y2,X3,Y3, n;
{
	long n2;
	short LEN = 10;
	short  Xs, Ys, Xe, Ye;
	long Xt, Yt;
	register u;

	if(n<LEN) n = LEN;
	n2 = n*n;
	Xe = (X3+50)/100;		Ye = (Y3+50)/100;
	for(u=1;u<=n;u++){
		Xs = Xe;	Ys = Ye;
		Xt = ( (X1*u + X2*n)*u ) / n2 + X3;
		Yt = ( (Y1*u + Y2*n)*u ) / n2 + Y3;
		Xe = (Xt + 50)/100;
		Ye = (Yt + 50)/100;
		pline(Pt(Xs,Ys),Pt(Xe,Ye));
	}
}

/*	dashed splines	*/

short nxtu(X1,Y1,X2,Y2,n,oldu)
	long X1,Y1,X2,Y2, n;
	short oldu;
{
	long xx, yy, rr;
	short ds;
	xx = (2*X1*oldu)/n + X2;	xx /= 4;
	yy = (2*Y1*oldu)/n + Y2;	yy /= 4;
	rr = sqrt(xx*xx + yy*yy)/4;
	if(rr<2) rr = 2;
	ds = n/rr;	/*assumes dash length one*/
	if(ds<2) ds = 2;
	return(oldu+ds);
}

short dspl_arc(X1,Y1,X2,Y2,X3,Y3,n)
	long X1,Y1,X2,Y2,X3,Y3, n;
{
	long n2;
	short LEN = 10;
	short  Xs, Ys, Xe, Ye;
	long Xt, Yt;
	register u;
	short u0, u1;

	if(n<LEN) n = LEN;
	n2 = n*n;
	Xe = (X3+50)/100;		Ye = (Y3+50)/100;
	u0 = 0;
	while(u0<=n) {
		u1 = nxtu(X1,Y1,X2,Y2,n,u0);	/* to draw */
		if(u1>n) u1 = n;
		for(u=u0;u<=u1;u++){
			Xs = Xe;	Ys = Ye;
			Xt = ( (X1*u + X2*n)*u ) / n2 + X3;
			Yt = ( (Y1*u + Y2*n)*u ) / n2 + Y3;
			Xe = (Xt + 50)/100;
			Ye = (Yt + 50)/100;
			if(u>u0) pline(Pt(Xs,Ys),Pt(Xe,Ye));
		}
		u0 = nxtu(X1,Y1,X2,Y2,n,u1);	/* to skip */
	}
}

/* -- dash line plotter -- */

long lng(p,q)
Point p, q;
{
	long xx, yy, rr;
	xx = abs(p.x - q.x);
	yy = abs(p.y - q.y);
	if(!xx & !yy) return(0);
	rr = xx*xx + yy*yy + xx*yy;
	rr *= 100;
	rr /= (xx+yy);
	return(rr);
}

short dline(p1,p2,dd)
	Point p1, p2;
{
	Point pa, pb;
	long nst, llen, ldx, ldy;
	long xa, ya, xb, yb;
	short slen;
	register i;

	llen = lng(p1,p2);
	nst = llen/dd;
	nst = (nst+50)/100;
	if(!(nst%2)) nst++;
	if(nst<2) {
		pline(p1,p2);
		return;
	}
	ldx = p2.x-p1.x;
	ldy = p2.y-p1.y;
	ldx = (100*ldx)/nst;
	ldy = (100*ldy)/nst;
	xa = 100*p1.x ;
	ya = 100*p1.y ;
	xb = xa + ldx/2;
	yb = ya + ldy/2;
	nst; 
	for(i=0; i<nst; i++){
		pa.x = (xa+50)/100;	pa.y = (ya+50)/100;
		pb.x = (xb+50)/100;	pb.y = (yb+50)/100;
		pline(pa,pb);
		xa += ldx;	ya += ldy;
		xb += ldx;	yb += ldy;
	}
	pa.x = (xa+50)/100;	pa.y = (ya+50)/100;
	pline(pa,p2);

}

/*	arrow plotter		*/
short plot_arrow(a,c)
	objP a; char c;
{
	short dx, dy, v, u, ll;
	short x1, y1, x2, y2;
	short f1, f2, f3, f4;
	short x, y;
	short push_size;

	if(a->n<=1 || a->size==' ') return(-1);

	push_size = eff_size;
	if(eff_size>0) eff_size--;

	arrow2:;
	if(c=='a' || c=='c')	{
		x1 = (a->p0)->x;	y1 = (a->p0)->y;
		x2 = ((a->p0)+1)->x;	y2 = ((a->p0)+1)->y;
	}
	else {
		if(c=='b') {
			x1 = (a->p0[a->n-1]).x;	y1 = (a->p0[a->n-1]).y;
			x2 = (a->p0[a->n-2]).x;		y2 = (a->p0[a->n-2]).y;
		}
	}
	dx = x1 - x2;
	dy = y1 - y2;
	if(a->type=='S' || (a->type=='s' && *(a->txt)=='0') ) {
		x1 = (x1+x2)/2;
		y1 = (y1+y2)/2;
		dx /= 2;
		dy /= 2;
	}
	ll = norm(dx,dy,0);
	if(ll<4) return(0);		/* too small */
	u = ll/4; if(u>7) u = 7;
	v = 2*u;
	f1 = muldiv(u,dy,ll);
	f2 = muldiv(v,dx,ll);
	f3 = muldiv(u,dx,ll);
	f4 = muldiv(v,dy,ll);
	x = x1 + f1 - f2;
	y = y1 - f3 - f4;
	pline(Pt(x1,y1),Pt(x,y));
	x = x1 - f1 - f2;
	y = y1 + f3 - f4;
	pline(Pt(x1,y1),Pt(x,y));
	if(c=='c') {
		c = 'b';
		goto arrow2;
	}
	eff_size = push_size;
	return(0);
}

/*	Fill a disc of radius r centered at x1,y1
 *	jerq lib routine modified by theo for ped portability
 */
short discture(b, p, r, t)
	Bitmap *b;
	Point p;
	Texture *t;
{
	long eps,exy,dxsq,dysq;
	register x0,y0,x1,y1;
	r--;
	eps = 0;
	dxsq = 1;
	dysq = 1 - 2*r;
	x0 = p.x-1;
	x1 = p.x+1;
	y0 = p.y-r-1;
	y1 = p.y+r;
	while(y1 > y0) {
		exy = eps + dxsq + dysq;
		if(-exy <= eps+dxsq) {
			texture(b, Rect(x0, y0, x1, y0+1), t, dr_mode);
			texture(b, Rect(x0, y1, x1, y1+1), t, dr_mode);
			y1--;
			y0++;
			eps += dysq;
			dysq += 2;
		}
		if(exy <= -eps) {
			x1++;
			x0--;
			eps += dxsq;
			dxsq += 2;
		}
	}
}


short no_line(c) char c;
{
	if(c=='c' || c=='t' || c=='f') return(1);
	else return(0);
}

short klosed(c) char c;
{
	if(c>='P' && c<='S') return(1);
	else return(0);
}

short fullob(a) objP a;
{
	if(a->type=='Q') return(1);
	if(a->color>='E' && a->color<='P') return(1);
	else return(0);
}

short pdline(c,p1,p2)
	char c;
	Point p1, p2;
{
	Point mp;
	if(c==DASH) dline(p1,p2,15);
	else pline(p1,p2);
	if(c==HEAVY || c==DOUBLE ) {
		mp.x = (p1.x+p2.x)/2;
		mp.y = (p1.y+p2.y)/2;
		if(c==DOUBLE) rtext(mp,4,&crcross);
		else if(c==HEAVY) rtext(mp,4,&blksh);
	}
	else if(c=='i') rtext(p1,5,&blksh);
}

short rtext(q,d,tx)
	Point q;
	Texture * tx;
{
	texture(&display,Rect(q.x-d,q.y-d,q.x+d,q.y+d),tx,dr_mode);
}

short pline(p1,p2)
	Point p1, p2;
{
	register i;
	Code loc_mode;

	Point pa;
	long nst, llen, ldx, ldy;
	long xa, ya;
	short slen;

	if(!eff_size) {
		segment(&display,p1,p2,dr_mode);
		return;
	}
	/*	draw thick lines	*/
	loc_mode = dr_mode==F_STORE ? F_OR: dr_mode;

	llen = lng(p1,p2);
	nst = llen/eff_size;
	nst = (nst+50)/100;
	if(nst<3) {
		bitblt(vball[eff_size],vball[eff_size]->rect,
		&display,sub(p1,Pt(eff_size,eff_size)),loc_mode);
		/*
		bitblt(vball[eff_size],vball[eff_size]->rect,
		&display,sub(p2,Pt(eff_size,eff_size)),loc_mode);
		*/
		return;
	}
	ldx = (100*(p2.x-p1.x))/nst;
	ldy = (100*(p2.y-p1.y))/nst;
	xa = 100*p1.x;
	ya = 100*p1.y;
	for(i=0; i<=nst; i++){
		pa.x = (xa+50)/100;	pa.y = (ya+50)/100;
		bitblt(vball[eff_size],vball[eff_size]->rect,
		&display,sub(pa,Pt(eff_size,eff_size)),loc_mode);
		xa += ldx;	ya += ldy;
	}
}

short effs(n)
{
	n /= 4;	/* (0,8,11,12,16,20,24,36,40)	=>(0,2,2,3,4,5,6,9,10) */
	n -= 2;
	if(n<0) n = 0;
	else if(n>7) n = 7;
		/* (0,8,11,12,16,20,24,36,40)	=>(0,0,0,1,2,3,4,7, 7) */
	return(n);
}
