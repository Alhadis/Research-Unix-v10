/*
 *	reccp [-z] source dest
 *
 *	     or
 *
 *	reccp [-z] source1 ... sourcen dest
 *
 *	In the first case, dest must not be a directory (else we have a
 *	degenerate version of the second case). In the second case,
 *	dest must be a directory; the resulting objects will be
 *	named dest/source1 ... dest/sourcen.
 *
 *	If any source is a directory, the tree structure under it
 *	will be (recursively) copied; this process will attempt to
 *	preserve any links that exist within the hierarchy.
 *
 *	This version of reccp copies symbolic links as symbolic links.
 *
 *	-z ==> copy blocks of zeros as holes.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define FTW_more_to_come 1
#include <ftw.h>
	/* rest of struct FTW: */
	int obase; /* added to give basename offset of output file */
	};
/* #include "ftw.body" */
/*
 * Struct FTW (whose definition starts at the end of ftw.h) must
 * must include at least the integers quit, base, and level.
 */

#define FTW_PATHLEN0 1000
#define FTW_PATHINC 1000
#ifndef S_IFLNK
#define lstat stat
#endif
#ifdef S_IFSOCK
#include <sys/dir.h>
#else
#include "ndir.h"
#endif
#ifndef ENOMEM
#include <errno.h>
#endif

	extern int errno;

/*
 *  Each generation of ftw1 (the real ftw) allocates one copy, R, of the
 *  following structure; it passes a pointer to this structure when it
 *  recursively invokes itself.  These structures are chained together,
 *  so that if it becomes necessary to recycle file descriptors, then
 *  the oldest descriptor (the one at the shallowest depth still open)
 *  can be recycled.
 */

	struct FTW_rec {
		struct FTW_rec *prev;
		long here;	/* seek to here when reopening at this level */
		DIR *fd;	/* file descriptor at this level */
		};

/*
 *  One instance, T, of the following structure is allocated by ftw; a
 *  pointer to it is passed to all generations of ftw1 (the real ftw).
 *  T could often be a global variable, but this way the parameter fn
 *  can invoke ftw for an independent tree walk.
 *  Component T->path points to storage for the object path-names;
 *  this storage may be relocated by realloc if T->path needs to be
 *  more than T->pathlast characters long.
 *  T->path[T->pathnext] is the next free character in the pathnames.
 *  T->depth = parameter depth to ftw.  T->lastout is the deepest level at
 *  which a file descriptor has been recycled.
 */

	struct FTW_top {
		int (*fn)();
		char *path;
		unsigned pathlast, pathnext;
		int lastout;
		int depth;
		};

int
ftw (path, fn, depth)
	char *path;
	int (*fn)();
	int depth;
{
	struct FTW_top T;
	struct FTW_rec R;
	struct FTW S;
	int rc;
	char *malloc(), *strcpy();

	T.depth = depth;
	T.lastout = -1;
	T.fn = fn;
	S.quit = 0;
	S.level = -1;

	/* initialize S.base, T.pathnext... */
		{
		register char c, *p, *q;
		for (p = q = path; c = *p; p++) if (c == '/') q = p + 1;
		S.base = q - path;
		T.pathnext = p - path;
		}

	T.pathlast = T.pathnext + FTW_PATHLEN0;
	T.path = malloc(T.pathlast);
	if (!T.path) { errno = ENOMEM; return -1; }
	strcpy(T.path, path);
	rc = ftw_1_(&R, &T, 0, &S);
	free(T.path);
	return rc;
}

int
ftw_1_ (R, T, level, S1)
	register struct FTW_rec *R;
	register struct FTW_top *T;
	int level;
	struct FTW *S1;
{
	int rc, n;
	DIR *fd;
	struct direct *dirp;
	char *component, *path;
	struct stat sb;
	struct FTW_rec mr;
	unsigned nextsave;
	struct FTW S;
	char *realloc();
	long lseek();

	mr.prev = R;
	path = T->path;
	S.level = level;
	S.quit = 0;
	S.base = S1->base;

	/* Try to get file status.  If unsuccessful, errno will say why. */
	if (lstat(path, &sb) < 0) {
		rc = (*T->fn) (path, &sb, FTW_NS, &S);
		S1->quit = S.quit;
		return rc;
		};

