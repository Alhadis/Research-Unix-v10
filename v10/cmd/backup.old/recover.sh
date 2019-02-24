#!/bin/sh
tmp=/tmp/bkp$$
trap "rm -f $tmp" 0 1
opts=-v
ropts=-v
map=x
toget=
LIB=/usr/lib/backup
x(){
	if [ "$map" ]
	then
		t=`backup grep -d $1`
		if [ "" = "$t" ]
		then
			echo "couldn't find $1"
			return 0
		fi
		t=`backup grep -d $t`
	else
		t=$1
	fi
	toget="$toget $t"
	return 1
}
usage()
{
	echo "Usage: backup recover [-m] [-Dold=new] [-F] [-d] [-fdev] [-o dir] files ..." 2>&1
	exit 1
}
if test $# = 0
then
	usage
fi
export opts
dest=no
me=/n/`cat /etc/whoami`
for i in $*
do
	case $dest in
	yes)	opts="$opts $i";
		ropts="$ropts $i";
		dest=no;
		continue;;
	esac
	case $i in
	-m)	map=; continue;;
	-o)	opts="$opts $i"; ropts="$ropts $i"; dest=yes; continue;;
	-d|-D*|-F|-f*)	opts="$opts $i"; ropts="$ropts $i"; continue ;;
	-*)	usage
	esac
	if x $i
	then
		aa=`echo "$i" | $LIB/absolute`
		if x $aa
		then
			y=/n/`cat /etc/whoami`$aa
			echo trying $y
			x $y
		fi
	fi
done
if [ -z "$toget" ]
then
	echo "backup recover: no files to recover"
	exit 1
fi
if backup fetch $opts $toget 2> $tmp
then
	:
else
	case `sed -n 3p /etc/backup` in
	*!*|worm)	egrep -v 'No such file or directory' < $tmp
		echo looking on optical disk
		sed -n -e '/No such file or directory/s/:.*//p' < $tmp | $LIB/wfetch $ropts
		;;
	*)	cat $tmp;;
	esac
fi
