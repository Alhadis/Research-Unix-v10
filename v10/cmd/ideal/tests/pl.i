.IS
triangle {
var a, b, c;
a = (-1,0);
b = (1,0);
c = (0,1);
conn a to b to c to a;
bdlist = a, b, c;
}
.IE

.IS
main {
conn (-2,0) to (2,0);
put triangle {
opaque interior;
}
'version 0' at (0,-4);
}
.IE
.IS
main {
conn (-2,0) to (2,0);
put triangle {
opaque exterior;
}
'version 1' at (0,-4);
}
.IE
.IS
main {
conn (2,0) to (-2,0);
put triangle {
opaque interior;
}
'version 2' at (0,-4);
}
.IE
.IS
main {
conn (2,0) to (-2,0);
put triangle {
opaque exterior;
}
'version 3' at (0,-4);
}
.IE
.IS
main {
conn (-2,0) to (1,0);
put triangle {
opaque interior;
}
'version 4' at (0,-4);
}
.IE
.IS
main {
conn (-2,0) to (1,0);
put triangle {
opaque exterior;
}
'version 5' at (0,-4);
}
.IE
.IS
main {
conn (1,0) to (-2,0);
put triangle {
opaque interior;
}
'version 6' at (0,-4);
}
.IE
.IS
main {
conn (1,0) to (-2,0);
put triangle {
opaque exterior;
}
'version 7' at (0,-4);
}
.IE
.IS
main {
conn (-1,0) to (1,0);
put triangle {
opaque interior;
}
'version 8' at (0,-4);
}
.IE
.IS
main {
conn (-1,0) to (1,0);
put triangle {
opaque exterior;
}
'version 9' at (0,-4);
}
.IE
.IS
main {
conn (1,0) to (-1,0);
put triangle {
opaque interior;
}
'version 10' at (0,-4);
}
.IE
.IS
main {
conn (1,0) to (-1,0);
put triangle {
opaque exterior;
}
'version 11' at (0,-4);
}
.IE
.IS
main {
conn (0,2) to (0,-1);
put triangle {
opaque interior;
}
'version 12' at (0,-4);
}
.IE
.IS
main {
conn (0,2) to (0,-1);
put triangle {
opaque exterior;
}
'version 13' at (0,-4);
}
.IE
.IS
main {
conn (0,-1) to (0,2);
put triangle {
opaque interior;
}
'version 14' at (0,-4);
}
.IE
.IS
main {
conn (0,-1) to (0,2);
put triangle {
opaque exterior;
}
'version 15' at (0,-4);
}
.IE
.IS
main {
conn (0,2) to (0,0);
put triangle {
opaque interior;
}
'version 16' at (0,-4);
}
.IE
.IS
main {
conn (0,2) to (0,0);
put triangle {
opaque exterior;
}
'version 17' at (0,-4);
}
.IE
.IS
main {
conn (0,0) to (0,2);
put triangle {
opaque interior;
}
'version 18' at (0,-4);
}
.IE
.IS
main {
conn (0,0) to (0,2);
put triangle {
opaque exterior;
}
'version 19' at (0,-4);
}
.IE
.IS
main {
conn (0,1) to (0,0);
put triangle {
opaque interior;
}
'version 20' at (0,-4);
}
.IE
.IS
main {
conn (0,1) to (0,0);
put triangle {
opaque exterior;
}
'version 21' at (0,-4);
}
.IE
.IS
main {
conn (0,0) to (0,1);
put triangle {
opaque interior;
}
'version 22' at (0,-4);
}
.IE
.IS
main {
conn (0,0) to (0,1);
put triangle {
opaque exterior;
}
'version 23' at (0,-4);
}
.IE
.IS
main {
conn (0,1) to (0,-1);
put triangle {
opaque interior;
}
'version 24' at (0,-4);
}
.IE
.IS
main {
conn (0,1) to (0,-1);
put triangle {
opaque exterior;
}
'version 25' at (0,-4);
}
.IE
.IS
main {
conn (0,-1) to (0,1);
put triangle {
opaque interior;
}
'version 26' at (0,-4);
}
.IE
.IS
main {
conn (0,-1) to (0,1);
put triangle {
opaque exterior;
}
'version 27' at (0,-4);
}
.IE
.IS
main {
conn (-1,0.5) to (1,0.5);
put triangle {
opaque interior;
}
'version 28' at (0,-4);
}
.IE
.IS
main {
conn (-1,0.5) to (1,0.5);
put triangle {
opaque exterior;
}
'version 29' at (0,-4);
}
.IE
.IS
main {
conn (1,0.5) to (-1,0.5);
put triangle {
opaque interior;
}
'version 30' at (0,-4);
}
.IE
.IS
main {
conn (1,0.5) to (-1,0.5);
put triangle {
opaque exterior;
}
'version 31' at (0,-4);
}
.IE
.IS
main {
conn (-1,0.5) to (0.5,0.5);
put triangle {
opaque interior;
}
'version 32' at (0,-4);
}
.IE
.IS
main {
conn (-1,0.5) to (0.5,0.5);
put triangle {
opaque exterior;
}
'version 33' at (0,-4);
}
.IE
.IS
main {
conn (0.5,0.5) to (-1,0.5);
put triangle {
opaque interior;
}
'version 34' at (0,-4);
}
.IE
.IS
main {
conn (0.5,0.5) to (-1,0.5);
put triangle {
opaque exterior;
}
'version 35' at (0,-4);
}
.IE
.IS
main {
conn (-0.5,0.5) to (0.5,0.5);
put triangle {
opaque interior;
}
'version 36' at (0,-4);
}
.IE
.IS
main {
conn (-0.5,0.5) to (0.5,0.5);
put triangle {
opaque exterior;
}
'version 37' at (0,-4);
}
.IE
.IS
main {
conn (0.5,0.5) to (-0.5,0.5);
put triangle {
opaque interior;
}
'version 38' at (0,-4);
}
.IE
.IS
main {
conn (0.5,0.5) to (-0.5,0.5);
put triangle {
opaque exterior;
}
'version 39' at (0,-4);
}
.IE
.IS
main {
conn (3,-1) to (-3,-1);
put triangle {
opaque interior;
}
'version 40' at (0,-4);
}
.IE
.IS
main {
conn (3,-1) to (-3,-1);
put triangle {
opaque exterior;
}
'version 41' at (0,-4);
}
.IE
.IS
main {
conn (-3,-1) to (3,-1);
put triangle {
opaque interior;
}
'version 42' at (0,-4);
}
.IE
.IS
main {
conn (-3,-1) to (3,-1);
put triangle {
opaque exterior;
}
'version 43' at (0,-4);
}
.IE
.IS
main {
conn (-0.25,0.25) to (0.25,0.25);
put triangle {
opaque interior;
}
'version 44' at (0,-4);
}
.IE
.IS
main {
conn (-0.25,0.25) to (0.25,0.25);
put triangle {
opaque exterior;
}
'version 45' at (0,-4);
}
.IE
.IS
main {
conn (0.25,0.25) to (-0.25,0.25);
put triangle {
opaque interior;
}
'version 46' at (0,-4);
}
.IE
.IS
main {
conn (0.25,0.25) to (-0.25,0.25);
put triangle {
opaque exterior;
}
'version 47' at (0,-4);
}
.IE
