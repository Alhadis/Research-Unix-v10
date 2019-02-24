/*
 * `space' tags: qualify addresses
 */

#define	SPTYPE	077

#define	NOSP	00	/* nowhere (= command) */
#define	DATASP	01	/* data space */
#define	INSTSP	02	/* instruction space */
#define	UBLKSP	03	/* user block (saved registers) */
#define	REGSP	04	/* registers */
#define	ANYSP	077	/* wildcard for symbol lookups */

#define	CORF	0	/* in core image */
#define	SYMF	0100	/* in symbol file */

#define	RAWADDR	0200	/* ignore kernel mapping for this address */