	/*
	 *	The stat succeeded, so we know the object exists.
	 *	If not a directory, call the user function and return.
	 */
#ifdef S_IFLNK
	if ((sb.st_mode & S_IFMT) == S_IFLNK) {
		rc = (*T->fn) (path, &sb, FTW_SL, &S);
		S1->quit = S.quit;
		if (rc || S.quit == FTW_SKR) return rc;
		if (S.quit != FTW_FOLLOW) return 0;
		S1->quit = S.quit = 0;
		if (stat(path, &sb) < 0) {
			rc = (*T->fn) (path, &sb, FTW_NSL, &S);
			S1->quit = S.quit;
			return rc;
			};
		}
#endif
		
	if ((sb.st_mode & S_IFMT) != S_IFDIR) {
		rc = (*T->fn) (path, &sb, FTW_F, &S);
		S1->quit = S.quit;
		return rc;
		}

	/*
	 *	The object was a directory.
	 *
	 *	Open a file to read the directory
	 */
	mr.fd = fd = opendir(path);

	/*
	 *	Call the user function, telling it whether
	 *	the directory can be read.  If it can't be read
	 *	call the user function or indicate an error,
	 *	depending on the reason it couldn't be read.
	 */
	if (!fd) {
		rc = (*T->fn) (path, &sb, FTW_DNR, &S);
		S1->quit = S.quit;
		return rc;
		}

	/* We could read the directory.  Call user function. */
	rc = (*T->fn) (path, &sb, FTW_D, &S);
	if (rc != 0)
		goto rtrn;
	if (S.quit == FTW_SKD) goto rtrn;
	if (S.quit == FTW_SKR) {S1->quit = FTW_SKR; goto rtrn;}

	/* Make sure path is big enough to hold generated pathnames. */

	n = nextsave = T->pathnext;
	if (n + MAXNAMLEN + 1 >= T->pathlast) {
		T->pathlast += FTW_PATHINC;
		path = T->path = realloc(T->path, T->pathlast);
		if (!path) {
			errno = ENOMEM;
			rc = -1;
			goto rtrn;
			}
		}
	
	/* Create a prefix to which we will append component names */

	if (n > 0 && path[n-1] != '/') path[n++] = '/';
	component = path + n;

	/*
	 *	Read the directory one component at a time.
	 *	We must ignore "." and "..", but other than that,
	 *	just create a path name and call self to check it out.
	 */
	while (dirp = readdir(fd)) {
		if (dirp->d_ino != 0
		    && strcmp (dirp->d_name, ".") != 0
		    && strcmp (dirp->d_name, "..") != 0) {
			int i;
			struct FTW_rec *pr;

			/* Append the component name to the working path */
			strcpy(component, dirp->d_name);
			T->pathnext = n + strlen(dirp->d_name);

			/*
			 *	If we are about to exceed our depth,
			 *	remember where we are and close the file.
			 */
			if (level - T->lastout >= T->depth) {
				pr = &mr;
				i = T->lastout++;
				while (++i < level) pr = pr->prev;
				pr->here = telldir(pr->fd);
				closedir(pr->fd);
			}

			/*
			 *	Do a recursive call to process the file.
			 */
			S.quit = 0;
			S.base = n;
			rc = ftw_1_(&mr, T, level+1, &S);
			if (rc != 0 || S.quit == FTW_SKR) {
				if (level > T->lastout) closedir(fd);
				T->pathnext = nextsave;
				return rc;
			}

			/*
			 *	If we closed the file, try to reopen it.
			 */
			if (level <= T->lastout) {
				char c = path[nextsave];
				path[nextsave] = 0;
				T->lastout = level - 1;
				mr.fd = fd = opendir(path);
				if (!fd) {
					rc = (*T->fn) (path, &sb, FTW_DNR, &S);
					S1->quit = S.quit;
					T->pathnext = nextsave;
					return rc;
					}
				path[nextsave] = c;
				seekdir(fd, mr.here);
				}
			}
		}
	T->pathnext = nextsave;
	path[nextsave] = 0;

	/*
	 *	We got out of the subdirectory loop.  Call the user
	 *	function again at the end and clean up.
	 */

	rc = (*T->fn) (path, &sb, FTW_DP, &S);
	S1->quit = S.quit;
rtrn:
	closedir(fd);
	return rc;
}

#define BLKSIZE BUFSIZ
#define	BSIZE(n)	BUFSIZ
char zero[BLKSIZE];	/* keep as zero */
int zflag;

/* destination path-name buffer initial value, increment */
#define PATHLEN0 1000
#define PATHINC 1000
#define IHTABLEN 287
#define SBUFLEN 1000

/* Non-zero if stat buffer b refers to a directory, zero otherwise */
#define isdir(b) (((b).st_mode & S_IFMT) == S_IFDIR)

char *malloc(), *realloc(), *strcpy(), *strcat();
int copy();

