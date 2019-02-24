#!/bin/sh
mach=`sed -n 2p /etc/backup`
b=`sed 1q /etc/backup`
VOL=`sed -n -e '$s/	.*//p' $b/volidmap`	# set below
WHO=${WHO:-backup}
tmp=/tmp/toworm$$
cd $b/v
x=`echo v*`
case $# in
1)	if [ $1 -ne 0 ]
	then	# only numeric args
		x=`ls -d $x | sed ${1}q`
		x=`echo $x`
	fi
	echo=echo
	;;
*)	echo=:
	;;
esac
DEV=0
do1(){
	DEV=`worm mount $VOL`
	worm stat -f$DEV -F30000 $VOL
	case $? in
	0)
		(ls $1 | sed "s:^:$1/:"; echo $1) > $tmp
		/usr/lib/backup/blog "toworm $1: starting"
		worm write -f$DEV $VOL < $tmp > $tmp.1 && xargs rm -f < $tmp
		$echo "toworm $1: `cat $tmp.1`"
		/usr/lib/backup/blog "toworm $1: `cat $tmp.1` done"
		;;
	1)
		(echo "please mount $VOL in $mach's worm and as root, run"
		echo "	/usr/backup/bin/toworm") | mail $WHO
		rm /tmp/worming
		exit 1
		;;
	2)
		exit 1
		;;
	3)
		/usr/backup/bin/bumpvolmap $1
		VOL=`sed -n -e '$s/	.*//p' $b/volidmap`	# set above
		rm /tmp/worming
		exec /usr/backup/bin/toworm
		;;
	esac
}
if [ -f /tmp/worming ]
then
	if [ -f /proc/`cat /tmp/worming` ]
	then
		echo "some one else is worming! pid =" `cat /tmp/worming`
		exit 1
	fi
fi
echo $$ > /tmp/worming
case $x in
'v*')	;;
*)	/usr/lib/backup/blog "toworm: $x"
	for i in `ls -d $x | sort +0.1n`
	do
		do1 $i
	done
	;;
esac
worm tmpdir -f$DEV $VOL
rm -f /tmp/worming
$echo "`worm stat -f$DEV`"
rm -f $tmp $tmp.1
