#include "trace.h"
#include "trace.d"

 extern struct TBL *tbl;
 extern int *globvars, nrvars;
 extern int assertbl, abase, errortbl, ebase;

require(TT, stuff, by)
{
	if (assertbl != NONE)
		assert(TT, stuff, by);
	if (errortbl != NONE)
		errort(TT, stuff, by);
}

inscope(TT, stuff, by)
{ register int i;

	for (i = 0; i < tbl[assertbl].nrcols; i++)
		if (TT == tbl[assertbl].coltyp[i]
		&&  stuff == tbl[assertbl].colmap[i]
		&&  by == tbl[assertbl].colorg[i])
			return 1;

	return 0;
}

assert(TT, stuff, by)
{ int h, i, j, x, frst;

	for (frst = 0; frst < tbl[assertbl].nrcols; frst++)
		if (tbl[assertbl].coltyp[frst] == TT
		&&  stuff == tbl[assertbl].colmap[frst]
		&&  by == tbl[assertbl].colorg[frst])
			break;

	if (frst == tbl[assertbl].nrcols)
		return;		/* not within assertion's scope */

	for (i = 0; i < tbl[assertbl].nrrows; i++)
	{
		if (globvars[i+abase] != 1)
			continue;

		for (j = frst; j < tbl[assertbl].nrcols; j++)
		{
			if (TT == tbl[assertbl].coltyp[j]
			&&  stuff == tbl[assertbl].colmap[j]
			&&  by == tbl[assertbl].colorg[j])
			{
			 if (tbl[assertbl].ptr[i][j].nrpils == 0)
			 	output("assertion violated: ", 1);
			 else
			 for (h = 0; h < tbl[assertbl].ptr[i][j].nrpils; h++)
			 {	x = tbl[assertbl].ptr[i][j].one[h].transf;
				globvars[x+abase] = 2;
				globvars[i+abase] = 0;
	}	}	}}
	for (i = abase; i < nrvars; i++)
		if (globvars[i] == 2)
			globvars[i] = 1;
}

assertholds()
{ int i;
	if (assertbl == NONE)
		return 1;

	for (i = abase; i < nrvars; i++)
		if (globvars[i] && tbl[assertbl].endrow[i-abase])
			return 1;
	return 0;
}

errort(TT, stuff, by)
{ int h, i, j, x, frst;

	for (frst = 0; frst < tbl[errortbl].nrcols; frst++)
		if (tbl[errortbl].coltyp[frst] == TT
		&&  stuff == tbl[errortbl].colmap[frst]
		&&  by == tbl[errortbl].colorg[frst])
			break;

	if (frst == tbl[errortbl].nrcols)
		return;		/* not within assertion's scope */

	for (i = 0; i < tbl[errortbl].nrrows; i++)
	{
		if (globvars[i+ebase] != 1)
			continue;

		for (j = frst; j < tbl[errortbl].nrcols; j++)
		{
			if (TT == tbl[errortbl].coltyp[j]
			&&  stuff == tbl[errortbl].colmap[j]
			&&  by == tbl[errortbl].colorg[j])
			{
			 if (tbl[errortbl].ptr[i][j].nrpils == 0)
			 	globvars[i+ebase] = 0;
			 else
			 for (h = 0; h < tbl[errortbl].ptr[i][j].nrpils; h++)
			 {	x = tbl[errortbl].ptr[i][j].one[h].transf;
				globvars[x+ebase] = 2;
				globvars[i+ebase] = 0;
			 }
			}
		}
	}
	for (i = ebase; i < nrvars; i++)
		if (globvars[i] == 2)
		{	globvars[i] = 1;
			if (tbl[errortbl].endrow[i-ebase])
				output("error matched: ", 2);
		}
	globvars[ebase] = 1;
}

peekassert(ice)
	struct FREEZE *ice;
{ register int i;
  if (assertbl != NONE)
	for (i = abase; i < nrvars; i++)
		globvars[i] = ice->varsaved[i];
  else if (errortbl != NONE)
	for (i = ebase; i < nrvars; i++)
		globvars[i] = ice->varsaved[i];
}
