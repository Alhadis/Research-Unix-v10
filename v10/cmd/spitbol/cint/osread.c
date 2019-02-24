
/*
 *	osread( mode,recsiz,ioptr,scptr ) reads the next record from
 *	the designated input file into the scblk. recsiz determines
 *	whether the read should be line or raw mode. line mode records
 *	are teminated with a new-line character (the new-line is not
 *	put in the scblk though). raw records are simply the next recsiz
 *	characters.
 */

#include "spitblks.h"
#include "spitio.h"

int	osread( mode, recsiz, ioptr, scptr )

int	mode;
int	recsiz;
struct	ioblk	*ioptr;
struct	scblk	*scptr;

{
	register struct bfblk	*bfptr = ioptr -> buf;
	register char	*cp = scptr -> str;
	register char *bp;
	register int	cnt = 0;
	int	fdn = ioptr -> fdn;
	register int	n;

	/*
	 *	Handle unbuffered reads.
	 */

	if ( ioptr -> flg & IO_WRC ) {

		/*	no line mode with unbuffered	*/
		if ( mode > 0 )
			return	-2;

		/*	attempt to read in scblk	*/
		n	= read( fdn,cp,recsiz );

		/*	if read error then take action	*/
		if ( n < 0 )
			return	-2;

		/*	check for eof			*/
		if ( n == 0 )
			return	-1;

		/*	everything ok, so return	*/
		return	n;
	}

	/*
	 *	Handle buffered reads.
	 */

	if ( mode > 0 ) {

		/* line mode */
		register char	*savecp;
		char	savechar;

		/*
		 *	First phase:  copy characters to the result
		 *	buffer either until recsiz is exhausted or
		 *	we have copied the last character of a line.
		 *	This loop is speeded up by pretending that
		 *	the input line is no longer than the result.
		 */

		do {
			char	*oldbp;

			/* if the buffer is empty, try to fill it */
			if ( bfptr -> rem == 0 ) {
				n = read ( fdn, bfptr -> buf, bfptr -> siz );

				/* eof is only ok at the beginning of a line */
				if ( n == 0 )
					return cnt > 0? -2: -1;

				/* input errors are never ok */
				if ( n < 0 )
					return -2;

				bfptr -> off = 0;
				bfptr -> rem = n;
			}

			/* set n to max # chars we can process this time */
			n = recsiz - cnt;
			if ( n > bfptr -> rem )
				n = bfptr -> rem;

			/* point bp and oldbp at the first char to be copied */
			oldbp = bp = bfptr -> buf + bfptr -> off;

			/* plant a newline at the end of the valid input */
			savecp = bp + n;
			savechar = *savecp;
                        *savecp = '\n';

			/* copy characters until we hit a newline */
                        while ( *bp != '\n' )
				*cp++ = *bp++;

			/* restore the stolen character */
			*savecp = savechar;

			/* calculate how many characters were moved */
			n = bp - oldbp;
			cnt += n;
			bfptr -> off += n;
			bfptr -> rem -= n;

		} while ( bp == savecp && cnt < recsiz );
		/* loop until we hit a real \n or recsiz is exhausted */

		/*
		 *	Second phase: discard characters up to and
		 *	including the next newline in the input.
		 *	This loop is optimized to miminize startup
		 *	overhead, because it will usually be executed
		 *	only once (but never less than once!)
		 */
		do {
			/*
			 *	decrement count of characters remaining
			 *	in the buffer, check for buffer underflow
			 */
			if ( --bfptr -> rem < 0 ) {
				n = read ( fdn, bfptr -> buf, bfptr -> siz );

				/* eof and input errors are never ok */
				if ( n < 0 )
					return -2;

				bfptr -> off = 0;
				bfptr -> rem = n - 1;
			}

			/*
			 *	The buffer is guaranteed non-empty,
			 *	and the count of characters remaining
			 *	has already been decremented by 1.
			 *	Pick up a character and bump the offset.
			 */
			n = bfptr -> buf [bfptr -> off++];
                } while (n != '\n');
		/* loop until we see a newline */

	} else {
		/* raw mode, try to read exactly "recsiz" characters */
		while ( recsiz > 0 ) {
			/* if the buffer is empty, try to fill it */
			if ( bfptr -> rem == 0 ) {
				n = read ( fdn, bfptr -> buf, bfptr -> siz );

				/* input error, no good */
				if ( n < 0 )
					return -2;

				/* eof, return what we got so far */
				if ( n == 0 )
					break;

				bfptr -> rem = n;
				bfptr -> off = 0;
			}

			/* calculate how many chars we can move */
			n = bfptr -> rem;
			if ( n > recsiz )
				n = recsiz;
			bp = bfptr -> buf + bfptr -> off;

			/* update pointers to move n characters */
			cnt += n;
			recsiz -= n;
			bfptr -> off += n;
			bfptr -> rem -= n;

			/* move n characters from bp to cp */
			if ( n & 1 )
				*cp++ = *bp++;
			n >>= 1;
			if ( n > 0 ) {
				do {
					*cp++ = *bp++;
					*cp++ = *bp++;
				} while ( --n > 0 );
			}
		}

                /* if we couldn't make any progress, signal end of file */
		if ( cnt == 0 )
			return -1;
	}

	return cnt;
}

