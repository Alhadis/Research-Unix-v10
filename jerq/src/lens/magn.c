
#include <jerq.h>

#define TABLESIZE	3
static char *(byn[TABLESIZE]);

/* A few words of strategy:  in the magnification routine, we will use
   table lookup to find the expansion of a byte into magsize bytes.  byn
   holds these tables.  For 2 <= m <= TABLESIZE, byn[m-2] points to
   a table containing the expansion for magnification m.  byn[TABLESIZE-1]
   is used to hold all larger expansion tables.  Currently, all of these
   tables are computed as needed, and the smaller ones, once
   initialized, are kept permanently.  If size were an issue, we could
   throw these away when not needed; if speed were a problem, we could
   generate more tables in advance, and keep more around.  What we do now
   seems to work well for our application. */

nullfunc() {
}

extern int magby2(), magby3(), magslow(), magbyeven(), magbyodd();
/* these are the functions which actually expand bitmaps */

static int (*(magby[]))() = {magby2,magby3};
/* for those sizes <= TABLESIZE, special magnification
   routines are expected.  we could put magbyeven and magbyodd here,
   but 2 and 3 have sufficiently nice properties that we can do better */

static int magsize = 1;
/* to keep track of the most recent x-magnification factor>TABLESIZE */

/* the client routine to set the current x-factor, and build the tables */
setmag(n) {
	if ((n <= 1) || (n == magsize))
		return 1;		/* no work necessary */
	if (n <= TABLESIZE) {
		if (byn[n-2])
			return 1;	/* we already did it */
		else {
			if (ngen(n,&byn[n-2]))
				return 1;
			freemag();	/* maybe this will get enough space */
			return ngen(n,&byn[n-2]); /* build a small table */
		}
	}

	/* build a large table */
	freemag();
	if (ngen(n,&byn[TABLESIZE-1])) {
		magsize = n;
		return 1;
	} else
		return 0;
}

/* dispose of the large magnification table */
freemag() {
	if (byn[TABLESIZE-1])
		gcfree(byn[TABLESIZE-1]);
	byn[TABLESIZE-1] = NULL;
	magsize = 1;
}

#define BYTESIZE 8		/* better be a power of 2 */
#define BYTEMASK (BYTESIZE-1)
#define BYTESHIFT 3		/* log2(BYTESIZE) */
#define NUMCHARS 256		/* 2**BYTESIZE */

/* allocate space for, and build, magnification table for x-factor n,
   leaving the result pointed to by byn.  Fails if there isn't enough
   memory by returning 0. */
ngen(n,byn)
char **byn;
{
	register int i,j,k,acc;
	register char *bits;

	if ((*byn == NULL) &&
	   ((*byn = gcalloc((unsigned long)NUMCHARS*n, byn)) == NULL))
		return 0;
	bits = *byn;
	for (i = 0; i<NUMCHARS; i++) /* for each possible byte... */
		/* and for each byte in the expansion... */
		for (j = BYTESIZE*(n-1); j >= 0; j -= BYTESIZE) {
			acc = 0;
			for (k = 0; k < BYTESIZE; k++) /* and for each bit... */
				/* if it corresponds to a bit which is on, */
				if (i & (1<<((k+j)/n)))
					acc |= (1<<k); /* turn on the bit */
			*bits++ = acc;	/* and store the byte away */
		}
	return 1;
}

/*
 * Like bitblt, only magnifies.  fac = (xscale, yscale), expansion factors.
 * F_STORE into tb only.  Assumptions: b != tb != db; tb offscreen and trashable;
 * db is the ultimate destination (0 if none desired) bitmap, and dp is the
 * destination point in that bitmap; the ultimate bitblt is done in mode mode.  If
 * db is 0, the magnified image is left in tb, in a rectangle based at (0,0).
 * magn does the last bitblt for you so that it can freely 'reshape' the
 * bitmap, and noone will notice.  If tb is exactly the right size, this is
 * not important, so feel free to set db = (Bitmap *)0.  tb->rect.origin MUST
 * equal (0,0).  If xscale is not equal to the last argument to setmag, the
 * expansion will be ungodly slow, unless it is 1, 2, or 3.  tb must contain
 * as many words as r*fac.x*fac.y.
 */

/* magnify the rectangle r in bitmap b by fac.  Bitblt the result to point dp
   in bitmap db by mode mode.  Use tb as temporary storage. */
