INC=/usr/jerq/include
CFLAGS= -g -I$(INC)
CC=3cc
libs= -ljj

all: graphdraw.m graphdraw.h 

Tsrcs=Tmain.c vertex.c edge.c move_vertex.c copy_vertex.c list_ops.c cmds.c\
	file_ops.c scale.c box.c stop.c gtypes.c gt_ops.c vert_ops.c edge_ops.c\
	labsmenu.c labels.c showlabels.c euc.c edgesum.c graphics.c graphicsmenu.c\
	macrosmenu.c mac_ops.c set_ops.c setscale_ops.c scaleset.c set_fcns.c\
	reshapeset.c reshaperect.c\
	Avert_ops.c Aedge_ops.c Ascale.c Animcmds.c

Tobjs=Tmain.o vertex.o edge.o move_vertex.o copy_vertex.o list_ops.o cmds.o\
	file_ops.o scale.o box.o stop.o gtypes.o gt_ops.o vert_ops.o edge_ops.o\
	labsmenu.o labels.o showlabels.o euc.o edgesum.o graphics.o graphicsmenu.o\
	macrosmenu.o mac_ops.o set_ops.o setscale_ops.o scaleset.o set_fcns.o\
	reshapeset.o reshaperect.o\
	Avert_ops.o  Aedge_ops.o Ascale.o Animcmds.o grid.o


Hsrcs=Hmain.c Hreadcmd.c


graphdraw.m:	$(Tobjs)
	$(CC) $(CFLAGS) $(Tobjs) $(libs) -o graphdraw.m


graphdraw.h: $(Hsrcs)	
	cc  $(Hsrcs) -o graphdraw.h


$(Tobjs): defs.h 

graphpic: gplib.o graphpic.o
	cc gplib.o graphpic.o -o graphpic

graphpic.o: graphtypes.h graphpic.c gplib.c
	cc -c graphpic.c

gplib.o: graphtypes.h gplib.c
	cc -c gplib.c

clean:
	rm -f *.o graphdraw.m graphdraw.h graphpic graphdraw

CMD=/usr/jerq/bin/graphdraw
CMD2=/usr/jerq/bin/graphpic
HPROG=/usr/jerq/bin/graphdraw.h
TPROG=/usr/jerq/mbin/graphdraw.m

install: graphdraw graphpic graphdraw.h graphdraw.m 
	cp graphdraw	$(CMD)
	cp graphpic	$(CMD2)
	cp graphdraw.h	$(HPROG)
	cp graphdraw.m	$(TPROG)
	chmod a=rx,u+w	$(CMD)
	chmod a=rx,u+w	$(CMD2)
	chmod a=rx,u+w	$(HPROG)
	chmod a=rx,u+w	$(TPROG)
	strip		$(HPROG)
	touch install
