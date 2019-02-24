#include <jerq.h>
#include "font.h"
#include "../comm.h"
#include "term.h"

struct reader reader;

radd(offset, data, size)
	char *data;
{
	register char *to, *from;
	int olds;
	char *nd;

	reader.end = offset+size;
	if(reader.end > reader.size)
	{
		olds = reader.size;
		nd = surealloc(reader.size = reader.end+1024);
		for(from = reader.data, to = nd; olds--;)
			*to++ = *from++;
		free(reader.data);
		reader.data = nd;
	}
	for(to = &reader.data[offset], from = data; size--;)
		*to++ = *from++;
	if(offset == 0)
		reader.start1 = reader.start2 = 0;
	if((nextpage = pkt.nextpage) == -1)
	{
		send(HSEEK);
		sendn(page);
		sendn(reader.end);
	}
}

char *
surealloc(n)
{
	register char *s;

	s = alloc(n);
	if(s == 0)
	{
		rectf(&display, Drect, F_OR);
		rectf(&display, Drect, F_CLR);
		rectf(&display, Drect, F_OR);
		string(&defont, "YOU NEED MORE ALLOC MEMORY, CLEAR SOMETHING UP",
			&display, add(Drect.origin, Pt(10, 10)), F_XOR);
		while((s = alloc(n)) == 0)
			sleep(60);
		rectf(&display, Drect, F_CLR);
	}
	return(s);
}