magn(b, r, tb, db, dp, fac, mode)
	register Bitmap *b, *tb, *db;
	Rectangle r;
	Point dp, fac;
	int mode;
{
	Bitmap temp;
	register i, shift;
	unsigned char *from;
	char *to;
	int tjump, fjump;
	Point d,q,o;
	int w,wid,goodbytes;
	int (*func)();
	char *bits;

/* multiplying by zero is very easy */
	if(fac.x<1 || fac.y<1) 
		return;

/* multiplying by one is almost as easy */
	if(fac.x==1 && fac.y==1) {
		d.x=d.y=0;
		bitblt(b, r, db?db:tb, db?dp:d, mode);
		return;
	}

/* compute the diagonal */
	d=sub(q=r.corner, o=r.origin);
/* multiplying an empty rectangle is also easy */
	if ((d.x <= 0) || (d.y <= 0))
		return;

	temp = *tb;	/* save the parameters of tb */
	/* reshape tb to fit */
	if (db) /* if we have a destination, we can really trash it up */
		tb->width = (d.x * fac.x + WORDMASK) / WORDSIZE;
	w = tb->width * WORDSIZE; 	/* width in bits of temp */
	tb->width *= fac.y;		/* reshape bitmap... */
	tb->rect.corner.x = tb->width * WORDSIZE;
	tb->rect.corner.y = d.y;	/* and the rectangle */
	wid = (d.x + BYTEMASK) / BYTESIZE;	/* width in bytes of source */

	/* 1: move into place.  Vertical expansion happens as
	 *    consequence of bitmap reshaping.  If the source
	 *    rectangle is byte-aligned, just take it from where
	 *    it is.  In either case, from should point to the byte
	 *    after the last byte on the first line of where the bits are,
	 *    and fjump should be the number of bytes needed to go
	 *    from the first byte on a line to the last byte on the
	 *    next line plus one.
	 */
	if ((fac.x == 1) || (o.x & BYTEMASK) || (b->_null)) {
		bitblt(b,r,tb,Pt(0,0),F_STORE);		/* move bits */
		from = ((unsigned char *)(tb->base)) + wid;
		fjump = tb->width * sizeof(Word) + wid;
	} else {
		from = ((unsigned char *)addr(b,o)) +
			((o.x & WORDMASK) >> BYTESHIFT) + wid;
		fjump = b->width * sizeof(Word) + wid;
	}

	/* 2: magnify horizontally, via table lookup.  'to' points to the
	 *    byte after the last byte on the first line of the enlarged 
	 *    image, and tjump is the increment to get us from the first 
	 *    byte on a line to the last byte on the next line plus one.
	 *    Since the width of the source needn't be an integral number
	 *    of bytes, goodbytes is used to determine how many of the
	 *    bytes in the last block of fac.x bytes are valid.
	 */
	to = ((char *)tb->base) + wid * fac.x;
	tjump = tb->width * sizeof(Word) + wid * fac.x;
	goodbytes = (i = d.x & BYTESIZE) ? ((i * fac.x + BYTEMASK) / BYTESIZE)
					 : fac.x;
	/* pick the appropriate magnification function, and set bits to
	 * point to the expanded bytes
	 */
	bits = NULL;
	if (fac.x == 1)
		func = nullfunc;
	else if ((fac.x <= TABLESIZE) && (bits = byn[i = fac.x-2]))
		func = magby[i];
	else if ((fac.x == magsize) && (bits = byn[TABLESIZE-1])) {
		if (fac.x & 1)
			func = magbyodd;
		else
			func = magbyeven;
	} else
		func = magslow;

	/* call the magnifying routine */
	(*func)(from, to, fjump, tjump, wid, d.y, bits, goodbytes, fac.x);

	/* 3: smear 'vertically', in the reshaped bitmap, using logarithmic
	 *    number of steps
	 */
	for(i=1; i<fac.y; i<<=1){
		shift=min(i, fac.y-i)*w;	/* don't shift too much */
		bitblt(tb, Rect(0, 0, shift, d.y),
		       tb, Pt(i*w, 0), F_STORE);
	}

	/* 4: unreshape tb, and copy the result where it belongs */
	if (db) {
		tb->rect.corner.y *= fac.y;
		tb->rect.corner.x = d.x * fac.x;
		tb->width /= fac.y;
		bitblt(tb, tb->rect, db, dp, mode);
	}  
	*tb = temp;	
}
