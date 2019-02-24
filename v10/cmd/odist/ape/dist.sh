#! /bin/sh -

umask 022

LDIR=%LDIR%
SDIR=%SDIR%
QPREFIX=Q.

PATH=$LDIR:/bin:/usr/bin:/usr/ucb:/usr/bsd
export PATH

DEST=$LDIR/destinations

showq=0
recv=0
notify=0
systems=
usage='usage: dist [-q|-r] [system|-[Ff] file] [-n] [files...]'

while [ $# -gt 0 ]
do
	case "$1" in
	-q)
		showq=1
		;;
	-r)
		recv=1
		;;
	-n)
		notify=1
		;;
	-[Ff])
		if [ $# -lt 2 ]
		then
			echo "$usage" 1>&2
			exit 1
		fi
		if [ x$1 = x-f -a -r "$2" ]
		then
			systems=`cat "$2" 2>/dev/null`
		elif [ -r $DEST/"$2" ]
		then
			systems=`cat $DEST/"$2" 2>/dev/null`
		else
			echo "dist: can't open '$2'" 1>&2
			exit 1
		fi
		shift
		;;
	-*)
		echo "dist: unknown flag '$1'" 1>&2
		exit 1
		;;
	*)
		if [ "x$systems" != x ]
		then
			break
		fi
		systems="$1"
		;;
	esac
	shift
done

if [ $recv = 1 ]
then
	if [ x"$systems" = x ]
	then
		echo dist: no systems specified 1>&2
		exit 1
	else
		for sys in $systems
		do
			echo $sys:
			connect $sys transmit io receive || exit 1
		done
		exit 0
	fi
fi

if [ $showq = 1 ]
then
	if [ x"$systems" = x ]
	then
		showq
		exit 0
	else
		for sys in $systems
		do
			echo $sys:
			connect $sys showq i /bin/cat
		done
		exit 0
	fi
fi

if [ $# = 0 ]
then
	echo "$usage" 1>&2
	exit 1
fi

# construct spool directory
for d in `echo $$ | awk '{for (i = 0; i < 1000; ++i) printf "%03d%d\n", i, $1}'`
do
	if [ ! -f $SDIR/$QPREFIX$d ]
	then
		dir=$QPREFIX$d
		mkdir $SDIR/$dir && break
		echo "dist: can't make spool directory" 1>&2
		exit 1
	fi
done

data=$SDIR/$dir/data
ctl=$SDIR/$dir/tempctl
files=$SDIR/$dir/files

# use canonical system names.
canon $systems | awk '{for (i = 1; i <= NF; ++i) printf "%d %s\n", ++n, $i}' > $ctl

mkpkg "$@" > $data

pwd=`pwd`
for f in "$@"
do
	case "$f" in
	-*)	# flag to mkpkg
		;;
	/*)
		echo "$f"
		;;
	*)
		echo "$pwd/$f"
		;;
	esac
done | sort > $files

# rename the ctl file to its real name; at this point the package
# is now available for shipment.
mv $ctl $SDIR/$dir/ctl

# supersede old jobs containing the same files
cd $SDIR
for d in $QPREFIX*
do
	test $d = $dir && continue
	test -w $d || continue
	if [ `comm -23 $d/files $dir/files | wc -c` = 0 ]
	then
		sort +1 $d/ctl $dir/ctl | uniq -d -2 |
		while read num sys
		do
			echo superseded by $dir > $d/$num.done
		done
	fi
done

# clean up any jobs we completely superseded.
cleanq
