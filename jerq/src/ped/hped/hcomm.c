#include "hped.h"
#include <sys/types.h>
#include <sys/stat.h>

#define ABSURD -8192
#define SILLY -4096
#define UBOUND 0
#define BOUND 1


short needfnt[6];

char g_char(){
	char c;
	if( fread(&c,sizeof(char),1,fromblit) <1) return(-1);
	return(c);
}

short g_short(){
	char c[2];
	short n;
	if( fread(c,sizeof(char),2,fromblit) <2) return(-1);
	n = c[0];	n &= 0377;	n |= c[1]<<8;
	return(n);
}

int g_Point(p) Point *p;
{
	short xt, yt;
	xt = g_short();
	yt = g_short();
	if(xt < SILLY) {
		p->x = p->y = ABSURD;
		return(-1);
	}
	else {
		p->x = (xt-xshft)/scale;
		p->y = (yt-yshft)/scale;
		return(0);
	}
}

int g_Rect(r) Rectangle *r;
{
	g_Point(&(r->origin));
	g_Point(&(r->corner));
}

short getstring(s)
char *s;
{
	register short n;
	n = g_short();
	if(n<1) return(n);
	n = fread(s,sizeof(char),n,fromblit);
	s[n] = NULL;
	return(n);
}

sendstring(s)
char *s;
{
	short n;
	n = strlen(s);
	if(s[n-1]=='\n') {
		s[n-1] = 0;
		n--;
	}
	if(n<1) {
		rrecord("Tried to send zero string");
		return;
	}
	s_short(n);
	fwrite(s,sizeof(char),n,toblit);
}

s_short(k)	short k;
{
	char c[2];
	c[0] = k & 0377;
	c[1] = (k>>8) & 0377;
	fwrite(c, sizeof c, 1, toblit);
}

s_char(a) char a;
{
	char c;
	c = a;
	fwrite(&c, sizeof c, 1, toblit);
}

/* receive 3 pnts: pp[123] */

get3(){
	pp1.x = (g_short()-xshft)/scale;
	pp1.y = (g_short()-yshft)/scale;
	pp2.x = (g_short()-xshft)/scale;
	pp2.y = (g_short()-yshft)/scale;
	pp3.x = (g_short()-xshft)/scale;
	pp3.y = (g_short()-yshft)/scale;
}

Point * objhead(a,lvl)
	objP a;
{
	register i;
	if(a->type == 'f') {
		*(a->p0) = *(objhead(a->mem[0],1));
		for(i=1;i<a->size;i++){
			if((a->mem[i])->type=='f') objhead(a->mem[i],2);
		}
	}
#ifdef FLAKES
	rrecord("obhd: %d-> %c %g %g",lvl,a->type,(a->p0)->x,(a->p0)->y);
#endif
	return(a->p0);
}

/* transmit */
transmit(tkind) char *tkind;
{
	register i;
	register objP a;
	short n_array, n_obj, nv;
	char b_a;

#ifdef CAUTIOUS
	rrecord("scale: %g %d %d",scale,xshft,yshft);
#endif
	if(*tkind=='T'){ /* is there space? */
		n_array = g_short();
		n_obj = g_short();
		if( ((n_obj + bn)>256 /*OUCH*/) || ( n_array < (nv=volume(bb,bn)) ) )
			b_a = 'N';
		else b_a = 'Y';
		s_char(b_a);
		fflush(toblit);
		rrecord("Space=%c, bn=%d, bl_o=%d, room=%d nv=%d",
			b_a,bn,n_obj,n_array,nv);
		if(b_a=='N') return(-1);
	}

	/*	see if any fonts are needed	*/
	if(def_font){
		if(*tkind=='D') qfont(obj,no_obj);
		else if(*tkind=='T') qfont(bb,bn);
	}
	switch(*tkind){
	case 'D':	/* display only */
		not_in_blit = 1;
		for(i=0;i<no_obj;i++) {
			if( !(a = obj[i])) continue;
			if(inters(a->bounds,viewport)) {
				if(a->type=='f') objhead(a,0);
				trans_one(a);
			}
		}
		break;
	case 'T':
		not_in_blit = 0;
		for(i=0;i<bn;i++) {
			if(!(a = bb[i])) continue;
			if(a->type=='f') objhead(a,0);
			trans_one(a);
		}
		break;
	default:;
	}
	s_char('$');
	fflush(toblit);
	return(0);
}


