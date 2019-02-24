#! /bin/sh
PATH=/bin:/usr/bin
# V, X flags, F files, C commands for auxiliary list and history
# D_SPELL dictionary, H_SPELL history, P_SPELL program
# V_SPELL -v file, A_SPELL auxiliary list

F= V= X= C="sort -u +0f +0"
SP_HOME=${SP_HOME-/usr/dict}
P_SPELL=${P_SPELL-${SP_HOME}/spellprog}
DEROFF="deroff -w"
for A
do
	case $A in
	-v)	V_SPELL=${V_SPELL-/tmp/spell$$} 
		trap "rm -f $V_SPELL" 0 1 2 13 15
		V="-v" ;;
	-b) 	D_SPELL=${D_SPELL-${SP_HOME}/brspell}
		X="$X -b" ;;
	-x)	X="$X -x" ;;
	-d)	DEROFF="cat" ;;
	-ns)	C=cat ;;
	*.tex)	case "$DEROFF" in
		"deroff -w") DEROFF=delatex
		esac
		F="$F $A" ;;
	*)	F="$F $A"
	esac
done

if test -w ${H_SPELL=${SP_HOME}/spellhist}
then C="$C | tee -a $H_SPELL"
fi
if test -r ${A_SPELL=$HOME/lib/spelldict}
then C="$C | sort - $A_SPELL $A_SPELL | uniq -u | sort -f"
fi	#kluge until this works:  gre -F -x -v -f $A_SPELL||exit 0

$DEROFF $F |
 $P_SPELL ${D_SPELL-${SP_HOME}/amspell}  ${V_SPELL=/dev/null} $X $V | 
eval $C 

case $V in
*-v*)	sort -u +1f +1 $V_SPELL
esac
