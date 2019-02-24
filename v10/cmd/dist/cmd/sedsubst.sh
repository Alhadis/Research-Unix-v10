#! /bin/sh

PATH=%LDIR%:/bin:/usr/bin
export PATH

usage='usage: sedsubst [-Dold=new...]'

subs=

case $# in 0) exec cat;; esac

set -- `getopt D: $*` || { echo "$usage" 1>&2; exit 1; }
for arg
do
	case $arg in
	-D) subs="$subs -D$2"; shift 2;;
	--) shift; break;;
	esac
done

# this bit of line noise converts the subs into a sed command
subs=`echo $subs | sed -e 's/-D\([^ =]*\)=\([^ =]*\)/-e s=^\1=\2= -e t/g' -e 's/\\\\/\\\\\\\\/g'`
sed $subs
