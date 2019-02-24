/* @(#) optutil.h: 1.3 3/27/84				*/
/* optutil.h
**
**	Declarations for 3B optimizer utilities
**
**	David Kristol, April, 1982
**/

extern boolean isreg();		/* is operand a register? */
extern boolean isnib();		/* is operand a nibble? */
extern boolean isnegnib();	/* is operand a negative nibble? */
extern int getbit();		/* get exponent if value is power of 2 */
extern NODE * insert();		/* insert new instruction node */
extern void chgop();		/* change op code */
extern char * copyopn();	/* copy operand */
extern boolean isdyadic();	/* is instruction dyadic? */
extern boolean istriadic();	/* is instruction triadic? */
extern boolean totriadic();	/* can dyadic be made triadic? */
extern void makelive();		/* make register live */
extern void makedead();		/* make register dead */
extern boolean isindex();	/* operand is an index off a register */
extern boolean ismove();	/* is instruction a move-type? */
extern boolean iszoffset();	/* is operand 0 offset from register? */
extern boolean isnumlit();	/* is operand a numeric literal: &[-]n? */
extern void exchange();		/* exchange node and successor */
extern boolean doindirect();	/* resolve indirect references in node */
extern boolean usesvar();	/* does first operand use second? */
extern boolean isiros();	/* is op either immed, reg, or on stack? */

extern void ldelin();	
extern void ldelin2();	/* routines to preserve */
extern void lexchin();
extern void lmrgin1();	/* line number info */
extern void lmrgin2();
extern void lmrgin3();

/* declare string and other functions that are always handy for optimizations */

extern int strcmp();
extern char * strcpy();
extern int strlen();
extern char * strchr();
extern long atol();


/* declare functions defined in local.c that are handy here */

extern char * dst();		/* return pointer to destination string */
extern int stype();		/* return implied type of op code */
extern boolean samereg();	/* return true if same register */
extern boolean usesreg();	/* usesreg(a,b) true if reg. b used in a */
extern int setreg();		/* return bits corresponding to register */
extern boolean isdead();	/* return true if reg. dead after node */
extern void wchange();		/* flag a window change */
