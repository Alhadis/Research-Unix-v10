/*	@(#)x25lib.h	1.1	*/
/*	NC identifier	*/
#define	NCDEV	"/dev/nc"
#define NULL	0

/*	mdevno error codes	*/
#define	MDERR1	-4
#define	MDERR2	-5

/*	PVC installation flag values */
#define PVCSESS	01	/* Session conect/disconnect packets used */
#define PVCREST	02	/* RESET in-order/out-of-order responded to */
#define PVCNONE	04	/* no establishment protocol used */

/*	PVC error codes */
/*	EPVCNP can occur only on a x25pvci */
#define EPVCNP	-1	/* no (or multiple) setup protocol specified */
			/* one of PVCSESS, REST, or NONE must be in flag arg */
#define EPVCNCO	-2	/* network control device open failed; check errno */
#define EPVCNCI	-3	/* nc device ioctl failed; check errno */
#define EPVCSS	-4	/* stat of slot (pvc) name failed; check errno */
#define EPVCSNC	-5	/* file assoc. with slot name not char special dev */


/*	x25lnk flags and return error codes */

/*	LNK flag values */
#define LNKPKT	01	/* packet size specified */
#define LNKMOD	02	/* Modem control device name specified */
#define LNKISB	010	/* tell interpreter line is an X.25 "B" address */
#define	LNKBACK	040	/* This cmd is for the backup device */
#define	LNKFAST	0100	/* The device speed is greater than 9.6KB */

/*	LNK error codes */
/*	nc open and ioctl errors only ones possible for x25lnkh and x25lnks */
#define ELNKPKT	-1	/* packet size specified is illegal */
#define ELNKNCO	-2	/* network control device open failed; check errno */
#define ELNKNCI	-3	/* nc device ioctl failed; check errno */
#define ELNKDS	-4	/* stat of physical device failed; check errno */
#define ELNKDNC	-5	/* file assoc. with device name not char special dev */
#define ELNKMCO	-8	/* modem control dev open failed; check errno */
#define ELNKMCI	-9	/* modem control ioctl failed; check errno */
#define ELNKLNO	-10	/* device line number illegal */


