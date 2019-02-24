# include	"trek.h"

/**
 **	phaser control
 **/

CVNTAB	Matab[]
{
	"a",		"utomatic",
	"m",		"anual",
	0
};

phaser()
{
	register int		i, k;
	int			manual, flag, units, extra;
	int			hit, reqd, n;
	double			tot;
	int			hits[MAXKLQUAD], hitreqd[MAXKLQUAD];
	double			dustfac, tothe;

	if (Status.cond == DOCKED) {
		printf("Phasers cannot fire through starbase shields.\n");
		return;
	}
	if (Damage[PHASER]) {
		printf("Phaser control damaged.\n");
		return;
	}
	if (Status.shldup) {
		printf("Scotty: we will have to divert power from the shields.\n");
		if(!getynpar("Is that wise"))
			return;
		Move.shldchg = 1;
	}
	if (Status.cloaked) {
		printf("Sulu: Captain, you know that we cannot fire while cloaked.\n");
		return;
	}
	manual = 0;
	if (Damage[COMPUTER])
		if(lineended()) {
			printf("Computer damaged, manual mode selected\n");
			manual = 1;
		}

	if (!manual)
	{
		if((manual=(getcodpar("Manual or automatic", Matab))) < 0) return;
	}
	if (!manual && Damage[COMPUTER])
	{
		printf("Computer damaged, manual selected\n");
		newline();
		manual = 1;
	}

	/* set up hits[] with amount for each klingon */
	flag = 1;
	n = Nkling;
	if (manual)
	{
		while (flag)
		{
			printf("%d units available\n", Status.energy);
			extra = 0;
			for (i = 0; i < n; i++)
			{
				printf("Klingon at %d,%d:",
					Kling[i].x, Kling[i].y);
				if(getintpar(" units to fire", &units)==0 || units < 0) return;
				hits[i] = units;
				extra =+ units;
				if (extra > Status.energy)
				{
					printf("Available energy exceeded. ");
					newline();
					break;
				}
			}
			flag = i < n;
			if (extra <= 0)
				return;
		}
		Status.energy =- extra;
		extra = 0;
	}
	else
	{
		/* automatic distribution of power */
		printf("Phasers locked on target.  ");
		while (flag)
		{
			printf("%d units available\n", Status.energy);
			if(getintpar("Units to fire", &units)==0 || units < 0) return;
			if (units > Status.energy)
			{
				printf("Available energy exceeded.  ");
				newline();
				continue;
			}
			flag = 0;
			Status.energy =- units;
			extra = units;
			if (Nkling != 0)
			{
				tot = Nkling * (Nkling + 1) / 2;
				for (i = 0; i < Nkling; i++)
				{
					hits[i] = ((Nkling - i) / tot) * extra;
					extra =- hits[i];
					dustfac = 0.90;
					tothe = Kling[i].dist;
					hitreqd[i] = Kling[i].power / pow(dustfac, tothe) + 0.5;
					if (hits[i] > hitreqd[i])
					{
						extra =+ hits[i] - hitreqd[i];
						hits[i] = hitreqd[i];
					}
				}
				if (extra != 0)
				{
					for (i = 0; i < Nkling; i++)
					{
						reqd = hitreqd[i] - hits[i];
						if (reqd <= 0)
							continue;
						if (reqd >= extra)
						{
							hits[i] =+ extra;
							extra = 0;
							break;
						}
						hits[i] = hitreqd[i];
						extra =- reqd;
					}
				}
			}
		}
	}

	/* actually fire the shots */
	Move.free = 0;
	k = 0;
	for (i = 0; i < n; i++)
	{
		if (hits[i] == 0)
		{
			k =+ 1;
			continue;
		}
		dustfac = 0.90 + 0.01 * franf();
		tothe = Kling[k].dist;
		hit = hits[i] * pow(dustfac, tothe) + 0.5;
		Kling[k].power =- hit;
		printf("%d unit hit\t", hit);
		if (Kling[k].power <= 0) {
			extra =- Kling[k].power;
			killk(Kling[k].x, Kling[k].y);
		} else {
			printf("on Klingon at %d,%d\n",	Kling[k].x, Kling[k].y);
			k =+ 1;
		}
	}
	if (extra > 0)
		printf("%d units expended on empty space\n", extra);
}