trans_one(a) objP a;
{
	register j;
	objP b;
	short truecol;

#ifdef CAUTIOUS
	rrecord("send %c %g %d %d",a->type,scale,xshft,yshft);
#endif
	if( not_in_blit && a->n>1 &&  (a->type=='p' || a->type=='P') ){
		if(a->color==HEAVY || a->color==DOUBLE) {
			heavy(a,&b);
			a = b;
		}
		else if(a->color=='i') {
				ispline(a,&b);
				a = b;
			}
	}

	if(not_in_blit && a->type=='S' && a->color>='E' && a->color<='P') {
		proc_fill(a);
		if(a->size) {
			truecol = a->color;
			a->color = SOLID;
			trans_one(a);
			a->color = truecol;
		}
		return(0);
	}

	s_char('^');
	s_char(a->status);
	s_short((short) a->n);
	if(a->type=='c' || a->type == 't') s_short((short)(a->size*scale+0.5));
	else s_short((short)a->size);
	s_short(a->ps);
	s_char('#');
	s_char(a->type);
	s_char(a->color);
	s_char(a->pen);
	if(a->type=='s' || a->type=='S') {
		s_char('#');
		sendstring(a->txt);
	}
	if(a->type=='t'){
		s_char('#');
		sendstring(a->txt);
	}
	for(j=0;j<a->n;j++){
		s_char('v');
		if(a->p0[j].t<0) {
			s_short((short)ABSURD);
			s_short((short)ABSURD);
		}
		else {
			s_short((short) (scale*a->p0[j].x + xshft + 0.5));
			s_short((short) (scale*a->p0[j].y + yshft + 0.5));
		}
	}
	if(a->type=='f'){
		for(j=0;j<a->size;j++){
			trans_one(a->mem[j]);
		}
	}
	return(1);
}

/* receive */
receive(){
	register i;
	if(bn){	/*erase bb*/
		for(i=0;i<bn;i++) if(bb[i]) Free(bb[i]);
	}
	bn  = 0;
	while(rec_one(bb,&bn));
	fwrite("H",sizeof(char),1,toblit);
	fflush(toblit);
}



rec_one(u,N) objP u[]; short *N;
{
	register j;
	register objP a;
	char aa[512];
	char stat;
	short n;

	stat = g_char();
#ifdef CAUTIOUS
	fprintf(frecord,"REC status = %d ",stat);
#endif
	if(stat=='Q') return(0);
	if(!stat){
		return(1);
	}
	/*	we always create new object now	*/
	a = (objP)malloc((unsigned)(sizeof(objS)));
	u[(*N)++] = a;
	n = g_short();
	a->status = stat;
	a->n = n;
	a->size = g_short();
	a->ps = g_short();
	a->type  = g_char();
#ifdef CAUTIOUS
	fprintf(frecord,"n = %d t = %c sz = %g\n",a->n,a->type,a->size);
#endif
	if(a->type == 'c' || a->type == 't') a->size /= scale;
	a->color = g_char();
	a->pen = g_char();
	if(a->type=='t' || a->type=='s' || a->type=='S') {
		n = g_short();
		fread(aa,sizeof(char),n,fromblit);
		aa[n] = 0;
		a->txt = malloc((unsigned)(strlen(aa)+1));
		strcpy(a->txt,aa);
	}
	a->p0 = (Point *)malloc((unsigned)((a->n+2)*sizeof(Point)));
	for(j=0;j<a->n;j++){
		(a->p0+j)->t = g_Point(a->p0+j);
	}
	if(a->type=='f'){
		a->mem = (objP *)malloc((unsigned)(a->size*sizeof(objP)));
		n = 0;
		while(rec_one(a->mem,&n));
		if(n != a->size) fprintf(frecord,"Disagree: %d %g\n",n,a->size);
	}
	return(1);
}

/* place everything in bb */

choose_all(){
	register j;
	if(bn) merge_bb();
	for(j=0;j<no_obj;j++){
		bb[j] = obj[j];
		obj[j] = 0;
	}
	bn = no_obj;
	no_obj = 0;
	rrecord("tot BBsize= %d",list_size(bb,bn));
}

/* locate objects in rectangle and transfer them from OBJ to BB */


