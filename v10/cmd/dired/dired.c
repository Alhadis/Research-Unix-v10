/*
 **********************************************************************
 *                                                                    *
 * dired [][dir][files]  Stuart Cracraft (mclure@sri-unix) Sept 1980  *
 *								      *
 *      Directory editor.					      *
 *		edit/delete files in a directory or a file list	      *
 *              compile with the berkeley termlib archive             *
 *              and don't forget to change the 'helpfile' and         *
 *              'dirednam' strings to reflect your own setup.         *
 *                                                                    *
 *      Note: if you make improvements, I'd like to get them too.     *
 *                      Stuart Cracraft mclure@sri-unix,              *
 *				ucbvax!menlo70!sri-unix!mclure	      *
 *	so would I:	Jay Lepreau 	lepreau@utah-20,	      *
 *				decvax!randvax!utah-cs!lepreau	      *
 **********************************************************************
*/

/*
 *	Enhanced by J.Lepreau, Univ of Utah, 8-10/81:
 *	      --bunch of stuff, including more cmds, 2 window mode,
 *		buffering output, pathname substition in !escapes,
 *		initial sort options, ^G escape from sorts, ^U escape
 *		from !escapes, empty dir check.
 */

#ifdef COMMENT

Modified 12/81 by Lepreau:
1. Fix up aborting of command escapes, with mildy tricky re-display.
2. Make default window size be half-screen (2 window mode).
   Added option -wf to get full screen.
3. Add check for too many files to avoid core-dump.
4. Fix bug in cmd escapes: must reset caught signals before invocation.

#endif COMMENT

/*
   things to consider
   -    perhaps should be able to edit protection field and
		be able to change protection instantly at the
		touch of a key (or queue the change for later exit)

   --	Would be nice if there were cmds (e.g. 'c') which would update
		1) the file-info & re-display it, & 2) update whole
		display's info.
*/

#ifdef vax				/* this does for now */
# define VFORK
#endif

#ifndef VFORK
# define vfork fork
#endif

#define	FALSE		0
#define	TRUE		1
#define	NOTDELETED	0
#define	DELETED		1

/*	Sort Orders */
#define NAME		0
#define	SIZE		1
#define	WRITE		2
#define	READ		3
char sortstr[] = "nswr";	/* must be in order by above */

#define ESC         '\033'
#define CURSOR          48	/* X-coord of cursor, just b4 file */
#define CTRL(c)		('c' & 037)

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sgtty.h>
#include <sys/stat.h>
#ifdef	BSD
#include <sys/dir.h>
#else
#include <ndir.h>
#define	index	strchr
#endif

#ifdef	UTAH
char   *dirednam = "/usr/local/dired ";    /* Where dired lives - need blnk */
char   *helpfile = "/usr/help/dired.hlp";	/* Where helpfile lives */
#else
char   *dirednam = "dired ";    /* Where dired lives - need blnk */
char   *helpfile = "/usr/lib/dired";	/* Where helpfile lives */
#endif

char   divider[132];	/* divides the windows */
#define DIVCHAR	'-'	/* makes up the divider */

#define MOREPGM	"p "	/* program to page thru a file */

#define MAXFILES	2000	/* Max number of files we can handle */
#define	MAXN	30

struct lbuf
{
    union
    {
	char    lname[MAXN];
	char   *namep;
    } ln;
    short   deleted;
    char    ltype;
    short   lnum;
    short   lflags;
    short   lnl;
    unsigned short   luid;
    unsigned short   lgid;
    long    lsize;
    long    latime;
    long    lctime;
    long    lmtime;
};

struct lbuf file[MAXFILES];

struct stat	statbuf;
struct sgttyb   ioctlb;

#define	ISARG	0100000

int     gflg,
        iflg,
        lflg,
        sflg;			/* Random flags */
int	splitflg;		/* Split screen? */
int     sortyp;			/* Key to sort on */
int     errcode;		/* Error variable used by rm */
int     rflg = 1;		/* Reverse sort flag */
int     totfiles = 0;		/* Total files */
int     flags;			/* Gets flags */
int     blurb;			/* 1=>thing in echo area,0 otherwise  */
int     numdeleted;		/* Number of files marked deleted */
long    ttime;			/* Temp time variable */
long    year;			/* Six months ago */
long    totblocks = 0;		/* Total blocks */
int     lastuid = -1;		/* Last uid/gid we handled */
char    tempbuf[128];		/* Random temporary buffer */
char    userbuf[35];		/* Temporary buffer for user name */
FILE 	*pwdf;			/* Password/group file */

int     curfile = 0;		/* Current file */
int     topfile = 0;		/* File at top of screen */
int     curline = 0;		/* Line that we're on */
int     scrlen =  999;		/* Length of screen - dired part: half size */
int	Worklen = 0;		/* Length of 'working window', the other part*/
int	Worktop = 0;		/* Top of    " " */
int     Tscrlen;		/*Total length of screen,minus 1 for cmd line*/
int     scrwid = 79;		/* Width of screen */
char   *CL,
       *UP,			/* Termcap strings we'll use */
       *HO,
       *CM,
       *CD,			/* clear to end of display */
       *CE,
       *AL;			/* insert line */
