/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax file support
 */

#include "pax.h"

extern char*	fmtuid();
extern char*	fmtgid();

/*
 * return read file descriptor for current input file
 */

int
openin(f)
register struct fileinfo*	f;
{
	register int	n;
	int		rfd;
	int		wfd;
	int		pfd;
	char*		cmdargv[3];

	extern int	cmdopen();
	extern int	cmdclose();

	if (f->type != S_IFREG) return(-1);
	if (!state.filter)
	{
		if ((rfd = open(f->path, 0)) < 0) error(ERROR_SYSTEM|2, "%s: cannot read", f->path);
		else if (state.formatout == ASCHK)
		{
			f->checksum = 0;
			if (lseek(rfd, 0L, 0) != 0) error(ERROR_SYSTEM|1, "%s: %s checksum seek error", f->path, format[state.formatout].name);
			else
			{
				while ((n = read(rfd, state.tmp.buffer, state.buffersize)) > 0)
					f->checksum = asc_checksum(state.tmp.buffer, n, f->checksum);
				if (n < 0) error(ERROR_SYSTEM|2, "%s: %s checksum read error", f->path, format[state.formatout].name);
				if (lseek(rfd, 0L, 0) != 0) error(ERROR_SYSTEM|1, "%s: %s checksum seek error", f->path, format[state.formatout].name);
			}
		}
		return(rfd);
	}
	if ((wfd = creat(state.tmp.file, S_IRUSR)) < 0)
	{
		error(2, "%s: cannot create filter temporary %s", f->path, state.tmp.file);
		return(-1);
	}
	if ((rfd = open(state.tmp.file, 0)) < 0)
	{
		error(2, "%s: cannot open filter temporary %s", f->path, state.tmp.file);
		(void)close(wfd);
		if (remove(state.tmp.file)) error(1, "%s: cannot remove filter temporary %s", f->path, state.tmp.file);
		return(-1);
	}
	if (remove(state.tmp.file)) error(1, "%s: cannot remove filter temporary %s", f->path, state.tmp.file);
	cmdargv[0] = state.filter;
	cmdargv[1] = f->path;
	cmdargv[2] = 0;
	if ((pfd = cmdopen(state.filter, cmdargv, (char**)0, (int*)0, "r")) < 0)
	{
		error(2, "%s: cannot execute filter %s", f->path, state.filter);
		(void)close(rfd);
		(void)close(wfd);
		return(-1);
	}
	if (state.formatout == ASCHK) f->checksum = 0;
	f->st->st_size = 0;
	while ((n = read(pfd, state.tmp.buffer, state.buffersize)) > 0)
	{
		if (write(wfd, state.tmp.buffer, n) != n)
		{
			error(2, "%s: filter write error", f->path);
			break;
		}
		if (state.formatout == ASCHK) f->checksum = asc_checksum(state.tmp.buffer, n, f->checksum);
		f->st->st_size += n;
	}
	if (n < 0) error(ERROR_SYSTEM|2, "%s: %s filter read error", f->path, state.filter);
	if (n = cmdclose(pfd)) error(2, "%s: %s filter exit code %d", f->path, state.filter, n);
	(void)close(wfd);
#if DEBUG
	message(-1, "%s: filter file size = %ld", f->path, f->st->st_size);
#endif
	return(rfd);
}

/*
 * open file for writing, set all necessary info
 */

