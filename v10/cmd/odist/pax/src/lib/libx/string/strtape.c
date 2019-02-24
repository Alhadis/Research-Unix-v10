/*
 * local device pathname for portable tape unit specification is returned
 * if e is non-null then it is set to the next unused char in s
 *
 *	<unit><density>[<no-rewind>]
 *	{0-7}[l,m,h][n]
 */

#include <stdio.h>

char*
strtape(s, e)
register char*	s;
register char**	e;
{
	int		mtunit;
	int		mtdensity;
	char*		mtrewind;

	static char	tapefile[sizeof("/dev/Xrmt/XXX")];

	switch (*s)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
		mtunit = *s++;
		break;
	default:
		mtunit = '1';
		break;
	}
	if (!access("/dev/rmt/.", 0))
	{
		/*
		 * system V
		 */

		switch (*s)
		{
		case 'l':
		case 'm':
		case 'h':
			mtdensity = *s++;
			break;
		default:
			mtdensity = 'm';
			break;
		}
		switch (*s)
		{
		case 'n':
			mtrewind = "n";
			s++;
			break;
		default:
			mtrewind = "";
			break;
		}
		(void)sprintf(tapefile, "/dev/rmt/%c%c%s", mtunit, mtdensity, mtrewind);
		if (access(tapefile, 0)) (void)sprintf(tapefile, "/dev/rmt/ctape%c%s", mtunit, mtrewind);
		if (access(tapefile, 0)) (void)sprintf(tapefile, "/dev/rmt/%c%c%s", mtunit, mtdensity, mtrewind);
	}
	else if (!access("/dev/nrmt0", 0))
	{
		/*
		 * 9th edition
		 */

		switch (*s)
		{
		case 'l':
			mtunit = '0';
			s++;
			break;
		case 'm':
			mtunit = '1';
			s++;
			break;
		case 'h':
			mtunit = '2';
			s++;
			break;
		}
		switch (*s)
		{
		case 'n':
			mtrewind = "n";
			s++;
			break;
		default:
			mtrewind = "";
			break;
		}
		(void)sprintf(tapefile, "/dev/%srmt%c", mtrewind, mtunit);
	}
	else
	{
		/*
		 * BSD
		 */

		mtunit -= '0';
		switch (*s)
		{
		case 'l':
			s++;
			break;
		case 'm':
			mtunit |= 010;
			s++;
			break;
		case 'h':
			mtunit |= 020;
			s++;
			break;
		default:
			mtunit |= 010;
			break;
		}
		switch (*s)
		{
		case 'n':
			mtunit |= 040;
			s++;
			break;
		}
		(void)sprintf(tapefile, "/dev/rmt%d", mtunit);
	}
	if (e) *e = s;
	return(tapefile);
}
