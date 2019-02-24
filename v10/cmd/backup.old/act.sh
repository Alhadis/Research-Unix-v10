#!/bin/sh
trap 'echo "act exiting because some command failed"' 0
set -e
if [ -S ]
then
	x=/usr/adm/bkp.`date | awk ' { print $1 }'`
else
	x=/tmp/pbkp$$
fi
m=`sed -n 2p /etc/backup`
tmp=/tmp/bkp$$
me=`cat /etc/whoami`
/usr/lib/backup/iprint | /usr/lib/backup/remnotdone $me $1 > $x
set +e
/usr/lib/backup/bpush $me $1 < $x > $x.sho 2>&1
if test -s $x.sho
then
	case "$1" in
	?*)	cat $x.sho; rm $x.sho;;
	*)	( echo Backup Report for $me!$x; cat $x.sho ) | mail backup;;
	esac
else
	rm $x.sho
fi
trap '' 0
