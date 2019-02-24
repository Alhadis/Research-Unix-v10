/*
 * data structures associated with nexus devices;
 * sbi and vaxbi
 */

#define	NXSIZE	0x2000		/* size of nexus register space */
#define	NXWSIZE	01000000	/* size of nexus window space */

struct nexus {
	char nx[NXSIZE];
	char wind[NXWSIZE];
};

struct nextab {			/* table of configured nexus */
	char bus;		/* which group of nexus, if more than one */
	char adp;		/* which nexus within group */
};

#ifdef KERNEL
extern struct nextab nextab[];		/* list of nexus configured */
#endif
