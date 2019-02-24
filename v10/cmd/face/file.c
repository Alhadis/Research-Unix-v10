/*
 * routines for creating and looking up internal files
 *
 * the internal file structure is a tree of Files.
 * terminal nodes are regular files;
 * their contents is in a real file outside.
 * other nodes are directories;
 * they exist only in our memory.
 *
 * directories are sorted lists;
 * files are looked up by binary search.
 */

#include "faces.h"
#include "faceproto.h"	/* for FROOT */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dirent.h"	/* non-portable portable routines */

static File *root;
static int nextino;

#define	DIRINIT		10
#define	DIRCHUNK	100

/*
 * built-in directories
 * every outside data file must be in one of these
 */

struct builtin {
	char *ourname;
	char *extname;
	File *file;
	time_t mtime;
} builtins[] = {
	{"48x48x1",	"/usr/jerq/icon/face48"},
	{"48x48x2",	"/usr/jerq/icon/48x48x2"},
	{"512x512x8",	"/t0/face/512x512x8"},
	0
};

static time_t ptime, mtime;

extern char *strchr();
extern File *falloc();

/*
 * remake the face tree
 */

maketree(pfile, mfile)
char *pfile, *mfile;
{
	File *df, *f;
	FILE *fp;
	char *dir, *pers, *ref;
	register struct builtin *bp;

	(void)newdata(pfile, mfile);	/* store times */
	if (root == NULL) {
		nextino = FROOT;
		root = newfile();
	} else {
		nextino = FROOT + 1;
		freetree(root);
	}
	initdir(root, root);
	for (bp = builtins; bp->ourname; bp++) {
		f = newfile();
		initdir(f, root);
		filldir(f, bp->extname);
		enterfile(root, bp->ourname, f);
		bp->file = f;
	}
	if ((fp = fopen(pfile, "r")) == NULL)
		log("%s: cannot open; no people\n", pfile);
	else {
		while (pcrack(fp, pfile, &dir, &pers, &ref)) {
			if ((df = lookent(root, dir)) == NULL) {
				df = newfile();
				initdir(df, root);
				enterfile(root, dir, df);
			}
			f = newfile();	/* make dir/pers */
			if (enterfile(df, pers, f)) {
				log("%s: %s/%s=%s: dup for %s/%s\n",
					pfile, dir, pers, ref, dir, pers);
				ffree(f);
				continue;
			}
			initdir(f, df);
			for (bp = builtins; bp->ourname; bp++)
				if ((df = lookent(bp->file, ref)) != NULL)
					enterfile(f, bp->ourname, df);
		}
		fclose(fp);
	}
	if ((fp = fopen(mfile, "r")) == NULL)
		log("%s: cannot open; no machines\n", mfile);
	else {
		while (pcrack(fp, mfile, &dir, (char **)NULL, &ref)) {
			if ((df = lookent(root, ref)) == NULL) {
				log("%s: %s=%s: %s not found\n",
					mfile, dir, ref, ref);
				continue;
			}
			if ((f = enterfile(root, dir, df)) != NULL) {
				if (f == df) {
					log("%s: %s=%s: already true\n",
						mfile, dir, ref);
					continue;
				}
				uniondir(df, f, mfile, dir, ref);
			}
		}
		fclose(fp);
	}
}

/*
 * parse a line in people file or machine file
 * fairly hackish
 * people file: dir/person=realfile
 * machine file: machdir=dir
 * return 1 if a valid entry found,
 * 0 if end of file
 */

pcrack(fp, fname, dirp, persp, refp)
FILE *fp;
char *fname;
char **dirp, **persp, **refp;
{
	static char buf[100];
	register char *p;

again:
	buf[sizeof(buf)-1] = 0;
	if (fgets(buf, sizeof(buf), fp) == NULL)
		return (0);		/* EOF */
	if (buf[sizeof(buf)-1]) {
		buf[sizeof(buf)-1] = 0;
		log("%s: line too long: %s ...\n", fname, buf);
		return (0);		/* too hard to recover for now */
	}
	if (buf[0] == '#' || buf[0] == '\n')
		goto again;	/* skip comments, empties */
	p = buf;
	*dirp = p;
	if (persp) {
		if ((p = strchr(p, '/')) == NULL) {
			log("%s: missing /: %s", fname, buf);
			goto again;
		}
		*p++ = 0;
		*persp = p;
	}
	if ((p = strchr(p, '=')) == NULL) {
		if (persp)
			(*persp)[-1] = '/';	/* hack */
		log("%s: missing =: %s", fname, buf);
		goto again;
	}
	*p++ = 0;
	*refp = p;
	if ((p = strchr(p, '\n')) != NULL)	/* fgets should promise this */
		*p = 0;
	return (1);
}

