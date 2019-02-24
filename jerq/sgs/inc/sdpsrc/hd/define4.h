/*
 * static char ID_def4h[] = "@(#) define4.h: 1.1 1/7/82";
 */

/*
 * macros for manipulation
 *
 *
 *
 *	BUILDPN		build itemid from page number and address space
 *				number
 *	BYTEMASK	mask the size of an itemid's byte-offset
 *	MAXSP		maximum number of address spaces:
 *				2**SPIDBITS
 *	PAGEMASK	mask the size of an itemid's page-offset
 *	PAGESIZE	size of page, in bytes:
 *				2**BYTEBITS
 *	SPIDBITS	number of bits of itemid holing the address
 *				space number
 *	SPIDMASK	mask the size of an itemid's space-number
 *	SPSIZE		size of address space, in pages
 *				2**PAGEBITS
 *	XTRCTBY		extract and right-justify byte offset from itemid
 *	XTRCTPN		extract and right-justify page number from itemid
 *	XTRCTSP	 	extract and right-justify address space number from
 *				itemid
 */

#define SPIDBITS	(ITEMSIZE-PAGEBITS-BYTEBITS)
#define MAXSP		(1 << SPIDBITS)
#define PAGESIZE	(1 << BYTEBITS)
#define SPSIZE		(1L << PAGEBITS)

#define BYTEMASK	(PAGESIZE - 1L)
#define PAGEMASK	(SPSIZE - 1L)
#define SPIDMASK	(MAXSP - 1L)

#define BUILDPN(p,s) (((long) s) << (PAGEBITS+BYTEBITS)) | (((long) p) << BYTEBITS)

#define XTRCTBY(i) ((int) ((i) & BYTEMASK))
#define XTRCTPN(i) (((i) >> BYTEBITS) & PAGEMASK)
#define XTRCTSP(i) ((int) (((i) >> (PAGEBITS+BYTEBITS)) & SPIDMASK))
