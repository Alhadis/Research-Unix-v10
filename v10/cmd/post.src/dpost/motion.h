/*
 *
 * Position adjusting macros.
 *
 */

#define hgoto(n)	hpos = n
#define hmot(n)		hpos += n
#define vgoto(n)	vpos = n
#define vmot(n)		vpos += n

extern int	hpos;
extern int	vpos;

