/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax block io support
 */

#include "pax.h"

#include "FEATURE/mtio"
#ifdef _sys_mtio
#include <ttyinfo.h>
#include <sys/mtio.h>
#endif

#include "FEATURE/bcopy"
#ifdef _lib_bcopy
#define memcpy(a,b,c)	bcopy(b,a,c)
#endif

#if DEBUG

/*
 * -RBi		input is BLOK file
 * -RBo		output file is BLOK file
 */

static int
blokread(fd, buf, n)
int	fd;
char*	buf;
int	n;
{
	register int		i;
	register int		j;
	char			c;

	static int		eof;

	if (!state.in.blokflag)
	{
		state.in.blokflag = 1;
		eof = 0;
		if ((i = read(fd, buf, state.in.blok ? 4 : n)) < 4 || !strneq(buf, "\002\014\017\013", 4))
		{
			if (state.in.blok) error(3, "input archive is not a BLOK file");
			return(i);
		}
		if (i > 4 && lseek(fd, 4L, 0) != 4L)
			error(3, "cannot seek on input archive BLOK file -- use -RBi");
		state.in.blok = 1;
	}
	if (state.in.blok)
	{
		j = 0;
		do
		{
			if ((i = read(fd, &c, 1)) < 1)
				return(i < 0 && ++eof == 1 ? 0 : -1);
			j <<= 7;
			j |= c & 0177;
		} while (c & 0200);
		if (j > 0)
		{
			if (j > n) error(2, "blokread: buffer overflow (%d>%d)", j, n);
			if ((i = read(fd, buf, j)) != j) error(2, "blokread: blocking error");
		}
		else i = 0;
	}
	else i = read(fd, buf, n);
	return(i);
}

static int
blokwrite(fd, buf, n)
int	fd;
char*	buf;
int	n;
{
	register char*	s;
	register int	i;
	register int	j;
	char		blk[9];

	if (state.out.blok)
	{
		s = blk;
		if (!state.out.blokflag)
		{
			state.out.blokflag = 1;
			*s++ = 002;
			*s++ = 014;
			*s++ = 017;
			*s++ = 013;
		}
		i = 0;
		if (j = (n >> 21) & 0177)
		{
			*s++ = j | 0200;
			i = 1;
		}
		if ((j = (n >> 14) & 0177) || i)
		{
			*s++ = j | 0200;
			i = 1;
		}
		if ((j = (n >> 7) & 0177) || i)
		{
			*s++ = j | 0200;
			i = 1;
		}
		*s++ = n & 0177;
		j = s - blk;
		if ((i = write(fd, blk, j)) != j)
			error(ERROR_SYSTEM|3, "blokwrite: count write error (%d!=%d)", i, j);
		if (n <= 0) i = n;
		else if ((i = write(fd, buf, n)) != n)
			error(ERROR_SYSTEM|3, "blokwrite: data write error (%d!=%d", i, n);
	}
	else i = write(fd, buf, n);
	return(i);
}

#define read(f,b,n)	blokread(f,b,n)
#define write(f,b,n)	blokwrite(f,b,n)

#endif

static int		bfill();

/*
 * initialize buffered io
 */