int
openout(f)
register struct fileinfo*	f;
{
	register int	fd;
	struct stat	st;

	(void)pathcanon(f->name);
	if ((*state.statf)(f->name, &st)) st.st_mode = 0;
	if (f->delta.op == DELTA_delete)
	{
		switch (st.st_mode & S_IFMT)
		{
		case 0:
			break;
		case S_IFDIR:
			if (!streq(f->name, ".") && !streq(f->name, "..") && rmdir(f->name))
				error(ERROR_SYSTEM|2, "%s: cannot remove directory", f->name);
			break;
		default:
			if (remove(f->name))
				error(ERROR_SYSTEM|2, "%s: cannot remove file", f->name);
			break;
		}
		return(-1);
	}
	if (state.operation == (IN|OUT))
	{
		if (st.st_mode && f->st->st_ino == st.st_ino && f->st->st_dev == st.st_dev)
		{
			error(2, "attempt to pass %s to self", f->name);
			return(-1);
		}
		if (state.linkf && f->type != S_IFDIR && (state.linkf == putsymlink || f->st->st_dev == state.dev))
		{
			if (st.st_mode) remove(f->name);
			if ((*state.linkf)(f->path, f->name))
			{
				if (!st.st_mode && missdir(f))
				{
					error(ERROR_SYSTEM|2, "%s: cannot create intermediate directories", f->name);
					return(-1);
				}
				if (st.st_mode || (*state.linkf)(f->path, f->name))
				{
					error(ERROR_SYSTEM|2, "%s: cannot link to %s", f->path, f->name);
					return(-1);
				}
			}
			setfile(f);
			return(-2);
		}
	}
	if ((state.delta.op & IN) && st.st_mode == f->st->st_mode && f->st->st_mtime == st.st_mtime && f->st->st_size == st.st_size) return(-1);
	switch (f->type)
	{
	case S_IFDIR:
		f->st->st_size = 0;
		if (f->name[0] == '.' && (f->name[1] == 0 || f->name[1] == '.' && f->name[2] == 0)) return(-1);
		if (st.st_mode)
		{
			if ((st.st_mode & S_IFMT) != S_IFDIR)
			{
				error(1, "current %s is not a directory", f->name);
				return(-1);
			}
		}
		else if (mkdir(f->name, f->st->st_mode) && (missdir(f) || mkdir(f->name, f->st->st_mode)))
		{
			error(ERROR_SYSTEM|2, "%s: cannot create directory", f->name);
			return(-1);
		}
		setfile(f);
		return(state.update && st.st_mode ? -1 : -2);
	case S_IFLNK:
		if (streq(f->name, f->linkname))
		{
			error(1, "%s: symbolic link loops to self", f->name);
			return(-1);
		}
		if (st.st_mode)
		{
			if (state.update && f->st->st_mtime <= st.st_mtime) return(-1);
			if (remove(f->name))
			{
				error(ERROR_SYSTEM|2, "cannot remove current %s", f->name);
				return(-1);
			}
		}
		if (putsymlink(f->linkname, f->name))
		{
			if (!st.st_mode && missdir(f))
			{
				error(ERROR_SYSTEM|2, "%s: cannot create intermediate directories", f->name);
				return(-1);
			}
			if (st.st_mode || putsymlink(f->linkname, f->name))
			{
				error(ERROR_SYSTEM|2, "%s: cannot symlink to %s", f->name, f->linkname);
				return(-1);
			}
		}
		return(-2);
	}
	if (!addlink(f)) return(-1);
	if (st.st_mode)
	{
		if (state.update && f->st->st_mtime <= st.st_mtime)
		{
			if (f->st->st_mtime < st.st_mtime) error(1, "current %s is newer", f->name);
			return(-1);
		}
		if (remove(f->name))
		{
			error(ERROR_SYSTEM|2, "cannot remove current %s", f->name);
			return(-1);
		}
	}
	switch (f->type)
	{
	case S_IFIFO:
	case S_IFSOCK:
		f->st->st_rdev = 0;
	case S_IFBLK:
	case S_IFCHR:
		f->st->st_size = 0;
		if (mknod(f->name, f->st->st_mode, f->st->st_rdev))
		{
			if (errno == EPERM)
			{
				error(ERROR_SYSTEM|2, "%s: cannot create %s special file", f->name, (f->type == S_IFBLK) ? "block" : "character");
				return(-1);
			}
			if (!st.st_mode && missdir(f))
			{
				error(ERROR_SYSTEM|2, "%s: cannot create intermediate directories", f->name);
				return(-1);
			}
			if (st.st_mode || mknod(f->name, f->st->st_mode, f->st->st_rdev))
			{
				error(ERROR_SYSTEM|2, "%s: cannot mknod", f->name);
				return(-1);
			}
		}
		setfile(f);
		return(-2);
	default:
		error(1, "%s: unknown file type 0%03o -- creating regular file", f->name, f->type >> 12);
		/*FALLTHROUGH*/
	case S_IFREG:
		if ((fd = creat(f->name, f->st->st_mode)) < 0)
		{
			if (!st.st_mode && missdir(f))
			{
				error(ERROR_SYSTEM|2, "%s: cannot create intermediate directories", f->name);
				return(-1);
			}
			if (st.st_mode || (fd = creat(f->name, f->st->st_mode)) < 0)
			{
				error(ERROR_SYSTEM|2, "%s: cannot create", f->name);
				return(-1);
			}
		}
		return(fd);
	}
}

