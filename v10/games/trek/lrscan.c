# include	"trek.h"

/**
 **	long range scanners
 **/

lrscan()
{
	register int			i, j;

	if (Damage[LRSCAN])
	{
		if(Status.cond!=DOCKED) {
			printf("Long range scanners inoperable.\n");
			return;
		} else
			printf("L.R. scanners damaged; using Starbase scanners.\n");
	}
	printf(" ");
	for (j = Quady-1; j <= Quady + 1; j++)
	{
		if (j < 0 || j >= NQUADS)
			printf("    ");
		else
			printf(" -%1d-", j);
	}
	printf("\tLong range scan for quadrant %d,%d\n", Quadx, Quady);
	for (i = Quadx - 1; i <= Quadx + 1; i++) {
		if (i < 0 || i >= NQUADS) {
			printf("   *   *   *");
		} else {
			printf("%1d", i);
			for (j = Quady - 1; j <= Quady + 1; j++) {
				if (j < 0 || j >= NQUADS) {
					printf("  * ");
				} else {
					prsect(1,i,j);
				}
			}
		}
		printf("\n");
	}
	return;
}