void
binit()
{
	struct stat		st;

	state.buffersize = DEFBUFFER * DEFBLOCKS;
	if (state.operation == OUT)
	{
		if (!state.blocksize)
		{
			if (fstat(1, &st)) error(3, "%s: cannot stat", state.file);
#if DEBUG
			if (state.test & 040) st.st_mode = S_IFCHR;
#endif
			if ((st.st_mode & S_IFMT) == S_IFREG)
			{
				state.blocksize = format[state.formatout].regular;
				state.out.unblocked = 1;
			}
			else state.blocksize = format[state.formatout].special;
			state.buffersize = state.blocksize *= BLOCKSIZE;
		}
		switch (state.formatout)
		{
		case ALAR:
		case IBMAR:
		case PAX:
			if (!state.record.format) state.record.format = state.formatout == PAX ? PAX_RECFORMAT : ALAR_RECFORMAT;
			if (state.record.size <= 0) switch (state.record.format)
			{
			case 'D':
			case 'U':
				state.record.size = state.blocksize;
				break;
			case 'F':
				state.record.size = state.formatout == PAX ? PAX_RECSIZE : ALAR_RECSIZE;
				break;
			case 'S':
			case 'V':
				state.record.size = 0;
				break;
			default:
				error(3, "%s %c record format not supported on output", format[state.formatout].name, state.record.format);
			}
			if (state.blocksize < state.record.size)
				error(3, "block size (%d) must be >= record size (%d)", state.blocksize, state.record.size);
			if (state.record.size && (state.blocksize / state.record.size) * state.record.size != state.blocksize)
				error(1, "block size should be a multiple of record size");
			break;
		default:
			if (state.record.format || state.record.size)
				error(1, "record format and size ignored for %s format", format[state.formatout].name);
			break;
		}
	}
	else
	{
		if (state.blocksize) state.buffersize = state.blocksize;
		else state.blocksize = DEFBLOCKS * BLOCKSIZE;
		if (state.record.size) error(1, "record size automatically determined on archive read");
	}
	if (state.buffersize < state.blocksize) state.buffersize = state.blocksize;
	state.tmp.buffersize = state.buffersize;
	if (!(state.tmp.buffer = malloc(state.tmp.buffersize)))
		error(3, "cannot allocate buffer");
	if (state.delta.op)
	{
		state.delta.buffersize = state.buffersize;
		if (!(state.delta.buffer = malloc(state.delta.buffersize)) || !(state.delta.hdr = state.delta.hdrbuf = malloc(state.delta.buffersize)))
			error(3, "cannot allocate buffer");
	}
	switch (state.operation)
	{
	case OUT:
		if (!(format[state.formatout].flags & IN)) error(3, "%s: archive format not supported on output" , format[state.formatout].name);
		balloc(&state.out, 2 * state.buffersize);
		if (!state.append && !state.delta.op) break;
		/*FALLTHROUGH*/
	case IN:
		if (!(format[state.formatout].flags & OUT)) error(3, "%s: archive format not supported on input" , format[state.formatout].name);
		balloc(&state.in, 2 * state.buffersize + MAXUNREAD);
		break;
	case IN|OUT:
		break;
	}
#if DEBUG
	message(-1, "blocksize=%d buffersize=%d recordsize=%d", state.blocksize, state.buffersize, state.record.size);
#endif
}

/*
 * allocate buffers for buffered io stream
 */

void
balloc(io, n)
register struct bio*	io;
register int		n;
{
	if (!(io->buffer = malloc(n)))
		error(3, "cannot allocate buffer");
	io->next = io->last = io->buffer;
}

/*
 * buffered char input
 * at least n chars required, m chars max
 * if b is 0 then n chars are skipped
 * if must!=0 then EOF causes query for next input volume file
 */

