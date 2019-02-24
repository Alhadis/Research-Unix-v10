arcs
.nf
.IS
...radians
...libfile a
main{
a1:put arc{start = 6; midway = (5,-1); end = 4;};
a2:put arc{start = (1,-3); midang = 3.14159/2; end = (0,-4);im(center) = -3;};
a3:put arc{center = (4.5,-3); radius = 1.2; startang = 2; endang = 3;};
a4:put arc{center = (3,-6); start = (4,-5); endang = 0; midang = 3.14159;};
put b1: arc {center = 0; radius = 0.25; startang = 1; midang = 0.5; endang = 0;};
put b2: arc {center = 0; radius = 0.5; startang = 1; midang = 0; endang = 0.5;};
put b3: arc {center = 0; radius = 0.75; startang = 0.5; midang = 1; endang = 0;};
put b4: arc {center = 0; radius = 1; startang = 0; midang = 1; endang = 0.5;};
put b5: arc {center = 0; radius = 1.25; startang = 0.5; midang = 0; endang = 1;};
put b6: arc {center = 0; radius = 1.5; startang = 0; midang = 0.5; endang = 1;};
}
.IE
