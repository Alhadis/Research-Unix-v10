pen
.nf
.IS
box wavy{
    /* perp is a unit vector normal
       to (end - start) */
    var start, end,
        ht, perp, pt1, pt2;
    pt1 = start + (end - start)/4 + ht*perp;
    pt2 = start + 3/4*(end - start) - ht*perp;
    perp = (0,1)*(end - start)/abs(diff);
	var diff;
	diff = end - start;
    conn start to pt1;
    conn pt1 to pt2;
    conn pt2 to end;
    }

box main{
    var a,b,c,d;
    a = (0.0,-4.5);
    b = a + 1;
    c = b + (1,1);
    d = c + 1;
    conn a to b;
    conn b to c using 5*int(abs(b-c)) wavy {ht = 0.05;} <start,end>;
    conn c to d;
    }
.IE
.IS
...forget wavy
.IE
