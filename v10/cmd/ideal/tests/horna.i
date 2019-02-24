.IS
...libfile circle
triangle
{
var a, b, c, d, e;
a = 0;
b = c/2;
c = (0,2);
d = (-1,1);
e = d/2;
bdlist = a, ^e, b, c, ^d;
}
.IE
.IS
main {
put circle {
radius = .5;
center = (0,.5);
}
put triangle {
opaque interior;
}
'version 0' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .5;
center = (0,.5);
}
put triangle {
opaque exterior;
}
'version 1' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .75;
center = (0,.5);
}
put triangle {
opaque interior;
}
'version 2' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .75;
center = (0,.5);
}
put triangle {
opaque exterior;
}
'version 3' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .25;
center = (0,-.5);
}
put triangle {
opaque interior;
}
'version 4' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .25;
center = (0,-.5);
}
put triangle {
opaque exterior;
}
'version 5' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .5;
center = (0,-.5);
}
put triangle {
opaque interior;
}
'version 6' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .5;
center = (0,-.5);
}
put triangle {
opaque exterior;
}
'version 7' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .5;
center = (0,1);
}
put triangle {
opaque interior;
}
'version 8' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .5;
center = (0,1);
}
put triangle {
opaque exterior;
}
'version 9' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = 1;
center = (0,1);
}
put triangle {
opaque interior;
}
'version 10' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = 1;
center = (0,1);
}
put triangle {
opaque exterior;
}
'version 11' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .25;
center = (-.5,1.5);
}
put triangle {
opaque interior;
}
'version 12' at (0,-4);
}
.IE
.IS
main {
put circle {
radius = .25;
center = (-.5,1.5);
}
put triangle {
opaque exterior;
}
'version 13' at (0,-4);
}
.IE
