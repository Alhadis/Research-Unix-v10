/*	locate object pointed by mouse		*/
/*		Own alloc/free				*/
/*	misc. blit I/O (kbdstr, jtag)			*/
/*	blitlt						*/

#include "jped.h"

pointP lockpnt;	/* point found by locate routine */

objP s_locate(b,N,cp) objP b[]; short N; Point cp;
{
	register i, j;
	register objP bb;
	register pointP tpp;
	register objP  tobj;
	for(j=0;j<N;j++){
		bb = b[j];
		if(!bb) continue;
		if(!bb->status) continue;
		if(bb->type=='f') {
			tobj = s_locate(bb->mem,bb->size,cp);
			if(tobj) {
				bb->status = 11;
				return(bb);
			}
		}
		else {
			for(tpp=bb->p0, i= 0; i<bb->n; i++, tpp++){
				if(abs(cp.x-tpp->x)<NR_TAR &&
				abs(cp.y-tpp->y)<NR_TAR) {
					lockpnt = tpp;
					bb->status = 11;
					return(bb);
				}
			}
		}
	}
	return((objP)0);
}

/*	6K seems to be enough for anything that can be worked on	*/

#define WSIZE 15360

#ifdef BLIT
#define WSIZE 6144
#endif

/*	not worth trying to draw on less than that	*/

#define QSIZE 1024

#define FSPACE 8192

static real_size;

char * wspace;
char * fsp;
char * fallout;
short room;

short inalloc(){	/* allocate space for objects */
	char *slack;
	slack = alloc((unsigned)QSIZE);
	for(real_size = WSIZE; real_size>QSIZE; real_size -= QSIZE){
		wspace = (char *)alloc((unsigned)real_size);
		if(!wspace) continue;
		free(wspace);
		free(slack);
		wspace = (char *)alloc((unsigned)real_size);
		fsp = wspace;
		fallout = wspace + real_size - 1;
		room = real_size - 1;
		return(real_size);
	}
	return(0);
}

char * Alloc(N) unsigned N;
{
	register char *p, *q;

	while(N%4) N++;
	p = fsp + N;
	if(p>fallout) {
		jtag(1,"NO MEMORY. Save your file and then do: e file");
		return((char *)0);
	}
	q = fsp;
	fsp = p;
	room = fallout - fsp;
	return(q);
}

char * Free(how)
{
	if(how) return((char *)0);
	fsp = wspace;
	room = fallout - fsp;
	return((char *)0);
}

short exalloc(){
	freefonts();
	free(wspace);
}

/*	replaces getrect() in comm,create,main,menu,mke,sub	*/

short prec(a,b)
	Point a,b;
{
	pline(a,Pt(b.x,a.y));
	pline(Pt(b.x,a.y),b);
	pline(b,Pt(a.x,b.y));
	pline(Pt(a.x,b.y),a);
}

short ownrec(r)
	Rectangle *r;
{
	register fail;
	Point u, ou;
	short dx, dy;
	Texture * oldcur;
	Code old_dr_mode;

	oldcur = cursswitch(&nsquare);
	old_dr_mode = dr_mode;
	dr_mode = F_XOR;

	for(;;nap(2),wait(MOUSE)){
		r->origin = mouse.xy;
		if(button23()) {
			fail = -1;
			goto relax;
		}
		if(button1()) break;
	}
	release1();
	ou = r->origin;
	for(;;nap(2),wait(MOUSE)){
		u = mouse.xy;
		if(button123()) break;
		prec(r->origin,ou);
		prec(r->origin,u);
		ou = u;
	}
	prec(r->origin,ou);
	r->corner = u;
	fail = 0;
	dx = (r->corner).x - (r->origin).x;
	dy = (r->corner).y - (r->origin).y;
	if(dx > 10) {
		if(dy > 10) goto relax;
		if(dy > -10) {	/* too small*/
			fail = -1;
			goto relax;
		}
		u.x = (r->origin).x;
		u.y = (r->corner).y;
		ou.x = (r->corner).x;
		ou.y = (r->origin).y;
		r->origin = u;
		r->corner = ou;
	}
	else if(dx< -10) {
		if(dy < -10) {
			r->corner = r->origin;
			r->origin = u;
			goto relax;
		}
		if(dy < 10) {	/* too small*/
			fail = -1;
			goto relax;
		}
		u.x = (r->corner).x;
		u.y = (r->origin).y;
		ou.x = (r->origin).x;
		ou.y = (r->corner).y;
		r->origin = u;
		r->corner = ou;
	}
	else fail = -1;	/* -10 < dx < 10 */

	relax: while(button123()) wait(MOUSE);
	cursswitch(oldcur);
	dr_mode = old_dr_mode;
	return(fail);
}

/*	Set up bitmaps	*/

