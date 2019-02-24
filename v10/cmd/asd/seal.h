/*
 *	common parameters for seal and unseal
 */

#define	MAXLINE 100
#define	MAXULINE 200
#define	RADBASE (' ' + 1)
#define	INCPW 4
#define	OUTCPW 5
#define	RADIX 94
#define	BPC 8
#define	BYTEMASK 0xff
#define	CSMASK 0xffffffffL
#define	CSHIBIT	0x80000000L

unsigned long mkcsum();
