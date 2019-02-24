/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax file copy support
 */

#include "pax.h"

static void	recordin();
static void	savesetin();
static void	recordout();

/*
 * copy files in from archive
 */

void
copyin()
{
	register struct fileinfo*	f;

	f = &file;
	while (getprologue())
	{
		while (getheader(f))
		{
			if (selectfile(f)) filein(f);
			else fileskip(f);
			gettrailer(f);
		}
		getepilogue();
	}
	deltaverify();
}

/*
 * copy a single file out to the archive
 * called by ftwalk()
 */

int
copyout(ftw)
register struct FTW*	ftw;
{
	register struct fileinfo*	f;
	register struct deltainfo*	d;
	int				fd;
	int				dfd;

	if (getfile(f = &file, ftw))
	{
		if (selectfile(f) && (!state.verify || verify(f)))
		{
			fd = openin(f);
			d = state.delta.tab ? (struct deltainfo*)hashget(state.delta.tab, f->name) : (struct deltainfo*)0;
			if (state.delta.op & (COMPRESS|OUT))
			{
				if (d) d->mark = 1;
				if (fd >= 0 && (!d || f->st->st_mtime != d->mtime))
				{
					if (d)
					{
						f->delta.op = DELTA_update;
						f->st->st_dev = d->dev;
						f->st->st_ino = d->ino;
#if DEBUG
						message(-2, "delta: delta: file=%s offset=%ld size=%ld", f->name, d->offset, d->size);
#endif
						if (d->size)
						{
							if (state.delta.buffersize < d->size)
							{
								state.delta.buffersize = round(d->size, state.blocksize);
								if (state.delta.buffer) free(state.delta.buffer);
								if (!(state.delta.buffer = malloc(state.delta.buffersize)))
									error(3, "input files too large to delta");
							}
							dfd = deltafd(d);
							if (read(dfd, state.delta.buffer, d->size) != d->size)
								error(3, "%s: base archive read error", f->name);
							(void)close(dfd);
						}
					}
					else
					{
						f->delta.op = DELTA_create;
#if DEBUG
						message(-2, "delta: create: file=%s", f->name);
#endif
					}
					if (d || (state.delta.op & COMPRESS))
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
						fd = dodelta(f, state.delta.buffer, d ? d->size : 0L, state.tmp.buffer);
					}
				}
				else
				{
#if DEBUG
					message(-2, "delta: verify: file=%s", f->name);
#endif
					f->delta.op = DELTA_verify;
					f->st->st_size = 0;
				}
			}
			if (fd >= 0)
			{
				if (!d || d->mtime != f->st->st_mtime) fileout(f, fd);
				else if (fd >= 0) close(fd);
			}
		}
		else ftw->status = FTW_SKIP;
	}
	return(0);
}

/*
 * low level for copyout()
 * if rfd<0 && st_size>0 then input from bread()
 */

