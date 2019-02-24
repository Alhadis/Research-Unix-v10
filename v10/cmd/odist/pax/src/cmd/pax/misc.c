/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax miscellaneous support
 */

#include "pax.h"

/*
 * return format index given format name
 */

int
getformat(name)
register char*	name;
{
	register int	i;

	(void)strlower(name);
	for (i = 0; format[i].name; i++)
		if (streq(name, format[i].name))
			break;
	return(format[i].name ? i : streq(name, "-") || streq(name, "pax") ? OUT_DEFAULT : -1);
}

/*
 * check f with patterns given on cmd line
 */

int
selectfile(f)
register struct fileinfo*	f;
{
	register struct deltainfo*	d;

	if (f->skip || f->namesize <= 1) return(0);
	if (f->record.format && state.record.pattern)
	{
		static char	fmt[2];

		fmt[0] = f->record.format;
		if (!strmatch(fmt, state.record.pattern)) return(0);
	}
	if (state.append || (state.delta.op & COLLECT))
	{
		(void)addlink(f);
		if (state.delta.tab)
		{
			if (!(d = allocate(struct deltainfo)))
				error(3, "%s: out of space", f->name);
			d->dev = f->st->st_dev;
			d->ino = f->st->st_ino;
			d->mtime = f->st->st_mtime;
			d->offset = state.in.offset + state.in.count;
			d->size = f->st->st_size;
			if (state.delta.op & COLLECT)
			{
				if (!(d->info = (struct fileinfo*)memdup(f, sizeof(struct fileinfo))) || !(d->info->st = (struct stat*)memdup(f->st, sizeof(struct stat))))
					error(3, "%s: out of space", f->name);
				d->info->name = hashput(state.delta.tab, f->name, (char*)d);
				if (d->info->uidname) d->info->uidname = strdup(d->info->uidname);
				if (d->info->gidname) d->info->gidname = strdup(d->info->gidname);
				d->info->delta.base = d;
			}
			else (void)hashput(state.delta.tab, f->name, (char*)d);
		}
		return(0);
	}
	if (!match(f->name) || state.verify && f->type != S_IFDIR && !verify(f)) return(0);
	state.selected++;
	if (state.list) (void)addlink(f);
	return(1);
}

/*
 * verify action on file
 *
 *	EOF	exit
 *	NULL	skip file
 *	.	keep file
 *	<else>	rename file
 */

int
verify(f)
register struct fileinfo*	f;
{
	register char*	prompt;
	register int	n;

	static char	namebuffer[PATH_MAX + 2];

	if (state.yesno) switch (state.operation)
	{
	case IN:
		prompt = "Read";
		break;
	case OUT:
		prompt = "Write";
		break;
	default:
		prompt = "Pass";
		break;
	}
	else prompt = "Rename";
	fprintf(state.wtty, "%s %s: " , prompt, f->name);
	if (!fgets(namebuffer, sizeof(namebuffer), state.rtty))
	{
		putc('\n', state.wtty);
		finish(2);
	}
	n = strlen(namebuffer);
	namebuffer[n - 1] = 0;
	if (state.yesno) return(namebuffer[0] == 'y');
	switch (namebuffer[0])
	{
	case 0:
		return(0);
	case '.':
		if (!namebuffer[1]) break;
		/*FALLTHROUGH*/
	default:
		f->namesize = pathcanon(f->name = namebuffer) - namebuffer + 1;
		break;
	}
	return(1);
}

/*
 * check for file name mapping
 * static data possibly returned
 * two simultaneous calls supported
 */

char*
map(name)
register char*	name;
{
	register struct maplist*	mp;

	static char			filebuffer[2][PATH_MAX];
	static char*			filename = filebuffer[0];

	filename = (filename == filebuffer[0]) ? filebuffer[1] : filebuffer[0];
	for (mp = state.maps; mp; mp = mp->next)
		if (reexec(mp->re, name))
		{
			(void)resub(mp->re, name, mp->into, filename, mp->flags);
			if (mp->flags & RE_VERBOSE) fprintf(stderr, "%s >> %s\n", name, filename);
			return(filename);
		}
	return(name);
}

/*
 * list entry information based on state.drop, state.list and state.verbose
 */

void
listentry(f)
register struct fileinfo*	f;
{
	register char*	s;
	long		size;
	char		buf[PATH_MAX];

	if (!f->skip && (state.drop || state.list || state.verbose))
	{
		if (state.drop)
		{
			if (++state.dropcount >= 50)
			{
				state.dropcount = 0;
				fprintf(stderr, ".\n");
			}
			else
			{
				fprintf(stderr, ".");
				fflush(stderr);
			}
		}
		else
		{
			switch (f->delta.op)
			{
			case 0:
				s = 0;
				break;
			case DELTA_create:
				s = "create";
				break;
			case DELTA_delete:
				s = "delete";
				break;
			case DELTA_update:
				s = "update";
				break;
			case DELTA_verify:
				s = "verify";
				break;
			default:
				s = "??????";
				break;
			}
			size = f->st->st_size;
			if (f->linktype == SOFTLINK) f->st->st_size = f->linknamesize - 1;
			else if (f->delta.size != -1) f->st->st_size = f->delta.size;
			(void)fmtls(buf, f->name, f->st, s, f->linktype == NOLINK ? (char*)0 : f->linkname, (state.list && state.verbose) ? LS_LONG : 0);
			f->st->st_size = size;
			fprintf(state.list ? stdout : stderr, "%s\n", buf);
		}
	}
}

/*
 * prepare patterns for match()
 */

char**
initmatch(p)
char**	p;
{
	register char**	a;

	a = p;
	while (*a)
		(void)pathcanon(*a++);
	return(p);
}

/*
 * determine if file s matches input patterns
 */

int
match(s)
register char*	s;
{
	register char**	p;
	register char*	t;
	int		n;

	if (!(p = state.patterns)) return(state.matchsense);
	if (state.exact)
	{
		n = 0;
		while (t = *p++)
			if (*t)
			{
				if (streq(s, t))
				{
					*--p = "";
					return(1);
				}
				n = 1;
			}
		if (!n) finish(0);
	}
	else while (t = *p++)
	{
		if (dirprefix(t, s) || strmatch(s, t))
			return(state.matchsense);
	}
	return(!state.matchsense);
}

/*
 * return 1 if p is a directory prefix of s
 */

int
dirprefix(p, s)
register char*	p;
register char*	s;
{
	while (*p)
		if (*p++ != *s++)
			return(0);
	return(!*s || *s == '/');
}

#if DEBUG

/*
 * place op names from op into buf
 */

static char*
opnames(p, name, op)
register char*	p;
char*		name;
register int	op;
{
	register char*	s;
	register int	n;

	static struct
	{
		char*	name;
		int	op;
	}		ops[] =
	{
		"IN",		IN,
		"OUT",		OUT,
		"COLLECT",	COLLECT,
		"COMPRESS",	COMPRESS,
		"CONVERT",	CONVERT,
	};

	extern char*	strcopy();

	if (name)
	{
		p = strcopy(p, name);
		*p++ = '=';
	}
	s = p;
	for (n = 0; n < elements(ops); n++)
		if (op & ops[n].op)
		{
			if (s != p) *p++ = '|';
			p = strcopy(p, ops[n].name);
		}
	return(p);
}

/*
 * return the current operation names
 */

char*
operations()
{
	static char	buf[256];

	(void)sprintf(opnames(opnames(buf, "operation", state.operation), " delta", state.delta.op), " sum=%d", state.delta.sum);
	return(buf);
}

#endif
