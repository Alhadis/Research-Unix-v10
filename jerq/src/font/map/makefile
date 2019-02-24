all:	spec smap imap dump  mtest

mtest:	mtest.o map.o
	cc -o  mtest mtest.o map.o

spec:	spec.o map.o
	cc -o  spec spec.o map.o
