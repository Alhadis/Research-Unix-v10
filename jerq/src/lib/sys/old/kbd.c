#include <jerq.h>
#include "queue.h"
#include <kbd.h>
#include <setup.h>

#define ESC	'\033'
#define ESCB	0200
#define DEL	'\177'
#define K_KEY	0
#define K_CHAR	1

/*
 * Note that the keypad keys send what they say.
 * the arrows send 'escape [' followed by A, B, C, D, H, except for HOME DOWN,
 * which sends (ugh) esc [ 70;0H.  Clear sends esc[0J,
 * Break, disconnect, and the pf keys send codes 0x80 through 0x89 respectively.
 * If you want to tell the keypad numbers from the real ones, work in raw.
 * The keytab table has been drastically extended for the TTYKBD and
 * a binary search is now used. Why could'nt they generate sane codes!!
 * WARNING, the keytab entries must be sorted into ascending order!
 * For any ordinary "key", char c is sent.  The arrow keys or 0x80 with
 * the ascii letter needed to send, except for HOME DOWN.  HOME DOWN
 * and Clear are done by 0x8a and 0x8b, respectively, and break,
 * disconnect and pfkeys are entered as shown above.
 */

unsigned char keytab[][2] = {
	0x8f, 0x81,	 	/* shifted key 42, discon  */
	0x90, ESCB | 'B', 	/* shifted key 109, down arrow */
	0x91, ESCB | 'H', 	/* shifted key 97, left-up arrow */
	0x92, ESCB | 'A', 	/* shifted key 98, up arrow */
	0x93, ESCB | 0xa, 	/* shifted key 99, home down arrow */
	0x94, '4',	 	/* shifted key 79, 4 */
	0x95, '5', 	/* shifted key 80, 5 */
	0x96, '6', 	/* shifted key 81, 6 */
	0x97, '7', 	/* shifted key 60, 7 */
	0x98, '8', 	/* shifted key 61, 8 */
	0x99, '9', 	/* shifted key 62, 9 */
	0x9a, ESCB | 'D',  		/* shifted key 108, left arrow */
	0x9b, ESCB | 'C', 		/* shifted key 110, right arrow */
	0xaf, 0x80, 	/* unshifted key 42, break */
	0xb0, '\0', 	/* control key 35, ignored */
	0xb1, '\0', 	/* control key 26, ignored */
	0xb3, '\0', 	/* control key 28, ignored */
	0xb4, '\0', 	/* control key 29, ignored */
	0xb5, '\0', 	/* control key 30, ignored */
	0xb6, '\012', 	/* unshifted key 96, line-feed */
	0xb7, '\0',  	/* control key 32, ignored */
	0xb8, '\0',  	/* control key 33, ignored */
	0xb9, '\0',  	/* control key 34, ignored */
	0xba, '\0',  	/* control key 27, ignored */
	0xbb, '\0',  	/* control key 75, ignored */
	0xbc, '\0', 	/* control key 31, ignored */
	0xbd, '\0', 	/* control key 37, ignored */
	0xbe, '\0', 	/* control key 59, ignored */
	0xbf, '\0', 	/* control key 36, ignored */
	0xc0, ESCB | 'H', 	/* unshifted key 97, left-up arrow */
	0xc1, ESCB | 'A', 	/* unshifted key 98, up arrow */
	0xc2, ESCB | 'B', 	/* unshifted key 109, down arrow */
	0xc3, ESCB | 'C', 	/* unshifted key 110, right arrow */
	0xc4, ESCB | 'D',  	/* unshifted key 108, left arrow */
	0xc6, ESCB | 0xa,  	/* unshifted key 99, home down arrow */
	0xc7, '\r', 		/* shifted key 77, return */
	0xc8, ESCB | 2, 	/* shifted key 3, f1 */
	0xc9, ESCB | 3, 	/* shifted key 4, f2 */
	0xca, ESCB | 4, 	/* shifted key 5, f3 */
	0xcb, ESCB | 5, 	/* shifted key 8, f4 */
	0xcc, ESCB | 6, 	/* shifted key 9, f5 */
	0xcd, ESCB | 7, 	/* shifted key 12, f6 */
	0xce, ESCB | 8, 	/* shifted key 13, f7 */
	0xcf, ESCB | 9, 	/* shifted key 14, f8 */
	0xd0, '\t', 	/* unshifted key 45, tab */
	0xd1, '\b', 	/* shifted key 38, bs */
	0xd2, '7', 	/* unshifted key 60, 7 */
	0xd3, '4', 	/* unshifted key 79, 4 */
	0xd4, '8', 	/* unshifted key 61, 8 */
	0xd5, '5', 	/* unshifted key 80, 5 */
	0xd6, '9', 	/* unshifted key 62, 9 */
	0xd7, '6', 	/* unshifted key 81, 6 */
	0xde, DEL, 	/* shifted key 39, del */
	0xe2, '\0', 	/* control key 76, */
	0xe3, ESC,  	/* unshifted key 25, esc */
	0xe5, ESCB | 0xb,  	/* unshifted key 40, clr */
	0xe7, '\r', 	/* unshifted key 77, return */
	0xe8, ESCB | 2, 	/* unshifted key 3, f1 */
	0xe9, ESCB | 3, 	/* unshifted key 4, f2 */
	0xea, ESCB | 4, 	/* unshifted key 5, f3 */
	0xeb, ESCB | 5, 	/* unshifted key 8, f4 */
	0xec, ESCB | 6, 	/* unshifted key 9, f5 */
	0xed, ESCB | 7, 	/* unshifted key 12, f6 */
	0xee, ESCB | 8, 	/* unshifted key 13, f7 */
	0xef, ESCB | 9, 	/* unshifted key 14, f8 */
	0xf0, '\t', 	/* shifted key 45, tab */
	0xf1, '\b', 	/* unshifted key 38, bs */
	0xf2, '\0',  	/* shifted key 107, num-lock */
	0xf3, ESC,  	/* shifted key 25, esc */
	0xf5, ESCB | 0xc,  	/* shifted key 40, local clear functin */
	0xf6, '\n', 	/* shifted key 96, line-feed */
	0xfe, DEL,  	/* unshifted key 39, del */
};