/*
 * get file info for output
 */

int
getfile(f, ftw)
register struct fileinfo*	f;
register struct FTW*		ftw;
{
	register char*		name;

	static struct stat	st;
	static char		pathbuffer[PATH_MAX];
	static char		namebuffer[PATH_MAX];

	name = ftw->path;
#if DEBUG
	message(-4, "getfile(%s)", name);
#endif
	if (ftw->pathlen >= sizeof(namebuffer))
	{
		error(2, "%s: file name too long", name);
		return(0);
	}
	switch (ftw->info)
	{
	case FTW_NS:
		error(2, "%s: not found", name);
		return(0);
	case FTW_DNR:
		if (state.files) error(2, "%s: cannot read directory", name);
		break;
	case FTW_D:
	case FTW_DNX:
		if (!state.files) ftw->status = FTW_SKIP;
		else if (ftw->info == FTW_DNX)
		{
			error(2, "%s: cannot search directory", name);
			ftw->status = FTW_SKIP;
		}
		break;
	}
	if (state.xdev && ftw->statb.st_dev != ftw->parent->statb.st_dev)
	{
		ftw->status = FTW_SKIP;
		return(0);
	}
	state.delta.hdr = state.delta.hdrbuf;
	f->path = strcpy(pathbuffer, name);
	(void)pathcanon(strcpy(namebuffer, name));
	f->name = map(namebuffer);
	if (state.files && state.operation == (IN|OUT) && dirprefix(state.destination, name)) return(0);
	f->namesize = strlen(f->name) + 1;
/* this may not be necessary */
	st = ftw->statb;
	f->st = &st;
	f->type = f->st->st_mode & S_IFMT;
	f->linktype = NOLINK;
	f->linkname = 0;
	f->linknamesize = 0;
	f->uidname = 0;
	f->gidname = 0;
	if (!validout(f)) return(0);
	if (state.operation == OUT && f->type != S_IFDIR)
	{
		if (!addlink(f)) f->st->st_size = 0;
#if DEBUG
		message(-3, "getfile(%s): dev'=%d ino'=%d", f->name, f->st->st_dev, f->st->st_ino);
#endif
	}
	state.entries++;
	f->delta.op = 0;
	f->skip = 0;
#if DEBUG
	message(-2, "getfile(): path=%s name=%s mode=%s size=%ld", name, f->name, fmtmode(f->st->st_mode), f->st->st_size);
#endif
	return(1);
}

/*
 * check that f is valid for archive output
 */