int
bread(b, n, m, must)
register char*	b;
register long	n;
long		m;
int		must;
{
	register int	c;
	register char*	ob;

	if (state.in.eof) return(-1);
	if (m <= 0) return(0);
	ob = b;
	if (state.in.blocked)
	{
		if (!b) b = state.tmp.buffer;
		while ((c = read(state.append, b, m)) <= 0)
		{
			if (must) newio(state.append, c, 0);
			else if (state.in.empty)
			{
				state.in.eof = 1;
				return(-1);
			}
			else
			{
				if (c < 0) state.in.eof = 1;
				else
				{
					state.in.empty = 1;
#if DEBUG
					if (ob) message(-7, "bread(%d@%ld):", c, state.in.count);
#endif
				}
				return(c);
			}
		}
		state.in.empty = 0;
		if (must && c < n) return(-1);
		state.in.count += c;
		if (state.delta.sum > 0) state.delta.membersum = memsum(ob ? ob : state.tmp.buffer, c, state.delta.membersum);
#if DEBUG
		if (ob) message(-7, "bread(%d@%ld): %-.*s%s", c, state.in.count, c > 32 ? 32 : c, ob, c > 32 ? "..." : "");
#endif
		return(c);
	}
	else
	{
		if (n <= 0) n = m;
		else m = n;
		for (;;)
		{
			if (n > (c = state.in.last - state.in.next))
			{
				if (c > 0)
				{
					if (state.delta.sum > 0) state.delta.membersum = memsum(state.in.next, c, state.delta.membersum);
					if (ob) memcpy(b, state.in.next, c);
					b += c;
					n -= c;
					state.in.count += c;
				}
				state.in.next = state.in.last = state.in.buffer + MAXUNREAD;
				if (!ob && state.delta.sum <= 0)
				{
					static int	seekok = -1;

					if (seekok < 0)
					{
						struct stat	st;

						seekok = lseek(state.append, 0L, 1) == (state.in.count + state.in.last - state.in.next) && fstat(state.append, &st) != -1 && st.st_size > 0;
					}
					if (seekok && (c = n / BUFFERSIZE) && lseek(state.append, (long)(c *= BUFFERSIZE), 1) != -1)
					{
						b += c;
						n -= c;
						state.in.count += c;
#if DEBUG
						message(-7, "bread: skip(%d@%ld) [%ld]", c, state.in.count);
#endif
					}
				}
				if (bfill(must) < 0)
				{
					if (ob && (c = (b - ob)))
					{
						bunread(ob, c);
						return(0);
					}
					return(-1);
				}
			}
			else
			{
				if (state.delta.sum > 0) state.delta.membersum = memsum(state.in.next, n, state.delta.membersum);
				if (ob) memcpy(b, state.in.next, n);
				state.in.next += n;
				state.in.count += n;
#if DEBUG
				if (ob)
				{
					n += b - ob;
					message(-7, "bread(%d@%ld): %-.*s%s", n, state.in.count, n > 32 ? 32 : n, ob, n > 32 ? "..." : "");
				}
#endif
				return(m);
			}
		}
	}
}

/*
 * fill input buffer at state.in.last
 * if must!=0 then EOF causes query for next input volume file
 */

static int
bfill(must)
int	must;
{
	register int	c;

	if (state.in.eof) return(-1);
	while ((c = read(state.append, state.in.last, state.buffersize)) <= 0)
	{
		if (must) newio(state.append, c, 0);
		else
		{
			state.in.eof = 1;
			return(-1);
		}
	}
#if DEBUG
	message(-8, "read(%d): %-.*s%s", c, c > 32 ? 32 : c, state.in.last, c > 32 ? "..." : "");
#endif
	state.in.eof = 0;
	state.in.last += c;
	return(0);
}

/*
 * pushback for next bread()
 */

void
bunread(b, n)
register char*	b;
register int	n;
{
	state.in.count -= n;
	if ((state.in.next -= n) < state.in.buffer + MAXUNREAD)
	{
		if (state.in.next < state.in.buffer) error(PANIC, "bunread(%d): too much pushback", n);
		memcpy(state.in.next, b, n);
	}
#if DEBUG
	message(-7, "bunread(%d@%ld): %-.*s%s", n, state.in.count, n > 32 ? 32 : n, b, n > 32 ? "..." : "");
#endif
}

/*
 * bread() n chars and return a pointer to the char buffer
 */

char*
bget(n)
register int	n;
{
	register char*	s;

	state.in.count += n;
	s = state.in.next;
	state.in.next += n;
	while (state.in.next > state.in.last)
	{
		if (state.in.last > state.in.buffer + MAXUNREAD + state.buffersize)
		{
                        register char*  b;
                        register int    k;
                        register int    m;

                        k = state.in.last - s;
			m = round(k, IOALIGN) - k;
#if DEBUG
			if (m) message(-8, "bget: buffer alignment offset=%d", m);
#endif
                        b = state.in.next = state.in.buffer + MAXUNREAD + m;
                        state.in.last = b + k;
                        m = s - b;
                        while (k > m)
                        {
#if DEBUG
                                message(-8, "bget: overlapping memcpy n=%d k=%d m=%d", n, k, m);
#endif
                                (void)memcpy(b, s, m);
                                b += m;
                                s += m;
                                k -= m;
                        }
                        (void)memcpy(b, s, k);
			s = state.in.next;
			state.in.next += n;
		}
		if (bfill(1) < 0) return(0);
	}
#if DEBUG
	message(-7, "bget(%d@%ld): %-.*s%s", n, state.in.count, n > 32 ? 32 : n, s, n > 32 ? "..." : "");
#endif
	return(s);
}