void
fileout(f, rfd)
register struct fileinfo*	f;
int				rfd;
{
	register int	n;
	register long	c;
	int		err;
	FILE*		rfp;

	if (f->delta.op == DELTA_verify)
	{
		state.selected--;
		if (rfd >= 0) close(rfd);
	}
	else
	{
		putheader(f);
		switch (state.formatout)
		{
		case ALAR:
		case IBMAR:
		case PAX:
			if (state.out.blocked)
			{
				if (f->st->st_size > 0)
				{
					if (state.record.format == 'F' && !state.record.line)
					{
						/*
						 * this is faster than recordout()
						 */

						state.record.file = f;
						err = 0;
						c = f->st->st_size;
						while (c > 0)
						{
							n = c > state.record.size ? state.record.size : c;

							/*
							 * NOTE: we expect that all but the last
							 *	 read returns state.record.size
							 *	 if not the the intermediate short
							 *	 reads are filled with 0's
							 */

							if (!err)
							{
								if (rfd >= 0) n = read(rfd, state.tmp.buffer, n);
								else if (bread(state.tmp.buffer, 0L, (long)n, 1) <= 0) n = -1;
							}
							if (n <= 0)
							{
								if (n) error(ERROR_SYSTEM|2, "%s: read error", f->path);
								else error(2, "%s: file size changed", f->path);
								(void)memset(state.tmp.buffer, 0, state.record.size);
								err = 1;
							}
							else
							{
								c -= n;
								if (n < state.record.size && (c > 0 || state.record.pad))
								{
									(void)memset(state.tmp.buffer + n, 0, state.record.size - n);
									n = state.record.size;
								}
								bwrite(state.tmp.buffer, n);
							}
						}
						state.record.file = 0;
						if (rfd >= 0) close(rfd);
					}
					else if (rfd < 0) recordout(f, (FILE*)0);
					else if (!(rfp = fdopen(rfd, "r")))
					{
						error(1, "%s: cannot read", f->path);
						close(rfd);
					}
					else
					{
						recordout(f, rfp);
						fclose(rfp);
					}
				}
				break;
			}
			/*FALLTHROUGH*/
		default:
			err = 0;
			c = f->st->st_size;
			while (c > 0)
			{
				n = c > state.buffersize ? state.buffersize : c;
				if (!err)
				{
					if (rfd >= 0) n = read(rfd, state.out.next, n);
					else if (bread(state.out.next, 0L, (long)n, 1) <= 0) n = -1;
				}
				if (n <= 0)
				{
					if (n) error(ERROR_SYSTEM|2, "%s: read error", f->path);
					else error(2, "%s: file size changed", f->path);
					(void)memset(state.out.next, 0, state.buffersize);
					err = 1;
				}
				else
				{
					c -= n;
					bput(n);
				}
			}
			if (rfd >= 0) close(rfd);
			break;
		}
		puttrailer(f);
	}
	if (state.acctime && f->type != S_IFLNK && !(state.delta.op & IN))
		settime(f->name, f->st->st_atime, f->st->st_mtime);
}

/*
 * fileout() record support
 */

