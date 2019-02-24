/*
	host <-> terminal protocol
*/

#define REQ	ACK	/* host->term; asks for a NAK \n if you are proof */
#define NAK	025	/* term->host; yes I am proof; goes into troff mode */
#define ACK	017
#define	SHORT	65336
#define		LARGE		9999

/*
	host -> terminal
*/
#define	TFONT	1	/* font coming */
#define	TTEXT	2	/* troff commands */
#define	TTERM	3	/* become a terminal */
#define	TEXIT	4	/* go away */

/*
	terminal -> host
*/
#define	HQUIT	1	/* I want to go home */
#define	HEXIT	2	/* I want to die */
#define	HSEEK	3	/* send page,offset */
#define	HWANT	4	/* want a font */
#define	HCOMMENT	5	/* comment */

/*
	troff commands
*/
#define	C_HOR	128	/* 128-227 right horizontal shifts */
#define	C_PAGE	228	/* N; here comes page N */
#define C_FONT	229	/* set font to c-C_FONT (=0-10) */
#define C_BSIZE	240	/* B; set point size to B */
#define C_ABSH	241	/* N; move horiz absolute */
#define C_ABSV	242	/* N; move ver absolute */
#define	C_DEV	243	/* STR; send devcntrl (only res, font) */
#define		RES	1	/* n */
#define		FONT	2	/* n,str  mount font */
#define		MODE	3	/* set pen mode */
#define	C_DRAW	244	/* STR; wholde draw string */
#define		LINE	1
#define		CIRCLE	2	/* center, radius */
#define		ELLIPSE	3	/* center, major, minor */
#define		ARC	4	/* p1, p2, p3 */
#define		SPLINE	5	/* n, n points */
#define C_SIZE	245
#define C_MAX	255	/* set point size to c-C_SIZE+SIZE_ORIG */

#define	SIZE_ORIG	6

#define	DEBUG		/* define this for debugging */