char    PC;
char    tbuf[1024],
        tcapbuf[128];
char   *tgetstr (), *tgoto ();
char   *getenv ();
int     compar ();
char   *ctime ();
char   *index();
char   *makename ();
char   *catargs();
char   *bldnam();
long    nblock ();
int	catchint();
int	sigint;

char bufout[BUFSIZ];

main (argc, argv)
int     argc;
char   *argv[];
{
    register	int     i,
    	    		cc;
    int     numarg,
	    special,		/* flag says had % or # in cmd */
            status;
    char    nambuf[128];
    char    command;		/* Holds last command */
    char   **oldargv;
    register char   *t,
    	   	    *tt;

 /* Get terminal type */

    getcap ();

 /* Process arg flags. They must be first! */
    for (i=1; i<argc; i++) {
	if (argv[i][0] != '-')
	    break;
	switch (cc = argv[i][1]) {
	  case 'w':			/* Window size */
	    if (argv[i][2] == 'h')
		scrlen = 999;		/* Half */
	    else if (argv[i][2] == 'f')
		scrlen = 0;		/* Full */
	    else
		scrlen = atoi(&argv[i][2]);
	    break;
	  case 's':			/* Initial sort order */
	  case 'r':
	    sortyp = index(sortstr, argv[i][2]) - sortstr;
	    rflg = (cc == 's') ? 1 : -1;
	    break;
	  default:
	    printf("Unknown option %s, ignored.\n", argv[i]);
	    break;
	 }
    }
    argc -= (i - 1);
    oldargv = argv;
    argv += (i - 1);

    if (scrlen == 0)			/* full screen */
	scrlen = Tscrlen;
    else if (scrlen == 999)		/* means split in half */
        scrlen = (Tscrlen - 1) >> 1;	
    if (scrlen < 2)
	scrlen = 2;
    if (Tscrlen < scrlen)
	scrlen = Tscrlen;
    splitflg = (Tscrlen > scrlen+1);	/* 1 extra line for separator */
    if (splitflg) {
	Worklen = Tscrlen - (scrlen+1);	/* size of 'working' window */
	Worktop = scrlen + 1;		/* bottom half for now */
    }
    else
	Worklen = 0;
    
    tt = divider + scrwid - 14;
    for (t = divider; t < tt;)	/* arbitrary length */
    	*t++ = DIVCHAR;
    *t = '\0';

    signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);

    setdpy ();
    printf ("Reading");

    time (&ttime);
    year = ttime - 6L * 30L * 24L * 60L * 60L;/* 6 months ago */

    lflg = 1;
    gflg = iflg = sflg = 0;
    if (lflg)
    {
	t = "/etc/passwd";
	if (gflg)
	    t = "/etc/group";
	pwdf = fopen (t, "r");
    }
    
    
    numarg = argc;
    if (argc == 1)
	getdir (".");
    else
	if (argc == 2)
	    getdir (argv[1]);
	else
	{
	    while (--argc > 0)
	    {
		if (totfiles == MAXFILES) {
		    overflow();
		    break;
		}
		if ((totfiles % 10) == 0)
		    putchar ('.');
		if (gstat(*++argv) == 0) {
		    file[totfiles].ln.namep = *argv;
		    file[totfiles].lflags |= ISARG;
		    totfiles++;
		}
	    }
	}
    if (totfiles == 0) {
	printf("\n?Empty directory\n");
	unsetdpy();
	sleep(1);		/* So user can see it - don't worry if less */
	exit(0);
    }
    qsort (file, totfiles, sizeof (struct lbuf), compar);
    blank ();
    showscreen ();
    curxy (0, Tscrlen);
    ceol ();
    curxy (CURSOR, 0);
