/*
 *	news foo	prints /usr/news/foo
 *	news -a		prints all news items, latest first
 *	news -n		lists names of new items
 *	news -s		tells count of new items only
 *	news		prints items changed since last news
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <signal.h>
#include <ndir.h>
#include <pwd.h>

/*
 *	The following items should not be printed.
 */
char *ignore[] = {
	"core",
	"dead.letter",
	NULL
};

struct n_file {
	long n_time;
	char n_name[MAXNAMLEN];
} *n_list;

char NEWS[] = "/usr/news";

int n_count;
char stdbuf[BUFSIZ];

jmp_buf	save_addr;

main (argc, argv)
	int argc;
	char **argv;
{
	int print_item(), notify(), count();

	setbuf (stdout, stdbuf);
	initialize();
	read_dir();
	if (argc <= 1)
		late_news (print_item, 1);
	else if	(	argc == 2 && argv[1][0] == '-'
	    && argv[1][1] != '\0' && argv[1][2] == '\0')
		switch (argv[1][1]) {
		case 'a':
			all_news();
			break;

		case 'n':
			late_news (notify, 0);
			break;

		case 's':
			late_news (count, 0);
			break;

		default:
			fprintf (stderr, "news: bad option %s\n", argv[1]);
			exit (1);
		}
	else {
		int i;
		for (i=1; i<argc; i++)
			print_item (argv[i]);
	}
	return 0;
}

/*
 *	read_dir: get the file names and modification dates for the
 *	files in /usr/news into n_list; sort them in reverse by
 *	modification date. We assume /usr/news is the working directory.
 */

read_dir()
{
	struct direct *nf;
	struct stat sbuf;
	char fname[MAXNAMLEN];
	DIR *dirp;
	int i, j;
	char *malloc(), *realloc();

	/* Open the current directory */
	if ((dirp = opendir(".")) == NULL) {
		fprintf (stderr, "news: ");
		perror (NEWS);
		exit (1);
	}

	/* Read the file names into n_list */
	n_count = 0;
	while (nf = readdir(dirp)) {
		strncpy (fname, nf->d_name, (unsigned) strlen(nf->d_name) + 1);
		if (nf->d_ino != 0 && stat (fname, &sbuf) >= 0
		 && (sbuf.st_mode & S_IFMT) == S_IFREG) {
			register char **p;
			p = ignore;
			while (*p && strncmp (*p, nf->d_name, MAXNAMLEN))
				++p;
			if (!*p) {
				if (n_count++ > 0)
					n_list = (struct n_file *)
						realloc ((char *) n_list,
						(unsigned)
						(sizeof (struct n_file)
						    * n_count));
				else
					n_list = (struct n_file *) malloc
						((unsigned)
						(sizeof (struct n_file) *
						n_count));
				if (n_list == NULL) {
					fprintf (stderr, "news: no storage\n");
					exit (1);
				}
				n_list[n_count-1].n_time = sbuf.st_mtime;
				strncpy (n_list[n_count-1].n_name,
					nf->d_name, MAXNAMLEN);
			}
		}
	}

	/* Sort the elements of n_list in decreasing time order */
	for (i=1; i<n_count; i++)
		for (j=0; j<i; j++)
			if (n_list[j].n_time < n_list[i].n_time) {
				struct n_file temp;
				temp = n_list[i];
				n_list[i] = n_list[j];
				n_list[j] = temp;
			}

	/* Clean up */
	closedir(dirp);
}

initialize()
{
	extern _exit();
	if (signal (SIGQUIT, SIG_IGN) != SIG_IGN)
		signal (SIGQUIT, _exit);
	umask (022);
	if (chdir (NEWS) < 0) {
		fprintf (stderr, "news: ");
		perror (NEWS);
		exit (1);
	}
}

all_news()
{
	int i;

	for (i=0; i<n_count; i++)
		print_item (n_list[i].n_name);
}

print_item (f)
	char *f;
{
	FILE *fd;
	char fname[MAXNAMLEN+1];
	static int firstitem = 1;
	int onintr();
	struct passwd *getpwuid();

	if (f == NULL) {
		return;
	}
	strncpy (fname, f, MAXNAMLEN);
	fname[MAXNAMLEN] = '\0';
	if ((fd = fopen (fname, "r")) == NULL) {
		fprintf (stderr, "news: %s/", NEWS);
		perror (fname);
	} else {
		register int c, ip, op;
		struct stat sbuf;
		char *ctime();
		struct passwd *pw;

		fstat (fileno (fd), &sbuf);
		if (firstitem) {
			firstitem = 0;
			putchar ('\n');
		}
		if (setjmp(save_addr))
			goto finish;
		if (signal(SIGINT, SIG_IGN) != SIG_IGN)
			signal(SIGINT, onintr);
		printf ("%s ", fname);
		pw = getpwuid (sbuf.st_uid);
		if (pw)
			printf ("(%s)", pw->pw_name);
		else
			printf (".....");
		printf (" %s\n", ctime (&sbuf.st_mtime));

		/*
		 *	copy the news item to the standard output
		 *
		 *	ip is the output character position corresponding
		 *	to the present input character.  op is the
		 *	actual current output character position.
		 */
		while ((c = getc (fd)) != EOF)
				putchar (c);
		fflush (stdout);
finish:
		putchar ('\n');
		fclose (fd);
		if (signal(SIGINT, SIG_IGN) != SIG_IGN)
			signal(SIGINT, SIG_DFL);
	}
}

late_news (emit, update)
	int (*emit)(), update;
{
	long cutoff;
	int i;
	char fname[50], *getenv(), *cp;
	struct stat newstime;
	int fd;
	struct {
		long actime, modtime;
	} utb;

	/* Determine the time when last called */
	cp = getenv ("HOME");
	if (cp == NULL) {
		fprintf (stderr, "news: cannot find HOME variable\n");
		exit (1);
	}
	strcpy (fname, cp);
	strcat (fname, "/");
	strcat (fname, ".news_time");
	cutoff = stat (fname, &newstime) < 0? 0: newstime.st_mtime;

	/* Print the recent items */
	for (i=0; i<n_count && n_list[i].n_time > cutoff; i++)
		(*emit) (n_list[i].n_name);
	(*emit) ((char *) NULL);
	fflush (stdout);

	if (update) {
		/* Re-create the file and refresh the update time */
		if (n_count > 0 && (fd = creat (fname, 0666)) >= 0) {
			utb.actime = utb.modtime = n_list[0].n_time;
			close (fd);
			utime (fname, &utb);
		}
	}
}

notify (s)
	char *s;
{
	static int first = 1;

	if (s) {
		if (first) {
			first = 0;
			printf ("news:", NEWS);
		}
		printf (" %.14s", s);
	} else if (!first)
		putchar ('\n');
}

/*ARGSUSED*/
count (s)
	char *s;
{
	static int nitems = 0;

	if (s)
		nitems++;
	else if (nitems) {
		printf ("%d news item", nitems);
		if (nitems > 1)
			putchar ('s');
		printf (".\n");
	}

}

onintr()
{
	sleep(2);
	longjmp(save_addr, 1);
}