int	kbdrepeat, rptcount, kbdstatus, dispstatus;

	/* lookup table for translating when keyboard uses numeric lock */
unsigned char	numlocktab[7] = {'1','2','0','.','-',0xc5,'3'};

kbdchar()
{
	return qgetc(&KBDQUEUE);
}


kbdinit()
{
	/* init	the keyboard */
	DUART->b_cmnd = RESET_RECV | DIS_TX | DIS_RX;
	DUART->b_cmnd = RESET_TRANS;
	DUART->b_cmnd = RESET_ERR;
	DUART->b_cmnd = RESET_MR;
	DUART->mr1_2b = CHAR_ERR | PAR_ENB | EVN_PAR | CBITS8;
	DUART->mr1_2b = NRML_MOD | ONEP000SB;
	DUART->b_sr_csr	 = BD4800BPS;
	DUART->b_cmnd = RESET_MR | ENB_TX | ENB_RX;
	DUART->scc_ropbc = 0x08; /* set	output pins for kbd tx port*/
	/* turn chirps on/off depending on BRAM */

	if (VALKEYTONE)
		kbdstatus = 0;	/* no chirp */
	else
		kbdstatus = TTY_CHIRP;	/* chirp, chirp */
	DUART->b_data = kbdstatus | 0x02; /* request status */
}

