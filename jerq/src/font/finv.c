/*
	create an inverse vieo font

	usage: jx finv < font > invfont
*/

#include <jerq.h>
#include <jerqio.h>
#include <font.h>

main()
{
	register Font *f;
	extern putchar(), getchar();

	if(f = infont(getchar))
	{
		rectf(f->bits, f->bits->rect, F_XOR);
		outfont(f, putchar);
		ffree(f);
	}
	exit(0);
}