/*
 * back up input to bsave()'d position and prime output buffer
 */

void
backup()
{
	register long	n;
	register long	m;
#ifdef MTIOCTOP
	struct mtop	mt;
#endif

	switch (state.formatin)
	{
	case ALAR:
	case IBMAR:
	case PAX:
#ifdef MTIOCTOP
		mt.mt_op = MTBSF;
		mt.mt_count = 1;
		if (!(ioctl(1, MTIOCTOP, &mt))) return;
#endif
		break;
	default:
		m = state.in.next - (state.in.buffer + MAXUNREAD);
		if ((n = state.in.count - m) > state.backup.count)
		{
#if DEBUG
			message(-1, "backup(): reread %ld", n + m);
#endif
			m = state.backup.last - (state.backup.buffer + MAXUNREAD);
			if (lseek(1, -(n + m), 1) == -1L)
			{
#ifdef MTIOCTOP
				mt.mt_op = MTBSR;
				mt.mt_count = 2;
				if (ioctl(1, MTIOCTOP, &mt)) break;
#else
				break;
#endif
			}
			if (read(1, state.in.buffer + MAXUNREAD, m) != m) break;
		}
		else m = state.in.last - (state.in.buffer + MAXUNREAD);
#if DEBUG
		message(-1, "backup(): %ld", m);
#endif
		if ((m = lseek(1, -m, 1)) == -1L)
		{
#ifdef MTIOCTOP
			mt.mt_op = MTBSR;
			mt.mt_count = 1;
			if (ioctl(1, MTIOCTOP, &mt)) break;
#else
			break;
#endif
		}
		if (state.backup.next < state.backup.last)
			bwrite(state.in.buffer + MAXUNREAD, state.backup.next - (state.backup.buffer + MAXUNREAD));
		return;
	}
	error(3, "%s: cannot position %s archive for append", state.file, format[state.formatin].name);
}

/*
 * flush buffered input
 */

void
bflushin()
{
	state.in.count += state.in.last - state.in.next;
	state.in.next = state.in.last = state.in.buffer + MAXUNREAD;
	if (!state.in.eof)
	{
		while (read(state.append, state.tmp.buffer, state.buffersize) > 0);
		state.in.eof = 1;
	}
}

/*
 * flush buffered output
 */

void
bflushout()
{
	register int	n;
	register int	c;

	if (n = state.out.next - state.out.buffer)
	{
		state.out.next = state.out.buffer;
		while ((c = write(1, state.out.next, n)) != n)
		{
			if (c <= 0) newio(1, c, n);
			else
			{
				state.out.next += c;
				n -= c;
			}
		}
		state.out.next = state.out.buffer;
	}
}

/*
 * buffered output
 */

