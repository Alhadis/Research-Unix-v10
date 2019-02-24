#include "jped.h"


char *menuMC[] = {
	"del obj",
	"copy obj",
	"mv obj",
	"brk obj",
	"attach",
	"join",
	"link obj",
	"match",
	"center",
	"family/pt",
	NULL};

short arrange(a)
	objP a;
{
	switch(subsel){
	case 0:
		zap_obj(a);
		break;
	case 1:
	case 2:
		mvcp(a,2-subsel);
		break;
	case 3:
		brk_obj(a);
		break;
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		join(a,subsel-5);
		break;
	case 9:
		join(a,8);
		break;
	}
}

short mvcp(a,copy)
	register objP a;
{
	register pointP pnt;
	register objP  bb;
	register abend = 0;
	Point start0;
	short dx, dy;
	short aseat;

	pnt = lockpnt;	lockpnt = 0;
	if(pnt==0) return(-1);
	release1();
	cursswitch( (Texture *)0 );
	start0 = *(a->p0);	/*save start*/
	aseat = obseat(a);
	if(a->type=='f' && a->size >(MXOBJ/2) ) bb = 0;
	else
	bb = dupl_obj(a,a->p0,a->p0 + a->n-1);
	if(!bb) {
		return(-1);
	}
	nosk = 0;	plot_one(a);	/*initial simplified*/
	for(;;wait(MOUSE)){
		nap(2);
		plot_one(a);	/*erase*/
		tr_obj(a,GMX-pnt->x,GMY-pnt->y);	/*see FT*/
		plot_one(a);
		if(button1()) break;
		if(button23()) { abend = 1; break; }
	}
	plot_one(a);	nosk = 1;
	while(button123()) wait(MOUSE);
	if(copy) {
		if(aseat>=0) obj[aseat] = bb;
		if(abend) return(0);
		obj[no_obj++] = a;
		plot_one(a);
	}
	else {
		plot_one(a);
		plot_one(bb);	/*erase old*/
		if(abend) {
			dx = (a->p0)->x-start0.x;
			dy = (a->p0)->y-start0.y;
			stp->a = a;	stp->x = -dx;	stp->y = -dy;
			stp++;
		}
	}
	return(0);
}

/* FT: pnt points to the same obj. point. Its x,y values change*/

short tr_obj(a,dx,dy) objP a;
{
	pointP pp;
	register i;
	if(a->type=='f') {
		(a->p0)->x += dx;	(a->p0)->y += dy;
		for(i=0;i<a->size;i++){
			tr_obj(a->mem[i],dx,dy);
		}
	}
	else {
		for(pp=a->p0, i=0; i<a->n; i++, pp++){
			if(pp->x < SILLY) continue;
			pp->x += dx;	pp->y += dy;
		}
	}
}

/* delete object */

short zap_obj(a)
	register objP a;
{
	release1();
	plot_one(a);	/*erase*/
	if(!no_kill())  a->status = 0;
	else plot_one(a);
}

/*	join objects	*/

/*
	mode = -1	attach (tightest internal connection)
	mode = 0	join visible (tighter internal connection)
	mode = 1	link (invisible)
	mode = 2	put on top
	mode = 3	center
	mode = 8	make them a family
*/

short join(a,mode)
	register objP a;
{
	register objP b;
	Point pa, pb,  pc, pp;
	register j = 0;
	Point linkers[8];
	register link = 0;
	short k;
	release1();
	if(!lockpnt) return(-1);
	if(mode<1) { if(a->type != 'p') return(-1); }
	else if(mode==1) { if(a->type != 'p' && a->type !='P') return(-1); }
	else if(mode>7) {
		if((k=obseat(a))>=0) {
			obj[k] = 0;
			tmpobf[j++] = a;
		}
	}
	if(mode != 3) pa = *lockpnt;
	else gcenter(a,&pa,1);
	if(mode<1) if(chk_end(a,1)) return(-1);
	while(rubber(pa,&pp)) {	/*search for other objects*/
		pb = pp;
		release1();
		b = s_locate(obj,no_obj,pb);
		if(!b) continue;
		if(lockpnt) pc = *lockpnt;
		else continue;
		if(mode <2) {
			if(b==a) {
				if(!mode && a->type=='p'){ /*close*/
					plot_one(a);
					a->type = 'P';
					plot_one(a);
					break;
				}
				continue;
			}
			if(b->type != a->type) continue;
			if(mode<1) if(chk_end(b,0)) continue;
			merge(a,b,mode);
			if(mode==1) {
				linkers[(link++)%8] = pc;
				dline(*(a->p0),pc,10);
			}
			else break;
		}
		else {	/*mode>=2*/
			if(mode<8) {	/* move*/
				plot_one(b);
				if(mode==3) gcenter(b,&pc,1);
				tr_obj(b,pa.x-pc.x,pa.y-pc.y);
				plot_one(b);
			}
			if(mode>7 && b!=a) {	/*family*/
				if((k=obseat(b))>=0) {
					obj[k] = 0;
					tmpobf[j++] = b;
					dr_link(tmpobf[0],b);
				}
			}
		}
	}
	if(mode>7 && j) {
		k = j;
		mk_fam(tmpobf,k);
	}
	if(mode==1) {
		for(j=0;j<link;j++) dline(*(a->p0),linkers[j],10);
	}
	return(0);
}

