#include	<jerq.h>
#include	<font.h>

static void
getstr(s, p, start, bs, at)
	char *s, *start;
	Point p;
{
	char c;
	static char str[] = "x";

	for(;;)
	{
		wait(KBD);
		if(((c=kbdchar()) == '\r') || (c == '\n'))
		{
			*s = '\0';
			return;
		}
		if(c == bs)
		{
			if(s>start)
			{
				str[0] = *(--s);
				p.x -= defont.info[*s].width;
				*s = 0;
				string(&defont, str, &display, p, F_XOR);
			}
		}
		else if(c == at)
		{
			if(s>start)
			{
				p.x -= strwidth(&defont, start);
				string(&defont, start, &display, p, F_XOR);
				s = start;
				*s = 0;
			}
		}
		else if((c >= ' ') && (c <= '~'))
		{
			if(s-start<50)
			{
				*s++ = (str[0] = c);
				*s = 0;
				p = string(&defont, str, &display , p, F_XOR);
			}
		}
	}
}

kbdstr(s)
	char *s;
{
	kbordstr(s, s+strlen(s), '\b', '@');
}

kbordstr(str, start, bs, at)
	char *str, *start;
{
	Bitmap *b;
	Point p;
	Rectangle r;

	p = mouse.xy;
	p.x = min(p.x, Drect.corner.x-300);
	b = balloc(raddp(Rect(0, 0, 300, defont.height+4), p));
	bitblt(&display, b->rect, b, b->rect.origin, F_STORE);
	rectf(&display, b->rect, F_STORE);
	rectf(&display, r = inset(b->rect, 1), F_XOR);
	p = add(r.origin, Pt(1,1));
	p = string(&defont, str, &display, p, F_XOR);
	getstr(start+strlen(start), p, start, bs, at);
	bitblt(b, b->rect, &display, b->rect.origin, F_STORE);
	bfree(b);
}