int
validout(f)
register struct fileinfo*	f;
{
	register char*	s;

	static char	linkbuffer[PATH_MAX];
	static char	idbuffer[ALAR_NAMESIZE + 1];

	switch (f->type)
	{
	case S_IFCHR:
	case S_IFBLK:
		f->st->st_size = 0;
		break;
	case S_IFREG:
		f->st->st_rdev = 0;
		break;
	case S_IFLNK:
		if (f->st->st_size >= sizeof(linkbuffer) - 1)
		{
			error(2, "%s: symbolic link too long", f->path);
			return(0);
		}
		if (readlink(f->path, linkbuffer, sizeof(linkbuffer) - 1) != f->st->st_size)
		{
			error(2, "%s: cannot read symbolic link", f->path);
			return(0);
		}
		linkbuffer[f->st->st_size] = 0;
		f->linktype = SOFTLINK;
		(void)pathcanon(linkbuffer);
		f->linkname = (state.ftwflags & FTW_PHYSICAL) ? linkbuffer : map(linkbuffer);
		f->linknamesize = strlen(f->linkname) + 1;
		if (streq(f->path, f->linkname))
		{
			error(2, "%s: symbolic link loops to self", f->path);
			return(0);
		}
		f->st->st_size = 0;
		f->st->st_rdev = 0;
		break;
	case S_IFDIR:
		if (streq(f->path, ".") || streq(f->path, "..")) return(0);
		f->st->st_size = 0;
		f->st->st_rdev = 0;
		break;
	}
	switch (state.formatout)
	{
	case ALAR:
	case IBMAR:
	case SAVESET:
		if (f->type != S_IFREG)
		{
			error(2, "%s: only regular files copied in %s format", f->path, format[state.formatout].name);
			return(0);
		}
		if (s = strrchr(f->name, '/'))
		{
			s++;
			error(1, "%s: file name stripped to %s", f->name, s);
		}
		else s = f->name;
		if (strlen(s) > sizeof(idbuffer) - 1)
		{
			error(2, "%s: file name too long", f->name);
			return(0);
		}
		f->id = strupper(strcpy(idbuffer, s));
		break;
	case BINARY:
		if (f->namesize > BINARY_NAMESIZE)
		{
			error(2, "%s: file name too long", f->name);
			return(0);
		}
		break;
	case PAX:
		if (s = strrchr(f->name, '/')) s++;
		else s = f->name;
		f->id = strupper(strncpy(idbuffer, s, sizeof(idbuffer) - 1));
		break;
	case TAR:
	case USTAR:
		if (f->namesize > sizeof(tar_header.name) + ((state.formatout == TAR) ? -(f->type == S_IFDIR) : sizeof(tar_header.prefix)))
		{
			error(2, "%s: file name too long", f->name);
			return(0);
		}
		if (f->linknamesize > sizeof(tar_header.linkname))
		{
			error(2, "%s: link name too long", f->name);
			return(0);
		}
		break;
	}
	return(1);
}

/*
 * add file which may be a link
 * 0 returned if <dev,ino> already added
 */

