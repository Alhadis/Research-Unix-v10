#include "defs.h"
#include <font.h> 


char getkbdstr() {

	int ch, i, strstatus=1, r;
	static char str[MXLINE];


	i = 0;
	for(;;)  {
		r = wait(KBD|MOUSE);

		if (r&KBD) {
			while ( (ch = kbdchar()) == -1 ) 
				wait(KBD);
			if ( (ch != '\n') && (ch != '\r') && (ch != '\b') ) {
				str[i] = ch;
				showstr( ch, strstatus, boxpt );
				if ( i == 0 )
					strstatus = 0;
				/* no longer a new string; cont. placing chars on box line */
				i++;
			}

			else {
				if ( ch == '\b' ) { 
				/* backspace */
		 			if ( i > 0 ) {
						i--;
						showstr( str[i], -1, boxpt );
					}
				}
				else {
					rectf( &display, box, f1 );
					if (ch == '\r' && i == 0)
					/* returned before typing anything */
						return("-1\0");
					else {
					/* end of string */
						str[i] = '\0';
						return( str );
					}
				}
			}
		}
		else if (r&MOUSE) {
			if (button1())  {
				while (button1())  ;
				if (!ptinrect(mouse.xy, box)) {
					rectf(&display, box, f1);
					sprintf(str, "-1\0");
					return(str);	
				}
			}
		}
	}
}



showcurrentlabel( str, pt )
char str[]; 
Point pt; {

	int i = 1;

	showstr( str[0], 1, pt );
	
	while ( str[i] != '\0' ) {
		showstr( str[i], 0, pt );
		i++;
	}
	return;
}
		


showstr( ch, newstr, firstpt )    /* show string in box or in pic */
char ch; 
int newstr; 
Point firstpt; {

	char str[MINSTR];
	static Point pt;
	

	sprintf( str, "%c\0", ch );

	if ( newstr == -1 ) {    /* backspace */
		pt.x -= 9;
		string( &defont, str, &display, pt, f2 );  
		return;
	}

	
	if ( newstr ) 
		pt = string( &defont, str, &display, firstpt, f2 );  
			/* start at beg of box line */
	else 
		pt = string( &defont, str, &display, pt, f2 );

	return;
}
