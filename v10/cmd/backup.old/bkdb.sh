#!/bin/sh
trap 'rm -f $tmp; echo "bkdb exiting because some command failed"' 0
set -e
FILENAMES=${FILENAMES:-/usr/backup/filenames}
y=`sed 1q /etc/backup`
x=$y/bkp/`date | tr -s ' ' ' ' | cut -d' ' -f2,3,6 | tr ' ' -`
tmp=/tmp/bkbkp$$
w=`sed -n -e '2s:.*/:/n/:p' /etc/backup`/etc/passwd
backup grep -V $w > $tmp.1
rm -f $y/bkp/*.[FTN]
> $tmp
z=`/usr/backup/bin/sweep -n`
echo `date` starting
if /usr/backup/bin/block $tmp
then
	/usr/lib/backup/blog "squash $z"
	echo `date` $z > $x.N
	mv $y/.db.T $x.T; cp /dev/null $y/db.T; ln -s $x.T $y/.db.T
	mv $y/.db.F $x.F; cp /dev/null $y/db.F; ln -s $x.F $y/.db.F
	chmod 666 $y/db.[TF]
	#set +e; chown daemon,sys $y/db.[TF]; set -e
	chmod 444 $x.[TFN]
	cd $y/bkp
	time cbt squash -o$y/db $x
	rm $y/.db.T; mv $y/db.T $y/.db.T
	rm $y/.db.F; mv $y/db.F $y/.db.F
	rm $tmp		# unlock, the next action may take a long time
	eval "${PROCPERM:-sync} 2>&1 " | mail ${WHO:-backup}
fi
rm -f $tmp
echo `date` report
cbt report $y/.db
backup grep -V $w > $tmp.2
set +e
diff $tmp.[12]
rm $tmp.[12]
if [ -w $FILENAMES ]
then
	echo `date` doing $FILENAMES
	nice cbt cat $y/.db | gre -v // | sed -n -e '/\//s/	.*//p' > $FILENAMES
fi
echo `date` done
trap '' 0
