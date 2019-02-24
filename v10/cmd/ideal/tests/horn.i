.IS
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
conn -1 to 1;
'version 0' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn -1 to 1;
'version 1' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn 1 to -1;
'version 2' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn 1 to -1;
'version 3' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,-1) to (0,3);
'version 4' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,-1) to (0,3);
'version 5' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,3) to (0,-1);
'version 6' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,3) to (0,-1);
'version 7' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,-1) to (0,2);
'version 8' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,-1) to (0,2);
'version 9' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,2) to (0,-1);
'version 10' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,2) to (0,-1);
'version 11' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,-1) to (0,1);
'version 12' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,-1) to (0,1);
'version 13' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,1) to (0,-1);
'version 14' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,1) to (0,-1);
'version 15' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,-1) to (0,.5);
'version 16' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,-1) to (0,.5);
'version 17' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,.5) to (0,-1);
'version 18' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,.5) to (0,-1);
'version 19' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn 0 to (0,1);
'version 20' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn 0 to (0,1);
'version 21' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,1) to 0;
'version 22' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,1) to 0;
'version 23' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn 0 to (0,2);
'version 24' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn 0 to (0,2);
'version 25' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,2) to 0;
'version 26' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,2) to 0;
'version 27' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (-2,1) to (1,1);
'version 28' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (-2,1) to (1,1);
'version 29' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (1,1) to (-2,1);
'version 30' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (1,1) to (-2,1);
'version 31' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (-1,1) to (1,1);
'version 32' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (-1,1) to (1,1);
'version 33' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (1,1) to (-1,1);
'version 34' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (1,1) to (-1,1);
'version 35' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (-1,1) to (0,1);
'version 36' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (-1,1) to (0,1);
'version 37' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,1) to (-1,1);
'version 38' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,1) to (-1,1);
'version 39' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (-1,2) to (1,2);
'version 40' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (-1,2) to (1,2);
'version 41' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (1,2) to (-1,2);
'version 42' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (1,2) to (-1,2);
'version 43' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (-1,2) to (0,2);
'version 44' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (-1,2) to (0,2);
'version 45' at -2;
put triangle {
opaque exterior;
}
}
.IE
.IS
main {
conn (0,2) to (-1,2);
'version 46' at -2;
put triangle {
opaque interior;
}
}
.IE
.IS
main {
conn (0,2) to (-1,2);
'version 47' at -2;
put triangle {
opaque exterior;
}
}
.IE
