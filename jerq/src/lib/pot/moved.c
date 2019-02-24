#include	<CC/jerq.h>
#undef kill
#include	<CC/jerqproc.h>

extern
moved()
{
	register z;

	if(z = P->state&(RESHAPED|MOVED))
		P->state &= ~(RESHAPED|MOVED);
	return(z);
}
