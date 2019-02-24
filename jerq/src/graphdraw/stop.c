#include "defs.h" 
extern char *labsmenutext[];
extern char *menutext[];
extern Rectangle Lrect, Rrect;

restart() {
	
	int i;	

	for(;; sleep(2), wait(MOUSE)) {	
		if ( button1() ) {
			while( button1() ) wait(MOUSE);

			if ( ptinrect(mouse.xy, box) ) {
				cmds();
				return;
			}
			restart_now();
			return;
		}
		if ( button3() )
			return;
	}
}

restart_now()
{
	rectf( &display, Dspace, f3);
	rectf( &display, box, f1 );
	freenodes();
	init();
}

reshape()
{
#define BORDER 8
#define C_RESHAPE 8
#define INSET 3

	struct Layer *new;
	struct Proc *p = P;
	Rectangle r;

	r.origin.x = BORDER;
	r.origin.y = p->rect.origin.y;
	r.corner.x = XMAX-BORDER;
	r.corner.y = r.origin.y + (XMAX + BOXWIDTH - 2*BORDER);
	if (r.corner.y > YMAX-BORDER) {
		r.corner.y = YMAX-BORDER;
		r.origin.y = r.corner.y - (XMAX + BOXWIDTH - 2*BORDER);
		}

	dellayer(p->layer);
	new = newlayer(r);
	if (new == 0) exit(0);
	p->layer = new;
	p->rect = inset(r,INSET);
	muxnewwind(p, C_RESHAPE);

	Set_Up_Space();
	restart_now();
}

freenodes() {
	int i=0;
	NODE *current, *next;

	while (i < last)  {
		current = list[i].next;
		while (current != 0) {
			next = current->next;
			free(current); 
			current = next;
		}
		i++;
	}
}

stop() {

	for(;; sleep(2), wait(MOUSE)) {	
		if ( button3() ) 
			return;
 		if ( button1() ) {
			while( button1() ) wait(MOUSE);

			if ( ptinrect(mouse.xy, box) ) {
				cmds();
				return;
			}

			sendchar( 'q' );
		}
	}
}

Set_Up_Space()
{
/* Dspace is P->rect minus the box; the display space */
	Dspace.origin = P->rect.origin;    
	Dspace.corner.x = P->rect.corner.x;
	Dspace.corner.y = P->rect.corner.y - BOXWIDTH;

/* define left half & right half of screen */
	Lrect.origin = Dspace.origin;
	Lrect.corner.x = (Dspace.origin.x + Dspace.corner.x)/2;
	Lrect.corner.y = Dspace.corner.y;

	Rrect.origin.x = (Dspace.origin.x + Dspace.corner.x)/2;
	Rrect.origin.y = Dspace.origin.y;
	Rrect.corner = Dspace.corner;

/* define box dimensions */
	box.origin.x = Dspace.origin.x;
	box.origin.y = Dspace.corner.y;
	box.corner.x = Dspace.corner.x;
	box.corner.y = Dspace.corner.y + BOXWIDTH;

/* blank goes in this space in box to show box chosen */
	boxstart.x = box.origin.x;
	boxstart.y = box.corner.y - (BOXWIDTH - 3);

/* first char goes here on line in box */
	boxpt.x = boxstart.x + (BOXWIDTH - 5);
	boxpt.y = boxstart.y;  
}


init() {
	int i, j;

	last = 1;
	lastset = 1;
	numverts = 0;
	numedges = 0;
	edgewtsum = 0;
	for ( i = 0; i < MXPTS; i++ ) {
		list[i].status = 0;  
/* list[0].status is always 0 so indices start at 1. (Hope I got them all!) */
		list[i].next = 0;
		list[i].deg = 0;
		list[i].set = -1;
	}
/* first time around 'show vert/edge labels' is selected */
	showingvertlabs = -1; 
	showingedgelabs = -1;
	labsmenutext[2] = "show vertex labels";
	labsmenutext[3] = "show edge labels";
	eucflag = -1;
	labsmenutext[5] = "turn euclidean edge labels on";  /*they start off */
	gridstate = 1;
	menutext[10] = "small grid";	/* no grid */

	edgesumflag = -1;
	labsmenutext[6] = "show sum of edges";  /*they start off off */
	setgraphtype(0);
	setgraphtype(5);
/* init sets */
	for (i = 1; i < MXSETS; i++) {
		set[i].status = 0;
		set[i].next = 0;
	}
	return;
}
