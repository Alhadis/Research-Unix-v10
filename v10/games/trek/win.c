# include	"trek.h"

/**
 **	Signal game won
 **/

extern long	writelog();
int		mkfault;

win()
{
	long		s;

	mkfault=0;
	sleep(1);
	printf("\nCongratulations, you have saved the Federation\n");
	Move.endgame = 1;
	s=writelog(0);
	if (Game.helps == 0 && Game.killb == 0 && Game.killinhab == 0 && 5 * Game.kills + Game.deaths < 100 &&
			s >= 1000 && Status.ship == ENTERPRISE)
	{
		printf("In fact, you are promoted one step in rank,\n");
		switch (Game.skill)
		{

		  case 1:
			printf("from 'novice' to 'fair'\n");
			break;

		  case 2:
			printf("from 'fair' to 'good'\n");
			break;

		  case 3:
			printf("from 'good' to 'expert'\n");
			break;

		  case 4:
			printf("from 'expert' to 'commodore'\n");
			break;

		  case 5:
			printf("from 'commodore' to 'impossible'\n");
			break;

		  case 6:
			printf("to the exalted rank of Commodore Emeritus\n");
			break;
		}
	}
	reset();
}
