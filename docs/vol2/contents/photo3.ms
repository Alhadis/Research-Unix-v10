.nr LL 6.5i
.nr LT 6.5i
.de XX
.nr %# \\$2
.ds %e .tl |\\$3||\\f(NI\\$1\\fP|
.ds %o .tl |\\f(NI\\$1\\fP||\\$3|
.ds %E .tl |\\\\n(PN||UNIX Papers|
.ds %O .tl |Research Tenth Edition||\\\\n(PN|
..
.ds cr \h'3p'\s+3\fS\N'191'\fP\s-3
.br
.pn 1
.XX \& \& Pico-Portrait
.de X1
.ds ZX \\$3
.ds ZY \\$2
.BP
...KS
.nr %# \\$1
.af PN 1
.so photo1.pic.o
...KE
.nr %# \\$1
..
.de Y1
.ds ZX \\$3
.ds ZY \\$2
.ds ZZ \\$4
.BP
...KS
.nr %# \\$1
.af PN i
.so pho11.pic.o
...KE
.nr %# \\$1
..
.de X2
.ds ZX \\$4
.ds ZY \\$3
.ds ZZ \\$2
.BP
...KS
.nr %# \\$1
.af PN i
.so photo2.pic.o
...KE
.nr %# \\$1
.af PN 1
..
.TL
photo pages (here's $4 wasted for a header page...)
.LP
.X2 2 "Dennis Ritchie" "Ken Thompson" "new = old[x_cart(r=16*sqrt(r_polar(x,y)),a=a_polar(x,y)), y_cart(r,a)]"
.Y1 6 "Andrew Hume" "new = $andrew[x_cart((r-(sin(a))/300),a), y_cart(r,a)]" "with r=r_polar(x,y) and a=a_polar(x,y)"
.X1 602 "Paul Glick" "new = $pg[X_cart(r=r_polar(x,y), a=(y*x)/4+A_polar(x,y)), Y_cart(r,a)]"
.BP
done
