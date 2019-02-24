#include "jraw.h"
#include "dots.h"

List *Dots.copy()
{
	return (sel) ? (List *) new Dots(R) : 0;
}

Dots.down(Point p)
{
	Rectangle r;
	sort();
	if (ul == p)
		sel=3;
	else if (lr == p)
		sel=12;
	else if (ll == p)
		sel=9;
	else if (ur == p)
		sel=6;
	else if (p <= R)
		sel = 15;
	else
		return mod;
	return mod |= DOTHIT;
}

Dots.selleck(Rectangle r)
{
	if (R <= r) {
		sel = 15;
		mod |= DOTBOX;
	}
	return mod;
}

Texture dots = {
	0xc1c1, 0xc1c1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc1c1,
	0xc1c1, 0xc1c1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc1c1,
};

void Dots.draw(Bitmap *b, int fc)
{
	Rectangle r = canon(R);
	texture(b,inset(r,-2),&dots,fc);
}

List *Dots.unsel(int mask, register List *prev)
{
	if (sel)
		sort();
	return List::unsel(mask,prev);
}
