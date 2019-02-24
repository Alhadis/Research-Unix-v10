fan
.nf
.IS
fan{
var x;
var b;
conn x to y0;
conn x to y1;
conn x to y2;
}
main{
var y0,y1,y2;
y0 = (2,2);
y1 = y0 + 1;
y2 = y1 + 1;
conn 2 to 5 using 3 fan {} <x,b>;
}
.IE
.IS
...forget fan
.IE
