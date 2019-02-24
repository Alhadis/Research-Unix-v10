/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax archive format support
 */

#include "pax.h"

static int		isalar();

#if CPIO_EXTENDED
static void		getxops();
static void		setxops();
static void		putxops();
static void		addxopstr();
static void		addxopnum();
#endif

/*
 * read archive prologue before files are copied
 */

int
getprologue()
{
	long	size;

	if (state.volume && state.append) return(0);
	state.formatin = IN_DEFAULT;
	state.id.volume[0] = 0;
	state.in.offset += state.in.count;
	state.in.count = 0;
	if (bread(alar_header, (long)ALAR_HEADER, (long)ALAR_HEADER, 0) <= 0)
	{
		if (!bcount(state.in)) return(0);
	}
	else if (!isalar(alar_header))
	{
		if (strneq(alar_header, PORTAR_MAG, PORTAR_MAGSIZ))
		{
			bunread(alar_header, ALAR_HEADER - 8);
			if (bread(&portar_header, 0, (long)PORTAR_HEADER, 0) > 0)
			{
				if (!strneq(portar_header.ar_fmag, PORTAR_END, PORTAR_ENDSIZ) || sscanf(portar_header.ar_size, "%ld", &size) != 1)
				{
					bunread(&portar_header, PORTAR_HEADER);
					bunread(PORTAR_MAG, PORTAR_MAGSIZ);
				}
				else if (strmatch(portar_header.ar_name, PORTAR_SYM) && (state.formatin = PORTAR) || strmatch(portar_header.ar_name, RANDAR_SYM) && (state.formatin = RANDAR))
				{
					size += (size & 01);
					if (bread((char*)0, 0L, size, 0) <= 0)
						error(3, "invalid %s format symbol table", format[state.formatin].name);
				}
				else
				{
					size = portar_header.ar_date[0];
					portar_header.ar_date[0] = 0;
					state.formatin = strchr(portar_header.ar_name, PORTAR_TERM) ? PORTAR : RANDAR;
					portar_header.ar_date[0] = size;
					bunread(&portar_header, PORTAR_HEADER);
				}
			}
		}
		else if (strneq(alar_header, "<ar>", 4) ||	/* s5r1	 */
			gethalf(0, alar_header) == 0177545 ||	/* pdp11 */
			gethalf(BYTE, alar_header) == 0177545)	/* pdp11 */
		    		error(3, "%s: use ar(1) for library archives", state.file);
		else bunread(alar_header, ALAR_HEADER);
	}
	if (state.volume++)
	{
		if (state.delta.op)
		{
			if (state.operation == (IN|OUT) || !(state.delta.op & COMPRESS))
				error(3, "%s: %s archive cannot be multi-volume", state.file, (state.delta.op & COLLECT) ? "base" : "delta");
			state.delta.op = 0;
		}

		/*
		 * no hard links between volumes
		 */

		hashfree(state.linktab);
		if (!(state.linktab = hashalloc((HASHTABLE*)0, HASH_set, HASH_ALLOCATE, HASH_namesize, sizeof(struct fileid), HASH_name, "links", 0)))
			error(3, "cannot re-allocate hard link table");
	}
	state.entry = 0;
	return(1);
}

/*
 * check for ASCII or EBCDIC ALAR prologue in alar_header
 */

static int
isalar(hdr)
register char*	hdr;
{
	char		buf[4];

	if (!strneq(hdr, "VOL1", 4))
	{
		cvtetoa(hdr, buf, 4);
		if (!strneq(buf, "VOL1", 4)) return(0);
		cvtetoa(hdr, hdr, ALAR_HEADER);
		state.formatin = IBMAR;
	}
	else state.formatin = ALAR;
	(void)getlabstr(hdr, 5, sizeof(state.id.volume) - 1, state.id.volume);
	(void)getlabstr(hdr, 25, 6, state.id.format);
	(void)getlabstr(hdr, 31, 7, state.id.implementation);
	(void)getlabstr(hdr, 38, 14, state.id.owner);
	state.in.blocked = !bcount(state.in);
	if (streq(state.id.format, PAX_ID))
	{
		state.formatin = PAX;
		state.record.line = 0;
	}
	return(1);
}

/*
 * write archive prologue before files are copied
 */

void
putprologue()
{
	switch (state.formatout)
	{
	case PAX:
		state.record.line = 0;
		/*FALLTHROUGH*/
	case ALAR:
	case IBMAR:
#if DEBUG
		if (state.out.blok) state.out.blocked = 1;
		else
#endif
		state.out.blocked = !state.out.unblocked;
		if (!state.id.owner[0])
		{
			(void)strncpy(state.id.owner, fmtuid(getuid()), sizeof(state.id.owner) - 1);
			state.id.owner[sizeof(state.id.owner) - 1] = 0;
			(void)strupper(state.id.owner);
		}
		if (!state.id.volume[0])
		{
			(void)strncpy(state.id.volume, state.id.owner, sizeof(state.id.volume) - 1);
			state.id.volume[sizeof(state.id.volume) - 1] = 0;
		}
		(void)strncpy(state.id.format, state.formatout == PAX ? PAX_ID : ALAR_ID, sizeof(state.id.format) - 1);
		(void)strncpy(state.id.implementation, IMPLEMENTATION, sizeof(state.id.implementation) - 1);
		(void)sprintf(alar_header, "VOL1%-6.6s              %-6.6s%-7.7s%-14.14s                            4", state.id.volume, state.id.format, state.id.implementation, state.id.owner);
		if (state.formatout == IBMAR) cvtatoe(alar_header, alar_header, ALAR_HEADER);
		bwrite(alar_header, ALAR_HEADER);
		(void)sprintf(alar_header, "VOL2     00000000000000                                                         ");
		if (state.formatout == IBMAR) cvtatoe(alar_header, alar_header, ALAR_HEADER);
		bwrite(alar_header, ALAR_HEADER);
		if (state.delta.op & (COMPRESS|OUT))
		{
			(void)sprintf(alar_header, "UVL1 %-6.6s%c%c%010ld%010ld                                               ", ID, (state.delta.op & COMPRESS) ? TYPE_COMPRESS : TYPE_DELTA, VERSION, state.operation == OUT ? state.delta.size : 0L, state.operation == OUT ? state.delta.checksum : 0L);
			if (state.formatout == IBMAR) cvtatoe(alar_header, alar_header, ALAR_HEADER);
			bwrite(alar_header, ALAR_HEADER);
		}
		break;
	default:
		if (state.delta.op & (COMPRESS|OUT))
		{
			register struct fileinfo*	f;
			struct fileinfo			file;

			f = &file;
#if new_delta_format
			(void)sprintf(state.tmp.file, "%c%s%c%c%c%c", INFO_SEP, ID, INFO_SEP, state.operation == OUT && state.delta.size ? TYPE_DELTA : TYPE_COMPRESS, VERSION, INFO_SEP);
			initfile(f, state.tmp.file, S_IFREG);
#else
			initfile(f, "DELTA!!!", S_IFREG);
#endif
			f->skip = 1;
			if (state.operation == OUT)
			{
				f->st->st_mtime = state.delta.size;
				f->st->st_uid = DELTA_LO(state.delta.checksum);
				f->st->st_gid = DELTA_HI(state.delta.checksum);
			}
			putheader(f);
			puttrailer(f);
		}
		break;
	}
}

/*
 * read archive epilogue after all files have been copied
 */

