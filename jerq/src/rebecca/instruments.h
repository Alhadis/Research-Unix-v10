
#define Button_r	10
#define OK	(*ALIVE && !QUIT)

int actbutton(), clearbutton(), makebutton();
int actzoom(), clearzoom(), makezoom();
int actpaint(), clearpaint(), makepaint();
int actruler(), nothing();
int actbar(), clearbar(), makebar();
int actmove(), clearmove(), makemove();
int actprobe(), clearprobe(), makeprobe();
int actwrite(), clearwrite(), makewriter();
int actrun(), clearrun(), makerunner();

Point whereami(), PT();
Rectangle realbox();

#define realpoint(n)	add((n), dr.origin)

#define width		 2
#define height		20

#define resident(a, b)	(have[b] < 2*SZ || (l_side[b] <= a && a < r_side[b]))
