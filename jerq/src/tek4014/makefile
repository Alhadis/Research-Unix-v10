CC=3cc
OFILES=jek.o flash.o tiny.o bigapl.o tinyapl.o dclip.o dline.o jdlineto.o dlsegment.o defont.o jerqreshape.o

jek.m:	$(OFILES)
	3cc -o jek.m $(OFILES)

install:	jek.m
	cp jek.m /usr/jerq/mbin/tek4014.m
	3strip -r /usr/jerq/mbin/tek4014.m

clean:
	rm -f *.o core tek4014.m
