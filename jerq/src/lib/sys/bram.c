#include <setup.h>
#include <jerq.h>
#include <font.h>
#define ESC	'\033'
#define	BRAM_SIZE	2048	/* size of BRAM in bytes */

checkbram()	/* return 1 if bram has valid checksum.  If not, clear */
		/* the battery backed ram, putting in a valid checksum */
{
	register long i = 0;
	register int j;

	for (j=0; j<BRAM_SIZE - 2; j++)
		i += BRAM->opts.opt_array[j].byte;
	j =  (BRAM->opts.opt_array[BRAM_SIZE - 2].byte << 8) + 
		BRAM->opts.opt_array[BRAM_SIZE - 1].byte;
	i %= (1 << 16);		/* truncate to checksum size */
	if ( i == j)
		return(1);
	for (i=0; i<BRAM_SIZE; i++)
		BRAM->opts.opt_array[i].byte = (unsigned char) 0;
/*	for (i=0; i<8; i++)						*/
/*	{								*/
/*		BRAM->pfkeys[i][0].byte = (unsigned char) ESC;		*/
/*		BRAM->pfkeys[i][1].byte = (unsigned char) '0';		*/
/*		BRAM->pfkeys[i][2].byte = (unsigned char) ('c' + i);	*/
/*	}								*/
/*	THIS is how to implement ansi std default pfkeys		*/

	return(0);
}

setbram()	/* reset checksum in bram... done when bram updated */
{
	register long i = 0;
	register int j;

	for (j=0; j<BRAM_SIZE - 2; j++)
		i += BRAM->opts.opt_array[j].byte;
	BRAM->opts.opt_array[BRAM_SIZE - 1].byte = i & 0xff;
	BRAM->opts.opt_array[BRAM_SIZE - 2].byte = (i >> 8) & 0xff;
}

