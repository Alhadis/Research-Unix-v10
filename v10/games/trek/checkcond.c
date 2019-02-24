# include	"trek.h"


int	overide;	/*set during autoveride*/

/**
 **	check for condition after a move
 **/

checkcond()
{
	/* see if we are still alive and well */
	if (Status.reserves < 0.0)
		lose(L_NOLIFE);
	if (Status.energy <= 0)
		lose(L_NOENGY);
	if (Status.crew <= 0)
		lose(L_NOCREW);
	/* if in auto override mode, ignore the rest */
	if (overide)
		return;
	/* call in automatic override if appropriate */
	if (Quad[Quadx][Quady].stars < 0)
		autover();
	if (Quad[Quadx][Quady].stars < 0)
		lose(L_SNOVA);
	/* nullify distress call if appropriate */
	if (Nkling <= 0)
		killd(Quadx, Quady);

	/* set condition code */
	if (Status.cond == DOCKED)
		return;

	if (Nkling > 0)
	{
		Status.cond = RED;
		return;
	}
	if (Status.energy < 1000)
	{
		if(Status.cond!=YELLOW)
			printf("Condition YELLOW\n");
		Status.cond = YELLOW;
		return;
	}
	Status.cond = GREEN;
	return;
}


/**
 **	automatic override in case of a supernova
 **/

autover()
{
	float			dist;
	int			course;

	printf("*** RED ALERT: The %s is in a supernova quadrant\n", Status.shipname);
	printf("***  Emergency override attempts to hurl %s to safety\n", Status.shipname);
	if (Damage[WARP]) {
		printf("Warp engines damaged\n");
		return;
	}
	/* let's get our ass out of here */
	Status.warp = 6.0 + 2.0 * franf();
	Status.warp2 = Status.warp * Status.warp;
	Status.warp3 = Status.warp2 * Status.warp;
	dist = 0.75 * Status.energy / (Status.warp3 * (Status.shldup + 1));
	if (dist > 1.4142)
		dist = 1.4142;
	course = ranf(360);
	Status.cond = RED;
	overide++;
	warp(0, course, dist);
	overide=0;
}
