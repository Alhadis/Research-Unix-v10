extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;
	char	*_base;
	short	_flag;
	char	_file;
} _iob[20];
struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*fgets();
typedef struct Triple{
	float	x;
	float	y;
	float	z;
}Triple;
typedef Triple Color;
typedef struct Point {
	int type;
	long stamp;		
	double x;
	double y;
	double z;
} Point;
typedef struct Plane {
	int type;
	long stamp;
	double A;
	double B;
	double C;
	double D;
} Plane;
typedef struct Conic {
	int type;
	long stamp;
	double A;
	double B;
	double C;
	double D;
	double E;
	double F;
	double G;
	double H;
	double I;
	double J;
} Conic;
typedef struct Transform {
	double A, B, C, D;
	double E, F, G, H;
	double I, J, K, L;
} Transform;
typedef struct Tpair {
	int type;
	long stamp;
	Transform t;
	Transform tinv;
} Tpair;
typedef struct Ctree {
	int type;
	int (*test)();
	struct Ctree *car;
	struct Ctree *cdr;
} Ctree;
typedef struct Surface {
	int type;
	short flags;
	Triple n;	
	short nfacet;
	struct{
		float m;
		float frac;
	}facet[5		];
	Triple *Dtab;
	Color speccolor;
	Color diffcolor;
	Color ambcolor;
	Color clearcolor;
} Surface;
typedef struct Object {
	int type;
	union {
		Plane *plane;
		Conic *conic;
	} surface;
	Conic *cover;
	Point *center;
	struct List *references;
	Ctree *ctree;
	Tpair *tpair;
	Surface *srf;
	float minrange1, minrange2;
	float Q1, R1, S1, T1;
	float Q2, R2, S2, T2;
	int (*fun)();
} Object;
typedef struct View {
	int type;
	Tpair *tpair;
	int style;
	Color lightcolor;
	Point *viewpoint;
	Point *ul_corner, *ll_corner, *lr_corner;	
	Plane *screen;
	struct List *bin[32		][32		];	
} View;
typedef struct List {
	int type;
	union {
		Point *point;
		Object *object;
		View *view;
		struct List *list;
	} car;
	struct List *cdr;
} List;
typedef struct Symbol{
	char	*name;
	short	type;
	union{
		double 	val;
		List	*obj;
		Tpair	*tpair;
		Triple	*triple;
		Surface	*srf;
	}u;
	struct Symbol *next;
}Symbol;
extern double	range1;
extern double	range2;
extern long	transform_stamp;
extern Color	black;
extern Color	fillcolor;
extern Color	white;
extern List	*scene;
extern Point	testpoint;
extern Object	*pobject;
extern Point	dirnums;
extern Transform tnull;
extern View	*camera;
extern List	*lightsource;
extern char	*progname;
char		*emalloc();
double		Pow();
double		setvalue();
double		value();
double		fsqrt();
Color		*create_color();
Color		light();
Ctree		*and();
Ctree		*create_ctree();
Ctree		*inside_list();
Ctree		*outside_list();
List		*cons_list();
Object		*adddefn();
Object		*boolean();
Object		*copy();
Object		*inside();
Object		*outside();
Object		*setobjvalue();
Object		*unit_cone();
Object		*unit_cube();
Object		*unit_disc();
Object		*unit_cylinder();
Object		*unit_hyperboloid();
Object		*unit_paraboloid();
Object		*unit_paracylinder();
Object		*unit_saddle();
Object		*unit_sphere();
Object		*unit_square();
Point		intersect();
Point		min_range();
Point		trans_point();
Surface		*create_surface();
Symbol		*install();
Symbol		*lookup();
Tpair		*create_tpair();
Tpair		*mulmat();
Tpair		*matvalue();
Tpair		*setmatvalue();
Transform	trans_product();
Triple		*trivalue();
Triple		*settrivalue();
Triple		*create_triple();
View		*create_view();
long transform_stamp = 1;
extern double sin(), cos();
Transform tnull = {1.0, 0.0, 0.0, 0.0,
		   0.0, 1.0, 0.0, 0.0,
		   0.0, 0.0, 1.0, 0.0,};
