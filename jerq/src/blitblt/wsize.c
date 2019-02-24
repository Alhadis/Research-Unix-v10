#include <jerq.h>

#define XSIZE	512
#define YSIZE	512

main(argc, argv)
char **argv;
{
	Point size; Rectangle r, canon();
	size.x = XSIZE;
	size.y = YSIZE;
	if (argc > 2) {
		size.x = atoi(argv[1]);
		size.y = atoi(argv[2]);
	}
	r = P->layer->rect;
	r.origin = div(sub(add(r.corner, r.origin), size), 2);
	r.corner = add(r.origin, size);
	r = canon(r);
	reshape(r, 1);
	exit();
}
