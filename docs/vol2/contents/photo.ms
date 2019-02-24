.so ../ADM/mac
.XX \& \& Pico-Portrait
.de X1
.nr %# \\$1
.ds ZX \\$3
.ds ZY \\$2
.BP
.KS
.so photo1.pic.o
.KE
.BP
filler
..
.de Y1
.nr %# \\$1
.ds ZX \\$3
.ds ZY \\$2
.ds ZZ \\$4
.BP
.KS
.so pho11.pic.o
.KE
.BP
filler
..
.de X2
.nr %# \\$1
.ds ZX \\$4
.ds ZY \\$3
.ds ZZ \\$2
.BP
.KS
.so photo2.pic.o
.KE
.BP
filler
..
.de Y2
.nr %# \\$1
.ds ZX \\$4
.ds ZY \\$3
.ds ZZ \\$2
.ds YZ \\$5
.BP
.KS
.so pho22.pic.o
.KE
.BP
filler
..
.TL
photo pages - part 1
.LP
.X2 4 "Dennis Ritchie" "Ken Thompson" "new = old[x_cart(r=16*sqrt(r_polar(x,y)),a=a_polar(x,y)), y_cart(r,a)]"
.X1 52 "Bart Locanthi" "new = $bart[sqrt(x*512),y+sin(x)/(x+50)]"
.X2 78 "Judy Paone" "Kam Montgomery" "new = old[x_cart(r=16*sqrt(r_polar(x,y)),a=a_polar(x,y)), y_cart(r,a)]"
.X1 108 "Doug McIlroy" "new = $doug[(x*x)/X+95, (y*y)/Y+25]"
.X1 222 "Brian Kernighan" "new = $bwk[a_polar(x,y)*5/2, r_polar(x,y)*5/2]"
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
