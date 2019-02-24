/*
 * VAX processor register numbers
 * only the generic ones
 */

#define	KSP	0x0		/* kernel stack pointer */
#define	ESP	0x1		/* exec stack pointer */
#define	SSP	0x2		/* supervisor stack pointer */
#define	USP	0x3		/* user stack pointer */
#define	ISP	0x4		/* interrupt stack pointer */
#define	P0BR	0x8		/* p0 base register */
#define	P0LR	0x9		/* p0 length register */
#define	P1BR	0xa		/* p1 base register */
#define	P1LR	0xb		/* p1 length register */
#define	SBR	0xc		/* system segment base register */
#define	SLR	0xd		/* system segment length register */
#define	PCBB	0x10		/* process control block base */
#define	SCBB	0x11		/* system control block base */
#define	IPL	0x12		/* interrupt priority level */
#define	ASTLVL	0x13		/* async. system trap level */
#define	SIRR	0x14		/* software interrupt request */
#define	SISR	0x15		/* software interrupt summary */
#define	ICCS	0x18		/* interval clock control */
#define	NICR	0x19		/* next interval count */
#define	ICR	0x1a		/* interval count */
#define	TODR	0x1b		/* time of year (day) */
#define	RXCS	0x20		/* console receiver control and status */
#define	RXDB	0x21		/* console receiver data buffer */
#define	TXCS	0x22		/* console transmitter control and status */
#define	TXDB	0x23		/* console transmitter data buffer */
#define	MAPEN	0x38		/* memory management enable */
#define	TBIA	0x39		/* translation buffer invalidate all */
#define	TBIS	0x3a		/* translation buffer invalidate single */
#define	PMR	0x3d		/* performance monitor enable */
#define	CPUSID	0x3e		/* system identification */
