#include <jerq.h>
#include <kbd.h>
#include <font.h>
#include <setup.h>
#include <pf.h>

#define xmax ((XMAX-2*XMARGIN)/CW-1)
#define ymax ((YMAX-2*YMARGIN)/NS-3)

extern Point pt();
extern Point cur;
extern Rectangle rect();
extern Rectangle Rect();

pfkey(keynum)
int keynum;	 /* pfkey number pressed */
{
	short edit, ins_mode;
	int inchar;
	char tempstr[2];
	char pfstr[PFKEYLEN + 1];
	Point savecur;

	savecur = cur;		/* save setup's cursor */
	clear (Rect(0, ymax+1, xmax, ymax+3));	/* clear converts to points */
	rectf(&display,rect(Rect(0,ymax+2,7,ymax+3)), F_XOR);
	rectf(&display,rect(Rect(9,ymax+2,16,ymax+3)), F_XOR);
	rectf(&display,rect(Rect(18,ymax+2,25,ymax+3)), F_XOR);
	rectf(&display,rect(Rect(30,ymax+2,36,ymax+3)), F_XOR);
	rectf(&display,rect(Rect(38,ymax+2,44,ymax+3)), F_XOR);
	rectf(&display,rect(Rect(49,ymax+2,56,ymax+3)), F_XOR);
	rectf(&display,rect(Rect(58,ymax+2,65,ymax+3)), F_XOR);
	rectf(&display,rect(Rect(67,ymax+2,74,ymax+3)), F_XOR);
	cur.x=0;
	cur.y=ymax+2;
	string (&defont, PFBOT_HEADINGS, &display, pt(cur), F_XOR);


next_key:      /* label for goto (shudder) if user wants to change keys */
	tempstr[0] = keynum + '0';
	tempstr[1] = (char) 0;
	keynum--;	/* adjust for storage, which starts with 0 */

	/*   Logic...
	   draw menu to screen
	   switch/wait on chars for fkeys 
	   ins/delete as necessary... keep one string
	   on exit update BRAM
	   pfkey is null terminated
	 */
	clear (Rect(0, ymax+1, xmax, ymax+2));	/* clear converts to points */

	cur.x = 0;
	cur.y = ymax + 1;
	string (&defont, PFTOP_HEADINGS, &display, pt(cur), F_STORE);
	cur.x = 1;
	string (&defont, tempstr, &display, pt(cur), F_STORE);
	cur.x=3;
	bramgetstr ( keynum, pfstr, EDLEN(0) ); /* put pfkey into str "pfstr" */
	displaystring (pfstr ,cur);	/* print string, display unprintables */

	/* Now the initial display is set:  wait for a char */

	ins_mode = edit = 0;	   /* set to editting pf key, may implement labels later */
	cur.x = PFMINPOS(edit); /* set cursor to start of pfkey def */
	curse();
	while(((inchar = kgetc()) == -1) || (inchar == (char) 0));	/* wait until we get a valid character */
	while ((inchar  != PF_EXITKEY) && (inchar != SETUPKEY))
	{
		switch(inchar)
		{
/*	       case PF_ED_KEY:
/*		 case PF_ED_LABEL:	 /* Not implemented yet */
/*		       if (edit == inchar-PF_ED_KEY)   /* turn off current edit mode */
/*				edit = NOT_EDITTING;
/*		    else
/*		    {
/*			       if (edit != NOT_EDITTING) /* switch editmodes */
/*					curse();	/* clean up old cursor*/
/*				edit = inchar - PF_ED_KEY;
/*			      cur.x = PFMINPOS(edit);
/*			 }
/*		       curse();        /* clear/set cursor */
/*			  break;				  */
		case PF_INSKEY:
			/*			 if (edit == NOT_EDITTING)
/*		       {	       /* not legal unless editting */
/*				 ringbell();
/*			     break;
/*			  }						  */
			/* reverse highlight insert char key */
			rectf(&display,inset(rect(Rect(0,ymax+2,7,ymax+3)),1), F_XOR);
			ins_mode = ~ins_mode;
			break;
		case PF_DELKEY:
			if (strlen(pfstr) > 0)
			{
				if(strlen(pfstr) == EDPOS(edit,cur.x)) {
					/* at end of string */
					curse();
					cur.x--;
					curse();
				}
				else {
					scroll(Rect(cur.x + 1, ymax+1,
					    PFMINPOS(edit) + strlen(pfstr), ymax+2),
					    Pt(cur.x, ymax+1),ymax+2);
					strdelchar(pfstr,cur.x - PFMINPOS(edit));
					if ((cur.x >= strlen(pfstr) + PFMINPOS(edit)) &&
					    (cur.x > PFMINPOS(edit)))
						cur.x--;
					curse();	/* restore creamed cursor */
				}
			}
			else
				ringbell();
			break;
		case PF_NEXTKEY:
			bramputstr ( keynum, pfstr );	/* Store string into bram */
			keynum += 2;
			if (keynum > 8)
				keynum = 1;
			if(ins_mode) {
				/* reverse highlight insert char key */
				rectf(&display,inset(rect(Rect(0,ymax+2,7,ymax+3)),1), F_XOR);
			}
			goto next_key;	/* top of routine, load new pfkey */
		case PF_LEFTKEY:
		case 0xc4:	/* left-arrow */
			/*			  if (edit != NOT_EDITTING)  */
			if (cur.x > PFMINPOS(edit))
			{
				curse();
				cur.x--;
				curse();
			}
			break;
		case PF_RIGHTKEY:
		case 0xc3:	/* right-arrow */
			/* EDPOS returns the position within the
				   currently editted string that the cursor
				points at			     */
			/*			if (edit != NOT_EDITTING)  */
			if (strlen(pfstr) >= EDPOS(edit, cur.x + 1))
			{
				curse();
				cur.x++;
				curse();
			}
			break;
		case 0x88:	/* PF 7	*/
		case 0x89:	/* PF 8		*/
			/*
			* Ignore these keys
			*/
			break;
		default:
			if( ins_mode )
				if( strlen(pfstr) >= EDLEN(edit) ) /* Can't insert */
				{		
					ringbell(); 
					break;	
				}
				else {
					/* First make room on screen */
					curse(); /* turn off old cursor */
					scroll(Rect(cur.x, ymax+1,PFMINPOS(edit) + strlen(pfstr) + 1, ymax+2),
					    Pt(cur.x + 1, ymax+1),ymax+2);
					curse(); /* turn it back on */

					/* Then update real string */
					tempstr[0] = inchar;
					ins_str(pfstr,tempstr,cur.x - PFMINPOS(edit));
				}
			else {	/* overstrike */
				if(EDPOS(edit,cur.x) >= EDLEN(edit)) {
					/* can't insert any more */
					ringbell();
					break;
				}
				else {
					/*
					* see if we are adding characters to the end of the string
					*/
					if((strlen(pfstr)  ) == EDPOS(edit,cur.x))
						pfstr[EDPOS(edit,cur.x)+1 ] = (char) 0;
					pfstr[EDPOS(edit,cur.x)] = inchar;
					tempstr[0] = inchar;
				}
			}
			if( cur.x <= PFMAXPOS(edit) )
			{
				curse();
				displaystring(tempstr,cur);
				if(cur.x++ > PFMAXPOS(edit)) {
					ringbell();
					cur.x--;
				}
				curse();
			}
			break;
		}
		while(((inchar = kgetc()) == -1) || (inchar == (char) 0));	/* wait until we get a valid character */
	}
	bramputstr ( keynum, pfstr );	/* Store string into bram */
	cur = savecur;	/* restore cursor */
	return(inchar);
}


