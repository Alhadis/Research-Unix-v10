CFLAGS=-c -D$(ARCH) -DM32 -I. -I$(INC)
ARCH=AR32W
MAXPC=\"20\"
INC= ../inc
OBJECTS= optim.o inter.o local.o \
		optutil.o peep.o w1opt.o w2opt.o w3opt.o func.o strtol.o

optim:		$(OBJECTS)
		$(CC) -o optim $(OBJECTS)

$(OBJECTS):	defs optim.h
local.o:	$(INC)/sgs.h $(INC)/paths.h
w1opt.o w2opt.o w3opt.o func.o:		optutil.h
func.o:
		$(CC) $(CFLAGS) -DMAXPC=$(MAXPC) -c func.c

install:	optim
		cp optim /usr/jerq/lib/m32/optim
		strip /usr/jerq/lib/m32/optim
clean:
		-rm -f optim $(OBJECTS) core
