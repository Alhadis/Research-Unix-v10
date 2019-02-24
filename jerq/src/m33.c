#include <jerq.h>
#include <font.h>

#define		LINESIZE	128
#define		NLSIZE		16
#define		CURSOR		'\01'
#define		D		(&display)
Rectangle	scrollrect;
Point		curpos;
char		line[LINESIZE];
char		*linep;

main()
{
	register got;
    Reshaped:
	scrollrect=inset(Drect, 5);
	curpos=scrollrect.origin;
	linep=line;
	echo(CURSOR, 0);
	request(RCV);
	for(; got=wait(RCV); ){
		if(P->state&RESHAPED){
			rectf(D, Drect, F_CLR);
			P->state&=~RESHAPED;
			goto Reshaped;
		}
		if(got&RCV)
			dorcv();
	}
}
dorcv(){
	register unsigned c;
	Point p;
	echo(CURSOR, 0);
	while((c=rcvchar())!=-1)
		echo(c, 1);
	echo(CURSOR, 0);
}
echo(c, advance)
{
	register Fontchar *fp;
	Rectangle r;
	Point p;
	switch(c&=0x7F){
	default:
		fp=defont.info+c;
		if(fp->width+curpos.x >= scrollrect.corner.x)
			newline();
		p=curpos;
		r.origin.x=fp->x;
		r.corner.x=(fp+1)->x;
		r.origin.y=fp->top;
		r.corner.y=fp->bottom;
		p.y+=fp->top;
		bitblt(defont.bits, r, D, p, F_XOR);
		if(advance){
			curpos.x+=fp->width;
			if(linep-line<LINESIZE)
				*linep++=c;
		}
		break;
	case '\n':
		newline();
		break;
	case '\7':
		*((char *)(384*1024L+062)) = 2;	/* beep */
		break;
	case '\r':
		curpos.x=scrollrect.origin.x;
		break;
	case '\013':	/* ^K: reverse linefeed */
		if(curpos.y>scrollrect.origin.y)
			curpos.y-=NLSIZE;
		break;
	case '\b':
		backspace();
		break;
	case '\014':
		formfeed();
		break;
	case '\t':
		curpos.x=nexttab(curpos.x);
		if(curpos.x>=scrollrect.corner.x)
			newline();
		if(linep-line<LINESIZE)
			*linep++=c;
		break;
	}
}
int eightspaces=72;
nexttab(x){
	register xx=x-scrollrect.origin.x;
	return(xx-(xx%eightspaces)+eightspaces+scrollrect.origin.x);
}
backspace()
{
	register char *p;
	register x=scrollrect.origin.x;
	if(linep>line){
		for(p=line; p<linep-1; p++)
			if(*p=='\t')
				x=nexttab(x);
			else
				x+=defont.info[*p].width;
		curpos.x=x;
		--linep;
		if(*p!='\t')
			echo(*p, 0);
	}
}
newline()
{
	Rectangle r;
	if(curpos.y+2*NLSIZE > scrollrect.corner.y-2){
		r=scrollrect;
		r.origin.y+=NLSIZE;
		bitblt(D, r, D, Pt(r.origin.x, r.origin.y-NLSIZE), F_STORE);
		clear(Rect(scrollrect.origin.x, curpos.y,
			scrollrect.corner.x, scrollrect.corner.y));
	}else
		curpos.y+=NLSIZE;
	curpos.x=scrollrect.origin.x;
	linep=line;
}
formfeed()
{
	clear(scrollrect);
	curpos=scrollrect.origin;
	linep=line;
}
clear(r)
	Rectangle r;
{
	rectf(D, r, F_CLR);
}