void
getepilogue()
{
	register char*	s;
	register int	n;
	register int	i;

	if (state.append) backup();
	else
	{
		switch (state.formatin)
		{
		case ALAR:
		case IBMAR:
		case PAX:
		case PORTAR:
		case RANDAR:
#if SAVESET
		case SAVESET:
#endif
			break;
		default:
			/*
			 * check for more volumes
			 * volumes begin on BLOCKSIZE boundaries
			 * separated by up to MAXBLOCKS null byte filler
			 */

			i = MAXBLOCKS;
			if (!(n = round(state.in.count, BLOCKSIZE) - state.in.count) || bread(state.tmp.buffer, 0L, (long)n, 0) > 0) do
			{
				if (*(s = state.tmp.buffer) && n == BLOCKSIZE)
				{
					bunread(state.tmp.buffer, BLOCKSIZE);
					state.formatin = IN_DEFAULT;
#if DEBUG
					message(-2, "go for next volume %-.32s...", state.tmp.buffer);
#endif
					return;
				}
				while (s < state.tmp.buffer + n && !*s++);
				if (s < state.tmp.buffer + n)
				{
					if (state.volume > 1) error(1, "junk data after volume %d", state.volume);
					break;
				}
				n = BLOCKSIZE;
			} while (i-- > 0 && bread(state.tmp.buffer, 0L, n, 0) > 0);
			bflushin();
			break;
		}
		state.formatin = IN_DEFAULT;
	}
}

/*
 * write archive epilogue after files have been copied
 */

void
putepilogue()
{
	register int			n;
	register int			boundary;
	register struct fileinfo*	f;

	struct fileinfo			tmp;

	static int			selected;

	if (state.selected > selected)
	{
		selected = state.selected;
		boundary = state.out.count;
		switch (state.formatout)
		{
		case ALAR:
		case IBMAR:
		case PAX:
			bwrite(alar_header, 0);
			bwrite(alar_header, 0);
			break;
		case BINARY:
		case CPIO:
		case ASC:
		case ASCHK:
			f = &tmp;
			initfile(f, CPIO_TRAILER, 0);
			f->skip = 1;
			putheader(f);
			puttrailer(f);
			boundary = state.out.unblocked ? BLOCKSIZE : state.blocksize;
			break;
		case TAR:
		case USTAR:
			memset(tar_block, 0, TAR_HEADER);
			bwrite(tar_block, TAR_HEADER);
			bwrite(tar_block, TAR_HEADER);
			boundary = state.out.unblocked ? BLOCKSIZE : state.blocksize;
			break;
		}
		if ((n = boundary - state.out.count) < 0) n = round(state.out.count, boundary) - state.out.count;
		if (n)
		{
			memset(state.tmp.buffer, 0, n);
			bwrite(state.tmp.buffer, n);
		}
		bflushout();
		state.volume++;
	}
	else
	{
		state.out.count = state.out.offset = 0;
		state.out.next = state.out.buffer;
	}
}

#if DEBUG

/*
 * try to repair pax binary cpio header botch that always set
 * mtime and size to 1
 */

static void
repair(f)
register struct fileinfo*	f;
{
	register char*		b;
	register char*		e;
	register int		null;
	int			n;
	long			pos;
	unsigned long		size;
	struct stat		st;
	struct binary_header	hdr;
	char			buf[4096];

	static int		magic0;
	static int		magic1;
	static time_t		mtime;
	static int		warned;

	if (!warned)
	{
		warned = 1;
		error(1, "repairing botched headers -- time stamps will be incorrect");
		mtime = fstat(state.append, &st) ? (time_t)1 : st.st_mtime;
		b = (char*)&binary_header.magic;
		magic0 = b[state.swap != 0];
		magic1 = b[state.swap == 0];
	}

	/*
	 * determine the file size by locating the next header
	 *
	 * NOTE: magic number is on even byte boundary
	 *	 file data is on even byte boundary
	 *       input buffer size is even
	 */

	null = 0;
	pos = 0;
	size = 0;
	b = state.in.next;
	e = state.in.last;
	for (;;)
	{
		while (b < e)
		{
			if (*b++ == magic0)
			{
				if (*b++ == magic1)
				{
					/*
					 * check a few header fields to weed
					 * out bad magic
					 */

					if ((n = e - (b - 2)) < BINARY_HEADER)
					{
						memcpy(&hdr, b - 2, n);
						if (read(state.append, (char*)&hdr + n, BINARY_HEADER - n) != BINARY_HEADER - n)
							error(ERROR_SYSTEM|3, "repair read error");
						if (lseek(state.append, (long)(n - BINARY_HEADER), 1) == -1L)
							error(3, "can only repair seekable input archives");
					}
					else memcpy(&hdr, b - 2, BINARY_HEADER);
					if (state.swap) memswap(state.swap, (char*)&hdr, BINARY_HEADER);
					if (cpio_long(hdr.mtime) == 1 && cpio_long(hdr.size) == 1)
					{
						/*
						 * found the next header
						 */

						f->st->st_size = size - (null == 1);
						f->st->st_mtime = mtime;
						if (pos && lseek(state.append, pos, 0) != pos)
							error(ERROR_SYSTEM|3, "repair seek error");
						return;
					}
				}
				else if (*(b - 1) == 0) null++;
			}
			else
			{
				if (*(b - 1) == 0) null++;
				if (*b++ == 0) null++;
			}
			size += 2;
		}
		if (!pos && (pos = lseek(state.append, 0L, 1)) == -1L)
			error(3, "can only repair seekable input archives");
		b = buf;
		if ((n = read(state.append, b, sizeof(buf))) <= 0)
			error(3, "cannot repair -- header missing");
		e = buf + n - 1;
	}
}

#endif

/*
 * read next archive entry header
 */

