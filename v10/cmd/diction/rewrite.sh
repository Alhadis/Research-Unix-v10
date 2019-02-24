B=/usr/lib/style
echo $HOME $* `date` >>/usr/llc/collect/catch 2>/dev/null
mflag=-ms
for i
do case $i in
-mm) mflag=-mm; shift; continue;;
-ms) mflag=-ms; shift; continue;;
*)deroff $mflag -p $*|$B/style1|$B/style2|$B/style3 -P -C|$B/rewrt|$B/dprog -c|nroff
esac
done
