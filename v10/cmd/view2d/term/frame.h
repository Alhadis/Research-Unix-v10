#ifndef	Rectangle
#include	<CC/jerq.h>
#endif
#include	<pot.pub>

class Camera;
class Frame;
class Clock;

class Frame
{
>pub
	char pub_filler[40];
>pri
	short *data;
	char time[20];
	Bitmap *b;
	short lat, lon, dist;
	short alat, alon, persp;
	friend Movie;
	void rd(int);
	void draw();
	void show();
	void view(short, short, short, short, short, short);
	void seg(short, short, short, short);
	void transdata();
>
};

class Movie
{
>pub
	char pub_filler[28];
>pri
	Frame *frames;
	Camera *camera;
	short nframes;
	short start;
	short showgrid;
	short period;
	short doshade;
	short floor;
	short maxtime, maxframe;
	short N;
	friend Clock;
	friend char *genfn(int);
	friend void Frame.draw();
>
public:
	Movie();
	short nx, ny;
	Point ctr;
	Rectangle bound;
	Bmappot *run, *adv, *hide;
	char *tdata;
	void read();
	void showframe(int);
	char *ft(int);
	void show();
	void advance();
	void adjust();
};
extern Movie *m;
