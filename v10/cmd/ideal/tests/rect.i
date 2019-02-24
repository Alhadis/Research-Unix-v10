rect
.nf
.IS
box rectangle{
    var se, sw,
        ne, nw,
        width, height;
    se = sw + width;
    nw = sw + (0,1)*height;
    ne = nw + width;
    conn sw to se;
    conn se to ne;
    conn ne to nw;
    conn nw to sw;
    }

box main{
    A:put rectangle{
	height = 1.75;
	width = 1.25;
	sw = (4.1, -1.5);
	};
    B:put rectangle{
	height = 1;
	width = 1.5;
	sw = (4,-1);
	opaque ;
	bdlist = sw, nw, ne, se;
	};
    spline A.nw to B.nw to A.ne to B.ne to A.sw to B.sw to A.se to B.se;
    }
.IE
