BIN=/usr/v9/bin

sysdaemon:	sundaemon.c
	cc sundaemon.c -o sysdaemon

install:	$(BIN)/sysdaemon

$(BIN)/sysdaemon:	sysdaemon
	cp sysdaemon $(BIN)/sysdaemon
	chgrp kmem $(BIN)/sysdaemon
	chmod 2755 $(BIN)/sysdaemon
