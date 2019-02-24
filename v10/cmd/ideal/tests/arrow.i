arrow
.nf
.IS
...radians
box arrow{var start, end, headvec, head;
headvec = end + head*(start - end)/abs(start - end);
conn start to end;
conn end to end + cis(3.14159/8)*(headvec - end);
conn end to end + cis(-3.14159/8)*(headvec - end);
}

box main{
put arrow{head = 0.25;start = 0; end = (1,-1);};
put arrow{head = 0.5; start = (0,-2); end = (1,-1.2);};
put arrow{head = 0.2; start = (2,-2); end = (0.1,-2.3);};
}
.IE
.IS
...forget arrow
.IE
