CC=3cc
CFLAGS=

menudrop: menudrop.m
	@echo exec 32ld `pwd`'/$@.m "$$@"' >$@
	@chmod +x $@

MENUDROP=menudrop.o menudemo.o cursor.o lalloc.o cleanup.o ident.o
menudrop.m: & $(MENUDROP)
	$(CC) $(CFLAGS) -o $@ $(MENUDROP)

menudemo.o: MENUHIT.h

MENUHIT.h: /usr/jerq/include/mux.h
	sed -n '/menuhit/s/.*(\([0-9][0-9]*\).*/#define MENUHIT	\1/p' \
		/usr/jerq/include/mux.h >$@

install: menudrop
	cp menudrop.m /usr/jerq/mbin/menudrop.m
	3strip -r /usr/jerq/mbin/menudrop.m
	-chmod 664 /usr/jerq/mbin/menudrop.m
	@echo exec 32ld /usr/jerq/mbin/menudrop.m "$$@" >/usr/jerq/bin/menudrop
	-chmod 775 /usr/jerq/bin/menudrop
	touch install

ship: install
	ship /usr/jerq/bin/menudrop /usr/jerq/mbin/menudrop.m
	touch ship

clean:
	rm -f *.o core menudrop menudrop.m install ship
