/*	bx25lib.h 1.1 of 7/6/83
	@(#)bx25lib.h	1.1
 */

#define NULL	0

/*	PVC error codes */
#define EPVCSZ	-1	/*  window or packet size values invalid */
#define EPVCNCO	-2	/* network control device open failed; check errno */
#define EPVCNCI	-3	/* nc device ioctl failed; check errno */

/*	bx25ilnk flag values */
#define LNKISB	01	/* tell interpreter line is an X.25 "B" address */
#define LNKMOD	02	/* wait for data-set ready */
#define LNKANS	04	/* answer a dial-up connection (3B20) */
#define	LNKFAST	010	/* The device speed is greater than 9.6KB */
 
/*	bx25alnk flag values */
#define LNKDTE	01	/* role of DTE */
#define LNKDCE	02	/* role of DCE */

/*	LNK error codes */
#define ELNKSZ	-1	/* window size or empbufsz specified is illegal */
#define ELNKNCO	-2	/* network control device open failed; check errno */
#define ELNKNCI	-3	/* nc device ioctl failed; check errno */
#define ELNKDS	-4	/* stat of physical device failed; check errno */
#define ELNKDNC	-5	/* file assoc. with device name not char special dev */
#define ELNKVCSZ  -6	/* hipvc or hisvc specified is illegal */
#define ELNKLNO	-7	/* device line number illegal */
#define ELNKADR	-8	/* address length is too long */