int
getheader(f)
register struct fileinfo*	f;
{
	register char*	s;
	register int	i;
	register long	n;
	char*		typename;
	long		num;
	int		warned;
	int		checkdelta;
	int		lab;
	int		type;
	short		magic;

	struct
	{
		long	dev;
		long	ino;
		long	mode;
		long	uid;
		long	gid;
		long	nlink;
		long	rdev;
		long	mtime;
		long	size;
		long	dev_major;
		long	dev_minor;
		long	rdev_major;
		long	rdev_minor;
		long	checksum;
	}		lst;

	static char	namebuffer[PATH_MAX * 2];
	static char	pathbuffer[PATH_MAX];
	static char	linkbuffer[PATH_MAX];
	static char	idbuffer[ALAR_NAMESIZE + 1];
	static char	uidname[9];
	static char	gidname[9];

 volume:
	warned = 0;
	checkdelta = !state.entry++ && !(state.test & 020);
	type = 0;
	typename = "";
	if (state.append) bsave();
 again:
	for (;;)
	{
		f->record.format = 0;
		f->skip = 0;
#if DEBUG
		message(-2, "%s:", format[state.formatin].name);
#endif
		switch (state.formatin)
		{
		case ALAR:
		case IBMAR:
		case PAX:
			if (!(lab = getlabel(f))) return(0);
			f->name = namebuffer;
			f->st->st_dev = 0;
			f->st->st_ino = 0;
			f->st->st_mode = S_IFREG|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
			f->st->st_uid = state.uid;
			f->st->st_gid = state.gid;
			f->st->st_nlink = 1;
			f->st->st_rdev = 0;
			f->st->st_size = 0;
			f->linktype = NOLINK;
			f->linkname = 0;
			f->uidname = 0;
			f->gidname = 0;
			type = 0;
			do
			{
				if (state.formatin == IBMAR) cvtetoa(alar_header, alar_header, ALAR_HEADER);
				if (checkdelta && strneq(alar_header, "UVL1", 4) && strneq(alar_header + 5, ID, IDLEN))
				{
					checkdelta = 0;
					s = alar_header + 10;
					f->st->st_mtime = getlabnum(alar_header, 14, 10, 10);
					n = getlabnum(alar_header, 24, 10, 10);
					f->st->st_uid = DELTA_LO(n);
					f->st->st_gid = DELTA_HI(n);
					goto deltaverify;
				}
				else if (strneq(alar_header, "HDR", 3))
				{
					if (getlabnum(alar_header, 4, 1, 10) != ++type) error(3, "%s format HDR label out of sequence", format[state.formatin].name);
					if (type == 1)
					{
						s = namebuffer;
						for (i = 4; i <= ALAR_NAMESIZE + 3; i++)
						{
							if (alar_header[i] == ' ')
							{
								if (i >= ALAR_NAMESIZE + 3 || alar_header[i + 1] == ' ') break;
								*s++ = '.';
							}
							else *s++ = isupper(alar_header[i]) ? tolower(alar_header[i]) : alar_header[i];
						}
						if ((n = getlabnum(alar_header, 40, 2, 10)) > 0 && n < 99) (void)sprintf(s, ".%02d", n);
						else *s = 0;
						f->record.section = getlabnum(alar_header, 28, 4, 10);
						getlabstr(alar_header, 5, ALAR_NAMESIZE, f->id = idbuffer);
						getlabstr(alar_header, 61, 6, state.id.format);
						getlabstr(alar_header, 67, 7, state.id.implementation);
						if (streq(state.id.format, PAX_ID))
							state.formatin = PAX;
#if SAVESET
						else if (streq(state.id.format, SAVESET_ID) && streq(state.id.implementation, SAVESET_IMPL))
							state.formatin = SAVESET;
#endif
						f->st->st_mtime = state.present;
						if (n = getlabnum(alar_header, 43, 2, 10))
						{
							if (alar_header[41] == '0') n += 100;
							if ((i = getlabnum(alar_header, 45, 3, 10)) >= 0 && i <= 365)
							{
								f->st->st_mtime = i;
								while (n-- > 70) f->st->st_mtime += ((n % 4) || n == 100) ? 365 : 366;
								f->st->st_mtime *= 24L * 60L * 60L;
								f->st->st_mtime += 12L * 60L * 60L;
							}
						}
					}
					else if (type == 2)
					{
						switch (f->record.format = alar_header[4])
						{
						case 'F': /* fixed length	*/
						case 'D': /* decimal variable	*/
						case 'S': /* spanned		*/
						case 'U': /* input block size	*/
						case 'V': /* binary variable	*/
							break;
						default:
							error(2, "%s record format %c not supported", format[state.formatin].name, f->record.format);
							f->skip = 1;
						}
						state.blocksize = getlabnum(alar_header, 6, 5, 10);
						state.record.size = getlabnum(alar_header, 11, 5, 10);
						if (!state.in.blocked) f->st->st_size = getlabnum(alar_header, 16, 10, 10);
						state.record.offset = getlabnum(alar_header, 51, 2, 10);
					}
					else if (state.formatin == PAX)
					{
						if (type == 3)
						{
							f->st->st_mode = strmode(alar_header + 20);
							if (alar_header[20] == 'z')
							{
								f->st->st_mode |= S_IFREG;
								f->linktype = HARDLINK;
							}
							getlabstr(alar_header, 5, 8, f->uidname = uidname);
							getlabstr(alar_header, 13, 8, f->gidname = gidname);
							f->st->st_mtime = getlabnum(alar_header, 31, 10, 10);
							f->st->st_ctime = getlabnum(alar_header, 41, 10, 10);
							f->st->st_atime = getlabnum(alar_header, 51, 10, 10);
							f->st->st_size = getlabnum(alar_header, 61, 10, 10);
						}
						else if (type == 4)
						{
							if (f->linktype != NOLINK)
							{
								if ((n = lab - ALAR_VARHDR) >= sizeof(linkbuffer))
								{
									n = sizeof(linkbuffer) - 1;
									error(1, "%s: link text too long -- truncating to %d", f->name, n);
								}
								getlabstr(alar_header, 10, n, f->linkname = linkbuffer);
								f->linknamesize = n + 1;
							}
							else switch (f->st->st_mode & S_IFMT)
							{
							case S_IFBLK:
							case S_IFCHR:
								i = getlabnum(alar_header, 5, 8, 8);
								num = getlabnum(alar_header, 13, 8, 8);
								f->st->st_rdev = makedev(i, num);
								break;
							case S_IFDIR:
							case S_IFREG:
							case S_IFIFO:
							case S_IFSOCK:
								break;
							default:
								error(1, "%s: unknown file type %07o -- regular file assumed", f->name, f->st->st_mode & S_IFMT);
								f->st->st_mode = S_IFREG|(f->st->st_mode & S_IFMT);
								break;
							}
						}
						else if (type == 5)
						{
							if ((n = lab - ALAR_VARHDR) >= sizeof(namebuffer))
							{
								n = sizeof(namebuffer) - 1;
								error(1, "%s: name too long -- truncating to %d", f->name, n);
							}
							getlabstr(alar_header, 10, n, f->name = namebuffer);
							f->namesize = n + 1;
						}
					}
				}
				else if (!state.in.blocked && strneq(alar_header, "VOL1", 4))
				{
					bunread(alar_header, lab);
					if (!(getprologue())) return(0);
					goto volume;
				}
			} while ((lab = getlabel(f)));
#if SAVESET
			if (state.formatin != SAVESET) goto found;
			state.saveset.time = f->st->st_mtime;
			if (state.blocksize > state.saveset.blocksize)
			{
				state.saveset.blocksize = state.blocksize;
				if (state.saveset.block) free(state.saveset.block);
				if (!(state.saveset.block = malloc(state.saveset.blocksize)))
					error(3, "cannot allocate %s format buffer", format[state.formatin].name);
			}
			state.saveset.bp = state.saveset.block + state.blocksize;
			/*FALLTHROUGH*/
		case SAVESET:
			f->name = namebuffer;
			if (!getsaveset(f, 1)) goto again;
#endif
			goto found;
		case BINARY:
			if (bread(&binary_header, (long)BINARY_HEADER, (long)BINARY_HEADER, 0) <= 0) break;
			if (state.swap)
			{
				memcpy(state.tmp.buffer, &binary_header, BINARY_HEADER);
				memswap(state.swap, (char*)&binary_header, BINARY_HEADER);
			}
			f->magic = binary_header.magic;
			if (f->magic == CPIO_MAGIC)
			{
				f->namesize = binary_header.namesize;
				f->st->st_dev = binary_header.dev;
				f->st->st_ino = binary_header.ino;
				f->st->st_mode = binary_header.mode;
				f->st->st_uid = binary_header.uid;
				f->st->st_gid = binary_header.gid;
				f->st->st_nlink = binary_header.links;
				f->st->st_rdev = binary_header.rdev;
				f->st->st_mtime = cpio_long(binary_header.mtime);
				f->st->st_size = cpio_long(binary_header.size);
			cpio_common:
				f->linktype = NOLINK;
				f->linkname = 0;
				f->uidname = 0;
				f->gidname = 0;
				f->name = namebuffer;
				switch (state.formatin)
				{
				case BINARY:
					i = BINARY_ALIGN;
					n = BINARY_HEADER;
					break;
				case ASC:
				case ASCHK:
					i = ASC_ALIGN;
					n = ASC_HEADER;
					break;
				default:
					i = 0;
					break;
				}
				if (i)
				{
					if (n = (n + f->namesize) % i) i -= n;
					else i = 0;
				}
				if (f->namesize >= sizeof(namebuffer))
				{
					error(2, "entry %d.%d file name too long", state.volume, state.entry);
					for (n = f->namesize + i; n > 0; n -= sizeof(namebuffer))
						(void)bread(namebuffer, 0L, n > sizeof(namebuffer) ? (long)sizeof(namebuffer) : n, 1);
					f->skip = 1;
				}
				else
				{
					(void)bread(namebuffer, 0, (long)(f->namesize + i), 1);
					if (namebuffer[f->namesize - 1])
					{
						bunread(&namebuffer[f->namesize - 1], 1);
						namebuffer[f->namesize - 1] = 0;
						error(state.keepgoing ? 1 : 3, "entry %d.%d file name terminating null missing", state.volume, state.entry);
					}
#if CPIO_EXTENDED
					getxops(f);
#endif
				}
				if (streq(f->name, CPIO_TRAILER))
				{
					getdeltaheader(f);
					return(0);
				}
				switch (f->st->st_mode & CPIO_FMT)
				{
				case CPIO_FIFO:
					f->type = S_IFIFO;
					break;
				case CPIO_DIR:
					f->type = S_IFDIR;
					break;
				case CPIO_CHR:
					f->type = S_IFCHR;
					break;
				case CPIO_BLK:
					f->type = S_IFBLK;
					break;
				case CPIO_LNK:
					f->type = S_IFLNK;
					break;
				case CPIO_SOCK:
					f->type = S_IFSOCK;
					break;
				default:
					error(1, "%s: unknown file type %07o -- regular file assumed", f->name, f->st->st_mode & S_IFMT);
					/*FALLTHROUGH*/
				case CPIO_REG:
					f->type = S_IFREG;
					break;
				}
				f->st->st_mode &= 07777;
				f->st->st_mode |= f->type;
#if DEBUG
				if (f->st->st_mtime == 1 && f->st->st_size == 1) repair(f);
#endif
				switch (f->type)
				{
				case S_IFLNK:
					getdeltaheader(f);
					if (f->st->st_size > sizeof(linkbuffer) - 1)
					{
						error(2, "entry %d.%d symbolic link text too long", state.volume, state.entry);
						f->skip = 1;
					}
					else
					{
						f->linktype = SOFTLINK;
						f->linkname = linkbuffer;
						(void)bread(linkbuffer, 0L, f->st->st_size, 1);
						linkbuffer[f->st->st_size] = 0;
						f->st->st_size = 0;
					}
					break;
				default:
					f->linktype = NOLINK;
					break;
				}
				goto found;
			}
			bunread(state.swap ? state.tmp.buffer : (char*)&binary_header, BINARY_HEADER);
			break;
		case CPIO:
			if (bread(state.tmp.buffer, 0L, (long)CPIO_HEADER, 0) <= 0) break;
			state.tmp.buffer[CPIO_HEADER] = 0;
			if (state.tmp.buffer[0] == '0' && sscanf(state.tmp.buffer, "%6o%6lo%6lo%6lo%6lo%6lo%6lo%6lo%11lo%6o%11lo",
				&f->magic,
				&lst.dev,
				&lst.ino,
				&lst.mode,
				&lst.uid,
				&lst.gid,
				&lst.nlink,
				&lst.rdev,
				&lst.mtime,
				&f->namesize,
				&lst.size) == 11 && f->magic == CPIO_MAGIC)
			{
				f->st->st_dev = lst.dev;
				f->st->st_ino = lst.ino;
				f->st->st_mode = lst.mode;
				f->st->st_uid = lst.uid;
				f->st->st_gid = lst.gid;
				f->st->st_nlink = lst.nlink;
				f->st->st_rdev = lst.rdev;
				f->st->st_mtime = lst.mtime;
				f->st->st_size = lst.size;
				goto cpio_common;
			}
			bunread(state.tmp.buffer, CPIO_HEADER);
			break;
		case TAR:
		case USTAR:
			if (bread(tar_block, 0L, (long)TAR_HEADER, 0) <= 0) break;
			if (!*tar_header.name)
			{
				if (state.entry == 1) goto notar;
				return(0);
			}
			if (sscanf(tar_header.mode, "%11lo", &num) != 1) goto notar;
			f->st->st_mode = num;
			if (sscanf(tar_header.uid, "%11lo", &num) != 1) goto notar;
			f->st->st_uid = num;
			if (sscanf(tar_header.gid, "%11lo", &num) != 1) goto notar;
			f->st->st_gid = num;
			if (sscanf(tar_header.size, "%11lo", &num) != 1) goto notar;
			f->st->st_size = num;
			if (sscanf(tar_header.mtime, "%11lo", &num) != 1) goto notar;
			f->st->st_mtime = num;
			if (sscanf(tar_header.chksum, "%11lo", &num) != 1) goto notar;
			if ((n = num) != (i = tar_checksum()))
			{
				if (state.entry == 1) goto notar;
				error(state.keepgoing ? 1 : 3, "%s format checksum error (%d != %d)", format[state.formatin].name, n, i);
			}
			if (state.formatin == USTAR)
			{
				if (!streq(tar_header.magic, TMAGIC))
				{
					if (strneq(tar_header.magic, TMAGIC, TMAGLEN - 1)) error(1, "%s format botched -- %s format assumed", format[state.formatin].name, format[TAR].name);
					else if (state.entry > 1) goto notar;
					state.formatin = TAR;
				}
				else if (!strneq(tar_header.version, TVERSION, sizeof(tar_header.version)))
				{
					error(1, "%s format version %-.*s incompatible with implementation version %-.*s -- assuming %s", format[state.formatin].name, sizeof(tar_header.version), tar_header.version, sizeof(tar_header.version), TVERSION, format[TAR].name);
					state.formatin = TAR;
				}
			}
			tar_header.name[sizeof(tar_header.name)] = 0;
			if (state.formatin == USTAR && *tar_header.prefix) (void)sprintf(f->name = namebuffer, "%-.%*s%s", sizeof(tar_header.prefix), tar_header.prefix, tar_header.name);
			else f->name = tar_header.name;
			tar_header.linkname[sizeof(tar_header.name)] = 0;
			f->linkname = tar_header.linkname;
			f->linktype = NOLINK;
			f->st->st_nlink = 1;
			switch (tar_header.typeflag)
			{
			case LNKTYPE:
				f->linktype = HARDLINK;
				f->st->st_mode |= S_IFREG;
				f->st->st_nlink = 2;
				f->st->st_size = 0;	/* lest they forget */
				break;
			case SYMTYPE:
				f->linktype = SOFTLINK;
				f->st->st_mode |= S_IFLNK;
				break;
			case CHRTYPE:
				f->st->st_mode |= S_IFCHR;
			device:
				if (sscanf(tar_header.devmajor, "%11o", &num) != 1) goto notar;
				i = num;
				if (sscanf(tar_header.devminor, "%11o", &num) != 1) goto notar;
				f->st->st_rdev = makedev(i, num);
				break;
			case BLKTYPE:
				f->st->st_mode |= S_IFBLK;
				goto device;
			case DIRTYPE:
				f->st->st_mode |= S_IFDIR;
				break;
			case FIFOTYPE:
				f->st->st_mode |= S_IFIFO;
				break;
#ifdef SOKTYPE
			case SOKTYPE:
				f->st->st_mode |= S_IFSOCK;
				break;
#endif
			default:
				error(1, "unknown file type `%c' -- regular file assumed", tar_header.typeflag);
				/*FALLTHROUGH*/
			case REGTYPE:
			case AREGTYPE:
				f->namesize = strlen(f->name) + 1;
				if (f->name[f->namesize - 2] == '/')
				{
					f->st->st_mode |= S_IFDIR;
					if (f->namesize > 2) f->name[--f->namesize - 1] = 0;
				}
				else f->st->st_mode |= S_IFREG;
				break;
			}
			f->uidname = 0;
			f->gidname = 0;
			if (state.formatin == USTAR)
			{
				if (*tar_header.uname) f->uidname = tar_header.uname;
				if (*tar_header.gname) f->gidname = tar_header.gname;
			}
			goto found;
		notar:
			bunread(tar_block, TAR_HEADER);
			break;
		case ASC:
		case ASCHK:
			if (bread(state.tmp.buffer, 0L, (long)ASC_HEADER, 0) <= 0) break;
			state.tmp.buffer[ASC_HEADER] = 0;
			if (state.tmp.buffer[0] == '0' && sscanf(state.tmp.buffer, "%6o%8lx%8lx%8lx%8lx%8lx%8lx%8lx%8lx%8lx%8lx%8lx%8x%8lx",
				&f->magic,
				&lst.ino,
				&lst.mode,
				&lst.uid,
				&lst.gid,
				&lst.nlink,
				&lst.mtime,
				&lst.size,
				&lst.dev_major,
				&lst.dev_minor,
				&lst.rdev_major,
				&lst.rdev_minor,
				&f->namesize,
				&lst.checksum) == 14 && (f->magic == ASC_MAGIC || f->magic == ASCHK_MAGIC))
			{
				if (f->magic == ASCHK_MAGIC) state.formatin = ASCHK;
				f->checksum = lst.checksum;
				f->st->st_dev = makedev(lst.dev_major, lst.dev_minor);
				f->st->st_ino = lst.ino;
				f->st->st_mode = lst.mode;
				f->st->st_uid = lst.uid;
				f->st->st_gid = lst.gid;
				f->st->st_nlink = lst.nlink;
				f->st->st_rdev = makedev(lst.rdev_major, lst.rdev_minor);
				f->st->st_mtime = lst.mtime;
				f->st->st_size = lst.size;
				goto cpio_common;
			}
			bunread(state.tmp.buffer, ASC_HEADER);
			break;
		case PORTAR:
		case RANDAR:
			if (bread(&portar_header, 0L, (long)PORTAR_HEADER, 0) <= 0) return(0);
			if (strneq(portar_header.ar_fmag, PORTAR_END, PORTAR_ENDSIZ) && sscanf(portar_header.ar_date, "%12ld%6ld%6ld%8lo%10ld",
				&lst.mtime,
				&lst.uid,
				&lst.gid,
				&lst.mode,
				&lst.size) == 5)
			{
				f->name = portar_header.ar_name;
				portar_header.ar_date[0] = 0;
				if ((s = strchr(f->name, PORTAR_TERM)) || (s = strchr(f->name, RANDAR_TERM))) *s = 0;
				f->st->st_dev = 0;
				f->st->st_ino = 0;
				f->st->st_mode = S_IFREG|(lst.mode&07777);
				f->st->st_uid = lst.uid;
				f->st->st_gid = lst.gid;
				f->st->st_nlink = 1;
				f->st->st_rdev = 0;
				f->st->st_mtime = lst.mtime;
				f->st->st_size = lst.size;
				f->linktype = NOLINK;
				f->linkname = 0;
				f->uidname = 0;
				f->gidname = 0;
				goto found;
			}
			bunread(&portar_header, PORTAR_HEADER);
			return(0);
		default:
			error(PANIC, "%s: incomplete input format implementation", format[state.formatin].name);
			break;
		}
		if (state.entry == 1) for (;;) switch (state.formatin)
		{
		case BINARY:
			state.formatin = USTAR;
			goto again;
		case CPIO:
			state.formatin = BINARY;
			state.swap = 0;
			if (bread(&binary_header.magic, 0L, (long)sizeof(binary_header.magic), 0) <= 0) break;
			bunread(&binary_header.magic, sizeof(binary_header.magic));
			if (binary_header.magic == CPIO_MAGIC) goto again;
			state.swap = BYTE;
			magic = binary_header.magic;
			memswap(state.swap, (char*)&magic, sizeof(magic));
#if DEBUG
			message(-1, "binary_header.magic=0%05o swap(BYTE)=0%05o", (unsigned short)binary_header.magic, (unsigned short)magic);
#endif
			if (magic == CPIO_MAGIC)
			{
#if DEBUG
				message(-1, "swapping %s header bytes", format[state.formatin].name);
#endif
				goto again;
			}
			break;
		case TAR:
		case USTAR:
			state.formatin = ASC;
			goto again;
		case ASC:
		case ASCHK:
			if (state.entries > 0 && state.volume > 1)
			{
				if (--state.volume > 1) error(1, "junk data after volume %d", state.volume);
				finish(0);
			}
			if (!state.keepgoing) error(3, "%s: unknown input format", state.file);
			state.formatin = IN_DEFAULT;
			goto skip;
		}
	skip:
		i = 3;
		if (state.keepgoing && bread(namebuffer, 0L, 1L, 0) > 0)
		{
			if (warned) continue;
			warned = 1;
			i = 1;
		}
		error(i, "entry %d.%d is out of phase", state.volume, state.entry);
		if (state.entry > 1) state.entry++;
	}
 found:
	if (checkdelta)
	{
		checkdelta = 0;
		i = 0;
#if new_delta_format
		if (!f->st->st_size && !f->st->st_dev && !f->st->st_ino && !(f->st->st_mode & (S_IRWXU|S_IRWXG|S_IRWXO)) && (s = f->name) && *s++ == INFO_SEP && *(s + strlen(s) - 1) == INFO_SEP)
		{
			if (strneq(s, ID, IDLEN) && (s = strchr(s, INFO_SEP)))
			{
			deltaverify:
				switch (*++s)
				{
				case TYPE_COMPRESS:
					typename = "compressed ";
					break;
				case TYPE_DELTA:
					typename = "delta ";
					break;
				default:
					error(3, "type %c encoding not supported", *s);
				}
				if (*++s != VERSION)
					error(3, "version %c encoding not supported", *s);

				/*
				 * NOTE: [<INFO_SEP><OP><VAL>]* may appear here
				 */

				i = 1;
			}
			else
			{
				if (s = strchr(f->name + 3, INFO_SEP)) *s = 0;
				error(1, "unknown %s header ignored", f->name + 3);
				goto again;
			}
		}
#else
		if (!f->st->st_size && !f->st->st_dev && !f->st->st_ino && !(f->st->st_mode & (S_IRWXU|S_IRWXG|S_IRWXO)) && streq(f->name, "DELTA!!!"))
		{
		deltaverify:
			typename = "delta ";
			i = 1;
		}
#endif
		n = (state.delta.op & (COLLECT|IN)) == IN;
#if DEBUG
		message(-5, "checkdelta: %sop=%d i=%d n=%d size=%ld:%ld LO=%d:%d HI=%d:%d", typename, state.delta.op, i, n, f->st->st_mtime, state.delta.size, DELTA_LO(f->st->st_uid), DELTA_LO(state.delta.checksum), DELTA_LO(f->st->st_gid), DELTA_HI(state.delta.checksum));
#endif
		if (i != n)
		{
			if (n) error(3, "%s: not a delta archive", state.file);
			else if (!(state.delta.op & COLLECT))
			{
				if (*typename == 'd') error(state.list ? 1 : 3, "%s: base archive must be specified", state.file);
				/*HUH*/if (!(state.delta.op & IN)) deltabase();
			}
		}
		if (i)
		{
			if (n)
			{
				if ((i = f->st->st_mtime != state.delta.size) || DELTA_LO(f->st->st_uid) != DELTA_LO(state.delta.checksum) || DELTA_LO(f->st->st_gid) != DELTA_HI(state.delta.checksum))
					error(i ? 3 : 1, "base archive %s mismatch", i ? "size" : "checksum");
			}
			if (!f->st->st_size) goto again;
		}
	}
	state.entries++;
	f->path = strcpy(pathbuffer, f->name);
	(void)pathcanon(f->name);
	f->name = map(f->name);
	f->namesize = strlen(f->name) + 1;
	if (f->linkname)
	{
		(void)pathcanon(f->linkname);
		if (!(state.ftwflags & FTW_PHYSICAL)) f->linkname = map(f->linkname);
		f->linknamesize = strlen(f->linkname) + 1;
	}
	else f->linknamesize = 0;
	if (f->uidname || f->gidname) setidnames(f);
	f->type = f->st->st_mode & S_IFMT;
	getdeltaheader(f);
#if DEBUG
	if (state.trace)
	{
		s = &state.tmp.buffer[0];
		if (f->record.format) (void)sprintf(s, " [%c,%d,%d]", f->record.format, state.blocksize, state.record.size);
		else *s = 0;
		message(-1, "path=%s name=%s entry=%d.%d size=%ld%s", f->path, f->name, state.volume, state.entry, f->st->st_size, s);
	}
#endif
	if (state.entry == 1)
	{
		if (state.delta.op & COLLECT)
		{
			if (state.in.blocked) error(3, "%s: blocked base archives are not supported", state.delta.base);
			switch (state.formatin)
			{
			case ALAR:
			case IBMAR:
#if SAVESET
			case SAVESET:
#endif
				error(3, "%s: %s format base archives not supported", state.delta.base, format[state.formatin].name);
			}
		}
		if (state.summary && state.verbose)
		{
			if (state.in.blok) fprintf(stderr, "BLOK ");
			if (state.delta.op & COLLECT) fprintf(stderr, "base");
			else fprintf(stderr, "volume %d", state.volume);
			if (state.id.volume[0]) fprintf(stderr, " label %s", state.id.volume);
			fprintf(stderr, " in %s%s format", typename, format[state.formatin].name);
			if (state.trace)
			{
				if (*state.id.format) fprintf(stderr, " %s", state.id.format);
				if (*state.id.implementation) fprintf(stderr, " implementation %s", state.id.implementation);
			}
			fprintf(stderr, "\n");
		}
	}
	state.delta.sum++;
	return(1);
}