Transform
trans_product(t1, t2)
register Transform *t1, *t2;
{
	Transform t3;
	t3.A = t1->A * t2->A + t1->B * t2->E + t1->C * t2->I;
	t3.B = t1->A * t2->B + t1->B * t2->F + t1->C * t2->J;
	t3.C = t1->A * t2->C + t1->B * t2->G + t1->C * t2->K;
	t3.D = t1->A * t2->D + t1->B * t2->H + t1->C * t2->L + t1->D;
	t3.E = t1->E * t2->A + t1->F * t2->E + t1->G * t2->I;
	t3.F = t1->E * t2->B + t1->F * t2->F + t1->G * t2->J;
	t3.G = t1->E * t2->C + t1->F * t2->G + t1->G * t2->K;
	t3.H = t1->E * t2->D + t1->F * t2->H + t1->G * t2->L + t1->H;
	t3.I = t1->I * t2->A + t1->J * t2->E + t1->K * t2->I;
	t3.J = t1->I * t2->B + t1->J * t2->F + t1->K * t2->J;
	t3.K = t1->I * t2->C + t1->J * t2->G + t1->K * t2->K;
	t3.L = t1->I * t2->D + t1->J * t2->H + t1->K * t2->L + t1->L;
	return t3;
}
Conic
trans_conic(t, c)
register Transform *t;
register Conic *c;
{
	Conic nc;
	nc.type = 3;
	nc.A =	(c->A*t->A*t->A + c->B*t->E*t->E + c->C*t->I*t->I) +
		(c->D*t->A*t->E + c->E*t->E*t->I + c->F*t->A*t->I);
	nc.B =	(c->A*t->B*t->B + c->B*t->F*t->F + c->C*t->J*t->J) +
		(c->D*t->B*t->F + c->E*t->F*t->J + c->F*t->B*t->J);
	nc.C =	(c->A*t->C*t->C + c->B*t->G*t->G + c->C*t->K*t->K) +
		(c->D*t->C*t->G + c->E*t->G*t->K + c->F*t->C*t->K);
	nc.D =	(c->A*t->A*t->B + c->B*t->E*t->F + c->C*t->I*t->J)*2.0 +
		c->D*(t->A*t->F + t->B*t->E) +
		c->E*(t->E*t->J + t->F*t->I) +
		c->F*(t->A*t->J + t->B*t->I);
	nc.E =	(c->A*t->B*t->C + c->B*t->F*t->G + c->C*t->J*t->K)*2.0 +
		c->D*(t->B*t->G + t->C*t->F) +
		c->E*(t->F*t->K + t->G*t->J) +
		c->F*(t->B*t->K + t->C*t->J);
	nc.F =	(c->A*t->A*t->C + c->B*t->E*t->G + c->C*t->I*t->K)*2.0 +
		c->D*(t->A*t->G + t->E*t->C) +
		c->E*(t->E*t->K + t->G*t->I) +
		c->F*(t->A*t->K + t->C*t->I);
	nc.G =	(c->A*t->A*t->D + c->B*t->E*t->H + c->C*t->I*t->L)*2.0 +
		c->D*(t->A*t->H + t->D*t->E) +
		c->E*(t->E*t->L + t->H*t->I) +
		c->F*(t->A*t->L + t->D*t->I) +
		c->G*t->A + c->H*t->E + c->I*t->I;
	nc.H =	(c->A*t->B*t->D + c->B*t->F*t->H + c->C*t->J*t->L)*2.0 +
		c->D*(t->B*t->H + t->D*t->F) +
		c->E*(t->F*t->L + t->H*t->J) +
		c->F*(t->B*t->L + t->D*t->J) +
		c->G*t->B + c->H*t->F + c->I*t->J;
	nc.I =	(c->A*t->C*t->D + c->B*t->G*t->H + c->C*t->K*t->L)*2.0 +
		c->D*(t->C*t->H + t->D*t->G) +
		c->E*(t->G*t->L + t->H*t->K) +
		c->F*(t->C*t->L + t->D*t->K) +
		c->G*t->C + c->H*t->G + c->I*t->K;
	nc.J =	(c->A*t->D*t->D + c->B*t->H*t->H + c->C*t->L*t->L) +
		(c->D*t->D*t->H + c->E*t->H*t->L + c->F*t->D*t->L) +
		c->G*t->D + c->H*t->H + c->I*t->L +
		c->J;
	return nc;
}
Plane
trans_plane(t, p)
register Transform *t;
register Plane *p;
{
	Plane np;
	np.type = 2;
	np.A = p->A*t->A + p->B*t->E + p->C*t->I;
	np.B = p->A*t->B + p->B*t->F + p->C*t->J;
	np.C = p->A*t->C + p->B*t->G + p->C*t->K;
	np.D = p->A*t->D + p->B*t->H + p->C*t->L + p->D;
	return np;
}
Point
trans_point(t, p)
register Transform *t;
register Point *p;
{
	Point np;
	np.type = 1;
	np.x = t->A*p->x + t->B*p->y + t->C*p->z + t->D;
	np.y = t->E*p->x + t->F*p->y + t->G*p->z + t->H;
	np.z = t->I*p->x + t->J*p->y + t->K*p->z + t->L;
	return np;
}
do_transform(tp, list)
Tpair *tp;
register List *list;
{
	if (!list)
		return;
	switch (list->type) {
	case 1:
		if (((Point *)list)->stamp == tp->stamp)
			return;
		*((Point *)list) = trans_point(&tp->t, ((Point *)list));
		((Point *)list)->stamp = tp->stamp;
		return;
	case 3:
		if (((Conic *)list)->stamp == tp->stamp)
			return;
		*((Conic *)list) = trans_conic(&tp->tinv, ((Conic *)list));
		((Conic *)list)->stamp = tp->stamp;
		return;
	case 2:
		if (((Plane *)list)->stamp == tp->stamp)
			return;
		*((Plane *)list) = trans_plane(&tp->tinv, ((Plane *)list));
		((Plane *)list)->stamp = tp->stamp;
		return;
	case 5:
		do_transform(tp, list->car.list);
		do_transform(tp, list->cdr);
		return;
	case 7:
		do_transform(tp, ((Ctree *)list)->car);
		do_transform(tp, ((Ctree *)list)->cdr);
		return;
	case 4:
		do_transform(tp, ((View *)list)->viewpoint);
		do_transform(tp, ((View *)list)->ul_corner);
		do_transform(tp, ((View *)list)->ll_corner);
		do_transform(tp, ((View *)list)->lr_corner);
		do_transform(tp, ((View *)list)->screen);
		do_transform(tp, ((View *)list)->tpair);
		return;
	case 6:
		do_transform(tp, ((Object *)list)->surface);
		do_transform(tp, ((Object *)list)->cover);
		do_transform(tp, ((Object *)list)->references);
		do_transform(tp, ((Object *)list)->ctree);
		do_transform(tp, ((Object *)list)->tpair);
		do_transform(tp, ((Object *)list)->center);
		return;
	case 8:
		if (((Tpair *)list)->stamp == tp->stamp)
			return;
		((Tpair *)list)->t = trans_product(&tp->t, &((Tpair *)list)->t);
		((Tpair *)list)->tinv = trans_product(&((Tpair *)list)->tinv, &tp->tinv);
		((Tpair *)list)->stamp = tp->stamp;
	}
	return;
}
translate(x, y, z, list)
double x, y, z;
List *list;
{
	Tpair tp;
	tp.stamp = transform_stamp++;
	tp.type = 8;
	tp.t = tnull;
	tp.t.A = 1.0;
	tp.t.F = 1.0;
	tp.t.K = 1.0;
	tp.t.D = x;
	tp.t.H = y;
	tp.t.L = z;
	tp.tinv = tp.t;
	tp.tinv.D = -x;
	tp.tinv.H = -y;
	tp.tinv.L = -z;
	do_transform(&tp, list);
}
stretch(x, y, z, list)
double x, y, z;
List *list;
{
	Tpair tp;
	tp.stamp = transform_stamp++;
	tp.type = 8;
	tp.t = tnull;
	tp.t.A = x;
	tp.t.F = y;
	tp.t.K = z;
	tp.tinv = tnull;
	tp.tinv.A = 1.0/x;
	tp.tinv.F = 1.0/y;
	tp.tinv.K = 1.0/z;
	do_transform(&tp, list);
}
rotateX(theta, list)
double theta;
List *list;
{
	Tpair tp;
	tp.stamp = transform_stamp++;
	tp.type = 8;
	theta = theta * 0.0174532925199432957692	;
	tp.t = tnull;
	tp.t.A = 1.0;
	tp.t.F = cos(theta);
	tp.t.G = -sin(theta);
	tp.t.J = -tp.t.G;
	tp.t.K = tp.t.F;
	tp.tinv = tp.t;
	tp.tinv.G = -tp.tinv.G;
	tp.tinv.J = -tp.tinv.J;
	do_transform(&tp, list);
}
rotateY(theta, list)
double theta;
List *list;
{
	Tpair tp;


	tp.stamp = transform_stamp++;
	tp.type = 8;
	theta = theta * 0.0174532925199432957692	;
	tp.t = tnull;
	tp.t.F = 1.0;
	tp.t.A = cos(theta);
	tp.t.C = -sin(theta);
	tp.t.I = -tp.t.C;
	tp.t.K = tp.t.A;
	tp.tinv = tp.t;
	tp.tinv.C = -tp.tinv.C;
	tp.tinv.I = -tp.tinv.I;
	do_transform(&tp, list);
}
rotateZ(theta, list)
double theta;
List *list;
{
	Tpair tp;


	tp.stamp = transform_stamp++;
	tp.type = 8;
	theta = theta * 0.0174532925199432957692	;
	tp.t = tnull;
	tp.t.K = 1.0;
	tp.t.A = cos(theta);
	tp.t.B = -sin(theta);
	tp.t.E = -tp.t.B;
	tp.t.F = tp.t.A;
	tp.tinv = tp.t;
	tp.tinv.B = -tp.tinv.B;
	tp.tinv.E = -tp.tinv.E;
	do_transform(&tp, list);
}
Conic
tangent_cone(p, c)
register Point *p;
register Conic *c;
{
	Conic e;
	double f4, fx, fy, fz, fw;
	
	f4 = (p->x*(c->A*p->x + c->D*p->y + c->F*p->z + c->G) +
	      p->y*(c->B*p->y + c->E*p->z + c->H) +
	      p->z*(c->C*p->z + c->I) + c->J) * 4.0;
	e.A = f4*c->A;
	e.B = f4*c->B;
	e.C = f4*c->C;
	e.D = f4*c->D;
	e.E = f4*c->E;
	e.F = f4*c->F;
	e.G = f4*c->G;
	e.H = f4*c->H;
	e.I = f4*c->I;
	e.J = f4*c->J;
	
	fx = 2.0*c->A*p->x + c->D*p->y + c->F*p->z + c->G;
	fy = 2.0*c->B*p->y + c->D*p->x + c->E*p->z + c->H;
	fz = 2.0*c->C*p->z + c->E*p->y + c->F*p->x + c->I;
	fw = 2.0*c->J	   + c->G*p->x + c->H*p->y + c->I*p->z;
	
	e.A -= fx*fx;
	e.B -= fy*fy;
	e.C -= fz*fz;
	e.J -= fw*fw;
	e.D -= 2.0*fx*fy;
	e.E -= 2.0*fy*fz;
	e.F -= 2.0*fz*fx;
	e.G -= 2.0*fx*fw;
	e.H -= 2.0*fy*fw;
	e.I -= 2.0*fz*fw;
	return e;
}
