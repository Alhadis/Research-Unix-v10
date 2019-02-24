/*
	This file contains the code that handles the scheduling of the
	processes in the system. All moving objects set up a 'next run
	time' using add_event.

*/
#include "pengo.h"
#define	MAX_ARRAY	100		/* number of schedule slots */
#define	MAX_TIME	10000L

int	ebase	= 0;	/* pointer to first event */

struct	event {
	long	event_time;
	int	(*event_fn)();
	int	event_arg;
	int	next_event;
} event_array[MAX_ARRAY];	/* space for scheduled events */

add_event(etime, efn, earg)	/* VERY simple event scheduler */
long	etime;
int	(*efn)();
int	earg;
{
	int	count;
	int	last;
	Word	*water();

	for (count = 0; event_array[count].next_event != -1; count++)
			;	/* do nothing, the condition says it all */
	event_array[count].event_time = (etime += my_time);
	event_array[count].event_fn   = efn;
	event_array[count].event_arg  = earg;
	if (event_array[(last = ebase)].event_time > etime)
	{
		event_array[count].next_event = ebase;
		ebase = count;
	}
	else
	{
		while (
			event_array[event_array[last].next_event].event_time
			<
			etime
		)
			last = event_array[last].next_event;
		event_array[count].next_event = event_array[last].next_event;
		event_array[last].next_event = count;
	}
}

next_event()	/* execute the next event on the list. FALSE if none left */
{
	int	tmp;
	long	tmp0;
	int	ebase0;

	wait_mouse();
	if ((tmp = event_array[(ebase0 = ebase)].next_event) == -2)
		return FALSE;	/* all is done. All is finished */
	if ((tmp0 = (event_array[ebase0].event_time - my_time)) > 0)
		sleep ((int)tmp0);
	event_array[ebase0].next_event = -1;	/* clear this entry */
	ebase = tmp;				/* and reset pointer */
	my_time = event_array[ebase0].event_time;
	(*event_array[ebase0].event_fn)(event_array[ebase0].event_arg);
	if (score != old_score)
	{
		add_men(score, old_score);
		put_score(old_score = score);
	}
	return (TRUE);
}

cancel_events()
{
	extern	int	allover();

	for (ebase = MAX_ARRAY - 1; ebase > 0; ebase--)
		event_array[ebase].next_event = -1;
	event_array[ebase = 0].next_event = -2;
	event_array[ebase].event_time = MAX_TIME;
	event_array[ebase].event_fn  = allover;
}

allover()
{
	int	ptr;

	for (ptr = MAX_ARRAY - 1; ptr > 0; ptr--)
		event_array[ptr].event_time -= MAX_TIME;
	add_event(MAX_TIME, allover, NULL);
	my_time -= MAX_TIME;
}
