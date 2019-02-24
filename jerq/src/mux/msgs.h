/*
 * Character-driven machine for Jerq to Unix communication.
 * Usual format is: [command][data]
 */

#define	C_SENDCHAR	1	/* Send character to layer process (delim implicit) */
#define	C_NEW		2	/* Create new layer process group */
#define	C_UNBLK		3	/* Unblock layer process */
#define	C_DELETE	4	/* Delete layer process group */
#define	C_EXIT		5	/* Exit */
#define	C_DELIM		6	/* Send delimiter message to stream */
#define	C_SENDNCHARS	7	/* Send several characters to layer proc. (delim explicit) */
#define	C_RESHAPE	8	/* Layer has been reshaped */
#define	C_KILL		9	/* Send signal to pgrp */
#define	C_PUSHLD	10	/* Push line discipline onto stream */
#define	C_POPLD		11	/* Pop line discipline from stream */
#define	C_SENDNCHARD	12	/* Send several characters to layer proc. (delim implicit) */
