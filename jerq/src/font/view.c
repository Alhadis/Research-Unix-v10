/*
	view:
		look at a font.
	usage:	jx view < font
*/
#include <jerq.h>
#include <jerqio.h>
#include <font.h>

main()
{
	register c;
	register char *s;
	char buf[256];
	Point p;
	register Font *f;
	int width = Drect.corner.x - Drect.origin.x;
	extern Font *get();

	request(KBD|RCV|SEND);
	for (;;) {
		f = get();
		s = buf;
		p = add(Pt(2, 2), Drect.origin);
		for(c = 1; c <= f->n; c++)
		{
			*s++ = c;
			*s = 0;
			if(strw(f, buf) > width)
			{
				*--s = 0;
				if((p.y + f->height) > Drect.corner.y)
					break;
				string(f, buf, &display, p, F_XOR);
				p.y += f->height;
				s = buf;
				*s++ = c;
				*s = 0;
			}
		}
		if((p.y + f->height) <= Drect.corner.y)
		{
			string(f, buf, &display, p, F_XOR);
		}
		ffree(f);
		wait(KBD);
		c = kbdchar();
	}
	exit(0);
}

strw(f, s)
	register char *s;
	register Font *f;
{
	register wid=0;
	for(; *s; s++)
		wid+=f->info[*s].width;
	return(wid);
}

Font *
get()
{
	static i = 0;
	Font *f;
	extern getchar();

	if(i++)
		exit(0);
	f = infont(getchar);
	return(f);
}