void
bwrite(b, n)
register char*	b;
register int	n;
{
	register int	c;

	if (state.maxout && state.out.count >= state.maxout)
	{
		bflushout();
		newio(1, 0, 0);
	}
	state.out.count += n;
	if (state.out.blocked)
	{
#if DEBUG
		if (n > 0) {message(-7, "bwrite(%d@%ld): %-.*s...", n, state.out.count + n, n > 32 ? 32 : n, b);}
		else message(-7, "bwrite(%d@%ld):", n, state.out.count + n);
#endif
		while ((c = write(1, b, n)) != n)
		{
			if (n <= 0)
			{
#ifdef MTIOCTOP
				{
					struct mtop	mt;

					mt.mt_op = MTWEOF;
					mt.mt_count = 1;
					if (ioctl(1, MTIOCTOP, &mt) >= 0) break;
				}
#endif
				error(3, "cannot write tape EOF marks");
			}
			if (c <= 0) newio(1, c, n);
			else if ((n -= c) > 0) b += c;
			else break;
		}
	}
	else
	{
#if DEBUG
		if (n > 0) {message(-7, "bwrite(%d@%ld): %-.*s...", n, state.out.count + n, n > 32 ? 32 : n, b);}
		else message(-7, "bwrite(%d@%ld):", n, state.out.count + n);
#endif
		for (;;)
		{
			if ((c = state.out.buffer + state.blocksize - state.out.next) <= n)
			{
				if (c)
				{
					memcpy(state.out.next, b, c);
					n -= c;
					b += c;
				}
				state.out.next = state.out.buffer;
				while ((c = write(1, state.out.next, state.blocksize)) != state.blocksize)
				{
					if (c <= 0) newio(1, c, n);
					else
					{
						memcpy(state.tmp.buffer, state.out.buffer + c, state.blocksize - c);
						memcpy(state.out.buffer, state.tmp.buffer, state.blocksize - c);
						state.out.next = state.out.buffer + state.blocksize - c;
						break;
					}
				}
#if DEBUG
				message(-8, "write(%d): %-.32s...", c, state.out.buffer);
#endif
			}
			else
			{
				memcpy(state.out.next, b, n);
				state.out.next += n;
				break;
			}
		}
	}
}

/*
 * bwrite() n chars that have been placed in state.out.next
 */

void
bput(n)
register int	n;
{
	state.out.count += n;
#if DEBUG
	message(-7, "bput(%d@%ld): %-.*s%s", n, state.out.count, n > 32 ? 32 : n, state.out.next, n > 32 ? "..." : "");
#endif
	if ((state.out.next += n) > state.out.buffer + state.blocksize)
	{
		n = (state.out.next - state.out.buffer) - state.blocksize;
		state.out.count -= n;

		/*
		 * flush out the buffer and slide over the remains
		 */

		bwrite(state.out.next = state.out.buffer + state.blocksize, n);
	}
}

static struct stat*	devst;

/*
 * find path name in /dev for <devst->st_dev,devst->st_ino>
 * called by ftwalk()
 */

/*ARGSUSED*/
static int
devname(ftw)
register struct FTW*	ftw;
{
	if (ftw->info == FTW_F && ftw->statb.st_dev == devst->st_dev && ftw->statb.st_ino == devst->st_ino)
	{
#if DEBUG
		message(-1, "device name is %s", ftw->path);
#endif
		state.file = strdup(ftw->path);
		return(1);
	}
	return(0);
}

/*
 * initilize tty file pointers for interactive prompting
 */

void
interactive()
{
	if (!state.rtty)
	{
		if (!(state.rtty = fopen("/dev/tty", "r")) || !(state.wtty = fopen("/dev/tty", "w")))
			error(ERROR_SYSTEM|3, "cannot prompt for interactive input");
		setbuf(state.rtty, (char*)0);
		setbuf(state.wtty, (char*)0);
	}
}

/*
 * check for new input or output stream
 * c is the io count causing the newio()
 * n is the pending buffered io count
 */

