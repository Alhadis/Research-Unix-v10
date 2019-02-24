/* Plotting functions for Sys V and BSD systems */

/* open the plotting output */
extern void openpl(void);

/* close the plotting output */
extern void closepl(void);

/* make sure the page or screen is clear */
extern void erase(void);

/* plot a point at _x,_y, which becomes current */
extern void point(int, int);	/* point(_x,_y) */

/* coordinates to be assigned to lower left and upper right
   corners of (square) plotting area */
extern void space(int, int, int, int);
#define range(_x,_y,_X,_Y)  space(_x,_y,_X,_Y)

/* place text, first letter at current point, which does not change */
extern void label(char*);
#define text(_s) label(_s);

/* draw line from current point to _x,_y, which becomes current */
extern void cont(int, int);
#define vec(_x,_y)  cont(_x,_y)

/* _x,_y becomes current point */
extern void move(int, int);	/* move(_x,_y) */

/* specify style for drawing lines: "dotted", "solid", "dotdash" */
extern void linemod(char*);
#define pen(_s)  linemod(_s)
