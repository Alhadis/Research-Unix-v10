PATH=.:/bin:/usr/bin
export PATH
DEST=/usr/bin

# Where asdrcv goes depends on whether we're running Honey Danber
if [ -f /usr/lib/uucp/L.sys ]
then	ASDRCV=/usr/bin/asdrcv
else	ASDRCV=/etc/asd/asdrcv
fi

if [ -d /etc/asd ]
then	:
else	mkdir /etc/asd
	chmod 755 /etc/asd
	setown root /etc/asd
fi

for i in seal unseal mkpkg inspkg
do	rm -f $DEST/$i
	cp $i $DEST/$i
	strip $DEST/$i
	setown bin $DEST/$i
	chmod 775 $DEST/$i
done

rm -f $ASDRCV
cp asdrcv $ASDRCV
strip $ASDRCV
setown root $ASDRCV
chmod 755 $ASDRCV
chmod og-w,u+s $ASDRCV

rm -f $DEST/uuship
cp ship.sh $DEST/uuship
