L=/usr/lib/style
C=/usr/llc/collect/scatch
if test -w $C
then
echo $HOME $* `date` >>$C 2>/dev/null
fi
echo " " $*
sflag=-S
eflag=
Eflag=
Pflag=
nflag=
lflag=
lcon=
rflag=
rcon=
mflag=-ms
mlflag=-ml
for i in $*
do case $i in
-r) rflag=-r; shift; rcon=$1;shift;continue;;
-l)lflag=-l; shift; lcon=$1;shift;continue;;
-mm) mflag=-mm;shift;continue;;
-ms) mflag=-ms;shift;continue;;
-li|-ml) mlflag=-ml;shift;continue;;
+li|-tt)mlflag=;shift;continue;;
-p) sflag=-p;shift;continue;;
-a) sflag=-a;shift;continue;;
-e) eflag=-e;shift;continue;;
-E) Eflag=-E;shift;continue;;
-P) Pflag=-P;shift;continue;;
-n) nflag=-n;shift;continue;;
-N) nflag=-N;shift;continue;;
-ver) echo $0 version 2.0;  exit;;
-flags) /bin/echo $0 "[-flags] [-ver] [-r num] [-l num] [-e] [-p] [-n] [-N] [-a] [-P] [-mm|-ms] [-li|+li] [file ...]";exit;;
-*) echo unknown style flag $i; exit;;
*) break;;
esac
done
deroff $mflag $mlflag $*|$L/style1|$L/style2|$L/style3 $rflag $rcon $lflag $lcon $sflag $nflag $eflag $Pflag $Eflag
