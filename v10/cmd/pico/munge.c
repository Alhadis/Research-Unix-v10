#include <stdio.h>
#include <signal.h>
#include <fb.h>
#include "pico.h"

extern short	CURSCRATCH, CUROLD;
extern struct	SRC src[MANY];
extern long	program();
extern char	usednew, usedold, frameb, metheus, faster;
extern int	DEF_LL, DEF_NL;
extern void	onquit();

#define RGBA	0xe
#define RGB	0xd
#define A	0x1
#define R	0x2
#define G	0x4
#define B	0x8

struct {
	long u;
	long p;
	long chld_usrt;
	long chld_syst;
} tim0, tim1;

int sx, sy, ex, ey, delta, offset;

munge(chann)
{	int i;
	float t0, t1;

	sx = Old->sx;
	sy = Old->sy;
	ex = Old->ex; if ((ex-sx)&1) ex++;
	ey = Old->ey;
	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;

	checkit();
	times(&tim0);

	switch (chann) {
	case NLOOP: noloop();  break;
	case SLOOP: smalloop(); break;
	case BLOOP: if (faster && (metheus || Old->nchan == 1) && usednew)
		    {	if (frameb)
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

hack()
{	register x, y, i = offset;	/* r11, r10, r9 */
	register r;
	extern fbreg  *F;
	register fbreg *f = F;

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
	unsigned char *pr, *pg, *pb;
	int chunk;
	extern int om_fd;

	pr = Scratch->pixred+i;
	pg = Scratch->pixgrn+i;
	pb = Scratch->pixblu+i;
	chunk = ex-sx+delta;
	signal(SIGINT, SIG_IGN);	/* don't mess up the device */
	if (Old->nchan==1)
	{	merect(sx, sy, ex, ey);
		setbank(RGBA);
		for (y = sy; y < ey; y++, i += delta, pr += chunk)
		{	for (x = sx; x < ex; x++, i++)
			{
asm("				jsb	*_program	");
asm("	CD:			brb	CD+9		");
				program();
			}
			write(om_fd, pr, ex-sx);
		}
	}
	else
	for (y = sy; y < ey; y++, i += delta)
	{	for (x = sx; x < ex; x++, i++)
		{
asm("			jsb	*_program	");
asm("	CE:		brb	CE+9		");
			program();
		}
		merect(sx, y, ex, y+1);
		setbank(R); write(om_fd, pr, ex-sx); pr += chunk;
		setbank(G); write(om_fd, pg, ex-sx); pg += chunk;
		setbank(B); write(om_fd, pb, ex-sx); pb += chunk;
	}
	signal(SIGINT, onquit);
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

noloop()
{	register x, y, i;	/* r11, r10, r9 */

asm("		jsb	*_program			");
asm("	C2:	brb	C2+9				");	
		program();
}

