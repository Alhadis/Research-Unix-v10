#include "defs.h"
#include <font.h>

struct list list[MXPTS];
struct set set[MXSETS];
Rectangle Dspace, box, Lrect, Rrect;
Point boxpt, boxstart;

char *menutext[11] = 
	{"create vertex",
	 "delete",
	 "move",
	 "copy",
	 "create edge(s)",
	 "delete edge(s)",
	 "labels menu",
	 "macros menu",
	 "restart",
	 "exit", 
	 0};
Menu stringsmenu={menutext};

char *animmenutext[5] = 
	{"halt",
	 "resume",
	 "kill",
	 "type command",
	 0};
Menu animmenu={animmenutext};

main( argc, argv ) 
int argc;
char *argv[]; {

	int r, i1, i2, x1, y1, t, haltflag;  /* var's for anim ops */

	int i, j, goodfn, goodfile, mkint();
	char tmp[MEDSTR], chstr[5], mkstr(), mkline(), code[5], label[30], msg[80];
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
	boxwidth = 20;  /* y dist of box */

/* Dspace is Drect minus the box; the display space */
	Dspace.origin = Drect.origin;    
	Dspace.corner.x = Drect.corner.x;
	Dspace.corner.y = Drect.corner.y - boxwidth;

/* define left half & right half of screen */
	Lrect.origin = Dspace.origin;
	Lrect.corner.x = (Dspace.origin.x + Dspace.corner.x)/2;
	Lrect.corner.y = Dspace.corner.y;

	Rrect.origin.x = (Dspace.origin.x + Dspace.corner.x)/2;
	Rrect.origin.y = Dspace.origin.y;
	Rrect.corner = Dspace.corner;


/* define box dimensions */
	box.origin.x = Drect.origin.x;
	box.origin.y = Drect.corner.y - boxwidth;
	box.corner.x = Drect.corner.x;
	box.corner.y = Drect.corner.y;

/* blank goes in this space in box to show box chosen */
	boxstart.x = box.origin.x;
	boxstart.y = box.corner.y - 17;

/* first char goes here on line in box */
	boxpt.x = boxstart.x + 15;
	boxpt.y = boxstart.y;  

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
					macros();
					break;

				case 8:
					restart();
					break;

				case 9:
					stop();
					break;

			}
		}

		if ((button2()) && (animflag == 1)) {
			rectf( &display, box, f1 );
			j = menuhit( &animmenu, 2 );
			for (;;wait(MOUSE)) if ( !button2() ) break;
			switch( j ) {
				case 0:
					sendchar('h');  /*halt*/
					sendchar('\n');
					haltflag = 1;
				/*can't rcv when haltflag is 1*/
					rectf(&display, box, f1);
					string(&defont, "program halted", &display, boxstart, f2);
					break;

				case 1:
					sendchar('r');  /*resume*/
					sendchar('\n');
					haltflag = 0;
					rectf(&display, box, f1);
					string(&defont, "program resumed", &display, boxstart, f2);
					break;

				case 2:
					if (haltflag) {
						rectf(&display, box, f1);
						string(&defont, "program halted: must resume to kill", &display, boxstart, f2);
						break;
					}
					/*this is signal to host to close process being animated*/
					sendchar('k');   /*kill*/
					sendchar('\n');
					/*finish receiving first*/
					for (;; wait(RCV) ) {
						Animcmds();
						if (animflag == 0) break;
					}
					rectf(&display, box, f1);
					string(&defont, "program dead", &display, boxstart, f2);
					break;

				case 3:
					if (haltflag) {
						rectf(&display, box, f1);
						string(&defont, "program halted: must resume to use commands", &display, boxstart, f2);
						break;
					}
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
		}  /*if button2 && animflag*/

	  }  /* if r&MOUSE */

	  /* NOTE: was (r&RCV) but that didn't work for ed */
	  if ((own()&RCV) && (animflag == 1) && (!haltflag)) {
		strcpy(chstr, mkstr());
		switch(chstr[0]) {
			case 'a':  /*vc: create vertex*/
				x1 = mkint();
				y1 = mkint();
				vc( Ascale(x1, y1) );
				break;

			case 'b':  /*vd: delete vertex*/
				i1 = mkint();
				if (list[i1].status != 1)  /* point doesn't exist */
					break;
				vd(i1);
				break;

			case 'c':  /*vm: move vertex*/
				i1 = mkint();
				x1 = mkint();
				y1 = mkint();
				if (list[i1].status != 1)  /* point doesn't exist */
					break;
				vm(i1, Ascale(x1, y1));
				break;

			case 'd': /*vp: copy vertex*/
				i1 = mkint();
				x1 = mkint();
				y1 = mkint();
				if (list[i1].status != 1)  /* point doesn't exist */
					break;
				vp(i1, Ascale(x1, y1));
				break;

			case 'e':  /*vg: change vertex graphics*/
				i1 = mkint();
				strcpy(code, mkstr());
				if (list[i1].status != 1)  /* point doesn't exist */
					break;
				vg(i1, code);
				break;

			case 'f':  /*vl: label vertex*/
				i1 = mkint();
				strcpy(label, mkstr());
				if (list[i1].status != 1)  /* point doesn't exist */
					break;
				vl(i1, label);
				break;

			case 'g':  /*vs: show vertex labels*/
				showvertlabels();  /* to show */
				break;

			case 'h':  /*vh: hide vertex labels*/
				showvertlabels();  /* to hide */
				break;

			case 'i':  /*vn: number vertices*/
				nmbrverts();
				break;

			case 'j':  /*ec: create edge*/
				i1 = mkint();
				i2 = mkint();
				if ((list[i1].status != 1) || (list[i2].status != 1))
					/* point doesn't exist */
					break;
				ecd(f1, i1, i2);
				break;

			case 'k':  /*ed: delete edge*/
				i1 = mkint();
				i2 = mkint();
				if ((list[i1].status != 1) || (list[i2].status != 1))
					break;
				ecd(f2, i1, i2);
				break;

			case 'l':  /*eg: change edge graphics*/
				i1 = mkint();
				i2 = mkint();
				strcpy(code, mkstr());
				if ((list[i1].status != 1) || (list[i2].status != 1))
					break;
				eg(i1, i2, code);
				break;

			case 'm':  /*el: label edge*/
				i1 = mkint();
				i2 = mkint();
				strcpy(label, mkstr());
				if ((list[i1].status != 1) || (list[i2].status != 1))
					break;
				el(i1, i2, label);
				break;

			case 'n':  /*es: show edge labels*/
				showedgelabels();  /* to show */
				break;

			case 'o':  /*eh: hide edge labels*/
				showedgelabels();  /* to hide */
				break;

			case 'p':   /*ee: euclidean edge labels*/
				euclabs();
				break;

			case 'q':   /*pd: program delay*/
				t = mkint();
				sleep(t);  /* delay for time t */
				break;

			case 'r':   /*ps: program stop*/
				for (;; wait(MOUSE))
					if (button2())  /* press b2 to continue */
						break;
				break;

			case 's':   /*pm: program message*/
				strcpy(msg, mkline());
				rectf(&display, box, f1);
				string(&defont, msg, &display, boxstart, f2);
				break;

			case 't':   /*pr: program restart*/
				bitblt( &display, Dspace, &display, Drect.origin, f3 ); 
				rectf( &display, box, f1 );
				freenodes();
				init();
				break;

			case 'u':   /*pc: program command*/
				rectf(&display, box, f1);
				string(&defont, "type command\0", &display, boxstart, f2);
				cmds();
				break;

			case 'z':   /*z ==> done running program*/
				rectf(&display, box, f1);
				string(&defont, "ran program\0", &display, boxstart, f2);
				animflag = 0;
				break;

		}  /* switch */
	  } /* if r&RCV */

	} /* for ..wait M|R */
}
