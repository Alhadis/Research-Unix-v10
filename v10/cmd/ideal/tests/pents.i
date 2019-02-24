pents
.nf
.IS
...degrees
box pentagon
{
	var pt1, pt2, pt3, pt4, pt5;
	var radius, center;
	pt1 = center + radius;
	pt2 = center + cis (72)*radius;
	pt3 = center + cis (144)*radius;
	pt4 = center + cis (-144)*radius;
	pt5 = center + cis (-72)*radius;
	conn pt1 to pt2;
	conn pt2 to pt3;
	conn pt3 to pt4;
	conn pt4 to pt5;
	conn pt5 to pt1;
	bdlist = 1.1[center, pt1],
		1.1[center, pt2],
		1.1[center, pt3],
		1.1[center, pt4],
		1.1[center, pt5]
	;
}
main
{
	put P1:pentagon{center = (1,0); radius = (0,0.25);};
	put P2:pentagon{pt3 = P1.pt4; radius = (0,0.5);opaque;};
	put P3:pentagon{pt3 = P2.pt4; radius = (0,0.75);opaque;};
	put P4:pentagon{pt3 = P3.pt4; radius = (0,1.0);opaque;};
}
.IE
.IS
...forget pentagon
.IE