startup: 
    while ((command = readchar()) != 'q')
    {
	if (blurb)
	{
	    telluser ("");
	    blurb = 0;
	}
	switch (command)
	{
	    case 'a': 		/* Abort completely */
		blank ();
		unsetdpy ();
		exit (1);
		break;
	    case 'P': 		/* Print a file */
	    case 'p':		/* do a pr2 */
		if (file[curfile].ltype == 'd')
		{
		    telluser ("?Can only print files");
		    break;
		}
		tempbuf[0] = '\0';
		bldnam(tempbuf, numarg, curfile, argv);
		telluser ("Printing...");
		while ((i = vfork ()) == -1)
		    sleep (3);
		if (i == 0) {
#ifdef UTAH
		    if (command == 'p')		/* little print */
		    	execlp("pr2", "pr2", tempbuf, 0);
		    else				/* big print */
#endif
		    	execlp ("print", "print", tempbuf, 0);
		    telluser ("?Can't find program to list file.\n");
		    _exit(1);
		}
		wait (&status);
		break;

	    case '!': 		/* Execute a system command */
		telluser ("");
		curxy (0, Tscrlen);
		unsetdpy ();
		printf ("Command: ");
		tempbuf[0] = 'x';	/* dummy kludge */
		if (gets(tempbuf) != NULL && tempbuf[0] != '\0') {
		    extern   char *skipto();
		    register char *op,		/* old ptr */
				  *np;		/* new ptr */
		    char     bldbuf[70];

		    bldbuf[0] = '\0';	
		    op = tempbuf;
		    special = 0;
		    while (cc = *(np = skipto(op, "%#"))) {
			special++;		/* set flag */
			*np++ = '\0';	     /* zap the %/# and bump past it */
			strcat(bldbuf, op);	/* 1st part */
			if (cc == '%')	/* complete file name */
			    bldnam(bldbuf, numarg, curfile, argv);
			else {		/* Had # sign, trailing comp only */
			    if (numarg <= 2)
				strcat (bldbuf, file[curfile].ln.lname);
			    else
			        strcat (bldbuf, file[curfile].ln.namep);
			}
			    
			op = np;
		    }
		    strcat(bldbuf, op);

		    blank ();
		    if (special) {		/* display expanded command */
			printf( "%s\n", bldbuf);
		    }	/* "system" takes long enuf for him to see it */
		    signal(SIGINT, SIG_DFL);	/* temp kludge here... */
		    signal(SIGQUIT, SIG_DFL);	/* should not use 'system' */
		    system (bldbuf);
		    signal (SIGINT, SIG_IGN);
		    signal (SIGQUIT, SIG_IGN);
		    printf ("\nCR to return...");
		    setdpy ();
		    readchar();
		    blank ();
		    showscreen ();
		    telluser ("");
		}
		else {			/* CR only, or EOF, or error */
	       /* 
	        * He changed his mind, skip it.  Since we were in
	        * cooked mode, the CR ending the gets is echoed and
	        * we lost the first line of the display.  So we
	        * re-insert it.
		*/
		    setdpy();
	            if (tempbuf[0] == 'x')	/* means EOF */
			telluser("");
		/* null entry, normal case */
		    else if (AL == 0) {		/* no insert line capability */
		        blank();
			showscreen();
		    }
		    else {			/* be a little sneakier */
			curxy(0, Tscrlen-1);	/* go back to where prompt */
			ceol();		  	/*  is now, and blank it */
			home();
			insline();		/* make some room */
			pentry(topfile);
			putchar('\n');
		    }
		    curxy(CURSOR, curline);
		}
		break;

	    case 'r': 		/* Reverse sort */
		curxy (0, Tscrlen);
		printf ("reverse ");
		rflg = -1;
	    case 's': 		/* Normal sort */
		if (command == 's')
		{
		    curxy (0, Tscrlen);
		    rflg = 1;
		}
		printf ("sort by [s,n,r,w]: ");
		command = readchar();
		while ((command == '?') || !((command == 'n') ||
		    (command == 'r') || (command == 'w') || (command == 's') ||
		    (command == CTRL(g))))
		{
		    curxy (0, Tscrlen);
		    ceol ();
		    if (rflg == -1)
			printf ("reverse ");
		    printf ("sort by size, name, read or write date: ");
		    command = readchar();
		}
		
		if (command == CTRL(g)) {		/* abort */
		    putchar(CTRL(g));			/* echo it */
		    telluser("");
		    curxy(CURSOR, curline);
		    break;
		}
		
		if (command == 's')
		    sortyp = SIZE;
		else
		    if (command == 'w')
			sortyp = WRITE;
		    else
			if (command == 'r')
			    sortyp = READ;
			else
			    sortyp = 0;
		printf ("%c", command);
		qsort (file, totfiles, sizeof (struct lbuf), compar);
		topfile = 0;
		curfile = 0;
		curline = 0;
		blank ();
		showscreen ();
		curxy (CURSOR, 0);
		break;

	    case 'e': 		/* Edit a file or directory */
		if (file[curfile].ltype == 'd') {
		    strcpy (tempbuf, dirednam);
		    catargs(tempbuf, oldargv);
		}
		else {
		    if ((t = getenv("EDITOR")) != NULL)
			strcat(strcpy(tempbuf, t), " ");
		    else
			strcpy (tempbuf, "ed ");
		}
		bldnam(tempbuf, numarg, curfile, argv);
		blank ();
		unsetdpy ();
		system (tempbuf);
		setdpy ();
		blank ();
		showscreen ();
		telluser ("");
		break;

	    case 'm': 		/* 'more' a file */
		if (file[curfile].ltype == 'd')
		{
		    telluser ("?Can only page thru files");
		    break;
		}
		strcpy (tempbuf, MOREPGM);
		bldnam(tempbuf, numarg, curfile, argv);
		blank ();
		unsetdpy ();
		system (tempbuf);
/*		if (!sigint) {	*/
		printf ("\nCR to return...");
/*  		}		*/
		setdpy ();
 	   	readchar();
		blank ();
		showscreen ();
		telluser ("");
		break;

	    case 'T':	/* don't wait at page end */
	    case 't':	/* quickly type the file -- added 5/81, J.Lepreau */ 
		if (file[curfile].ltype == 'd')
		{
		    telluser ("?Can only type files");
		    break;
		}
		tempbuf[0] = '\0';
		bldnam(tempbuf, numarg, curfile, argv);
		if (type(tempbuf, command == 't'))    /* little t means wait */
		    showscreen ();
		curxy (CURSOR, curline);
		break;

	    case 'l': 		/* Refresh screen */
	    case CTRL(l):	/* added for editor compatibility -fjl */
		blank ();
		showscreen ();
		telluser ("");
		break;
	    case 'c': 		/* Refresh current line */
		curxy (0, curline);
		pentry (curfile);
		curxy (CURSOR, curline);
		break;
	    case CTRL(v):
	    case 'f': 		/* forward window */
		fscreen ();
		break;
				/* wish we could do meta-v */
	    case 'b': 		/* backward window */
		bscreen ();
		break;
	    case CTRL(n):
	    case '\r': 
	    case '\n': 		/* next file */
		if (curfile == totfiles - 1)
		    telluser ("?At end of files");
		else
		    if (curline == scrlen - 1)
		    {
			topfile = curfile;
			curline = 0;
			blank ();
			showscreen ();
			curxy (CURSOR, 0);
			downline ();
		    }
		    else
			downline ();
		break;
	    case '^': 		/* previous file */
	    case CTRL(h):		/* backspace */
	    case '-':
	    case CTRL(p):
		if (curfile == 0)
		    telluser ("?At start of files");
		else
		    if (curline == 0)
		    {
			topfile = curfile - scrlen + 1;
			curline = scrlen - 1;
			blank ();
			showscreen ();
			curxy (CURSOR, curline);
			upline ();
		    }
		    else
			upline ();
		break;
	    case 'h': 		/* Help */
	    case '?': 
		if (type(helpfile, 1))		/* wait */
		    showscreen ();
		curxy (CURSOR, curline);
		break;
	    case 'd': 		/* delete file */
		if (file[curfile].deleted == DELETED)
		    telluser ("?Already marked deleted");
		else
		{
		    numdeleted++;
		    file[curfile].deleted = DELETED;
		    printf ("D%c", 010);
		    if (curline + 1 == scrlen)
		    {
			fscreen ();
			downline ();
		    }
		    else
			if (curfile != totfiles - 1)
			    downline ();
		}
		break;
	    case 'u': 		/* undelete file */
		if (file[curfile].deleted == NOTDELETED)
		    telluser ("?Not marked deleted");
		else
		{
		    numdeleted--;
		    file[curfile].deleted = NOTDELETED;
		    printf (" %c", 010);
		}
		break;
	    default: 
		telluser ("Unknown command. Type ? or h for help");
		break;
	}
    }
    if (numdeleted)
    {
	blank ();
	printf ("The following %s marked for deletion:\n",
		(numdeleted == 1) ? "is" : "are");
	typefiles ();
	printf ("\nShall I delete %s? ",
		(numdeleted == 1) ? "this" : "these");
	if ((command = readchar()) != 'y')
	{
	    blank ();
	    showscreen ();
	    curxy (0, Tscrlen);
	    ceol ();
	    curxy (CURSOR, curline);
	    goto startup;
	}
	else
	{
	    printf ("y\n");
	    for (i = 0; i < totfiles; i++)
		if (file[i].deleted == DELETED) {
		    nambuf[0] = '\0';
		    bldnam(nambuf, numarg, i, argv);
		    if (file[i].ltype == 'd')
			rm (nambuf, 0);
		    else
		    	if (unlink (nambuf) < 0)
			    printf ("Delete of %s failed.\n", nambuf);
		}
	}
    }
    else
	blank ();

    unsetdpy ();
    exit(0);
}

