	/* hydro	0.40 */
	/* carbon	0.77 */
	/* oxygen	1.40 */
	/* nitrogen	1.50 */
	/* phosphorus	1.90 */

#define NNODES	 26

#define	CLIM	2000		/* maximum number of nodes */

typedef struct Node
{	Point p;		/* where to draw at */
	short z;		/* depth */
	Point op;		/* original base values */
	short oz;
	short flag;		/* node number */
} Node;

struct BALLS {
	Bitmap *Ball;				/* shaded sphere     */
	unsigned char radius;
	unsigned char oradius;			/* radius before scaling */
	int n, vis;				/* count, visibility */
	char name[2];				/* symbol name       */
};

struct LRot {
	unsigned short angle;
	unsigned short axis;
	struct LRot *link;
};

struct Pair {
	Node *a, *b;
	struct Pair *nxt;
};

#define announce(X) string(&defont,X,&display,add(Drect.origin, Pt(12,30)),F_STORE)
