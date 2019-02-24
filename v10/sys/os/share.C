/*
**	Share scheduler
*/

#include	"sys/param.h"
#include	"sys/lnode.h"
#include	"sys/share.h"
#include	"sys/charges.h"


float	zerof	= 0.0;
float	onef	= 1.0;
float	twof	= 2.0;

static void	adjgroups();



/*
**	Assumes that a group parent has been inserted into list before any group member.
**	List is rooted in ``lnodes[0]''.
*/

share()
{
	register KL_p	lp;
	register float	maxusage;
	register float	f;
	register float	g;
	register float	h;
	register float	one = onef;

	Users = 1;
	Groups = 0;
	TotUsage = zerof;
	MaxUsage = one;

	/*
	**	Special treatment for root.
	*/

	lp = &lnodes[0];
	f = lp->kl_cost;
	lp->kl_cost = 0;
	lp->kl.l_charge += f;
	lp->kl_temp += f;	/* Root's node accumulates all charges here */
	lp->kl_usage = one;	/* Root is not shared */

	if ( Shareflags & NOSHARE )
		return;

	/*
	**	Scan up active list and accumulate charges.
	*/

	for ( lp = lastlnode ; lp->kl_prev != (KL_p)0 ; lp = lp->kl_prev )
	{
		lp->kl_cost += lp->kl_muse * shconsts.sc_click;	/* Charge for memory */
		shconsts.sc_clickc += lp->kl_muse;		/* Count memory click */
		lp->kl_temp += lp->kl_cost;
		lp->kl.l_charge += lp->kl_temp;
		lp->kl_parent->kl_temp += lp->kl_temp;
	}

	/*
	**	Adjust costs for groups that are receiving too small a share.
	*/

	if ( Shareflags & ADJGROUPS )
		adjgroups(lnodes, one, 0);

	/*
	**	Scan down active list and process usage.
	*/

	maxusage = one;

	for ( lp = &lnodes[0] ; (lp = lp->kl_next) != (KL_p)0 ; )
	{
		if ( lp->kl_ghead != (KL_p)0 )
			Groups++;
		if ( !(lp->kl.l_flags & NOTSHARED) )
			Users++;

		/*
		**	Accumulate usage.
		*/

		f = lp->kl.l_usage;

		f *= DecayUsage;

		if ( lp->kl_ghead == (KL_p)0 )
			f += lp->kl_temp;
		else
			f += lp->kl_cost;

		lp->kl.l_usage = f;

		if ( g = lp->kl_norms )
		{
			if ( (f /= g) < twof )
				f = twof;	/* The minimum */

			lp->kl_usage = f;

			if ( f > maxusage && f < MAXUSAGE )
				maxusage = f;

			TotUsage += one / f;
		}
		else
			lp->kl_usage = MAXUSAGE;

		lp->kl_temp = zerof;
		lp->kl_cost = 0;
	}

	MaxUsage = maxusage;	/* Export value */

	/*
	**	Limit kl_usage so that no user has more than MAXUSHARE * <allocated share>.
	*/

	if ( !(Shareflags & LIMSHARE) || TotUsage == zerof )
		return;

	g = one / MAXUSHARE;

	for ( lp = &lnodes[0] ; (lp = lp->kl_next) != (KL_p)0 ; )
		if
		(
			(h = lp->kl_usage) < maxusage
			&&
			(f = g / (TotUsage * lp->kl_eshare)) > h
			&&
			f < maxusage
		)
		{
			TotUsage -= one / h;
			f = g / lp->kl_eshare - one;
			f /= TotUsage;
			lp->kl_usage = f;
			TotUsage += one / f;

			if ( f > maxusage && f < MAXUSAGE )
				maxusage = f;
		}

	MaxUsage = maxusage;	/* Export value */
}



/*
**	Increase run rates for groups getting more than their share.
*/

static void
adjgroups(gl, aa, d)
	KL_p		gl;
	float		aa;
	int		d;
{
	register KL_p	lp;
	register float	f;
	register float	totcharges = zerof;
	register float	a = aa;

	for ( lp = gl->kl_ghead ; lp != (KL_p)0 ; lp = lp->kl_gnext )
		totcharges += lp->kl_temp;

	if ( (totcharges /= gl->kl_eshare * MINGSHARE) == zerof )
		totcharges = onef;

	for ( lp = gl->kl_ghead ; lp != (KL_p)0 ; lp = lp->kl_gnext )
	{
		if ( lp->kl_ghead == (KL_p)0 || lp->kl_eshare == zerof )
		{
			lp->kl_rate *= a;
			continue;
		}

		if ( (f = lp->kl_temp / (lp->kl_eshare * totcharges)) > onef )
			f *= a;
		else
			f = a;

		lp->kl_rate *= f;

		if ( d < MAXGROUPS )
			adjgroups(lp, f, d+1);
	}
}



/*
**	Decay run rate.
*/

decayrate()
{
	register KL_p	lp = lnodes;
	register float	min = onef - NiceRates[NZERO];
	register float	dr = DecayRate;

	do
	{
		if ( (lp->kl_rate *= dr) < min )
			lp->kl_rate = min;
	}
	while
		( (lp = lp->kl_next) != (KL_p)0 );
}
