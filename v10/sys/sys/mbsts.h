/*
 * error bits in the VAX MASSBUS status register
 * common to error checking in MASSBUS device drivers
 *
 * some of these bits can probably go away
 */

#define	MBSR_DTBUSY	0x80000000	/* data transfer busy */
#define	MBSR_NRCONF	0x40000000	/* no response confirmation */
#define	MBSR_CRD	0x20000000	/* corrected read data */
#define	MBSR_CBHUNG	0x00800000	/* control bus hung */
#define	MBSR_PGE	0x00080000	/* programming error */
#define	MBSR_NED	0x00040000	/* non-existant drive */
#define	MBSR_MCPE	0x00020000	/* massbus control parity error */
#define	MBSR_ATTN	0x00010000	/* attention from massbus */
#define	MBSR_SPE	0x00004000	/* silo parity error */
#define	MBSR_DTCMP	0x00002000	/* data transfer completed */
#define	MBSR_DTABT	0x00001000	/* data transfer aborted */
#define	MBSR_DLT	0x00000800	/* data late */
#define	MBSR_WCKUP	0x00000400	/* write check upper */
#define	MBSR_WCKLWR	0x00000200	/* write check lower */
#define	MBSR_MXF	0x00000100	/* miss transfer error */
#define	MBSR_MBEXC	0x00000080	/* massbus exception */
#define	MBSR_MDPE	0x00000040	/* massbus data parity error */
#define	MBSR_MAPPE	0x00000020	/* page frame map parity error */
#define	MBSR_INVMAP	0x00000010	/* invalid map */
#define	MBSR_ERRCONF	0x00000008	/* error confirmation */
#define	MBSR_RDS	0x00000004	/* read data substitute */
#define	MBSR_ISTIMO	0x00000002	/* interface sequence timeout */
#define	MBSR_RDTIMO	0x00000001	/* read data timeout */

#define	MBSR_HARD	(MBSR_PGE|MBSR_ERRCONF|MBSR_ISTIMO|MBSR_RDTIMO)
#define MBSR_EBITS	(~(MBSR_DTBUSY|MBSR_CRD|MBSR_ATTN|MBSR_DTCMP))
