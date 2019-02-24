print2621:	print2621.c
		cc -O -s print2621.c -o print2621

paper:		paper.sh
		cp paper.sh paper

clean:
		rm -f print2621

install:	print2621 paper
		mv paper print2621 /usr/lib/print2621
