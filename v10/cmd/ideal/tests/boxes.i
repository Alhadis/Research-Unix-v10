.IS
...libfile rect
main {
	conn (-2,-2) to (2,2);
	put rect {
		sw = (-1,-1);
		ht = 1.5;
		wd = 1/2;
		opaque interior;
	};
}
.IE
.IS
...libfile rect
main {
	conn (2,2) to (-2,-2);
	put rect {
		sw = (-1,-1);
		ht = 1.5;
		wd = 1/2;
		opaque exterior;
	};
}
.IE
.IS
...libfile rect
main {
 conn (-2,-1) to (2,-1);
	conn -2 to 2;
	conn (-2,1) to (2,1);
	put rect {
		sw = (-1,-1);
		ht = 1.5;
		wd = 1/2;
		opaque interior;
	};
}
.IE
.IS
...libfile rect
...width 0.5
main {
	conn (-1,-2) to (-1,2);
	put rect {
		sw = (-1,-1);
		ht = 1.5;
		wd = 1/2;
		opaque interior;
	};
}
.IE