int
addlink(f)
register struct fileinfo*	f;
{
	register struct linkinfo*	p;
	register char*			s;
	int				fmt;
	struct fileid			id;
	unsigned short			us;

	static int			warned;

	id.dev = f->st->st_dev;
	id.ino = f->st->st_ino;
	if (!(state.delta.op & (IN|OUT))) switch (state.operation)
	{
	case IN:
		us = id.dev;
		if (us > state.devcnt)
		{
			state.devcnt = us;
			state.inocnt = id.ino;
		}
		else if (us == state.devcnt)
		{
			us = id.ino;
			if (us > state.inocnt) state.inocnt = us;
		}
		break;
	case OUT:
		if (!++state.inocnt)
		{
			if (!++state.devcnt) goto toomany;
			state.inocnt = 1;
		}
		f->st->st_dev = state.devcnt;
		f->st->st_ino = state.inocnt;
		break;
	}
	if (f->type == S_IFDIR) return(0);
	fmt = state.operation == IN ? state.formatin : state.formatout;
	switch (fmt)
	{
	case ALAR:
	case IBMAR:
	case SAVESET:
		if (state.operation == IN || f->st->st_nlink <= 1) return(1);
		break;
	case TAR:
	case USTAR:
		if (state.operation == IN)
		{
			if (f->linktype == NOLINK) return(1);
			goto linked;
		}
		/*FALLTHROUGH*/
	default:
		if (f->st->st_nlink <= 1) return(1);
		break;
	}
	if (p = (struct linkinfo*)hashget(state.linktab, (char*)&id))
	{
		switch (fmt)
		{
		case ALAR:
		case IBMAR:
		case SAVESET:
			error(1, "%s: hard link information lost in %s format", f->name, format[fmt].name);
			return(1);
		}
		f->st->st_dev = p->id.dev;
		f->st->st_ino = p->id.ino;
		f->linktype = HARDLINK;
		f->linkname = p->name;
		f->linknamesize = p->namesize;
		if (state.operation == OUT) return(0);
	linked:
#if DEBUG
		message(-1, "addlink(%s,%s)", f->name, f->linkname);
#endif
		if (streq(f->name, f->linkname))
		{
			error(2, "%s: hard link loops to self", f->name);
			return(0);
		}
		if (!state.list)
		{
			s = f->linkname;
			if (access(s, 0))
			{
				f->skip = 1;
				error(2, "%s must exist for hard link %s", s, f->name);
				return(0);
			}
			remove(f->name);
			if (state.operation == IN && *s != '/')
			{
				(void)strcpy(state.pwd + state.pwdlen, s);
				s = state.pwd;
			}
			if (link(s, f->name))
			{
				if (missdir(f))
				{
					error(ERROR_SYSTEM|2, "%s: cannot create intermediate directories", f->name);
					return(0);
				}
				if (link(s, f->name))
				{
					error(ERROR_SYSTEM|2, "%s: cannot link to %s", f->linkname, f->name);
					return(-1);
				}
			}
		}
		return(0);
	}
	if (!(p = allocate(struct linkinfo)) || !(p->name = strdup(f->name))) goto toomany;
	p->namesize = strlen(p->name) + 1;
	p->id.dev = f->st->st_dev;
	p->id.ino = f->st->st_ino;
	(void)hashput(state.linktab, (char*)0, p);
	return(-1);
 toomany:
	if (!warned)
	{
		warned = 1;
		error(1, "too many hard links -- some links may become copies");
	}
	return(-1);
}

/*
 * get file uid and gid names given numbers
 */

void
getidnames(f)
register struct fileinfo*	f;
{
	if (!f->uidname) f->uidname = fmtuid(f->st->st_uid);
	if (!f->gidname) f->gidname = fmtgid(f->st->st_gid);
}

/*
 * set file uid and gid numbers given names
 */

void
setidnames(f)
register struct fileinfo*	f;
{
	register int	id;

	if (f->uidname)
	{
		if ((id = struid(f->uidname)) < 0)
		{
			if (id == -1 && state.owner) error(1, "%s: invalid user name", f->uidname);
			f->uidname = 0;
			id = state.uid;
		}
		f->st->st_uid = id;
	}
	if (f->gidname)
	{
		if ((id = strgid(f->gidname)) < 0)
		{
			if (id == -1 && state.owner) error(1, "%s: invalid group name", f->gidname);
			f->gidname = 0;
			id = state.gid;
		}
		f->st->st_gid = id;
	}
}

/*
 * initialize file info with name and mode
 */

void
initfile(f, name, mode)
register struct fileinfo*	f;
register char*			name;
int				mode;
{
	static struct stat	st;

	(void)memset(f, 0, sizeof(*f));
	(void)memset(&st, 0, sizeof(st));
	f->st = &st;
	if (name)
	{
		f->id = f->name = f->path = name;
		f->namesize = strlen(name) + 1;
	}
	f->st->st_mode = mode;
	f->st->st_nlink = 1;		/* system V needs this!!! */
}

/*
 * set copied file info
 */

