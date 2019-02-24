#include "asd.h"
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "ftw.h"

FILE *tf;
static Sig_typ sigsav;
static int rc;
static char tfname[L_tmpnam];
static struct stat outsb;
static struct pack *pkhead, *pktail;

/*
 *	The following structure helps keep track of things being packaged.
 *	iname is the internal name of the component -- in other words,
 *	the archive element name.  ename is the (short) pathname of the
 *	file.  The structures are chained by the "link" field.  All the
 *	other fields are copies of things returned by "stat" and are
 *	used mostly to make sure nothing changed while we were packaging.
 *	head and tail point to the first and last items in the chain.
 *	The first item is known to refer to the "Instructions" component.
 */
struct pack {
	char *iname;
	char *ename;
	struct pack *link;
	dev_t dev;
	ino_t ino;
	int uid, gid, mode;
	time_t time;
	off_t size;
};

/*
 *	Prepare to build a package.  The package will appear on stdout.
 *	The argument is nonzero if remarks are to be read.
 */

static void
delete(int arg)
{
	unlink (tfname);
	exit (3);
}

void
pkgstart(void)
{
	struct stat pks;

	rc = 0;

	nchk (fstat (fileno (stdout), &outsb));

	/* establish a temporary file to hold the instruction information */
	tmpnam (tfname);
	tf = fopen (tfname, "w");
	sigsav = signal (SIGINT, SIG_IGN);
	if (sigsav != SIG_IGN)
		signal (SIGINT, delete);
	nchk (chmod (tfname, 0644));
	schk ((char *) tf);

	/* create the initial element in the component chain */
	pkhead = pktail = new (struct pack);
	pkhead->iname = copy (instr);
	pkhead->ename = copy (tfname);
	pkhead->time = 0;
	pkhead->link = NULL;
	pkhead->uid = getuid();
	nchk (fstat (fileno(tf), &pks));
	pkhead->gid = pks.st_gid;
	pkhead->mode = 0100644;
}

static int consider(char *, struct stat *, int, struct FTW *);

/*
 *	put a file (or directory) into a package
 */
