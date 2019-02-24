#include <stdio.h>

#define SCALE_X      0.031980
#define SCALE_Y      0.015463
#define SCALE_Z      0.051282

main()
{	float cx, cy, cz, cr;
	char c;

	while (scanf("%c %f %f %f %f\n", &c, &cx, &cy, &cz, &cr) == 5)
	{	cx = cx/SCALE_X;
		cy = cy/SCALE_Y;
		cz = cz/SCALE_Z;
		printf("%c %f %f %f %f\n", c, cx, cy, cz, cr);
	}
}
