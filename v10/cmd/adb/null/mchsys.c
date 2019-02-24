/*
 * system-mode mapping
 * stub version
 */

#include "defs.h"

kmsys()
{

	printf("no sys map\n");
}

kmproc()
{

	printf("no sys map\n");
}

/*
 * map a kernel address to a physical address
 * arg is a pointer to be filled in
 * returns nonzero if address is valid
 */

int
kmap(paddr, sp)
ADDR *paddr;
{

	return (1);
}
