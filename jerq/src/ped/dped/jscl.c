/*	two menus: ch_size and remote	*/

#include "jped.h"

extern short not_obj();

extern short noscale;

short kseat;

char *menuTR[] = {
	"expand",
	"shrink",
	"rotate",
	"turn 45",
	"v-flip",
	"h-flip",
	"v-elong",
	"h-elong",
	"undo rot",
	NULL};

short transf_obj(a)
	objP a;
{
	Point cp;
	short jseat;

	if(subsel != 8) kseat = -1;
	if( (jseat = obseat(a)) <0 ) return(-1);
	if(noscale && a->type=='t') return(0);

	switch(subsel){
	case 0:
		host_one(jseat,"x");
		break;
	case 1:
		host_one(jseat,"s");
		break;
	case 2:
		if(gcenter(a,&cp,1)) break;
		irotate(jseat,cp);
		kseat = jseat;
		break;
	case 3:
		host_one(jseat,"t");
		kseat = jseat;
		break;
	case 4:
		flip(a,0,1);
		break;
	case 5:
		flip(a,1,0);
		break;
	case 6:
		host_one(jseat,"v");
		break;
	case 7:
		host_one(jseat,"h");
		break;
	case 8:
		if(jseat==kseat)
		host_one(jseat,"u");
		kseat = -1;
		break;
	}
	return(0);
}

/*	center of bounding rectangle	*/
Rectangle bbox;

short gcenter(a,cp,half)
	objP a;
	Point *cp;
{
	bbox.origin = board.corner;
	bbox.corner = board.origin;
	adj_box(a);
	cp->x = (bbox.origin.x + bbox.corner.x);
	cp->y = (bbox.origin.y + bbox.corner.y);
	if(half) { cp->x /=2;	cp->y /=2; }
	return(0);
}

short adj_box(a)
	register objP a;
{
	register i;
	register pointP pp;
	switch(a->type){
	case 'f':
		for(i=0;i<a->size;i++) adj_box(a->mem[i]);
		break;
	default:
		for(i=0,pp=a->p0;i<a->n;i++,pp++) {
			if(pp->x < SILLY) continue;
			if(pp->x < bbox.origin.x) bbox.origin.x = pp->x;
			if(pp->x > bbox.corner.x) bbox.corner.x = pp->x;
			if(pp->y < bbox.origin.y) bbox.origin.y = pp->y;
			if(pp->y > bbox.corner.y) bbox.corner.y = pp->y;
		}
	}
}

/* -- -- flip -- -- */

short flip(a,horz,vert)
	register objP a;
{
	Point cp;
	if(gcenter(a,&cp,0)) return(-1);
	plot_one(a);
	f1(a,horz,vert,cp);
	plot_one(a);
	return(0);
}

short f1(a,h,v,c)
	register objP a;
	Point c;
{
	register j;
	register pointP pp;
	if(a->type=='f'){
		for(j=0;j<a->size;j++) f1(a->mem[j],h,v,c);
	}
	else {
		for(pp=a->p0,j=0; j<a->n; pp++,j++) {
			if(pp->x < SILLY) continue;
			if(h) pp->x = c.x - pp->x;
			if(v) pp->y = c.y - pp->y;
		}
	}
	return(0);
}

/* interactive rotation routine */

short irotate(idx,cp)
	short idx;
	Point cp;
{
	register j;
	short  rad;
	Point op, fp;
	bmark(cp,ball);
	/* select two points */
	if(!lockpnt) return(0);
	op = *lockpnt;	lockpnt = 0;
	rad = norm(cp.x-op.x,cp.y-op.y,0);
	pcirc(cp,rad);
	fp.x = 2*op.x-cp.x;	fp.y = 2*op.y-cp.y;
	pline(cp,fp);
	while(button1()) wait(MOUSE);	/*release*/
	for(;;wait(MOUSE)) {
		if(button1()) j=1;
		else if(button3()) j = 3;
			else j = 0;
		if(j) break;
	}
	pline(cp,fp);	pcirc(cp,rad);
	bmark(cp,ball);
	if(j==3) return(0);	/*cancel*/
	affine(cp,op,mouse.xy);
	host_one(idx,"r");
	return(0);
}

short host_one(idx,s)
	short idx;
	char *s;
{
	register objP a;
	static char p[4];
	char c1;
	Texture * oldcur;

	if(idx<0 || idx>=no_obj) return(-1);
	a = obj[idx];
	p[0]='P';	p[1] = s[0];	p[2] = 0;
	oldcur  = cursswitch(&wave2);
	sendstring(p);
	trans_one(a);
	s_char('Q');
	s_flush();
	if(s[0] != 'k') plot_one(a);	/*erase*/
	while((c1=g_char()) != '$'){
		if(c1=='^') {
			if(s[0] < 'i' || s[0] > 'k') obj[idx] = rec_one(a);
			else obj[idx] = rec_one((objP)0);
		}
		else not_obj(c1);
	}
	obj[idx]->status = 11;
	if(s[0] != 'k') plot_one(obj[idx]);
	cursswitch(oldcur);
	return(-1);
}

/*	remote menu	*/


char *menuRO[] = {
	"delete",
	"copy",
	"move",
	"read",
	"write",
	"formal",
	"xpand",
	"shrink",
	"v-flip",
	"h-flip",
	NULL};

short remote()
{
	Rectangle sweep;
	char p[16], c1;
	short n;
	register objP a;
	Texture * oldcur;
	short zapr;

	if( subsel<0 || subsel>m_size ) return(-1);
	if(no_obj) return(-1);
	if(ownrec(&sweep)) sweep = board;
	rectf(&display,sweep,F_XOR);
	zapr = 1;
	p[0] = 'Q';
	p[1] = menuRO[subsel][0];
	p[2] = 0;
	if(p[1]=='d') {
		if(no_kill()) goto cancel;
	}
	else {
		if(p[1]=='r' || p[1]=='w') {
			oldcur = cursswitch(&deadmouse);
			jtag(1," ");
			D_ON = 1;
			n = kbdstr(scbf,placelab,1);
			cursswitch(oldcur);
			if(n<1 || scbf[n-1]=='@') goto cancel;
			strcat(p,scbf);
			placelab.x += strwidth(&defont,scbf);
		}
		else {
			press1();
			release1();
		}
	}
	oldcur = cursswitch(&wave2);
	sendstring(p);
	s_rect(sweep);
	s_short(mouse.xy.x);
	s_short(mouse.xy.y);
	s_flush();
	/* mouse coordinates are not used always */
	rectf(&display,sweep,F_XOR);
	zapr = 0;
	while( (c1=g_char()) != '$'){
		switch(c1){
		case 'E':	/* erase */
			dr_mode = F_CLR;
			continue;
		case 'W':	/* write */
			dr_mode = F_STORE;
			continue;
		case('^'):
			a = rec_one((objP)0);
			plot_one(a);
			Free(0);
			continue;
		default:
			not_obj(c1);
		}
	}
	dr_mode = F_XOR;
	/* get message from end of edit loop */
	n = g_short();
	getnchars(n,scbf);
	jtag(0,scbf);
	cursswitch(oldcur);
	cancel:
	if(zapr) rectf(&display,sweep,F_XOR);
	return(0);
}