#define BNULL (Bitmap *)0
short bconstruct()
{
	register i;
	short rad;

	poscross = balloc(Rect(0,0,16,16));
	if(poscross==BNULL) return(1);
	texture(poscross,poscross->rect,&pcross,F_STORE);
	negsq = balloc(Rect(0,0,16,16));
	if(negsq==BNULL) return(1);
	texture(negsq,negsq->rect,&nsquare,F_STORE);

	ball = balloc(Rect(0,0,17,17));
	if(ball==BNULL) return(1);
	rectf(ball,ball->rect,F_CLR);
	disc(ball,add(ball->rect.origin,Pt(8,8)),3,F_STORE);

	vball[0] = balloc(Rect(0,0,2,2));
	if(vball[0]==BNULL) return(1);
	rectf(vball[0],(vball[0])->rect,F_STORE);

	for(i=1, rad=1; i<8; i++, rad++){
		vball[i] = balloc(Rect(0,0,2*rad+1,2*rad+1));
		if(vball[i]==BNULL) return(1);
		rectf(vball[i],(vball[i])->rect,F_CLR);
		disc(vball[i],add((vball[i])->rect.origin,Pt(rad,rad)),rad,F_STORE);
	}
	return(0);
}


/* get string from keyboard */

short kbdstr(s,p,klean)	char *s; Point p;
{
	char c, *sP;
	register kount;
	static char str[]="x";
	static char crs[]="_";

	/* flush junk */ if(klean) while(kbdchar() != -1);
	sP = s;
	kount = 0;
	for (;;) {
		string(&defont,crs,&display,p,dr_mode);
		wait(KBD);
		string(&defont,crs,&display,p,dr_mode);
		if (((c=kbdchar()) == '\r') || (c == '\n')) {
			*s = '\0';
			return(kount);
		}
		if (c == '\b') {
			kount--;
			if (s>sP) {
				str[0] = *(--s);
				string(&defont,str,&display,
					(p = sub(p,Pt(9,0))),dr_mode);
			}
		}
		else {
			if(c>=' ' && c< 0177) {
				kount++;
				if (s-sP<50) {
					*s++ = (str[0] = c);
					p = string(&defont,str,&display,p,dr_mode);
				}
			}
		}
	}
}

short jtag(replc,a)
	char *a;
{
	switch(replc) {
	case 1:
		rectf(&display,bulletin,F_STORE);
		placelab = add(bulletin.origin,Pt(5,5));
		break;
	case 2:
		rectf(&display,
			Rpt(sub(bulletin.corner,
			Pt(strwidth(&defont,a),(defont.height+2))),
			bulletin.corner),F_STORE);
		placelab
		= sub(bulletin.corner,Pt(strwidth(&defont,a),(defont.height+2)));
		break;
	case 0:
	default:
		placelab.x += strwidth(&defont,"a");
	}
	pstring(placelab,a);
	placelab.x += strwidth(&defont,a);
}

/*
	since alloc/free do no garbage collection we use
	our own Alloc and a drastic measure for free.
	Free all space at purge times
*/

short purge(keepgrid)
{
	/*	now we do something drastic	*/
	Free(0);
	no_obj = 0;
	rectf(&display,board,F_CLR);
	if(!keepgrid) grsize = 0;
	return(-1);
}


/*XMAX, WORDSIZE are defined in blit.h*/

#define SHORT		short
#define SHORTSIZE	16
#define SCAST(W)	((SHORT *)W)

static Word raster[XMAX/WORDSIZE];

static Bitmap braster={ raster, sizeof raster, 0, 0, XMAX, 1};

static short ctype, count; static SHORT *p1, *endraster;

short blitblt(showcase)
	Rectangle showcase;
{
	short nrasters, rastwid; Rectangle rrast;

	cursswitch(&clock);

	nrasters = showcase.corner.y-showcase.origin.y;
	rastwid  =(showcase.corner.x-showcase.origin.x+15)/16;
	endraster= SCAST(raster) + rastwid-1;
	sendstring("BB");
	s_short(nrasters);	s_short(rastwid);

	rectf(&braster,braster.rect,F_CLR);
	rrast = showcase;
	rectf(&display,showcase,dr_mode);

	for (; rrast.origin.y<showcase.corner.y; rrast.origin.y++) {
		rrast.corner.y = rrast.origin.y+1;
		rectf(&display,rrast,dr_mode);
		bitblt(&display,rrast,&braster,Pt(0,0),dr_mode);
		writerast();
		bitblt(&display,rrast,&braster,Pt(0,0),F_STORE);
	}

	s_flush();

	cursswitch(&livemouse);
	return(-1);
}

static writerast()
{
	SHORT *p2;

	p1 = p2 = SCAST(raster);
	do {
		if (p1 >= p2) {
			p2=p1+1; count=2;
			ctype=(*p1 == *p2);

		} else if ((*p2 == *(p2+1)) == ctype) {
			if (++count >= 127) {
				writewords();
				p1=p2+2;
			} else p2++;

		} else if (ctype) {
			writewords();
			p1=p2+1;
			ctype=0;

		} else {
			count--; writewords();
			p1=p2;
			ctype=1;
		}
	} while (p2<endraster);

	if (p1 > endraster) return;
	if (p2 > endraster) count--;
	writewords();
}

static writewords()
{
	short c;
	register j;
	register char * pp;
	c=count; if (ctype) { c += 128; count=1; }
	s_char(c);
	for(j=0,pp=(char *)p1;j<2*count;j++,pp++) s_char(*pp);
}
