/*
 * generic disk-driver ioctls
 * not necessarily supported by every device
 */

#define	DIOSSIZ		(('D'<<8)|0)	/* set size for this device */
#define	DIOGSIZ		(('D'<<8)|1)	/* fetch size */
#define	DIOHDR		(('D'<<8)|2)	/* write sector header */
