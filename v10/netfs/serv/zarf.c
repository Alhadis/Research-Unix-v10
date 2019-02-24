/*
 * simple unix filesystem interface;
 * access local files through system calls
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <rf.h>
#include "zarf.h"

extern int errno;

static Rfile *rootf;
static int servuid, servgid;

char rootname[200];	/* arbitrarily large */
extern Namemap *exulist, *exglist;

#define	rfmode(m)	(m)
#define	fsmode(m)	(m)
#define	FSPERM	07777	/* just the permission bits */

static int clientdev();
static int maxopen();
static int samefile();

char *malloc();
long lseek();

/*
 * init:
 * return the root
 */

Rfile *
fsinit(argc, argv)
int argc;
char **argv;
{
	register Rfile *f;
	char *passwd = "/etc/passwd";
	char *group = "/etc/group";
	FILE *fp;
	int foundex;

	foundex = 0;
	while (--argc > 0) {
		if (**++argv != '-')
			continue;	/* skip unknown args */
		switch (argv[0][1]) {
		case 'd':
			if (--argc <= 0) {
				rflog("-d: missing arg\n");
				continue;
			}
			rfdebug = atoi(*++argv);
			break;

		case 'p':
			if (--argc <= 0) {
				rflog("-p: missing arg\n");
				continue;
			}
			passwd = *++argv;
			break;

		case 'g':
			if (--argc <= 0) {
				rflog("-g: missing arg\n");
				continue;
			}
			group = *++argv;
			break;

		case 'r':
			if (--argc <= 0) {
				rflog("-r: missing arg\n");
				continue;
			}
			strcpy(rootname, *++argv);
			break;
		case 'e':
			if (--argc <= 0) {
				rflog("-e: missing arg\n");
				continue;
			}
			if (foundex == 0 && (fp = fopen(*++argv, "r")) != NULL) {
				foundex = rdexcept(fp);
				fclose(fp);
			}
			break;

		default:
			rflog("unknown flag %s\n", *argv);
			break;
		}
	}
	rfuidmap = rfmkidmap(passwd, exulist);
	rfgidmap = rfmkidmap(group, exglist);
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL)
		return (NULL);
	if ((f->fs = malloc(sizeof(Fsfile))) == NULL) {
		free((char *)f);
		return (NULL);
	}
	if ((fsp(f)->name = malloc(strlen(rootname)+1)) == NULL) {
		free(f->fs);
		free((char *)f);
		return (NULL);
	}
	if (rootname[0] == 0)
		strcpy(rootname, "/");
	strcpy(fsp(f)->name, rootname);
	fsp(f)->fd = -1;
	fsp(f)->flags = WONTREAD|WONTWRITE|NOSTAT;
	fsstat(f);
	maxopen(f, 0);
	rootf = f;
	umask(0);
	servuid = geteuid();
	servgid = getegid();
	setgid(servgid);	/* to avoid inconsistencies */
	setuid(servuid);
	return (f);
}

Rfile *
fswalk(df, name)
Rfile *df;
char *name;
{
	register Rfile *f;
	char *nname;

	if (rfdebug)
		rflog("walk %d,%ld '%s' '%s'\n", df->dev, df->ino, fsp(df)->name, name);
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL) {
		rflog("fswalk: no mem\n");
		fserrno = RFEINVAL;
		return (NULL);
	}
	if ((f->fs = malloc(sizeof(Fsfile))) == NULL) {
		free((char *)f);
		rflog("fswalk: no mem\n");
		fserrno = RFEINVAL;
		return (NULL);
	}
	if ((nname = malloc(strlen(fsp(df)->name) + strlen(name) + 2)) == NULL) {
		free(f->fs);
		free((char *)f);
		rflog("fswalk: no mem\n");
		fserrno = RFEINVAL;
		return (NULL);
	}
	fsp(f)->name = nname;
	strcpy(nname, fsp(df)->name);
	strcat(nname, "/");
	strcat(nname, name);
	fsp(f)->fd = -1;
	fsp(f)->flags = WONTREAD|WONTWRITE|NOSTAT;
	if (fsstat(f) < 0)
		goto bad;
	/*
	 * hack to avoid hanging in open:
	 * don't open yet unless regular file (not device)
	 * and nonzero link count (not Research mounted stream)
	 * better not to defer open for ordinary files,
	 * in case someone unlinks the name we know
	 */
	if ((fsmode(f->mode) & S_IFMT) == S_IFREG && f->nlink != 0)
		maxopen(f, 0);
	if (df == rootf) {		/* magic for . and .., mostly for pwd */
#if NOTDEF
		if (name[0] == 0 || strcmp(name, ".") == 0) {
			close(fsp(f)->fd);
			free(nname);
			free(f->fs);
			free((char *)f);
			return (df);
		}
#endif
		if (strcmp(name, "..") == 0) {
			close(fsp(f)->fd);
			fserrno = 0;		/* pseudo-error: popped out of root */
			goto bad;
		}
	}
	return (f);

