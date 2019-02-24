/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * command line option parse assist
 */

char*		opt_arg;		/* {:,#} string argument	*/
char**		opt_argv;		/* most recent argv		*/
int		opt_char;		/* char pos in argv[opt_index]	*/
int		opt_index;		/* argv index			*/
char*		opt_msg;		/* error/usage message buffer	*/
long		opt_num;		/* # numeric argument		*/
char		opt_option[3];		/* current flag {-,+} + option  */
int		opt_pchar;		/* prev opt_char for backup	*/
int		opt_pindex;		/* prev opt_index for backup	*/

static char*	opt_opts[8];		/* cached opts arguments	*/
static int	opt_nopt;		/* opt_opts[] index		*/

#define elements(x)	(sizeof(x)/sizeof(x[0]))

#define OPT_MSG		512

static int	error();

extern char*	malloc();
extern char*	optusage();
extern long	strtol();

/*
 * argv:	command line argv where argv[0] is command name
 *
 * opts:	option control string
 *
 *	':'		option takes string arg
 *	'#'		option takes numeric arg (concat option may follow)
 *	'?'		(first) following options not in usage
 *			(following # or :) optional arg
 *			(following option or ]) remaining not in usage
 *	'['...']'	(following # or :) optional option arg description
 *			(otherwise) mutually exclusive option grouping
 *	' '...		optional argument(s) description (to end of string)
 *
 * return:
 *
 *	0		no more options
 *	'?'		usage: opt_arg points to message sans `Usage: command'
 *	':'		error: opt_arg points to message sans `command:'
 *
 * -- or ++ terminates option list and returns 0
 *
 * -? puts usage message sans command name in opt_arg and returns '?'
 *
 * if any # option is specified then numeric options (e.g., -123)
 * are associated with the leftmost # option in opts
 */

int
optget(argv, opts)
register char**	argv;
char*		opts;
{
	register int	c;
	register char*	s;
	int		n;
	char*		e;

	opt_pindex = opt_index;
	opt_pchar = opt_char;
	for (;;)
	{
		if (!opt_index)
		{
			opt_index = 1;
			opt_char = 0;
		}
		if (!opt_char)
		{
			if (opt_index == 1)
			{
				opt_argv = argv;
				opt_nopt = 0;
				if (*opts != '?') opt_opts[opt_nopt++] = opts;
			}
			if (!(s = argv[opt_index]) || (opt_option[0] = *s++) != '-' && opt_option[0] != '+' || !*s)
				return(0);
			if (*s++ == opt_option[0] && !*s)
			{
				opt_index++;
				return(0);
			}
			opt_char++;
		}
		if (c = opt_option[1] = argv[opt_index][opt_char++]) break;
		opt_char = 0;
		opt_index++;
	}
	if ((!opt_nopt || opt_opts[opt_nopt - 1] != opts) && opt_nopt < elements(opt_opts) && *opts != '?')
	{
		n = 0;
		do if (n >= opt_nopt)
		{
			opt_opts[opt_nopt++] = opts;
			break;
		} while (opt_opts[n++] != opts);
	}
	if (c == '?')
	{
		opt_arg = optusage((char*)0);
		return(c);
	}
	e = 0;
	if (c == ':' || c == '#' || c == ' ' || c == '[' || c == ']') s = "";
	else
	{
		s = opts;
		while (*s)
		{
			if (*s == c) break;
			if (*s == ' ')
			{
				s = "";
				break;
			}
			if (*s == ':' || *s == '#')
			{
				if (*++s == '?') s++;
				if (*s == '[')
				{
					n = 1;
					while (*++s)
					{
						if (*s == '[') n++;
						else if (*s == ']' && !--n)
						{
							s++;
							break;
						}
					}
					n = 0;
				}
			}
			else
			{
				if (!e && *s == '#' && s > opts) e = s - 1;
				s++;
			}
		}
	}
	if (!*s)
	{
		if (c < '0' || c > '9' || !e) return(error(""));
		c = opt_option[1] = *(s = e);
		opt_char--;
	}
	opt_arg = 0;
	if (*++s == ':' || *s == '#')
	{
		if (*(opt_arg = &argv[opt_index++][opt_char]))
		{
			if (*s == '#')
			{
				opt_num = strtol(opt_arg, &e, 0);
				if (e == opt_arg)
				{
					if (*(s + 1) == '?')
					{
						opt_arg = 0;
						opt_index--;
						return(c);
					}
					else c = error(s);
				}
				else if (*e)
				{
					opt_char += e - opt_arg;
					opt_index--;
					return(c);
				}
			}
		}
		else if (opt_arg = argv[opt_index])
		{
			opt_index++;
			if (*(s + 1) == '?' && (*opt_arg == '-' || *opt_arg == '+'))
			{
				if (*(opt_arg + 1)) opt_index--;
				opt_arg = 0;
			}
			else if (*s == '#')
			{
				opt_num = strtol(opt_arg, &e, 0);
				if (*e)
				{
					if (*(s + 1) == '?')
					{
						opt_arg = 0;
						opt_index--;
					}
					else c = error(s);
				}
			}
		}
		else if (*(s + 1) != '?') c = error(s);
		opt_char = 0;
	}
	return(c);
}

/*
 * return pointer to usage message sans `Usage: command'
 * if opts is 0 then opt_opts is used
 */

