HDR = 
SRC = term.c mouse.c host.c spline.c track.c
OBJ = term.o mouse.o host.o spline.o track.o

all: jterm.m

jterm.m: $(OBJ)
	3cc -g -o jterm.m $(OBJ) 

commands.h: 
	cp /usr/src/libj/commands.h .

term.o:  term.c commands.h
	3cc -g -c term.c 

mouse.o: mouse.c 
	3cc -g -c mouse.c

host.o: host.c
	3cc -g -c host.c
	
spline.o: spline.c
	3cc -g -c spline.c
	
track.o: track.c
	3cc -g -c track.c
	
clean:
	rm jterm.m $(OBJ)

print:
	pr $(HDR) $(SRC) | can

install: jterm.m
	cprs jterm.m /usr/jerq/lib/jterm.m
