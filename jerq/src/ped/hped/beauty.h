#include "hped.h"
#include <math.h>
#ifndef PI
#define PI 3.1415926536
#endif

#define SLOPE 1

#define EXTENSION 2

#define EQUAL 3

#define SAMEX 7
#define SAMEY 8

#define SIDE_BUF 2048

#define DELTA 0.2
/*	in radians. that's 11.5 degrees	*/
#define DELTA1 0.1
/*	about 6 degrees		*/

#define SMALL_MV 5.0

#define	QPI	(PI/4)
#define HPI (PI/2)
#define	TPI	(3*PI/4)

extern Point * row;	/* where a copy of all coordinates is stored */
extern short  * offrow;	/* where ith objets starts in row[] */
extern short  * nrow;	/* obj[]->n  */
extern Point * mv_tol;	/* where maximum allowed dx, dy are stored */

/*
	this structure stores (in t) parameter used
	for clustering. it also contains pointers to
	original points (through `i1' and `i2')
*/

typedef struct sides{
	Point * p1, * p2;	/* pointers to row[] */
	double s;	/* slope */
	double l;	/* length */
	double t;	/* number used for clustering */
	short nu;	/* do not use in equations if set */
	}	sides, * sideP;

extern sides * stt;

extern sideP * stp;	/* pointers to stp for processing by cluster1() */
extern sideP * cmem;	/* store members of clusters */
extern sideP ** lp;

extern sides * srow;

typedef struct clust {
	sideP *mem;
	short n;
	double val;
	} clust, * clustP;

extern clust * cls;

/*
	structure for constructing equations
	if eqp is a pointer to a member, we have the following
	production (for now)
	((eqp->s1)->p1)->y - ((eqp->s1)->p2)->y
	= ((eqp->s1)->s)*( ((eqp->s1)->p1)->x - ((eqp->s1)->p2)->x )
*/

typedef struct eqn {
	sideP s1, s2;
	short u;	/* type of equation */
	double penalty;	/* to be filled by solver*/
	} eqnS, eqnP;

extern eqnS * eq;

extern FILE *fq;

extern short detail;

extern double smalls, bigs;
