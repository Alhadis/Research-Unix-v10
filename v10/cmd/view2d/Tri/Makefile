FFLAGS=-g
CFLAGS=-g
tri: tri.o box.o
	cc -o tri tri.o box.o ../sys.o -lview2d

cp:
	cp tri.c box.f tri /usr2/ehg/q/Mos
clean:
	rm -f *.o a.out core tmp t
