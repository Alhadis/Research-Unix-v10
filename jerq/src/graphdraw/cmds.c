#include "defs.h"
#include <font.h>
extern char *labsmenutext[];

/* read & exec commands typed on command line */
cmds() {
	extern Rectangle Lrect, Rrect;
	int r, goodline,goodfn, goodfile, mkint(), i, j, k, lastind;
	char tmp[MEDSTR], cmd, fname[MXSTR], str[MXLINE];   


	rectf(&display, box, f1);
	string( &defont, " \0", &display, boxstart, f1 );

 	for (;;r=wait(KBD|MOUSE)) {

		if (r&MOUSE) {
			if (button1() && !ptinrect(mouse.xy, box)) {
				if ((animflag == 1) || (animflag == 2)) {
				/*tell host no cmd coming*/
					sendchar('0');
					sendchar('\n');
				}
				return;
			}
			if (button3()) {
				if ((animflag == 1) || (animflag == 2)) {
				/*tell host no cmd coming*/
					sendchar('0');
					sendchar('\n');
				}
				return;
			}
		}

		if (r&KBD) {
			rectf(&display, box, f1);
			string( &defont, " \0", &display, boxstart, f1 );
	
			strcpy( str, getkbdstr() );
			if ( strcmp(str, "-1\0") == 0 ) { 
				if ((animflag == 1) || (animflag == 2)) {
				/*tell host no cmd coming*/
					sendchar('0');
					sendchar('\n');
				}
				return;
			}


			goodline = 0;
			i = 0;
			while (str[i] == ' ') i++;  /* get to first letter */
			
			switch(str[i]) {
				case 'q':
					if ((animflag == 1) || (animflag == 2)) {
					/*tell host cmd is coming*/
						sendchar('1');
						sendchar('\n');
					}
					sendchar('q');
					break;

				case '!':
				case 'r':
				case 'w':
					cmd = str[i];
					i++;
					if ((str[i] != ' ') && (str[i] != '\0')) {
						sprintf(tmp, "no such command: `%s'\0", str);
						break;
					}
					while (str[i] == ' ') /*get to  next char */
						i++;
					if ( str[i] == '\0')  /* end of string */
						sprintf(tmp, "need a filename\0");
					else {
						j = 0;
						/* extract fname from str */
						while ( (fname[j++] = str[i++]) != '\0' )  ;
						fname[j] = '\0'; /* to end string */

						if ((animflag == 1) || (animflag == 2)) {
						/*tell host cmd is coming*/
							sendchar('1');
							sendchar('\n');
						}

						sendchar(cmd);
						sendnchars( strlen(fname), fname  );  
						sendchar('\n'); /* to terminate string */
						goodline = 1; /* valid cmd + a fn */
					}
					break;

				case 'c':  /* for cd .. change directory */
					cmd = str[i];
					if (str[++i] != 'd')
						break;
					i++;
					if ((str[i] != ' ') && (str[i] != '\0')) {
						sprintf(tmp, "no such command: `%s'\0", str);
						break;
					}
					while (str[i] == ' ') /*get to  next char */
						i++;
					if ( str[i] == '\0')  /* end of string */
						sprintf(tmp, "need a directory name\0");
					else {
						j = 0;
						/* extract dir from str */
						while ( (fname[j++] = str[i++]) != '\0' )  ;
						fname[j] = '\0'; /* to end string */

						if ((animflag == 1) || (animflag == 2)) {
						/*tell host cmd is coming*/
							sendchar('1');
							sendchar('\n');
						}

						sendchar(cmd);
						sendnchars( strlen(fname), fname  );  
						sendchar('\n'); /* to terminate string */
						goodline = 1; /* valid cmd + a fn */
					}
					break;

					

				default:

					if ((animflag == 1) || (animflag == 2)) {
					/*tell host no cmd is coming*/
						sendchar('0');
						sendchar('\n');
					}


					sprintf(tmp, "no such command: `%s'\0", str);
					break;
			}
						

				
		if (goodline) {
			if ( goodfn = mkint() ) {
			/* host sends "1" if file can be opened, "0" if not */

				switch( cmd ) {

				case 'c':
					
					sprintf(tmp, "directory changed to `%s'\0", fname);
					break;

				case '!':
					/* clear list & screen */
					string(&defont, "running program...", &display, boxstart, f2); 
					sprintf(tmp, "running program `%s'\0", fname);
					animflag = 1;
					sprintf(Amsg, ""); /*clear Amsg*/
					break;

				case 'w':
					string(&defont, "writing file...", &display, boxstart, f2); 
					if ((animflag != 1) && (animflag != 2))
					/*don't pack list of animated algorithm*/
						packlist();  /* fill in holes */
					sendlist();  	
						/* send list to host in file form */
					sprintf(tmp, "wrote file `%s'\0", fname);
					break;
		

				case 'r':
					if ( ((goodfile = mkint()) != 0) && ((goodfile + numverts) < 501 ) ) {
					/*file is in correct format & room for file*/
						sendchar('1');
						sendchar('\n');
						string(&defont, "reading file...", &display, boxstart, f2);
						rectf( &display, Dspace, f3);
						if (last != 1) {
							scale(Lrect, 1);
							displaylist(1);
							for (k = 1; k < lastset; k++) {
								tightsetrect(k);
								rectf(&display,set[k].Srect, f2);
							}
						}
						lastind = last;
						rcvfile(); 
					/*host sends file; blit puts in adj list*/
						if (lastind == 1) {
							scale(Dspace, 1);
							displaylist(1);
						}
						else {
							scale(Rrect, lastind); 
							displaylist(lastind);
						}
					/* draw list on screen in graph form */
						sprintf(tmp, "read file `%s'\0", fname);
					}
					else {
						if (!goodfile)	
							sprintf(tmp, "file `%s' is in wrong format\0", fname);
						else {
							sendchar('0');
							sendchar('\n');
							sprintf(tmp, "not enough room for file `%s': max is 500 vertices\0", fname);
						}
					}
					break;
					}
				}
				else {   /* goodfn == 0 */
					switch(cmd) {
					case 'r':
						sprintf(tmp, "file `%s' doesn't exist\0", fname);
						break;

					case 'w':
						sprintf(tmp, "can't open file `%s'\0", fname);
						break;

					case '!':
						sprintf(tmp, "`%s'\0", fname);
						break;

					case 'c':
						sprintf(tmp, "no such directory: `%s'\0", fname);
						break;
					}
				}
			}

			rectf(&display, box, f1);
			string(&defont, tmp, &display, boxstart, f2);
			if ((animflag == 1) || (animflag == 2))  /* if cmd is '!' */
			/* 2 means command option from animmenu was chosen*/
				return;

		}
	}
}