bad:
	free(nname);
	free(f->fs);
	free((char *)f);
	return (NULL);
}

Rfile *
fscreate(df, name, mode, uid, gid)
Rfile *df;
char *name;
int mode;
int uid, gid;
{
	register Rfile *f;
	char *nname;

	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL) {
		rflog("fscreate: no mem\n");
		fserrno = RFEINVAL;
		return (NULL);
	}
	if ((f->fs = malloc(sizeof(Fsfile))) == NULL) {
		free((char *)f);
		rflog("fscreate: no mem\n");
		fserrno = RFEINVAL;
		return (NULL);
	}
	if ((nname = malloc(strlen(fsp(df)->name) + strlen(name) + 2)) == NULL) {
		free(f->fs);
		free((char *)f);
		rflog("fscreate: no mem\n");
		fserrno = RFEINVAL;
		return (NULL);
	}
	fsp(f)->name = nname;
	strcpy(nname, fsp(df)->name);
	strcat(nname, "/");
	strcat(nname, name);
	if ((fsp(f)->fd = creat(nname, fsmode(mode))) < 0) {
		fserrno = errno;
		goto bad;
	}
	fsp(f)->flags = WONTREAD|NOSTAT;
	if (fsstat(f) < 0) {
		rflog("fscreate: can't stat %s, err %d\n", nname, errno);
		fserrno = RFEINVAL;
		close(fsp(f)->fd);
		goto bad;
	}
	if (uid != f->uid || gid != f->gid) {
		if (chown(nname, uid, gid) >= 0) {
			f->uid = uid;		/* stat would be too slow */
			f->gid = gid;
		} else if (servuid == 0) {	/* if not root, ignore error */
			rflog("fscreate: can't set owner %s err %d\n", nname, errno);
			fserrno = errno;
			close(fsp(f)->fd);
			goto bad;
		}
	}
	return (f);

bad:
	free(nname);
	free(f->fs);
	free((char *)f);
	return (NULL);
}

int
fsmkdir(df, name, mode, uid, gid)
register Rfile *df;
char *name;
int mode;
int uid, gid;
{
	char *nname;
	struct stat st;

	if ((nname = malloc(strlen(fsp(df)->name) + strlen(name) + 2)) == NULL) {
		rflog("fsmkdir: no mem\n");
		fserrno = RFEINVAL;
		return (-1);
	}
	strcpy(nname, fsp(df)->name);
	strcat(nname, "/");
	strcat(nname, name);
	if (mkdir(nname, fsmode(mode)) < 0) {
		fserrno = errno;
		free(nname);
		return (-1);
	}
	if (stat(nname, &st) < 0
	||  (uid != st.st_uid || gid != st.st_gid) && chown(nname, uid, gid) < 0) {
		rflog("fsmkdir: can't set owner %s err %d\n", nname, errno);
		fserrno = errno;
		rmdir(nname);	/* maybe this will work */
		free(nname);
		return (-1);
	}
	free(nname);
	return (0);
}

int
fsrmdir(df, name)
register Rfile *df;
char *name;
{
	char *nname;

	if ((nname = malloc(strlen(fsp(df)->name) + strlen(name) + 2)) == NULL) {
		rflog("fsrmdir: no mem\n");
		fserrno = RFEINVAL;
		return (-1);
	}
	strcpy(nname, fsp(df)->name);
	strcat(nname, "/");
	strcat(nname, name);
	if (rmdir(nname) < 0) {
		fserrno = errno;
		free(nname);
		return (-1);
	}
	free(nname);
	return (0);
}

int
fsdelete(df, name)
register Rfile *df;
char *name;
{
	char *nname;

	if ((nname = malloc(strlen(fsp(df)->name) + strlen(name) + 2)) == NULL) {
		rflog("fsdelete: no mem\n");
		fserrno = RFEINVAL;
		return (-1);
	}
	strcpy(nname, fsp(df)->name);
	strcat(nname, "/");
	strcat(nname, name);
	if (unlink(nname) < 0) {
		fserrno = errno;
		free(nname);
		return (-1);
	}
	free(nname);
	return (0);
}

