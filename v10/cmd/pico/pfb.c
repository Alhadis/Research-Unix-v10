#include <stdio.h>
#include <fb.h>
#include "pico.h"

extern short	CURSCRATCH, CUROLD;
extern struct	SRC src[MANY];
extern char	frameb, metheus;
extern int	DEF_LL, DEF_NL;

fbreg  G;	/* place holder (68 bytes) */
fbreg *F = &G;	/* initial value */
int delay=0;

#define Delay	delay+=2

fbsnore()
{	int it;
	if (!frameb) return;
	if((it=open("/dev/iti0", 2))<0 || ioctl(it, ITIADDR, &fb.addr)<0)
	{	fprintf(stderr, "pico: cannot open framebuffer\n");
		frameb = 0;
	} else
	{	fb.fd = it;
		F=fb.addr;
	}
}

closefb()
{	close(fb.fd);
}

fbputitall(k)
{	register fbreg *f=F;
	register unsigned char *pr, *pg, *pb;
	struct SRC *from = &src[k];
	register int j, ex = from->sx + from->ex;
	int i, offset, delta, sx = from->sx;
	int sy = from->sy, ey = from->sy + from->ey;

	if (!frameb) return;

	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;

	pr = from->pixred+offset;
	pg = from->pixgrn+offset;
	pb = from->pixblu+offset;

	f->red.csr=f->grn.csr=f->blu.csr=F_IXW;

	switch (from->nchan)
	{	case 1:
			for (i = sy; i < ey; i++, pr+=delta)
			{	f->red.x=f->grn.x=f->blu.x=sx;
				f->red.y=f->grn.y=f->blu.y=i;
				for (j = sx; j < ex; j++)
				{	f->red.z=f->grn.z=f->blu.z= (*pr++);
					Delay;
				}
			}
			break;
		case 4:
		case 3:
			for (i = sy; i < ey; i++, pr+=delta, pg+=delta, pb+=delta)
			{	f->red.x=f->grn.x=f->blu.x=sx;
				f->red.y=f->grn.y=f->blu.y=i;
				for (j = sx; j < ex; j++)
				{	f->red.z=*pr++; Delay;
					f->grn.z=*pg++; Delay;
					f->blu.z=*pb++; Delay;
			}	}
			break;
		default:
			fprintf(stderr, "undefined number of channels\n");
			break;
		}
}

fbgetitall(from)
	struct SRC *from;
{
	register fbreg *f=F;
	register unsigned char *pr, *pg, *pb;
	register int j, ex = from->sx + from->ex;
	int i, offset, delta, sx = from->sx;
	int sy = from->sy, ey = from->sy + from->ey;

	if (!frameb) yyerror("framebuffer is not open");

	if (from == Old)
		fprintf(stderr, "reading screen...");

	offset = sy*DEF_LL + sx;
	delta  = DEF_LL - ex + sx;

	pr = from->pixred+offset;
	pg = from->pixgrn+offset;
	pb = from->pixblu+offset;

	f->red.csr=f->blu.csr=f->grn.csr=F_IXR;

	switch (from->nchan)
	{	case 1:
			for (i = sy; i < ey; i++, pr+=delta, pg+=delta, pb+=delta)
			{	f->red.x=f->grn.x=f->blu.x=sx;
				f->red.y=f->grn.y=f->blu.y=i;
				for (j = sx; j < ex; j++)
				{	*pg = *pb = *pr = f->grn.z; Delay;
					pg++; pb++; pr++;
				}
			}
			break;
		case 4:
		case 3:
			for (i = sy; i < ey; i++, pr+=delta, pg+=delta, pb+=delta)
			{	f->red.x=f->grn.x=f->blu.x=sx;
				f->red.y=f->grn.y=f->blu.y=i;
				for (j = sx; j < ex; j++)
				{	*pr++ = f->red.z; Delay;
					*pg++ = f->grn.z; Delay;
					*pb++ = f->blu.z; Delay;
			}	}
			break;
		default:
			fprintf(stderr, "undefined number of channels\n");
			break;
	}
	RESIDENT;
}

fbup()  
{	if (!frameb) yyerror("up: no access to framebuffer");
	F->red.scroll = F->grn.scroll = F->blu.scroll = 32;
}

fbdown()
{	if (!frameb) yyerror("down: no access to framebuffer");
	F->red.scroll = F->grn.scroll = F->blu.scroll =  0;
}

fblyank(x, y)
{	register fbreg *f=F;
	register int i;
	register unsigned char *pr, *pg, *pb;
	extern char faster;

	if (!faster)
	{	fbyank(x, y);
		return;
	} else
	{	if (x != 0)
			return;
	}
	i = (y < 1 || y >= DEF_LL) ? 0 : DEF_LL*(y-1);
	pr = scratchred+i;
	pg = scratchgrn+i;
	pb = scratchblu+i;

	f->red.y=f->grn.y=f->blu.y=y-1; Delay;
	for (i = 0; i < DEF_LL; i++)
	{	f->red.x=f->grn.x=f->blu.x=i; Delay;
		f->red.z = *pr++; Delay;
		f->grn.z = *pg++; Delay;
		f->blu.z = *pb++; Delay;
	}
	return 1;
}

fbyank(x, y)
{	register fbreg *f=F;
	register int i = DEF_LL*y+x;
	unsigned char *pr = scratchred+i;
	unsigned char *pg = scratchgrn+i;
	unsigned char *pb = scratchblu+i;

	f->red.x=f->grn.x=f->blu.x=x; Delay;
	f->red.y=f->grn.y=f->blu.y=y; Delay;

	f->red.z = *pr; Delay;
	f->grn.z = *pg; Delay;
	f->blu.z = *pb; Delay;

	return *pr;
}

redcmap(i, z)
{	if (frameb) fbredcmap(i, z);
	else if (metheus) metredcmap(i, z);
}

grncmap(i, z)
{	if (frameb) fbgrncmap(i, z);
	else if (metheus) metgrncmap(i, z);
}

blucmap(i, z)
{	if (frameb) fbblucmap(i, z);
	else if (metheus) metblucmap(i, z);
}

getcmap(i, r,g,b)
{
	if (frameb) return fbgetcmap(i, r,g,b);
	else if (metheus) return metgetcmap(i, r,g,b);
	return 0;
}

fbredcmap(i, z)
{	F->map.addr = i%256;
	F->map.sele =  0;
	F->map.data = z;
	return z;
}
fbgrncmap(i, z)
{	F->map.addr = i%256;
	F->map.sele = 16;
	F->map.data = z;
	return z;
}
fbblucmap(i, z)
{	F->map.addr = i%256;
	F->map.sele = 32;
	F->map.data = z;
	return z;
}

fbgetcmap(i, r,g,b)
{	fbreg *f=F;
	int h = 0, k = 0;

	f->map.addr = i%256;
	if (r) { h++; f->map.sele =  0; k += f->map.data; }
	if (g) { h++; f->map.sele = 16; k += f->map.data; }
	if (b) { h++; f->map.sele = 32; k += f->map.data; }
	if (h > 1)
		k /= h;
	return k;
}