/*
 * write next archive entry header
 */

void
putheader(f)
register struct fileinfo*	f;
{
	register char*	s;
	register int	n;

	if (f->delta.op)
	{
		s = state.delta.hdrbuf;
#if new_delta_format
		*s++ = f->delta.op;
		*s++ = VERSION;
		*s++ = 0;
		state.delta.hdr = s;
		adddelnum('s', f->delta.size);
		*state.delta.hdr++ = 0;
#else
		*s++ = f->delta.op;
		*s++ = '2';
		*s++ = '\n';
		state.delta.hdr = s;
#endif
		n = state.delta.hdr - state.delta.hdrbuf;
		f->st->st_size += n;
		f->linknamesize += n;
	}
	if (state.complete)
	{
		unsigned long	c = f->st->st_size;

		switch (state.formatout)
		{
		case ALAR:
		case IBMAR:
		case PAX:
			c += 4 * ALAR_HEADER;
			break;
		case ASC:
		case ASCHK:
			c += ASC_HEADER + f->namesize;
			break;
		case BINARY:
			c += BINARY_HEADER + f->namesize;
			c = round(c, 2);
			break;
		case CPIO:
			c += CPIO_HEADER + f->namesize;
			break;
		case TAR:
		case USTAR:
			c += TAR_HEADER;
			c = round(c, BLOCKSIZE);
			break;
		}
		if (state.out.count + c > state.maxout)
		{
			if (c > state.maxout) error(1, "%s: too large to fit in one volume", f->name);
			else
			{
				state.complete = 0;
				putepilogue();
				newio(1, 0, 0);
				putprologue();
				state.complete = 1;
			}
		}
	}
	switch (state.formatout)
	{
	case ALAR:
	case IBMAR:
	case PAX:
		putlabels(f, "HDR");
		break;
	case BINARY:
		binary_header.magic = CPIO_MAGIC;
		binary_header.namesize = f->namesize;
		cpio_short(binary_header.size, f->type == S_IFLNK ? (long)f->linknamesize : f->st->st_size);
		binary_header.dev = f->st->st_dev;
		binary_header.ino = f->st->st_ino;
		binary_header.mode = cpio_mode(f);
		binary_header.uid = f->st->st_uid;
		binary_header.gid = f->st->st_gid;
		binary_header.links = f->st->st_nlink;
		binary_header.rdev = f->st->st_rdev;
		if (binary_header.rdev != f->st->st_rdev) error(1, "%s: special device numbers truncated", f->name);
		cpio_short(binary_header.mtime, (long)f->st->st_mtime);
		bwrite(&binary_header, BINARY_HEADER);
		bwrite(f->name, f->namesize);
		if (n = (BINARY_HEADER + f->namesize) % BINARY_ALIGN)
			while (n++ < BINARY_ALIGN) bwrite("", 1);
		if (f->type == S_IFLNK)
		{
		cpio_link:
			putdeltaheader(f);
			if (streq(f->name, f->linkname)) error(1, "%s: symbolic link loops to self", f->name);
			bwrite(f->linkname, f->linknamesize);
		}
		break;
	case CPIO:
#if CPIO_EXTENDED
		if (!f->skip)
		{
			getidnames(f);
			addxopstr('U', f->uidname);
			addxopstr('G', f->gidname);
			if (CPIO_TRUNCATE(f->st->st_rdev) != f->st->st_rdev) addxopnum('d', f->st->st_rdev);
#if NUMBER_EVEN_THOUGH_NAME
			if (CPIO_TRUNCATE(f->st->st_gid) != f->st->st_gid) addxopnum('g', f->st->st_gid);
			if (CPIO_TRUNCATE(f->st->st_uid) != f->st->st_uid) addxopnum('u', f->st->st_uid);
#endif
			setxops(f);
		}
#else
		if (CPIO_TRUNCATE(f->st->st_rdev) != f->st->st_rdev) error(1, "%s: special device number truncated", f->name);
		if (CPIO_TRUNCATE(f->st->st_gid) != f->st->st_gid) error(1, "%s: gid number truncated", f->name);
		if (CPIO_TRUNCATE(f->st->st_uid) != f->st->st_uid) error(1, "%s: uid number truncated", f->name);
#endif
		(void)sprintf(state.tmp.buffer, "%.6o%.6o%.6o%.6o%.6o%.6o%.6o%.6o%.11lo%.6o%.11lo",
			CPIO_TRUNCATE(CPIO_MAGIC),
			CPIO_TRUNCATE(f->st->st_dev),
			CPIO_TRUNCATE(f->st->st_ino),
			CPIO_TRUNCATE(cpio_mode(f)),
			CPIO_TRUNCATE(f->st->st_uid),
			CPIO_TRUNCATE(f->st->st_gid),
			CPIO_TRUNCATE(f->st->st_nlink),
			CPIO_TRUNCATE(f->st->st_rdev),
			f->st->st_mtime,
			f->namesize,
			f->type == S_IFLNK ? f->linknamesize : f->st->st_size);
		bwrite(state.tmp.buffer, CPIO_HEADER);
#if CPIO_EXTENDED
		putxops(f);
#else
		bwrite(f->name, f->namesize);
#endif
		if (f->type == S_IFLNK) goto cpio_link;
		break;
	case ASC:
		f->checksum = 0;
		/*FALLTHROUGH*/
	case ASCHK:
		(void)sprintf(state.tmp.buffer, "%.6o%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx%.8lx",
			state.formatout == ASC ? ASC_MAGIC : ASCHK_MAGIC,
			(long)f->st->st_ino,
			(long)cpio_mode(f),
			(long)f->st->st_uid,
			(long)f->st->st_gid,
			(long)f->st->st_nlink,
			(long)f->st->st_mtime,
			f->type == S_IFLNK ? (long)f->linknamesize : (long)f->st->st_size,
			(long)major(f->st->st_dev),
			(long)minor(f->st->st_dev),
			(long)major(f->st->st_rdev),
			(long)minor(f->st->st_rdev),
			(long)f->namesize,
			f->checksum);
		bwrite(state.tmp.buffer, ASC_HEADER);
		bwrite(f->name, f->namesize);
		if (n = (ASC_HEADER + f->namesize) % ASC_ALIGN)
			while (n++ < ASC_ALIGN) bwrite("", 1);
		if (f->type == S_IFLNK) goto cpio_link;
		break;
	case TAR:
	case USTAR:
message(-1, "tar_header#1: name=%s namesize=%d max=%d", f->name, f->namesize, sizeof(tar_header.name));
		memset(tar_block, 0, TAR_HEADER);
		if (f->namesize > sizeof(tar_header.name) + 1)
		{
			(void)sprintf(tar_header.prefix, "%-.*s", sizeof(tar_header.prefix), f->name);
			s = f->name + sizeof(tar_header.prefix);
		}
		else s = f->name;
		(void)sprintf(tar_header.name, "%s%s", s, f->type == S_IFDIR ? "/" : "");
		switch (f->linktype)
		{
		case HARDLINK:
			tar_header.typeflag = LNKTYPE;
		linked:
			(void)sprintf(tar_header.linkname, "%s", f->linkname);
			break;
		case SOFTLINK:
			tar_header.typeflag = SYMTYPE;
			goto linked;
		default:
			switch (state.formatout == USTAR ? f->type : S_IFREG)
			{
			case S_IFCHR:
				tar_header.typeflag = CHRTYPE;
			special:
				(void)sprintf(tar_header.devmajor, "%*o ", sizeof(tar_header.devmajor) - 2, major(f->st->st_rdev));
				(void)sprintf(tar_header.devminor, "%*o ", sizeof(tar_header.devminor) - 2, minor(f->st->st_rdev));
				break;
			case S_IFBLK:
				tar_header.typeflag = BLKTYPE;
				goto special;
			case S_IFDIR:
				tar_header.typeflag = DIRTYPE;
				break;
			case S_IFIFO:
				tar_header.typeflag = FIFOTYPE;
				break;
			case S_IFSOCK:
				tar_header.typeflag = SOKTYPE;
				goto special;
			default:
				if (!f->skip) error(1, "%s: unknown file type 0%03o -- assuming regular file", f->name, f->type >> 12);
				/*FALLTHROUGH*/
			case S_IFREG:
				tar_header.typeflag = REGTYPE;
				break;
			}
			break;
		}
		(void)sprintf(tar_header.mode, "%*o ", sizeof(tar_header.mode) - 2, f->st->st_mode & 07777);
		(void)sprintf(tar_header.uid, "%*o ", sizeof(tar_header.uid) - 2, f->st->st_uid);
		(void)sprintf(tar_header.gid, "%*o ", sizeof(tar_header.gid) - 2, f->st->st_gid);
		(void)sprintf(tar_header.size, "%*lo ", sizeof(tar_header.size) - 1, f->st->st_size);
		(void)sprintf(tar_header.mtime, "%*lo ", sizeof(tar_header.mtime) - 2, f->st->st_mtime);
		if (state.formatout == USTAR)
		{
			(void)strncpy(tar_header.magic, TMAGIC, sizeof(tar_header.magic));
			(void)strncpy(tar_header.version, TVERSION, sizeof(tar_header.version));
			getidnames(f);
			(void)strcpy(tar_header.uname, f->uidname);
			(void)strcpy(tar_header.gname, f->gidname);
		}
		(void)sprintf(tar_header.chksum, "%*o ", sizeof(tar_header.chksum) - 2, tar_checksum());
		bwrite(tar_block, TAR_HEADER);
		break;
	}
	putdeltaheader(f);
}

