/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * in-place path name canonicalization -- preserves the logical view
 * pointer to trailing 0 in path returned
 *
 *	remove redundant .'s and /'s
 *	move ..'s to the front
 *	/.. preserved (for pdu and newcastle hacks)
 *	FS_3D handles ...
 * 
 * longer pathname possible if FS_3D ... involved
 */

char*
pathcanon(path)
char*	path;
{
	register char*	r;
	register char*	s;
	register char*	t;
	register int	dots;

	dots = 0;
	r = s = t = path;
	for (;;) switch (*t++ = *s++)
	{
	case '.':
		dots++;
		break;
	case 0:
		s--;
		/*FALLTHROUGH*/
	case '/':
		while (*s == '/') s++;
		switch (dots)
		{
		case 1:
			t -= 2;
			break;
		case 2:
			if (t - 5 < r) r = t;
			else for (t -= 5; t > r && *(t - 1) != '/'; t--);
			break;
		case 3:
#if defined(FS_3D)
			{
				char*		x;
				char*		o;
				int		c;

				extern char*	pathnext();

				o = t;
				if ((t -= 5) <= path) t = path + 1;
				c = *t;
				*t = 0;
				if (x = pathnext(path, s - (*s != 0)))
				{
					r = path;
					if (t == r + 1) x = r;
					s = t = x;
				}
				else
				{
					*t = c;
					t = o;
				}
			}
#else
			r = t;
#endif
			break;
		}
		if (!*s)
		{
			if (t > path && !*(t - 1)) t--;
			if (t == path) *t++ = '.';
#if DONT_PRESERVE_TRAILING_SLASH
			else if (t > path + 1 && *(t - 1) == '/') t--;
#else
			else if ((s <= path || *(s - 1) != '/') && t > path + 1 && *(t - 1) == '/') t--;
#endif
			*t = 0;
			return(t);
		}
		dots = 0;
		break;
	default:
		dots = 4;
		break;
	}
}
