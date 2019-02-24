/* Plotting functions for v8 and v9 systems */
/* This file is an alternative to plot.h */

#define SOLID "solid"
#define DOTTED "dotted"
#define DASHED "dashed"
#define DOTDASH "dotdash"

/* open the plotting output */
#define openpl()  printf("o\n")

/* close the plotting output */
#define closepl()  printf("cl\n")

/* make sure the page or screen is clear */
#define erase() printf("e\n")

/* plot a point at _x,_y, which becomes current */
#define point(_x,_y)  printf("poi %d %d\n", _x,_y)

/* coordinates to be assigned to lower left and upper right
   corners of (square) plotting area */
#define range(_x,_y,_X,_Y)  printf("ra %d %d %d %d\n", _x,_y,_X,_Y)

/* place text, first letter at current point, which does not change */
#define text(_s)  {if(*(_s) == ' ')printf("t \"%s\"\n",_s); else printf("t %s\n", _s); }

/* draw line from current point to _x,_y, which becomes current */
#define vec(_x,_y)  printf("v %d %d\n", _x,_y)

/* _x,_y becomes current point */
#define move(_x, _y)  printf("m %d %d\n", _x, _y)

/* specify style for drawing lines: "dotted", "solid", "dotdash" */
#define pen(_s)  printf("pe %s\n", _s)

#define BLACK "black"
#define RED "red"
#define GREEN "green"
#define BLUE "blue"

static struct {
	char *name;
	char *hsbcode;
} colors[] = {
	BLACK, "0 0 0",
	RED, "0 1 1",
	GREEN, ".33 1 1",
	BLUE, ".67 1 1"
};

static void
color(char *s)
{
	char *t;
	int i;
	for(i=0; i<sizeof(colors)/sizeof(*colors); i++)
		if(strcmp(s, colors[i].name) == 0)
			s = colors[i].hsbcode;
	while(t = strchr(s, ','))
		*t = ' ';
	printf("co \"%s\"\n",s);
}