typefiles ()
{
    int     longsiz,
            i,
            j,
            maxperln,
            numout,
            longthis;
    longsiz = numout = 0;
    for (i = 0; i < totfiles; i++)
	if (file[i].deleted == DELETED)
	    if (file[i].lflags & ISARG)
	    {
		if (strlen (file[i].ln.namep) > longsiz)
		{
		    longsiz = strlen (file[i].ln.namep);
		}
	    }
	    else
	    {
		if (strlen (file[i].ln.lname) > longsiz)
		{
		    longsiz = strlen (file[i].ln.lname);
		}
	    }
    maxperln = scrwid / (longsiz + 3);
    for (i = 0; i < totfiles; i++)
	if (file[i].deleted == DELETED)
	{
	    if (file[i].lflags & ISARG)
	    {
		printf ("%s", file[i].ln.namep);
		longthis = strlen (file[i].ln.namep);
	    }
	    else
	    {
		printf ("%.14s", file[i].ln.lname);
		longthis = strlen (file[i].ln.lname);
	    }
	    numout++;
	    if ((numout % maxperln) == 0)
		putchar ('\n');
	    else if (numout != numdeleted)
		for (j = 0; j < (longsiz + 3 - longthis); j++)
		    putchar (' ');
	}
}

rm (arg, level)
char    arg[];
{
    struct stat buf;
    struct direct   direct;
    char    name[100];
    int     d;

    if (stat (arg, &buf))
    {
	return;
    }
    if ((buf.st_mode & S_IFMT) == S_IFDIR)
    {
	if (access (arg, 02) < 0)
	{
	    printf ("%s not deleted.\n", arg);
	    return;
	}
	if ((d = open (arg, 0)) < 0)
	{
	    printf ("rm: %s: cannot read\n", arg);
	    return;
	}
	while (read (d, (char *) & direct, sizeof (direct)) == sizeof (direct))
	{
	    if (direct.d_ino != 0 && !dotname (direct.d_name))
	    {
		sprintf (name, "%s/%.14s", arg, direct.d_name);
		rm (name, level + 1);
	    }
	}
	close (d);
	errcode += rmdir (arg);
	return;
    }

    if (unlink (arg))
    {
	++errcode;
	printf ("%s not deleted.\n", arg);
    }
}