void
pkgfile (char *file)
{
	register int x;
	struct stat sb;

	/* if the file is truly not present, generate a remove request */
#ifdef S_IFLNK
	if (lstat (file, &sb) < 0 && errno == ENOENT) {
#else
	if (stat (file, &sb) < 0 && errno == ENOENT) {
#endif
		fprintf (tf, "r ");
		putpath (tf, transname (file));
		putc ('\n', tf);
	} else {
		x = ftw (file, consider, 8);
		if (x != 0) {
			rc++;
			if (x == -1)
				perror (file);
		}
	}
}

/*
 *	we are done building a package.  This writes the actual file
 *	contents, so make sure the files are still around
 */
int
pkgend(void)
{
	register struct pack *pack;
	struct stat s;

	/* write 'x' or 'X' execution item if requested */
	if (xstr) {
		fprintf (tf, "x ");
		putpath (tf, xstr);
		fprintf (tf, "\n");
	}
	if (Xstr) {
		fprintf (tf, "X ");
		putpath (tf, Xstr);
		fprintf (tf, "\n");
	}

	/* we now know how long the first component is */
	pkhead->size = ftell (tf);

	/* we no longer need to write the temporary file */
	fclose (tf);

	/* describe the temp file correctly so it will pass later checks */
	if (pkhead->time == 0)
		(void) time (&pkhead->time);
	nchk (stat (tfname, &s));
	pkhead->dev = s.st_dev;
	pkhead->ino = s.st_ino;

	/*
	 * write the files out into an archive
	 */

	/* first the archive header */
	printf (ARMAG);

	/*
	 *	run through the list, creating the archive components
	 *	and deleting the list entries.  One iteration per component.
	 *	We know there is at least one component, because the
	 *	"Instructions" component must be represented.
	 */
	do {
		struct ar_hdr ah;
		char buf[30];
		register int c;

		/* "pack" is the package component under consideration */
		pack = pkhead;

		if (pack->iname) {	/* non-regular file entry, no data */

			/* log it if requested */
			if (vflag)
				fprintf (stderr, "package %s\n",
				    strcmp (pack->iname, instr)? pack->ename: instr);

			/* write the archive element header */

#		define ent(a,x) sprintf(buf, "%-*s", sizeof(ah.a), x); \
			strncpy (ah.a, buf, sizeof (ah.a))
			ent (ar_name, pack->iname);
			ent (ar_fmag, ARFMAG);
#		undef ent

#		define ent(a,x) sprintf(buf, "%*ld", sizeof(ah.a), (long) x); \
			strncpy (ah.a, buf, sizeof (ah.a))
			ent (ar_date, pack->time);
			ent (ar_uid, pack->uid);
			ent (ar_gid, pack->gid);
			ent (ar_size, pack->size);
#		undef ent

#		define ent(a,x) sprintf(buf, "%*o", sizeof(ah.a), x); \
			strncpy (ah.a, buf, sizeof (ah.a))
			ent (ar_mode, pack->mode);
#		undef ent

			fwrite ((char *) &ah, sizeof (ah), 1, stdout);

			/* write the archive element itself */
			tf = fopen (pack->ename, "r");
			if (tf == NULL) {
				perror (pack->iname);
				exit (1);
			}

			while ((c = getc (tf)) != EOF)
				putchar (c);
			
			/* if things now don't match, complain */
			if (fstat (fileno (tf), &s) < 0 || s.st_size != pack->size ||
			    (s.st_mtime != pack->time && strcmp (pack->iname, instr)) ||
			    s.st_dev != pack->dev || s.st_ino != pack->ino ||
			    s.st_uid != pack->uid || s.st_gid != pack->gid ||
			    (s.st_mode & 07777) != (pack->mode & 07777)) {
				fprintf (stderr, "phase error on %s\n",
				    pack->ename);
				rc++;
			}

			fclose (tf);

			if (pack->size & 1)
				putchar ('\n');
		}
		
		/* delete the element, move on to the next */
		if (pack->iname) free (pack->iname);
		free (pack->ename);
		pkhead = pack->link;
		free ((char *) pack);
	} while (pkhead);

	/* zap the tail pointer for general cleanliness */
	pktail = NULL;

	nchk (unlink (tfname));
	signal (SIGINT, sigsav);

	return rc;
}

static void hdrsub(char *, struct stat *);
static char *iname(char *);

/* internal function for package creation */
static int
consider (char *name, struct stat *buf, int type, struct FTW *ftw)
{
	register struct pack *pack;
	register int mode;
	char *biname;
#ifdef	S_ISLNK
	char *slname;
#endif

	switch (type) {
	case FTW_D:
		fprintf (tf, "d %-*.4o ", MAXCOMP, buf->st_mode & 07777);
		hdrsub (name, buf);
		fprintf (tf, "\n");
		break;

	case FTW_SL:
		if (ftw->level == 0) {
			ftw->quit = FTW_FOLLOW;
			return 0;
		}
		/* fall through */

	case FTW_F:
		mode = buf->st_mode;

		/* Has this file already appeared?  If so, it's a link */
		for (pack = pkhead->link; pack; pack = pack->link) {
			if (buf->st_dev == pack->dev &&
			    buf->st_ino == pack->ino) {
				fprintf (tf, "l %s ", transname (pack->ename));
				fprintf (tf, "%s\n", transname (name));
				return 0;
			}
		}

		if (S_ISREG(mode)) {
			/* refuse to package the standard output */
			if (buf->st_dev == outsb.st_dev &&
			    buf->st_ino == outsb.st_ino) {
				fprintf (stderr, "skipping output file %s\n",
				    fullname (name));
				return 0;
			}
			biname = iname (name);
			fprintf (tf, "f ");
			putpath (tf, biname);
			fprintf (tf, " ");
			hdrsub (name, buf);
			fprintf (tf, "\n");
		}
#ifdef	S_ISLNK
		else if (S_ISLNK(mode)) {
			slname = alloc((unsigned)buf->st_size+1);
			slname[buf->st_size] = '\0';
			if (readlink(name, slname, buf->st_size) !=
			    buf->st_size) {
				perror(name);
				return (0);
			}
			fprintf (tf, "s %s ", transname (slname));
			fprintf (tf, "%s\n", transname (name));
			biname = NULL;
		}
#endif
#if 0 /* We don't handle special files any more.  */
		else if (S_ISBLK(mode) || S_ISCHR(mode)) {
			fprintf (tf, "%c %#o %d %d ",
			    mode == S_IFBLK? 'b': 'c',
			    buf->st_mode & 07777,
			    major (buf->st_rdev),
		 	    minor (buf->st_rdev));
			hdrsub (name, buf);
			fprintf (tf, "\n");
			biname = NULL;
		}
#endif
		else {
			fprintf (stderr, "%s: unrecognized file type\n",
				 fullname (name));
			break;
		}


		/* package the file */
		pack = new (struct pack);
		pack->ename = copy (name);
		pack->dev = buf->st_dev;
		pack->ino = buf->st_ino;
		pack->uid = buf->st_uid;
		pack->gid = buf->st_gid;
		pack->time = buf->st_mtime;
		if (pack->time > pkhead->time)
			pkhead->time = pack->time;
		pack->size = buf->st_size;
		pack->mode = buf->st_mode;
		pack->link = NULL;
		pack->iname = biname;
		pktail->link = pack;
		pktail = pack;
		break;

	case FTW_DNR:
		fprintf (stderr, "cannot read directory %s\n", name);
		return 1;

	case FTW_NS:
		fprintf (stderr, "cannot stat %s\n", name);
		return 1;

	case FTW_DP:
		break;

	default:
		fprintf (stderr, "impossible code %d from ftw\n", type);
		exit (1);
	}
	return 0;
}

static void
hdrsub (char *name, struct stat *buf)
{
	putpath (tf, uidstr (buf->st_uid));
	putc ('\t', tf);
	putpath (tf, gidstr (buf->st_gid));
	putc ('\t', tf);
	putpath (tf, transname (name));
}

/*
 *	generate a unique internal name for a file
 */
static char *
iname (char *s)
{
	register char *p;
	register char *lastcomp;
	register struct pack *pack;
	char trial[MAXCOMP+1];

	/* point lastcomp at the last pathname component */
	lastcomp = s;
	for (p = s; *p; p++) {
		if (*p == '/')
			lastcomp = p + 1;
	}

	/* if the name is acceptably short, modify it slightly */
	if (strlen (lastcomp) <= MAXCOMP) {

		char prefix[MAXCOMP+1], suffix[MAXCOMP+1], num[30];
		register int n;

		/* split the name, remove unprintables */
		strcpy (prefix, lastcomp);
		for (p = prefix; *p; p++)
			if (*p == ' ' || !isprint (*p))
				*p = '$';
		suffix[0] = '\0';
		p = strrchr (prefix, '.');
		if (p != NULL) {
			strcpy (suffix, p);
			*p = '\0';
		}

		/* generate trial names until we run out of space */
		for (n = 0, num[0] = '\0';
		    strlen(prefix) + strlen(suffix) + strlen(num) <= MAXCOMP;
		    n++, sprintf (num, "%.0d", n)) {
			
			/* generate the trial name */
			strcpy (trial, prefix);
			strcat (trial, num);
			strcat (trial, suffix);

			/* if the name is unique, we're done */
			pack = pkhead;
			while (pack != NULL && strcmp (pack->iname, trial) != 0)
				pack = pack->link;
			if (pack == NULL)
				return copy(trial);

		}
	}

	/* punt -- generate a completely new name */
	do {
		static int tempno;

		tempno++;
		sprintf (trial, "Temp%d", tempno);
		pack = pkhead;
		while (pack != NULL && strcmp (trial, pack->iname) != 0)
			pack = pack->link;
	} while (pack != NULL);
	return copy(trial);
}
