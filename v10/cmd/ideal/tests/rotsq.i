rotsq
.nf
.IS
box square{var ne, nw, se, sw;
ne = nw + size;
se = sw + size;
ne = se + (0,1)*size;
conn ne to se;
conn se to sw;
conn sw to nw;
conn nw to ne;
}

box main{var anchor;
anchor = (1.5,-3);
put square{var size;nw=anchor;size = (1,1);conn (nw + ne)/2 to 0.5*(sw + se);};
}
.IE
.IS
...forget square
.IE
