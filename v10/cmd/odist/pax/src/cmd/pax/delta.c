/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax delta archive support
 */

#include "pax.h"

static void		deltacopy();

extern int		delta();
extern int		update();

#if 0

static char*
getdelstr(f)
register struct fileinfo*	f;
{
	register char*	s;
	register char*	b = state.delta.hdrbuf;
	unsigned char	c;

	s = b;
	do
	{
		if (f->st->st_size <= 0) return(0);
		if (bread(&c, 1L, 1L, 1) <= 0)
			error(ERROR_SYSTEM|3, "%s: read error", f->path);
		f->st->st_size--;
	} while (*s++ = c);
	return(*b ? b : 0);
}

#endif


/*
 * get supplemental delta header info
 */

void
getdeltaheader(f)
register struct fileinfo*	f;
{
	register char*	s;

	f->delta.version = VERSION;
	f->delta.size = -1;
	if ((state.delta.op & (COLLECT|IN)) == IN)
	{
		if (state.delta.tab && (f->delta.base = (struct deltainfo*)hashget(state.delta.tab, f->path)))
			f->delta.base->mark = 1;
		f->delta.op = DELTA_create;
#if new_delta_format
		if (f->st->st_size && (s = getdelstr(f)))
		{
			if (f->delta.op = *s++) f->delta.version = *s;
			while (s = getdelstr(f)) switch (*s++)
			{
			case 's':
				f->delta.size = strtol(s, (char*)0, 16);
				break;
			/*
			 * NOTE: ignore unknown ops for future extensions
			 */
			}
		}
#else
		if (f->st->st_size)
		{
			char	c;

			f->delta.op = 0;
			do
			{
				if (bread(&c, 1L, 1L, 1) <= 0)
					error(ERROR_SYSTEM|3, "%s: read error", f->path);
				f->st->st_size--;
				if (!f->delta.op) f->delta.op = c;
			} while (c != '\n');
		}
#endif
	}
	else if (state.delta.op & CONVERT) f->delta.op = DELTA_pass;
	else f->delta.op = 0;
}

/*
 * add delta header string
 */

void
adddelstr(op, s)
int		op;
register char*	s;
{
	register char*	p = state.delta.hdr;
	register char*	e = state.delta.hdrbuf + state.buffersize - 3;

	if (p < e)
	{
		*p++ = op;
		while (*s && p < e) *p++ = *s++;
		*p++ = 0;
		state.delta.hdr = p;
	}
#if DEBUG
	if (*s) error(PANIC, "adddelstr('%c',\"%s\") overflow", op, s);
#endif
}

/*
 * add delta header number
 */

void
adddelnum(op, n)
int	op;
long	n;
{
	char	buf[17];

	(void)sprintf(buf, "%x", n);
	adddelstr(op, buf);
}

/*
 * output supplementary delta header info
 */

void
putdeltaheader(f)
register struct fileinfo*	f;
{
	register char*	s;
	int		n;

	if (f->delta.op && (n = state.delta.hdr - state.delta.hdrbuf))
	{
		switch (state.formatout)
		{
		case IBMAR:
			if (state.formatout == IBMAR) cvtatoe(state.delta.hdrbuf, state.delta.hdrbuf, n);
			/*FALLTHROUGH*/
		default:
			bwrite(state.delta.hdrbuf, n);
			break;
		}
		f->st->st_size -= n;
		f->linknamesize -= n;
		state.delta.hdr = state.delta.hdrbuf;
	}
}

/*
 * initialize delta tables
 */

void
initdelta()
{
	if (!(state.delta.tab = hashalloc((HASHTABLE*)0, HASH_set, HASH_ALLOCATE, HASH_name, "delta", 0)))
			error(3, "cannot allocate delta table");
}

/*
 * get delta base archive info
 */

void
deltabase()
{
	register char*	s;
	register int	fd;
	int		format;
	int		operation;
	struct stat	st;

	if (!state.delta.base) state.delta.base = "/dev/null";
	if ((state.delta.fd = open(state.delta.base, 0)) < 0 || fstat(state.delta.fd, &st))
		error(ERROR_SYSTEM|3, "%s: cannot open base archive", state.delta.base);
	initdelta();
	if (st.st_size)
	{
		operation = state.operation;
		state.operation = IN;
		fd = dup(0);
		(void)close(0);
		if (dup(state.delta.fd) != 0)
			error(ERROR_SYSTEM|3, "%s: cannot dup base archive", state.delta.base);
		if (lseek(0, 0L, 0) != 0)
			error(ERROR_SYSTEM|3, "%s: base archive must be seekable", state.delta.base);
		s = state.file;
		state.file = state.delta.base;
		format = state.formatin;
		state.formatin = IN_DEFAULT;
		if (state.delta.op)
		{
			state.delta.op |= COLLECT;
			state.delta.sum++;
		}
		copyin();
		if (state.delta.op)
		{
			state.delta.op &= ~COLLECT;
			state.delta.sum--;
		}
		state.formatin = format;
		state.file = s;
		(void)close(0);
		if (dup(fd) != 0)
			error(3, "cannot dup standard input");
		(void)close(fd);
		state.delta.size = state.in.offset + state.in.count;
		state.in.offset = 0;
		state.in.count = 0;
#if DEBUG
		state.in.blokflag = 0;
#endif
		state.in.eof = 0;
		state.volume = 0;
		state.operation = operation;
	}
	else state.delta.op |= COMPRESS;
}

