/*	@(#)hashserv.c	1.4	*/
/*
 *	UNIX shell
 *
 *	Bell Telephone Laboratories
 *
 */

#include	"defs.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<errno.h>

#define		EXECUTE		01

findpath(name)
	register char *name;
{
	register char 	*path;
	register int	count = 1;

	char	*p;
	int	ok = 1;
	int 	e_code = 1;
	
	path = getpath(name);

	while (path)
	{
		path = catpath(path, name);
		p = curstak();

		if ((ok = chk_access(p)) == 0)
			break;
		else
			e_code = max(e_code, ok);

		count++;
	}

	return(ok ? -e_code : count);
}


chk_access(name)
	register char	*name;
{
	struct stat buf;

	if (access(name, EXECUTE)==0 && stat(name, &buf)==0 && (buf.st_mode&S_IFMT)==S_IFREG)
		return(0);

	return(errno == EACCES ? 3 : 1);
}


pr_path(name, count)
	register char	*name;
	int count;
{
	register char	*path;

	path = getpath(name);

	while (--count && path)
		path = nextpath(path, name);

	catpath(path, name);
	prs_buff(curstak());
}

what_is(name)
	register char *name;
{
	struct namnod *n;
	int		cnt;
	short	hashval;
	int	isparam = 0;

	if ((n = findnam(name)) && !(n->namval.flg & N_FUNCTN))
	{
		printnam(n);
		isparam = n->namval.val!=0;
	}

	if (n && (n->namval.flg & N_FUNCTN))
	{
		prs_buff(strf(n));
		prc_buff(NL);
		return;
	}

	if (syslook(name, commands, no_commands))
	{
		prs_2buff("builtin ", name);
		prc_buff(NL);
		return;
	}
	if ((cnt = findpath(name)) > 0)
	{
		pr_path(name, cnt);
		prc_buff(NL);
	}
	else if (!isparam)
		failed(name, notfound, 0);
}

