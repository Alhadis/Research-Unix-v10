#!/bin/sh

spelldir=/usr/lib/spell

dict=${D_SPELL:-$spelldir/amspell}
hist=${H_SPELL:-$spelldir/spellhist}
addenda=${A_SPELL:-$HOME/lib/spelldict}
sprog=${P_SPELL:-$spelldir/sprog}
deroff=${deroff-"deroff -w"}

spellflags= deroffargs=

for x in $*
do
	case $x in
	-b)
		dict=${D_SPELL:-$spelldir/brspell}
		spellflags="$spellflags $x" ;;
	-[Ccvx])
		spellflags="$spellflags $x" ;;
	-d)
		deroff=cat ;;
	*.tex)
		deroff=delatex
		deroffargs="$deroffargs $x" ;;
	*)
		deroffargs="$deroffargs $x" 
	esac
done

case "$spellflags" in
*c*)
	$sprog -c -f $dict ;;
*C*)
	$sprog -C -f $dict 
	exit
esac

if test -r $addenda
then coda="sort - $addenda $addenda | uniq -u | sort -f"
else coda="sort -f"
fi	#kluge until this works:  gre -F -x -v -f $addenda||exit 0

if test -w $hist
then coda="$coda | tee -a $hist"	# puts -v trash in spellhist
fi

$deroff $deroffargs | $sprog -f $dict $spellflags | sort -u | eval $coda
