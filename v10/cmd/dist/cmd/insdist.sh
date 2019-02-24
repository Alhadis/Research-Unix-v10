#! /bin/sh

PATH=%LDIR%:/bin:/usr/bin
export PATH

usage='usage: insdist [-Dold=new...] [-Rrootdir]'

backup=%SDIR%/backup.$$		# no huge temporaries in /tmp
contents=/tmp/contents.$$
cmd=/tmp/cmd.$$
cmdfile=/tmp/cmdfile.$$
newlist=/tmp/newlist.$$
oldlist=/tmp/oldlist.$$
remove=/tmp/remove.$$
rootdir=
subst="-Dcontents=$contents -Dcmd=$cmd -Dcmdfile=$cmdfile"

rm -f $backup $contents $cmd $cmdfile $newlist $oldlist $remove

set -- `qargs getopt D:R: "$@"` || { echo "$usage" 1>&2; exit 1; }
while :
do
	case $1 in
	-D) subst="$subst -D$2"; shift 2;;
	-R) root=$2; shift 2;;
	--) shift; break;;
	esac
done

if [ "$root/" != "/" ]
then
	subst=`echo $subst | sed "s -D\(/[^ =]*\)=/*\([^ =]*\) -D\1=$root/\2 g"`
	subst="$subst -D/=$root/"
fi

uargs tarf $subst | {
	tarx -f
	files=`cat $contents | sedsubst $subst`

	realfiles=
	for f in $files
	do
		if uargs test -d $f -o -f $f
		then
			realfiles="$realfiles $f"
		fi
	done
	uargs tarc $realfiles | tee $backup | tarf -qt | sort > $oldlist

	if tarx -qt > $newlist
	then
		sort -o $newlist $newlist
		comm -23 $oldlist $newlist > $remove
		test -s $remove && xargs uargs rm -fr < $remove
		test -f $cmd && uargs sh -c `cat $cmd`
		test -f $cmdfile && uargs sh -c `sedsubst $subst < $cmdfile`
	else
		sort -o $newlist $newlist
		tarx < $backup
		comm -13 $oldlist $newlist > $remove
		test -s $remove && xargs uargs rm -fr < $remove
	fi
}

rm -f $backup $contents $cmd $cmdfile $newlist $oldlist $remove
