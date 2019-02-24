union M {
	long	s;
};
main()
{
	register long u;
	register union M *m;

	u = (m->s &= 0xFFFFFF);
}
