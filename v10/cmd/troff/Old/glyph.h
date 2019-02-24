/*	@(#)font.h	1.1	*/
struct Fontheader
  {
	short		f_magic ;	/* Should be 0436 */
	unsigned short	f_size ;	/* Total size of raster section
					 * of the font file.
					 */
	short		f_maxx ;	/* Largest x of character */
	short		f_maxy ;	/* Largest y of character */
	short		f_xtnd ;	/* Largest extender */
  } ;

struct Charparam
  {
	unsigned short	c_addr ;	/* Offset in file where character
					 * is located.
					 */
	short		c_size ;	/* Size in bytes of character	*/
	char		c_up ;		/* c_up + c_down = heigth of	*/
	char		c_down ;	/* raster in font file.		*/
	char		c_left ;	/* c_right + c_left = width of	*/
	char		c_right ;	/* raster in font file.		*/
	char		c_width ;	/* Width, including space required
					 * to print character.
					 */
  } ;

/*	RSTRUCTS is the size of the structures at the beginning of a	*/
/*	Versatec raster file.  RSTRUCTS + (long)charparam->c_addr is	*/
/*	the address of the character's raster.				*/

#define	RSTRUCTS	((long)(sizeof(struct Fontheader)+ \
			    256*sizeof(struct Charparam)))
