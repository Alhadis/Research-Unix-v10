opqcirc
.nf
.IS
...libfile circle
main{
	conn -3 to 3;
	put circle {
		center = 0;
		radius = 0.5;
	};
	put circle {
		center = 0.5;
		radius = 1;
	};
	put circle {
		center = 3;
		radius = 1;
	};
	put circle {
		center = 0;
		radius = 1;
		opaque;
	};
}
.IE
