#! /bin/sh
echo "<mkconf.$1" > mkfile

cat << 'EOF' >> mkfile
DIRS=ape tar $SYS

%-compile:V: paths.h
	cd $stem; mk compile

%-install:V: %-compile
	cd $stem; mk install

%-clean:V:
	cd $stem; mk clean

compile:V: ${DIRS:%=%-compile}

install:V: ${DIRS:%=%-install}

clean:V: ${DIRS:%=%-clean}
	rm -f paths.h

ape-compile:Pexit 1: $SYS-compile

paths.h: mkconf.$SYS
	echo '#define LDIR "'$LDIR'"' > paths.h
	echo '#define SDIR "'$SDIR'"' >> paths.h
EOF
