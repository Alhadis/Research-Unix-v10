all:	man.out mm.out

man.out:	proof.1
	troff -man proof.1 > man.out

mm.out:	proof.mm
	tbl proof.mm | troff -mm > mm.out
