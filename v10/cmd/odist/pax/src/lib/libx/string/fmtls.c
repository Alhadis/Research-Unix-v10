/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * ls formatter
 */

#include <ls.h>
#include <tm.h>
#include <stdio.h>

#ifndef S_IFLNK
#define S_IFLNK		0120000
#endif

extern char*		fmtmode();
extern char*		fmtuid();
extern char*		fmtgid();

/*
 * ls formatter
 *
 *	buf	results placed here
 *	name	file name
 *	st	file stat buffer
 *	info	optional info
 *	link	link text if != 0
 *	flags	LS_* flags
 *
 *	return	end of formatted buf
 */

#if __STDC__ || __cplusplus
char*
fmtls(char* buf, const char* name, register struct stat* st, const char* info, const char* link, register int flags)
#else
char*
fmtls(buf, name, st, info, link, flags)
char*			buf;
char*			name;
register struct stat*	st;
char*			info;
char*			link;
register int		flags;
#endif
{
	register char*		s;
	time_t			tm;

	s = buf;
	if (flags & LS_INUMBER)
	{
		(void)sprintf(s, "%*lu ", LS_W_INUMBER - 1, (unsigned long)st->st_ino);
		while (*++s);
	}
	if (flags & LS_BLOCKS)
	{
		(void)sprintf(s, "%*lu ", LS_W_BLOCKS - 1, (unsigned long)tblocks(st));
		while (*++s);
	}
	if (flags & LS_LONG)
	{
		(void)sprintf(s, "%s%3u", fmtmode(st->st_mode), (unsigned int)st->st_nlink);
		while (*++s);
		if (!(flags & LS_NOUSER))
		{
			(void)sprintf(s, " %-*s", LS_W_NAME - 1, fmtuid(st->st_uid));
			while (*++s);
		}
		if (!(flags & LS_NOGROUP))
		{
			(void)sprintf(s, " %-*s", LS_W_NAME - 1, fmtgid(st->st_gid));
			while (*++s);
		}
		switch (st->st_mode & S_IFMT)
		{
		case S_IFBLK:
		case S_IFCHR:
			(void)sprintf(s, " %3u,%3u ", (unsigned int)major(st->st_rdev), (unsigned int)minor(st->st_rdev));
			break;
		default:
			(void)sprintf(s, "%8lu ", (unsigned long)st->st_size);
			break;
		}
		while (*++s);
		tm = (flags & LS_ATIME) ? st->st_atime : (flags & LS_CTIME) ? st->st_ctime : st->st_mtime;
		s = tmform(s, "%?%l", &tm);
		*s++ = ' ';
	}
	if (info)
	{
		while (*s = *info++) s++;
		*s++ = ' ';
	}
	while (*s = *name++) s++;
	if (flags & LS_MARK) switch (st->st_mode & S_IFMT)
	{
	case S_IFDIR:
		*s++ = '/';
		break;
	case S_IFLNK:
		*s++ = '@';
		break;
	case S_IFREG:
		if (st->st_mode & (S_IXUSR|S_IXGRP|S_IXOTH)) *s++ = '*';
		break;
	}
	if (link)
	{
		(void)sprintf(s, " %s %s", (st->st_mode & S_IFMT) == S_IFLNK ? "->" : "==", link);
		while (*++s);
	}
	*s = 0;
	return(s);
}
