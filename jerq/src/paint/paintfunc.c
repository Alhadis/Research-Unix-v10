#include "paint.h"
int textured=0;
Texture tex={
	 0xCCCC, 0xCCCC, 0x3333, 0x3333,
	 0xCCCC, 0xCCCC, 0x3333, 0x3333,
	 0xCCCC, 0xCCCC, 0x3333, 0x3333,
	 0xCCCC, 0xCCCC, 0x3333, 0x3333,
};
curbutton(){
	return(button1()?0:1);
}
drawbrush(p, but)
Point p;
{
	if(textured)
		texcomp(canvas, raddp(brush->rect, sub(sub(p, brushcen),
					brush->rect.or)),
			&tex, brush->alf, ops[curop][but]);
	else
		composite(brush, brush->rect, canvas, sub(p, brushcen),
			ops[curop][but]);
}	
Fpaint(){
	Point p, q;
	p=mouse.xy;
	q=sub(p, Pt(1, 1));
	while(button12()){
		if(p.x!=q.x || p.y!=q.y){
			drawbrush(p, curbutton());
			q=p;
		}
		p=mouse.xy;
		wait(CPU);
	}
}
int lsindex;
Flines(){
	Point p, q, r;
	register but=curbutton();
	lsindex=0;
	p=q=mouse.xy;
	segment(&display, p, q, F_XOR);
	while(button12()){
		r=mouse.xy;
		if(r.x!=q.x || r.y!=q.y){
			segment(&display, p, q, F_XOR);
			segment(&display, p, r, F_XOR);
			q=r;
			sleep(2);
		}
		else
			wait(CPU);
	}
	segment(&display, p, q, F_XOR);
	paintline(p, q, but, 1);
}
Fcurves(){
	Point p, q;
	lsindex=0;
	p=mouse.xy;
	q=sub(p, Pt(1, 1));
	while(button12()){
		if(p.x!=q.x || p.y!=q.y){
			paintline(q, p, curbutton(), 0);
			q=p;
		}
		p=mouse.xy;
		wait(CPU);
	}
}
paintline(p, q, but, endpt)
Point p, q;
{
	register delta;
	Point slope, t, d;
	slope.x=2*abs(q.x-p.x);
	slope.y=2*abs(q.y-p.y);
	d.x=q.x>p.x?1:-1;
	d.y=q.y>p.y?1:-1;
	if(slope.x>slope.y){
		delta = -slope.x/2;
		for(;;){
			if(!endpt && p.x==q.x)
				break;
			switch(linestring[lsindex]){
			case '.':			break;
			case 'A': drawbrush(p, but);	break;
			case 'B': drawbrush(p, 1-but);	break;
			}
			if(linestring[++lsindex]=='\0')
				lsindex=0;
			if(p.x==q.x)
				break;
			p.x+=d.x;
			if((delta+=slope.y)>0){
				delta-=slope.x;
				p.y+=d.y;
			}
			wait(CPU);
		}
	}
	else{
		delta = -slope.y/2;
		for(;;){
			if(!endpt && p.y==q.y)
				break;
			switch(linestring[lsindex]){
			case '.':			break;
			case 'A': drawbrush(p, but);	break;
			case 'B': drawbrush(p, 1-but);	break;
			}
			if(linestring[++lsindex]=='\0')
				lsindex=0;
			if(p.y==q.y)
				break;
			p.y+=d.y;
			if((delta+=slope.x)>0){
				delta-=slope.y;
				p.x+=d.x;
			}
			wait(CPU);
		}
	}
}
Fcircles(){
	Point p, q, r;
	register but=curbutton();
	lsindex=0;
	p=q=mouse.xy;
	circle(&display, p, len(sub(q, p)), F_XOR);
	while(button12()){
		r=mouse.xy;
		if(r.x!=q.x || r.y!=q.y){
			circle(&display, p, len(sub(q, p)), F_XOR);
			circle(&display, p, len(sub(r, p)), F_XOR);
			q=r;
			sleep(2);
		}
		else
			wait(CPU);
	}
	circle(&display, p, len(sub(q, p)), F_XOR);
	lsindex=0;
	paintcircle(p, len(sub(q, p)), but);
}
len(p)
Point p;
{
	register s, u, x=p.x*p.x+p.y*p.y;
	if(x<=0)
		return(0);
	for(s=2, u=4;u<x;s+=s, u*=4);
	while((u=((x+s*s)/s)>>1)<s)
		s=u;
	return(s);
}
/*	Form a circle of radius r centered at x1,y1
 *	The boundary is a sequence of vertically, horizontally,
 *	or diagonally adjacent points that minimize 
 *	abs(x^2+y^2-r^2).
 */
