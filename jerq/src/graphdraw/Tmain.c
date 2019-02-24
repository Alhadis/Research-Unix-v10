#include "defs.h"
#include <font.h>
#include <string.h>

struct list list[MXPTS];
struct set set[MXSETS];
Rectangle Dspace, box, Lrect, Rrect;
Point boxpt, boxstart;

char *menutext[14] = 
	{"create vertex",
	 "delete",
	 "move",
	 "copy",
	 "create edge(s)",
	 "delete edge(s)",
	 "labels menu",
	 "graphics menu",
	 "macros menu",
	 "standard window",
	 "small grid",
	 "restart",
	 "exit", 
	 0};
Menu stringsmenu={menutext};

char *animmenutext[4] = 
	{"pause",
	 "kill",
	 "type command",
	 0};
Menu animmenu={animmenutext};

main( argc, argv ) 
int argc;
char *argv[]; {

	int r, i1, i2, x1, y1, t, haltflag;  /* var's for anim ops */

	int i, j, goodfn, goodfile, mkint();
	char tmp[MEDSTR], chstr[5], mkstr(), mkline(), code[5], label[30], 
		msg[80], *strcat();
	Point Ascale();

	animflag = 0;
	haltflag = 0;
	dirflag = 0;
	multedflag = 0;
	loopflag = 0;

	f1 = F_STORE;
	f2 = F_XOR;
	f3 = F_CLR;

	dsize = 4;
	csize = 10;
	Hcsize = 7;
	loopcsize = 18;

	Set_Up_Space();	/* set up all variables related to current space */

	init();
	request( MOUSE|KBD|RCV );

/* construct rect at bottom of window for entering kbd chars */
	rectf( &display, box, f1 );


	if ( argc > 1 ) {
		if ( goodfn = mkint() ) {
			if ( ((goodfile = mkint()) != 0) && ((goodfile + numverts) < 501 ) ) {
				sendchar('1');
				sendchar('\n'); 
	/*need this? yes! I think bec. not checking IS_SET like for animmenu...*/
				string(&defont, "reading file...", &display, boxstart, f2);
				rcvfile();
				scale(Dspace, 1);
				displaylist(1);
				sprintf(tmp, "read file `%s'\0", argv[1]);
			}
			else { 
				if (!goodfile)	
					sprintf(tmp, "file `%s' is in wrong format\0", argv[1]);
				else {
					sendchar('0');
					sendchar('\n');
					sprintf(tmp, "not enough room for file `%s': max is 500 vertices\0", argv[1]);
				}
			}
		}
		else
			sprintf(tmp, "file `%s' doesn't exist\0", argv[1]);
		rectf(&display, box, f1);
		string(&defont, tmp, &display, boxstart, f2);

	}

	for(;; r=wait(MOUSE|RCV) ) {

	  if (r&MOUSE) {
		if ( button123() ) CheckWindow();
		if ( (button1()) && (!haltflag) ) {
		/*haltflag means no communic. w. host*/
			rectf( &display, box, f1 );
			if (edgesumflag == 1)
				showEsum();
		        /* see if box was selected */
			if ( ptinrect(mouse.xy, box) ) {
				if (animflag == 1) { 
					sendchar('c');   /*command*/
					sendchar('\n');
					/*finish receiving first*/
					for (;; wait(RCV) ) {
						Animcmds();
						if ((animflag == 2) || (animflag == 0))
					/*process may end before host gets to the command*/
							break;
					}
				}
				cmds();
				if (animflag == 2) animflag = 1;
			}
		}

		if ( button3() ) {
			rectf( &display, box, f1 );
			if (edgesumflag == 1)
				showEsum();
			i = menuhit( &stringsmenu, 3 );
			for (;;wait(MOUSE)) if ( !button3() ) break;
			switch( i ) {
				case 0:
					vertex( f1 );
					break;

				case 1:
					vertex( f2 );
					break;

				case 2:
					move_vertex(); 
					break;

				case 3:
					copy(); 
					break;

				case 4:	
					edge( f1 );
					break;

				case 5:
					edge( f2 );
					break;

				case 6:
					labs();
					break;

				case 7:
					graphicsmenu();
					break;

				case 8:
					macros();
					break;

				case 9:
					reshape();
					break;

				case 10:
					grid();
					break;

				case 11:
					restart();
					break;

				case 12:
					stop();
					break;

			}
		}

		if ((button2()) && (animflag == 1) && (!haltflag)) {
			rectf( &display, box, f1 );
			j = menuhit( &animmenu, 2 );
			for (;;wait(MOUSE)) if ( !button2() ) break;
			switch( j ) {
				case 0:
					sendchar('h');  /*halt*/
					sendchar('\n');
					animmenutext[0] = "resume";
					haltflag = 1;
					/*can't rcv when haltflag is 1*/
					rectf(&display, box, f1);
					strcpy(msg, Amsg);
					strcat(msg, "; manual pause");
					string(&defont, msg, &display, boxstart, f2);
					break;

				case 1:
					/*this is signal to host to close process being animated*/
					sendchar('k');   /*kill*/
					sendchar('\n');
					/*finish receiving first*/
					for (;; wait(RCV) ) {
						Animcmds();
						if (animflag == 0) break;
					}
					rectf(&display, box, f1);
					strcpy(msg, Amsg);
					strcat(msg, "; program killed");
					string(&defont, msg, &display, boxstart, f2);
					break;

				case 2:
					sendchar('c');   /*command*/
					sendchar('\n');
					/*finish receiving first*/
					for (;; wait(RCV) ) {
						Animcmds();
						if ((animflag == 2) || (animflag == 0))
					/*process may end before host gets to the command*/
							break;
					}
					cmds();
					if (animflag == 2) animflag = 1;
					break;

			}  /*switch*/
		}  /*if button2 && animflag && !haltflag*/

		if ((button2()) && (animflag == 1) && (haltflag)) {
			animmenutext[2] = "~~~~";
			rectf( &display, box, f1 );
			j = menuhit( &animmenu, 2 );
			for (;;wait(MOUSE)) if ( !button2() ) break;
			switch( j ) {
				case 0:
					sendchar('r');  /*resume*/
					sendchar('\n');
					animmenutext[0] = "pause";
					haltflag = 0;
					rectf(&display, box, f1);
					strcpy(msg, Amsg);
					strcat(msg, "; program resumed");
					string(&defont, msg, &display, boxstart, f2);
					break;

				case 1:
					sendchar('k');   /*kill*/
					sendchar('\n');
					animmenutext[0] = "pause";
					haltflag = 0;
					/*finish receiving first*/
					for (;; wait(RCV) ) {
						Animcmds();
						if (animflag == 0) break;
					}
					rectf(&display, box, f1);
					strcpy(msg, Amsg);
					strcat(msg, "; program killed");
					string(&defont, msg, &display, boxstart, f2);
					break;


			}  /*switch*/
			animmenutext[2] = "type command";
		}  /*if button2 && animflag && haltflag*/

	  }  /* if r&MOUSE */

	  /* NOTE: was (r&RCV) but that didn't work for ed */
	  if ((own()&RCV) && (animflag == 1) && (!haltflag)) {
		Animcmds();
	  } /* if r&RCV */

	} /* for ..wait M|R */
}

CheckWindow()
{
	if (P->state & MOVED) {
		Set_Up_Space();
		restart_now();
		P->state &= ~MOVED;
		P->state &= ~RESHAPED;
		}

	if (P->state & RESHAPED) {
		Set_Up_Space();
		restart_now();
		menutext[9] = "standard window";
		P->state &= ~RESHAPED;
		}
}