/*
 * see if the outside data has changed,
 * and we should rebuild the tree
 */

newdata(pfile, mfile)
char *pfile, *mfile;
{
	struct stat st;
	int new;
	register struct builtin *bp;

	new = 0;
	for (bp = builtins; bp->ourname; bp++) {
		if (stat(bp->extname, &st) < 0)
			continue;
		if (bp->mtime == 0)
			bp->mtime = st.st_mtime;
		if (bp->mtime < st.st_mtime) {
			bp->mtime = st.st_mtime;
			new = 1;
		}
	}
	if (stat(pfile, &st) >= 0) {
		if (ptime == 0)
			ptime = st.st_mtime;
		if (ptime < st.st_mtime) {
			ptime = st.st_mtime;
			new = 1;
		}
	}
	if (stat(mfile, &st) >= 0) {
		if (mtime == 0)
			mtime = st.st_mtime;
		if (mtime < st.st_mtime) {
			mtime = st.st_mtime;
			new = 1;
		}
	}
	return (new);
}

/*
 * free a sub-tree
 */
freetree(f)
register File *f;
{
	register int i;
	register File *nf;

	if (!isdir(f))
		return;
	for (i = 0; i < f->nfiles; i++) {
		nf = f->dir[i].file;
		if (nf == f				/* probably "." */
		||  strcmp(f->dir[i].name, "..") == 0) {
			nf->nlinks--;
			continue;
		}
		if (isdir(nf))
			freetree(nf);
		if (--nf->nlinks > 0)
			continue;
		if (nf->data)
			free(nf->data);
		if (nf->dir)
			free((char *)nf->dir);
		ffree(nf);
	}
	if (f->data)
		free(f->data);
	f->data = NULL;
	f->nfiles = 0;		/* (but still allocated) */
}

/*
 * make a new file
 */

File *
newfile()
{
	register File *f;

	f = falloc();
	f->nlinks = 0;		/* no entries yet */
	f->ino = nextino++;
	f->data = NULL;
	f->dir = NULL;
	return (f);
}

/*
 * init a directory:
 * set up . and ..
 */

initdir(f, parent)
register File *f;
File *parent;
{
	extern long time();

	if (f->dir == NULL) {
		f->dir = (Dirent *)emalloc(DIRINIT*sizeof(Dirent));
		f->nalloc = DIRINIT;
	} else if (f->nalloc < 2)
		panic("init badly formed dir\n");
	if (f->data) {
		free(f->data);
		f->data = NULL;
	}
	f->nfiles = 2;
	strcpy(f->dir[0].name, ".");
	f->dir[0].file = f;
	f->nlinks++;
	strcpy(f->dir[1].name, "..");
	f->dir[1].file = parent;
	parent->nlinks++;
	f->tc = time((long *)0);
	f->ta = f->tc;
	f->tm = f->tc;
}

/*
 * look up an entry in a directory
 */

File *
lookent(f, name)
File *f;
register char *name;
{
	register Dirent *d;
	register int i, cmp, u, l;

	if (!isdir(f))
		return (NULL);
	d = f->dir;
	u = f->nfiles;	/* first entry known to be too big */
	l = 0;		/* first entry known to be big enough */
	while (u > l) {
		i = (u + l)/2;
		if ((cmp = name[0] - d[i].name[0]) == 0
		&&  (cmp = strcmp(name, d[i].name)) == 0)
			return (d[i].file);
		else if (cmp < 0)
			u = i;
		else
			l = i + 1;
	}
	return (NULL);
}

/*
 * enter newf in f with the given name
 * if it is a new entry, return NULL
 * if f already had name, return the old one, leave it in place
 */

File *
enterfile(f, name, newf)
File *f;
register char *name;
File *newf;
{
	register Dirent *d;
	register int i, cmp, u, l;

	if (!isdir(f))
		panic("enter non-dir\n");
	if (f->nalloc <= f->nfiles) {	/* need more room */
		f->nalloc += DIRCHUNK;
		f->dir = (Dirent *)erealloc((char *)f->dir, f->nalloc*sizeof(Dirent));
	}
	d = f->dir;
	u = f->nfiles;	/* first entry known to be too big */
	l = 0;		/* first entry known to be big enough */
	while (u > l) {
		i = (u + l)/2;
		if ((cmp = name[0] - d[i].name[0]) == 0
		&&  (cmp = strcmp(name, d[i].name)) == 0)
			return (d[i].file);	/* unexpected */
		else if (cmp < 0)
			u = i;
		else
			l = i + 1;
	}
	/*
	 * not found; d[u] is too big
	 * insert before d[u]
	 */
	for (i = f->nfiles; i > u; i--)
		d[i] = d[i-1];
	strncpy(d[i].name, name, DIRNSIZE);
	d[i].file = newf;
	f->nfiles++;
	newf->nlinks++;
	if (f->data) {		/* invalid now */
		free(f->data);
		f->data = NULL;
	}
	return (NULL);
}

