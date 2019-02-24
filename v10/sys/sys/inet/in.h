#ifndef INADDR_ANY
/*
 * Bits in internet addresses.
 */
#define IN_CLASSA(i)		((((long)(i))&0x80000000)==0)
#define IN_CLASSA_NET		0xff000000
#define IN_CLASSA_NSHIFT	24
#define IN_CLASSA_HOST		0x00ffffff

#define IN_CLASSB(i)		((((long)(i))&0xc0000000)==0x80000000)
#define IN_CLASSB_NET		0xffff0000
#define IN_CLASSB_NSHIFT	16
#define IN_CLASSB_HOST		0x0000ffff

#define IN_CLASSC(i)		((((long)(i))&0xc0000000)==0xc0000000)
#define IN_CLASSC_NET		0xffffff00
#define IN_CLASSC_NSHIFT	8
#define IN_CLASSC_HOST		0x000000ff

#define INADDR_ANY		0x0

#ifdef KERNEL
extern u_long in_class_nmask[];
extern u_long in_class_hmask[];
#define IN_CLASS_NMASK(i) in_class_nmask[((u_long)i)>>30]
#define IN_CLASS_HMASK(i) in_class_hmask[((u_long)i)>>30]
extern struct block *bp_pullup();
extern unsigned short u_cksum(), ntohs(), htons();
extern unsigned long htonl(), ntohl();
#endif KERNEL

#ifndef KERNEL
struct in_service {
	char *name;		/* service name */
	char *proto;		/* protocol name */
	unsigned long port;	/* port number */
};

extern struct in_service *in_service();
#endif KERNEL

typedef unsigned long in_addr;
#endif INADDR_ANY
