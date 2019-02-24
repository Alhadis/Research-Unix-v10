boxgrid
.nf
.IS
...minx -1.5
...maxy 1.5
...maxx 1.5
...miny -1.5
gridline {
	var a,b;
	var neg, pos;
	conn 0.5*(a+b) - neg to 0.5*(a+b) + pos;
}
rect {
	var ne, nw, sw, se;
	var ht, wd;
	var cen;
	se = sw + wd;
	ne = nw + wd;
	ne = se + (0,1)*ht;
	bdlist = ne, nw, sw, se;
	cen = 0.5*(ne+sw);
}
main{
	conn (0,-1) to (0,1) using 5 gridline {neg = 1; pos = 1;} <a,b>;
	conn (-1,0) to (1,0) using 5 gridline {neg = (0,1); pos = (0,1);} <a,b>;
	put rect{
		ht = wd = 0.5;
		cen = (0,-0.5);
		opaque;
	};
	put rect{
		ht = wd = 0.5;
		cen = (0,0.5);
		opaque;
	};
	put rect{
		ht = wd = 0.5;
		cen = 0.5;
		opaque;
	};
	put rect{
		ht = wd = 0.5;
		cen = -0.5;
		opaque;
	};
}
.IE
.IS
...forget rect gridline
.IE