auto2()
{
	register i;
	register start, end;
	Point	setupcur;
	char	s;
	char	c;
	extern int blocked, ublocked;

	/* don't actually set the repeat bit until the character after the control code */
	
	s = DUART->b_sr_csr;
	c = DUART->b_data;

	if (s & (FRM_ERR | OVR_RUN)) 
		return; /* framing error or overrun error */

	if (s & PAR_ERR) {
		/* parity error indicates this is a control word */
		/* see what state caps lock is in 	*/
		checkbram();
		VALCAPS = (c & 0x4) ? 0 : 1; /* set the caps lock flag */
		caps_msg();
		setbram();	/* must adjust checksum */
		if (c & 0x10) {
			/* turn repeat off */
			kbdrepeat = 0;
			rptcount = 0;
		}
		else { /* turn repeat on, the next character is to be repeated */
			kbdrepeat = RPTON;
		}
		return;
	}

	rptcount = 0;	/* new charcter so restart repeat timer	*/
	if (c & 0x80) {
		switch (c & 0xff) {
		case 0xb2:	/* numeric lock toggle */
		case 0xf2:
			if(kbdrepeat & RPTON) {
				kbdrepeat = 0; /* don't want to repeat */
				return;
			}
			checkbram();
			VALNUM = 1 - VALNUM;
			setbram();
			num_msg();
			return;
		case 0x8e:	/* un/shited key 41, selftest */
			test32(1);
			asm("  JMP reboot");
		case 0xae:	/* setup */
			if(kbdrepeat & RPTON) {
				kbdrepeat = 0; /* don't want to repeat */
				return;
			}
			if(!ublocked && !blocked && !VALDWNLDFLAG)
				sendchar(0x13);		/* ^S */
			setupdisplay();
			if(!ublocked && !blocked && !VALDWNLDFLAG)
				sendchar(0x11);		/* ^Q */
			return;
		}
		if ((VALNUM) && (c >= 0xc0) && (c <= 0xc6)) {
				/* what fun: software lookup of numeric */
				/* locked values... why burden the hardware */
			c = numlocktab[c - 0xc0];
			qputc(&KBDQUEUE, (int)c);
			if((kbdrepeat & RPTMASK) == RPTON) {
				kbdrepeat = RPTHAVECHR | RPTON;
				kbdrepeat += (c & 0xff);
			}
			return;
		}

		i = keytabsearch(c);
		/*
 *	here either keytab[i][K_KEY] == c, or c is not in the table
 *	If c is not in the table, there is an error somewhere, so ring the bell
 */
		if (c == keytab[i][K_KEY]) {
			if(((c = (keytab[i][K_CHAR]) & 0xff) >= 0x80) && (c <= 0x89)) /* pfkey, break, disconnect, clear */
			{
				if(kbdrepeat & RPTON) {
					kbdrepeat = 0; /* don't want to repeat */
					return;
				}
				qputc( &KBDQUEUE, c );
				return;
			}
			kchkchar(c);
			if ((kbdrepeat & RPTMASK) == RPTON) {
				/* this is it! */
				kbdrepeat = RPTHAVECHR | RPTON | RPTLOOKUP;
				kbdrepeat += i;
			}
		}
		else {
			ringbell();
		}
	}
	else {
		qputc(&KBDQUEUE, (int)c);
		if((kbdrepeat & RPTMASK) == RPTON) {
			kbdrepeat = RPTHAVECHR | RPTON;
			kbdrepeat += (c & 0xff);
		}
	}
}



kchkchar(c)
register char c;
{
	switch (c & 0xff) {
	case 0x8a:	/* HOME DOWN	*/
		qputstr( &KBDQUEUE, "\033[70;1H");
		break;
	case 0x8b:	/* CLEAR */
		if(kbdrepeat & RPTON) { /* turn off repeat */
			kbdrepeat = 0;
			return;
		}
		qputstr( &KBDQUEUE, "\033[2J");
		break;
	case 0x8c:	/* local clear function */
		if(kbdrepeat & RPTON) {
			kbdrepeat = 0;
			return;
		}
		else
			qputc(&KBDQUEUE, c);
		break;
	case 0x80 | 'A':
	case 0x80 | 'B':
	case 0x80 | 'C':
	case 0x80 | 'D':
	case 0x80 | 'H':
		qputstr( &KBDQUEUE, "\033[");
		qputc( &KBDQUEUE, c & 0x7f);
		break;
	default:
		if (c != '\0')
			qputc(&KBDQUEUE, (int)c);
	}
}


