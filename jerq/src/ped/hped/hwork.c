/*	process objects at host		*/

#include "hped.h"

extern short no_scale;

extern double cos(), sin();
#define PI	3.14159265358979

double v, w;	/*rotation parameters*/

/*	find center of bounding rectangle	*/
Rectangle bbox;

gcenter(a,cp,half)
	objP a;
	Point *cp;
{
	bbox.origin.x = 8000;
	bbox.origin.y = 8000;
	bbox.corner.x = -8000;
	bbox.corner.y = -8000;
	adj_box(a);
	cp->x = bbox.origin.x + bbox.corner.x;
	cp->y = bbox.origin.y + bbox.corner.y;
	if(half) { cp->x /=2;	cp->y /=2; }
	return(0);
}

adj_box(a)
	register objP a;
{
	register i;
	register Point * pp;
	switch(a->type){
	case 'f':
		for(i=0;i<a->size;i++) adj_box(a->mem[i]);
		break;
	default:
		for(i=0,pp=a->p0;i<a->n;i++,pp++) {
			if(pp->t < 0) continue;
			if(pp->x < bbox.origin.x) bbox.origin.x = pp->x;
			if(pp->x > bbox.corner.x) bbox.corner.x = pp->x;
			if(pp->y < bbox.origin.y) bbox.origin.y = pp->y;
			if(pp->y > bbox.corner.y) bbox.corner.y = pp->y;
		}
	}
}

actupon(kind,a)
	char kind;
	objP a;
{
	Point cp;
	objP bb;
	double t1, t2;

	switch(kind){
	case 'f':	/*formal*/
		formatt(1,&a);
		break;
	case 'h':	/*hor.elong*/
		if(gcenter(a,&cp,1)) break;
		ch_size(a,1.25,0.8,cp);
		break;
	case 'i':	/* fix format */
		if(a->type != 'p' && a->type != 'P') break;
		if(a->color==HEAVY || a->color==DOUBLE) {
			heavy(a,&bb);
			free_one(a);
			a = dupl_obj(bb);
		}
		else if(a->color=='i') {
				ispline(a,&bb);
				free_one(a);
				a = dupl_obj(bb);
			}
		break;
	case 'j':	/* process by user command */
		if(utrans(a,&bb)) {
			rrecord(mailer);
			break;
		}
		free_one(a);
		a = dupl_obj(bb);
		break;
	case 'k':	/* replace circle by spline */
		cir_to_sp(a,&bb);
		free_one(a);
		a = dupl_obj(bb);
		break;
	case 'r':	/*rotate around pp1, from pp2 to pp3*/
		rotate(a);
		break;
	case 's':	/*shrink*/
		if(gcenter(a,&cp,1)) break;
		ch_size(a,0.8,0.8,cp);
		break;
	case 't':	/*rotate by 45*/
		if(gcenter(a,&pp1,1)) break;
		v = cos(PI/4);
		w = sin(PI/4);
		rot_one(a,v,w);
		break;
	case 'u':	/*cancel rotation*/
		t1 = v*v + w*w;
		if(t1) {
			v /= t1;
			w /= t1;
			w = -w;
		}
		rot_one(a,v,w);
		break;
	case 'v':	/*vertical elong*/
		if(gcenter(a,&cp,1)) break;
		ch_size(a,0.8,1.25,cp);
		break;
	case 'x':	/*expand*/
		if(gcenter(a,&cp,1)) break;
		ch_size(a,1.25,1.25,cp);
		break;
	}
	return(0);
}

/* change size of obj */

ch_size(a,scalex,scaley,cp)
	objP a;
	double scalex, scaley;
	Point cp;
{
	register Point *pp;
	register j;

	if(a->type=='f'){
		for(j=0;j<a->size;j++) ch_size(a->mem[j],scalex,scaley,cp);
	}
	else {
		if(a->type=='c' || (a->type=='t' && !no_scale) ) {
			a->size = (a->size*scalex);
		}
		for(pp=a->p0,j=0; j<a->n; j++, pp++){
			if(pp->t <0) continue;
			pp->x = cp.x + (scalex*(pp->x - cp.x));
			pp->y = cp.y + (scaley*(pp->y - cp.y));
		}
		fprintf(frecord,"PED.ch_size: type=%c size=%d=%c\n",
		a->type,(short)a->size,(char)a->size);
	}
	return(0);
}

#define fabs(A) ( A>0? A: -A)

rotate(a)
	objP a;
{
	/* compute rotation angle from glob. vars */
	double odx, ody, ndx, ndy,  u;
	odx = pp2.x - pp1.x;	ody = pp2.y - pp1.y;
	ndx = pp3.x - pp1.x;	ndy = pp3.y - pp1.y;
	/*	do not rotate if first point is too close to the center	*/
	if(fabs(odx)<(5.*scale) && fabs(ody)<(5.*scale)) return(0);
	/*	rotate by 90 deg if second point is too close to the center	*/
	if(fabs(ndx)<(5.*scale) && fabs(ndy)<(5.*scale)){
		v = 0;
		w = 1;
	}
	else {
		v = ndx*odx + ndy*ody;
		w = ndy*odx - ndx*ody;
		u = odx*odx + ody*ody;
		v /= u;
		w /= u;
	}
	rot_one(a,v,w);
	return(0);
}
/* basic object rotation: around pp1, cosb = v, sinb = w (0-1) */

