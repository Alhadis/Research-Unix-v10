/*
 * Format of messages between Unix and Jerq.
 * Identically-shaped messages are sent both ways.
 * The order they appear in the structure is the order they get sent
 */
/* NDATA==59==MAXPKTDSIZE-5 */
#define	NDATA	59	/* max number of chars at once */
/*
 * This is what the code uses, and what appears on the line.
 * The extra byte of data is not sent, it is for the trailing NUL.
 */
typedef struct Message{
	unsigned char	file;
	unsigned char	nbytes;
	char		op;
	short		posn;
	char		data[NDATA+1];	/* +1 for the null */
}Message;
char *data2();
/*
 * Operations
 */
#define	O_DIAGNOSTIC	0	/* string is for diagnostic purposes */
#define	O_INSERT	1	/* insert string */
#define	O_BACKSPACE	2	/* backspace typed */
#define	O_SEARCH	3	/* search or remember search pattern */
#define	O_CUT		4	/* cut selected text */
#define	O_PASTE1	5	/* delete selected text */
#define	O_SELECT	6	/* select block of text */
#define	O_SNARF		7	/* remember selected text */
#define	O_CLOSE		8	/* close file */
#define	O_REQUEST	9	/* request NDATA chars from host */
#define	O_SEEK		10	/* seek to position in file */
#define	O_SCROLL	11	/* relative seek; Jerq talks lines, Unix chars */
#define	O_DONE		12	/* end of command processing */
#define	O_RESET		13	/* reset file/frame */
#define	O_MOVE		14	/* move to somewhere else in file */
#define	O_FILENAME	15	/* open new file; name is in data */
#define	O_CHARSONSCREEN	16	/* number of chars visible in frame */
#define	O_PASTE2	17	/* insert from paste buffer */
#define	O_SETSNARF	18	/* set snarf buffer contents */
#define	O_MODIFIED	19	/* set modified for file */
#define	O_WRITE		20	/* write file */
#define	O_SWEEP		21	/* go to box cursor state */
