#include	<jerq.h>
#include	<font.h>
#define Edge	(defont.height)

struct DICT {
	struct {
		int freq;
		unsigned char val;
	} dict[256];
	int ndict;
} **Dict;
int nDict=0, nFrames=0;

unsigned char *which;
unsigned char **compact;
char unwound[2];

int Xsize = 256;
int Ysize = 256;
int Want = 256*256/8;

Rectangle Dor, Sor;
Bitmap *frame;
int Vis, maxframe=0;

main()
{	request(RCV|SEND|MOUSE|KBD);
	unwound[0] = unwound[1] = 0;
	Dor = inset(Drect, Edge);

	frame = balloc(Rect(0, 0, Xsize, Ysize));
	Sor = frame->rect;
	Vis = rectclip(&Sor, rsubp(Dor, Dor.origin));

	getdicts();
	run();
}

getframe()
{	register unsigned char *t;
	register int z, n, x;

	x = getn();		/* width   */
	z = getn();		/* height  */
	if ((x != Xsize) || (z != Ysize))
	{	Xsize = x; Ysize = z; Want = Xsize*(Ysize/8);
		frame = balloc(Rect(0, 0, Xsize, Ysize));
		Sor = frame->rect;
		Vis = rectclip(&Sor, rsubp(Dor, Dor.origin));
	}
	n = getn();		/* compacted size */

	if (!(compact[maxframe] = (unsigned char *) alloc(n*sizeof(unsigned char))))
	{	spr(1, "          out of memory");
		sendnchars(1, "0");
		return 0;
	}
	spr(1, "frame %   ", maxframe+1);
	sendnchars(1, "1");

	which[maxframe] = (unsigned char) rerecv();		/* dict# */
	for (z = 0, t = compact[maxframe]; z < n; z++)
		*t++ = (unsigned char) rerecv();

	return 1;
}

getdicts()
{	register int i, j, k, n;

	i = getn();
	which   = (unsigned char  *) alloc(i*sizeof(unsigned char));
	compact = (unsigned char **) alloc(i*sizeof(unsigned char *));
	n = rerecv();
	spr(1, "% frames % tables ", i, n);
	Dict = (struct DICT **) alloc(n*sizeof(struct DICT *));
	if (!which || !compact || !Dict) return 0;

	nDict = n;
	nFrames = i;
	for (i = 0; i < n; i++)
	{	Dict[i] = (struct DICT *) alloc(sizeof(struct DICT));
		if (!Dict[i])
			return 0;
		k = getn();
		Dict[i]->ndict = k;
		for (j = 0; j < k; j++)
		{	Dict[i]->dict[j].freq = getn();
			Dict[i]->dict[j].val = (unsigned char) rerecv();
			wait(CPU);
		}
	}
	spr(1, "                           ");
	return 1;
}

winder(m)
{	int n, k=0;

	unwound[m] = 1;
	for (n = m; n < maxframe; n += 2, k++)
	{	spr(1, "frame % ", n);
		if (!prewind(n)) break;
		Blast(n);
	}
	spr(1, "unwound % frames", k);
}

prewind(m)
{	unsigned char *tmp;

	if (which[m] >= nDict) return 1;	/* non-compacted frame */
	if (!(tmp = (unsigned char *) alloc(Want * sizeof(unsigned char))))
		return 0;

	unwind(m);
	free(compact[m]);
	compact[m] = tmp;
	which[m] = nDict+1;
	memcpy(tmp, frame->base, Want);
	return 1;
}

Blast(m)
{
	if (P->state & (RESHAPED|MOVED))
	{	P->state &= ~(RESHAPED|MOVED);
		Dor = inset(Drect, Edge);
		Sor = frame->rect;
		Vis = rectclip(&Sor, rsubp(Dor, Dor.origin));
	}
	if (!Vis) return;

	if (which[m] >= nDict)		/* non-compacted frame */
	{	Word *tmp;
		tmp = frame->base;
		frame->base = (Word *) compact[m];
		bitblt(frame, Sor, &display, Dor.origin, F_STORE);
		frame->base = tmp;
	} else
	{	unwind(m);
		bitblt(frame, Sor, &display, Dor.origin, F_STORE);
	}
}

unwind(m)
{	register unsigned char *t, *f;
	register int n;
	register unsigned char v;
	register int z;
	struct DICT *w = Dict[which[m]];

	t = compact[m];
	f = (unsigned char *)(frame->base);
	for (z = 0; z < Want; )
	{	for (n = 0; *t == 255; t++)
			n += 255;
		n += *t++;
		for (z += n; n > 0; n--)
			*f++ = *t++;

		if (z >= Want) break;

		n = w->dict[*t].freq;
		v = w->dict[*t].val;
		for (z += n; n > 0; n--)
			*f++ = v;
		t++;
	}
}

memcpy(to, from, n)
	register unsigned char *from, *to;
	register n;
{
	while (n--)
		*to++ = *from++;
}