/*
 * look up a full pathname
 */

File *
lookfile(path)
register char *path;
{
	File *f;
	register char *sl;

	f = root;
	for (;;) {
		while (*path == '/')
			path++;
		if (*path == 0)
			return (f);
		if ((sl = strchr(path, '/')) != NULL)
			*sl = 0;		/* cheap hack */
		f = lookent(f, path);
		if (sl)
			*sl = '/';		/* cheap hack returns */
		if (f == NULL)
			return (NULL);
		if (sl == NULL)
			return (f);	/* no more slashes, so we're done */
		path = sl + 1;
	}
}

/*
 * link each entry from f to uf,
 * fussing about duplicates
 * --the string arguments are just for the log message
 */

uniondir(f, uf, mfile, dir, ref)
File *f, *uf;
char *mfile, *dir, *ref;
{
	register int i;
	register Dirent *dp;
	register File *ef;

	dp = f->dir;
	for (i = 0; i < f->nfiles; dp++, i++) {
		if (dp->name[0] == '.'
		&&  (strcmp(dp->name, ".") == 0 || strcmp(dp->name, "..") == 0))
			continue;
		if ((ef = enterfile(uf, dp->name, dp->file)) != NULL
		&&  ef != dp->file)		/* same file already there */
			log("%s: %s=%s: file %s duplicate\n",
				mfile, dir, ref, dp->name);
	}
}

/*
 * copy directory entries from the outside
 * to one of ours
 * save many calls to enterfile, and many searches
 * for files that don't exist, by building the directory
 * here and sorting it
 */

static int
dircmp(a, b)
char *a, *b;
{
	return (strncmp(((Dirent *)a)->name, ((Dirent *)b)->name, DIRNSIZE));
}

filldir(f, realdir)
register File *f;
char *realdir;
{
	DIR *fp;
	register struct dirent *dp;
	register Dirent *idp;
	File *nf;
	int rdlen;

	if ((fp = opendir(realdir)) == NULL) {
		log("%s: can't opendir\n", realdir);
		return;
	}
	rdlen = strlen(realdir);
	idp = &f->dir[f->nfiles];
	while ((dp = readdir(fp)) != NULL) {
		if (dp->d_name[0] == '.'
		&&  (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0))
			continue;
		nf = newfile();
		if (f->nalloc <= f->nfiles) {
			f->nalloc += DIRCHUNK;
			f->dir = (Dirent *)erealloc((char *)f->dir, f->nalloc*sizeof(Dirent));
			idp = &f->dir[f->nfiles];	/* realloc may move it */
		}
		strncpy(idp->name, dp->d_name, DIRNSIZE);
		idp->file = nf;
		idp++;
		f->nfiles++;
		nf->nlinks++;
		nf->data = emalloc(rdlen+1+strlen(dp->d_name)+1);
		strcpy(nf->data, realdir);
		nf->data[rdlen] = '/';
		strcpy(nf->data+rdlen+1, dp->d_name);
	}
	closedir(fp);
	qsort((char *)f->dir, f->nfiles, sizeof(Dirent), dircmp);
}

/*
 * set up directory data to be sent to client
 */
dirdata(f)
register File *f;
{
	register int i;
	register unsigned char *p;

	if (!isdir(f))
		panic("dirdata not dir\n");
	f->size = f->nfiles * FDLEN;
	f->data = emalloc((int)f->size);
	p = (unsigned char *)f->data;
	for (i = 0; i < f->nfiles; i++) {
		tofshort(p, FD_INO, f->dir[i].file->ino);
		strncpy(p + FD_NAME, f->dir[i].name, FDLEN - FD_NAME);
		p += FDLEN;
	}
}

/*
 * manage storage for Files;
 * cheap hack for speed, to cut malloc calls
 * keep a free list of Files, linked on f->data
 */

static File *fflist;
#define	FILECHUNK	100

File *
falloc()
{
	register File *f;
	register int i;

	if ((f = fflist) != NULL) {
		fflist = (File *)f->data;
		return (f);
	}
	f = (File *)emalloc(FILECHUNK*sizeof(File));
	for (i = 0; i < FILECHUNK-1; f++, i++) {
		f->data = (char *)fflist;
		fflist = f;
	}
	return (f);
}

ffree(f)
File *f;
{
	f->data = (char *)fflist;
	fflist = f;
}
