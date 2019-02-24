OBJ=alloc.o arc.o bquote.o builtins.o dir.o expand.o gen.o glob.o graph.o job.o lex.o main.o makefile.o match.o mk.o parse.o quote.o recipe.o regexp.o rule.o run.o shprint.o sig.o symtab.o t_ar.o t_driver.o t_file.o var.o word.o
# first define the system type from V9, BSD42, SEQUENT, SYSV
SYSTEM=SYSV
# second add any special C flags (and -DCRAY and -g if on the cray)
CFLAGS=-I. -D$(SYSTEM)
# third, if you don't have a ranlib, set RANLIB=:
RANLIB=:	# ranlib on other systems
# now, to make everything just type make

mk:	nlib $(OBJ)
	$(CC) $(CFLAGS) -o mk $(OBJ) fio/fio.a libc/libc.a math/math.a
nlib:
	cd math; make CFLAGS="$(CFLAGS)" CC=$(CC) RANLIB=$(RANLIB)
	cd libc; make CFLAGS="$(CFLAGS)" CC=$(CC) RANLIB=$(RANLIB)
	cd fio; make CFLAGS="$(CFLAGS)" CC=$(CC) RANLIB=$(RANLIB)

clean:
	rm -f *.o mk nmk */*.[oa]
alloc.o arc.o bquote.o builtins.o dir.o expand.o gen.o glob.o graph.o job.o lex.o main.o makefile.o match.o mk.o parse.o quote.o recipe.o regexp.o rule.o run.o shprint.o sig.o symtab.o t_ar.o t_driver.o t_file.o var.o word.o:	 fio.h libc.h mk.h ndir.h regexp.h
