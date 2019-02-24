


/*
	sioarg( ioflg,ioptr,scptr ) scans any arguments after the
	filename and sets appropriate values in the passed ioblk.

	ioflg	0 - input / 3 - output association
	ioptr	-> ioblk

*/

#include "spitblks.h"
#include "spitio.h"

int	sioarg( ioflg,ioptr,scptr )

int	ioflg;
struct	ioblk	*ioptr;
struct	scblk	*scptr;

{
	int	cnt, lastdash = 0, strlen, v;
	int	lenfnm(), scnint();
	char	chr, *cp, tab = 011;

	cp	= scptr -> str;

	ioptr -> typ = 0;
	ioptr -> len = RECSIZ;
	ioptr -> pid = BUFSIZ;
	ioptr -> fdn = 0;
	ioptr -> flg = ioflg ? IO_OUP : IO_INP;

	if ( (cnt = lenfnm( scptr )) < 0 )
		return	-1;

	strlen	= scptr -> len;

	while ( cnt < strlen ) {

		chr	= *(cp + cnt++);

                if ( (chr == ' ') || (chr == tab) ) {
			lastdash = 0;
			continue;
		}

                if ( chr == '-' ) {
			if ( lastdash != 0 ) {
				return ( -1 );
			} else {
				lastdash = 1;
				continue;
			}
		}

		switch ( chr ) {


                case 'a':
                case 'A':
			ioptr -> flg |= IO_APP;
			break;

                case 'b':
                case 'B':
			v	= scnint( cp + cnt,strlen - cnt,&cnt );
			if ( v > 0 ) {
				ioptr -> pid = v;
			} else {
				return	-1;
			}
			break;

                case 'c':
                case 'C':
			ioptr -> len = -1;
			break;

                case 'f':
                case 'F':
			v	= scnint( cp + cnt,strlen - cnt,&cnt );
			ioptr -> fdn = v;
			ioptr -> flg |= ( IO_OPN | IO_SYS );
			if ( testty( v ) == 0 )
				ioptr -> flg |= IO_WRC;
			break;

                case 'l':
                case 'L':
			v	= scnint( cp + cnt,strlen - cnt,&cnt );
			if ( v > 0 ) {
				ioptr -> len = v;
			} else {
				return	-1;
			}
			break;

                case 'r':
                case 'R':
			v	= scnint( cp + cnt,strlen - cnt,&cnt );
			if ( v > 0 ) {
				ioptr -> len = -v;
			} else {
				return	-1;
			}
			break;

                case 'w':
                case 'W':
			ioptr -> flg |= IO_WRC;
			break;

		default:
			return	-1;
		}
		ioptr -> typ = 1;
		lastdash = 0;
	}
	return	0;
}

/*
	scnint( str,len ) scans an integer starting at the front of
	str.  in no case can the scan go more than len digits.
*/

int	scnint( str,len,intptr )

char	*str;
int	len;
int	*intptr;

{
	int	i = 0, n = 0;
	char	chr;

	while ( i < len ) {
		chr	= *(str + i++);
                if ( (chr >= '0') && (chr <= '9')) {
                        n = 10 * n + chr - '0';
		} else {
			--i;
			break;
		}
	}
	*intptr += i;
	return	n;
}



