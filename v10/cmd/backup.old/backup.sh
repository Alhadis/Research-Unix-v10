#!/bin/sh
PATH=/bin:/usr/bin
BACKUPLIB=${BACKUPLIB-/usr/lib/backup}
usage(){
	echo 'usage: backup grep|recover|fetch|wfetch|stats|backup ...'
	exit 1
}
if test $# = 0
then
	usage
fi
x=$1
shift
case $x in
backup)	case "$@" in
	'-?')	echo 'Usage: backup backup [file ...]'
		exit 1 ;;
	?*)	for i in "$@"
		do
			echo "$i"
		done | $BACKUPLIB/absolute | $BACKUPLIB/act x ;;
	*)	$BACKUPLIB/absolute | $BACKUPLIB/act x ;;
	esac
	echo "remember, things can still go awry until your files appear in backup grep"
	;;
recover|fetch|wfetch|stats)	exec $BACKUPLIB/$x "$@" ;;
grep)	exec $BACKUPLIB/b$x "$@" ;;
*)	echo 1>&2 backup: unknown command $x; usage;;
esac