dotname (s)
char   *s;
{
    if (s[0] == '.')
	if (s[1] == '.')
	    if (s[2] == '\0')
		return (1);
	    else
		return (0);
	else
	    if (s[1] == '\0')
		return (1);
    return (0);
}

rmdir (f)
char   *f;
{
    int     status,
            i;

    if (dotname (f))
	return (0);
    while ((i = vfork ()) == -1)
	sleep (3);
    if (i == 0) {
    	execl ("/bin/rmdir", "rmdir", f, 0);
    	execl ("/usr/bin/rmdir", "rmdir", f, 0);
    	printf ("rm: can't find rmdir\n");
    	_exit(1);
    }
    wait (&status);
    return (status);
}

fscreen ()
{
    if (topfile + scrlen - 1 > totfiles - 1)
	telluser ("?No remaining windows");
    else
    {
	topfile = topfile + scrlen - 1;
	curfile = topfile;
	curline = 0;
	blank ();
	showscreen ();
	curxy (CURSOR, 0);
    }
}

bscreen ()
{
    if (topfile - scrlen + 1 < 0)
	telluser ("?No previous windows");
    else
    {
	topfile = topfile - scrlen + 1;
	curfile = topfile;
	curline = 0;
	blank ();
	showscreen ();
	curxy (CURSOR, 0);
    }
}

showscreen ()
{
    int     i,
            numprint;
    home ();
    numprint = 0;
    for (i = topfile; (numprint < scrlen) && (i < totfiles); i++)
    {
	numprint++;
	pentry (i);
	putchar ('\n');
    }
    if (splitflg)
    	printf ("%s\n", divider);
}

getdir (dir)			/* Reads directory dir */
char   *dir;
{
    static struct direct    dentry;
    register int    j;
    struct direct *dp;
    DIR *dirf;

    if ((dirf = opendir (dir)) == NULL)
    {
	printf ("\nSorry, %s unreadable.\n", dir);
	unsetdpy ();
	exit(1);
    }
    for (;;)
    {
	if ((dp = readdir(dirf)) == NULL)
	    break;
	dentry = *dp;
	if (dentry.d_ino == 0
		|| dentry.d_name[0] == '.' && (dentry.d_name[1] == '\0'
		    || dentry.d_name[1] == '.' && dentry.d_name[2] == '\0'))
	    continue;

	if (totfiles == MAXFILES)
	    overflow();			/* abort, too may files */
	/* Just ignore if can't find the file, dir may be changing */
	if (gstat (makename (dir, dentry.d_name)) == 0) {	/* 0 == Ok */
	    file[totfiles].lnum = dentry.d_ino;
	    strncpy(file[totfiles].ln.lname,  dentry.d_name, MAXN);
	    totfiles++;
	    if (totfiles % 10 == 0)
	        putchar ('.');
	}
    }
    closedir (dirf);
}

