#!/bin/sh
check(){
	if [ ! -f $sentinel ]
	then
		echo "munge: $sentinel removed"
		> $finished
		exit 1
	fi
}
x=`sed 1q /etc/backup`/rcvdirs
tmp=/tmp/$$
sentinel=/tmp/munging
finished=/tmp/mungedone
if [ $# != 0 ]
then
	xx=echo
else
	xx=:
fi

case `basename $0` in
stopmunge)
	if [ -f $sentinel ]
	then
		rm $sentinel
		$xx "removed sentinel; waiting for munge to finish"
		while [ ! -f $finished ]
		do
			sleep 5
			$xx "waiting..."
		done
		rm $finished
		$xx "munge stopped"
	fi
	exit 0;;
esac

if [ -f $sentinel ]
then
	echo "munge: $sentinel exists; exiting" 1>&2
	exit 1;
fi
trap "rm -f $sentinel; > $finished" 0
> $sentinel
rm -f $finished
for i in `cat $x`
do
	/usr/lib/backup/blog "munge: `df $i | sed -n '2s:\([^ ]*\) *\([^ ]*\) *\([^ ]*\) *\([^ ]*\) *\([^ ]*\) *\([^ ]*\):\1 \4/\3 \6:p'`"
	cd $i
	for j in rcv*
	do
		cd $i/$j
		$xx $i/$j
		ls | /usr/backup/bin/sweep
		check
		ls | /usr/backup/bin/copies > $tmp
		check
		for d in `sed 's:.*	\(.*\)/.*:\1:' $tmp | sort -u`
		do
			if [ ! -d $d ]
			then
				$xx mkdir $d 2>&1
				mkdir $d
			fi
		done
		sed 's:^:/usr/backup/bin/bcp -r :' $tmp | sh -e
		check
		cut -f2 $tmp | /usr/backup/bin/fileupd | /usr/backup/bin/dbupdate
		check
	done
done
rm -f $tmp
/usr/lib/backup/stats -c
$PROCPERM
