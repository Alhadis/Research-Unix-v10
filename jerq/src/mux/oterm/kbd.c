#include <jerq.h>
#include <queue.h>
#include <kbd.h>
#include <setup.h>

int	kbdrepeat;
int	rptcount;
int	kbdstatus;

kbdchar(){
	return qgetc(&KBDQUEUE);
}
kbdinit()
{
	/* init	the keyboard */
	DUART->b_cmnd=RESET_RECV|DIS_TX|DIS_RX;
	DUART->b_cmnd=RESET_TRANS;
	DUART->b_cmnd=RESET_ERR;
	DUART->b_cmnd=RESET_MR;
	DUART->mr1_2b=CHAR_ERR|NO_PAR|CBITS8;
	DUART->mr1_2b=NRML_MOD|ONEP000SB;
	DUART->b_sr_csr=BD300BPS;
	DUART->b_cmnd=RESET_MR|ENB_TX|ENB_RX;
	DUART->scc_ropbc=0x08; /* set output pins for kbd tx port*/
#ifdef ftang
	/* turn chirps on/off depending on BRAM */
	if(VALKEYTONE)
		kbdstatus=0;	/* no chirp */
	else
		kbdstatus=TTY_CHIRP;	/* chirp, chirp */
	DUART->b_data=kbdstatus|0x02; /* request status */
#endif
}
auto2(){
	register unsigned s, c;
	s=DUART->b_sr_csr;
	c=DUART->b_data;
	if(s&(FRM_ERR|OVR_RUN)) 
		return;
#ifdef ftang
	if(s&PAR_ERR){	/* control word: caps lock or repeat */
		checkbram();
		VALCAPS=(c&0x04)? 0 : 1; /* set the caps lock flag */
		setbram();
		if(c&0x10)	/* turn repeat off */
			kbdrepeat=rptcount=0;
		else		/* the next character is to be repeated */
			kbdrepeat=RPTON;
		/*
		 * Don't actually set the repeat bit until the character
		 * after the control code
		 */
	}else{		/* ordinary character */
		rptcount=0;	/* new char so restart repeat timer */
		if(c&0x80)
			c=defkeymap[c&0x7f];
		qputc(&KBDQUEUE, (int)c);
		if((kbdrepeat&RPTMASK) == RPTON){
			kbdrepeat=RPTHAVECHR|RPTON;
			kbdrepeat|=c;
		}
	}
#else
	qputc(&KBDQUEUE, (int)c);
}
kbdrpt(){
#ifdef ftang
	qputc(&KBDQUEUE, kbdrepeat&0xff);
#endif
}
