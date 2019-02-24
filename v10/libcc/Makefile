OBJ=balloc.t getbitmap.t inbitmap.t outbitmap.t

ARCHIVE=libcc.a

%.t: %.c
	$(CC) $(CFLAGS) -c $%.c && touch $%.t

$(ARCHIVE):&	$(OBJ)
	ar r $(ARCHIVE) *.o
	ranlib $(ARCHIVE)
	rm *.o

install: $(ARCHIVE)
	cp $(ARCHIVE) /usr/lib
	touch install

ship: install
	ship /usr/lib/$(ARCHIVE)
	touch ship

Make:
	sed	-e "/^OBJ=.*/{s/.*/OBJ=`echo *.c`/" \
		-e 's/\.c/.t/g' \
		-e '}' Makefile >makefile
	mv makefile Makefile
