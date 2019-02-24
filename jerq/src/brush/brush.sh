
#!/bin/sh

while test "$1" != ""
do
	case $1 in
	-f)	FONTS=$2
		shift 2
		;;
	-p)	PICS=$2
		shift 2
		;;
	-t)	TEXT=$2
		shift 2
		;;
	*)	shift
		;;
	esac
done

BRUSH=/usr/jerq/mbin
FONTS=${FONTS:-/usr/jerq/font}
PICS=${PICS:-.}
TEXT=${TEXT:-.}

jx $BRUSH/brush.m $FONTS $PICS/ $TEXT/