int
fslink(df, name, f)
register Rfile *df, *f;
char *name;
{
	char *nname;

	if ((nname = malloc(strlen(fsp(df)->name) + strlen(name) + 2)) == NULL) {
		rflog("fslink: no mem\n");
		fserrno = RFEINVAL;
		return (-1);
	}
	if (samefile(f) == 0) {
		rflog("fslink: wrong file %s\n", fsp(f)->name);
		fserrno = RFEINVAL;
		return (-1);
	}
	strcpy(nname, fsp(df)->name);
	strcat(nname, "/");
	strcat(nname, name);
	if (link(fsp(f)->name, nname) < 0) {
		fserrno = errno;
		free(nname);
		return (-1);
	}
	free(nname);
	return (0);
}

int
fsdone(f)
register Rfile *f;
{

	if (fsp(f)->flags & DIDDIR)
		dodircleanup(f);
	close(fsp(f)->fd);
	free(fsp(f)->name);
	free(f->fs);
	free((char *)f);
	return (0);
}

/*
 * this should really use fchmod and fchown, but
 * (1) some systems don't have them;
 * (2) there must be code to call chmod anyway,
 * else how can we chmod a file in mode 0 if not super-user?
 * (3) there's no futime anyway
 *
 * the test in front is to skip samefile (and perhaps a spurious
 * error message) if nothing is really changing;
 * usually this just means ta or tm is being updated to the
 * value the local system has already stored after read or write
 */

int
fsupdate(f, nf)
register Rfile *f, *nf;
{
	long ft[2];
	int nfd;
	int status;

	if ((fsmode(nf->mode)&FSPERM) == (fsmode(f->mode)&FSPERM)
	&&  nf->uid == f->uid && nf->gid == f->gid
	&&  nf->ta == f->ta && nf->tm == f->tm
	&&  nf->size == f->size)
		return (0);
	if (samefile(f) == 0) {
		rflog("fsupdate: wrong file %s\n", fsp(f)->name);
		fserrno = RFEINVAL;
		return (-1);
	}
	status = -1;
	if ((fsmode(nf->mode)&FSPERM) != (fsmode(f->mode)&FSPERM)) {
		if (chmod(fsp(f)->name, fsmode(nf->mode) & FSPERM) < 0) {
			fserrno = errno;
			goto out;
		}
	}
	if (nf->ta || nf->tm) {
		ft[0] = nf->ta;
		ft[1] = nf->tm;
		if (utime(fsp(f)->name, ft) < 0) {
			fserrno = errno;
			goto out;
		}
	}
	if (f->uid != nf->uid || f->gid != nf->gid) {
		if (chown(fsp(f)->name, nf->uid, nf->gid) < 0) {
			fserrno = errno;
			goto out;
		}
	}
	if (f->size != nf->size && nf->size == 0) {
		if ((nfd = creat(fsp(f)->name, 0)) < 0) {
			fserrno = errno;
			goto out;
		}
		close(nfd);
	}
	status = 0;
out:
	fsstat(f);
	return (status);
}

fsstat(f)
register Rfile *f;
{
	struct stat st;

	if (fsp(f)->fd >= 0 && fstat(fsp(f)->fd, &st) < 0) {
		fserrno = errno;
		return (-1);
	}
	else if (stat(fsp(f)->name, &st) < 0) {
		fserrno = errno;
		return (-1);
	}
	if (fsp(f)->flags & NOSTAT)
		fsp(f)->flags &=~ NOSTAT;
	else if (clientdev(st.st_dev) != f->dev || st.st_ino != f->ino) {
		rflog("fsstat: wrong file %s\n", fsp(f)->name);
		fserrno = RFEINVAL;
		return (-1);
	}
	f->ino = st.st_ino;
	f->dev = clientdev(st.st_dev);
	f->mode = rfmode(st.st_mode);
	switch(st.st_mode & S_IFMT) {
	case S_IFDIR:
		f->type = RFTDIR;
		break;

	case S_IFCHR:
	case S_IFBLK:
		f->mode &=~ 07777;	/* deny access to devices */
		/* fall through */
	case S_IFREG:
		f->type = RFTREG;
		break;
	}
	f->nlink = st.st_nlink;
	f->uid = st.st_uid;
	f->gid = st.st_gid;
	f->size = st.st_size;
	f->ta = st.st_atime;
	f->tm = st.st_mtime;
	f->tc = st.st_ctime;
	return (0);
}

