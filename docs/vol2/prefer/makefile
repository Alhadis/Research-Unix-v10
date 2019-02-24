MLIB=/usr/lib/monk

all:	prefer.out.i mycite examples paper

prefer.out.i: prefer.out
	pinvert prefer.out

examples:
	cd example; make all

mycite: 
	$LIB/cite -s .cite >mycite; rm .cite

paper:
#	/usr/mad/bin/mymonk -x -t -Rprefer.out paper.m
	monk -x -Sproof -t -Rprefer.out paper.m

clean:
	rm -f prefer.out.[hi] mycite .cite
	cd example; make clean

