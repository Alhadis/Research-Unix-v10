#include <jerq.h>
#include "font.h"
#include "../comm.h"
#include "term.h"

struct arena arena;
int More, pstate, page, nextpage, cstate, dying, resolution = 1;
extern Font *curfont;
extern curpage;

char *b3m[] =
{
	"more",
	"goto page",
	"quit",
	"exit forever",
	NULL
};
static Menu menu = { b3m };

tterm()
{
	Texture *t;

	pstate = S_DRAW;
	cstate = 0;
	gap = 0;
	page = nextpage = -1;
	arena.page = rscale(rpage);
	arena.layer = rscale(rsubp(Drect, Drect.origin));
	cursor.x = cursor.y = 0;
	Mode = F_OR;
	reader.start1 = reader.start2 = reader.end = 0;
	if(arena.bp) rectf(arena.bp, arena.bp->rect, F_CLR);
	while(wait(KBD|MOUSE|RCV))
	{
		if(own()&KBD) switch(kbdchar())
		{
		case '\n':
		case '\r':
			More++;
			break;
		case 'x':
			Exit();
			break;
		case 'q':
			Quit();
			break;
		case 'p':
			Page();
			break;
		default:
			ringbell();
			break;
		}
		if(own()&RCV)
			if(Receive()) break;
		if((own()&MOUSE) && button123() && ptinrect(mouse.xy, Drect))
		{
			if(button1())
				Pan(1);
			else if(button2())
				Speed(2);
			else
			{
				t = cursswitch((Texture *)0);
				switch(menuhit(&menu, 3))
				{
				case 0:
					More++;
					break;
				case 1:
					Page();
					break;
				case 2:
					if(confirm(3)) Quit();
					break;
				case 3:
					if(confirm(3)) Exit();
					break;
				}
				(void)cursswitch(t);
			}
		}
		if(pstate != S_FONT) drawabit();
	}
	(void)cursswitch((Texture *)0);
	dying = 0;
}

Quit()
{
	send(HQUIT);
	dying++;
}

Exit()
{
	send(HEXIT);
	dying++;
}

Page()
{
	char buf[64];
	char *s;
	int n;

	strcpy(buf, "Page ");
	edit(buf);
	s = &buf[5];
	n = Atoi(s, page);
	if(n > 0)
	{
		arena.layer.corner.y -= arena.layer.origin.y;
		arena.layer.origin.y = 0;
		stipple(display.rect);
		if(arena.bp) rectf(arena.bp, arena.bp->rect, F_CLR);
		nextpage = -1;
		cursor.x = cursor.y = 0;
		(void)cursswitch((Texture *)0);
		cstate = 0;
		send(HSEEK);
		sendn(n);
		sendn(0);
		page = n;
		reader.start1 = reader.start2 = reader.end = 0;
		pstate = S_DRAW;
		More = 0;	/* throw away any accumalted more's */
	}
}

Receive()
{
	char *s;

	if(buildpkt() == 0) return(0);
	switch(pkt.type)
	{
	case TEXIT:
		(void)cursswitch((Texture *)0);
		exit();
	case TTERM:
		return(1);
	case TFONT:
		for(s = pkt.data; *s++;);
		addfont(pkt.data, s);
		pstate = S_DRAW;
		break;
	case TTEXT:
		if((page == pkt.page) || (page == -1))
		{
			if(page == -1) page = pkt.page;
			radd(pkt.offset, pkt.data, pkt.size);
		}
		break;
	}
	return(0);
}

Texture tslice = {
	0xFFC0, 0x8060, 0x8050, 0x8048, 0x8044, 0x807E, 0x8002, 0x8002,
	0xAAAA, 0x8002, 0x8002, 0x8002, 0xAAAA, 0x8002, 0x8002, 0xFFFE,
};
Texture tpage = {
	0x7FE0, 0x4030, 0x4028, 0x4024, 0x4022, 0x403F, 0x4001, 0x4001,
	0x4001, 0x4001, 0x4001, 0x4001, 0x4001, 0x4001, 0x4001, 0x7FFF,
};

drawabit()
{
	int y;

	switch(pstate)
	{
	case S_PAGE:
		if(More)
		{
			More--;
			arena.layer.corner.y -= arena.layer.origin.y;
			arena.layer.origin.y = 0;
			stipple(display.rect);
			if(arena.bp) rectf(arena.bp, arena.bp->rect, F_CLR);
			send(HSEEK);
			sendn(page = nextpage);
			sendn(0);
			nextpage = -1;
			cursor.x = cursor.y = 0;
			pstate = S_DRAW;
			if(cstate) cstate = 0, (void)cursswitch((Texture *)0);
		}
		break;
	case S_SLICE:
		if(More)
		{
			More--;
			y = min(2*arena.layer.corner.y-arena.layer.origin.y,
				arena.page.corner.y);
			arena.layer.origin.y += y - arena.layer.corner.y;
			arena.layer.corner.y = y;
			if(arena.bp)
				bitblt(arena.bp, riscale(arena.layer),
					&display, Drect.origin, F_STORE);
			else
				stipple(display.rect);
			pstate = S_DRAW;
			if(cstate) cstate = 0, (void)cursswitch((Texture *)0);
		}
		break;
	case S_DRAW:
		if(cursor.y > arena.layer.corner.y)
		{
	sliceit:
			if(cstate != S_SLICE)
			{
				cstate = S_SLICE;
				(void)cursswitch(&tslice);
			}
			pstate = S_SLICE;
			if(arena.bp && (reader.start2 < reader.end))
			{
				pmap = plmap;
				troff(arena.bp, &reader.start2, reader.end);
			}
		}
		else if(reader.start1 < reader.end)
		{
			pmap = pdmap;
			troff(&display, &reader.start1, reader.end);
			if((pstate != S_FONT) && arena.bp && (reader.start2 < reader.end))
			{
				pmap = plmap;
				troff(arena.bp, &reader.start2, reader.end);
			}
			if(cursor.y > arena.layer.corner.y)
				goto sliceit;
		}
		if((nextpage != -1) && (pstate == S_DRAW))
		{
			if(cstate != S_PAGE)
			{
				cstate = 2;
				(void)cursswitch(&tpage);
			}
			pstate = S_PAGE;
		}
	}
}
