#include "defs.h"
#include <font.h> 

char *animmenutext[4];
Menu animmenu;

Animcmds() {
	int r, i1, i2, x1, y1, t;  /* var's for anim ops */

	int i, j, mkint();
	char tmp[MEDSTR], chstr[5], mkstr(), mkline(), code[5], label[30], msg[80];
	Point Ascale();

	strcpy(chstr, mkstr());
	switch(chstr[0]) {
		case '2':
		/*msg from host: ready to read cmd*/
			animflag = 2;
			return;
			break;

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
			rectf(&display, box, f1);
			strcpy(msg, Amsg);
			strcat(msg, "; programmed pause");
			string(&defont, msg, &display, boxstart, f2);
			animmenutext[0] = "resume";
			for (;; wait(MOUSE)) {
				/* press b2 to continue */
				if (button2()) {
				rectf( &display, box, f1 );
				j = menuhit( &animmenu, 2 );
				for (;;wait(MOUSE)) if ( !button2() ) break;
				switch( j ) {
				case 0:
					sendchar('r');  /*resume*/
					sendchar('\n');
					rectf(&display, box, f1);
					strcpy(msg, Amsg);
					strcat(msg, "; program resumed");
					string(&defont, msg, &display, boxstart, f2);
					break;

				case 1:
					/*this is signal to host to close process being animated*/
					sendchar('k');   /*kill*/
					sendchar('\n');
					rectf(&display, box, f1);
					strcpy(msg, Amsg);
					strcat(msg, "; program killed");
					string(&defont, msg, &display, boxstart, f2);
					break;

				case 2:
					sendchar('c');   /*command*/
					sendchar('\n');
					cmds();
					break;
				}
				break; /*out of for loop?*/
				}
			}
			animmenutext[0] = "pause";
			break;

		case 's':   /*pm: program message*/
			strcpy(Amsg, mkline());
			rectf(&display, box, f1);
			string(&defont, Amsg, &display, boxstart, f2);
			break;

		case 't':   /*pr: program restart*/
			restart_now();
			break;

		case 'u':   /*pc: program command*/
			cmds();
			break;

		case 'v':   /*pw: program window scale*/
			i1 = mkint();
			Ascale(-1, i1);
			break;

		case 'z':   /*z ==> done running program*/
			rectf(&display, box, f1);
			strcpy(msg, Amsg);
			strcat(msg, "; ran program");
			string(&defont, msg, &display, boxstart, f2);
			animflag = 0;
			break;
		}
	return;
}
