norm
.nf
.IS
...minx -1.5
...maxy 1.5
...maxx 1.5
...miny -1.5
...libfile hole
gridline {
	var a,b;
	var neg, pos;
	conn 0.5*(a+b) - neg to 0.5*(a+b) + pos;
}
main{
	conn (1,0) to (1,-1) using 10 gridline {neg = 1; pos = 0;} <a,b>;
	conn (0,-1) to (1,-1) using 10 gridline {neg = 0; pos = (0,1);} <a,b>;
	put hole{
		radius = 1;
		center = (0,0);
		opaque exterior;
	};
}
.IE
.IS
...forget gridline
.IE
