#include	<jerq.h>
#include	<jerqio.h>
#include	<font.h>
#define ISIZE(n)	((n+1)*sizeof(Fontchar))

/*
 * read a font from an input stream
 * 	<font header>
 *	<f->info>
 *	<f->bits>	no bitmap header!!
 *
 * WARNING! This code believes it knows what the Font structure looks like
 */

static int (*inch)(), (*ouch)();

Font *
infont(incharg)
	int (*incharg)();
{
/*
 * Following line changed for the DMD!
 *
 */
	short n;
	register Font *f;
	register Bitmap *b;
	char *temp;

	inch = incharg;
	temp = (char *)&n;
	if(ninch(2, &temp))
		return((Font *)0);
	if((f = (Font *) alloc(sizeof(Font)+ISIZE(n-1))) == (Font *)0)
		return(f);
	f->n = n;
	temp = 2 + (char *)f;
	if(ninch(6, &temp))	/* 6 == sizeof(height+ascent+unused) */
		goto err;
	temp = (char *)f->info;
	if(ninch(ISIZE(n), &temp))
		goto err;
	if((b = balloc(Rect(0, 0, f->info[n].x, f->height))) == (Bitmap *)0)
		goto err;
	f->bits = b;
	if(ninch(sizeof(Word)*f->height*b->width, (char **)&(b->base)))
		goto berr;
	return(f);

	berr:
		bfree(f->bits);
	err:
		free(f);
	return((Font *)0);
}

static
ninch(n, base)
	register n;
	register char **base;
{
	register x, i;

	i = 0;
	do {
		x = (*inch)();
		(*base)[i++] = x;
		if(x == -1)
			return(1);
	} while (--n > 0);
	return(0);
}

ffree(f)
	register Font *f;
{
	if (f != (Font *) NULL) {
		bfree(f->bits);
		free(f);
	}
}

outfont(f,oucharg)
	register Font *f;
	int (*oucharg)();
{
	register Bitmap *b = f->bits;
	char *temp;

	ouch = oucharg;
	if(
		(temp = (char *)f, nouch(8, &temp)) ||
		(temp = (char *)f->info, nouch(ISIZE(f->n), &temp)) ||
		nouch(sizeof(Word)*f->height*b->width, (char **)&b->base)
	)
	{
		return(-1);
	}
	else
	{
		return(0);
	}
}

static
nouch(n,base)
	register n;
	register char **base;
{
	register i = 0;
	do {
		if((*ouch)((*base)[i++]) == -1)
			return(-1);
	} while (--n > 0);
	return(0);
}
