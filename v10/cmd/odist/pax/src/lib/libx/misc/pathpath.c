/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * return full path to p with mode access using $PATH
 * if a!=0 then it and $0 and $_ with $PWD are used for
 * related root searching
 * the related root must have a bin subdir
 * full path returned in path buffer
 */

extern char*	getenv();
extern char*	pathaccess();
extern char*	strchr();
extern char*	strcopy();

char*
pathpath(path, p, a, mode)
register char*	path;
char*		p;
char*		a;
int		mode;
{
	register char*	s;
	char*		x;

	extern char**	environ;
	extern char**	opt_argv;

	static char*	cmd;

	if (s = a)
	{
		x = s;
		a = p;
		p = "..";
		if (strchr(s, '/') || ((s = cmd) || opt_argv && (s = *opt_argv)) && strchr(s, '/') || (s = *environ) && *s++ == '_' && *s++ == '=' && strchr(s, '/') || *x && !access(x, 0) && (s = getenv("PWD")) && *s == '/')
		{
			if (!cmd) cmd = s;
			s = strcopy(path, s);
			for (;;)
			{
				do if (s <= path) goto normal; while (*--s == '/');
				do if (s <= path) goto normal; while (*--s != '/');
				(void)strcpy(s + 1, "bin");
				if (!access(path, 0))
				{
					if (s = pathaccess(path, path, p, a, mode)) return(s);
					goto normal;
				}
			}
		normal: ;
		}
	}
	if (!a && strchr(p, '/')) x = "";
	else if (!(x = getenv("PATH"))) x = ":/bin:/usr/bin";
	if (!(s = pathaccess(path, x, p, a, mode)) && !*x && (x = getenv("FPATH")))
		s = pathaccess(path, x, p, a, mode);
	return(s);
}
