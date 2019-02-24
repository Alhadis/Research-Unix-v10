#include <jerq.h>
#define MXPTS 505  /* max no. of verts */
#define MXSETS 105 
#define MXDIST 50  /* max allowed dist of mouse from vert when selecting */

/* string lengths */
#define MXLINE 80
#define MXSTR 50
#define MEDSTR 30
#define MINSTR 5

#define SEG 0
#define ARC 1
#define LOOP 2

#define BOXWIDTH 20	/* width of bottom box */

struct node {
	int ind;
	char wt[MEDSTR];
	char color[MINSTR];
	int type;
	struct node *next;
} ;
typedef struct node NODE;

struct list {
	int status;
	int deg;
	char wt[MEDSTR];
	char color[MINSTR];
	int set;   /* number of set this point is in, if any */
	Point pt;
	NODE *next;
} ; 
extern struct list list[MXPTS];

struct member {
	int ind;  /* index of vertex in set */
	struct member *next;
} ;
typedef struct member MEMBER;

struct set {
	int status;
	Rectangle Srect;
	MEMBER *next;
} ;
extern struct set set[MXSETS];

extern Rectangle Dspace, box;
extern Point boxpt, boxstart;  
Code f1, f2, f3;
Point closestpt();
int findind();
NODE *findnode();
int last, numverts, numedges, edgewtsum, lastset;
	/* last is the next free index */
	/* numedges counts undir edges twice */


int dsize, csize, Hcsize, loopcsize;
	/* disc size, circle size, heavy vertex circle size, loop circle size */
int showingvertlabs, showingedgelabs;  /* flags for show/hide labels */
int gridstate;	/* is there a grid?  How big? */

char getkbdstr(), Amsg[80];  /*Amsg for anim msg*/
Point vertlabpt(), looplabpt(), arclabpt(), grid_enforce();

int dirflag, multedflag, loopflag, eucflag, edgesumflag, animflag;