/*
 * read entry trailer
 */

/*ARGSUSED*/
void
gettrailer(f)
struct fileinfo*	f;
{
	register int	n;

	if (state.delta.sum-- > 0)
	{
		state.delta.checksum ^= state.delta.membersum;
		state.delta.membersum = 0;
	}
	if ((n = format[state.formatin].align) && (n = round(state.in.count, n) - state.in.count))
		(void)bread(state.tmp.buffer, 0L, (long)n, 1);
}

/*
 * write entry trailer
 */

void
puttrailer(f)
register struct fileinfo*	f;
{
	register int	n;

	switch (state.formatout)
	{
	case ALAR:
	case IBMAR:
	case PAX:
		putlabels(f, "EOF");
		break;
	}
	if ((n = format[state.formatout].align) && (n = round(state.out.count, n) - state.out.count))
	{
		memset(state.tmp.buffer, 0, n);
		bwrite(state.tmp.buffer, n);
	}
	listentry(f);
}

/*
 * return length of next label
 * variable length labels have label number > 3 and Vnnnn at position 5
 * where nnnn is the decimal length of the entire label
 * nnnn may be < ALAR_HEADER but label block must be >= ALAR_HEADER
 * 0 returned at end of label group
 */

int
getlabel(f)
register struct fileinfo*	f;
{
	register int	c;
	register int	n;

	static char	last[5];
	static int	done;

	if (done || (c = bread(alar_header, (long)ALAR_HEADER, (long)ALAR_LABEL, 0)) < ALAR_HEADER) return(*last = done = c = 0);
	if (state.formatin == IBMAR) cvtetoa(alar_header, alar_header, ALAR_HEADER);
	if (alar_header[4] == 'V' && ((n = getlabnum(alar_header, 4, 1, 10)) < 1 || n > 3) && (n = getlabnum(alar_header, 6, 4, 10)) != c)
	{
		if ((c = n - c) > 0)
		{
			if (state.in.blocked || bread(alar_header + ALAR_HEADER, 0L, (long)c, 1) != c)
			{
				c = ALAR_HEADER;
				error(2, "%s: %-*.*s: variable length label record too short", f->name, c, c, alar_header);
			}
			else
			{
				if (state.formatin == IBMAR) cvtetoa(alar_header + ALAR_HEADER, alar_header + ALAR_HEADER, c);
				c = n;
			}
		}
		else if (n <= ALAR_VARHDR) c = ALAR_VARHDR;
		else c = n;
	}
	if (!state.in.blocked && !*last && alar_header[3] == '2' && (strneq(alar_header, "HDR", 3) || strneq(alar_header, "EOF", 3) || strneq(alar_header, "EOV", 3)))
		getlabstr(alar_header, 26, 4, last);
	if (*last && strneq(alar_header, last, 4)) done = 1;
#if DEBUG
	message(-4, "label: %-*.*s", c, c, alar_header);
#endif
	return(c);
}

