#include "univ.h"

int ALARM_CYCLE = 15;		/* 1/4 second for internal refresh */
int HOST_CYCLE = 4;		/*  1  second for host objects */

ALARMServe()
{
	static cycle;

	if( own()&ALARM ){
		if( !(own()&RCV) ) Dirty((Pad*)0);
		if( ++cycle, (cycle%=HOST_CYCLE)==0 ) Cycle();
		alarm( ALARM_CYCLE );
	}
}

main()
{
	request(KBD|MOUSE|SEND|RCV|ALARM);
	cursswitch(&Coffee);
	Configuration |= NOVICEUSER;
#ifdef mac32
		Configuration |= BIGMEMORY;
#endif
	sleep( 60 );
	PadClip();
	alarm( ALARM_CYCLE );
	for( ;; ){
		LayerReshaped();
		MOUSEServe();
		KBDServe();
		RCVServe();
		ALARMServe();
		wait(ALARM|RCV|MOUSE|KBD);
	}
}
