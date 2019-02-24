#include "tr.h"
openpl(s) 
char	*s;
{
	register double	t;
	/* CREATE SPACE FOR TROFF */
	t = atof(s);
	if (t <= 0. || t > 1000.) 
		printf("\n.sp\n.ne 6i\n.rs\n.sp 6i\n");
	else 
		printf("\n.sp\n.ne %s\n.rs\n.sp %s\n", s, s);
}