char symbuf[SBUFLEN];	/* Buffer for symbolic link names */
int firstyell = 1;
int destlen;		/* length of destination (last arg) */
int iamsu;
char *dest;
char *destdir;		/* Storage for output pathname -- handled like
			 * the source pathname in ftw, subject to being
			 * relocated by realloc.
			 */
unsigned dlast, dnext;	/* Offsets of next available, last+1 chars of
			 * output pathname.
			 */

/* Final component of the path name by which we were invoked */
char *pgmname;

struct inodetab {	/* for keeping track of links */
	struct inodetab *nextint;
	ino_t i_ino;
	dev_t i_dev;
	char iname[1];
	};

struct inodetab *ihtab[IHTABLEN]; /* hash table for remembering links */

int retcode;

main(argc, argv)
int argc;
char **argv;
{
	char *p, *q;
	struct stat *argbufs, destbuf, statbuf, rootbuf;
	int i;

	umask(0);
	pgmname = argv[0];

	if (argc > 1 && !strcmp(argv[1],"-z")) {
		zflag = 1;
		--argc;
		++argv;
		}

	/*
	 *	Start validity checking
	 */

	/* Check for too few arguments given */
	if (argc < 3) {
		fprintf(stderr,
			"usage:  %s [-z] f1 f2  or  %s [-z] f1 ... fn d\n",
				pgmname, pgmname);
		return 1;
	}

	/*
	 *	If the last argument is a non-directory, we require
	 *	argc == 3 (in the case of: cp f1 f2)
	 */
	dest = argv[argc-1];
	destbuf.st_mode = 0;	/* Force isdir(destbuf) false */
	if ((stat (dest, &destbuf) < 0 || !isdir(destbuf)) && argc != 3) {
		scream ("%s not a directory", dest);
		return 1;
	}

	/*
	 *	Allocate storage for a stat buffer for each argument
	 *	but the last. This will be used to check for trying to
	 *	copy something into a subdirectory of itself.
	 */
	argbufs = (struct stat *)
		malloc ((unsigned) (sizeof (struct stat) * (argc-2)));
	if (argbufs == NULL) {
		scream ("insufficient storage", "");
		return 1;
	}
	iamsu = (getuid() == 0);

	for (i = 0; i < argc-2; i++) {
		if (lstat (argv[i+1], &argbufs[i]) < 0) {
			scream ("cannot access %s", argv[i+1]);
			return 1;
		}
	}
	
	/*
	 *	Now run back from the destination to the root directory,
	 *	checking along the way for any matches with anything in
	 *	the argbufs array. If we find any, we tried a forbidden
	 *	operation. If the destination is not already a directory,
	 *	we must start from its parent. Note that even in this case,
	 *	we can't elide the test, because otherwise we wouldn't get
	 *
	 *		cp . x
	 *
	 *	This code also handles pathological but legal destinations
	 *	properly; in particular note that the null string is a
	 *	valid name for the current directory. Sigh.
	 *
	 *	First figure out the name of the directory to check.
	 */
	dnext = destlen = strlen(dest);
	dlast = destlen + 2;
	if (dlast < PATHLEN0) dlast = PATHLEN0;
	destdir = malloc(dlast);
	if (!destdir) { scream ("no storage", ""); return 1; }
	strcpy (destdir, dest);
	if (!isdir (destbuf)) {
		p = q = destdir;
		while (*q) {
			if (*q++ == '/')
				p = q - 1;
		}
		if (p == destdir) {
			strcpy (destdir, ".");
			dnext = 1;
			}
		else {
			*p = '\0';
			dnext = p - destdir;
			}
	}

	/* Now stat the root directory for later use */
	if (stat ("/", &rootbuf) < 0) {
		scream ("cannot access root directory", "");
		return 1;
	}

	/* Now run back from destdir to the root, checking against argbufs */
	do {
		if (stat (destdir, &statbuf) < 0) {
			scream ("cannot stat %s", destdir);
			return 1;
		}
		for (i = 0; i < argc-2; i++)
			if (argbufs[i].st_dev == statbuf.st_dev
			 && argbufs[i].st_ino == statbuf.st_ino) {
				fprintf (stderr, "%s: %s contains %s\n",
					pgmname, argv[i+1], dest);
				return 1;
			}
		dcheck(3, "initial arg checking");
		strcpy(destdir+dnext, "/..");
		dnext += 3;
	} while (statbuf.st_dev != rootbuf.st_dev
	      || statbuf.st_ino != rootbuf.st_ino);

	/*
	 *	End of validity checking
	 */

	strcpy(destdir, dest);
	for (i = 0; i < IHTABLEN; i++) ihtab[i] = 0;
	if (isdir(destbuf)) {
		destdir[destlen] = '/';
		dnext = destlen + 1;
		for (i = 1; i < argc-1; i++) {

			char *p, *q;

			p = q = argv[i];
			while (*p)
				if (*p++ == '/')
					q = p;
			dnext = destlen + (p - q) + 1;

			p = destdir + destlen;
			*p = '/';
			strcpy(p+1, q);
			retcode |= ftw(argv[i], copy, 12);
			}
		}

	else {
		dnext = destlen;
		retcode = ftw(argv[1], copy, 12);
		}

	return retcode;
}