static void
recordout(f, fp)
struct fileinfo*	f;
FILE*			fp;
{
	register int	c;
	register char*	p;
	register char*	recdat;
	register char*	blkdat;
	char*		rec;
	char*		blk;
	int		span;

	int		count = 0;
	int		partial = 0;
	int		truncated = 0;

	static char	span_out[] = "0132";
	static char*	pardat;

	if (!fp) error(3, "cannot handle record output from buffer");
	state.record.file = f;
	f->record.blocks = 0;
	span = 0;
	blk = state.tmp.buffer;

	/*
	 * file loop
	 */

	for (;;)
	{
		p = blk;
		switch (state.record.format)
		{
		case 'V':
			p += 4;
			break;
		}
		blkdat = p;

		/*
		 * block loop
		 */

		for (;;)
		{
			rec = p;
			switch (state.record.format)
			{
			case 'D':
			case 'V':
				p += 4;
				break;
			case 'S':
				p += 5;
				break;
			}
			recdat = p;

			/*
			 * check for partial record from previous block
			 */

			if (partial)
			{
				(void)memcpy(recdat, pardat, partial);
				p += partial;
				partial = 0;
			}

			/*
			 * record loop
			 */

			span &= 01;
			span <<= 1;
			for (;;)
			{
				if (p >= &rec[state.record.size] && state.record.size)
				{
					if (state.record.line)
					{
						truncated++;
						while ((c = getc(fp)) != EOF && c != '\n');
					}
					break;
				}
				else if (p >= &blk[state.blocksize])
				{
					if (state.record.format == 'S' || state.record.format == 'V')
					{
						if (p > recdat)
						{
							span |= 01;
							break;
						}
					}
					else if (partial = p - recdat)
					{
						/*
						 * save partial record for next block
						 */

						if (!pardat && !(pardat = malloc(state.blocksize))) error(3, "out of space [record pushback buffer]");
						(void)memcpy(pardat, recdat, partial);
					}
					p = rec;
					goto eob;
				}
				else if ((c = getc(fp)) == EOF)
				{
					if (p == recdat)
					{
						if (rec == blkdat) goto eof;
						p = rec;
						goto eob;
					}
					break;
				}
				else if (c == '\n' && state.record.line) break;
				else *p++ = c;
			}
			switch (state.record.format)
			{
			case 'D':
				c = recdat[0];
				(void)sprintf(rec, "%04d", p - rec);
				recdat[0] = c;
				break;
			case 'F':
				if (c != EOF || state.record.pad)
				{
					(void)memset(p, ' ', state.record.size - (p - rec));
					p = rec + state.record.size;
				}
				break;
			case 'S':
				c = recdat[0];
				(void)sprintf(rec, "%c%04d", span_out[span], p - rec);
				recdat[0] = c;
				break;
			case 'U':
				if (p == recdat) *p++ = ' ';
				break;
			case 'V':
				rec[0] = ((p - rec) >> 8) & 0xff;
				rec[1] = (p - rec) & 0xff;
				rec[2] = span;
				rec[3] = 0;
				break;
			}
			if (state.record.charset && state.formatout == IBMAR) cvtatoe(recdat, recdat, p - recdat);
			count++;
			if (p >= &blk[state.blocksize] || state.record.format == 'U') break;
		}
	eob:
		switch (state.record.format)
		{
		case 'D':
		case 'S':
			if (state.record.pad)
			{
				(void)memset(p, '^', state.blocksize - (p - blk));
				p = blk + state.blocksize;
			}
			break;
		case 'V':
			blk[0] = ((p - blk) >> 8) & 0xff;
			blk[1] = (p - blk) & 0xff;
			blk[2] = 0;
			blk[3] = 0;
			break;
		}
		bwrite(blk, p - blk);
		f->record.blocks++;
	}
 eof:
	state.record.file = 0;
	if (truncated) error(1, "%s: %d out of %d record%s truncated", f->name, truncated, count, count == 1 ? "" : "s");
}

/*
 * low level for copyin()
 */

void
filein(f)
register struct fileinfo*	f;
{
	register long	c;
	register int	n;
	register char*	s;
	int		wfd;
	long		checksum;
	struct stat	st;

	if (f->skip || state.list) fileskip(f);
	else switch (f->delta.op)
	{
	case DELTA_create:
		if (f->delta.base) error(3, "%s: base archive mismatch", f->name);
		if (state.delta.op & COMPRESS) goto update;
		goto regular;
	case DELTA_update:
		if (!f->delta.base || f->delta.base->mtime >= f->st->st_mtime) error(3, "%s: base archive mismatch", f->name);
	update:
		if ((wfd = openout(f)) < 0) fileskip(f);
		else if (doupdate(f, state.delta.fd, f->name, wfd) < 0)
			error(ERROR_SYSTEM|2, "%s: delta update error", f->name);
		break;
	case DELTA_verify:
		if (!f->delta.base || f->delta.base->mtime != f->st->st_mtime) error(3, "%s: base archive mismatch", f->name);
		if ((*state.statf)(f->name, &st)) error(2, "%s: not copied from base archive", f->name);
		else if (st.st_size != f->delta.base->size || state.modtime && st.st_mtime != f->st->st_mtime) error(1, "%s: changed from base archive", f->name);
		break;
	case DELTA_delete:
		if (!f->delta.base) error(3, "%s: base archive mismatch", f->name);
		/*FALLTHROUGH*/
	default:
	regular:
		wfd = openout(f);
		switch (state.formatin)
		{
		case ALAR:
		case IBMAR:
		case PAX:
			recordin(f, wfd);
			break;
#if SAVESET
		case SAVESET:
			savesetin(f, wfd);
			break;
#endif
		default:
			if (wfd >= 0)
			{
				checksum = 0;
				for (c = f->st->st_size; c > 0; c -= n)
				{
					n = (c > state.buffersize) ? state.buffersize : c;
					if (!(s = bget(n)))
					{
						error(ERROR_SYSTEM|2, "%s: read error", f->name);
						break;
					}
					if (write(wfd, s, n) != n)
					{
						error(ERROR_SYSTEM|2, "%s: write error", f->name);
						break;
					}
					if (state.formatin == ASCHK) checksum = asc_checksum(s, n, checksum);
				}
				close(wfd);
				setfile(f);
				if (state.formatin == ASCHK && checksum != f->checksum) error(1, "%s: %s checksum error (0x%08x != 0x%08x)", f->name, format[state.formatin].name, checksum, f->checksum);
			}
			else fileskip(f);
			break;
		}
	}
	listentry(f);
}

