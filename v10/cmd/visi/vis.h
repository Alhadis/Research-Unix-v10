/*
 *      vis.h 1.3
 *
 *	Header file for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/1/82 11:14:53
 *      Retrieved 11/15/82 13:22:55
 */
#define	HELPFILE	"/usr/lib/vis.help"

#define	DEFWIDTH	10
#define	DEFSCALE	2

#define loop	for(;;)
#define	copystr(x)	strcpy(malloc((unsigned)(strlen(x)+1)),x)
#define	NODE(x)	(struct x *)malloc(sizeof(struct x))
#define multnode(type,num)      (struct type *)malloc((unsigned)(sizeof(struct type)*num))

char	*malloc(), *strcpy(), *getexp(), *foldup(), *realloc();
char	*dtoa(), *getfn(), *mktemp(), *getenv(), *collect();
char	*cannon(), *strcat(), *strchr(), *makevar(), *strncpy();
double	atof();
void	pinit(), lexinit(), startlex(), scrinit(), prheading();
void	hashenter(), hashinit(), qadd(), qempty(), qinit();
struct node	*qread(), *newnode(), mathop();
struct node	*getnode();
struct qheader	*qcopy();

#ifndef TRUE
#define	TRUE	(1)
#define	FALSE	0
#endif
#define BIG		123456789E35
/*
 *	Structures Needed for the Program
 */
enum rtntyp { UNDEF, UNRES, NUM, STRING };

/*
 *	Structures required for the hashing
 */

struct hashentry {
        char    *symbol;
	int	key;
	int	value;
};

struct hashhdr {
        int     size;
        int     entry;
        struct hashentry        *table;
};

struct reswd {
	char	*name;
	int	value;
};

/*
 *	Structures required for the queues
 */

struct qheader {
	struct	qentry	*first;
	struct	qentry	*last;
};

struct qentry {
	struct node	*nodeptr;
	struct qentry	*next;
};

/*
 *	Return structure from lexical analysis
 */

struct vdef {
	int	row, col;
	char	*tval;
};

/*
 *	Parsing Structures
 */

struct range {
	int	fromrow, fromcol;
	int	torow, tocol;
};

struct colstat {
	int	colnum;
	int	number
};

/*
 *	Structures required to keep track of the nodes
 */

struct rowhdr {
	int	size;
	struct rowlabel	**table;
};

struct colhdr {
	int	size;
	struct collabel	**table;
};

struct rowlabel {
	int	rownum;
	int	position;
	struct node	*next;
};

struct collabel {
	int	colnum;
	int	position;
	int	cell;
	int	width, scale;
	int	size;
};

/*
 *	NODE STRUCTURE
 */
struct	node {
	int	row, col;
	enum rtntyp	type;
	double	value;
	char	*def;
	char	*svalue;
	struct node	*next;
	struct qheader	depend;
};
