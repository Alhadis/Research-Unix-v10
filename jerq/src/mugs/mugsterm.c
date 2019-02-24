/*% 3cc -go #.m %
 */
#include <jerq.h>
#include "mugs.h"
#include <font.h>
#ifdef lint
void rectf(), sendnchars(), bfree(), reboot(), sendchar(), bitblt(), segment();
#endif
#define	NGAL	8
#define	or	origin
#define	co	corner
int locor=0, hicor=0, range=256;
char buf[100];
char squash[FSIZE][FSIZE];
Bitmap *whole;
char *menutext[]={
	"window",
	"in",
	"out",
	"save",
	"read",
	"exit",
	0
};
int xsize, ysize;
Point pick={(NGAL+1)/2, (NGAL+1)/2};
Rectangle msgrect, galrect;
Rectangle grect(p)
Point p;
{
	Rectangle v;
	v.or=add(galrect.or, Pt(p.x*(FSIZE+3), p.y*(FSIZE+3)));
	v.co=add(v.or, Pt(FSIZE, FSIZE));
	return v;
}
Point big;
Menu menu={menutext};
picksquare(rp)
register Rectangle *rp;
{
	Point p, q, r, s;
	p=mouse.xy;
	xhair(p);
	while(!button123()){
		q=mouse.xy;
		if(!eqpt(p, q)){
			xhair(p);
			p=q;
			xhair(p);
		}
		wait(CPU);
	}
	if(!xhair(p) || !button3()){
		while(button123())
			wait(CPU);
		alive();
		return(0);
	}
	q=p;
	box(Rpt(Pt(2*p.x-q.x, p.y), q), F_XOR);
	while(button3()){
		r=mouse.xy;
		if(r.x<big.x) r.x=big.x; else if(r.x>big.x+xsize) r.x=big.x+xsize;
		if(r.y<big.y) r.y=big.y; else if(r.y>big.y+ysize) r.y=big.y+ysize;
		if(2*abs(r.x-p.x)<abs(r.y-p.y)){
			if(r.x>p.x) r.x=p.x+abs(r.y-p.y)/2;
			else r.x=p.x-abs(r.y-p.y)/2;
		}
		else if(r.y>p.y) r.y=p.y+2*abs(r.x-p.x);
		else r.y=p.y-2*abs(r.x-p.x);
		if(!eqpt(r, q)){
			box(Rpt(Pt(2*p.x-q.x, p.y), q), F_XOR);
			q=r;
			box(Rpt(Pt(2*p.x-q.x, p.y), q), F_XOR);
		}
	}
	box(Rpt(Pt(2*p.x-q.x, p.y), q), F_XOR);
	rp->or=sub(Pt(2*p.x-q.x, p.y), big);
	rp->co=sub(q, big);
	alive();
	return(1);
}
xhair(p)
Point p;
{
	if(!ptinrect(p, Rpt(big, add(big, Pt(xsize, ysize))))) return 0;
	segment(&display, Pt(big.x, p.y), Pt(big.x+xsize, p.y), F_XOR);
	segment(&display, Pt(p.x, big.y), Pt(p.x, big.y+ysize), F_XOR);
	return 1;
}
box(r, f)
Rectangle r;
{
	r.co=sub(r.co, Pt(1, 1));
	segment(&display, r.or, Pt(r.or.x, r.co.y), f);
	segment(&display, Pt(r.or.x, r.co.y), r.co, f);
	segment(&display, r.co, Pt(r.co.x, r.or.y), f);
	segment(&display, Pt(r.co.x, r.or.y) ,r.or, f);
}
readface(){
	char name[512];
	getline(name, "File name: ");
	if(getwhole(name)){
		locor=0;
		hicor=0;
		range=256;
		getsquash(Rect(0, 0, xsize, ysize));
	}
}
main(){
	Rectangle r;
	request(KBD|MOUSE);
	msgrect.or.x=Drect.or.x+5;
	msgrect.or.y=Drect.co.y-20;
	msgrect.co=Drect.co;
	galrect.or.x=Drect.co.x-5-(NGAL+1)*(FSIZE+3);
	galrect.or.y=Drect.or.y+5;
	galrect.co=add(galrect.or, Pt((NGAL+1)*(FSIZE+3), (NGAL+1)*(FSIZE+3)));
	readface();
	drawborder(grect(pick));
	for(;;){
		while(!(own()&MOUSE))
			wait(MOUSE);
		if(button1()) pickface();
		if(button3()) switch(menuhit(&menu, 3)){
		case 0:
			bitblt(whole, whole->rect, &display, big, F_STORE);
			if(picksquare(&r)){
				drawborder(raddp(r, big));
				getsquash(r);
			}
			break;
		case 1:
			zoomin();
			break;
		case 2:
			zoomout();
			break;
		case 3:
			sendicon();
			break;
		case 4:
			readface();
			break;
		case 5:
			ok();
			while(!button123())
				wait(CPU);
			alive();
			if(button3()){
				sendchar(QUIT);
				exit();
			}
			while(button123())
				wait(CPU);
		}
		gallery();
		wait(CPU);
	}
}
int floyd[FSIZE+1][FSIZE+1];
int contab[RES+1];
Bitmap *icon;
drawface(lo, hi, facep)
Point facep;
{
	register int *p, *f;
	register char *sq;
	register v, h, s, e, t;
	if(!icon)
		icon=balloc(Rect(0, 0, FSIZE, FSIZE));
	if(!icon)
		return;
	if(hi<lo){e=hi; hi=lo; lo=e;}
	if(lo<0) lo=0; else if(RES<lo) lo=RES;
	if(hi<0) hi=0; else if(RES<hi) hi=RES;
	t=hi-lo;
	p=contab;
	for(v=0;v!=lo;v++) *p++=0;
	for(;v!=hi;v++) *p++=(v-lo)*RES/t;
	for(;v<=RES;v++) *p++=RES;
	sq=squash[0];
	for(v=0;v!=FSIZE;v++) for(h=0,f=floyd[v];h!=FSIZE;h++)
		*f++=contab[*sq++&0xFF];
	for(v=0;v!=FSIZE;v++){
		f=floyd[v];
		for(h=0,p=addr(icon, Pt(0, v));h<FSIZE;h+=32,p++){
			*p=0;
			for(s=0;s!=32 && s+h!=FSIZE;s++,f++){
				if((e=f[0])>RES/2)
					e-=RES;
				else
					*p|=1<<(31-s);
				t=3*e/8;
				f[FSIZE+1]+=t;
				f[FSIZE+2]+=e-2*t;
				f[1]+=t;
			}
		}
	}
	bitblt(icon, icon->rect, &display, facep, F_STORE);
}
getwhole(name)
char *name;
{
	register Word *w;
	register x, y;
	dead();
	if(whole)
		bfree(whole);
	sendchar(WHOLE);
	while(*name) sendchar(*name++);
	sendchar('\n');
	switch(gtchar()){
	case WHOLE:
		break;
	case NGFILE:
		alive();
		pause("Can't read face");
		return 0;
	default:
		err("WHOLE failure");
	}
	xsize=gtint();
	ysize=gtint();
	big.x=Drect.or.x;
	big.y=msgrect.or.y-2-ysize;
	whole=balloc(Rect(0, 0, xsize, ysize));
	if(!whole)
		err("whole balloc failure");
	w=whole->base;
	for(y=0;y!=ysize;y++){
		for(x=0;x<xsize;x+=32)
			*w++=gtint();
		bitblt(whole, Rect(0, y, xsize, y+1),
			&display, add(big, Pt(0, y)), F_STORE);
	}
	alive();
	return 1;
}
getsquash(r)
Rectangle r;
{
	register h, v, hsize, vsize;
	dead();
	sendchar(SQUASH);
	sendint(r.or.x);
	sendint(r.or.y);
	sendint(r.co.x);
	sendint(r.co.y);
	if(gtchar()!=SQUASH)
		err("bad SQUASH");
	hsize=gtint();
	vsize=gtint();
	for(v=0;v!=vsize;v++) for(h=0;h!=hsize;h++)
		squash[v][h]=gtchar();
	alive();
	startgallery();
}
gtchar(){
	register c;
	while((c=rcvchar())==-1)
		wait(RCV);
	return(c&0xff);
}
sendicon(){
	register v, h;
	char name[512];
	register char *s;
	if(!icon)
		return;
	bitblt(&display, grect(pick), icon, icon->rect.or, F_STORE);
	getline(name, "Face name: ");
	dead();
	sendchar(SAVEFACE);
	s=name;
	while(*s)
		sendchar(*s++);
	sendchar('\n');
	if(gtchar()!=OK){
		alive();
		pause("write failed");
	}
	else{
		sendint(icon->rect.co.x-icon->rect.or.x);
		sendint(icon->rect.co.y-icon->rect.or.y);
		for(v=icon->rect.or.y;v!=icon->rect.co.y;v++)
			for(h=0;h!=icon->width;h++)
				sendint(icon->base[v*icon->width+h]);
		alive();
	}
	msg("");
}
gtint(){
	register i;
	i=gtchar();
	i=(i<<8)|gtchar();
	i=(i<<8)|gtchar();
	return((i<<8)|gtchar());
}
sendint(i)
register i;
{
	char buf[4];
	buf[0]=i>>24;
	buf[1]=i>>16;
	buf[2]=i>>8;
	buf[3]=i;
	sendnchars(4, buf);
}
Texture deadmouse = {
	 0x0000, 0x0114, 0xA208, 0x4100,
	 0x0000, 0x0008, 0x0004, 0x0082,
	 0x0441, 0xFFE1, 0x5FF1, 0x3FFE,
	 0x17F0, 0x03E0, 0x0000, 0x0000,
};
Texture *svcurs;
dead(){
	svcurs=cursswitch(&deadmouse);
}
Texture confirm = {
	 0x000E, 0x071F, 0x0317, 0x736F,
	 0xFBCE, 0xDB8C, 0xDBC0, 0xFB6C,
	 0x77FC, 0x0000, 0x0001, 0x0003,
	 0x94A6, 0x633C, 0x6318, 0x9490,
};
ok(){
	svcurs=cursswitch(&confirm);
}
Texture question={
	0x1e00, 0x3f80, 0x6180, 0xc0c0, 
	0xc0c0, 0x60c0, 0x0180, 0x0300, 
	0x0600, 0x0c00, 0x0c00, 0x0c00, 
	0x0000, 0x0000, 0x0c00, 0x0c00, 

};
what(){
	svcurs=cursswitch(&question);
}
Texture sweep = {
	 0x43FF, 0xE001, 0x7001, 0x3801,
	 0x1D01, 0x0F01, 0x8701, 0x8F01,
	 0x8001, 0x8001, 0x8001, 0x8001,
	 0x8001, 0x8001, 0x8001, 0xFFFF,
};
boxcur(){
	svcurs=cursswitch(&sweep);
}
alive(){
	cursswitch(svcurs);
}
msg(s)
char *s;
{
	rectf(&display, msgrect, F_CLR);
	nmsg(s);
}
nmsg(s)
char *s;
{
	rectf(&display, Rpt(string(&defont, s, &display, msgrect.or,
					F_STORE), msgrect.co), F_CLR);
}
pause(m)
char *m;
{
	msg(m);
	what();
	while(!button123())
		wait(CPU);
	while(button123())
		wait(CPU);
	alive();
}
err(s)
char *s;
{
	pause(s);
	exit();
}
getline(buf, prompt)
char buf[];
char *prompt;
{
	register int c;
	register char *s=buf;
	char line[512];
	sprintf(line, "%s\1", prompt);
	msg(line);
	wait(KBD);
	while((c=kbdchar())!='\n' && c!='\r'){
		switch(c){
		case -1:
			wait(KBD);
			continue;
		case '\b':
			if(s!=buf)
				*--s='\0';
			break;
		case 027:	/* ctrl-w */
			while(s!=buf && !idchar(s[-1]))
				--s;
			while(s!=buf && idchar(s[-1]))
				--s;
			*s='\0';
			break;
		case '@':
			s=buf;
			*s='\0';
			break;
		default:
			*s++=c;
			*s='\0';
			break;
		}
		sprintf(line, "%s%s\1 ", prompt, buf);
		nmsg(line);
	}
}
idchar(c)
register c;
{
	return('a'<=c && c<='z' || 'A'<=c && c<='Z' || '0'<=c && c<='9' || c=='_');
}
drawborder(r)
Rectangle r;
{
	box(inset(r, -1), F_OR);
	box(inset(r, -2), F_CLR);
	box(inset(r, -3), F_OR);
}
clrborder(r)
Rectangle r;
{
	box(inset(r, -1), F_CLR);
	box(inset(r, -2), F_CLR);
	box(inset(r, -3), F_CLR);
}
zoomin(){
	int lopick=locor+range*pick.y/NGAL, hipick=hicor+range*pick.x/NGAL;
	if(range>NGAL) range>>=1;
	zoomset(lopick, hipick);
}
zoomout(){
	int lopick=locor+range*pick.y/NGAL, hipick=hicor+range*pick.x/NGAL;
	if(range!=256) range<<=1;
	zoomset(lopick, hipick);
}
zoomset(locen, hicen){
	locor=locen-range/2;
	hicor=hicen-range/2;
	if(hicor<locor) hicor=locor=(hicor+locor)/2;
	if(locor<0) locor=0;
	if(locor+range>256) locor=256-range;
	if(hicor<0) hicor=0;
	if(hicor+range>256) hicor=256-range;
	startgallery();
}
int galy=NGAL+1, galx=0;
startgallery(){
	data();
	galy=0;
	galx=0;
}
data(){
	char buf[100];
	sprintf(buf, "locor %d hicor %d range %d pick lo %d pick hi %d",
		locor, hicor, range,
		locor+pick.y*range/NGAL, hicor+pick.x*range/NGAL);
	msg(buf);
}
gallery(){
	if(galy>NGAL) return;
	if(hicor+galx*range/NGAL>=locor+galy*range/NGAL)
		drawface(locor+galy*range/NGAL, hicor+galx*range/NGAL,
			grect(Pt(galx, galy)).or);
	else
		rectf(&display, grect(Pt(galx, galy)), F_CLR);
	if(++galx>NGAL){
		galx=0;
		galy++;
	}
}
pickface(){
	Point p;
	while(button1()){
		p=mouse.xy;
		if(ptinrect(p, galrect)){
			p=sub(p, galrect.or);
			p.x/=(FSIZE+3);
			p.y/=(FSIZE+3);
			if(hicor+p.x*range/NGAL>=locor+p.y*range/NGAL
			&& !eqpt(p, pick)){
				clrborder(grect(pick));
				pick=p;
				drawborder(grect(pick));
			}
		}
	}
	data();
}