/*
 * filein() record support
 */

static void
recordin(f, wfd)
register struct fileinfo*	f;
int				wfd;
{
	register long			n;
	register long			size;
	int				c;
	int				i;
	int				j;
	int				k;
	int				nl;
	long				m;
	FILE*				wfp;

	if (wfd < 0) wfp = 0;
	else if (!(wfp = fdopen(wfd, "w"))) error(1, "%s: cannot write", f->name);
	state.in.empty = 0;
	nl = state.record.line;
	size = 0;
	for (;;)
	{
		if (state.in.blocked) n = bread(state.tmp.buffer, 0L, (long)state.buffersize, 0);
		else if ((m = f->st->st_size - size) <= 0) n = 0;
		else if (wfp) 
		{
			if (m > state.buffersize) m = state.buffersize;
			n = bread(state.tmp.buffer, 0L, m, 1);
		}
		else n = bread((char*)0, 0L, m, 1);
		if (n < 0) break;
		if (n == 0)
		{
			k = 1;
			state.delta.sum--;
			while (getlabel(f))
			{
				if (strneq(alar_header, "EOV1", 4)) k = 0;
				else if (!strneq(alar_header, "EOF", 3) && !strneq(alar_header, "EOV", 3) && !strneq(alar_header, "UTL", 3) && ++n >= 16 && !state.keepgoing)
					error(3, "%s: %d invalid %s end of file/volume labels detected", f->name, n, format[state.formatin].name);
			}
			if (n) error(1, "%s: %d invalid %s end of file/volume labels detected", f->name, n, format[state.formatin].name);
			if (k)
			{
				state.delta.sum++;
				break;
			}
			f->record.section++;
			f->id = strcpy(state.tmp.buffer, f->id);
			f->name = strcpy(state.tmp.buffer + ALAR_NAMESIZE + 1, f->name);
			for (;;)
			{
				newio(0, 0, 0);
				if (getprologue())
				{
					struct fileinfo	v;
					struct stat	st;

					v.st = &st;
					if (getheader(&v))
					{
						if (streq(f->id, v.id) && streq(f->name, v.name) && f->record.section == v.record.section)
						{
							f->id = v.id;
							f->name = v.name;
							break;
						}
						error(1, "volume containing %s id %s section %d required", f->name, f->id, f->record.section);
					}
					state.volume--;
				}
				state.part--;
			}
			state.delta.sum++;
			continue;
		}
		if (f->record.format == 'V')
		{
			if ((k = ((unsigned char*)state.tmp.buffer)[0] << 8 | ((unsigned char*)state.tmp.buffer)[1]) != n)
				error(3, "%s: invalid %s V format block descriptor [%d!=%d]", f->name, format[state.formatin].name, k, n);
			i = 4;
		}
		else i = 0;
		while (i < n)
		{
			i += state.record.offset;
			if (state.tmp.buffer[i] == '^') switch (f->record.format)
			{
			case 'F':
				if (state.formatin == IBMAR || state.formatin == PAX) break;
				for (j = i; j < n && state.tmp.buffer[j] == '^'; j++);
				if (j < n) break;
				/*FALLTHROUGH*/
			case 'D':
			case 'S':
				i = n;
				continue;
			}

			/*
			 * get record size
			 */

			switch (f->record.format)
			{
			case 'D':
				if (sscanf(&state.tmp.buffer[i], "%4d", &k) != 1) k = -1;
				j = i + 4;
				break;
			case 'F':
				if (i + state.record.size > n) k = n - i;
				else if (state.record.line || state.record.offset) k = state.record.size;
				else k = n;
				j = i;
				break;
			case 'S':
				switch (state.tmp.buffer[i])
				{
				case '0':
				case '3':
					nl = 1;
					break;
				default:
					nl = 0;
					break;
				}
				if (sscanf(&state.tmp.buffer[i + 1], "%4d", &k) != 1) k = -1;
				j = i + 5;
				break;
			case 'U':
				k = n;
				j = i;
				break;
			case 'V':
				nl = !(state.tmp.buffer[i + 2] & 01);
				k = ((unsigned char*)state.tmp.buffer)[i] << 8 | ((unsigned char*)state.tmp.buffer)[i + 1];
				j = i + 4;
				break;
			}
			if (k < 0)
			{
				error(2, "invalid %s %c record size", format[state.formatin].name, f->record.format);
				break;
			}
			m = i += k;
			if (state.record.charset && state.formatin == IBMAR) cvtetoa(&state.tmp.buffer[j], &state.tmp.buffer[j], m - j);
			if (state.record.line) switch (f->record.format)
			{
			case 'F':
			case 'U':
				while (--m >= j && state.tmp.buffer[m] == ' ');
				m++;
				break;
			}
			k = m - j + nl;
			size += k;
			if (wfp)
			{
				if (nl)
				{
					c = state.tmp.buffer[m];
					state.tmp.buffer[m] = '\n';
				}
				if (fwrite(&state.tmp.buffer[j], 1, k, wfp) != k)
				{
					error(ERROR_SYSTEM|1, "%s: write error", f->name);
					break;
				}
				if (nl) state.tmp.buffer[m] = c;
			}
		}
	}
	if (f->st->st_size && f->st->st_size != size) error(1, "%s: header size %ld does not match data size %ld", f->name, f->st->st_size, size);
	f->st->st_size = size;
	if (wfp)
	{
		fclose(wfp);
		setfile(f);
	}
	if (n < 0) error(ERROR_SYSTEM|3, "%s: archive read error", f->name);
}

