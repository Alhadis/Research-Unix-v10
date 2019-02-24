#include <jerq.h>
#include "rock.h"
extern Bitmap *buildimage();
extern notdrawn;
#define	NROCK	32	/* == number of bits in a long */
#define	V	16	/* velocity */
#define	DV	5	/* delta v after explosion */
#define	W	48	/* radius of rock (sort of) */
long allocated=0;
Rock rock[NROCK];
#define	BINWIDTH	64	/* number of bins across screen */
#define	HASHMASK	(YMAX/BINWIDTH-1)
#define	HASHSHIFT	4	/* log2(HASHMASK+1) */
long	bitarray[(YMAX/BINWIDTH)*(YMAX/BINWIDTH)];
char rock0[]={
	'R',	-50,	-50,	50,	50,
	'm', 11, -50,
	'l', 29, -30,
	'l', 24, -7,
	'l', 49, 20,
	'l', 29, 44,
	'l', -24, 44,
	'l', -48, 13,
	'l', -40, 0,
	'l', -50, -21,
	'l', -41, -36,
	'l', 11, -50,
	0
};
char rock1[]={
	'R', -30, -30, 32, 32,
	'm', 2, -30,
	'l', 30, -10,
	'l', 10, 30,
	'l', -10, 30,
	'l', -21, 9,
	'l', -10, -20,
	'l', 2, -30,
	0
};
char rock2[]={
	'R', -15, -15, 15, 15,
	'm', 4, -11,
	'l', 6, -4,
	'l', 15, 5,
	'l', 1, 13,
	'l', -15, -4,
	'l', -4, -11,
	'l', 4,	-11,
	0
};
char *rockim[]={
	rock0, rock1, rock2
};
#define	NROCKIMAGES	((sizeof rockim)/(sizeof (char *)))
Bitmap *rockseq[NROCKIMAGES];
initrock(n)
{
	register i;
	for(i=0; i<sizeof bitarray/sizeof(long); i++)
		bitarray[i]=0;
	allocated=0;
	for(i=0; i<NROCK; i++)
		rock[i].hash= -1;
	for(i=0; i<NROCKIMAGES; i++)
		rockseq[i]=buildimage(rockim[i]);
	for(i=0; i<n; i++)	/* start with 6 rocks */
		(void)spawn();
}
spawn()
{
	Point x, v;
	register i;
	if(rnd(100)>=50){
		x.x=0;
		x.y=rnd(YMAX);
	}else{
		x.x=rnd(XMAX);
		x.y=0;
	}
	do				/* 0 velocities are dull */
		v.x=V-rnd(2*V);
	while(v.x==0);
	do
		v.y=V-rnd(2*V);
	while(v.y==0);
	newrock(x, v, 0);
	return 1;
}
rockfree()
{
	register i;
	for(i=0; i<NROCKIMAGES; i++)
		bfree(rockseq[i]);
}
newrock(x, v, im)
	Point x, v;
	int im;
{
	register long i;
	register Rock *r;
	for(i=1; i && (allocated&i); i<<=1)
		;
	if(i==0 || im>=NROCKIMAGES)
		return;
	allocated |= i;
	r= &rock[bittoindex(i)];
	r->x=x;
	r->v=v;
	r->r=rockim[im][3];	/* It's true! */
	r->i=im;
	r->hash=hash(r->x);
	draw(r);
	addbits(i, r->hash);
}
draw(r)
	register Rock *r;
{
	register Bitmap *b=rockseq[r->i];
	bitblt(b, b->rect, &display, add(transform(r->x), b->rect.origin), F_XOR);
}
addbits(bits, h)
	register long bits;
	register h;
{
	register long *lp= &bitarray[h];
	register x=h&HASHMASK;
	register y=(h>>HASHSHIFT)&HASHMASK;
	int xlo=0, ylo=0, xhi=0, yhi=0;
	if(x==0)
		xlo=1;
	if(y==0)
		ylo=1;
	if(x==HASHMASK)
		xhi=1;
	if(y==HASHMASK)
		yhi=1;
	lp[0]|=bits;
	if(!xlo){
		lp[-1]|=bits;
		if(!ylo)
			lp[-YMAX/BINWIDTH-1]|=bits;
		if(!yhi)
			lp[YMAX/BINWIDTH-1]|=bits;
	}
	if(!ylo)
		lp[-YMAX/BINWIDTH]|=bits;
	if(!yhi)
		lp[YMAX/BINWIDTH]|=bits;
	if(!xhi){
		lp[1]|=bits;
		if(!ylo)
			lp[-YMAX/BINWIDTH+1]|=bits;
		if(!yhi)
			lp[YMAX/BINWIDTH+1]|=bits;
	}
}
subbits(bits, h)
	register long bits;
	register h;
{
	register long *lp= &bitarray[h];
	register x=h&HASHMASK;
	register y=(h>>HASHSHIFT)&HASHMASK;
	int xlo=0, ylo=0, xhi=0, yhi=0;
	if(x==0)
		xlo=1;
	if(y==0)
		ylo=1;
	if(x==HASHMASK)
		xhi=1;
	if(y==HASHMASK)
		yhi=1;
	bits= ~bits;
	lp[0]&=bits;
	if(!xlo){
		lp[-1]&=bits;
		if(!ylo)
			lp[-YMAX/BINWIDTH-1]&=bits;
		if(!yhi)
			lp[YMAX/BINWIDTH-1]&=bits;
	}
	if(!ylo)
		lp[-YMAX/BINWIDTH]&=bits;
	if(!yhi)
		lp[YMAX/BINWIDTH]&=bits;
	if(!xhi){
		lp[1]&=bits;
		if(!ylo)
			lp[-YMAX/BINWIDTH+1]&=bits;
		if(!yhi)
			lp[YMAX/BINWIDTH+1]&=bits;
	}
}
bittoindex(i)
	register long i;
{
	register j;
	register long b;
	for(j=0, b=1; (b&i)==0; b<<=1, j++)
		;
	return j;
}
hash(p)
	Point p;
{
	return (p.x/BINWIDTH)|(((unsigned)p.y/BINWIDTH)<<HASHSHIFT);
}
Rock *
collision(p, hashv, rad)
	Point p;
{
	register long bits, b;
	register i;
	bits=bitarray[hashv];
	for(i=0, b=1; i<NROCK; i++, b<<=1)
		if(bits&b){	/* There is a nearby rock */
			register Rock *r= &rock[i];
			Point pp;
			pp=r->x;
			pp.x-=p.x;
			pp.y-=p.y;
			if(pp.x*pp.x + pp.y*pp.y < 
				rad*rad + r->r*r->r)
				return &rock[i];
		}
	return 0;
}
moverock()
{
	static rocknum=0;
	register Rock *r= &rock[rocknum];
	register hsh=r->hash;
	if(hsh==-1){
		notdrawn++;
		goto Return;
	}
	draw(r);
	r->x=add(r->x, r->v);
	onscreen(&r->x);
	draw(r);
	r->hash=hash(r->x);
	if(r->hash != hsh){
		subbits(1L<<rocknum, hsh);
		addbits(1L<<rocknum, r->hash);
	}
	batcomp(r);
   Return:
	if(++rocknum >= NROCK)
		rocknum=0;
}
split(r)
	register Rock *r;
{
	register i=r->i+1;
	newrock(r->x, add(r->v, Pt(DV-rnd(2*DV), DV-rnd(2*DV))), i);
	newrock(r->x, add(r->v, Pt(DV-rnd(2*DV), DV-rnd(2*DV))), i);
}
char killvalue[]={
	1, 2, 5
};
killrock(r)
	register Rock *r;
{
	extern score;
	register long b=1L<<(r-rock);
	allocated&=~b;
	subbits(b, r->hash);
	r->hash= -1;
	score+=killvalue[r->i];
	draw(r);
}