rot_one(a,vv,ww) objP a; double vv,ww;
{
	double tx, ty;
	register Point * pp;
	register i;
	if(a->type=='f') {
		for(i=0;i<a->size;i++) rot_one(a->mem[i],vv,ww);
	}
	else {
		for(pp=a->p0,i=0;i<a->n;pp++,i++){
			if(pp->t <0) continue;
			tx = (pp->x-pp1.x)*vv - (pp->y-pp1.y)*ww;
			ty = (pp->x-pp1.x)*ww + (pp->y-pp1.y)*vv;
			pp->x = pp1.x + tx;	pp->y = pp1.y + ty;
		}
	}
}


goperate(kind,cp,r,backtalk)
	char *kind;
	Point cp;
	Rectangle r;
	char *backtalk;
{
	register i, j;
	double vx, vy;
	short obindex;

	backtalk[0]='h';
	backtalk[1]=0;
	vx = cp.x - r.origin.x;
	vy = cp.y - r.origin.y;
	work_no = 0;
	if(kind[0] != 'r') {	/*not reading*/
		for(i=0,j=0;i<no_obj;i++) {
			if(!obj[j]->status) continue;
			if(inters(obj[i]->bounds,r)) work_obj[j++] = obj[i];
		}
		for(i=0;i<j;i++) {
		fprintf(frecord,"%g,%g~%g,%g cuts %g,%g~%g,%g\n",
		work_obj[i]->bounds.origin.x, work_obj[i]->bounds.origin.y,
		work_obj[i]->bounds.corner.x, work_obj[i]->bounds.corner.y,
		r.origin.x, r.origin.y, r.corner.x, r.corner.y);
		}
		sprintf(backtalk,"work=%d ",j);
		if(!(work_no = j)) {
			s_char('$'); fflush(toblit);
			return(-1);
		}
	}
	/*	erase blit screen selectively	*/
	s_char('E');
	if(kind[0] != 'r' && kind[0] != 'w' && kind[0] != 'c') {
		for(j=0;j<work_no;j++) trans_one(work_obj[j]);
	}
	s_char('W');
	switch(kind[0]){
	case 'd':	/*delete*/
		rrecord("delete %d objetcs",work_no);
		for(j=0;j<work_no;j++) work_obj[j]->status = 0;
		s_char('$'); fflush(toblit);
		return(0);
	case 'c':	/*copy*/
		rrecord("copy %d objects",work_no);
		obindex = no_obj;
		for(j=0;j<work_no;j++) cp_obj(work_obj[j],vx,vy);
		for(j=obindex;j<no_obj;j++) {
			ex_one(obj[j]);
			trans_one(obj[j]);
		}
		s_char('$'); fflush(toblit);
		return(0);
	case 'm':	/*move*/
		rrecord("move %d objects",work_no);
		for(j=0;j<work_no;j++) tr_obj(work_obj[j],vx,vy);
		break;
	case 'r':	/*read*/
		rd_ped(kind+1,work_obj,&work_no);
		strcat(backtalk,mailer);
		obindex = no_obj;
		blend(r,work_obj,work_no);
		if(def_font) qfont(obj+obindex,work_no);
		for(j=obindex;j<no_obj;j++) {
			trans_one(obj[j]);
			ex_one(obj[j]);
		}
		s_char('$'); fflush(toblit);
		return(0);
	case 'w':	/*write*/
		parked  = 1;
		wr_objs(kind+1);
		rrecord("write %d objects",work_no);
		strcat(backtalk,mailer);
		s_char('$'); fflush(toblit);
		return(0);
	case 'f':	/*formal*/
		rrecord("formalize %d objects",work_no);
		formatt(work_no,work_obj);
		break;
	case 'x':	/*expand*/
		rrecord("expand %d objects",work_no);
		for(j=0;j<work_no;j++) ch_size(work_obj[j],1.25,1.25,cp);
		break;
	case 's':	/*shrink*/
		rrecord("shrink %d objects",work_no);
		for(j=0;j<work_no;j++) ch_size(work_obj[j],0.8,0.8,cp);
		break;
	case 'v':	/*vert. flip*/
		rrecord("v-flip %d objects",work_no);
		for(j=0;j<work_no;j++) flip(work_obj[j],cp,0,1);
		break;
	case 'h':	/*horz. flip*/
		rrecord("h-flip %d objects",work_no);
		for(j=0;j<work_no;j++) flip(work_obj[j],cp,1,0);
		break;
	}
	if( def_font && (kind[0]=='x' || kind[0]=='s' || kind[0]=='e') ){
		qfont(work_obj,work_no);
	}
	for(j=0;j<work_no;j++) {
		ex_one(work_obj[j]);	/*wasteful but safe*/
		trans_one(work_obj[j]);
	}
	s_char('$'); fflush(toblit);
	return(0);
}

blend(r,o,n)
	Rectangle r;	/*where objects should fit*/
	objP o[];
	short n;
{
	register i, j;
	register objP a;
	Rectangle rr;
	double sc;
	short dx, dy;
	rrecord("blending %d objects into %d",n,no_obj);
	ob_range(&rr,o,n);
	mapr(0,r,rr,&sc,&dx,&dy);
	fprintf(frecord,"scale = %g dx=%d, dy=%d\n",sc,dx,dy);
	for(j=0;j<n;j++) {
		scale_obj(o[j],sc,dx,dy);
		obj[no_obj++] = o[j];
	}
	rrecord("new total = %d",no_obj);
}

scale_obj(a,sc,dx,dy)
	objP a;
	double sc;
	short dx, dy;
{
	register i;
	for(i=0;i<a->n;i++) {
		a->p0[i].x = sc*a->p0[i].x + dx;
		a->p0[i].y = sc*a->p0[i].y + dy;
	}
	if(a->type=='c' || a->type=='t') a->size *= sc;
	if(a->type=='f') {
		for(i=0;i<a->size;i++) scale_obj(a->mem[i],sc,dx,dy);
	}
}
