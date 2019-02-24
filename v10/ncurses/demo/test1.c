#include <curses.h>

main()
{
	WINDOW* mywin;
	WINDOW* mysubwin;
	initscr();

	mywin = newwin(24,80,0,0);
	mysubwin = subwin(mywin,10,70,5,5);

	wprintw(mysubwin,"Hello World\n");
	touchwin(mywin);
	wnoutrefresh(mywin);
	doupdate();
	endwin();
}
