CFLAGS =
DEST = /usr/bin/server

server: server.o
	$(CC) -o server server.o -ldk

install: server
	cp server $(DEST)
	strip $(DEST)
	/etc/chown daemon,sys $(DEST)
	chmod a=rx,u+ws $(DEST)

clean:
	rm -f server *.o