choose(){
	register j;
	register objP a;
	Rectangle ch_rect;
	g_Rect(&ch_rect);
	if(bn)  merge_bb();
	for(j=0, bn=0; j<no_obj;j++){
		a = obj[j];
		if(!a) continue;
		if(obj_in(a,ch_rect)) {
			bb[bn++] = a;
			obj[j] = 0;
		}
	}
	if(bn) {
		viewport = ch_rect;
		scal(UBOUND,viewport);
	}
	rrecord("sel BBsize= %d",list_size(bb,bn));
	if(!bn) sendstring("NN");
	else sendstring("TT");
	fflush(toblit);
	return(0);
}


obj_in(a,r)
	objP a;
	Rectangle r;
{
	register i, k;
	register Point *pp;
	if(a->type=='f') k = 1; else k = a->n;
	for(i=0, pp=a->p0; i<k; i++,pp++){
		if(pp->x < r.origin.x || pp->x > r.corner.x) continue;
		if(pp->y < r.origin.y || pp->y > r.corner.y) continue;
		/*	point in rectangle	*/
		a->status = 2;
		return(2);
	}
	/*	object not in rectangle	*/
	if(a->type=='f'){
		for(i=0;i<a->size;i++){
			k = obj_in(a->mem[i],r);
			if(k>0) {
				a->status = k;
				return(k);
			}
		}
	}
	a->status = 9;
	return(0);
}

/*	merge BB into OBJ	*/

objP tmpobf[MXOBJ];

merge_bb(){
	register i, j;
	/*	pack obj first	*/
	if(!no_obj) goto merg;
	for(i=0;i< no_obj;i++) tmpobf[i] = obj[i];
	for(i=0,j=0;i< no_obj;i++){
		if(tmpobf[i]) obj[j++] = tmpobf[i];
	}
	no_obj = j;
	/*	now merge	*/
	merg: if(!bn) return(0);
	for(i=0;i<bn;i++){
		if(bb[i]) obj[no_obj++] = bb[i];
		ex_one(bb[i]);
		bb[i] = 0;
	}
	bn = 0;
	return(1);
}

/*	zap object	*/

free_one(a)	objP a;
{
	register i;
	if(a->type=='f'){
		for(i=0;i<a->size;i++) free_one(a->mem[i]);
	}
	if(a->type=='t') Free(a->txt);
	Free(a->p0);
	Free(a);
	return(0);
}

#define OBJ_SIZE 16
#define PNT_SIZE  4

list_size(v,n)
	objP v[];
	short n;
{
	register i, k;
	register objP a;
	k = 0;
	for(i=0;i<n;i++) {
		a = v[i];
		if(a->type=='f') {
			k += OBJ_SIZE + (a->size)*4;
			k += list_size(a->mem,(short)a->size);
		}
		else {
			k += OBJ_SIZE + (a->n)*PNT_SIZE;
			if(a->type=='t') k += strlen(a->txt);
			else if(a->type=='s' || a->type=='S') k += a->n;
		}
	}
	return(k);
}

qfont(a1,n1)
	objP a1[];
	short n1;
{
	register i, j;
	if(!def_font) {
		rrecord("uncalled call for font loading");
		return(-1);
	}
	for(i=0;i<5;i++) needfnt[i] = 0;
	fmark(a1,n1);
	fprintf(frecord,"need fonts: ");
	for(i=0;i<5;i++) fprintf(frecord,"%d",needfnt[i]);
	fprintf(frecord,"\n");
	for(i=0;i<5;i++){
		if(needfnt[i]) {
			j = loadfont(i);
			if(j) rrecord("sent font %d",i);
			else rrecord("font %d not sent",i);
		}
	}
	return(0);
}

volume(o,n)
	objP o[];
	short n;
{
	short k;
	register i, j;
	register objP a;
#ifdef FLAKY
	rrecord("volume of %d objects",n);
#endif
	k = 0;
	for(i=0;i<n;i++){
		a = o[i];
		if(!a) continue;
		k += 16;	/*sizeof(objS) on Blit*/
		if(!a->status) continue;
		if(a->type=='f') k += volume(a->mem,(short)a->size);
		k += 4*a->n;
		if(a->type=='t') k += ( strlen(a->txt) + 1);
		if(a->type=='s' || a->type=='S') k += a->n;
	}
	return(k);
}
