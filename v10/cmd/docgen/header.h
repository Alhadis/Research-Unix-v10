/**********		docsend.h - 092884		**********/

#define HEADSIZE 160
#define UJN 0		/* unique job name - 14 bytes */
#define ULOG 15		/* user login - 8 bytes */
#define CO 24		/* company code - 2 bytes */
#define DOCNUM 28	/* first document number (cover sheet only) - 18 bytes */
#define TIMESTAMP 53	/* docsend timestamp - 9 bytes */
#define DEVICE 64	/* device: aps (APS-5), i10 (imagen), X97 (troff xerox), X (nroff xerox), 450 (nroff terminal), a.i (imagen with APS-5 simulation) - 3 bytes */
#define NETWORK 68	/* network - nusend (1), usend (2), others (0) - 1 byte */
#define TODSEND 70	/* TOD of docsend - 10 bytes */
#define DEST 81		/* JES remote station - 5 bytes */
#define BIN 87		/* bin or mail info - 34 bytes */
#define PAGES 90	/* TP%d total pages */
#define DUMMY 122	/* position for next field */
#define VERSION 152	/* docsend version (vbt) - 6 bytes */

