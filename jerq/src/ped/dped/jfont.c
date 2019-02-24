/*	get font from host	*/

#include "jped.h"

#define ISIZE(n)	((n+1)*sizeof(Fontchar))
#ifdef	MPX
#undef	alloc
#define		alloc(u)	Tpchar(4)(u)
#endif	MPX

/*
 * read a font from an input stream
 * 	<font header>
 *	<f->info>
 *	<f->bits>	no bitmap header!!
 *
 * WARNING! This code believes it knows what the Font structure looks like
 */

/*	read n chars from host and place in base. if not enough		*/
/*	(i.e. flen<0), then return 1.					*/
short flen;

short fsenior = 0;
char forder[MXFONT] = { 0,0,0,0,0};


static
short ninch(n, base)
	register n;
	register char **base;
{
	register i;

	i = 0;
	do {
		if(--flen>=0) (*base)[i++] = g_char();
		else return(1);
	} while (--n > 0);
	return(0);
}


Font * infont()
{
	short n;
	register Font *f;
	register Bitmap *b;
	char *temp;

	temp = (char *)&n;
	if(ninch(2, &temp)) goto fabort; /* too cautious: return would suffice*/
	if((f = (Font *) alloc(sizeof(Font)+ISIZE(n))) == (Font *)0)
		goto fabort;
	f->n = n-1;
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
	fabort:
		while(--flen>=0) g_char();
		return(&defont);
}
short ffree(f)
	register Font *f;
{
	if (f != (Font *) NULL) {
		bfree(f->bits);
		free(f);
	}
}

short loadfont()
{
	Font  *infont();
	register n;
	Texture * oldcur;
	oldcur = cursswitch(&wave1);
	n = g_short();
	if(n<0 || n>4) {
		jtag(0,garbage);
		n = 0;
	}
	if(fontrcv[n]) jtag(0," mult. font");
	flen = g_short();
	jtag(2,"Loading font ");
	if( (fontrcv[n] = infont()) != &defont )  forder[fsenior++] = n;
	jtag(2,"                    ");
	cursswitch(oldcur);
	return(n);
}
short freefonts() {
	register j;
	for(j = fsenior-1; j>=0; j--) {
		bfree(fontrcv[forder[j]]->bits);
		free(fontrcv[forder[j]]);
	}
}
