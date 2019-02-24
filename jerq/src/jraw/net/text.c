#include "geom.h"
#include "thing.h"
#include "text.h"

void TextList.walk(Vector *v, int (*f)(...))
{
	register i;
	register Text *t;
	for (i = 0; i < n; i++)
		if ((t = text(i))->used == 0)
			if ((*f)(v,t))
				t->used = 1;
}
