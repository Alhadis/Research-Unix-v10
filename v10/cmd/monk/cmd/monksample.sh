#!/bin/sh

#
# remove any possibly exported variables
#
unset echo preface

#
# initalizations
#
case `uuname -l` in

alux*)
	MHOME=`sed -n "/^frodo:/s/.*:\(.*\):.*/\1/p" /etc/passwd`/lib/monk
	echo='echo'
	;;

cinnamon|coqui|coyote|dolphin|golem|lynx|marmot|polecat|python)
	MHOME=/usr/local/lib/monk
	echo='echo'
	;;

sneezy)
	MHOME=/usr/llib/monk
	echo='echo'
	;;

*)
	if [ "$TOOLS" ]
	then
		MHOME=$TOOLS/lib/monk
		echo=echo
	else
		echo='echo -e'
	fi
        ;;
esac

#
# path names for samples
#
MHOME=${MHOME-/usr/lib/monk}

#
# check the number of args
#
if [ $# != 1 -o ! -r $MHOME/sample/$1 ]
then
	cd $MHOME/sample
	$echo "Usage: monksample \c" 1>&2
	preface="[ "
	for i in *
	do
		$echo "$preface$i\c" 1>&2
		preface=" | "
	done

	echo " ]" 1>&2
	exit 1
fi

#
# produce the samples
#
cat $MHOME/sample/$1
