
#define PICO	1
#define DUMP	2
#define RUNCODE	3
#define BINARY	4

typedef struct point
{	short x, y;
} Point;

typedef struct rectangle
{	Point origin, corner;
} Rectangle;

struct pfile
{	Rectangle r;
	int nchan, type;
	char name[64];
	unsigned char *pixred, *pixgrn, *pixblu;
};
