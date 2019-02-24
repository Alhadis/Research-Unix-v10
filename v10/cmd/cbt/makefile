LCBT=-lcbt

ALL = btadd btbuild btcat btcreat btdelete btlook btran btreport btsquash

all:    $(ALL)

btadd:	btadd.c
	cc $(CFLAGS) btadd.c $(LCBT) -o btadd
btbuild:	btbuild.c
	cc $(CFLAGS) btbuild.c $(LCBT) -o btbuild
btcat:	btcat.c
	cc $(CFLAGS) btcat.c $(LCBT) -o btcat
btcreat:	btcreat.c
	cc $(CFLAGS) btcreat.c $(LCBT) -o btcreat
btdelete:	btdelete.c
	cc $(CFLAGS) btdelete.c $(LCBT) -o btdelete
btlook:	btlook.c
	cc $(CFLAGS) btlook.c $(LCBT) -o btlook
btran:	btran.c
	cc $(CFLAGS) btran.c $(LCBT) -o btran
btreport:	btreport.c 
	cc $(CFLAGS) btreport.c $(LCBT) -o btreport
btsquash:	btsquash.c
	cc $(CFLAGS) btsquash.c $(LCBT) -o btsquash

install:
	strip $(ALL)
	test -d /usr/lib/btree || mkdir /usr/lib/btree
	mv $(ALL) /usr/lib/btree
	cp cbt /usr/bin

clean:
	rm -f $(ALL) nohup.out *.o *.T *.F