bramgetstr(keynum,str,maxlen)	/* get pfkey "keynum" from bram into "str" */
int keynum;
register char *str;
int maxlen;  /* max length allowed for string */
{
	register int i = 0;
	while ((*str++ = BRAM->pfkeys[keynum][i].byte) && (++i < maxlen));
	*str = (char) 0;	/* guarantee null termination if full length */
}

bramputstr(keynum,str)	/* store string "str" into pfkey "keynum" in bram */
int keynum;
char *str;
{
	register int i = 0;

	while (BRAM->pfkeys[keynum][i++].byte = (unsigned char) (*str++));
}

displaystring(str,cur)	     /* print the real string with screen printable chars */
char *str;
Point cur;
{
	register char tmpc;
	char overstrike[2];	/* string holding overstrike char */
	char printstr[2];

	overstrike[0] = overstrike[1] =  printstr[1] = (char) 0;

	str--;		/* prepare for the loop */
	while ((tmpc = *++str) != (char) NULL)
	{
		printstr[0] = tmpc;	/* printable char */
		string(&defont, printstr, &display, pt(cur), F_STORE);
		cur.x++;
	}
}

extern int kbdstatus;
ringbell()
{
	kbdstatus |= TTY_ALARM;
	DUART->b_data = kbdstatus; /* ding!!!*/
	kbdstatus &= ~TTY_ALARM;
}

strdelchar(str, pos)   /* delete char at position pos in string str */
char *str;
int pos;
{
	str = &(str[pos]);
	while (*str++ != (char) NULL)
		*(str - 1) = *str;
}

ins_str(str,instr, pos)     /* insert instr into str at position pos */
char *str, *instr;
int pos;
{
	register int i, j;

	i = strlen(str) + 1;
	j = strlen(instr);
	while ( --i >= pos )	/* make room for new string */
		str[i + j] = str[i];

	/* insert new string */
	while ( j--)
		str[++i] = *instr++;
}