/*
 * output file HDR/EOF/EOV labels
 */

void
putlabels(f, type)
register struct fileinfo*	f;
char*				type;
{
	register int	n;
	char*		s;
	struct tm*	tm;

	static int	section = 1;
	static int	sequence;

	switch (*type)
	{
	case 'E':
		bwrite(alar_header, 0);
		break;
	case 'H':
		sequence++;
		break;
	}
	tm = gmtime(&f->st->st_mtime);
	(void)sprintf(alar_header, "%s1%-17.17s000001%04d%04d000100 %02d%03d 00000 %06d%-6.6s%-7.7s       ", type, f->id, section, sequence, tm->tm_year, tm->tm_yday, f->record.blocks, state.id.format, state.id.implementation);
	if (state.formatout == IBMAR) cvtatoe(alar_header, alar_header, ALAR_HEADER);
	bwrite(alar_header, ALAR_HEADER);
	(void)sprintf(alar_header, "%s2%c%05d%05d%010d%s%c                     00                            ", type, state.record.format, state.blocksize, state.record.size, f->st->st_size, type, state.formatout == PAX && *type == 'H' ? '5' : '2');
	if (state.formatout == IBMAR) cvtatoe(alar_header, alar_header, ALAR_HEADER);
	bwrite(alar_header, ALAR_HEADER);
	if (state.formatout == PAX && *type == 'H')
	{
		getidnames(f);
		s = fmtmode(f->st->st_mode);
		if (f->linktype == HARDLINK) *s = 'z';
		(void)sprintf(alar_header, "%s3%-8.8s%-8.8s%-10.10s%010d%010d%010d%010d          ", type, f->uidname, f->gidname, s, f->st->st_mtime, f->st->st_ctime, f->st->st_atime, f->st->st_size);
		bwrite(alar_header, ALAR_HEADER);
		n = ALAR_HEADER;
		if (f->linkname)
		{
			if ((n = f->linknamesize + 8) < ALAR_HEADER) n = ALAR_HEADER;
			(void)sprintf(alar_header, "%s4V%04d%-80s", type, f->linknamesize + 8, f->linkname);
		}
		else switch (f->type)
		{
		case S_IFBLK:
		case S_IFCHR:
			(void)sprintf(alar_header, "%s4%08o%08o%-15.15s                                             ", type, major(f->st->st_rdev), minor(f->st->st_rdev), SYSTEM);
			break;
		case S_IFDIR:
		case S_IFREG:
		case S_IFIFO:
			(void)sprintf(alar_header, "%s4                                                                            ", type);
			break;
		default:
			(void)sprintf(alar_header, "%s4V%04d                                                                       ", type, ALAR_VARHDR);
			break;
		}
		bwrite(alar_header, n);
		if ((n = f->namesize + 8) < ALAR_HEADER) n = ALAR_HEADER;
		(void)sprintf(alar_header, "%s5V%04d%-80s", type, f->namesize + 8, f->name);
		bwrite(alar_header, n);
	}
	bwrite(alar_header, 0);
	if (streq(type, "EOV"))
	{
		section++;
		sequence = 0;
	}
	else section = 1;
}

