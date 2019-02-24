ulong floyd[FSIZE+1][FSIZE+1];
ulong contab[RES+1];
Bitmap *icon;

drawface(lo, hi, facep)
	Point facep;
{

	ulong *p, *f;
	char *sq;
	int v, h, s, e, t, i;
	if (!icon)
		icon=balloc(Rect(0, 0, FSIZE, FSIZE), 1);
	if (!icon)
		return;
	if (hi<lo){e=hi; hi=lo; lo=e;}
	if (lo<0) lo=0; else if (RES<lo) lo=RES;
	if (hi<0) hi=0; else if (RES<hi) hi=RES;
	t = hi-lo;
	p = contab;
	for (v = 0;v !=lo; v++)
		*p++ = 0;
	for (;v !=hi; v++)
		*p++ = (v-lo)*RES/t;
	for (;v <= RES;v++)
		*p++ = RES;
	sq=squash[0];
	for (v=0;v!=FSIZE;v++) for(h=0,f=floyd[v];h!=FSIZE;h++)
		*f++ = contab[*sq++&0xFF];
	for (v = 0;v != FSIZE; v++)
	{	f = floyd[v];
		for (h=0,p=addr(icon, Pt(0, v));h<FSIZE;h+=16,p++)
		{	*p=0;
			for(s=0;s!=16 && s+h!=FSIZE;s++,f++)
			{	e=f[0];
				e+=dithmat[s&7][v&7];
				i=e/72;
				if (i<0)
					i=0;
				else if (i>3)
					i=3;
				e-=(i*RES)/3;
				*p|=(3-i)<<(30-2*s);
#ifndef THRESH
				t=3*e/8;
				f[FSIZE+1]+=t;
				f[FSIZE+2]+=e-2*t;
				f[1]+=t;
#endif
			}
		}
	}
	bitblt(&display, facep, icon, icon->rect, S);
}