kbdrpt()
{
	char	c;

	if (kbdrepeat & RPTLOOKUP) {
		if ((c = keytab[kbdrepeat & 0xff][K_CHAR]) & 0x80) {
			kchkchar(c);
		}
		else if (c != '\0') {
			qputc(&KBDQUEUE, (int)c);
		}
	}
	else {
		qputc(&KBDQUEUE, kbdrepeat & 0xff);
	}
}



kgetc()
{
	register int	c;
	register char	s;
	register start, end, i;

	if(KBDQUEUE.c_cc > 0)
		return(qgetc(&KBDQUEUE));

	while ((s = (DUART->b_sr_csr & (RCV_RDY | FRM_ERR | PAR_ERR | OVR_RUN | RCVD_BRK))) == 0)
		aciapaws();		/* so we don't overload stupid part */

	if (!(s & RCV_RDY))
		return(-1);
	c = DUART->b_data;
	if ((s & (FRM_ERR | PAR_ERR | OVR_RUN | RCVD_BRK)) == 0) /* just a char */
	{
 		if(kbdrepeat & RPTON) {
			kbdrepeat = 0;	/* turn off repeat */
			return(-1);
		}
		if (c & 0x80) {
			switch (c & 0xff) {
			case 0x8e:	/* un/shited key 41, selftest */
				test32(1);
			case 0x8f:	/* shift break (disconnect) reboot the terminal */
				sendchar(0x11);	/* cntl-Q	*/
				asm("  JMP  reboot");
			case 0xae:	/* setup */
			case 0xc3:	/* right-arrow */
			case 0xc4:	/* left-arrow */
				return(c);
			case 0xb2:
			case 0xf2:	/* num lock key */
				VALNUM= 1 - VALNUM;
				setbram();
				num_setup();
				return(-1);
			}
			i = keytabsearch(c);
			/*
	 *	here either keytab[i][K_KEY] == c, or c is not in the table
	 *	If c is not in the table, there is an error somewhere, so ring the bell
	 */
			if (c == keytab[i][K_KEY]) {
				if (((c = keytab[i][K_CHAR]) & 0xff) <= 0x89 )	/* pfkey, break, disconnect */
					return(c);
				kchkchar(c);
				return(-1);
			}
		}
		else
			return(c);
	}
	else if ((s & PAR_ERR) && (s & RCV_RDY))
	{
		VALCAPS = (c & 0x4) ? 0 : 1; /* set the caps lock flag */
		cap_setup();
		setbram();	/* fix checksum */
		if(c & 0x10) {
			/* not repeating anything */
			kbdrepeat = 0;
			rptcount = 0;
		}
		else {
			/*
			* the keyboard will send us a character to repeat
			* since we dont repeat in setup, throw the next
			* character away.
			*/
			kbdrepeat = RPTON;
		}
	}
	return(-1);
}

keytabsearch(c)		/* search keytab[] for char c */
unsigned char c;
{
	register int start, end, i;
/*
 *	Assume start, end and i are indexes into keytab,
 *	an array of two element structures. The elements are:
 *		keytab[][2], the two elements are:
 *			char key;
 *			char c; /* the char to send, 0 - not sent
 *		}
 */

		start = 0;
		end = sizeof(keytab) / sizeof(keytab[0]);
		for (i = (start + end) / 2; 
		    (keytab[i][K_KEY] != c) && (start <= end); 
		    i = (start + end) / 2)
			if (c > keytab[i][K_KEY])
				start = i + 1;
			else
				end = i - 1;

	return (i);
			/*
	 *	here either keytab[i][K_KEY] == c, or c is not in the table
	 *	If c is not in the table, there is an error somewhere, so ring the bell
	 *	(bell gets rung by caller)
	 */
 }
