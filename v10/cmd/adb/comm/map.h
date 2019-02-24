/*
 * file address maps
 * each open file has one per segment
 * if b <= address <= e, address is valid in space type sp
 * and may be found at address + f in the file
 */

#define	NMAP	5	/* text data stack u-area endmarker */

struct map {
	ADDR	b;		/* base */
	ADDR	e;		/* end */
	ADDR	f;		/* offset within file */
	short	sp;		/* type of space */
	short	flag;
};
typedef	struct map	MAP;
#define	MPINUSE	01

extern	MAP	cormap[], symmap[];