#if SAVESET

/*
 * filein() saveset support
 */

static void
savesetin(f, wfd)
register struct fileinfo*	f;
int				wfd;
{
	register long		c;
	int			i;
	int			j;
	int			k;
	FILE*			wfp;

	if (wfd < 0) wfp = 0;
	else if (!(wfp = fdopen(wfd, "w"))) error(1, "%s: cannot write", f->name);
	j = 0;
	k = 0;
	c = 0;
	while (getsaveset(f, 0))
	{
		/*
		 * this part transcribed from vmsbackup
		 */

		i = 0;
		if (wfp) while ((c + i) < f->st->st_size && i < state.saveset.lastsize) switch (state.saveset.recfmt)
		{
		case 1:	/* fixed length		*/
			if (j <= 0) j = state.saveset.reclen;
			fputc(state.saveset.bp[i], wfp);
			i++;
			j--;
			break;
		case 2:	/* variable length	*/
		case 3:	/* with fixed control	*/
			if (j <= 0)
			{
				j = k = gethalf(BYTE|HALF, &state.saveset.bp[i]);
				i += 2;
				if (state.saveset.recfmt == 3)
				{
					i += state.saveset.recvfc;
					j -= state.saveset.recvfc;
				}
			}
			else
			{
				if (j == k && state.saveset.recatt == 1)
				{
					if (state.saveset.bp[i] == '0') state.saveset.bp[i] = '\n';
					else if (state.saveset.bp[i] == '1') state.saveset.bp[i] = '\f';
				}
				fputc(state.saveset.bp[i], wfp);
				i++;
				j--;
			}
			if (j <= 0)
			{
				fputc('\n', wfp);
				if (i & 1) i++;
			}
			break;
		case 4:	/* seq stream		*/
		case 5:	/* seq LF stream	*/
			if (j <= 0) j = 512;
			if (state.saveset.bp[i] == '\n') j = 0;
			else j--;
			fputc(state.saveset.bp[i], wfp);
			i++;
			break;
		case 6:	/* seq CR stream	*/
			if (state.saveset.bp[i] == '\r') state.saveset.bp[i] = '\n';
			fputc(state.saveset.bp[i], wfp);
			i++;
			break;
		default:
			error(state.keepgoing ? 1 : 3, "%s: invalid %s format data record format=%d", f->name, format[state.formatin].name, state.saveset.recfmt);
			goto next;
		}
	next:
		c += i;
	}
	if (wfp)
	{
		fclose(wfp);
		setfile(f);
	}
}

