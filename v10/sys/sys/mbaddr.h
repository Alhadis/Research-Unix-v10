/*
 * addressing data structures for massbus devices
 */

struct mbaddr {
	char unit;	/* unit number within this massbus */
	char mbno;	/* massbus number within system */
};

#if KERNEL
caddr_t mbaddr();
long mbcuraddr();
#endif
