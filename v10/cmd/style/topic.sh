# NOTICE-NOT TO BE DISCLOSED OUTSIDE BELL SYS EXCEPT UNDER WRITTEN AGRMT
L=/usr/lib/style
mflag=-ms
mlflag=-ml
for i in $*
	do case $i in
		-mm) mflag=-mm;shift;continue;;
		-ms) mflag=-ms;shift;continue;;
		-ml|-li) mlflag=-ml;shift;continue;;
		+li) mlflag=;shift;continue;;
		-ver) echo $0 version 2.0: 2.0;exit;;
		-flags) echo $0 \[-flags\] \[-ver\] \[file ...\];exit;;
		-*) echo unknown topic flag $i;exit;;
		*) if test ! -r $i
		   then echo Can\'t find the file $i\; try specifying a more complete pathname.; exit
		   fi
	esac
done
echo $*
deroff $mflag $mlflag $* | $L/style1 | $L/style2  |$L/style3 -T | sort|uniq -c|sort -nr|sed -n "1,20p" 