#endif

/*
 * pass data from rfd to wfd
 */

void
filepass(f, rfd, wfd)
register struct fileinfo*	f;
int				rfd;
int				wfd;
{
	register long	c;
	register long	n;

	for (c = f->st->st_size; c > 0; c -= n)
	{
		if ((n = read(rfd, state.tmp.buffer, (c > state.buffersize) ? state.buffersize : c)) <= 0)
		{
			error(ERROR_SYSTEM|2, "%s: read error", f->name);
			break;
		}
		if (write(wfd, state.tmp.buffer, n) != n)
		{
			error(ERROR_SYSTEM|2, "%s: write error", f->name);
			break;
		}
		state.out.count += n;
	}
	(void)close(rfd);
	(void)close(wfd);
	setfile(f);
	listentry(f);
}

/*
 * skip over archive member f file data
 */

void
fileskip(f)
register struct fileinfo*	f;
{
	switch (state.formatin)
	{
	case ALAR:
	case IBMAR:
	case PAX:
		recordin(f, -1);
		break;
#if SAVESET
	case SAVESET:
		savesetin(f, -1);
		break;
#endif
	default:
		if (bread((char*)0, 0L, f->st->st_size, 1) < 0)
			error(ERROR_SYSTEM|2, "%s: skip error", f->name);
		break;
	}
}

/*
 * single file copyin() and copyout() smashed together
 * called by ftwalk()
 */

int
copyinout(ftw)
struct FTW*	ftw;
{
	register struct fileinfo*	f;
	register int			rfd;
	register int			wfd;

	static char			path[PATH_MAX];

	if (getfile(f = &file, ftw) && selectfile(f))
	{
		(void)strcpy(path, state.pwd);
		(void)strcpy(path + state.pwdlen, f->name + (*f->name == '/'));
		f->name = path;
		if ((wfd = openout(f)) >= 0)
		{
			if ((rfd = openin(f)) >= 0) filepass(f, rfd, wfd);
			else (void)close(wfd);
		}
		else if (wfd != -1) listentry(f);
	}
	return(0);
}

/*
 * compare ft1 and ft2 for ftwalk() sort
 */

int
cmpftw(ft1, ft2)
struct FTW*	ft1;
struct FTW*	ft2;
{
	return(strcmp(ft1->name, ft2->name));
}


/*
 * copy files out using copyfile
 */

void
copy(copyfile)
register int	(*copyfile)();
{
	register char*	s;
	register int	n;

	if (state.files) n = ftwalk((char*)state.files, copyfile, state.ftwflags|FTW_MULTIPLE, state.exact ? (int(*)())0 : cmpftw);
	else
	{
		n = 0;
		while (s = fgetline(stdin, 0))
			if (n = ftwalk(s, copyfile, state.ftwflags, (int(*)())0)) break;
		if (n) error(2, "%s: not completely copied", s);
	}
}

/*
 * position archive for appending
 */

void
append()
{
	if (state.update) initdelta();
	state.operation = IN;
	state.formatin = IN_DEFAULT;
	copyin();
	state.append = 0;
	state.formatout = state.formatin;
	state.operation = OUT;
}