#ifdef SAVESET

/*
 * get next saveset record
 * if header!=0 then all records skipped until REC_file found
 * otherwise REC_vbn cause non-zero return
 * 0 returned for no record match
 */

int
getsaveset(f, header)
register struct fileinfo*	f;
int				header;
{
	register char*	p;
	register char*	s;
	char*		t;
	int		i;
	long		n;

	for (;;)
	{
		state.saveset.bp += state.saveset.lastsize;
		while (state.saveset.bp >= state.saveset.block + state.blocksize)
		{
			state.saveset.bp = state.saveset.block;
			state.saveset.lastsize = 0;
			if (bread(state.saveset.bp, 0L, (long)state.blocksize, 0) <= 0)
			{
				state.formatin = ALAR;
				if (header) gettrailer(f);
				return(0);
			}
			if (gethalf(BYTE|HALF, state.saveset.bp + BLKHDR_hdrsiz) != BLKHDR_SIZE)
				error(3, "invalid %s format block header", format[state.formatin].name);
			if (!(n = getlong(BYTE|HALF, state.saveset.bp + BLKHDR_blksiz)))
				state.saveset.bp += state.blocksize;
			else if (n != state.blocksize)
				error(3, "invalid %s format blocksize=%ld", format[state.formatin].name, n);
			state.saveset.bp += BLKHDR_SIZE;
		}
		state.saveset.lastsize = gethalf(BYTE|HALF, state.saveset.bp + RECHDR_size);
		i = gethalf(BYTE|HALF, state.saveset.bp + RECHDR_type);
		state.saveset.bp += RECHDR_SIZE;
#if DEBUG
		message(-2, "record: type=%d size=%d", i, state.saveset.lastsize);
#endif
		if (i == REC_file)
		{
			if (header)
			{
				p = state.saveset.bp;
				if (gethalf(BYTE|HALF, p) != FILHDR_MAGIC)
					error(3, "invalid %s format file header", format[state.formatin].name);
				p += 2;
				i = gethalf(BYTE|HALF, p + FILHDR_size);
				if (p + FILHDR_data + i > state.saveset.block + state.blocksize)
					error(3, "invalid %s format file attribute", format[state.formatin].name);
				t = f->name;
				n = 0;
				for (s = p + FILHDR_data + 1; s < p + FILHDR_data + i; s++)
				{
					if (isupper(*s)) *t++ = tolower(*s);
					else if (n)
					{
						if (*s == ';') break;
						*t++ = *s;
					}
					else if (*s == ']')
					{
						n = 1;
						*t++ = '/';
					}
					else if (*s == '.') *t++ = '/';
					else *t++ = *s;
				}
				*t = 0;
				for (i = 0; i < 5; i++)
				{
					s = p + FILHDR_size;
					if ((p += FILHDR_SIZE + gethalf(BYTE|HALF, s)) > state.saveset.block + state.blocksize)
						error(3, "invalid %s format file attribute", format[state.formatin].name);
				}
				state.saveset.recatt = getbyte(BYTE|HALF, p + FILHDR_data + FILATT_recfmt);
				state.saveset.recfmt = getbyte(BYTE|HALF, p + FILHDR_data + FILATT_recfmt);
				state.saveset.reclen = gethalf(BYTE|HALF, p + FILHDR_data + FILATT_reclen);
				state.saveset.recvfc = gethalf(BYTE|HALF, p + FILHDR_data + FILATT_recvfc);
				f->st->st_size = (long)(gethalf(BYTE|HALF, p + FILHDR_data + FILATT_blocks) - 1) * BLOCKSIZE + (long)gethalf(BYTE|HALF, p + FILHDR_data + FILATT_frag);
				for (; i < 15; i++)
				{
					s = p + FILHDR_size;
					if ((p += FILHDR_SIZE + gethalf(BYTE|HALF, s)) > state.saveset.block + state.blocksize)
						error(3, "invalid %s format file attribute", format[state.formatin].name);
				}

				/*
				 * pure guesswork based on 100 nsec epoch
				 * 17-NOV-1858 00:00:00 GMT
				 */

				if ((n = getlong(BYTE|HALF, p + FILHDR_data + 4) - 7355000) < 0) n = 1;
				else n = (n << 8) + getbyte(BYTE|HALF, p + FILHDR_data + 3);
				f->st->st_mtime = (time_t)n;
				return(1);
			}
			state.saveset.bp -= RECHDR_SIZE;
			state.saveset.lastsize = 0;
			return(0);
		}
		else if (i == REC_vbn && !header) return(1);
	}
}

