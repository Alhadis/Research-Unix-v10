a()
{	register int c;
	register int *p, *q;
	while((*p++ = *q++));
	while((*p++ = *q++) == '\n');
	c = -(*p++ = *q++);
	c = -(p[c]=q[c]);
}
