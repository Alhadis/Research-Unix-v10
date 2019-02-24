
#include"jped.h"

extern short eff_size;

extern short effs();

short noscale = 0;

char story[256];

Point pbuf[MXPTS];


char *menuDR[] = {
	"no grid",
	"grid:FMC",
	"lines",
	"text",
	"circle",
	"box",
	"oval",
	"family/bx",
	"user oper",
	"fix sz(t)",
	NULL};

short use_pen()
{
	short k;
	static nett = 0;
	objP a;

	if(D_ON){
		D_ON = 0;
		rectf(&display,board,F_CLR);
		return(0);
	}

	switch(subsel){
	case 0:
		tog_grid(nett=0);
		break;
	case 1:
		nett = (nett+1)%4;
		tog_grid(nett);
		break;
	case 2: 
		if(nosk && !drawpar) eff_size = effs(lin_size);
		mice_pts();
		eff_size = 0;
		break;
	case 3:
	case 4:
		draw_spec(subsel-3);
		break;
	case 5:
	case 6:
		draw_box(subsel-5);
		break;
	case 7:
		if(mk_assemb(tmpobf,&k)) mk_fam(tmpobf,k);
		break;
	case 8:
		k = no_obj;
		draw_spec(0);
		if(k>=no_obj) break;
		a = obj[k];
		story[0] = 'u';
		story[1] = ' ';
		story[2] = 0;
		strcat(story,a->txt+2);
		sendstring(story);
		host_one(k,"juser");
		break;
	case 9:
#ifdef OLDTIMES
		if(def_font) {
			def_font = 0;
			sendstring("FN");
			jtag(0,"fixed char. size");
		}
		else {
			def_font = 1;	/*it will be activated on next plot*/
			sendstring("FY");
			jtag(0,"varbl. char. size");
		}
#else
		noscale = 1 - noscale;
		jtag(0,"text will");
		if(noscale) { jtag(0," not"); sendstring("Fn"); }
		else sendstring("Fy");
		jtag(0," be scalled");
#endif
		break;
	}
	return(0);
}

/* -- collect points through the mouse -- */

short rubber(cp,rp)
	Point cp, *rp;
{
	Point q, oq;

	oq = cp;
	for(;;nap(2),wait(MOUSE)){
		q.x = GMX;	q.y = GMY;
		*rp = q;
		pline(cp,oq);	/*erase old*/
		if(button23()) return(0);
		if(button1()) return(1);
		pline(cp,q);	/*draw new*/
		oq = q; /* save mouse coord. */
	}
}

short mice_pts() {
	short nn;
	Point  *pbfp;
	Point q;
	Point pq;
	Texture *oldcur;
	objP bb;
	pbfp = pbuf;

	oldcur = cursswitch( (Texture *)0);
	for(;;nap(2),wait(MOUSE)) {
		if(button23()) goto telos;
		if(button1()) break;
	}
	q.x = GMX;	q.y = GMY;
	*pbfp++ = q;
	
	while(rubber(q,&pq)){
		pline(q,pq);
		q = pq;
		if(FAR) *pbfp++ = q;
		release1();
	}

	nn = pbfp - pbuf;
	if(nn>1) {
		bb =
		mk_obj(pbuf,(char *)0,'p',SOLID,nn,(short)' ',(short)0,lin_size);
		if(bb) {
			if(drawpar) bmark(pbuf[0],ball);
			obj[no_obj++] = bb;
		}
	}
	telos: cursswitch(oldcur);
	while(button123()) wait(MOUSE);
	return(-1);
}

char strsep[2] = { SEPAR, 0 };

short draw_spec(kind){
	Point cp, cp0;
	Point pq;
	objP bb;
	short rad;
	register char * cpn;
	Texture *oldcur;

	for(;;sleep(2),wait(MOUSE)){
		if(button23()) break;
		if(!button1()) continue;
		release1();
		cp.x = GMX;	cp.y = GMY;
		cp0 = cp;
		switch(kind){
		case 0:	/*text*/
			story[0] = '\\';
			story[1] = textmode;
			story[2] = 0;
			oldcur = cursswitch(&deadmouse);
			while( kbdstr(scbf,cp,1) > 0){
				strcat(story,scbf);
				strcat(story,strsep);
				cp.y += (defont.height+3);
			}
			for(cpn=story;*cpn;cpn++);
			cpn--; *cpn = 0;
			if((cpn-2)>story)
			bb = 
			mk_obj(&cp0,story,'t',ROMAN,
				(short)1,def_size,(short)0,def_size);
			else bb = 0;
			if(bb) wipe(cp0,story+2);
			cursswitch(oldcur);
			goto f01;
		case 1:	/*circle*/
			bmark(cp,poscross);
			if(rubber(cp,&pq)) {
				rad = norm(pq.x-cp.x,pq.y-cp.y,0);
				if(rad < 2) rad = 2;
				bb =
				mk_obj(&cp,(char *)0,'c',SOLID,1,rad,0,lin_size);
			}
			else bb = 0;
			bmark(cp,poscross);
			goto f01;
		default:
			continue;
		}
	}
	f01: if(bb) {
		obj[no_obj++] = bb;
		plot_one(bb);
	}
	for(;;wait(MOUSE)) if(!button23()) break;
	return(0);
}

extern char * find_sep();

short wipe(cp,st)
	Point cp;
	char st[];
{
	register char *sp, *sp0;
	sp = sp0 = st;
	do {
		sp = find_sep(sp0);
		if(sp) *sp = 0;
		pstring(cp,sp0);
		sp0 = sp + 1;
		cp.y += (defont.height+3);
	} while(sp);
}


short draw_box(oval)
{
	Rectangle sweep;
	Point cp[4];
	objP bb;
	if(ownrec(&sweep)) return(-1);
	if(grsize){
		sweep.origin.x = gri(sweep.origin.x,xshift);
		sweep.origin.y = gri(sweep.origin.y,yshift);
		sweep.corner.x = gri(sweep.corner.x,xshift);
		sweep.corner.y = gri(sweep.corner.y,yshift);
	}
	cp[0] = sweep.origin;
	cp[2] = sweep.corner;
	cp[1].x = sweep.origin.x;
	cp[1].y = sweep.corner.y;
	cp[3].x = sweep.corner.x;
	cp[3].y = sweep.origin.y;
	bb = mk_obj(cp,(char *)0,'P',SOLID,(short)4,(short)' ',(short)0,lin_size);
	if(bb) {
		obj[no_obj++] = bb;
		plot_one(bb);
		if (oval) poly_to_spl(bb);
	}
	return(0);
}

short xshift, yshift;

short tog_grid(stepsz) {
	if(grsize) plot_grid();	/*erase old*/
	grsize = 0;
	if(!stepsz) return(-1);
	sendstring("GQ");
	if(g_char()=='Y'){
		xshift = g_short();
		yshift = g_short();
	}
	else {
		if(no_obj) transmit();
		sendstring("II");
		xshift = g_short();
		yshift = g_short();
		if(no_obj) {
			purge(0);
			receive('R');
		}
	}
	grsize = 010<<stepsz;
	grihalf = grsize>>1;
	plot_grid();
	return(-1);
}

short gri(k,shf){
	short u, v;
	v = (k-shf)%grsize;	u = (k-shf)/grsize;
	if(v>grihalf) u++;
	else if(v < -grihalf) u--;
	k = shf + grsize*u;
	return(k);
}

