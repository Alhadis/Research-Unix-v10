#include <jerq.h>
#include "queue.h"
own(){
	register got=MOUSE|SEND|CPU;
	if(KBDQUEUE.c_cc>0)
		got|=KBD;
	if(RCVQUEUE.c_cc>0)
		got|=RCV;
	return got;
}
wait(r){
	register u;
	do; while((u=(own()&r))==0);
	return u;
}
