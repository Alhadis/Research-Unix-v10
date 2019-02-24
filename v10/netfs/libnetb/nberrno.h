/*
 * error numbers understood by netb
 * these should be an explicit part of the protocol,
 * but they aren't;
 * this stuff is specific to rf/libfsb
 *
 * the values intentionally correspond exactly
 * to Unix errno numbers and to fserrno numbers
 */

#define	NBEPERM		1	/* not file owner */
#define	NBENOENT	2	/* no such file or directory */
#define	NBEIO		5	/* general I/O error */
#define	NBENXIO		6	/* no such file address */
#define	NBEACCES	13	/* no permission */
#define	NBEBUSY		16	/* something is in use and can't be used again */
#define	NBEEXIST	17	/* file exists when it shouldn't */
#define	NBEXDEV		18	/* link across devices (or just not allowed) */
#define	NBENOTDIR	20	/* not a directory and should be */
#define	NBEISDIR	21	/* is a directory and shouldn't be */
#define	NBEINVAL	22	/* invalid arg or function; catchall for internal errors */
