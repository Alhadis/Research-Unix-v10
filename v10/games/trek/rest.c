# include	"trek.h"

/**
 **	rest for repairs
 **/

rest()
{
	float			t;
	register int		percent;

	if(getfltpar("How long", &t)==0) return;
	if (t <= 0.0)
		return;
	percent = 100 * t / Status.time + 0.5;
	if (percent >= 70)
	{
		printf("Spock: That would take %d%% of our remaining time.\n",
			percent);
		if (!getynpar("Are you sure that is wise"))
			return;
	}
	Move.delta = t;
	t = Etc.eventptr[E_LRTB]->date - Status.date;
	if (Status.cond != DOCKED && Move.delta > t)
		Move.delta = t + 0.0001;
	Move.free = 0;
	Move.resting = 1;
}
