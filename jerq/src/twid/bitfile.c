#include <jerq.h>
#include <jerqio.h>
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
putbitmap(bp, r, s, compl)
Bitmap *bp, *compl;
Rectangle r;
char *s;
{
	register i, nrasters, rastwid, nrword;
	Rectangle rrast;
	register FILE *f=fopen(s, "w");
	if(f==0)
		return 0;
	nrasters=r.corner.y-r.origin.y;
	i=r.corner.x-r.origin.x;
	rastwid=(i+SHORTSIZE-1)/SHORTSIZE;
	nrword=(i+WORDSIZE-1)/WORDSIZE;
	endraster=(short *)raster+rastwid-1;
	sendword(0, f);
	sendword(r.origin.x-Drect.origin.x, f);
	sendword(r.origin.y-Drect.origin.y, f);
	sendword(r.corner.x-Drect.origin.x, f);
	sendword(r.corner.y-Drect.origin.y, f);
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
	fclose(f);
	return(1);
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
	int c, i;
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
 * reads current format and two obsolete formats
 */
Bitmap *getbitmap(s, compl)
char *s;
Bitmap *compl;
{
	register Bitmap *bp=0;
	register w, i, j;
	register FILE *f=fopen(s, "r");
	Rectangle r;
	if(f==0)
		return 0;
	if((r.corner.y=getc(f))<0){
		fclose(f);
		return 0;
	}
	r.corner.y|=getc(f)<<8;
	if(r.corner.y==0){
		r.origin.x=getc(f);
		r.origin.x|=getc(f)<<8;
		r.origin.y=getc(f);
		r.origin.y|=getc(f)<<8;
		r.corner.x=getc(f);
		r.corner.x|=getc(f)<<8;
		r.corner.y=getc(f);
		r.corner.y|=getc(f)<<8;
		w=(r.corner.x-r.origin.x+SHORTSIZE-1)/SHORTSIZE;
	}
	else{
		r.origin.x=0;
		r.origin.y=0;
		w=getc(f);
		w|=getc(f)<<8;
		if(w&0x8000){
			w&=~0x8000;
			r.corner.x=w;
			w=(w+SHORTSIZE-1)/SHORTSIZE;
		}
		else
			r.corner.x=SHORTSIZE*w;
	}
	bp=balloc(r);
	if(bp==0){
		fclose(f);
		return(0);
	}
	rectf(&bbuffer, bbuffer.rect, F_CLR);
	if(compl)
		rectf(compl, raddp(bp->rect, compl->rect.origin), F_XOR);
	for(i=r.origin.y;i!=r.corner.y;i++){
		if(getrast(f, (short *)raster, w)){
			bfree(bp);
			fclose(f);
			return(0);
		}
		for(j=0;j!=bp->width;j++)
			buffer[j]^=raster[j];
		bitblt(&bbuffer, bbuffer.rect, bp, Pt(r.origin.x, i), F_STORE);
		if(compl)
			rectf(compl, raddp(Rect(0, i, SHORTSIZE*w, i+1),
						compl->rect.origin), F_XOR);
	}
	fclose(f);
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
