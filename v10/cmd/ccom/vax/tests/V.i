struct x {
	int a,b;
};
ptmatch()
{
	register struct x *p;
	if( psize(p->b) != psize(p->b) )
		;
}
