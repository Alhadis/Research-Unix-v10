

/*
	lenfnm( scptr ) returns the length of the filename in the
	passed scblk.  the filename may take on a number of forms:

        'filename'      'filename options'      ' options'

        '!*commandstring'

        '!*commandstring*'

        '!*command string* options'
*/

#include "spitblks.h"

int	lenfnm( scptr )

struct	scblk	*scptr;

{
	int	cnt = 0,
		len;
	char	*cp, delim;

	len	= scptr -> len;
	if ( len == 0 )
		return	0;

	cp	= scptr -> str;

        if ( *cp == '!' ) {
		if ( len < 3 )
			return	-1;
		delim	= *(cp + ++cnt);
		if ( *(cp + ++cnt) == delim )
			return	-1;
		while ( cnt < len )
			if ( *(cp + cnt++) == delim )
				break;
	} else {
                if ( *cp == ' ' )
			return	0;
		while ( cnt < len )
                        if ( *(cp + cnt++) == ' ' )
				break;
                if ( cp[cnt - 1] == ' ' )
			--cnt;
	}
	return	cnt;
}


