typedef int boolean;
const boolean true = 1;
const boolean false = 0;

struct dag_label_t {
	int			type;	// STRING or DESC or 0 (no label)
	char*		value;	// macro name or drawing code

	dag_label_t(int intype,char *invalue) {
		type	=	intype;
		value	=	invalue;
	}
	dag_label_t()	{
		type	=	0;
		value	=	0;
	}
};

enum shape_id_t 	{Box,Square,Diamond,Circle,Doublecircle,Ellipse,Plaintext,User_defined};

struct shape_t {
	int			type;		// STRING or DESC
	char*		value;		// ascii name
	boolean		predefined;	// built-in type
	shape_id_t	shape_id;	// if predefined, else is clipping shape_id

	shape_t(int intype, char* invalue);
	boolean is_fixed_aspect_ratio();
};

enum	dag_ink_t	{solid_ink,dashed_ink,dotted_ink,invis_ink};

struct DAG_edge_t : public edge_t {
	char*		color;
	dag_label_t	label;		// label to put on the edge
	int			pointsize;
	int			flipped;	// true means backedge
	dag_ink_t	ink;		// solid_ink, ...

	DAG_edge_t();
	DAG_edge_t*	nextof() {return (DAG_edge_t*) next;}
	void		setlabel(int intype, char *invalue);
	void		setcolor(char* color);
	void		setweight(int inweight);
	void		setink(dag_ink_t inink);
	void		setpointsize(int inpointsize);
};

struct DAG_node_t : public node_t {
	int	 		pointsize;
	dag_label_t	label;
	shape_t		shape;
	char*		color;
	int			xsize;	// note: these do not depend on orientation,
	int			ysize;	//  but the width and height in node_t do.

	void		autosize();
	void		sethw();

	DAG_node_t();
	void		setname(char *invalue);
	void		setshape(int intype, char* invalue);
	void		setcolor(char *incolor);
	void		setlabel(int intype, char* invalue);
	void		setxsize(int inxsize);
	void		setysize(int inysize);
	void		setpointsize(int inpointsize);
};

enum output_lang_t	{Pic,Postscript,Graphdraw,Cip};

struct user_info_t {
	double		height,width;
	boolean		rotated;

	user_info_t() {
		height = width = 0.0;
		rotated = false;
	}
	void set_rotate()	{rotated = true;}
	void set_size(double w, double h) {
		 width = w; height = h;
	}
};

struct Infile {
	FILE	*fp;
	int   	line_number;
	char	*name;

	Infile() {
		name = strcpy(new char[2],"-");
		line_number = 0;
		fp = stdin;
	}
	Infile(char *p) {
		name = strcpy(new char[strlen(p)+1],p);
		line_number = 0;
		fp = fopen(name,"r");
	}
	char *gets(char*,int);
};

struct pair_t {
	int			node;
	pair_t		*next;

	pair_t	(int in_node, pair_t *in_next) {
			node = in_node;
			next = in_next;
	}
};

/* --- front end --- */
extern const int Resolution;	// points per virtual inch
extern const double IPP;	// real inches per point of type (NOT Resolution!)
extern const double Pi;

class shared_string_t {
	shared_string_t *next;
	char data[1];	// dynamic
	friend char* newstring(char*);
	friend void freestrings();
};
extern shared_string_t *shared_string_pool;

extern Infile		Current_file;
extern boolean		Uselib;		// use standard DAG graphics library
extern char*		Lib_path;	// library directory
extern int			N;
#define Init_extent	256
extern int			Extent;
extern DAG_node_t	**Node;
extern DAG_edge_t	**Edge;
extern user_info_t	User;
extern options_t	Options;
extern char*		Cmd_name;
extern output_lang_t	Output_type;
extern int			Xmax,Ymax;
extern Point		Page_size;
extern Point		Margin;

/* parser */
extern int			Syntax_error;
extern DAG_node_t	Reset_node,Default_node;
extern DAG_edge_t	Reset_edge,Default_edge;
int					yyparse();
int					yylex();
void				yyerror(char*s,char *s1="", char* s2="", char *s3="", char *s4="");

/* code generation */
void				make_drawing();
void				emit_pic();
void 				emit_ps();
void 				emit_graphdraw();
Point				find_edge_midpoint(DAG_edge_t *e);
Point				find_nodeport(int node, Point p0, Point p1);
void				cat_libfile(char*);
void				unsquirrel();

/* util */
Infile				nextfile(char** &argv);
void				reclaim_hashtable();
void				set_output_type(char*);
void				set_page_size(char*);
options_t			reset_options();
void				set_bounds(double,double);
int					node_lookup(char*);

void				dumpgraph();

/* useful inline functions */
overload sign;
static inline int sign (int i) {return (i == 0? 0 : (i > 0 ? 1: - 1));}
static inline int sign (double i) {return (i == 0? 0 : (i > 0 ? 1: - 1));}
static inline double inchof(int i) { return (double)i / Resolution; }
static inline double distance(double x1, double y1, double x2, double y2)
	{return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));}
static inline int between (int i1, int i2, int i3) {
	return ((i1 <= i2) && (i2 <= i3)) || ((i1 >= i2) && (i2 >= i3));
}
static inline int round(double d) { return( d > 0 ? (int)(d + .5) : (int)(d - .5)); }