gstat (name)			/* Stats the file with name */
char   *name;
{

    file[totfiles].lflags = 0;
    file[totfiles].lnum = 0;
    file[totfiles].ltype = '-';

    if (stat (name, &statbuf) < 0)
    {
	return(-1);
    }
    file[totfiles].lnum = statbuf.st_ino;
    file[totfiles].lsize = statbuf.st_size;
    switch (statbuf.st_mode & S_IFMT)
    {
	case S_IFLNK: 
	    file[totfiles].ltype = 'l';
	    break;
	case S_IFDIR: 
	    file[totfiles].ltype = 'd';
	    break;
	case S_IFBLK: 
	    file[totfiles] .ltype = 'b';
	    file[totfiles].lsize = statbuf.st_rdev;
	    break;
	case S_IFCHR: 
	    file[totfiles].ltype = 'c';
	    file[totfiles].lsize = statbuf.st_rdev;
	    break;
    }
    file[totfiles].lflags = statbuf.st_mode & ~S_IFMT;
    file[totfiles].luid = statbuf.st_uid;
    file[totfiles].lgid = statbuf.st_gid;
    file[totfiles].lnl = statbuf.st_nlink;
    file[totfiles].latime = statbuf.st_atime;
    file[totfiles].lctime = statbuf.st_ctime;
    file[totfiles].lmtime = statbuf.st_mtime;
    totblocks += nblock (statbuf.st_size);
    return(0);
}

char *
makename (dir, filen)
char   *dir,
       *filen;
{
    static char dfile[100];
    register char  *dp,
                   *fp;
    register int    i;

    dp = dfile;
    fp = dir;
    while (*fp)
	*dp++ = *fp++;
    *dp++ = '/';
    fp = filen;
    strcpy(dp, fp);
    return (dfile);
}

long
nblock (size)
long    size;
{
    return ((size + 511) >> 9);
}

pentry (whichone)
int     whichone;
{
    struct
    {
	char    dminor,
	        dmajor;
    };
    register    t;
    register char  *cp;

    if (file[whichone].lnum == -1)
	return;
    if (iflg)
	printf ("%5u ", file[whichone].lnum);
    if (sflg)
	printf ("%4D ", nblock (file[whichone].lsize));
    if (lflg)
    {
	putchar (file[whichone].ltype);
	pmode (file[whichone].lflags);
	printf ("%2d ", file[whichone].lnl);
	t = file[whichone].luid;
	if (gflg)
	    t = file[whichone].lgid;
	if (getname (t, userbuf) == 0)
	    printf ("%-14.14s", userbuf);
	else
	    printf ("%-14d", t);
	if (file[whichone].ltype == 'b' || file[whichone].ltype == 'c')
	    printf ("%3d,%3d", major ((int) file[whichone].lsize),
		    minor ((int) file[whichone].lsize));
	else
	    printf ("%7ld", file[whichone].lsize);
	if ((sortyp == WRITE) || (sortyp == 0) || (sortyp == SIZE))
	{
	    cp = ctime (&file[whichone].lmtime);
	    if (file[whichone].lmtime < year)
		printf (" %-7.7s %-4.4s ", cp + 4, cp + 20);
	    else
		printf (" %-12.12s ", cp + 4);
	}
	else
	    if (sortyp == READ)
	    {
		cp = ctime (&file[whichone].latime);
		if (file[whichone].latime < year)
		    printf (" %-7.7s %-4.4s ", cp + 4, cp + 20);
		else
		    printf (" %-12.12s ", cp + 4);
	    }
    }
    printf ("%c", file[whichone].deleted ? 'D' : ' ');
    if (file[whichone].lflags & ISARG)
	printf (" %s", file[whichone].ln.namep);
    else {
	putchar(' ');
	for (t=0; t<14; t++) {
	    register c = file[whichone].ln.lname[t] & 0377;;
	    if (c == '\0')
		break;
	    if (c<' ' || c>=0200) {
		c &= 0177;
		c |= 0100;
		putchar('^');
	    }
	    putchar(c);
	}
    }
}

getname (uid, buf)
int     uid;
char    buf[];
{
    int     j,
            c,
            n, sign,
            i;

    if (uid == lastuid)
	return (0);
    if (pwdf == NULL)
	return (-1);
    rewind (pwdf);
    lastuid = -1;
    do
    {
	i = 0;
	j = 0;
	n = 0;
	sign = 0;
	while ((c = fgetc (pwdf)) != '\n')
	{
	    if (c == EOF)
		return (-1);
	    if (c == ':')
	    {
		j++;
		c = '0';
	    }
	    if (j == 0)
		buf[i++] = c;
	    if (j == 2) {
		if (c=='-')
		    sign = 1;
		else
		    n = n * 10 + c - '0';
	    }
	}
	if (sign)
	    n = -n;
	n = (unsigned short)n;
    } while (n != uid);
    buf[i++] = '\0';
    lastuid = uid;
    return (0);
}

int     m1[] =
{
    1, S_IREAD >> 0, 'r', '-'
};
int     m2[] =
{
    1, S_IWRITE >> 0, 'w', '-'
};
int     m3[] =
{
    2, S_ISUID, 's', S_IEXEC >> 0, 'x', '-'
};
int     m4[] =
{
    1, S_IREAD >> 3, 'r', '-'
};
int     m5[] =
{
    1, S_IWRITE >> 3, 'w', '-'
};
int     m6[] =
{
    2, S_ISGID, 's', S_IEXEC >> 3, 'x', '-'
};
int     m7[] =
{
    1, S_IREAD >> 6, 'r', '-'
};
int     m8[] =
{
    1, S_IWRITE >> 6, 'w', '-'
};
int     m9[] =
{
    2, 0/*S_ISVTX*/, 't', S_IEXEC >> 6, 'x', '-'
};