#endif

#if CPIO_EXTENDED

static char	opsbuf[PATH_MAX];	/* extended ops buffer		*/

static char*	ops = opsbuf;		/* opsbuf output pointer	*/

/*
 * get and execute extended ops from input
 */

static void
getxops(f)
register struct fileinfo*	f;
{
	register char*	p;
	register char*	s;
	register int	c;
	long		n;

	if (f->namesize > (n = strlen(f->name) + 1)) for (p = f->name + n; c = *p++;)
	{
		for (s = p; *p; p++);
		p++;
		n = strtol(s, (char*)0, 16);
#if DEBUG
		message(-2, "%s: entry %d.%d op = %c%s", f->name, state.volume, state.entry, c, s);
#endif
		switch (c)
		{
		case 'd':
			f->st->st_rdev = n;
			break;
		case 'g':
			f->st->st_gid = n;
			break;
		case 'u':
			f->st->st_uid = n;
			break;
		case 'G':
			f->gidname = s;
			break;
		case 'U':
			f->uidname = s;
			break;

			/*
			 * NOTE: ignore unknown ops for future extensions
			 */
		}
	}
}

/*
 * set end of extended ops
 */

static void
setxops(f)
register struct fileinfo*	f;
{
	register int	n;

	if (n = ops - opsbuf)
	{
		n++;
		*ops++ = 0;
		if ((f->namesize += n) > CPIO_NAMESIZE) error(1, "%s: extended ops may crash older cpio programs", f->name);
	}
}

/*
 * output filename and extended ops
 */

static void
putxops(f)
register struct fileinfo*	f;
{
	register int	n;

	n = ops - opsbuf;
	bwrite(f->name, f->namesize -= n);
	if (n) bwrite(ops = opsbuf, n);
}


/*
 * add extended op string
 */

static void
addxopstr(op, s)
int		op;
register char*	s;
{
	register char*	p = ops;
	register char*	e = opsbuf + sizeof(opsbuf) - 3;

	if (p < e)
	{
		*p++ = op;
		while (*s && p < e) *p++ = *s++;
		*p++ = 0;
		ops = p;
	}
#if DEBUG
	if (*s) error(PANIC, "addxopstr('%c',\"%s\") overflow", op, s);
#endif
}

/*
 * add numeric op for current entry
 */

static void
addxopnum(op, n)
int	op;
long	n;
{
	char	buf[17];

	(void)sprintf(buf, "%x", n);
	addxopstr(op, buf);
}

#endif
