union data { double	f; };
struct xnode { union { struct xnode	*xu_xnd; } x_u1;
	struct xnode	*x_right;
	union data	x_value;
};
main()
{	struct xnode	*x;
	*x = *x->x_right->x_u1.xu_xnd;
}
