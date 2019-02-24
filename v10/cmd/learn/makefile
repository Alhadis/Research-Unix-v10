LESSONS = files editor morefiles macros eqn C

FILES = lrndef lrnref \
	copy.c dounit.c learn.c list.c \
	makpipe.c maktee.c mem.c mysys.c selsub.c selunit.c \
	start.c whatnow.c wrapup.c \
	lcount.c tee.c makefile

OBJECTS = copy.o dounit.o learn.o list.o mem.o \
	makpipe.o maktee.o mysys.o selsub.o selunit.o \
	start.o whatnow.o wrapup.o

CFLAGS = -O
LIBRARY =
LLIB	= /usr/lib/learn

a.out: $(OBJECTS)
	cc -n $(CFLAGS) $(OBJECTS) $(LIBRARY)

$(OBJECTS): lrnref
learn.o: lrndef

list:
	pr $(FILES) ../README

bin:
	cp a.out /usr/bin/learn
	strip /usr/bin/learn

lcount tee:
	cc $(CFLAGS) -s $@.c -o $(LLIB)/$@ $(LIBRARY)

lessons:	$(LESSONS)

play log:
	-rm -r $(LLIB)/$@; mkdir $(LLIB)/$@; chmod +w $(LLIB)/$@

$(LESSONS):
	-rm -r $(LLIB)/$@
	mkdir $(LLIB)/$@
	(cd $(LLIB)/$@; ar x ../$@.a)

all:	a.out bin tee lcount lessons play log check

archives:
	(cd $(LLIB)/files; ar r ../files.a L*)
	(cd $(LLIB)/morefiles; ar r ../morefiles.a L*)
	(cd $(LLIB)/editor; ar r ../editor.a L*)
	(cd $(LLIB)/macros; ar r ../macros.a L*)
	(cd $(LLIB)/eqn; ar r ../eqn.a L* tinyms Init)
	(cd $(LLIB)/C; ar r ../C.a L* get*)

export:
	: make the directories
	mkdir export export/source export/lib
	: copy source files
	-cp $(FILES) export/source
	-cp ../README export
	: non-directories in lib
	-for i in $(LLIB)/*;\
	do if test -f $$i; then	cp $$i export/lib; fi;\
	done

check:
	-@test -r $(LLIB)/tee || echo 'tee not present; make tee'
	-@test -r $(LLIB)/lcount || echo 'lcount not present; make lcount'
	-@test -r $(LLIB)/play || echo 'play directory not present; make play'
	-@test -r $(LLIB)/log || echo 'log directory not present; make log'
	-@for i in $(LESSONS); do test -r $(LLIB)/$$i/L0 || echo $$i not unarchived, make $$i; done

clean:
	rm *.o a.out
