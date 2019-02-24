main()
{ unsigned short a, b;
	a = b ? 1 : 0;
}
struct vertex { double x, y;};
double dist();
flip(v)
register struct vertex *v;
{
	struct vertex p;
	if(dist(p.x, p.y)<dist(v->x, v->y))
		return(1);
	return(0);
}
f()
{
  double x[10], step;
  int n;
  step = (x[n-1]-x[0])/(n-1);
}
eval(n,x,f,d,m,p,g)
  int n,m;
  double x[], f[], d[], p[], g[];
{	int i, j;
  g[i] = f[j] + (f[j+1]-f[j])*(p[i]-x[j])/(x[j+1]-x[j]);
}