int    *m[] =
{
    m1, m2, m3, m4, m5, m6, m7, m8, m9
};

pmode (aflag)
{
    register int  **mp;

    flags = aflag;
    for (mp = &m[0]; mp < &m[sizeof (m) / sizeof (m[0])];)
	select (*mp++);
}

select (pairp)
register int   *pairp;
{
    register int    n;

    n = *pairp++;
    while (--n >= 0 && (flags & *pairp++) == 0)
	pairp++;
    putchar (*pairp);
}

compar (pp1, pp2)
struct lbuf *pp1,
           *pp2;
{
    register struct lbuf   *p1,
                           *p2;

    p1 = pp1;
    p2 = pp2;
    if (p1 -> lflags & ISARG && p1 -> ltype == 'd')
    {
	if (!(p2 -> lflags & ISARG && p2 -> ltype == 'd'))
	    return (1);
    }
    else
    {
	if (p2 -> lflags & ISARG && p2 -> ltype == 'd')
	    return (-1);
    }
    if (sortyp == SIZE)
    {
	if (p2 -> lsize == p1 -> lsize)
	    return (0);
	if (p2 -> lsize > p1 -> lsize)
	    return (rflg);
	return (-rflg);
    }
    else
	if (sortyp == WRITE)
	{
	    if (p2 -> lmtime == p1 -> lmtime)
		return (0);
	    if (p2 -> lmtime > p1 -> lmtime)
		return (rflg);
	    return (-rflg);
	}
	else
	    if (sortyp == READ)
	    {
		if (p2 -> latime == p1 -> latime)
		    return (0);
		if (p2 -> latime > p1 -> latime)
		    return (rflg);
		return (-rflg);
	    }
    return (rflg * strcmp (p1 -> lflags & ISARG ? p1 -> ln.namep : p1 -> ln.lname,
		p2 -> lflags & ISARG ? p2 -> ln.namep : p2 -> ln.lname));
}

ceod()
{
    putpad(CD);
}

ceol ()
{
    putpad (CE);
}
blank ()
{
    putpad (CL);
}
home ()
{
    if (HO)
	putpad (HO);
    else
	curxy(0,0);
}
insline ()
{
    putpad (AL);
}

/* Yes, folks, we use direct cursor addressing to get to next line!
   Before you mumble "What sort of cretin would do this?" here's
   the reason. We don't use \n since that obviously won't work.
   We don't use \012 since virgin version 7 makes that into a crlf.
   We don't use raw mode since we type out help files efficently,
   and we don't want to switch modes all the time. So enjoy. -- SMC */

downline ()
{
    curxy (CURSOR, ++curline);
    curfile++;
}
upline ()
{
    putpad (UP);
    curline--;
    curfile--;
}

/*VARARGS1*/
telluser (msg, args)
char   *msg;
{
    curxy (0, Tscrlen);
    ceol ();
    printf (msg, args);
    curxy (CURSOR, curline);
    blurb++;
}
curxy (col, lin)
{
    char   *cmstr = tgoto (CM, col, lin);
    putpad (cmstr);
}

char *fgets();

type (filestr, waitflg)    /* Modified to type help file & others. fjl 5/81 */
char *filestr;			/* Kludgy now with split screen stuff! */
{
    int     helpfd = 5;
    FILE    *fd = stdin;
    char    *eof;
    register int     i, n;
    register int     cc = 0;
    int	    cur_scrl;		/* current screen length */
    char    helpbuf[512];
    
    if (!splitflg)
	helpfd = open(filestr, 0);
    else
    	fd = fopen(filestr, "r");
    if (helpfd < 0 || fd == NULL) {
    	telluser("?Unable to open %sfile",strcmp(filestr,helpfile)?"":"help ");
	return (FALSE);
    }
    
    signal(SIGINT, catchint);
    sigint = 0;
    
    if (!splitflg) {
    	blank();
	fflush(stdout);
    	while ((i = read (helpfd, helpbuf, 512)) > 0 && !sigint)
	    write (1, helpbuf, i);
	close(helpfd);
    }
    else {
	cur_scrl = totfiles - topfile + 1;	/* topfile starts at 0 */
	Worktop = ((cur_scrl < scrlen) ? cur_scrl : scrlen) + 1;
	do {
	    curxy(0, Worktop);
	    ceod();
	    for (i = Worktop; (i < Tscrlen) && !sigint && (cc != EOF); i++) {
		n = 0;
		while ((cc = getc(fd)) != EOF) {
		    if (cc == '\t')
			n |= 07;
chklen:		    if (n++ == scrwid-1) {	/* Use most of screen */
			if (cc != '\n') {
			    ungetc(cc, fd);
			    cc = '\n';
			}
		    }
		    if (cc < ' ' && cc!='\n' && cc!='\t' || cc>=0200) {
			putchar('^');
			cc &= 0177;
			cc |= 0100;
			goto chklen;
		    }
		    putchar(cc);
		    if (cc == '\n')
		    	break;
		}
	    }
	} while (!sigint && (cc != EOF) && waitchk(waitflg));
	
	if (feof(fd))	
	    printf("===== End-of-File =====\n");
	fflush(stdout);
	fclose(fd);
	return(FALSE);		/* means needs no re-display */
    }
	    
    if (!splitflg) {		/* redundant now... */
    	if (!sigint) {
	    curxy(0, Tscrlen);
	    printf ("CR to return...");
    	    readchar();
    	}
    	blank ();
	return (TRUE);
    }
}

