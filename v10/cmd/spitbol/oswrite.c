/*
 *	oswrite( mode,recsiz,ioptr,scptr ) writes the record in scblk to
 *	the designated output file.  there are two types of transfers:
 *
 *	unbuffered	write is done immediately
 *
 *	buffered	write is done into buffer
 *
 *	in either case, a new-line is appended to the record if in
 *	line mode (mode > 0)
 */

#include "spitblks.h"
#include "spitio.h"

int
oswrite( mode, recsiz, ioptr, scptr )

int	mode;
register int	recsiz;
struct	ioblk	*ioptr;
struct	scblk	*scptr;

{
	char	*saveloc, savech;
	register char	*cp = scptr -> str;
	register int	fdn = ioptr -> fdn;
	int	ioerrcnt = 0;

	/*
	 *	if in line mode, save the character after the string
	 *	requested to be written and insert a new-line
	 */
	if ( mode > 0 ) {
		saveloc = cp + recsiz;
		savech = *saveloc;
		*saveloc = '\n';
		recsiz++;
	}

	/* if unbuffered, write the string directly to the output */
	if ( ioptr -> flg & IO_WRC ) {
		if ( write( fdn, cp, recsiz ) != recsiz)
			ioerrcnt++;

	} else {	/* buffered */
		register struct	bfblk	*bfptr = ioptr -> buf;

		while (recsiz > 0) {
			register int n = recsiz;

			if ( n > bfptr -> rem )
				n = bfptr -> rem;

			recsiz -= n;
			
			if ( n == bfptr -> siz ) {
				if ( write ( fdn, cp, n ) != n)
					ioerrcnt++;
				cp += n;
			} else {
				register char *r = bfptr -> buf + bfptr -> off;
				bfptr -> off += n;
				bfptr -> rem -= n;

				/* copy n characters from *cp to *r */
				if ( n & 1 )
					*r++ = *cp++;
				n >>= 1;
				while (--n >= 0) {
					*r++ = *cp++;
					*r++ = *cp++;
				}

				if ( bfptr -> rem == 0 ) 
					ioerrcnt += flush( ioptr );
			}
		}
			
	}

	/* if line mode, restore the previously saved character */
	if ( mode > 0 )
		*saveloc = savech;

	return ioerrcnt;
}
