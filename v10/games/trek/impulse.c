# include	"trek.h"

/**
 **	move under impulse power
 **/

float newpos();

# define ISPEED	0.095

impulse()
{
	int			course;
	register int		power;
	float			dist, time;
	register int		percent;

	if (Status.cond == DOCKED) {
		printf("Scotty: Sorry captain, but we are still docked\n");
		return;
	}
	if (Damage[IMPULSE]) {
		printf("Scotty: Our impulse engines are damaged\n");
		return;
	}
	if (getcodi(&course, &dist)==0)
		return;
	power = 20 + 100 * dist;
	percent = 100 * power / Status.energy + 0.5;
	if (percent >= 85)
	{
		printf("Scotty: That would consume %d%% of our remaining energy.\n",
			percent);
		if (!getynpar("Are you sure that is wise"))
			return;
		printf("Aye aye, sir\n");
	}
	time = dist / ISPEED;
	percent = 100 * time / Status.time + 0.5;
	if (percent >= 85)
	{
		printf("Spock: That would take %d%% of our remaining time.\n",
			percent);
		if (!getynpar("Are you sure that is wise"))
			return;
		printf("(He's finally gone mad)\n");
	}
	Move.delta = newpos(0, course, time, ISPEED);
	dist = Move.delta * ISPEED;
	Status.energy =- 20 + 100 * dist;
}