/*
 * return read fd to delta base member d
 * some formats may require a tmp file copy
 */

int
deltafd(d)
register struct deltainfo*      d;
{
	register int	fd;

	if ((fd = dup(state.delta.fd)) < 0)
		error(ERROR_SYSTEM|3, "%s: cannot reopen", state.delta.base);
	if (lseek(fd, d->offset, 0) < 0)
		error(ERROR_SYSTEM|3, "%s: base archive seek error", state.delta.base);
	return(fd);
}

/*
 * verify untouched base files
 */

void
deltaverify()
{
	register int			wfd;
	register struct deltainfo*	d;
	HASHPOSITION			pos;

	if (!state.list && (state.delta.op & (COLLECT|IN)) == IN)
	{
#if DEBUG
		message(-2, "verify untouched base files");
#endif
		hashscan(state.delta.tab, 0, &pos);
		while (hashnext(&pos))
		{
			d = (struct deltainfo*)pos.bucket->value;
message(-1, "%s: mark=%d", d->info->name, d->mark);
			if (!d->mark && selectfile(d->info) && (wfd = openout(d->info)) >= 0)
			{
				state.entries++;
				filepass(d->info, deltafd(d), wfd);
			}
		}
		hashdone(&pos);
	}
}

/*
 * update file deltas from archive and output to archive
 */

void
deltapass()
{
	register struct fileinfo*	f;
	register long			c;
	register long			n;
	struct deltainfo*		d;
	char*				p;
	int				fd;
	HASHPOSITION			pos;

#if DEBUG
	message(-1, "delta PASS %s", operations());
#endif
	putprologue();
	f = &file;
	while (getprologue())
	{
		while (getheader(f))
		{
			switch (f->delta.op)
			{
			case DELTA_create:
				if (f->delta.base) error(3, "%s: base archive mismatch", f->name);
				/*FALLTHROUGH*/
			case DELTA_pass:
				if (validout(f) && selectfile(f))
				{
					if (state.delta.op & COMPRESS)
					{
						if (state.tmp.buffersize < f->st->st_size)
						{
							state.tmp.buffersize = round(f->st->st_size, state.blocksize);
							if (state.tmp.buffer) free(state.tmp.buffer);
							if (!(state.tmp.buffer = malloc(state.tmp.buffersize)))
								error(3, "input files too large to delta");
						}
						p = state.tmp.buffer;
						for (c = f->st->st_size; c > 0; c -= state.buffersize)
						{
							n = c > state.buffersize ? state.buffersize : c;
							if (bread(p, n, n, 1) <= 0)
							{
								error(ERROR_SYSTEM|2, "%s: read error", f->name);
								break;
							}
							p += n;
						}
						goto delta;
					}
					f->delta.op = 0;
					fileout(f, -1);
				}
				else fileskip(f);
				break;
			case DELTA_delete:
				if (!f->delta.base) error(3, "%s: base archive mismatch", f->name);
				break;
			case DELTA_update:
				if (!f->delta.base || f->delta.base->mtime >= f->st->st_mtime) error(3, "%s: base archive mismatch", f->name);
				if (validout(f) && selectfile(f))
				{
					if ((fd = creat(state.tmp.file, S_IRUSR|S_IRGRP|S_IROTH)) < 0)
						error(ERROR_SYSTEM|3, "%s: cannot create delta temporary file", state.tmp.file);
					if (doupdate(f, state.delta.fd, state.tmp.file, fd) < 0)
						error(ERROR_SYSTEM|2, "%s: delta update error", f->name);
					if ((fd = open(state.tmp.file, 0)) < 0)
						error(3, "%s: cannot read delta temporary file", state.tmp.file);
					if (remove(state.tmp.file))
						error(1, "%s: cannot remove delta temporary file", state.tmp.file);
					if (state.delta.op & COMPRESS)
					{
						if (state.tmp.buffersize < f->st->st_size)
						{
							state.tmp.buffersize = round(f->st->st_size, state.blocksize);
							if (state.tmp.buffer) free(state.tmp.buffer);
							if (!(state.tmp.buffer = malloc(state.tmp.buffersize)))
								error(3, "input files too large to delta");
						}
						if (f->st->st_size && read(fd, state.tmp.buffer, f->st->st_size) != f->st->st_size)
							error(ERROR_SYSTEM|3, "%s: read error", f->path);
						(void)close(fd);
					delta:
						f->delta.op = DELTA_create;
						fd = dodelta(f, state.delta.buffer, 0L, state.tmp.buffer);
					}
					else f->delta.op = 0;
					fileout(f, fd);
				}
				else fileskip(f);
				break;
			case DELTA_verify:
				if (!f->delta.base || f->delta.base->mtime != f->st->st_mtime) error(3, "%s: base archive mismatch", f->name);
				if (validout(f) && selectfile(f))
				{
					f->delta.op = 0;
					deltacopy(f);
				}
				else fileskip(f);
				break;
			default:
				error(3, "%s: not a delta archive (2)", state.file);
				break;
			}
			gettrailer(f);
		}
		getepilogue();
	}
	if (state.delta.tab)
	{
		/*
		 * copy the non-empty untouched base hard links first
		 */

#if DEBUG
		message(-2, "copy non-empty untouched base hard links");
#endif
		hashscan(state.delta.tab, 0, &pos);
		while (hashnext(&pos))
		{
			d = (struct deltainfo*)pos.bucket->value;
			if (!d->mark && d->info->st->st_nlink > 1 && d->info->st->st_size > 0 && selectfile(d->info))
			{
				d->mark = 1;
				deltacopy(d->info);
			}
		}
		hashdone(&pos);

		/*
		 * copy the remaining untouched base files
		 */

#if DEBUG
		message(-2, "copy remaining untouched base files");
#endif
		hashscan(state.delta.tab, 0, &pos);
		while (hashnext(&pos))
		{
			d = (struct deltainfo*)pos.bucket->value;
			if (!d->mark && selectfile(d->info))
			{
				state.entries++;
				deltacopy(d->info);
			}
		}
		hashdone(&pos);
	}
	putepilogue();
	state.volume = 0;
}

