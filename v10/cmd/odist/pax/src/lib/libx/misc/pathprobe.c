/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * return in path the full path name of the probe(1)
 * information for lang and tool using proc
 * if attr != 0 then path attribute assignments placed here
 *
 * if path==0 then the space is malloc'd
 *
 * op:
 *
 *	-1	return path name with no generation
 *	0	verbose probe
 *	1	silent probe
 *
 * 0 returned if the info does not exist and cannot be generated
 */

#include <limits.h>
#include <ls.h>

#ifndef PROBE
#define PROBE		"probe"
#endif

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

extern int	access();
extern int	cmdrun();
extern char*	malloc();
extern char*	pathpath();
extern char*	pathkey();
extern int	stat();
extern char*	strchr();
extern char*	strcopy();
extern char*	strcpy();
extern char*	strncpy();
extern char*	strdup();

char*
pathprobe(path, attr, lang, tool, proc, op)
char*	path;
char*	attr;
char*	lang;
char*	tool;
char*	proc;
int	op;
{
	register char*	p;
	register char*	k;
	register char**	ap;
	int		n;
	char*		e;
	char*		probe;
	char		buf[PATH_MAX];
	char		cmd[PATH_MAX];
	char		lib[PATH_MAX];
	char*		arg[6];
	time_t		ptime;
	struct stat	st;

	if (*proc != '/')
	{
		if (p = strchr(proc, ' '))
		{
			n = p - proc;
			proc = strncpy(buf, proc, n);
			*(proc + n) = 0;
		}
		if (!(proc = pathpath(cmd, proc, (char*)0, 031))) return(0);
		if (p) (void)strcpy(proc + strlen(proc), p);
	}
	if (!path) path = buf;
	probe = PROBE;
	p = strcopy(lib, "lib/");
	p = strcopy(p, probe);
	*p++ = '/';
	p = strcopy(k = p, lang);
	*p++ = '/';
	p = strcopy(p, tool);
	*p++ = '/';
	e = strcopy(p, probe);
	if (!pathpath(path, lib, "", 021) || stat(path, &st)) return(0);
	ptime = st.st_mtime;
	(void)pathkey(p, attr, lang, proc);
	p = path + strlen(path) - (e - k);
	(void)strcpy(p, k);
	if (op >= 0 && !stat(path, &st))
	{
		if (ptime <= st.st_mtime || ptime <= st.st_ctime) op = -1;
		else if (st.st_mode & S_IWUSR)
		{
			if (op == 0) error(0, "%s probe information for %s language processor %s must be manually regenerated", tool, lang, proc);
			op = -1;
		}
	}
	if (op >= 0)
	{
		(void)strcpy(p, probe);
		ap = arg;
		*ap++ = path;
		if (op > 0) *ap++ = "-s";
		*ap++ = lang;
		*ap++ = tool;
		*ap++ = proc;
		*ap = 0;
		if (cmdrun(path, arg)) return(0);
		(void)strcpy(p, k);
		if (access(path, 4)) return(0);
	}
	return(path == buf ? strdup(path) : path);
}
