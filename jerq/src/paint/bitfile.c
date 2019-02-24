#include "paint.h"
/*
 * bitmap read/write code.  Stolen from Tom Killian and reworked for 5620 and jx.
 */
#define SHORTSIZE	16
#define sendword(w, f)	(putc(w&255, f), putc((w>>8)&255, f))
static Word buffer[XMAX/WORDSIZE], raster[XMAX/WORDSIZE];
static Bitmap bbuffer={ buffer, (sizeof buffer)/(sizeof(Word)), 0, 0, XMAX, 1};
static int ctype, count; static short *p1, *endraster;
/*
 * copy a rectangle from the given bitmap into a file on the host
 */
static putrast(), putbits(), getrast();
putbitmap(bp, r, f, compl)
Bitmap *bp, *compl;
Rectangle r;
FILE *f;
{
	register i, nrasters, rastwid, nrword;
	Rectangle rrast;
	nrasters=r.corner.y-r.origin.y;
	i=r.corner.x-r.origin.x;
	rastwid=(i+SHORTSIZE-1)/SHORTSIZE;
	nrword=(i+WORDSIZE-1)/WORDSIZE;
	endraster=(short *)raster+rastwid-1;
	sendword(nrasters, f);
	i|=0x8000;
	sendword(i, f);
	rectf(&bbuffer, bbuffer.rect, F_CLR);
	for(i=0;i<nrword;i++)
		raster[i]=0;
	rrast=r;
	if(compl)
		rectf(compl, r, F_XOR);
	for(;rrast.origin.y<r.corner.y;rrast.origin.y++){
		rrast.corner.y=rrast.origin.y+1;
		if(compl)
			rectf(compl, rrast, F_XOR);
		bitblt(bp, rrast, &bbuffer, Pt(0, 0), F_STORE);
		for(i=0;i<nrword;i++)
			raster[i]^=buffer[i];
		putrast(f);
		for(i=0;i<nrword;i++)
			raster[i]=buffer[i];
	}
}
static putrast(f)
FILE *f;
{
	register short *p2;
	p1=p2=(short *)raster;
	do{
		if(p1>=p2){
			p2=p1+1;
			count=2;
			ctype = *p1==*p2;

		}
		else if((*p2==*(p2+1))==ctype){
			if(++count>=127){
				putbits(f);
				p1=p2+2;
			}
			else
				p2++;

		}
		else if(ctype){
			putbits(f);
			p1=p2+1;
			ctype=0;

		}
		else{
			count--;
			putbits(f);
			p1=p2;
			ctype=1;
		}
	}while(p2<endraster);
	if(p1<=endraster){
		if(p2>endraster)
			count--;
		putbits(f);
	}
}
static putbits(f)
FILE *f;
{
	int c;
	c=count;
	if(ctype){
		c+=128;
		count=1;
	}
	putc(c, f);
	fwrite((char *)p1, sizeof *p1, count, f);
}
/*
 * allocate a bitmap and copy a bitmap from the given file into it
 */
Bitmap *getbitmap(f, compl)
FILE *f;
Bitmap *compl;
{
	register Bitmap *bp;
	register h, w, i, j;
	if((h=getc(f))<0)
		return 0;
	h|=getc(f)<<8;
	w=getc(f);
	w|=getc(f)<<8;
	if(w&0x8000){
		w&=~0x8000;
		w=(w+SHORTSIZE-1)/SHORTSIZE;
	}
	bp=balloc(Rect(0, 0, SHORTSIZE*w, h));
	if(bp==0)
		return(0);
	rectf(&bbuffer, bbuffer.rect, F_CLR);
	if(compl)
		rectf(compl, raddp(bp->rect, canvas->rect.or), F_XOR);
	for(i=0;i!=h;i++){
		if(getrast(f, (short *)raster, w)){
			bfree(bp);
			return(0);
		}
		for(j=0;j!=bp->width;j++)
			buffer[j]^=raster[j];
		bitblt(&bbuffer, bbuffer.rect, bp, Pt(0, i), F_STORE);
		if(compl)
			rectf(compl, raddp(Rect(0, i, SHORTSIZE*w, i+1),
						canvas->rect.or), F_XOR);
	}
	return(bp);
}
static getrast(f, p, nwords)	/* receive single compressed raster */
FILE *f;
register short *p;
int nwords;
{
	register i, count;
	for(;nwords>0;nwords-=count){
		if((count=getc(f))<=0)
			return 1;
		if(count&128){
			count&=127;	/* count must not be zero, not checked */
			if(fread(p, sizeof(short), 1, f)!=1)
				return 1;
			for(i=1;i!=count;i++){
				*(p+1) = *p;
				p++;
			}
			p++;
		}
		else{
			if(fread(p, sizeof(short), count, f)!=count)
				return 1;
			p+=count;
		}
	}
	return(nwords!=0);
}