waitchk(waitflg)
{
	if (!waitflg)
	    return(1);
	fflush(stdout);
	curxy(0, Tscrlen);
	printf ("---Continue---");
	ceol();
	curxy(0, Tscrlen);
	readchar();
	ceol();
	if (sigint)
	    return(0);		/* avoids clear of screen */
	return(1);
}

setdpy ()
{
    ioctl (0, TIOCGETP, &ioctlb);
    ioctlb.sg_flags |= CBREAK;
    ioctlb.sg_flags &= ~ECHO;
    ioctl (0, TIOCSETP, &ioctlb);
}

unsetdpy ()
{
    ioctlb.sg_flags &= ~CBREAK;
    ioctlb.sg_flags |= ECHO;
    ioctl (0, TIOCSETP, &ioctlb);
}

getcap ()
{
    char   *ap;
    char   *term;
    char   *xPC;

    term = getenv ("TERM");
    if (term == 0)
    {
	printf("No TERM in environment\n");
	exit(1);
    }

    switch (tgetent (tbuf, term))
    {
	case -1: 
	    printf("Cannot open termcap file\n");
	    exit (2);
	case 0: 
	    printf("%s: unknown terminal", term);
	    exit (3);
    }

    ap = tcapbuf;

    Tscrlen = tgetnum ("li") - 1;
    scrwid = tgetnum ("co") - 1;/* lose 1 so won't scroll in last line */

    UP = tgetstr ("up", &ap);
    CD = tgetstr ("cd", &ap);
    CE = tgetstr ("ce", &ap);
    HO = tgetstr ("ho", &ap);
    CL = tgetstr ("cl", &ap);
    CM = tgetstr ("cm", &ap);
    AL = tgetstr ("al", &ap);		/* insert line, optional */

    xPC = tgetstr ("pc", &ap);
    if (xPC)
	PC = *xPC;

    if ((CM == 0) || (CL == 0) || (UP == 0))
    {
	printf("Tty must have cursor addr, clear, and 4 cursor motions.\n");
	exit (1);
    }
    if (Tscrlen <= 0 || scrwid <= 0)
    {
	printf("Must know the screen size\n");
	exit (1);
    }
}

outch (c)
{
    putchar (c);
}

putpad (str)
char   *str;
{
    if (str)
	tputs (str, 0, outch);
}

catchint(sig)
{
	signal(SIGINT, SIG_IGN);	/* reset it */
	sigint = 1;
}

char *
bldnam(str, numarg, filidx, argv)
char *str;
char *argv[];
{
	
	if (numarg == 1)
	    strcat (str, file[filidx].ln.lname);
	else
	    if (numarg == 2) {
		strcat (str, argv[1]);
		strcat (str, "/");
		strcat (str, file[filidx].ln.lname);
	    }
	    else
		strcat (str, file[filidx].ln.namep);
	return(str);
}

char *
catargs(str, argv)
char *str;
char *argv[];
{
	register int i;

	for (++argv; *argv; argv++) {
		if (**argv == '-') {
			strcat(str, *argv);
			strcat(str, " ");
		}
	}
	return(str);
}

overflow()
{
    printf("\n?Too many files\007\n");
    fflush(stdout);
    sleep(1);		/* So user can see it - don't worry if less */
}

char *skipto (string,charset)
char *charset,*string;
{
	register char *setp,*strp;
	register int found;

	found = 0;			/* not found yet */
	strp = string;			/* start at first char */

	while (*strp && !found) {	/* until null or found */
		/* find first char in charset matching *strp */
		for (setp=charset; (*setp) && (*setp != *strp); setp++) ;
		if (*setp)	found = 1;	/* matches a char */
		else		strp++;		/* else keep looking */
	}

	return (strp);
}

readchar()
{
	static neofs;
	register c;

	fflush(stdout);
	c = getchar();
	if (c == EOF)
		if (++neofs > 100)
			exit(1);
	return(c);
}
