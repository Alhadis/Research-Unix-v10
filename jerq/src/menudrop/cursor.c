#include <jerq.h>

static Texture c_ul[]={
	0xFF80, 0xFE00, 0xF800, 0xFC00, 0xFE00, 0xDF00, 0xCF80, 0x87C0,
	0x83E0, 0x01F0, 0x00F8, 0x007C, 0x003E, 0x001F, 0x000E, 0x0004 };
static Texture  c_ll[]={
	0x0004, 0x000E, 0x001F, 0x003E, 0x007C, 0x00F8, 0x01F0, 0x83E0,
	0x87C0, 0xCF80, 0xDF00, 0xFE00, 0xFC00, 0xF800, 0xFE00, 0xFF80 };
static Texture  c_ur[]={
	0x01FF, 0x007F, 0x001F, 0x003F, 0x007F, 0x00FB, 0x01F3, 0x03E1,
	0x07C1, 0x0F80, 0x1F00, 0x3E00, 0x7C00, 0xF800, 0x7000, 0x2000 };
static Texture  c_lr[]={
	0x2000, 0x7000, 0xF800, 0x7C00, 0x3E00, 0x1F00, 0x0F80, 0x07C1,
	0x03E1, 0x01F3, 0x00FB, 0x007F, 0x003F, 0x001F, 0x007F, 0x01FF };

static Texture *TextTab[] = { c_ul, c_ll, c_ur, c_lr };

docursor(i)
int i;
{
	static Point curspt;
	static int cursordown;
	static Bitmap *cursaved, blit;
	if (cursaved == 0) {
		cursaved = balloc(Rect(0, 0, 16, 16));
		blit.base=addr(&display,Pt(0,0));
		blit.width=display.width;
		blit.rect=Jrect;
	}
	if (cursordown) {
		bitblt(cursaved, cursaved->rect, &blit, curspt, F_XOR);
		cursordown = 0;
	}
	if (i < 0)
		return;
	texture(cursaved, cursaved->rect, TextTab[i], F_STORE);
	Jcursinhibit();
	for (;;) {
		curspt = mouse.xy;
		bitblt(cursaved, cursaved->rect, &blit, curspt, F_XOR);
		nap(2);
		if (button3()) break;
		bitblt(cursaved, cursaved->rect, &blit, curspt, F_XOR);
	}
	while (button3())
		/* void */;
	cursset(add(curspt, Pt(50,50)));
	Jcursallow();
	cursordown = 1;
}
