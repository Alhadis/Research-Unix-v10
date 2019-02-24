BIN=/usr/v9

sysdaemon:	mipsdaemon.c
	cc mipsdaemon.c -o sysdaemon -lmld

install:	$(BIN)/sysdaemon

$(BIN)/sysdaemon:	sysdaemon
	cp sysdaemon $(BIN)/sysdaemon
	chgrp sys $(BIN)/sysdaemon
	chmod 2755 $(BIN)/sysdaemon
