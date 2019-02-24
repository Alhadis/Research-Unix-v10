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
.de Y2
.ds ZX \\$4
.ds ZY \\$3
.ds ZZ \\$2
.ds YZ \\$5
.BP
...KS
.nr %# \\$1
.so pho22.pic.o
...KE
..
.TL
photo pages - part 1
.LP
.X2 6 "Dennis Ritchie" "Ken Thompson" "new = old[x_cart(r=16*sqrt(r_polar(x,y)),a=a_polar(x,y)), y_cart(r,a)]"
.X1 4 "Judy Paone" "new = old[x_cart(r=16*sqrt(r_polar(x,y)),a=a_polar(x,y)), y_cart(r,a)]"
.X1 52 "Brian Kernighan" "new = $bwk[a_polar(x,y)*5/2, r_polar(x,y)*5/2]"
.X1 78 "Kam Montgomery" "new = old[x_cart(r=16*sqrt(r_polar(x,y)),a=a_polar(x,y)), y_cart(r,a)]"
.X1 108 "Bart Locanthi" "new = $bart[sqrt(x*512),y+sin(x)/(x+50)]"
.X1 222 "Doug McIlroy" "new = $doug[(x*x)/X+95, (y*y)/Y+25]"
.Y1 282 "Howard Trickey" "new = old[x_cart((r-(sin(a))/300),a), y_cart(r,a)]" "with r=r_polar(x,y) and a=a_polar(x,y)"
.Y1 310 "Peter Weinberger" "new = (x<L)?$pjw:(x>R)?$melt:3*((x-L)*$melt+(R-x)*$pjw)/X" "with L=X/3 and R= 2*X/3"
.Y2 324 "Ed Sitar" "Andrew Hume" "new = (y<T)?$ejs:(y>B)?$andrew:6*((y-T)*$andrew+(B-y)*$ejs)/Y" "with T=Y/6 and B= Y/3"
.X1 344 "Bill Marshall" "Seurat Transformation (program by Rodian Paul, 1988)"
.X1 388 "Lorinda Cherry" "new = ((x+sin((4*y))/1000)%31<rand()%31)?$llc:$llc[X-x,y]"
.X1 398 "Rob Pike" "new = $rob[x+(X*cos(((x-X)*360)*2/X))/6, y]"
.X1 422 "Gerard Holzmann" "new[x,y+121-(6*r_polar(x,y))/9] = $gerard"
.X1 476 "Ken Thompson" "Picture Map (produced by a pico function)"
.X1 482 "Tom Duff" "Manual Pico-Edit (including new = (x<Y/2)?$td:$td[x,Y-y])"
.X1 494 "Jim McKie" "new = $jmk[r=r_polar(x,y), a_polar(x,y)+r/3]"
.BP
we are not done
