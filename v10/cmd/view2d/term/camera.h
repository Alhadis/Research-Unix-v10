#ifndef	defont
#include	<CC/jerq.h>
#endif
#include	"3d.h"
>pri
#ifndef	P_TICKS
#include	<pot.pub>
#endif
>

class Camera
{
>pub
	char pub_filler[44];
>pri
	Globepot *obs, *cam;
	Rulepot *dist;
	Bmappot *aim, *perspective;
	Bmappot *ospin, *cspin;
	Rulepot *ospinr, *cspinr;
	Rulepot *vertscale;
	short xdim;
	short pad;
>
public:
	short clat, clon, cdist, alat, alon, persp;
	Camera(Rectangle, Rectangle *);
	void reset();
	void redraw();
	void advance();
	void adjust();
};
