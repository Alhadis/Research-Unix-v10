#include <jerq.h>

reshaped()
{
	if (P->state & (RESHAPED|MOVED)) {
		P->state &= ~(RESHAPED|MOVED);
		return 1;
	} else
		return 0;
}
