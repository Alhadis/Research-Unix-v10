#include "playdefs.h"

/* 'put_chunk' puts piece number 'chunknum' of a mushroom (glowing)
 *	or otherwise at 'col' and 'row'.
 */
put_chunk(chunknum, col, row)
int	chunknum,
	col,
	row;
{
	switch (chunknum) {
		case 1:
			if (glowmush[col][row])
				bitblt (&gl1looks, gl1looks.rect,
				&display, mushorigin (col, row), F_XOR);
			else
				bitblt (&chunk1looks, chunk1looks.rect,
				&display, mushorigin (col, row), F_XOR);
			return;
		case 2:
			if (glowmush[col][row])
				bitblt (&gl2looks, gl2looks.rect, &display,
				 add (mushorigin (col, row), Pt (0, 8)),
				 F_XOR);
			else
				bitblt (&chunk2looks, chunk2looks.rect, 
				&display, add (mushorigin (col, row),
					       Pt (0, 8)), F_XOR);
			return;
		case 3:
			if (glowmush[col][row])
				bitblt (&gl3looks, gl3looks.rect, &display,
				 add (mushorigin (col, row), Pt (0, 16)),
				 F_XOR);
			else
				bitblt (&chunk3looks, chunk3looks.rect,
				&display, add (mushorigin (col, row),
						Pt (0, 16)), F_XOR);
			return;
		case 4:
			if (glowmush[col][row])
				bitblt (&gl4looks, gl4looks.rect, &display,
				 add (mushorigin (col, row), Pt (0, 24)),
				 F_XOR);
			else
				bitblt (&chunk4looks, chunk4looks.rect,
				&display, add (mushorigin (col, row),
						Pt (0, 24)), F_XOR);
			return;
		default:
			error ("Funny chuck - can't put it");
			exit (1);
	}
}
