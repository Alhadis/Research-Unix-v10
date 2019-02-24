BIN=/etc

sysdaemon:	bsddaemon.c
	cc -DBSD bsddaemon.c -o sysdaemon

install:	$(BIN)/sysdaemon

$(BIN)/sysdaemon:	sysdaemon
	cp sysdaemon $(BIN)/sysdaemon
	chgrp kmem $(BIN)/sysdaemon
	chmod 2755 $(BIN)/sysdaemon
