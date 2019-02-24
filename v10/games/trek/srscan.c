# include	"trek.h"

/**
 **	short range sensor scan
 **/

srscan()
{
	scansys(0);
}

status()
{
	scansys(-1);
}

scansys(f)
int f;
{
	register int		i, j;
	register int		statinfo;
	QUAD			*q;
	static int		savestat;

	if (Damage[SRSCAN] && f >= 0) {
		if (!f)
			printf("S.R. scanners out");
		if(Status.cond==DOCKED) {
			printf("; using Starbase scanners.\n");
		} else {
			printf("\n");
			return;
		}
	}
	if (f)
		statinfo = 1;
	else
	{
		if (lineended()==0)
			savestat = getynpar("Status report");
		statinfo = savestat;
	}
	if (f > 0)
		savestat = 1;
	if (f >= 0)
	{
		q = &Quad[Quadx][Quady];
		q->scanned = q->qkling * 100 + q->bases * 10 + q->stars;
		printf("  ");
		for (i = 0; i < NSECTS; i++)
		{
			printf("%d ", i);
		}
		printf("\tS.R. sensor scan for quadrant %d,%d\n", Quadx, Quady);
	}

	for (i=0; i<NSECTS && mkfault==0; i++)
	{
		if (f >= 0)
		{
			printf("%d ", i);
			for (j = 0; j < NSECTS; j++)
				printf("%c ", Sect[i][j]);
			printf("%d", i);
			if (statinfo)
				printf("   ");
		}
		if (statinfo)
			getinfo(i);
		else	printf("\n");
	}
	if (f < 0)
	{
		printf("current crew  %d\n", Status.crew);
		printf("brig space    %d\n", Status.brigfree);
		return;
	}
	printf("  ");
	for (i = 0; i < NSECTS; i++)
		printf("%d ", i);
	if (q->systemname&Q_DISTRESS)
		printf("\tDistressed starsystem %s",
				Systemname[Event[q->systemname&Q_STARNAME].evdata]);
	else
		if (q->systemname&Q_GHOST)
			printf("\tGhost starsystem %s",
					Systemname[q->systemname&Q_STARNAME]);
		else	if(q->systemname)
			printf("\tStarsystem %s", Systemname[q->systemname]);
	printf("\n");
}

effshld()
{
	return(100.0 * Status.shield / Initial.shield);
}