int
copy(source, srcbuf, code, S)
	char *source;
	struct stat *srcbuf;
	int code;
	struct FTW *S;
{
	char *s;
	int rc, slen;

	switch(code) {

		case FTW_F:		/* nondirectory */

			dcheck(MAXNAMLEN+1,source);
			if (S->level)
				strcpy(destdir + dnext, source + S->base);
			return copy1(source, srcbuf, destdir);

		case FTW_D:		/* new directory */
	
			dcheck(MAXNAMLEN+1,source);
			S->obase = dnext;
			s = destdir+dnext;
			if (S->level) {
				for (source += S->base; *s = *source;)
					{ s++, source++; }
				}
			dnext = s - destdir;
			if (mkdir(destdir,0700)) {
				scream("can't mkdir %s", destdir);
				return 1;
				}
			*s++ = '/'; *s = 0;
			++dnext;
			return 0;

		case FTW_DNR:		/* unreadable directory */

			scream("can't read %s", source);
			return 1;

		case FTW_NS:		/* unstatable file */

			scream("can't stat %s", source);
			return 1;

		case FTW_DP:		/* end of directory */

			destdir[dnext-1] = 0;
			dnext = S->obase;
			if (chmod(destdir, (int)srcbuf->st_mode))
				scream("can't chmod %s", destdir);
			if(iamsu && chown(destdir, srcbuf->st_uid,
					srcbuf->st_gid))
				scream("can't chown %s", destdir);
			return settime(srcbuf, destdir);

#ifdef S_IFLNK
		case FTW_SL:		/* symbolic link */

			slen = readlink(source, symbuf, SBUFLEN);
			if (slen >= SBUFLEN) {
				scream("Symbolic path to %s too long.",
					source);
				return 1;
				}
			dcheck(MAXNAMLEN+1,source);
			symbuf[slen] = 0;
			if (S->level) strcpy(destdir + dnext,
						source + S->base);
			rc = 0;
			umask(~(int)srcbuf->st_mode);
			if (symlink(symbuf,destdir)) {
				fprintf(stderr,
				    "%s: can't symbolically link %s to %s\n",
			    		pgmname, destdir, symbuf);
				rc = 1;
				}
			umask(0);
			return rc;
#endif

		default:		/* bug */
			fprintf(stderr,"%s: unknown code = %d from ftw\n",
				pgmname, code);
			exit(1);
		}
	}

/*
 *	Copy a file. The source file name is in "source",
 *	and the destination file name is in "dest". "srcbuf" points to
 *	a stat buffer for the source.
 */

int
copy1(source, srcbuf, dest)
	char *source, *dest;
	struct stat *srcbuf;
{
	int bsize, inf, outf, n, type, zsize;
	char buffer[BLKSIZE];
	struct stat destbuf;
	long lseek();

	if (stat (dest, &destbuf) >= 0
	 && srcbuf->st_dev == destbuf.st_dev
	 && srcbuf->st_ino == destbuf.st_ino) {
		scream ("cannot copy %s to itself", source);
		return 1;
		}

	switch(type = srcbuf->st_mode & S_IFMT) {

		case S_IFCHR:	/* character special */
		case S_IFBLK:	/* block special */
			if (!iamsu) {
				scream("can't copy special file %s", source);
				return 1;
				}
			if (n = linkchk(srcbuf,destdir)) return n-1;
			if (mknod(destdir, (int) srcbuf->st_mode,
					   (int) srcbuf->st_rdev)) {
				scream("can't create %s", destdir);
				return 1;
				}
			goto finish;

		case S_IFREG:	/* regular file */
			if (n = linkchk(srcbuf,destdir)) return n-1;
			break;

		default:
			fprintf(stderr,
				"%s: unknown file type 0x%x for %s\n",
				pgmname, type, source);
			return 1;
		}

	/* Now copy the file, whether or not there was a link */

