int x;  float f;
struct {
     int  d;
     int  x;
} s, *sp;

main() {
     sp->x = x + f * x;
}
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
  struct _Point {
	int	x;
	int	y;
};
struct _Rectangle {
	  struct _Point	ll;
	  struct _Point	ur;
};
  struct _Object {
	  struct _Object	*link;
};
  struct _Pool {
	  struct _Object	*freelist;
	  struct _Object	*arealist;
	int	objsize;
};
  struct _Vpool {
	  struct _Object	*next;
	  struct _Object	*arealist;
	int	left;
	int	quantum;
};
	  struct _SEQFILE {
		int	hidden;
	};
extern char *myname;
extern int nullstring;
extern char *stringtab;
extern char *execname();
extern struct _iobuf *chkfopen();
extern   struct _Pool *newpool();
extern   struct _Vpool *newvpool();
extern char *maketemp();
extern char *permspace();
extern   struct _SEQFILE *seqopen();
extern char *nettoname();
extern char *strchr();
extern char *strrchr();
extern char debug;
typedef struct {
	float	x;
	float	y;
} FPT;
typedef struct {
	FPT	ll;
	FPT	ur;
} FRECT;
  struct _Traphd {
	int magicnumb;
	int nrows, ncols;
	int xdelta, ydelta;	
	struct _Rectangle bb;		
	struct _Rectangle rbb ;		
	char empty;
};
struct _Trapezoid {
	int net;
	int xmin, xmax, lby, lty, rby, rty;
};
struct _Traplabel {
	int net;
	int x, y;
	char label[4*sizeof(int)];
};
extern char intrflag;
extern   struct _Point center;
extern struct _Rectangle iuwind;
extern double supiu;
extern char debug;
extern short netcolor;
extern char debug;
int iuplu = 	1000;
short nlayers = 40;
short color [40];
char *layername [40];
char *colorfile;
char *fullname [40];
static   struct _Traphd *header;
static int **index;
static int identify (i, p)
  struct _Point p;
{
	register   struct _Traphd *h;
	register struct _iobuf *f;
	int col, row, pos, addr, stop;
	float w, x, slope;
	struct _Trapezoid t;
	h = &header[i];
	col = 	(((p.x) - ( h)->rbb.ll.x) / ( h)->xdelta);
	row = 	(((p.y) - ( h)->rbb.ll.y) / ( h)->ydelta);
	if (col < 0 || col >= h->ncols) return -1;
	if (row < 0 || row >= h->nrows) return -1;
	pos = col * h->nrows + row;
	addr = index[i][pos];
	stop = index[i][pos + 1];
	if (addr >= stop) return -1;
	f = (struct _iobuf *) forceopen(i);
	fseek (f, addr, 0);
	while (addr < stop) {
		if (intrflag) return -1;
		chkfread (&t, sizeof(struct _Trapezoid), 1, f);
		addr += sizeof(struct _Trapezoid);
		if (p.x < t.xmin || t.xmax < p.x) continue;
		x = p.x - t.xmin;
		w = t.xmax - t.xmin;
		
		  slope = (t.rby - t.lby) / w;
		  if (p.y < t.lby + x*slope) continue;
		
		  slope = (t.rty - t.lty) / w;
		  if (p.y > t.lty + x*slope) continue;
		
		return t.net;
	}
	return -1;
}
m()
{	register unsigned char a, b;
	a/=b;
}
