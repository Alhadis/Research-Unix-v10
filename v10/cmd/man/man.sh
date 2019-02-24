#!/bin/sh
set -p
PATH=/bin:/usr/bin
SOURCE=/usr/man
CACHE=/usr/spool/man
cmd=-q sec='?*' secn= c=x s=x
opt= slow= quick= T=

(cd $CACHE 2>/dev/null) && c=c
(cd $SOURCE 2>/dev/null) && s=s

while	case $1 in
	[1-9]|10)	sec=$1 secn=$1 ;;
	-f)	c=f s=f ;;
	-[qw])	cmd=$1 ;;
	-[nt])	cmd=$1 ;;
	-T*)	T=$1 ;;
	-*)	opt="$opt $1" ;;
	*)	break
	esac
do	shift
done

csc=$c$s$cmd
case $csc in
ff-q)	      cmd=-t ;;
xx-?|?x-[nt]) echo man data not available 1>&2; exit ;;	
c?-[qw])      cd $CACHE ;;
?s-?)	      cd $SOURCE
esac

for i
do	case $csc in
	ff-?)	slow="$slow $i" ;;
	*)	fil=`echo man$sec/$i.*`
		for j in $fil
		do	case $csc$j in
			cs-[qw]*\*)	man -n $secn $i ;;
			*\*)	echo man: $i not found 1>&2 ;;
			cx-[qw]*) quick="$quick $j" ;;
			cs-[qw]*) if newer $SOURCE/$j $j
				then	slow="$slow $SOURCE/$j"
				else	quick="$quick $j"
				fi ;;
			*)	slow="$slow $j"
			esac
		done
	esac
done

case "$slow$quick" in
"")	exit
esac

#slightly faster than the honest way: doctype
typeset() { tbl $slow | ${1}eqn $T | ${1}roff -man $T $opt }
teqn() { eqn $* }

case $cmd in
-q)	if test -t
	then	undln="| ul"
	else	undln=
	fi
	eval '(	case "$quick" in
		?*)	cat $quick
		esac
		case "$slow" in
		?*)	typeset n
		esac)' $undln  ;;
-t)	typeset t ;;
-n)	typeset n ;;
-w)	echo $quick $slow
esac
