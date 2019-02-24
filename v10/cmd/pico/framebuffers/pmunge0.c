#include <stdio.h>
#include <fb.h>
#include "pico.h"

#define lowbyte(A) (A)&0377
#define higbyte(A) ((A) >> 8) & 07

extern short	CURSCRATCH, CUROLD;
extern struct	SRC src[MANY];
extern long	program();
extern char	usednew, usedold, ramtek, frameb, metheus, faster;
extern int	DEF_LL, DEF_NL;

struct {
	long u;
	long p;
	long chld_usrt;
	long chld_syst;
} tim0, tim1;

int sx, sy, ex, ey, delta, offset;
int lastx, lasty, lastc;

munge(chann)
{	int i;
	float t0, t1;

	sx = Old->sx;
	sy = Old->sy;
	ex = Old->ex;
	ey = Old->ey;
	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;

	checkit();
/*	if (usednew) { setscratch(Old, Scratch); }	*/
	times(&tim0);

	switch (chann) {
	case NLOOP: noloop();  break;
	case SLOOP: smalloop(); break;
	case XLOOP: oneloop(); break;
	case BLOOP: if (faster && (metheus || Old->nchan == 1) && usednew)
		    {	if (ramtek)
			{	hock();
				break;
		    	} else if (frameb)
			{	hack();
				break;
		    	} else if (metheus)
			{	hick();
				break;
			}
		    }
		    bigloop();
		    break;
	}
	times(&tim1);
	t0 = (float) (tim1.u - tim0.u);
	t1 = (float) (tim1.p - tim0.p);

	fprintf(stderr, "time: %4.2fu  + %4.2fs = %4.2f\n",
		t0/60.0, t1/60.0, (t1+t0)/60.0);
	if (usednew) { setscratch(Scratch, Old); RESIDENT; }
}

hock()
{	register x, y, i = offset;	/* r11, r10, r9 */
	register short *w;
	short W[1024+16];		/* enough room for largest line */
	short dW = 2*(7+ex-sx);
	extern int Rfd;

	W[0] = 05003; W[1] = 000100; W[2] = sx;
	W[4] = ex-1;  W[6] = 2*(ex-sx);

	for (y = sy; y < ey; y++, i += delta)
	{	W[3] = y; W[5] = y+1; w = &W[7];
		for (x = sx; x < ex; x++, i++)
		{
asm("			jsb	*_program	");
asm("	CB:		brb	CB+9		");
			*w++ = program();
		}
		write(Rfd, W, dW);
	}
}

hack()
{	register x, y, i = offset;	/* r11, r10, r9 */
	register r;

	extern fbreg  *F;	/* place holder, only 68 bytes */
	register fbreg *f = F;		/* initial value */

	f->red.csr=f->grn.csr=f->blu.csr=F_IXW;
	for (y = sy; y < ey; y++, i += delta)
	{	f->red.x=f->grn.x=f->blu.x=sx;
		f->red.y=f->grn.y=f->blu.y=y;
		for (x = sx; x < ex; x++, i++)
		{
asm("			jsb	*_program	");
asm("	CA:		brb	CA+9		");
			r=program();
			f->red.z=f->grn.z=f->blu.z=r&255;
		}
	}
}

hick()
{	register x, y, i = offset;	/* r11, r10, r9 */
	register unsigned char *w;
	unsigned char mcode[16];
	unsigned char W[1024];	/* avoid allocation */
	unsigned char *pr, *pg, *pb;
	extern int om_fd;

	pr = Scratch->pixred+i;
	pg = Scratch->pixgrn+i;
	pb = Scratch->pixblu+i;

	mcode[0] = 0x4F;
	mcode[1] = 0377;	/* WRMASK */
	mcode[2]  = 0x52;	/* set P1 */
	mcode[7]  = 0x53;	/* set P2 */
	mcode[12] = 0;
	mcode[13] = 'o';
	mcode[3]  = lowbyte(sx);
	mcode[4]  = higbyte(sx);
	mcode[8]  = lowbyte(ex);
	mcode[9]  = higbyte(ex);
if (Old->nchan==1)
	for (y = sy; y < ey; y++, i += delta)
	{	mcode[5]  = mcode[10] = lowbyte(y);
		mcode[6]  = mcode[11] = higbyte(y);
		for (x = sx, w = W; x < ex; x++, i++)
		{
asm("			jsb	*_program	");
asm("	CD:		brb	CD+9		");
			*w++ = program();
		}
		write(om_fd, mcode, 14);
		write(om_fd, W, ex-sx);
	}
else
	for (y = sy; y < ey; y++, i += delta)
	{	mcode[5]  = mcode[10] = lowbyte(y);
		mcode[6]  = mcode[11] = higbyte(y);
		for (x = sx; x < ex; x++, i++)
		{
asm("			jsb	*_program	");
asm("	CE:		brb	CE+9		");
			program();
		}
		for (x = sx, w = W; x < ex; x++)
			*w++ =  ((*pr++ & 224))|
				((*pg++ & 192)>>3)|
				((*pb++ & 224)>>5);
		write(om_fd, mcode, 14);
		write(om_fd, W, ex-sx);
		pr += delta; pg += delta; pb += delta;
	}
}

bigloop()
{	register x, y, i = offset;	/* r11, r10, r9 */

	for (y = sy; y < ey; y++, i += delta)
	for (x = sx; x < ex; x++, i++)
	{
asm("		jsb	*_program	");
asm("	C0:	brb	C0+9		");
		program();
	}
}

smalloop()
{	register x, y, i;	/* r11, r10, r9 */

	for (i = 0; i < 256; i++)
	{
asm("		jsb	*_program	");
asm("	C1:	brb	C1+9		");
		program();
	}
}

oneloop()
{	register x, y, i;	/* r11, r10, r9 */

	for (x = 0; x < DEF_LL; x++)
	{
asm("		jsb	*_program	");
asm("	C2:	brb	C2+9		");
		program();
	}
}

noloop()
{	register x, y, i;	/* r11, r10, r9 */

asm("		jsb	*_program			");
asm("	C3:	brb	C3+9				");	
		program();
}