void
newio(fd, c, n)
register int	fd;
int		c;
int		n;
{
	register char*	s;
	register char*	rw;
	char*		file;
	char*		io;
	char*		t;
	int		vol;
	long		z;
	struct stat	st;

	static int	locked;
	static long	total;

	vol = 0;
	if (fd)
	{
		rw = "write";
		io = "output";
		state.out.offset += state.out.count - n;
		state.out.count = n;
		z = state.out.offset + state.out.count;
		if (state.out.blocked && state.record.file) switch (state.formatout)
		{
		case ALAR:
		case IBMAR:
		case PAX:
			if (locked) return;
			locked = 1;
			putlabels(state.record.file, "EOV");
			locked = 0;
			vol = 1;
			break;
		}
	}
	else
	{
		rw = "read";
		io = "input";
		z = state.in.offset + state.in.count;
	}
	if (fstat(fd, &st) < 0) error(ERROR_SYSTEM|3, "%s: cannot stat", io);
	switch (st.st_mode & S_IFMT)
	{
	case S_IFBLK:
	case S_IFCHR:
		file = 0;
		break;
	default:
		if (fd) switch (c < 0 ? errno : 0)
		{
		case 0:
#ifdef EFBIG
		case EFBIG:
#endif
#ifdef EDQUOT
		case EDQUOT:
#endif
			file = "file";
			break;
		default:
			error(ERROR_SYSTEM|3, "%s %s error -- cannot recover", io, rw);
			break;
		}
		else file = "file";
		break;
	}
	switch (c < 0 ? errno : 0)
	{
	case 0:
	case ENOSPC:
	case ENXIO:
		error(1, "end of %s medium", io);
		break;
	default:
		error(ERROR_SYSTEM|1, "%s %s error", io, rw);
		break;
	}
	if (total == z) error(1, "no %s on part %d", io, state.part--);
	else total = z;
	if (!file && state.file != definput && state.file != defoutput)
	{
		devst = &st;
		(void)ftwalk("/dev", devname, 0, (int(*)())0);
	}
	close(fd);
	if (file && state.file != definput && state.file != defoutput && strmatch(state.file, "*.+([0-9])") && (s = strrchr(state.file, '.')) && ((c = strtol(++s, (char*)0, 10)) == state.part || c == (state.part - 1)))
	{
		if (state.part == 1 && (!(state.file = strdup(state.file)) || !(s = strrchr(state.file, '.'))))
			error(3, "out of space");
		s += strlen(s);
		while (*--s != '.')
		{
			if (*s < '9')
			{
				(*s)++;
				break;
			}
			*s = '0';
		}
		if (*s != '.' && creat(state.file, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) == fd)
		{
			state.part++;
			error(1, "continuing %s %d %s on %s", state.part == state.volume + 1 ? "volume" : "part", state.part, io, state.file);
			return;
		}
		error(ERROR_SYSTEM|1, "%s: cannot create for writing", state.file);
	}
	if (file || state.file == definput || state.file == defoutput)
	{
		for (;;)
		{
			interactive();
			putc('\007', state.wtty);
			fprintf(state.wtty, "Enter part %d %s %s name: ", state.part + 1, io, file ? file : "device");
			if (!(s = fgetline(state.rtty, 0)))
			{
				putc('\n', state.wtty);
				finish(2);
			}
			if (*s)
			{
				if (!file) break;
				if (fd)
				{
					if ((n = creat(s, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) == fd) break;
					error(ERROR_SYSTEM|1, "%s: cannot creat", s);
				}
				else
				{
					if ((n = open(s, 0)) == fd) break;
					error(ERROR_SYSTEM|1, "%s: cannot read", s);
				}
				if (n >= 0) close(n);
			}
		}
		state.file = strdup(s);
	}
	if (!file)
	{
		for (;;)
		{
			interactive();
			putc('\007', state.wtty);
			fprintf(state.wtty, eomprompt, state.part + 1);
			if (!(s = fgetline(state.rtty, 0)))
			{
				putc('\n', state.wtty);
				finish(2);
			}
			if (*s == '!')
			{
				static char*	last;

				if (*++s)
				{
					if (last) free(last);
					last = strdup(s);
				}
				else s = last;
				if (!s) error(1, "no previous command");
				else if (n = system(s)) error(1, "exit status %d", n);
			}
			else
			{
				file = *s ? s : state.file;
				if (open(file, fd + state.append) == fd) break;
				file = strtape(file, &t);
				if (!*t && open(file, fd + state.append) == fd) break;
				error(ERROR_SYSTEM|1, "cannot %s %s", rw, *s ? s : state.file);
			}
		}
		if (state.file != file) state.file = strdup(file);
	}
	state.part++;
	if (vol && !locked)
	{
		locked = 1;
		putprologue();
		putlabels(state.record.file, "HDR");
		locked = 0;
	}
}
