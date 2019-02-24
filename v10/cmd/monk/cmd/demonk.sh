#!/bin/sh
#
# demonk shell script:
#	runs demonk to remove all monk commands from input file(s)
#	then runs deroff on output to remove troff commands
#       finally runs sed to strip characters used to hold blank lines
#

#
# initalizations: protect from the definitions of others
# 		  remove any possibly exported variables
#

unset options deroffop files


#
# initalizations: where to find utilities is system dependent
#

system=`uuname -l`

case $system in

mh*)
	TOOLS=`logdir exptools`
	export TOOLS
	MHOME=${MHOME-$TOOLS/lib/monk}
	STDIN=-
	;;

alux*)
	MHOME=`sed -n "/^frodo:/s/.*:\(.*\):.*/\1/p" /etc/passwd`/lib/monk
	STDIN=-
	;;

cinnamon|coqui|coyote|dolphin|golem|lynx|marmot|polecat|python)
	MHOME=/usr/local/lib/monk
	STDIN=-
	;;

sneezy)
	MHOME=/usr/llib/monk
	STDIN=-
	;;

*)
	if [ "$TOOLS" ]
	then
		MHOME=$TOOLS/lib/monk
		STDIN=-
	else
		MHOME=${MHOME-/usr/lib/monk}
		STDIN=/dev/stdin
	fi
	;;

esac

# 
# set MDB to default monk data base directory
#
MDB=${MDB-$MHOME/db}

#
# set USAGE message
#
USAGE="usage: demonk [-biw] [-d path] [-mm] [-ms] [-ml] [file...]" 
#
# get options
#

set -- `getopt bd:im:w $*`
if [ $? != 0 ]
then
	echo "$USAGE"
	exit 1
fi

for i in $*
do
	case $i in
#
# options
#  -i causes files specified w/ |insert or |source to be ignored
#      this option is also passed to deroff
#  -b forces unnecessary blank lines to be removed
#  -d causes demonk program to use directory specifed for databases

		-b)	options="$options -b";shift ;;
		-i)	options="$options -i";
			deroffop="$deroffop -i"; shift ;; 
		-d)	MDB=$2;  shift; shift;;
#
# options for deroff
#
		-w)	deroffop="$deroffop -w";shift ;;
		-m)	case $2 in
				m|s|l)	deroffop="$deroffop -m$2"  ;;
				*)	echo "$USAGE"
			   		exit 2 ;;
		     	esac ; shift ;shift;;

		--) 	shift; break ;;
	esac
done

#
# input files
#

files="$*"

#
# do it
#
sh -c "exec $MHOME/demonk $options -d $MDB $files  \\
	| /usr/bin/deroff $deroffop \\
	| sed 's/XYZQZY$//g' "

