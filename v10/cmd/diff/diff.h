/* @(#)diff.h 4.1 10/9/80" */

/*
 * diff - common declarations
 */

#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/dir.h>

/*
 * Output format options
 */
int	opt;

#define	D_NORMAL	0	/* Normal output */
#define	D_EDIT		-1	/* Editor script out */
#define	D_REVERSE	1	/* Reverse editor script */
#define	D_CONTEXT	2	/* Diff with context */

/*
 * Algorithm related options
 */
int	hflag;	/* -h, use halfhearted DIFFH */
int	bflag;	/* 1 = ignore quantity of white space
		   2 = ignore white space entirely */

/*
 * Options on hierarchical diffs.
 */
int	rflag;			/* recursively trace directories */
int	sflag;			/* announce files which are same */

/*
 * Variables for -c context option.
 */
int	context;		/* lines of context to be printed */

/*
 * State for exit status.
 */
int	status;
int	anychange;
char	tempfile[2][16];	/* used when comparing against char special 
				   devices or standard input */
int	whichtemp;

/*
 * Variables for diffdir.
 */
char	**diffargv;		/* option list to pass to recursive diffs */

/*
 * Input file names.
 * With diffdir, file1 and file2 are allocated BUFSIZ space,
 * and padded with a '/', and then efile0 and efile1 point after
 * the '/'.
 */
char	*file1, *file2, *efile1, *efile2;
struct	stat stb1, stb2;
struct	stat stb1, stb2;

/*
 * This is allocated early, and used
 * to reset the free storage pointer to effect space compaction.
 */
char	*dummy;

char	*malloc(), *realloc(), *talloc(), *ralloc();
char	*savestr(), *splice(), *splicen();
char	*mktemp(), *copytemp(), *strrchr();
int	done();

extern	char diffh[], diff[], pr[];