short merge(o1,o2,mode) objP o1,o2;
{
	register pointP q1, q2;
	register pointP pp;
	short dx, dy;
	register j;

	q1 = o1->p0;
	q2 = o2->p0;
	dx = (q1+o1->n-1)->x - q2->x;
	dy = (q1+o1->n-1)->y - q2->y;
	plot_one(o1);
	plot_one(o2);
	o1->p0 = (pointP)Alloc((unsigned)((o1->n+o2->n+mode+2)*sizeof(pointS)));
	o1->txt = 0;	/*just in case*/
	for(j=0,pp=o1->p0;j<o1->n;j++,pp++,q1++) *pp = *q1;
	if(mode>0) {
		pp->x = pp->y = ABSURD;
		pp++;
	}
	else if(mode<0){
		tr_obj(o2, dx,  dy);
		q2++;
		o2->n--;
	}
	for(j=0;j<o2->n;j++,pp++,q2++) *pp = *q2;
	o1->n += o2->n;
	if(mode>0) o1->n += mode;	/*for phantom point*/
	plot_one(o1);
	if((j=obseat(o2))>=0) obj[j] = 0;
	return(0);
}



short rev_obj(a)
	register objP a;
{
	register j, n;
	register pointP pp, qq;
	pointS tpp;
	if( !(a->type=='p' || a->type=='s') ) return(-1);
	pp = a->p0;	qq = pp + (a->n - 1);
	if(drawpar) bmark(*(a->p0),ball);
	n = a->n /2;
	for(j=0;j<n;j++,pp++,qq--) {
		tpp = *qq;	*qq = *pp;	*pp = tpp;
	}
	if(a->size=='a') a->size = 'b';
	else if(a->size=='b') a->size = 'a';
	if(drawpar) bmark(*(a->p0),ball);
	return(0);
}

short chk_end(a,tk)
	register objP a;
{
	register i, j;
	if(tk) { i = a->n - 1; j = 0; }	/* we look for end */
	else { i = 0; j = a->n-1; }
	if(lockpnt != (a->p0 + i)) { /*not end if tk=1, not start if tk=0*/
		if(lockpnt == (a->p0 +j)) {	/*other end*/
			rev_obj(a);
		}
		else return(-1);
	}
	return(0);
}


short oddf(a) objP a;
{
	register i;
	register pointP pp;
	for(pp=a->p0, i=0; i<a->n; i++, pp++){
		if(pp->x < SILLY || pp->y < SILLY) return(i+1);
	}
	return(0);
}

short brk_obj(a)
	register objP a;
{
	register j, k;
	register objP b;
	Point cp;
	register Point *pp, *qq;
	register i;

	release1();
	if(a->type != 'p' && a->type != 'P') return(-1);
	if(k=oddf(a)) {
		j = k - 2;
		plot_one(a);
		goto horror;
	}
	if(!lockpnt) return(-1);
	j = lockpnt - a->p0;
	if(a->type=='p' && (!j || j==a->n-1) ) return(-1);
	cp = *lockpnt;
	rtext(cp,4,&grey);
	while(!button1()) wait(MOUSE);
	if(a != s_locate(obj,no_obj,mouse.xy)) goto breturn;
	k = lockpnt - a->p0;
	if(a->type=='P'){
		if( (!j && k==a->n-1) || (!k && j==a->n-1) ) {
			a->type = 'p';
			pline(*(a->p0+j),*(a->p0+k));
			goto breturn;
		}
	}
	if(k>j) k = j + 1; 
	else {
		k = j;	j--;
	}
	if((a->p0+j-1)->x < SILLY ) j--;
	if((a->p0+k)->x < SILLY ) k++;
	if(j<0 || k>a->n-1) goto breturn;
	plot_one(a);
	if(a->type=='p') {	/*break open object into two*/
		horror:
		b = dupl_obj(a,a->p0+k,a->p0+a->n-1);
		obj[no_obj++] = b;
		a->n = j+1;
		plot_one(b);
	}
	else { 	/* break closed object*/
		a->type = 'p';
		for(pp=pbuf,qq=a->p0+k,i=k;i<a->n;i++) *pp++ = *qq++;
		for(qq=a->p0,i=0;i<=j;i++) *pp++ = *qq++;
		a->n = pp - pbuf;
		for(i=0,pp=pbuf,qq=a->p0;i<a->n;i++) *qq++ = *pp++;
	}
	plot_one(a);
	breturn:
		rtext(cp,4,&grey);
	return(0);
}
