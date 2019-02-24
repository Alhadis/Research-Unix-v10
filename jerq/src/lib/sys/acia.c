#include <jerq.h>
#include <queue.h>
#include <setup.h>
/*static*/ int dtr;
aciainit(speed)
char speed;
{
	/* init	the host side */
	int i;

	DUART->a_cmnd=RESET_RECV|DIS_TX|DIS_RX;
	DUART->a_cmnd=RESET_TRANS;
	DUART->a_cmnd=RESET_ERR;
	DUART->a_cmnd=RESET_MR;
	DUART->mr1_2a=CHAR_ERR|NO_PAR | CBITS8;
	DUART->mr1_2a=NRML_MOD|ONEP000SB;
	DUART->a_sr_csr=speed;
	DUART->a_cmnd=ENB_TX|ENB_RX; /* enable TX and RX last */
	/* configure the output	port */
	DUART->ip_opcr=0x70;
	DUART->scc_ropbc=0xff;	/* make sure the port is reset first */
	DUART->scc_sopbc=0x01;	/* make sure DTR is on */
	if(VALSCREENCOLOR)
		BonW();
	else
		WonB();
	for(i=0; i<255; i++)
		;		/*delay	for stupid device */
	dtr=1;
}
rcvchar(){
	register c=qgetc(&RCVQUEUE);
	if(c!=-1)
		c&=0xFF;
	return c;
}
sendchar(c){
	qputc(&OUTQUEUE, c);
	aciatrint();
}
aciatrint(){
	register sr, c;
	sr=spl5();
	/* should check	here for carrier fail */
	DUART->a_cmnd=ENB_TX;		/* done	here so	flags are right	*/
	if(DUART->a_sr_csr&(XMT_EMT|XMT_RDY)){
		if((c=qgetc(&OUTQUEUE))==-1 || dtr==0)
			DUART->a_cmnd=DIS_TX;		/* forget it */
		else
			DUART->a_data=c;		/* send the char */
	}
	splx(sr);
}
sendbreak(){
	register int sr,i;
	sr=spl5();
	DUART->a_cmnd=(STRT_BRK|ENB_TX);
	i= 0x27106;
	while(--i>0)
		;
	DUART->a_cmnd=(STOP_BRK|DIS_TX);
	kbdinit();
	splx(sr);
}
dtrctl(flg) {
	dtr=flg;
	aciatrint();
}
tstdcd() {
	return((DUART->ip_opcr & 0x40) != 0);
}
aciapaws()
{
	int i;
	for(i=0 ; i<16; i++);
}
/*
 * disconnect will drop the DTR line for approx. 1/2 second.
 */
disconnect() {
	register long i, sr;
	
	if(dtr==0)
		return;	/* already being disconnected */
	sr=spl5();
	dtr=0;
	DUART->scc_ropbc=0x01;	/* disable DTR	*/
	i=0x30d48;
	while(--i>0)
		;
	DUART->scc_sopbc=0x01;	/* Turn back on DTR */
	dtr=1;	/* allow disconnects again */
	kbdinit();
	splx(sr);
}
