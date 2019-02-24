typedef struct cfrag	cfrag;
typedef struct choice	choice;
typedef struct cnode	cnode;
typedef struct decl	decl;
typedef struct ident	ident;
typedef struct level	level;
typedef struct object	object;
typedef struct operand	operand;
typedef struct optinfo	optinfo;
typedef struct st_node	st_node;
typedef struct tkdope	tkdope;
typedef struct token	token;
typedef struct xnode	xnode;
typedef union data	data;

union data
{
	cfrag	*k;
	cnode	*c;
	double	f;
	ident	*d;
	long	i;
	object	*obj;
	st_node	*n;
	token	*t;
	xnode	*x;
	struct
	{
		long	from;
		long	to;
	}
		r;
	struct
	{
		st_node	*no_name;
		long	no_offset;
	}
		no;
	struct
	{
		char	*str_value;
		int	str_size;
	}
		s;
};
