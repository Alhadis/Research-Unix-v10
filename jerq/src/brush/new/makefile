MYJERQ	= myjerq.h
BRUSH   = .
SOURCES = brush.c circles.c copy.c draw.c editext.c errmsg.c help.c icons.c \
	  newfill.c readmap.c writemap.c options.c smooth.c sweeps.c text.c \
	  texture32.c type.c updown.c shrink.c details.c
GINCS   = $(MYJERQ) brush.h
OBJS    = brush.o circles.o copy.o draw.o editext.o errmsg.o help.o icons.o \
	  newfill.o readmap.o writemap.o options.o smooth.o sweeps.o text.o \
	  texture32.o type.o updown.o shrink.o details.o
LIBS    = -lc
CC	= $(DMD)/bin/dmdcc
STRIP	= $(DMD)/bin/m32strip
CFLAGS	= -DJ5620 -DBITFILE -I$(BRUSH) 

brush.56:$(OBJS)
	@ls -l brush.56
	$(CC) -o brush.56 $(OBJS) $(LIBS)
	@$(STRIP) -r brush.56
	@ls -l brush.56
	@echo ""

test.56:newtest.o errmsg.o
	$(CC) -o newtest.56 newtest.o errmsg.o $(LIBS)
	@echo ""

freehand.56:freehand.o errmsg.o
	$(CC) -o freehand.56 freehand.o errmsg.o $(LIBS)
	@echo ""

$(OBJS): $(GINCS)

readmap.o:	makefile
writemap.o:	makefile

print:
	@print $(SOURCES) $(GINCS) makefile56
	@echo ""

lf:
	@lf $(SOURCES) $(GINCS) makefile56
	@echo ""

bundle:
	bundle $(SOURCES) $(GINCS) makefile > brush.bundle
	@echo ""

cpio:
	echo $(SOURCES) $(GINCS) makefile | 1per | cpio -ocv > brush.cpio
	@echo ""

safe:
	@cp *.[ch] makefile56 brush.56 backup
	@echo "`date`" > backup/last_time
	@echo "`date`"
	@echo ""

