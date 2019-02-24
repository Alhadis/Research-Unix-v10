/*
 * generic ethernet ioctls
 */

#define ENIOTYPE	(('e'<<8)|1)	/* set receive packet type */
#define ENIOADDR	(('e'<<8)|2)	/* fetch physical addr */
#define ENIOCMD		(('e'<<8)|3)	/* perform interface board cmd */
#define ENIOPROM	(('e'<<8)|4)	/* go promiscuous */
