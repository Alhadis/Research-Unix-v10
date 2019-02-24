#!/bin/sh
dialstring=`qns value in $1`
case "$dialstring" in
"")	;;
*)	lpsend $dialstring tcp printer
esac
quux=$?
case $quux in
0) ;;
*)	echo lpsend $1 tcp fails with $quux
	dialstring=`qns value dk $1`
	case "$dialstring" in
	"")	;;
	*)	lpsend $dialstring dk printer
	esac
esac
exit $?
