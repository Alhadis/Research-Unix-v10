
/*
 *
 * A complete set of Impress opcode definitions up to Version 2.0
 * Pre-Release. I've removed the following commands which are now
 * obsolete and should not be used:
 *
 *	AM	130		Move command
 *	ASRULE	192		Rule command - use ABRULE
 *	ASETH	195		Horizontal motion - use ASETAH or
 *				ASETRH
 *	ASETV	196		Vertiacl motion - use ASETAV or
 *				ASETRV instead.
 *	ADELG	200		Delete named glyph
 *	AFGD	240		Force glyph delete
 *
 * The ASGLY command is also obsolete but it's I think it's useful so I
 * haven't removed its definition from this file.
 *
 * I've added a few definitions needed for things like graphics and
 * glyph rotation.
 *
 */



#define	ASP		128		/* space */
#define	ASP1		129		/* space plus 1 */
#define	AMPLUS		131		/* mplus */
#define	AMMINUS		132		/* mminus */

#define	AMMOVE		133		/* mmove */
#define	ASMOVE		134		/* smove */

#define	ASETAH		135		/* set absolute h */
#define	ASETRH		136		/* set relative h */
#define	ASETAV		137		/* set absolute v */
#define	ASETRV		138		/* set relative v */

#define	ACIRCARC	150		/* circular arc */
#define	AELLIPSEARC	151		/* ellipse */
#define	ACIRCSEGM	160		/* circular segment */

#define	ABRULE		193		/* brule */

#define	ACRLF		197		/* crlf */

#define ASGLY		198		/* define small glyph */
#define	ABGLY		199		/* bgly */
#define	ADELG		200		/* delete glyph */

#define	ASETHV		205		/* set hv system */
#define	ASETAD		206		/* set advance directions */

#define	ASF		207		/* set family */

#define	ASETIL		208		/* set inter-line space */
#define	ASETBOL		209		/* set beginning of line */
#define	ASETSP		210		/* set sp */

#define	APUSH		211		/* push */
#define	APOP		212		/* pop */
#define	ASETPM		214		/* set push mask */

#define	APAGE		213		/* page */
#define	AENDP		219		/* endpage */

#define	ACFT		221		/* create family table */
#define	ACM		222		/* create map */

#define	ASETPUM		225		/* path update mode */

#define	ASETP		230		/* set path */
#define	ASETX		231		/* set texture */
#define	ASETPEN		232		/* set pen */
#define	AFILLP		233		/* fill path */
#define	ADRAWP		234		/* draw path */
#define	ABITMAP		235		/* bitmap */
#define	ASETMAG		236		/* set magnification */

#define	ADEFMAC		242		/* define macro */
#define	AXQTMAC		243		/* execute macro */

#define	ANOOP		254		/* no op - used for padding */
#define	AEOF		255		/* eof */


/*
 *
 * A few special definitions needed if we're doing graphics.
 *
 */


#define	WHITE_OP	0
#define	OPAQUE_OP	3
#define	OR_OP		7
#define	BLACK_OP	15


/*
 *
 * Glyphs used in rotated coordinate systems need special treatment.
 * These defintions didn't fit well in any of the other header files
 * so I've included them here.
 *
 */


#define ROT_0		0
#define ROT_90		1
#define ROT_180		2
#define ROT_270		3
#define ROT_COUNT	4		/* for array sizes etc. */


