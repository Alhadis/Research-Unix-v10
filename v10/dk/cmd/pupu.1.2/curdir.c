static char Sccsid[]="@(#)curdir.c	3.4";

# define	IROOT	2
# include	"sys/types.h"
# include	"macros.h"

#include <stdio.h>
#include "sys/param.h"
#include <mtab.h>
#include "sys/dir.h"
#include "sys/filsys.h"
/*
	current directory.
	Places the full pathname of the current directory in `str'.
	Handles file systems not mounted on a root directory
	via /etc/mtab (see mtab(V)).
	NOTE: PWB systems don't use mtab(V), but they don't mount
	file systems anywhere but on a root directory (so far, at least).

	returns 0 on success
	< 0 on failure.

	Current directory on return:
		success: same as on entry
		failure: UNKNOWN!
*/
static char *curdirp;

static char	*flg[] = {
	"read/write",
	"read only"
	};

static struct mtab mtab[NMOUNT], *mp;
static struct filsys super;


curdir(str)
char *str;
{
	register int n;

	curdirp = str;
	n = findir(0);
	return(n+chdir(str));
}


# define ADDSLASH	if (flag) *curdirp++ = '/';
# define QUIT		{ close(fd); return(-1); }

findir(flag)
{
	register int fd,inum;
	register char *tp;
	char *slashp,tmp[100];
	int dev, r;
	struct direct entry;
	struct stat s;

	if (stat(".",&s)<0) return(-1);
	if ((inum = s.st_ino) == IROOT) {
		dev = s.st_dev;
		if ((fd = open("/",0))<0) return(-1);
		if (fstat(fd,&s)<0)
			QUIT;
		if (dev == s.st_dev) {
			*curdirp++ = '/';
			*curdirp = 0;
			close(fd);
			return(0);
		}
		slashp = entry.d_name;
		slashp--;
		while (read(fd,&entry,sizeof(entry)) == sizeof(entry)) {
			if (entry.d_ino == 0) continue;
			*slashp = '/';
			if (stat(slashp,&s)<0) continue;
			if (s.st_dev != dev) continue;
			if ((s.st_mode&S_IFMT) != S_IFDIR) continue;
			for (tp = slashp; *curdirp = (*tp++); curdirp++);
			ADDSLASH;
			*curdirp = 0;
			close(fd);
			return(0);
		}
		close(fd);

		fd = open("/etc/mtab",0);
		if(fd < 0) {
			fprintf(stderr, "curdir: cannot open /etc/mtab!\n");
			return(-1);
		}
		read(fd, mtab, sizeof mtab);
		for(mp = mtab; mp < &mtab[NMOUNT]; mp++) {
			if(mp->m_dname[0]) {
				sprintf(tmp,"/dev/%s",mp->m_dname);
				if ( stat(tmp,&s) < 0 )
					QUIT;
				if ( s.st_rdev != dev ) continue;
				for ( tp = mp->m_path; *curdirp = (*tp++); curdirp++ );
				ADDSLASH;
				close(fd);
	/*  test print
				printf("%.10s on /dev/%s %s on %s",
				mp->m_path, mp->m_dname);
	end test print */
				return(0);
			}
		}
	
		QUIT;
	}
	if ((fd = open("..",0))<0) return(-1);
	for (entry.d_ino = 0; entry.d_ino != inum; )
		if (read(fd,&entry,sizeof(entry)) != sizeof(entry))
			QUIT;
	close(fd);
	if (chdir("..")<0) return(-1);
	if (findir(-1)<0) r = -1;
	else r = 0;
	for (tp = entry.d_name; *curdirp = (*tp++); curdirp++);
	ADDSLASH;
	*curdirp = 0;
	return(r);
}
