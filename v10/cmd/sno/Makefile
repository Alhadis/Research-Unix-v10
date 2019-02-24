CFLAGS = -Od2
OBJS = sno1.o sno2.o sno3.o sno4.o

sno:	$(OBJS)
	cc -o sno $(OBJS)

$(OBJS):	sno.h

install:	sno
	strip sno
	mv sno /usr/bin

clean:
	rm -f sno $(OBJS)
