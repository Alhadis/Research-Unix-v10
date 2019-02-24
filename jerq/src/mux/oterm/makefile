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

OBJS = $(TOBJS) $(POBJS)
.SUFFIXES:	.k
.c.k:
	-mv $*.o $*.oo
	$(CC) $(CFLAGS) -DBLITKBD -c $<
	mv $*.o $*.k
	-mv $*.oo $*.o
.c.o:
	$(CC) $(CFLAGS) -c $<
.s.o:
	$(CC) -c $<

muxterm: map pcbs.o pl.o $(OBJS) rom.o frame/frame.a proctab.o
	3ld -o muxterm map pl.o -L$(JERQ)/lib rom.o\
		$(OBJS) frame/frame.a \
		-llayer -lj -lsys -lj -lc proctab.o
		3nm muxterm > nm.out
		@egrep Sys nm.out
		@echo should be 71E700

2term: map pcbs.o pl.o $(OBJS) rom.o frame/frame.a proctab.o
	sed -e '/allocinit.*256/s/256/750/' -e '/NPROC=16/s/16/4/' control.c > x.c
	mv control.o 2co; $(CC) $(CFLAGS) -c x.c; mv x.o control.o
	3ld -o 2term map pl.o -L$(JERQ)/lib rom.o\
		$(OBJS) frame/frame.a \
		-llayer -lj -lsys -lj -lc proctab.o
	mv 2co control.o

luxterm: map pcbs.o pl.o $(OBJS) rom.o frame/frame.a proctab.o
	sed '/allocinit.*256/s/256/512/' control.c > x.c
	$(CC) $(CFLAGS) -c x.c
	mv x.o control.o
	3ld -o luxterm map pl.o -L$(JERQ)/lib rom.o\
		$(OBJS) frame/frame.a \
		-llayer -lj -lsys -lj -lc proctab.o
	rm control.o

aplterm: map pcbs.o pl.o $(OBJS) rom.o frame/frame.a proctab.o afont.o key.o
	sed -e '/defont/s/^/#/' < rom.s > x.s; 3cc -c x.s; mv x.o rom.o
	sed -e '/aplterm1/s/(c.=0x7F)/c/'\
	-e '/aplterm2/s/^/\/*/'\
	-e '/aplterm3/s/c=ac/if((c=mapchar(ac))==255)return/'  \
	-e 's/==SCROLLKEY/==254/' window.c > x.c
	3cc $(CFLAGS) -c x.c; mv x.o window.o
	3ld -o aplterm map pl.o -L$(JERQ)/lib rom.o\
		$(OBJS) frame/frame.a key.o afont.o \
		-llayer -lj -lsys -lj -lc proctab.o
	rm rom.o window.o

ddsterm: pcbs.o pl.o $(OBJS) dds.o frame/frame.a
	3ld -o ddsterm map pl.o tiny.o -L$(JERQ)/lib dds.o\
		$(OBJS) frame/frame.a \
		-llayer -lj -lsys -lj -lc
		3nm muxterm > nm.out
		@egrep Sys nm.out
		@echo should be 71E700

bkmuxterm: pcbs.o pl.o boot.o comm.k control.o demux.o exec.o task.o trap.o user.o window.k $(PBOJS) rom.o kbd.k frame/frame.a
	3ld -o a.out map pl.o -L$(JERQ)/lib rom.o kbd.k\
		boot.o comm.k control.o demux.o exec.o task.o trap.o user.o window.k $(POBJS) frame/frame.a \
		-llayer -lj -lsys -lj -lc -o bk
		3nm a.out > nm.out
		@egrep Sys nm.out
		@echo should be 71C9E0


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
	rm -f *.o *.hup* procsize.s core
