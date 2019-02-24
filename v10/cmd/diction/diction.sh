B=/usr/lib/style
C=/usr/llc/collect/catch
if test -w $C
then
	echo $HOME $* `date` >>$C 2>/dev/null
fi
echo " " $*
flag=
nflag=
mflag=-ms
lflag=-ml
file=
line=-l
for i
do case $i in
 -f) flag=-f;shift; file=$1; shift; continue;;
-n) nflag=-n;shift; continue;;
 -mm) mflag=-mm; shift; continue;;
-ms) mflag=-ms;shift;continue;;
-ml) lflag=-ml;shift;continue;;
-l) line=-l;shift;continue;;
-s) line=; shift;continue;;
-ver) echo $0 version 2.0;exit;;
-*) echo unknown diction flag $i; exit;;
*) if test $line
then $B/dprog $nflag $line $flag $file $*
 else deroff $mflag $lflag $*|$B/dprog $nflag $flag $file
fi
break;;
esac
done
