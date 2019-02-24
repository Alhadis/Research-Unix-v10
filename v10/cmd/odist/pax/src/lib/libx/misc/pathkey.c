/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * generate 14 char lookup key for lang path in key
 * based on 32-bit checksum on path
 *
 * if key==0 then space is malloc'd
 * if attr != 0 then attribute var assignments placed here:
 *	ATTRIBUTES	list of attribute names
 */

#include <limits.h>
#include <ctype.h>
#include <preroot.h>
#include <stdio.h>

#define elements(x)	(sizeof(x)/sizeof(x[0]))

char*
pathkey(key, attr, lang, path)
char*		key;
char*		attr;
char*		lang;
register char*	path;
{
	register char*		s;
	register char*		k;
	char*			t;
	char*			flags;
	char**			p;
	int			c;
	unsigned long		n;
	char			buf[15];
	char*			usr[16];
	char*			env[elements(usr) + 3];
	char*			ver[2];
	char			tmp[PATH_MAX];

	extern char*		getenv();
	extern int		getuniv();
	extern unsigned long	memsum();
	extern char*		strcopy();
	extern char*		strdup();
	extern int		strlen();
	extern char*		strchr();
	extern char*		strrchr();

	extern char**		environ;

	for (c = 0; c < elements(env); c++) env[c] = 0;
	if (!key) key = buf;
	n = 0;

	/*
	 * trailing flags in path
	 */

	if (flags = strchr(path, ' '))
	{
		if (flags == path) flags = 0;
		else
		{
			(void)strcpy(tmp, path);
			*(flags = tmp + (flags - path)) = 0;
			path = tmp;
		}
	}

	/*
	 * 3D
	 */

	if (fs3d(2) && (c = mount(path, tmp, 012|(PATH_MAX<<4))) > 1 && c < PATH_MAX)
		path = tmp;

	/*
	 * preroot
	 */

	if (attr) attr = strcopy(attr, "PREROOT='");
#if FS_PREROOT
	if (k = getenv(PR_BASE))
	{
		if (s = strrchr(k, '/')) k = s + 1;
		n = memsum(k, strlen(k), n);
	}
	if (attr && (getpreroot(attr, path) || getpreroot(attr, (char*)0))) attr += strlen(attr);
#else
	if ((k = getenv("VIRTUAL_ROOT")) && *k == '/')
	{
		n = memsum(k, strlen(k), n);
		if (attr) attr = strcopy(attr, k);
	}
#endif

	/*
	 * universe
	 */

	if (attr) attr = strcopy(attr, "' UNIVERSE='");
	if (!getuniv(buf))
	{
		n = memsum(buf, strlen(buf), n);
		if (attr) attr = strcopy(attr, buf);
	}

	/*
	 * environment
	 *
	 *	${VERSION_ENVIRONMENT}	: list of alternate env vars
	 *	${VERSION_<lang>}
	 *	${VERSION_<base(path)>}
	 *	${<toupper(base(path))>VER}
	 */

	if (attr) *attr++ = '\'';
	k = getenv("VERSION_ENVIRONMENT");
	c = 0;
	while (c < elements(usr))
	{
		usr[c++] = k;
		if (k) for (;;)
		{
			if (!*k)
			{
				k = 0;
				break;
			}
			if (*k++ == ':') break;
		}
	}
	ver[0] = lang;
	ver[1] = k = (s = strrchr(path, '/')) ? s + 1 : path;
	s = buf;
	if (isdigit(*k))
	{
		if (*k == '3' && *(k + 1) == 'b')
		{
			/*
			 * cuteness never pays
			 */

			k += 2;
			*s++ = 'B';
			*s++ = 'B';
			*s++ = 'B';
		}
		else *s++ = 'U';
	}
	for (; (c = *k) && s < &buf[sizeof(buf) - 1]; k++)
	{
		if (!isalnum(c)) c = '_';
		else if (islower(c)) c = toupper(c);
		*s++ = c;
	}
	*s = 0;
	for (p = environ; *p; p++)
	{
		s = "VERSION_";
		for (k = *p; *k && *k == *s; k++, s++);
		if (*k && !*s)
		{
			for (c = 0; c < elements(ver); c++)
				if (!env[c] && (s = ver[c]))
				{
					for (t = k; *t && *t != '=' && *t++ == *s; s++);
					if (*t == '=' && (!*s || (s - ver[c]) > 1))
					{
						env[c] = *p;
						goto found;
					}
				}
		}
		if (!env[2])
		{
			s = buf;
			for (k = *p; *k && *s++ == *k; k++);
			if ((s - buf) > 2 && k[0] == 'V' && k[1] == 'E' && k[2] == 'R' && k[3] == '=')
			{
				env[2] = *p;
				goto found;
			}
		}
		for (c = 0; c < elements(usr) && (s = usr[c]); c++)
			if (!env[c + elements(env) - elements(usr)])
			{
				for (k = *p; *k && *k == *s; k++, s++);
				if (*k == '=' && (!*s || *s == ':'))
				{
					env[c + elements(env) - elements(usr)] = *p;
					goto found;
				}
			}
	found:	;
	}
	for (c = 0; c < elements(env); c++)
		if (k = env[c])
		{
			if (attr)
			{
				*attr++ = ' ';
				while ((*attr++ = *k++) != '=');
				*attr++ = '\'';
				attr = strcopy(attr, k);
				*attr++ = '\'';
			}
			else while (*k && *k++ != '=');
			n = memsum(k, strlen(k), n);
		}
	if (attr)
	{
		attr = strcopy(attr, " ATTRIBUTES='PREROOT UNIVERSE");
		for (c = 0; c < elements(env); c++)
			if (k = env[c])
			{
				*attr++ = ' ';
				while ((*attr = *k++) != '=') attr++;
			}
		*attr++ = '\'';
		*attr = 0;
	}

	/*
	 * now the normal stuff
	 */

	if (flags) *flags = ' ';
	s = path + strlen(path);
	(void)sprintf(key, "%08lX", memsum(path, s - path, n));
	k = key + 14;
	*k = 0;
	if (!flags) t = path;
	else if ((t = s - 4) < flags) t = flags + 1;
	for (;;)
	{
		if (--s < t)
		{
			if (t == path) break;
			s = flags - 2;
			t = path;
		}
		if (*s != '/' && *s != ' ')
		{
			*--k = *s;
			if (k <= key + 8) break;
		}
	}
	while (k > key + 8) *--k = '.';
	return(key == buf ? strdup(key) : key);
}
