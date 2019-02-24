#! /bin/sh

LDIR=%LDIR%
SDIR=%SDIR%

PATH=$LDIR:/bin:/usr/bin
export PATH

umask 022

# improve this
usage="usage: dist [options] files..."

distcmd=
dest=
insopts=
mkopts=
nonotify=
subst=
verbose=

set -- `qargs getopt qrnvd:D:f:F:R:x:X: "$@"` || { echo "$usage" 1>&2; exit 1; }
while :
do
	case $1 in
	-q) distcmd=q; shift;;
	-r) distcmd=r; shift;;
	-n) nonotify=n; shift;;
	-v) verbose=-v; shift;;
	-d) dest="$dest `uargs echo $2`"; shift 2;;
	-D) subst="$subst -D$2"; shift 2;;
	-f) dest="$dest `cat $2`"; shift 2;;
	-F) dest="$dest `cat $LDIR/destinations/$2`"; shift 2;;
	-R) insopts="$insopts -R$2"; shift 2;;
	-x) mkopts="$mkopts -x$2"; shift 2;;
	-X) mkopts="$mkopts -X$2"; shift 2;;
	--) shift; break;;
	esac
done

case "$distcmd" in
q)
	case $# in
	0) showq;;
	*) for arg in $*; do uargs connect $arg showq i /bin/cat; done;;
	esac
	exit $?
	;;
r)
	case $# in
	0) echo "$usage" 1>&2; exit 1;;
	*) sys=$1; shift;;
	esac
	uargs connect $sys transmit io $LDIR/receive $verbose $insopts $subst $*
	exit $?
	;;
esac

test -n "$dest" || dest=`cat $LDIR/destinations/default 2>/dev/null` || {
	echo "dist: $LDIR/destinations/default not found" 1>&2
	exit 1
}

# make spool directory
for n in `seq -w 0 999`
do
	dir=Q.$$.$n
	test -d $dir -o -f $dir || {
		trap 'rm -fr $SDIR/$dir' 1 2 3
		mkdir $SDIR/$dir && break
		echo "dist: can't make spool directory" 1>&2
		exit 1
	}
done

case "$verbose" in -v) echo "queue job $dir:" 1>&2;; esac

data=$SDIR/$dir/data
ctl=$SDIR/$dir/tempctl
files=$SDIR/$dir/files

# use canonical system names.
cansys $dest | awk '{print ++n, $1}' > $ctl

uargs mkdist $verbose $mkopts $subst $* > $data

cwd=`pwd`
for f
do
	case $f in
	/*)
		echo $f
		;;
	*)
		echo $cwd/$f
		;;
	esac
done | sort | sedsubst $subst > $files

mv $ctl $SDIR/$dir/ctl

# supersede old jobs containing the same files
cd $SDIR
for d in Q.*
do
	case $d in
	$dir)
		continue
		;;
	*)
		test -w $d || continue
		if [ `comm -23 $d/files $dir/files | wc -c` = 0 ]
		then
			sort +1 $d/ctl $dir/ctl | uniq -d -1 |
			while read num sys
			do
				echo superseded by $dir > $d/$num.done
			done
		fi
		;;
	esac
done
cleanq

case $nonotify in n) exit $?;; esac

# notify destination systems in the background
exec < /dev/null >/dev/null 2>&1 3>&-
for d in $dest
do
	connect $d notice io $LDIR/notify $dir
done &
