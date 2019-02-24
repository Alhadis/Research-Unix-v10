#include <pads.pub>
SRCFILE("help.c")

char *HelpFile[] = {
"The generic behavior of Pads:",
" ",
"Mouse:",
"LEFT:   select window; select line; move scroll bar",
"MIDDLE: line menu; sweep window",
"RIGHT:  window menu; move, sweep, select, window",
" ",
"         MIDDLE          RIGHT",
" ",
"          line          window",
"        specific       specific",
"       operations     operations",
"       ~~~~~~~~~~     ~~~~~~~~~~",
"          line          window",
"        generics       generics",
" ",
"'sever' removes the selected line and those above it.",
"'fold' and 'truncate' toggle right-hand wrap of lines",
"wider than the window.  Move cursor over the arrow in",
"'top ->' for a sub-menu of all windows.",
" ",
"Keyboard:",
"Characters from the keyboard accumulate at the bottom",
"of the layer. The window or line currently associated",
"with the keyboard, if any, flashes at each keystroke.",
"At carriage return, the text is sent to the window or",
"line. Each window or line that accepts keyboard input",
"displays help in response to '?'. ESCAPE key gets mux",
"snarf buffer. '<shell cmd' gets each line of standard",
"output. '>shell cmd' sends selected line or window as",
"standard input.",
0 };

class Help : public PadRcv {
	Pad	*pad;
	int	lines;
	void	linereq(long,Attrib=0);
public:
		Help();
};

Help.Help()
{
	trace( "%d.Help()", this );
	for( lines = 1; HelpFile[lines]; ++lines ) {}
	pad = new Pad( (PadRcv*) this );
	pad->lines(lines);
	pad->options(TRUNCATE);
	pad->banner( "Help:" );
	pad->name( "help" );
}

void Help.linereq(long l, Attrib)
{
	trace( "%d.linereq(%d)", this, l );
	if( l>=1 && l<=lines )
		pad->insert( l, HelpFile[l-1] );
}

void NewHelp()
{
	new Help;
}
