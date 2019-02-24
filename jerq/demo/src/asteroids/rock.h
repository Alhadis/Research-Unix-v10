typedef struct Rock{
	Point	x;	/* position */
	Point	v;	/* velocity */
	int	i;	/* picture index of rock */
	int	r;	/* radius of rock */
	int 	hash;	/* hash of position for fast collision detection */
}Rock;
Rock *collision();
extern Rock rock[];

#undef	Pt
extern Point Pt();
#undef	transform
extern Point transform();
#undef	rtransform
extern Rectangle rtransform();
