extern	WR_IMAGE, WR_CMD, WR_RLE, RD_CMD, RD_IMAGE;
#define	TOGGLE	(8<<1)

#ifdef	DEFINING
int	WR_IMAGE	=(1<<1);
int	WR_CMD		=(4<<1);
int	WR_RLE		=(5<<1);	
int	RD_CMD		=(6<<1);
int	RD_IMAGE	=(7<<1);
#endif

#define	CVSETDMA	(('c'<<8)|1)

typedef struct Point{
	int x, y;
}Point;
typedef struct Rectangle{
	Point origin, corner;
}Rectangle;
#define	Pt(x, y)	(x), (y)
#define	Rect(a, b, c, d)	(a), (b), (c), (d)
#define	Rpt(a, b)	(a), (b)

extern int	cv;
