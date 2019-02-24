CC=cc
CFLAGS=
HOST=.
INCLUDES=-I. -I.. -I../../c
LDFLAGS=-s
SHELL=/bin/sh
OBJS=dag.o decl.o enode.o error.o expr.o init.o input.o lex.o main.o \
	output.o profio.o simp.o stmt.o string.o sym.o tree.o types.o \
	gen.o decode.o pseudos.o stab.o sel.o

rcc:		$(OBJS)
		$(CC) $(LDFLAGS) -o rcc $(OBJS)

$(OBJS):	../../c/c.h ../../c/token.h ../../c/ops.h config.h ../gen.h
lex.o:		../../c/keywords.h

gen.o:		../gen.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../gen.c
dag.o:		../../c/dag.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/dag.c
decl.o:		../../c/decl.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/decl.c
enode.o:	../../c/enode.c;	$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/enode.c
error.o:	../../c/error.c;	$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/error.c
expr.o:		../../c/expr.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/expr.c
init.o:		../../c/init.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/init.c
input.o:	../../c/input.c;	$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/input.c
lex.o:		../../c/lex.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/lex.c
main.o:		../../c/main.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/main.c
output.o:	../../c/output.c;	$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/output.c
profio.o:	../../c/profio.c;	$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/profio.c
simp.o:		../../c/simp.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/simp.c
stmt.o:		../../c/stmt.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/stmt.c
string.o:	../../c/string.c;	$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/string.c
sym.o:		../../c/sym.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/sym.c
tree.o:		../../c/tree.c;		$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/tree.c
types.o:	../../c/types.c;	$(CC) -c $(CFLAGS) $(INCLUDES) ../../c/types.c
decode.o:	$(HOST)/decode.c;	$(CC) -c $(CFLAGS) $(INCLUDES) $(HOST)/decode.c
pseudos.o:	pseudos.c;		$(CC) -c $(CFLAGS) $(INCLUDES) pseudos.c
stab.o:		stab.c ../stabstub.c ../stabbsd.c; $(CC) -c $(CFLAGS) $(INCLUDES) stab.c
sel.o:		sel.c;			$(CC) -c $(CFLAGS) $(INCLUDES) sel.c

clean:
		rm -f *.o tst/od[23] tst/*.[12so]
		-cd tst; rm -f 8q array cf cq cvt fields front incr init \
			limits paranoia sort spill stdarg struct switch wf1 yacc

test:		tst/8q.s tst/array.s tst/cf.s tst/cq.s tst/cvt.s tst/fields.s tst/front.s \
		tst/incr.s tst/init.s tst/limits.s tst/paranoia.s tst/sort.s tst/spill.s \
		tst/stdarg.s tst/struct.s tst/switch.s tst/wf1.s tst/yacc.s

tst/8q.s:	../run rcc ../../tst/8q.c       ../../tst/8q.0;		@../run 8q	$(OS)
tst/array.s:	../run rcc ../../tst/array.c    ../../tst/array.0;	@../run array	$(OS)
tst/cf.s:	../run rcc ../../tst/cf.c       ../../tst/cf.0;		@../run cf	$(OS)
tst/cq.s:	../run rcc ../../tst/cq.c       ../../tst/cq.0;		@../run cq	$(OS)
tst/cvt.s:	../run rcc ../../tst/cvt.c      ../../tst/cvt.0;	@../run cvt	$(OS)
tst/fields.s:	../run rcc ../../tst/fields.c   ../../tst/fields.0;	@../run fields	$(OS)
tst/front.s:	../run rcc ../../tst/front.c    ../../tst/front.0;	@../run front	$(OS)
tst/incr.s:	../run rcc ../../tst/incr.c     ../../tst/incr.0;	@../run incr	$(OS)
tst/init.s:	../run rcc ../../tst/init.c     ../../tst/init.0;	@../run init	$(OS)
tst/limits.s:	../run rcc ../../tst/limits.c   ../../tst/limits.0;	@../run limits	$(OS)
tst/paranoia.s:	../run rcc ../../tst/paranoia.c ../../tst/paranoia.0;	@../run paranoia	$(OS)
tst/sort.s:	../run rcc ../../tst/sort.c     ../../tst/sort.0;	@../run sort	$(OS)
tst/spill.s:	../run rcc ../../tst/spill.c    ../../tst/spill.0;	@../run spill	$(OS)
tst/stdarg.s:	../run rcc ../../tst/stdarg.c   ../../tst/stdarg.0;	@../run stdarg	$(OS)
tst/struct.s:	../run rcc ../../tst/struct.c   ../../tst/struct.0;	@../run struct	$(OS)
tst/switch.s:	../run rcc ../../tst/switch.c   ../../tst/switch.0;	@../run switch	$(OS)
tst/wf1.s:	../run rcc ../../tst/wf1.c      ../../tst/wf1.0;	@../run wf1	$(OS)
tst/yacc.s:	../run rcc ../../tst/yacc.c     ../../tst/yacc.0;	@../run yacc	$(OS)

triple:		rcc
		rm -f *.o
		make CC='lcc -B./ -d0.1 -A' CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' rcc
		-od +8 <rcc >tst/od2
		rm -f *.o
		make CC='lcc -B./ -d0.1 -A' CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' rcc
		-od +8 <rcc >tst/od3
		cmp tst/od[23] && rm tst/od[23]
