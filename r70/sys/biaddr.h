/*
 * vaxbi addressing
 */

struct biaddr {
	short nexus;		/* nexus number; software index */
	short vec;		/* error interrupt vector */
	short ovec;		/* vector offset if this is an adapter */
	short adno;		/* adapter number; e.g. nbia */
};

#if KERNEL
caddr_t biaddr();
int biinit();
#endif
