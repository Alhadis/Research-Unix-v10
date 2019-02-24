.G1
frame ht 1.5 wid 1.5
define square { ($1)*($1) }
define root { ($1^0.5) }
define P {
	times at i, square(i); i = i+1
	circle at j, root(j); j = j+5
}
i = 1; j = 5
P; P; P; P; P
.G2
