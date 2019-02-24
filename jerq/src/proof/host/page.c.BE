#include	<stdio.h>
#include	"../comm.h"
#include	"host.h"

struct input input;
int lastpage = -1;

#define	CLICK	200000

tsend(page, offset)
{
	int count, np;
	char *s;

	if(debug)fprintf(debug, "tsend page=%d offset=%d ", page, offset);
	if(page == LARGE)
	{
		if(debug)fprintf(debug, "<<sending TTERM>>\n");
		send(TTERM);
		going = 0;
		return;
	}
	while((page > input.maxpage) && !input.eof)
		readpage();
	if(lastpage == LARGE)
	{
		if(debug)fprintf(debug, "<<sending TTERM>>\n");
		send(TTERM);
		going = 0;
		return;
	}
	if(page > input.maxpage) page = input.maxpage;
	while(input.pages[page].start < 0)
		page++;
	s = &input.base[input.pages[page].start + offset];
	count = lump(s, &input.base[input.pages[page].end]);
	if(count >= 0)
		np = -1;
	else
	{
		count = -count;
		if(page < input.maxpage)
			for(np = page+1; input.pages[np].start < 0; np++);
		else if(input.eof)
			np = LARGE;
		else
		{
			readpage();
			np = lastpage;
		}
	}
	if(debug)fprintf(debug, "nextpage=%d count=%d\n", np, count);
	send(TTEXT);
	sendn(page);
	sendn(np);
	sendn(offset);
	sendn(count);
	write(jerq, s, count);
}

initpage(n)
{
	register m;

	if(n > input.maxpage)
		input.maxpage = n;
	if(lastpage == -1)
		m = 0;
	else
		input.pages[lastpage].end = m = input.next;
	input.pages[n].start = m;
	lastpage = n;
#ifdef	DEBUG
	if(debug)fprintf(debug, "initpage: page[%d] start=%d\n", n, m);
#endif	DEBUG
}

out(c)
{
	extern chv3594/456(jhs,bin,0600): permission denied
ar *malloc(), *realloc();

	if(input.size == input.next)
	{
		if(input.base == 0)
			input.base = malloc(2);
		input.base = realloc(input.base, input.size += CLICK);
	}
	input.base[input.next++] = c;
}

outn(i)
	register i;
{
	out(i>>8);
	out(i&0377);
}

outs(s)
	char