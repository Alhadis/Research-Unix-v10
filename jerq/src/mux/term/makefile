JERQ=/usr/jerq
JINCL = $(JERQ)/include
CC = 3cc
CFLAGS = -gJDMUXTERM -I. -I$(PDIR) -DBlit
AS = 3as

JHEADERS = $(JINCL)/layer.h $(JINCL)/jerq.h $(JINCL)/jerqproc.h

PDIR = ../proto
PHEADERS = $(PDIR)/packets.h $(PDIR)/proto.h $(PDIR)/pstats.h $(PDIR)/pconfig.h
PFILES = $(PDIR)/pcheck.c $(PDIR)/pinit.c \
	 $(PDIR)/precv.c $(PDIR)/psend.c $(PDIR)/ptimeout.c
POBJS = pcheck.o pinit.o precv.o psend.o ptimeout.o

TFILES = boot.c comm.c control.c demux.c exec.c task.c trap.c user.c window.c
TOBJS =  boot.o comm.o control.o demux.o exec.o task.o trap.o user.o window.o

OBJS = $(TOBJS) $(POBJS) menuhit.o
muxterm: map pcbs.o pl.o $(OBJS) frame/frame.a proctab.o /usr/jerq/lib/libj.a /usr/jerq/lib/liblayer.a
	3ld -o muxterm map pl.o -L$(JERQ)/lib \
		$(OBJS) frame/frame.a \
		-llayer -lj -lsys -lj -lc proctab.o
		3nm muxterm > nm.out
		@egrep Sys nm.out
		@echo should be 71E700

print:
	@pr /usr/jerq/include/mux.h $(JHEADERS) $(JINCL)/jioctl.h $(JINCL)/queue.h ../msgs.h \
		Pl.template $(TFILES)

pl.o:	pl.s $(JERQ)/include/mux.h procsize.s
	sed -n '/Ken/,$$'"s/[^']*'\\(.*\\)'.*/	.word	\\1/p" $(JERQ)/include/mux.h > x1
	(echo '	.set	CONTROL_STATE, '`sed -n '/.*proctab+\([0-9][0-9]*\).*/s//\1/p' procsize.s`; \
	sed -n '2,/# start/p' pl.s; \
	echo "	.word	0	# this is really P == Sys[0]"; \
	echo "	.word	defont"; \
	cat x1; \
	sed -n '/# end/,$$p' pl.s) > x.s
	mv pl.s opl.s
	mv x.s pl.s
	$(AS) -o pl.o pl.s
	

procsize.s:	procsize.c $(JINCL)/jerqproc.h
	$(CC) -S procsize.c

#
#	make sure stacks are in .bss instead of .comm
#
pcbs.o:
	$(CC) ${CFLAGS} -S pcbs.c
	mv pcbs.s xpcbs.s
	sed -f :sed.comm-bss xpcbs.s > pcbs.s
	$(AS) -o pcbs.o pcbs.s

pp:
	pp -kjerq -tmuxterm $(JINCL)/mux.h $(JHEADERS) $(JINCL)/jioctl.h \
		$(JINCL)/queue.h ../msgs.h pl.s pcbs.c $(TFILES) | dcan

lint:
	lint -I/usr/jerq/include $(CFLAGS) -Dmc68000 -Uvax -Dvoid=int $(TFILES) $(PFILES) -lj -llayer

boot.o:		$(JHEADERS)
comm.o:		$(JHEADERS) $(JINCL)/queue.h
control.o:	$(JHEADERS) $(JINCL)/queue.h $(PHEADERS) ../msgs.h
demux.o:	$(JHEADERS) $(JINCL)/tty.h $(JINCL)/jioctl.h \
		$(JINCL)/queue.h $(PHEADERS) ../msgs.h
menuhit.o:	$(JHEADERS) $(JINCL)/font.h
		$(CC) $(CFLAGS) -c /src/jerq/src/lib/j/menuhit.c
task.o:		$(JHEADERS)
trap.o:		$(JHEADERS)
user.o:		$(JHEADERS)
window.o:	$(JHEADERS) $(JINCL)/tty.h $(JINCL)/queue.h frame/frame.h

pcheck.o:	$(PDIR)/pcheck.c
		$(CC) $(CFLAGS) -c $(PDIR)/pcheck.c
pinit.o:	$(PDIR)/pinit.c $(PHEADERS)
		$(CC) $(CFLAGS) -c $(PDIR)/pinit.c
precv.o:	$(PDIR)/precv.c $(PHEADERS)
		$(CC) $(CFLAGS) -c $(PDIR)/precv.c
psend.o:	$(PDIR)/psend.c $(PHEADERS)
		$(CC) $(CFLAGS) -c $(PDIR)/psend.c
ptimeout.o:	$(PDIR)/ptimeout.c $(PHEADERS)
		$(CC) $(CFLAGS) -c $(PDIR)/ptimeout.c

install:	muxterm
	cprs muxterm $(JERQ)/lib/muxterm

clean:
	rm -f *.o *.hup* procsize.s core nohup.out
