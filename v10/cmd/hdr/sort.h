typedef struct
{
	union
	{
		cnode	*so_c;
		xnode	*so_x;
	}
		so_u;
	long	so_value;
}
	sorts;

extern sorts	*sort_vect;
extern int	sort_size;
extern int	sort_index;
