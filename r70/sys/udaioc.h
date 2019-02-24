/*
 * ioctl info for ra driver
 */

#define	UIORRCT	(('u'<<8) | 0)	/* read a block of the rct */
#define	UIOWRCT	(('u'<<8) | 1)	/* write a block of rct */
#define	UIOCHAR	(('u'<<8) | 2)	/* get drive characteristics (sizes) */
#define	UIOREPL	(('u'<<8) | 3)	/* replace a block */
#define	UIOSPDW	(('u'<<8) | 4)	/* spin down on close */
#define	UIORST	(('u'<<8) | 5)	/* hopeless controller reset */

/*
 * struct for RRCT and WRCT
 */

struct ud_rctbuf {
	caddr_t	buf;
	int	lbn;
};

/*
 * struct for REPL
 */

struct ud_repl {
	daddr_t	replbn;		/* good block */
	daddr_t	lbn;		/* bad block */
	short prim;		/* nonzero if primary replacement */
};

/*
 * struct for CHAR
 */

struct ud_unit {
	daddr_t radsize;		/* size, from ONLINE end packet */
	daddr_t rctsize;		/* rct size, from get unit status */
	long medium;			/* generic type name */
	short tracksz;
	short groupsz;
	short cylsz;
	char rbns;
	char copies;
};
