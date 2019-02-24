all:
	(cd libnetb; make)
	(cd serv; make)

install:	all
	mv /v/lib/netb/zarf /v/lib/netb/ozarf && cp serv/zarf /v/lib/netb