void
setfile(f)
register struct fileinfo*	f;
{
	register struct postinfo*	p;
	struct postinfo			post;

	if (!(state.delta.op & OUT)) switch (f->type)
	{
	case S_IFLNK:
		break;
	case S_IFDIR:
		if (state.modtime || state.owner || (f->st->st_mode & (S_IWUSR|S_IXUSR)) != (S_IWUSR|S_IXUSR))
		{
			if (!(p = allocate(struct postinfo))) error(3, "not enough space for file restoration info");
			p->mtime = f->st->st_mtime;
			p->uid = f->st->st_uid;
			p->gid = f->st->st_gid;
			p->mode = f->st->st_mode;
			if ((f->st->st_mode & S_IRWXU) != S_IRWXU && chmod(f->name, f->st->st_mode|S_IRWXU))
				error(1, "%s: cannot chmod to %s", f->name, fmtmode(f->st->st_mode|S_IRWXU) + 1);
			(void)hashput(state.restore, f->name, p);
			break;
		}
		/*FALLTHROUGH*/
	default:
		p = &post;
		p->mtime = f->st->st_mtime;
		p->uid = f->st->st_uid;
		p->gid = f->st->st_gid;
		p->mode = f->st->st_mode;
		(void)restore(f->name, p);
		break;
	}
}

/*
 * set access and modification times of file
 */

void
settime(name, atime, mtime)
char*	name;
time_t	atime;
time_t	mtime;
{
	if (touch(name, atime, mtime, 0)) error(1, "%s: cannot set times", name);
}

/*
 * create directory and all path name components leading to directory
 */

int
missdir(f)
register struct fileinfo*	f;
{
	register char*	s;
	register char*	t;
	struct stat*	st;
	struct stat*	sp;
	struct stat	st0;
	struct stat	st1;

	s = f->name;
	(void)pathcanon(s);
	if (t = strchr(*s == '/' ? s + 1 : s, '/'))
	{
		if (!state.intermediate)
		{
			static int	warned;

			if (!warned)
			{
				error(1, "omit the -d option to create intermediate directories");
				warned = 1;
			}
			return(-1);
		}
		st = 0;
		sp = &st0;
		do
		{
			*t = 0;
			if (stat(s, sp))
			{
				*t = '/';
				break;
			}
			*t = '/';
			st = sp;
			sp = (sp == &st0) ? &st1 : &st0;
		} while (t = strchr(t + 1, '/'));
		if (t)
		{
			if (!st && stat(".", st = &st0))
				error(ERROR_SYSTEM|3, "%s: cannot stat .", s);
			sp = f->st;
			f->st = st;
			do
			{
				*t = 0;
				if (mkdir(s, st->st_mode & state.modemask))
				{
					error(ERROR_SYSTEM|2, "%s: cannot create directory", s);
					*t = '/';
					f->st = sp;
					return(-1);
				}
				setfile(f);
				*t = '/';
			} while (t = strchr(t + 1, '/'));
			f->st = sp;
		}
	}
	return(0);
}

/*
 * restore file status after processing
 */

int
restore(name, p)
register char*			name;
register struct postinfo*	p;
{
	int		m;
	struct stat	st;

	if (state.owner)
	{
		if (state.flags & SETIDS)
		{
			p->uid = state.setuid;
			p->gid = state.setgid;
		}
		if (chown(name, p->uid, p->gid) < 0)
			error(1, "%s: cannot chown to (%d,%d)", name, p->uid, p->gid);
	}
	if (p->mode != S_IFMT)
	{
		if (chmod(name, p->mode & state.modemask))
			error(1, "%s: cannot chmod to %s", name, fmtmode(p->mode & state.modemask) + 1);
		else if (m = p->mode & (S_ISUID|S_ISGID|S_ISVTX))
		{
			if (stat(name, &st))
				error(1, "%s: not found", name);
			else if (m ^= (st.st_mode & (S_ISUID|S_ISGID|S_ISVTX)))
				error(1, "%s: mode %s not set", name, fmtmode(m) + 1);
		}
	}
	if (state.modtime) settime(name, p->mtime, p->mtime);
	return(0);
}
