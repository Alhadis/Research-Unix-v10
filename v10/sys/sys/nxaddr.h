/*
 * addressing and adapters for old-style nexus devices
 * sbi, cmi, et al
 * not vaxbi
 */

struct nxaddr {
	char nexus;	/* nexus number; index in nexus table */
	char adno;	/* adapter number; e.g. 8600 */
	short voff;	/* vector offset, e.g. unibus adapter */
};

#if KERNEL
caddr_t nxaddr();
#endif
