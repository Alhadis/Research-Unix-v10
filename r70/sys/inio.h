/*
 * internet ioctls
 */

/* internet goo */
#define IPIOHOST	(('i'<<8)|1)
#define IPIONET		(('i'<<8)|2)
#define IPIOLOCAL	(('i'<<8)|3)
#define IPIOARP		(('i'<<8)|4)
#define IPIORESOLVE	(('i'<<8)|5)
#define IPIOMTU		(('i'<<8)|6)
#define IPIOROUTE	(('i'<<8)|7)
#define IPIOGETIFS	(('i'<<8)|8)
#define IPIOMASK	(('i'<<8)|9)

/* ugly tcp ioctls */
#define TCPIOHUP	(('T'<<8)|1)	/* HANGUP on TH_FIN */
#define	TCPIOMAXSEG	(('T'<<8)|2)
#define	TCPGETADDR	(('T'<<8)|3)
