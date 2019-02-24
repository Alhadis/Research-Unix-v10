# include	"trek.h"

/**
 **	schedule damages to device
 **/

damage(dev, dam)
int	dev;		/*  device index */
float	dam;		/* time to repair */
{
	register int		i;
	register EVENT		*e;
	register int		f;

	if (dam <= 0.0)
		return;
	if (Status.cond == DOCKED)
		dam =* Param.dockfac;
	f = Damage[dev];
	Damage[dev] = 1;
	if (!f)
	{
		schedule(E_FIXDV, dam, 0, 0, dev);
		return;
	}
	for (i = 0;  i  < MAXEVENTS; i++)
	{
		e = &Event[i];
		if (e->evcode != E_FIXDV || e->systemname != dev)
			continue;
		reschedule(e, dam);
		return;
	}
	syserr("Cannot find old damages %d\n", dev);
}