	if ((outf = creat (dest, (int) (srcbuf->st_mode & 0777))) < 0) {
		scream ("cannot create %s", dest);
		return 1;
	}
	if ((inf = open (source, 0)) < 0) {
		scream ("cannot open %s", source);
		close(outf);
		return 1;
	}
	if (zflag) {
		fstat(outf, &destbuf);
		bsize = BSIZE(destbuf.st_dev);
		if (bsize > BLKSIZE) {
			fprintf(stderr, "%s: BSIZE(%s) > %d\n", pgmname,
				dest, BLKSIZE);
			return 1;
			}
		}
	else bsize = BLKSIZE;
	zsize = 0;
	while ((n = read (inf, buffer, bsize)) > 0) {
		if (zflag && (n == bsize) && !memcmp(buffer, zero, bsize)) {
			if (lseek(outf, (long)bsize, 1) < 0) {
				scream("lseek (for block of 0's) failed on %s",
					dest);
				goto endit;
				}
			zsize = bsize;
			}
		else if (write (outf, buffer, n) != n) {
			scream ("output error on %s", dest);
endit:
			close (inf);
			close (outf);
			return 1;
		}
		else zsize = 0;
	}
	if (n < 0) {
		scream ("input error on %s", source);
		goto endit;
	}
	if (zsize && (lseek(outf, (long)-zsize, 1) < 0
		|| write(outf,zero,zsize) != zsize)) {
			scream("error writing trailing 0's from %s", source);
			goto endit;
			}
	if (close (outf) < 0) {
		scream ("cannot close %s", dest);
		close (inf);
		return 1;
	}
	if (close (inf) < 0) {
		scream ("cannot close %s", source);
		return 1;
	}

 finish:
	if(iamsu && chown(dest, srcbuf->st_uid, srcbuf->st_gid)) {
		scream("can't chown %s", dest);
		return 1;
		}
	return settime(srcbuf, dest);
}

scream (s1, s2)
	char *s1, *s2;
{
	fprintf (stderr, "%s: ", pgmname);
	fprintf (stderr, s1, s2);
	putc ('\n', stderr);
	retcode |= 1;
}

dcheck(len, source)
int len;
char *source;
{
	int dnext1 = dnext + len;
	if (dnext1 >= dlast) {
		dlast += PATHINC;
		destdir = realloc(destdir, dlast);
		if (!destdir) {
			scream("malloc failed on %s",source);
			exit(1);
			}
		}
}

int
linkchk(sb,d)
struct stat *sb;
char *d;
{
	register struct inodetab *p, *p0;
	struct stat buf;

	if (sb->st_nlink < 2) return 0;
  
	p0 = (struct inodetab *)
		(((sb->st_dev + sb->st_ino) % IHTABLEN) + ihtab);
	while ((p = p0->nextint) &&
		(p->i_ino != sb->st_ino || p->i_dev != sb->st_dev)) p0 = p;

	if (p) {/* If this is a link to a file already seen... */
		if (link(p->iname, d)) {

			/* unlink d and try again, unless d is a directory */

			if (lstat(d,&buf)
					|| (buf.st_mode & S_IFMT) == S_IFDIR
					|| unlink(d)
					|| link(p->iname,d)) {
				fprintf(stderr,"%s: can't link %s to %s\n",
					pgmname, p->iname, d);
				return 2;
				}
			}
		return 1;
		}

	/* This file has more than one link to it -- add it to the list */
	/* of files that may be seen later. */

	p = (struct inodetab *)
		malloc((unsigned) (sizeof(struct inodetab) + strlen(d)));
	if (!p) {
		if (firstyell) {
			firstyell = 0;
			scream("storage for links exhausted with %s", d);
			}
		return 0;
		}

	p0->nextint = p;
	p->nextint = 0;
	p->i_ino = sb->st_ino;
	p->i_dev = sb->st_dev;
	strcpy(p->iname, d);
	return 0;
	}

settime(srcbuf, dest)		/* set time and mode of dest */
	struct stat *srcbuf;
	char *dest;
	{
	time_t utimbuf[2];
	int m, rc = 0;

	utimbuf[0] = srcbuf->st_atime;
	utimbuf[1] = srcbuf->st_mtime;
	utime (dest, utimbuf);

	m = srcbuf->st_mode;
#ifdef S_ISVTX
	if (m & S_ISVTX && !iamsu) {
		scream("can't set save-text bit of %s", dest);
		m &= ~S_ISVTX;
		}
	if ((m & (S_ISUID | S_ISGID | S_ISVTX)) && (rc = chmod(dest,m)))
#else
	if ((m & (S_ISUID | S_ISGID)) && (rc = chmod(dest,m)))
#endif
		scream("can't set mode of %s", dest);
	return rc;
	}
