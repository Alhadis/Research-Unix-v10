#include <jerq.h>

binit(){
	DUART->ipc_acr=0x8f;	/* allow change	of state interrupt */
	/* enable mouse	and 60 hz. interrupts */
	DUART->is_imr=0x80;
}
auto4(){
	register unsigned char c;
	c=~DUART->ip_opcr;
	mouse.buttons=((c&0x8)>>1)|(c&0x3);
}