char*
optusage(opts)
char*	opts;
{
	register int	c;
	register char*	p;
	register char*	s;
	register char*	t;
	char*		b;
	char*		x;
	char*		z;
	char**		o;
	char**		v;
	char**		e;
	int		n;
	int		m;

	if (opts)
	{
		o = &opts;
		e = o + 1;
	}
	else
	{
		o = opt_opts;
		e = o + opt_nopt;
	}
	if (e == o) return("[* call optget() before optusage() *]");
	if (!opt_msg && !(opt_msg = malloc(OPT_MSG))) return("[* out of space *]");
	s = opt_msg;
	for (v = o; v < e; v++)
	{
		p = *v;
		while ((c = *p++) && c != ' ' && c != '?')
		{
			if (c == '[')
			{
				if (s != opt_msg) *s++ = ' ';
				*s++ = '[';
				b = s;
				m = 1;
				for (;;)
				{
					if (!(c = *p)) break;
					p++;
					if (c == '[')
					{
						m++;
						if (s != b)
						{
							*s++ = ' ';
							*s++ = '|';
							*s++ = ' ';
						}
						*s++ = '[';
						b = s;
					}
					else if (c == ']')
					{
						*s++ = ']';
						if (!--m) break;
					}
					else
					{
						if (s != b)
						{
							*s++ = ' ';
							*s++ = '|';
							*s++ = ' ';
						}
						*s++ = '-';
						*s++ = c;
						if (*p == ':' && (t = "arg") || *p == '#' && (t = "num"))
						{
							if (c = (*++p == '?'))
							{
								p++;
								*s++ = '[';
							}
							else *s++ = ' ';
							if (*p == '[')
							{
								n = 1;
								while (*s = *++p)
								{
									if (*p == '[') n++;
									else if (*p == ']' && !--n)
									{
										p++;
										break;
									}
									s++;
								}
							}
							else while (*s = *t++) s++;
							if (c) *s++ = ']';
						}
					}
				}
			}
			else if (*p == ':' || *p == '#')
			{
				if (*++p == '?') p++;
				if (*p == '[')
				{
					n = 1;
					while (*++p)
					{
						if (*p == '[') n++;
						else if (*p == ']' && !--n)
						{
							p++;
							break;
						}
					}
				}
			}
		}
	}
	b = s;
	for (v = o; v < e; v++)
	{
		p = *v;
		while ((c = *p++) && c != ' ' && c != '?')
		{
			if (c == '[')
			{
				m = 1;
				for (;;)
				{
					if (!(c = *p)) break;
					p++;
					if (c == '[') m++;
					else if (c == ']' && !--m) break;
				}
			}
			else if (*p == ':' || *p == '#')
			{
				if (*++p == '?') p++;
				if (*p == '[')
				{
					n = 1;
					while (*++p)
					{
						if (*p == '[') n++;
						else if (*p == ']' && !--n)
						{
							p++;
							break;
						}
					}
				}
			}
			else
			{
				if (s == b)
				{
					if (s != opt_msg) *s++ = ' ';
					*s++ = '[';
					*s++ = '-';
				}
				*s++ = c;
			}
		}
	}
	if (s != b) *s++ = ']';
	x = 0;
	z = 0;
	for (v = o; v < e; v++)
	{
		p = *v;
		while (c = *p++)
		{
			if (c == '[')
			{
				m = 1;
				for (;;)
				{
					if (!(c = *p)) break;
					p++;
					if (c == '[') m++;
					else if (c == ']' && !--m) break;
				}
			}
			else if (c == ' ')
			{
				x = p;
				break;
			}
			else if (c == '?') z = s;
			else if (*p == ':' && (t = "arg") || *p == '#' && (t = "num"))
			{
				if (s != b) *s++ = ' ';
				*s++ = '[';
				*s++ = '-';
				*s++ = c;
				if (c = (*++p == '?'))
				{
					p++;
					*s++ = '[';
				}
				else *s++ = ' ';
				if (*p == '[')
				{
					n = 1;
					while (*s = *++p)
					{
						if (*p == '[') n++;
						else if (*p == ']' && !--n)
						{
							p++;
							break;
						}
						s++;
					}
				}
				else while (*s = *t++) s++;
				if (c) *s++ = ']';
				*s++ = ']';
				if (z) s = z;
			}
		}
	}
	if (p = x)
	{
		if (s != opt_msg) *s++ = ' ';
		while (*s = *p++) s++;
	}
	return(opt_msg);
}

/*
 * point opt_arg to an error message for opt_option
 * p points to opts location for opt_option
 * optget() return value is returned
 */

static int
error(p)
register char*	p;
{
	register char*	s;
	register char*	t;
	int		n;

	if (!opt_msg && !(opt_msg = malloc(OPT_MSG))) opt_arg = "[* out of space *]";
	else
	{
		s = opt_arg = opt_msg;
		*s++ = opt_option[0];
		*s++ = opt_option[1];
		*s++ = ':';
		*s++ = ' ';
		if (*p == '#' || *p == ':')
		{
			if (*p == '#')
			{
				t = "numeric ";
				while (*s = *t++) s++;
			}
			if (*++p == '[')
			{
				n = 1;
				while (*s = *++p)
				{
					if (*s == '[') n++;
					else if (*s == ']' && !--n) break;
					s++;
				}
				*s++ = ' ';
			}
			p = "argument expected";
		}
		else p = "unknown option";
		while (*s++ = *p++);
	}
	return(':');
}
