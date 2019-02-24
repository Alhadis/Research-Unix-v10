#include	<CC/jerq.h>
#include	"frame.pri"
#include	"camera.pub"

Movie.Movie()
{
	start = 0;
	showgrid = 1;
	doshade = 0;
	nx = ny = 0;
	period = 0;
	maxframe = 0;
	N = 10;
	m = this;
	tdata = 0;
	camera = new Camera(Drect, &bound);
	ctr = bound.o;
	string(&defont, "hold down buutton 2 near a control for help", &display,
		add(bound.o, Pt(20, 20)), F_OR);
}

void
Movie.read()
{
	register n;
	Point temp, orig;
	Bitmap *tempb;

	for(wait(RCV); (n = rcvchar()) != 'Q'; wait(RCV))
		switch(n)
		{
		case 'G':
			showgrid = 1;
			break;
		case 'S':
			doshade = 1;
			break;
		case 'X':
			nx = getn();
			break;
		case 'Y':
			ny = getn();
			break;
		case 'P':
			period = getn();
			break;
		case 'n':
			nframes = getn();
			for(;;)
			{
				if(tempb = balloc(bound))
				{
					bfree(tempb);
					break;
				}
				orig = (bound.o+bound.c)/2;
				temp = bound.c - bound.o;
				n = muldiv(temp.x, 95, 100);
				if(temp.x <= temp.y)
					temp.x = temp.y = n;
				else
					temp.x = n;
				bound.o = orig - temp/2;
				bound.c = bound.o + temp;
			}
			frames = (Frame *)alloc(nframes*sizeof(Frame));
			break;
		case 'F':
			if(tdata == 0)
				tdata = alloc(nx*ny*sizeof(Hcoord));
			n = maxframe++;
			frames[n].rd(nx*ny);
			frames[n].view(camera->clat, camera->clon, camera->cdist,
				camera->alat, camera->alon, camera->persp);
			break;
		case 'I':
			floor = getn();
			break;
		}

}

void
Movie.showframe(int i)
{
	rectf(&display, bound, F_CLR);
	start = i;
	show();
}

char *
Movie.ft(int i)
{
	return(frames[i].time);
}

void
Movie.show()
{
	frames[start].view(camera->clat, camera->clon, camera->cdist,
		camera->alat, camera->alon, camera->persp);
}

void
Movie.advance()
{
	if(adv->val)
	{
		if(++start == maxframe) start = 0;
	}
	camera->advance();
}

void
Movie.adjust()
{
	camera->adjust();
}