int
fsread(f, off, buf, len)
register Rfile *f;
long off;
char *buf;
int len;
{
	register int n;

	if (fsp(f)->flags & WONTREAD)
		maxopen(f, 0);
	if (fsp(f)->flags & CANTREAD) {
		fserrno = RFEACCES;	/* probably */
		return (-1);
	}
	lseek(fsp(f)->fd, off, 0);
	if ((n = read(fsp(f)->fd, buf, len)) < 0)
		fserrno = errno;
	return (n);
}

int
fsdirread(f, off, buf, len, offp)
register Rfile *f;
long off;
char *buf;
int len;
long *offp;
{

	if (fsp(f)->flags & WONTREAD)
		maxopen(f, 0);
	if (fsp(f)->flags & CANTREAD) {
		fserrno = RFEACCES;	/* probably */
		return (-1);
	}
	fsp(f)->flags |= DIDDIR;
	return (dodirread(f, buf, len, off, offp));
}

int
fswrite(f, off, buf, len)
register Rfile *f;
long off;
char *buf;
int len;
{
	register int n;

	if (fsp(f)->flags & WONTWRITE)
		maxopen(f, 1);
	if (fsp(f)->flags & CANTWRITE) {
		fserrno = RFEACCES;
		return (-1);
	}
	lseek(fsp(f)->fd, off, 0);
	if ((n = write(fsp(f)->fd, buf, len)) < 0)
		fserrno = errno;
	else if (off + len > f->size)
		f->size = off + len;	/* should stat, but too slow */
	return (n);
}

/*
 * turn a client st_dev into an rf minor device
 * exactly eight bits allowed
 */

#define	MAXDEVS	255

int maxdev = 0;
dev_t devtab[MAXDEVS];

static int
clientdev(d)
register dev_t d;
{
	register int i;

	for (i = 0; i < maxdev; i++)
		if (devtab[i] == d)
			return (i);
	if (i >= MAXDEVS) {
		rflog("too many devs, can't map %x\n", d);
		return (MAXDEVS);
	}
	devtab[i] = d;
	maxdev = i + 1;
	return (i);
}

/*
 * open a file as broadly as possible
 * to avoid `text busy,' don't open for write
 * unless specifically requested
 * or unless file has no execute permissions (hack)
 * directories usually have execute permissions (further hack)
 */

#define	ANYEXEC	((RFPEX<<RFPOWNER)|(RFPEX<<RFPGROUP)|(RFPEX<<RFPOTHER))

static int
maxopen(f, mustwrite)
register Rfile *f;
int mustwrite;
{
	register Fsfile *fs;
	int fd;
	struct stat st;
	register int flags;

	fd = -1;
	fs = fsp(f);
	flags = fs->flags;
	if ((flags & (WONTREAD|WONTWRITE)) == 0)
		return;
	if ((flags & (CANTREAD|CANTWRITE)) == 0
	&&  mustwrite || (f->mode & ANYEXEC) == 0) {
		if ((fd = open(fs->name, 2)) >= 0)
			flags &=~ (WONTREAD|WONTWRITE);
	}
	if ((flags & (CANTREAD|WONTREAD)) == WONTREAD) {
		flags &=~ WONTREAD;
		if ((fd = open(fs->name, 0)) < 0)
			flags |= CANTREAD;
	}
	if (mustwrite && (flags & (CANTWRITE|WONTWRITE)) == WONTWRITE) {
		flags &=~ WONTWRITE;
		if ((fd = open(fs->name, 1)) < 0)
			flags |= CANTWRITE;
	}
	if (fd >= 0) {	/* if we opened something */
		if ((flags & NOSTAT) == 0) {
			if (fstat(fd, &st) < 0) {
				rflog("maxopen: fstat %s err %d\n", fs->name, errno);
				close(fd);
				return;		/* flags and fd unchanged */
			} else if (clientdev(st.st_dev) != f->dev || st.st_ino != f->ino) {
				rflog("maxopen: wrong file %s\n", fs->name);
				close(fd);
				return;		/* flags and fd unchanged */
			}
		}
		/* it's the same file, or we can't tell */
		fs->flags = flags;
		if (fs->fd >= 0)
			close(fs->fd);
		fs->fd = fd;
	}
}

/*
 * is this the same file we opened?
 * call before operations that must use name rather than fd
 */

static int
samefile(f)
register Rfile *f;
{
	struct stat st;

	if (fsp(f)->flags & NOSTAT)
		return (1);	/* can't tell */
	if (stat(fsp(f)->name, &st) < 0)
		return (0);
	if (clientdev(st.st_dev) != f->dev || st.st_ino != f->ino)
		return (0);
	return (1);
}
