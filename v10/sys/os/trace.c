/*
 * general-purpose event tracing;
 * for debugging only
 * don't leave calls to trace in the system;
 * put them in when debugging things,
 * and remove them when you're through,
 * so you won't waste memory uselessly
 */

#include "sys/param.h"
#include "sys/systm.h"

#define	NTRACE	512

struct trace {
	time_t time;
	int i[4];
} tracebuf[NTRACE];

int traceptr = 0;

trace(p0, p1, p2, p3)
int p0, p1, p2, p3;
{
	register struct trace *tp;
	register int s;

	s = spl7();
	tp = &tracebuf[traceptr];
	if (++traceptr >= NTRACE)
		traceptr = 0;
	splx(s);
	tp->time = time;
	tp->i[0] = p0;
	tp->i[1] = p1;
	tp->i[2] = p2;
	tp->i[3] = p3;
}
