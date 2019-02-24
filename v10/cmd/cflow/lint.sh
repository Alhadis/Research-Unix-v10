L=/usr/lib/lint/lint T=/usr/tmp/lint.$$ PATH=/bin:/usr/bin O="-C -Dlint"
X= P=unix LL=/usr/lib/lint
S=/usr/tmp/lints$$
trap "rm -f $S $T; exit" 1 2 15
for A in $*
do
	case $A in
	-n)	P= ;;
	-p)	P=port ;;
	*.ln)	cat $A >>$T ;;
	-l*)	cat $LL/llib$A.ln >>$T ;;
	-[IDOU]*)	O="$O $A" ;;
	-*)	X="$X$A" ;;
	*)	(/lib/cpp $O $A | ${L}1 $X -S$S >>$T) 2>&1
	esac
	done
case $P in
	unix)	cat $LL/llib-lc.ln >>$T ;;
	port)	cat $LL/llib-port.ln >>$T ;;
	esac
${L}2 $T $X -S$S
rm -f $S $T