err(p, r)
Point p;
{
	return(abs(p.x*p.x+p.y*p.y-r*r));
}
paintcircle(p, r, but)
Point p;
{
	Point q;
	register ex, ey, exy;
	if(r<=0){
		switch(linestring[lsindex]){
		case '.':			break;
		case 'A': drawbrush(p, but);	break;
		case 'B': drawbrush(p, 1-but);	break;
		}
		return;
	}
	q.x=p.x-r;
	q.y=p.y;
	do{
		switch(linestring[lsindex]){
		case '.':			break;
		case 'A': drawbrush(q, but);	break;
		case 'B': drawbrush(q, 1-but);	break;
		}
		if(linestring[++lsindex]=='\0')
			lsindex=0;
		ex=err(add(sub(q, p), Pt(1, 0)), r);
		ey=err(add(sub(q, p), Pt(0, 1)), r);
		exy=err(add(sub(q, p), Pt(1, 1)), r);
		if(exy<ex && exy<ey){
			q.x++;
			q.y++;
		}
		else if(ex<ey)
			q.x++;
		else
			q.y++;
		wait(CPU);
	}while(q.x!=p.x);
	do{
		switch(linestring[lsindex]){
		case '.':			break;
		case 'A': drawbrush(q, but);	break;
		case 'B': drawbrush(q, 1-but);	break;
		}
		if(linestring[++lsindex]=='\0')
			lsindex=0;
		ex=err(add(sub(q, p), Pt(1, 0)), r);
		ey=err(add(sub(q, p), Pt(0, -1)), r);
		exy=err(add(sub(q, p), Pt(1, -1)), r);
		if(exy<ex && exy<ey){
			q.x++;
			--q.y;
		}
		else if(ex<ey)
			q.x++;
		else
			--q.y;
		wait(CPU);
	}while(q.y!=p.y);
	do{
		switch(linestring[lsindex]){
		case '.':			break;
		case 'A': drawbrush(q, but);	break;
		case 'B': drawbrush(q, 1-but);	break;
		}
		if(linestring[++lsindex]=='\0')
			lsindex=0;
		ex=err(add(sub(q, p), Pt(-1, 0)), r);
		ey=err(add(sub(q, p), Pt(0, -1)), r);
		exy=err(add(sub(q, p), Pt(1, -1)), r);
		if(exy<ex && exy<ey){
			--q.x;
			--q.y;
		}
		else if(ex<ey)
			--q.x;
		else
			--q.y;
		wait(CPU);
	}while(q.x!=p.x);
	do{
		switch(linestring[lsindex]){
		case '.':			break;
		case 'A': drawbrush(q, but);	break;
		case 'B': drawbrush(q, 1-but);	break;
		}
		if(linestring[++lsindex]=='\0')
			lsindex=0;
		ex=err(add(sub(q, p), Pt(-1, 0)), r);
		ey=err(add(sub(q, p), Pt(0, 1)), r);
		exy=err(add(sub(q, p), Pt(-1, 1)), r);
		if(exy<ex && exy<ey){
			--q.x;
			q.y++;
		}
		else if(ex<ey)
			--q.x;
		else
			q.y++;
		wait(CPU);
	}while(q.y!=p.y);
}
