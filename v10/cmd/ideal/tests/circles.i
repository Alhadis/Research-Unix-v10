circles
.nf
.IS
...radians
...libfile c
main{
c1:put circle{radius = 0.5; center = (4,-2);};
c2:put circle{center = 3; z1 = 2;};
c3:put circle{center = 4; z2 = (3,1);};
c4:put circle{center = 2; z3 = (1.4,1.6);};
c5:put circle{z1 = 2.4; z2 = 1.7; z3 = (2.3,2.3);};
c6:put circle{z1 = (2,3); z2 = (4,6); re(center) = 3;};
c7:put circle{z1 = (3,2); z3 = (6,4); im(center) = 3;};
c8:put circle{z2 = (1,-1); z3 = (-1,1); im(center) = 0;};
}
.IE
