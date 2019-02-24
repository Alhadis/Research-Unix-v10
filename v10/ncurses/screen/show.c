#include <curses.h>
#include <signal.h>

#ifdef DEBUGfoo
#undef LINES
#define LINES 5
#endif

main(argc, argv)
char **argv;
{
	FILE *fd;
	char linebuf[512];
	int line;
	int done();

	if (argc < 2) {
		(void) fprintf(stderr, "Usage: show file\n");
		exit(1);
	}
	fd = fopen(argv[1], "r");
	if (fd == NULL) {
		perror(argv[1]);
		exit(2);
	}
	(void) signal(SIGINT, done);	/* die gracefully */

	initscr();			/* initialize curses */
	noecho();			/* turn off tty echo */
	cbreak();			/* enter cbreak mode */
	nonl();				/* allow more optimizations */
	idlok(stdscr, TRUE);		/* allow insert/delete line */

	for (;;) {			/* for each screen full */
		(void) move(0, 0);
		/* werase(stdscr); */
		for (line=0; line<LINES; line++) {
			if (fgets(linebuf, sizeof linebuf, fd) == NULL) {
				clrtobot();
				done();
			}
			(void) mvprintw(line, 0, "%s", linebuf);
		}
		(void) refresh();	/* sync screen */
		if(getch() == 'q')	/* wait for user to read it */
			done();
	}
}

/*
 * Clean up and exit.
 */
done()
{
	(void) move(LINES-1,0);		/* to lower left corner */
	clrtoeol();			/* clear bottom line */
	(void) refresh();		/* flush out everything */
	endwin();			/* curses cleanup */
	exit(0);
}
