/*
 *	ieee.h	-	IEEE INTERFACE HEADER
 *		Version II (modified for portability) 12/84
 */


/*	ioctl modes, form  ioctl(dev,mode,arg).  arg is pointer
 *	to argument of type integer or short as indicated.
 *	(Short and int are the same for DEC PDP11/23)
 */
#define	IBSETPID	001	/* set new pid for SRQ signal (short) */
#define	IBSETSIG	002	/* set returned signal for SRQ (short) */
#define	IBGETSTAT	004	/* get device status (int) */
#define	IBSENDC		010	/* send cmd (int) */


#define	TALK		(1<<6)	/* talk device address mask */
#define	LISTEN		(1<<5)	/* listen device address mask */
#define	MSA	(TALK | LISTEN)	/* secondary device access mask */

	/*	COMMANDS	*/
#define	LLO	021	/* universal:	LOCAL LOCKOUT	*/
#define	DCL	024	/*		DEVICE CLEAR	*/
#define	PPU	025	/*		PARALLEL POLL UNCONFIGURE	*/
#define	SPE	030	/*		SERIAL POLL ENABLE	*/
#define	SPD	031	/*		SERIAL POLL DISABLE	*/
#define	GTL	001	/* addressed:	GO TO LOCAL	*/
#define	SDC	004	/*		SELECTIVE DEVICE CLEAR	*/
#define	PPC	005	/*		PARALLEL POLL CONFIGURE	*/
#define	GET	010	/*		GROUP EXECUTE TRIGGER	*/
#define	TCT	011	/*		TAKE CONTROL	*/

#define	UNTALK		0137	/* bus untalk character ("_") */
#define	UNLISTEN	077	/* bus unlisten character ("?") */

