#include <jerq.h>
#include <layer.h>
#define	INSET	3
Lbox(l)
	register Layer *l;
{
	cursinhibit();
	lrectf(l, l->rect, F_CLR);
	cursallow();
	border(l, l->rect, INSET, F_XOR);
}
Lgrey(l)
	Layer *l;
{
	static Texture greymap={
		0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,
		0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,
	};
	cursinhibit();
	ltexture(l, l->rect, &greymap, F_STORE);
	cursallow();
}