/*
 * copy file from input to output archive
 */

static void
deltacopy(f)
register struct fileinfo*	f;
{
	f->st->st_size = f->delta.base->size;
	fileout(f, deltafd(f->delta.base));
}

/*
 * copy delta base archive delete entries
 */

void
deltadelete()
{
	register struct fileinfo*	f;
	HASHPOSITION			pos;

	f = &file;
	hashscan(state.delta.tab, 0, &pos);
	while (hashnext(&pos))
	{
		if (!((struct deltainfo*)pos.bucket->value)->mark)
		{
			state.entries++;
			state.selected++;
			initfile(f, pos.bucket->name, S_IFREG);
			f->delta.op = DELTA_delete;
			putheader(f);
			puttrailer(f);
		}
	}
	hashdone(&pos);
}

/*
 * delta algorithm wrapper
 */

/*ARGSUSED*/
int
dodelta(f, old, oldsize, new)
struct fileinfo*	f;
char*			old;
long			oldsize;
char*			new;
{
	int	fd;

	if ((fd = creat(state.tmp.file, S_IRUSR)) < 0)
		error(3, "%s: cannot create delta temporary file", state.tmp.file);
	if (delta(old, oldsize, new, f->st->st_size, fd) < 0)
		error(3, "%s: delta write error", f->name);
	f->delta.size = f->st->st_size;
	if ((f->st->st_size = lseek(fd, 0L, 2)) < 0)
		error(3, "%s: delta seek error", f->name);
	(void)close(fd);
	if ((fd = open(state.tmp.file, 0)) < 0)
		error(3, "%s: cannot read delta temporary file", state.tmp.file);
	if (remove(state.tmp.file))
		error(1, "%s: cannot remove delta temporary file", state.tmp.file);
	return(fd);
}

/*
 * delta update algorithm wrapper
 */

/*ARGSUSED*/
int
doupdate(f, oldfd, file, wfd)
struct fileinfo*	f;
int			oldfd;
char*			file;
int			wfd;
{
	register long	c;
	register int	n;
	register int	v;
	int		rfd;

	switch (f->delta.version)
	{
	case VERSION:
		c = lseek(0, 0L, 1);
		if (lseek(0, state.in.count, 0) != state.in.count) return(-1);
		if ((rfd = open(file, 0)) < 0)
			error(ERROR_SYSTEM|3, "%s: cannot read delta temporary file", file);
		v = update(oldfd, f->delta.base->offset, 0, wfd, rfd, 0L);
		if ((n = lseek(wfd, 0L, 2)) < 0)
			error(ERROR_SYSTEM|3, "%s: update seek error", f->name);
		(void)close(rfd);
		(void)close(wfd);
		if (state.operation == IN) setfile(f);
		if (lseek(0, c, 0) != c || v < 0) return(-1);
		fileskip(f);
		f->st->st_size = n;
		return(0);
	default:
		(void)close(wfd);
		fileskip(f);
		error(2, "%s: version %c delta not supported", f->name, f->delta.version);
		return(-1);
	}
}
