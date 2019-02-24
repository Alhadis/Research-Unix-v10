/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*	@(#)picasso:math.c	1.0	*/
#include	"picasso.h"
#include	"y.tab.h"

double	Log(x)
	double	x;
{
	return errcheck(log(x), "log");
}

double	Log10(x)
	double	x;
{
	return errcheck(log10(x), "log");
}

double	Exp(x)
	double	x;
{
	return errcheck(exp(x), "exp");
}

double	Sqrt(x)
	double	x;
{
	return errcheck(sqrt(x), "sqrt");
}

double	Cos(x)
	double	x;
{
/*	if (fmod(x,180.) == 90)
		return 0;	*/
	return cos(x/* * M_PI/180 */);
}

double	Sin(x)
	double	x;
{
/*	if (fmod(x,180.) == 0)
		return 0;	*/
	return sin(x/* * M_PI/180 */);
}

double	Pow(x, y)
	double	x, y;
{
	return errcheck(pow(x,y), "pow");
}

double	Atan2(y, x)
	double	y, x;
{
	return (atan2(y, x)/* * 180.0/M_PI */);
}

double	Minvar(s)
	char	*s;
{
	struct	symtab	*p;
	double	m;
	int	i;

	p = findvar(s, VARNAME);
	if (p->s_dim == 0)
		m = p->s_val.f; 
	else for (m = p->s_val.a[0], i = 1; i < p->s_dim; ++i)
		if (p->s_val.a[i] < m)
			m = p->s_val.a[i];
	return m;
}

double	Minlist()
{
	double	m;

	for (m = exprlist[--nexpr]; nexpr; )
		if (exprlist[--nexpr] < m)
			m = exprlist[nexpr];
	return m;
}

double	Maxvar(s)
	char	*s;
{
	struct	symtab	*p;
	double	m;
	int	i;

	p = findvar(s, VARNAME);
	if (p->s_dim == 0)
		m = p->s_val.f; 
	else for (m = p->s_val.a[0], i = 1; i < p->s_dim; ++i)
		if (p->s_val.a[i] > m)
			m = p->s_val.a[i];
	return m;
}

double	Maxlist()
{
	double	m;

	for (m = exprlist[--nexpr]; nexpr; )
		if (exprlist[--nexpr] > m)
			m = exprlist[nexpr];
	return m;
}
