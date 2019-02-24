B=/usr/lib/style
echo " " $*
sflag=
eflag=
Pflag=-P
lflag=
lcon=
rflag=
rcon=
mflag=-ms
mlflag=
for i in $*
do case $i in
-mm) mflag=-mm;shift;continue;;
-ms) mflag=-ms;shift;continue;;
-ml) mlflag=-ml;shift;continue;;
-P) Pflag=-P;shift;continue;;
*) rest=$*;break;;
esac
done
deroff $mflag $mlflag $*|$B/style1|$B/style2|$B/style3 $rflag $rcon $lflag $lcon $sflag $nflag $eflag $Pflag
