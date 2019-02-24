#! /bin/sh -

PATH=%LDIR%:/bin:/usr/bin
export PATH

usage='usage: mkdist [-Dold=new...] [-[xX]command] files...'

cwd=`pwd`
contents=/tmp/contents.$$
cmd=/tmp/cmd.$$
cmdfile=/tmp/cmdfile.$$
cmdstr=
cmdfilestr=
files=
subst="-D$contents=contents -D$cmd=cmd -D$cmdfile=cmdfile"

rm -f $contents $cmd $cmdfile

set -- `qargs getopt D:x:X: "$@"` || { echo "$usage" 1>&2; exit 1; }
for arg
do
	case $arg in
	-D) subst="$subst -D$2"; shift 2;;
	-x) cmdstr=$2; shift 2;;
	-X) cmdfilestr=$2; shift 2;;
	--) shift; break;;
	esac
done

cwd=`pwd`

for f
do
	case $f in
	/*) echo $f;;
	*) echo $cwd/$f;;
	esac
done | sedsubst $subst > $contents

for f
do
	case $f in
	/*) uargs test -d $f -o -f $f && files="$files $f";;
	*) uargs test -d $cwd/$f -o -f $cwd/$f && files="$files $cwd/$f";;
	esac
done

case "$cmdstr" in
?*)
	echo $cmdstr > $cmd
	files="$cmd $files"
	;;
esac

case "$cmdfilestr" in
?*)
	echo $cmdfilestr | sedsubst $subst > $cmdfile
	files="$cmdfile $files"
	;;
esac

uargs tarc $contents $files | uargs tarf $subst
rm -f $contents $cmd $cmdfile
