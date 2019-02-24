#
# Trivial script for checking and setting the default PostScript font
# encoding. Changing the default assumes you can write in $POSTLIB.
# Available font encodings are files in $POSTLIB that end in .enc.
# The default is $POSTLIB/Default.enc.
#

POSTLIB=/usr/lib/postscript
DEFAULT=Default.enc
CURRENTDEFAULT=

case "$1" in
    Default) ;;
 
    Standard)
	rm -f $POSTLIB/$DEFAULT;;

    "") cd $POSTLIB
	for i in *.enc; do
	    if [ -f "$i" -a "$i" != $DEFAULT ]; then
		NAME=`echo $i | sed s/\\.enc//`
		if cmp $i $DEFAULT >/dev/null 2>/dev/null; then
		    CURRENTDEFAULT=$NAME
		fi
		echo $NAME
	    fi
	done
	echo Standard
	echo "Default=${CURRENTDEFAULT:-Standard}";;

    *)  if [ -f "$POSTLIB/$1.enc" ]
	    then rm -f $POSTLIB/$DEFAULT
		 ln $POSTLIB/$1.enc $POSTLIB/$DEFAULT
	    else echo "unrecognized encoding name $1" >&2
	fi;;
esac

