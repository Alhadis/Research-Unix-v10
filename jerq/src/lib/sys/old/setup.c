#define y_line_to_pt(val)  ((val)*NS + YMARGIN)

#define SETUP_OWN	1	/* owner of setup.h definitions */

/* entrance to this routine happens when keyboard interrupt handler sees
	a setup key.   */
#include <jerq.h>
#include <font.h>
#include <kbd.h>
#include <setup.h>
extern Point cur;			/* current cursor position */
extern int kbdstatus;
extern Point pt();		/* converts from row/col to Point */
extern aciainit();

int	Pf_edit;	/* flag 1= editing pf key 0= in setup options mode */
Word setup_space[XMAX/(8*sizeof(Word))][NS*2];	/* space for setup to 	*/
						/* screenswap to 	*/
Bitmap setup_swap = { setup_space, XMAX/(8*sizeof(Word)), 
			{{(short) 0, (short) 0},{(short) XMAX,(short) NS*2}},
			(char *) 0};

setupdisplay()	/* draws the initial setup display, and the current values */
{
	Point setupcur;		/* save value of terminal cursor during setup */
	int tmpc;		/* temp char value : (int to avoid sgs bug) */
	int cursindx;		/* index into cursor position array */

	/* the setup routines assume a routine called kgetc(), which is */
	/* equivalent to getc() or kbdchar(), except that it comes straight */
	/* from the keyboard DUART, not through the queues */

	checkbram();		/* check bram checksum, if bad, set all 0s */
	setupcur = cur;		/* save cursor away */
	screenswap(&setup_swap,setup_swap.rect,
			Rect(0,y_line_to_pt(ymax+1),XMAX,y_line_to_pt(ymax+3)));

reset_setup:
	clear (Rect(0, ymax+1, xmax+1, ymax+3));	/* might want to use */
							/* a graypat instead */

			/* NOTE: in layers mode, must do a "top" of the */
			/* physical setup layer first			*/
		/* HMM... TOM, I just realized we really do want to allow */
		/* interrupts while doing this stuff... otherwise, suppose */
		/* I hit setup during download,  the terminal will never get */
		/* out of download mode if I take too long setting up */
		/* are there other UGH problems if the download completes */
		/* while still in setup mode???				*/
		
	Pf_edit =  cur.x = 0;
	cur.y = ymax+1;
	string (&defont, SETUP_HEADINGS, &display, pt(cur), F_STORE);
	cur.y++;

	for (cursindx = BAUD; cursindx < NUMOPTS; cursindx++)
	{
		string (&defont, val_indx[cursindx][BRAM->opts.opt_array[cursindx].byte],
					&display, pt(cur), F_STORE);
		cur.x = curpos[cursindx+1];
	}

	cap_setup();
	num_setup();
	cur.x = 0;
	cursindx = 0;
	curse();
	while((tmpc = kgetc()) != SETUPKEY)	/* 0xae */
	{
		switch (tmpc) {
		case TABKEY: 
			curse();	/* erase old cursor */
			cur.x = curpos[(cursindx + 1 >= NUMOPTS ? cursindx : ++cursindx)];
			curse();
			break;
		case BACKSPCKEY:
			curse();	/* erase old cursor */
			cur.x = curpos [(cursindx - 1 < 0 ? 0 : --cursindx)];
			curse();
			break;
		case SPACEKEY:
			curse();	/* erase old cursor */
			if (++BRAM->opts.opt_array[cursindx].byte >= val_maxes[cursindx])
				BRAM->opts.opt_array[cursindx].byte = 0;
			string (&defont, val_indx[cursindx][BRAM->opts.opt_array[cursindx].byte], &display, pt(cur), F_STORE);
			switch (cursindx){
			case BAUD:
				aciainit(baud_speeds[VALBAUD]);
				break;
			case RETKEY:
				/* This is examined in vitty.c:main */
			case NEWLINE:
				/* This is examined in vitty.c:introutine */
			case DUPLEX:
				/* This is examined in vitty.c:introutine */
				break;
			case KEYTONE:
				if (VALKEYTONE)
					kbdstatus &= ~TTY_CHIRP;
				else
					kbdstatus |= TTY_CHIRP;
				DUART->b_data = kbdstatus;
				break;
			case SCREENCOLOR:
				if (VALSCREENCOLOR)
					BonW();
				else
					WonB();
				break;
			default:
				/* Illegal value, force cursindx to 0 */
				cursindx = 0;
				break;
			}
			curse();	/* restore cursor */
			break;
		case FUNC1KEY: case FUNC2KEY: case FUNC3KEY: case FUNC4KEY:
		case FUNC5KEY: case FUNC6KEY: case FUNC7KEY: case FUNC8KEY: 
			Pf_edit = 1;
			if (pfkey(tmpc - FUNC1KEY + 1) != SETUPKEY)
				goto reset_setup;
			else	/* exit of pfkey mode with setup means quit */
				goto exit_setup;
		default:
			/* ignore */
			break;
		}
	}
exit_setup:		/* can jump here from pfkey exit via setupmode key */

		/* restore saved screen values */
	screenswap(&setup_swap,setup_swap.rect,Rect(0,y_line_to_pt(ymax+1),XMAX,y_line_to_pt(ymax+3)));
	/*
	* fix caps lock label 
	*/
	caps_msg();
	num_msg();
	cur = setupcur;	/* restore cursor value */
	setbram();	/* reset checksum value in BRAM */
}

caps_msg()
{
	Point ncur;
	int mode;

	if(VALDWNLDFLAG)
		return; /* a program is downloaded so don't show the message */
	mode = (VALCAPS ? F_STORE : F_CLR);
	ncur.x = 25;
	ncur.y = ymax+1;
	string(&defont, "caps", &display, pt(ncur), mode);
	ncur.y = ymax+2;
	string(&defont, "lock", &display, pt(ncur), mode);
}
num_msg()
{
	Point ncur;
	int mode;

	if(VALDWNLDFLAG)
		return;	/* a program is downloaded so don't show message */
	mode = (VALNUM ? F_STORE : F_CLR);
	ncur.x = 44;
	ncur.y = ymax+1;
	string(&defont, "num", &display, pt(ncur), mode);
	ncur.y = ymax+2;
	string(&defont, "lock", &display, pt(ncur), mode);
}

cap_setup()
{
	Point ncur;

	if(Pf_edit)
		return;
	ncur.x = 64;
	ncur.y = ymax+2;
	string(&defont,"CAPS LOCK",&display,pt(ncur),(VALCAPS ? F_STORE : F_CLR));
}

num_setup()
{
	Point ncur;
	
	if(Pf_edit)
		return;
	ncur.x = 77;
	ncur.y = ymax + 2;
	string(&defont,"NUM LOCK", &display, pt(ncur), (VALNUM ? F_STORE : F_CLR));
}
